#!/bin/bash
# machine_profile.sh -- sourceable per-machine toolchain + topology profile.
#
#   source scripts/machine_profile.sh [machine]
#
# Sets up whatever the named machine needs to build and run this repo:
# compiler/toolchain environment (modules + spack on NERSC, nothing on a
# plain workstation) and the topology values in machine.env.
#
# Machine is resolved as:  $1  >  $NUMATYPING_MACHINE  >  auto-detect.
# Known names: stormbreaker, perlmutter-cpu, generic.
#
# WHY THIS EXISTS ALONGSIDE detect_machine.sh
# -------------------------------------------
# detect_machine.sh probes the machine it is running on. That is correct and
# stays the source of truth -- but on a batch system you often *configure* on a
# login node and *run* on a compute node, and NERSC's own documentation says
# those two have different NUMA layouts (login nodes NPS=1, CPU compute nodes
# NPS=4). Probing a login node and running the result on a compute node would
# silently produce the wrong node pair and thread count.
#
# So: a profile may declare EXPECTED topology. We still probe (detect_machine.sh
# is unchanged and still does the real work), then compare. If they agree,
# nothing happens. If they disagree, the profile's pinned values win and we say
# so loudly. Profiles with no pins -- stormbreaker, generic -- are pure probe,
# byte-identical to today's behaviour.
#
# Provides after sourcing:
#   $NUMATYPING_MACHINE          resolved machine name
#   $ROOT_DIR                    repo root
#   the contents of machine.env  (NUMA_NODE_ORDER, PARTITION_THREADS, ...)
#   numatyping_verify_topology   re-check pins against the live machine
#   numatyping_profile_summary   print what was resolved

# --- locate repo root (works whether sourced or executed) -------------------
if [ -n "${BASH_SOURCE[0]:-}" ]; then
    _np_self="${BASH_SOURCE[0]}"
else
    _np_self="$0"
fi
ROOT_DIR="${ROOT_DIR:-$(cd "$(dirname "$_np_self")/.." && pwd)}"
export ROOT_DIR

# ---------------------------------------------------------------------------
# 1. Resolve machine name
# ---------------------------------------------------------------------------
_np_autodetect() {
    if [ "${NERSC_HOST:-}" = "perlmutter" ] || [ "${LMOD_SYSTEM_NAME:-}" = "perlmutter" ]; then
        echo "perlmutter-cpu"; return
    fi
    case "$(hostname -s 2>/dev/null)" in
        stormbreaker*) echo "stormbreaker"; return ;;
    esac
    echo "generic"
}

# NB: `source file` with no arguments leaves $@ as the *caller's* positional
# parameters, so $1 here can be some unrelated flag of whoever sourced us.
# Only accept it if it looks like a machine name.
_np_arg="${1:-}"
case "$_np_arg" in -*) _np_arg="" ;; esac

NUMATYPING_MACHINE="${_np_arg:-${NUMATYPING_MACHINE:-$(_np_autodetect)}}"
export NUMATYPING_MACHINE
unset _np_arg

# ---------------------------------------------------------------------------
# 2. Per-machine expected topology (empty = probe only, no pinning)
# ---------------------------------------------------------------------------
_NP_EXP_PHYS_NODES=""
_NP_EXP_CPU_NODES=""
_NP_EXP_NODE_ORDER=""
_NP_EXP_PART_THREADS=""
_NP_EXP_BIND=""

case "$NUMATYPING_MACHINE" in
    perlmutter-cpu)
        # Perlmutter CPU compute node: 2x AMD EPYC 7763, NPS=4 per socket
        # -> 8 NUMA domains x ~64 GB, 128 cores / 256 threads.
        # Verified against `sinfo` (all 3072 cpu,milan,ss11 nodes: 256 CPUs,
        # 515100 MB) and docs.nersc.gov/systems/perlmutter/architecture.
        # Requires --constraint=cpu; without it you land on a GPU node
        # (128 CPUs, 4 NUMA domains) and these pins would be wrong.
        _NP_EXP_PHYS_NODES=8
        _NP_EXP_CPU_NODES="0,1,2,3,4,5,6,7"
        _NP_EXP_NODE_ORDER="0,7"
        _NP_EXP_PART_THREADS=64
        _NP_EXP_BIND="--cpunodebind=0,7 --membind=0,7"
        ;;
    stormbreaker|generic)
        # Pure probe. detect_machine.sh already yields 0,1 / 80 on stormbreaker,
        # byte-identical to what was hardcoded before it existed.
        ;;
    *)
        echo "machine_profile.sh: unknown machine '$NUMATYPING_MACHINE'." >&2
        echo "  known: stormbreaker, perlmutter-cpu, generic" >&2
        return 1 2>/dev/null || exit 1
        ;;
esac

# ---------------------------------------------------------------------------
# 3. Toolchain environment
# ---------------------------------------------------------------------------
_np_setup_toolchain() {
    case "$NUMATYPING_MACHINE" in
    perlmutter-cpu)
        # Python FIRST: the system python3 here is 3.6, and scripts/campaign.py
        # calls subprocess.run(capture_output=True), which is 3.7+. Without this
        # every campaign dies in git_gate() with
        #     TypeError: __init__() got an unexpected keyword argument 'capture_output'
        # run.py happens to survive on 3.6 because it never calls git_status(),
        # so a working smoke test proves nothing about campaign.py.
        # NB: `module load python` (no version) does NOT exist on Perlmutter --
        # the stale root *.slurm files use it and it fails.
        module load python/"${NUMATYPING_PYTHON:-3.13-26.8.0}" >/dev/null 2>&1 \
            || module load python >/dev/null 2>&1 \
            || echo "machine_profile.sh: warning: no python module loaded" >&2

        # Pinned versions, not defaults: `PrgEnv-llvm` currently defaults to
        # 21.1.4, which happens to match stormbreaker's LLVM 21 major -- but a
        # default that moves to 22 would drag the AST-matcher APIs in
        # numa-clang-tool with it (stormbreaker.md section 4.4).
        module load PrgEnv-llvm/"${NUMATYPING_LLVM:-21.1.4}" >/dev/null 2>&1 \
            || { echo "machine_profile.sh: failed to load PrgEnv-llvm" >&2; return 1; }
        module load spack/"${NUMATYPING_SPACK:-1.1.1}" >/dev/null 2>&1 || true
        module load cmake/"${NUMATYPING_CMAKE:-3.30.2}" >/dev/null 2>&1 || true
        spack env activate "${NUMATYPING_SPACK_ENV:-NUMATyping}" >/dev/null 2>&1 || true
        ;;
    stormbreaker|generic)
        # No modules, no spack -- everything comes from the system toolchain.
        # env.py's spack lookup returns empty here and its callers already
        # handle that (stormbreaker.md section 3.3); preserve that.
        ;;
    esac

    # Shared: resolve CC/CXX/JEMALLOC_ROOT/MAX_NODE_ID via the existing env.py
    # rather than reimplementing it, so both machines keep identical semantics.
    local ev
    ev="$(python3 "$ROOT_DIR/scripts/env.py" 2>/dev/null)"
    [ -n "$ev" ] && eval "$ev"

    # campaign.py needs >= 3.7 (subprocess capture_output). Fail here, loudly,
    # rather than four hours into a batch job.
    if ! python3 -c 'import sys; sys.exit(0 if sys.version_info >= (3,7) else 1)' 2>/dev/null; then
        echo "machine_profile.sh: ERROR python3 is $(python3 --version 2>&1), campaign.py needs >= 3.7" >&2
        echo "  load a newer python module, or set NUMATYPING_PYTHON=<module version>" >&2
        return 1
    fi
}

# ---------------------------------------------------------------------------
# 4. Topology: probe, then reconcile against pins
# ---------------------------------------------------------------------------
_np_kv() {  # _np_kv FILE KEY -> value, quotes stripped
    sed -n "s/^export $2=//p" "$1" 2>/dev/null | head -n1 | sed 's/[[:space:]]*#.*$//' | tr -d '"'
}

_np_setup_topology() {
    local env_file="$ROOT_DIR/machine.env"

    bash "$ROOT_DIR/scripts/detect_machine.sh" "$ROOT_DIR" >/dev/null || return 1

    if [ -z "$_NP_EXP_NODE_ORDER" ]; then
        NUMATYPING_TOPOLOGY_SOURCE="probed"
        return 0
    fi

    # Compare probe against the profile's pins.
    local got_order got_threads got_bind got_phys mismatch=0
    got_order=$(_np_kv "$env_file" NUMA_NODE_ORDER)
    got_threads=$(_np_kv "$env_file" PARTITION_THREADS)
    got_bind=$(_np_kv "$env_file" NUMACTL_BIND)
    got_phys=$(_np_kv "$env_file" NUM_PHYS_NODES)

    [ "$got_order"   = "$_NP_EXP_NODE_ORDER"   ] || mismatch=1
    [ "$got_threads" = "$_NP_EXP_PART_THREADS" ] || mismatch=1
    [ "$got_bind"    = "$_NP_EXP_BIND"         ] || mismatch=1
    [ "$got_phys"    = "$_NP_EXP_PHYS_NODES"   ] || mismatch=1

    if [ "$mismatch" -eq 0 ]; then
        NUMATYPING_TOPOLOGY_SOURCE="probed (matches $NUMATYPING_MACHINE profile)"
        return 0
    fi

    echo "" >&2
    echo "!! machine_profile.sh: probed topology does NOT match the" >&2
    echo "!! '$NUMATYPING_MACHINE' profile. Pinning the profile values." >&2
    echo "!!   probed : nodes=$got_phys order=$got_order threads=$got_threads bind='$got_bind'" >&2
    echo "!!   profile: nodes=$_NP_EXP_PHYS_NODES order=$_NP_EXP_NODE_ORDER threads=$_NP_EXP_PART_THREADS bind='$_NP_EXP_BIND'" >&2
    echo "!! Expected when configuring on a login node -- NERSC login nodes and" >&2
    echo "!! CPU compute nodes do not share a NUMA layout. Run" >&2
    echo "!!   bash scripts/configure_machine.sh --stages=topo" >&2
    echo "!! inside the job (or trust these pins) before measuring anything." >&2
    echo "" >&2

    cat >> "$env_file" <<EOF

# ---------------------------------------------------------------------------
# OVERRIDE: pinned by scripts/machine_profile.sh for machine '$NUMATYPING_MACHINE'
# because the probe above ran somewhere with a different NUMA layout
# (probed order=$got_order threads=$got_threads). These later exports win.
# ---------------------------------------------------------------------------
export NUM_PHYS_NODES=$_NP_EXP_PHYS_NODES
export CPU_NODES=$_NP_EXP_CPU_NODES
export NUM_CPU_NODES=$(echo "$_NP_EXP_CPU_NODES" | tr ',' '\n' | grep -c .)
export NUM_PARTITIONS=2
export NUMA_NODE_ORDER=$_NP_EXP_NODE_ORDER
export PARTITION_THREADS=$_NP_EXP_PART_THREADS
export NUMACTL_BIND="$_NP_EXP_BIND"
EOF
    NUMATYPING_TOPOLOGY_SOURCE="PINNED from $NUMATYPING_MACHINE profile (probe disagreed)"
}

# ---------------------------------------------------------------------------
# 5. Public helpers
# ---------------------------------------------------------------------------

# Re-check the live machine against the profile's pins. Returns non-zero on
# mismatch. Call this at the top of a batch job: it is the guard that catches
# "configured on a login node, running somewhere unexpected".
numatyping_verify_topology() {
    [ -z "$_NP_EXP_NODE_ORDER" ] && { echo "verify: no pins for '$NUMATYPING_MACHINE' (probe-only)"; return 0; }

    local nd live_nodes live_threads rc=0
    live_nodes=$(ls -d /sys/devices/system/node/node[0-9]* 2>/dev/null | wc -l)
    live_threads=0
    for nd in ${_NP_EXP_NODE_ORDER//,/ }; do
        local cl n
        cl=$(cat "/sys/devices/system/node/node$nd/cpulist" 2>/dev/null) || true
        [ -z "$cl" ] && { echo "verify: FAIL node $nd has no CPUs on this machine" >&2; rc=1; continue; }
        n=$(python3 -c "
import sys
t=0
for p in sys.argv[1].split(','):
    if '-' in p:
        a,b=p.split('-'); t+=int(b)-int(a)+1
    elif p: t+=1
print(t)" "$cl")
        live_threads=$(( live_threads + n ))
    done

    if [ "$live_nodes" != "$_NP_EXP_PHYS_NODES" ]; then
        echo "verify: FAIL expected $_NP_EXP_PHYS_NODES NUMA nodes, found $live_nodes" >&2
        echo "verify:      (on Perlmutter this usually means --constraint=cpu was missing)" >&2
        rc=1
    fi
    if [ "$live_threads" != "$_NP_EXP_PART_THREADS" ]; then
        echo "verify: FAIL expected $_NP_EXP_PART_THREADS hw threads on nodes $_NP_EXP_NODE_ORDER, found $live_threads" >&2
        rc=1
    fi
    [ "$rc" -eq 0 ] && echo "verify: OK  $_NP_EXP_PHYS_NODES NUMA nodes, $live_threads hw threads on nodes $_NP_EXP_NODE_ORDER"
    return $rc
}

numatyping_profile_summary() {
    echo "  machine            : $NUMATYPING_MACHINE"
    echo "  root               : $ROOT_DIR"
    echo "  topology source    : ${NUMATYPING_TOPOLOGY_SOURCE:-unset}"
    echo "  NUMA_NODE_ORDER    : ${NUMA_NODE_ORDER:-unset}"
    echo "  PARTITION_THREADS  : ${PARTITION_THREADS:-unset}"
    echo "  NUMACTL_BIND       : ${NUMACTL_BIND:-unset}"
    echo "  CXX                : ${CXX:-unset}"
    echo "  clang++            : $(command -v clang++ 2>/dev/null || echo 'NOT FOUND')"
    echo "  clang version      : $(clang++ --version 2>/dev/null | head -n1 || echo 'n/a')"
    echo "  JEMALLOC_ROOT      : ${JEMALLOC_ROOT:-unset}"
    echo "  THP                : $(sed -n 's/.*\[\(.*\)\].*/\1/p' /sys/kernel/mm/transparent_hugepage/enabled 2>/dev/null || echo 'n/a')"
    echo "  numa_balancing     : $(cat /proc/sys/kernel/numa_balancing 2>/dev/null || echo 'n/a')"
}

# ---------------------------------------------------------------------------
# 6. Run
# ---------------------------------------------------------------------------
_np_setup_toolchain || return 1 2>/dev/null || exit 1
if [ "${NUMATYPING_SKIP_TOPOLOGY:-0}" != "1" ]; then
    _np_setup_topology || return 1 2>/dev/null || exit 1
fi
# shellcheck disable=SC1090
[ -f "$ROOT_DIR/machine.env" ] && . "$ROOT_DIR/machine.env"
