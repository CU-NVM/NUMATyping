#!/bin/bash
# configure_machine.sh -- build and install everything a given machine needs.
#
#   bash scripts/configure_machine.sh [machine] [--stages=LIST] [--suites=LIST] [-j N]
#
#   machine    stormbreaker | perlmutter-cpu | generic   (default: auto-detect)
#   --stages   comma list of: topo, clangtool, umf, suites   (default: all)
#   --suites   comma list of suite names                     (default: ycsb)
#              valid: ycsb DataStructureTests Histogram Array DataStructureTests_four
#   -j N       parallel build jobs (default: 16, capped for login-node etiquette)
#
# Examples
#   bash scripts/configure_machine.sh                       # everything, auto-detect
#   bash scripts/configure_machine.sh perlmutter-cpu
#   bash scripts/configure_machine.sh --stages=suites --suites=ycsb,DataStructureTests
#   bash scripts/configure_machine.sh --stages=topo         # re-probe inside a job
#
# Everything this builds is gitignored (numa-clang-tool/build,
# unified-memory-framework/build, Output/*, machine.env), so a clean tree stays
# clean and campaign.py's git_gate() keeps working.
set -uo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export ROOT_DIR

MACHINE=""
STAGES="topo,clangtool,umf,suites"
SUITES="ycsb"
JOBS=16

for arg in "$@"; do
    case "$arg" in
        --stages=*) STAGES="${arg#*=}" ;;
        --suites=*) SUITES="${arg#*=}" ;;
        -j*)        JOBS="${arg#-j}" ;;
        --help|-h)  sed -n '2,20p' "${BASH_SOURCE[0]}"; exit 0 ;;
        --*)        echo "unknown option: $arg" >&2; exit 2 ;;
        *)          MACHINE="$arg" ;;
    esac
done

has_stage() { [[ ",$STAGES," == *",$1,"* ]]; }
say() { printf '\n=== %s ===\n' "$*"; }
die() { echo "configure_machine.sh: $*" >&2; exit 1; }

# --- environment ------------------------------------------------------------
# machine_profile.sh runs the topo stage itself (probe + reconcile), so skip its
# copy when the caller did not ask for topo.
has_stage topo || export NUMATYPING_SKIP_TOPOLOGY=1
# shellcheck disable=SC1091
source "$ROOT_DIR/scripts/machine_profile.sh" ${MACHINE:+"$MACHINE"} \
    || die "could not set up machine profile"

say "PROFILE"
numatyping_profile_summary

command -v clang++ >/dev/null || die "clang++ not on PATH after profile setup"
command -v cmake   >/dev/null || die "cmake not on PATH after profile setup"

CLANG_MAJOR="$(clang++ --version | grep -Eo 'clang version [0-9]+' | awk '{print $3}' | head -n1)"

# ---------------------------------------------------------------------------
# stage: clangtool
# ---------------------------------------------------------------------------
if has_stage clangtool; then
    say "BUILD numa-clang-tool  (clang $CLANG_MAJOR)"
    # Fresh configure: the cache pins absolute LLVM paths, and those move when
    # the module version moves. Cheap to redo, expensive to debug.
    rm -rf "$ROOT_DIR/numa-clang-tool/build"
    cmake -S "$ROOT_DIR/numa-clang-tool" -B "$ROOT_DIR/numa-clang-tool/build" \
          -DCMAKE_BUILD_TYPE=Release \
          -DCLANG_VER="$CLANG_MAJOR" \
        || die "numa-clang-tool configure failed"
    cmake --build "$ROOT_DIR/numa-clang-tool/build" -j "$JOBS" \
        || die "numa-clang-tool build failed (stormbreaker.md section 4.4: AST-matcher APIs move across LLVM majors)"
    [ -x "$ROOT_DIR/numa-clang-tool/build/bin/clang-tool" ] \
        || die "numa-clang-tool built but bin/clang-tool is missing"
    echo "ok: $ROOT_DIR/numa-clang-tool/build/bin/clang-tool"
fi

# ---------------------------------------------------------------------------
# stage: umf
# ---------------------------------------------------------------------------
if has_stage umf; then
    say "BUILD unified-memory-framework"
    rm -rf "$ROOT_DIR/unified-memory-framework/build"
    cmake -S "$ROOT_DIR/unified-memory-framework" \
          -B "$ROOT_DIR/unified-memory-framework/build" \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_C_COMPILER="${CC:-clang}" \
          -DCMAKE_CXX_COMPILER="${CXX:-clang++}" \
          -DUMF_BUILD_TESTS=OFF \
          -DUMF_BUILD_EXAMPLES=OFF \
          -DUMF_BUILD_BENCHMARKS=OFF \
          -DUMF_BUILD_LIBUMF_POOL_JEMALLOC=ON \
          -DUMF_BUILD_SHARED_LIBRARY=OFF \
          ${JEMALLOC_ROOT:+-DCMAKE_PREFIX_PATH="$JEMALLOC_ROOT"} \
        || die "UMF configure failed"
    cmake --build "$ROOT_DIR/unified-memory-framework/build" -j "$JOBS" \
        || die "UMF build failed"
    for lib in libumf.a libjemalloc_pool.a; do
        [ -f "$ROOT_DIR/unified-memory-framework/build/lib/$lib" ] \
            || die "UMF built but lib/$lib is missing (benchmarks link both statically)"
    done
    ls -l "$ROOT_DIR/unified-memory-framework/build/lib/"libumf.a \
          "$ROOT_DIR/unified-memory-framework/build/lib/"libjemalloc_pool.a
fi

# ---------------------------------------------------------------------------
# stage: suites  (numafy transform, then compile)
# ---------------------------------------------------------------------------
if has_stage suites; then
    [ -x "$ROOT_DIR/numa-clang-tool/build/bin/clang-tool" ] \
        || die "clang-tool missing -- run with --stages=clangtool first"
    [ -f "$ROOT_DIR/unified-memory-framework/build/lib/libumf.a" ] \
        || die "libumf.a missing -- run with --stages=umf first"

    IFS=',' read -ra suite_list <<< "$SUITES"
    for suite in "${suite_list[@]}"; do
        [ -z "$suite" ] && continue
        say "NUMAFY $suite"
        python3 "$ROOT_DIR/scripts/numafy.py" --ROOT_DIR="$ROOT_DIR" --umf=1 "$suite" \
            || die "numafy failed for $suite"

        say "COMPILE $suite"
        make -C "$ROOT_DIR/Output/$suite" ROOT_DIR="$ROOT_DIR" UMF=1 \
             ${JEMALLOC_ROOT:+JEMALLOC_ROOT="$JEMALLOC_ROOT"} -j "$JOBS" \
            || die "make failed for $suite"
        echo "ok: binaries in $ROOT_DIR/Output/$suite/bin"
        ls -l "$ROOT_DIR/Output/$suite/bin" 2>/dev/null
    done
fi

say "DONE"
echo "machine : $NUMATYPING_MACHINE"
echo "stages  : $STAGES"
has_stage suites && echo "suites  : $SUITES"
echo
echo "Next: source machine.env, then run scripts/run.py or scripts/campaign.py."
echo "In a batch job, call numatyping_verify_topology after sourcing"
echo "scripts/machine_profile.sh to confirm the compute node matches the profile."
