#!/bin/bash
#SBATCH --account=m5308
#SBATCH --constraint=cpu
#SBATCH --qos=regular
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --time=05:00:00
#SBATCH --job-name=ycsb-campaign
#SBATCH --output=Runs/slurm/%x-%j.out
#SBATCH --error=Runs/slurm/%x-%j.out
#SBATCH --signal=B:USR1@600
#
# One ycsb campaign, one AutoNUMA arm.  Submit as:
#
#   mkdir -p Runs/slurm
#   sbatch --export=ALL,CAMPAIGN=campaign01,AN_MODE=off \
#          --job-name=campaign01-off scripts/ycsb_campaign.slurm
#   sbatch --export=ALL,CAMPAIGN=campaign01,AN_MODE=on  \
#          --job-name=campaign01-on  scripts/ycsb_campaign.slurm
#
# Watch a running job from a login node with:
#   bash scripts/watch_campaign.sh            # all active jobs
#   bash scripts/watch_campaign.sh <jobid>
#
# The two arms MUST be separate jobs -- campaign.py writes one AN folder per
# invocation, and its manifest guards check that both arms share a commit and an
# identical parameter set.  DO NOT COMMIT between submitting the two arms: the
# guard compares HEAD against the commit recorded in the slug's manifest and
# hard-exits on a mismatch, stranding the second arm.
#
# --constraint=cpu is load-bearing, not decoration: it selects the 2x EPYC 7763
# CPU nodes (256 CPUs, NPS=4 -> 8 NUMA domains, ~64 GB each).  Without it you can
# land on a GPU node, which is single-socket with 4 NUMA domains, and the 0,7
# node pair would mean something entirely different.
#
# FAILURE HANDLING.  campaign.py does NOT abort when an individual run fails --
# it prints "rc=N  FAILED (see log)" and continues (campaign.py:400).  So a job
# can burn its full walltime, produce empty CSVs and still exit 0.  This wrapper
# therefore: runs a preflight before committing to hours of walltime, watches the
# campaign output for failure markers and aborts early, and validates row counts
# at the end so a partial campaign exits non-zero.
set -euo pipefail

CAMPAIGN="${CAMPAIGN:?set CAMPAIGN, e.g. --export=ALL,CAMPAIGN=campaign01}"
AN_MODE="${AN_MODE:?set AN_MODE to on or off}"
case "$AN_MODE" in on|off) ;; *) echo "AN_MODE must be on|off"; exit 2 ;; esac
ABORT_ON_RUN_FAILURE="${ABORT_ON_RUN_FAILURE:-1}"

cd "$SCRATCH/NUMATyping"
START_EPOCH=$(date +%s)
JOBID="${SLURM_JOB_ID:-interactive}"
RUNLOG="Runs/slurm/${CAMPAIGN}-${AN_MODE}-${JOBID}.campaign.log"
mkdir -p Runs/slurm

# --- say why we stopped, always, in a form that greps ------------------------
FAILREASON=""
on_exit() {
    local rc=$?
    local mins=$(( ($(date +%s) - START_EPOCH) / 60 ))
    echo
    echo "======================================================================"
    if [ "$rc" -eq 0 ]; then
        echo "STATUS: OK   campaign=$CAMPAIGN arm=$AN_MODE job=$JOBID  (${mins} min)"
    else
        echo "STATUS: FAILED rc=$rc  campaign=$CAMPAIGN arm=$AN_MODE job=$JOBID  (${mins} min)"
        [ -n "$FAILREASON" ] && echo "REASON: $FAILREASON"
        echo "--- last 40 lines of $RUNLOG ---"
        tail -40 "$RUNLOG" 2>/dev/null || echo "(no campaign log written)"
    fi
    echo "ended  : $(date)"
    echo "======================================================================"
    exit $rc
}
trap on_exit EXIT
fail() { FAILREASON="$*"; echo "ERROR: $*" >&2; exit 1; }
trap 'echo "WARNING: 10 minutes from the walltime limit at $(date); campaign will be cut short"' USR1

echo "=== JOB ==============================================================="
echo "job            : $JOBID  on $(hostname)"
echo "campaign       : $CAMPAIGN"
echo "AutoNUMA arm   : $AN_MODE"
echo "started        : $(date)"
echo "campaign log   : $RUNLOG"

# --- environment -------------------------------------------------------------
# machine_profile.sh loads python, PrgEnv-llvm, spack and cmake, and verifies
# python is >= 3.7 (campaign.py needs it; the system python3 is 3.6).
#
# Do NOT let the profile rewrite machine.env here: it was derived at configure
# time, concurrent jobs share this directory, and we would rather VERIFY the
# compute node matches than silently re-derive and run on whatever we got.
export NUMATYPING_SKIP_TOPOLOGY=1
source scripts/machine_profile.sh perlmutter-cpu || fail "machine profile setup failed"
source machine.env

echo
echo "=== MACHINE ==========================================================="
numatyping_profile_summary
echo
echo "--- topology check (fails the job rather than measuring the wrong nodes)"
numatyping_verify_topology || fail "compute node topology does not match the perlmutter-cpu profile"
echo
echo "--- numactl -H"
numactl -H | grep -E '^node [0-7] (size|cpus)|^available'
echo
# THP and the kernel balancing knob are recorded because they change how AN_on
# behaves and are NOT comparable across machines (stormbreaker.md section 2.2).
echo "THP            : $(cat /sys/kernel/mm/transparent_hugepage/enabled)"
echo "numa_balancing : $(cat /proc/sys/kernel/numa_balancing)  (kernel knob; we cannot change it without root)"
echo "                 the AN contrast here is numactl --balancing per process, via --an-mode"
echo "free memory on the bound nodes:"
numactl -H | grep -E '^node (0|7) (size|free)'

# --- campaign definition -----------------------------------------------------
source scripts/ycsb_campaigns.sh
PARAMS="$(ycsb_campaign_params "$CAMPAIGN")" || fail "unknown campaign '$CAMPAIGN'"
PURPOSE="$(ycsb_campaign_purpose "$CAMPAIGN"); AN arm via numactl -b (--an-mode $AN_MODE)"

echo
echo "=== CAMPAIGN =========================================================="
echo "params  : $PARAMS"
echo "threads : $PARTITION_THREADS  (PARTITION_THREADS, not the benchmarks.py default of 80)"
echo "purpose : $PURPOSE"

# --- preflight: prove the binary works HERE before spending hours -------------
# A ~40 s run at 1/1000th the keyspace. Catches a missing binary, a broken UMF
# link, an unbindable node, a jemalloc rpath problem -- in under a minute rather
# than at the end of a 5 hour reservation.
BIN="Output/ycsb/bin/ycsb"
[ -x "$BIN" ] || fail "$BIN missing or not executable -- run scripts/configure_machine.sh"

if [ "${SKIP_PREFLIGHT:-0}" != "1" ]; then
    echo
    echo "--- preflight (short run under the real binding) ---"
    PRE=Runs/slurm/${CAMPAIGN}-${AN_MODE}-${JOBID}.preflight.csv
    if ! NUMA_NODE_ORDER="$NUMA_NODE_ORDER" numactl $NUMACTL_BIND "$BIN" \
            --th_config=numa --DS_config=numa --mix=uniform --hash=djb2 -z 0.7 \
            -t "$PARTITION_THREADS" -b 1009 -a 100 \
            --workload=A-50-50-50,A-100-0-50 -u 20 -k 2000000 -i 10 -p 64 -W 5 \
            > "$PRE" 2>&1; then
        cat "$PRE"; fail "preflight run exited non-zero"
    fi
    if [ "$(grep -c ',' "$PRE")" -lt 2 ]; then
        cat "$PRE"; fail "preflight produced no data rows"
    fi
    echo "preflight OK ($(grep -c ',' "$PRE") rows)"
fi

# DRY_RUN=1 validates everything above -- modules, topology, campaign lookup,
# the exact campaign.py command -- without running the campaign.  Use it on a
# login node before submitting, so a typo costs seconds rather than node hours.
if [ "${DRY_RUN:-0}" = "1" ]; then
    echo
    echo "DRY_RUN=1 -- would run:"
    echo "  python3 -u scripts/campaign.py --bench ycsb --slug ${CAMPAIGN}-perl \\"
    echo "      --purpose \"$PURPOSE\" \\"
    echo "      $PARAMS \\"
    echo "      --threads $PARTITION_THREADS --an-mode $AN_MODE"
    echo
    echo "=== DRY RUN OK $(date) ================================================"
    exit 0
fi

# --- run ---------------------------------------------------------------------
# python3 -u so the log updates per line and `tail -f` is actually live.
# The reader loop mirrors every line into the job output AND trips on the first
# failure marker, so a broken run surfaces in minutes instead of at the end.
echo
echo "=== RUNNING $(date) ==================================================="
set +e
(
    python3 -u scripts/campaign.py --bench ycsb \
        --slug "${CAMPAIGN}-perl" \
        --purpose "$PURPOSE" \
        $PARAMS \
        --threads "$PARTITION_THREADS" \
        --an-mode "$AN_MODE" 2>&1
    echo "__CAMPAIGN_EXIT__=$?"
) | tee "$RUNLOG" | while IFS= read -r line; do
    printf '%s\n' "$line"
    case "$line" in
        *"FAILED (see log)"*|*"ABORT ["*|*"Refusing:"*|*"Traceback (most recent"*)
            echo "!!! FAILURE MARKER: $line"
            if [ "$ABORT_ON_RUN_FAILURE" = "1" ]; then
                echo "!!! aborting the job now rather than burning the remaining walltime."
                echo "!!! set ABORT_ON_RUN_FAILURE=0 to run through failures instead."
                # kill the whole job step; the EXIT trap prints the banner
                scancel --signal=TERM "${SLURM_JOB_ID:-0}" 2>/dev/null || kill -TERM $$
            fi
            ;;
    esac
done
set -e

CAMPAIGN_RC=$(grep -o '__CAMPAIGN_EXIT__=[0-9]*' "$RUNLOG" | tail -1 | cut -d= -f2)
CAMPAIGN_RC="${CAMPAIGN_RC:-1}"
[ "$CAMPAIGN_RC" = "0" ] || fail "campaign.py exited $CAMPAIGN_RC"

# --- validate: campaign.py exits 0 even when individual runs failed -----------
echo
echo "=== VALIDATION ========================================================"
OUTDIR="Campaigns/ycsb/${CAMPAIGN}-perl/AN_${AN_MODE}"
[ -d "$OUTDIR" ] || fail "no output directory $OUTDIR"

DURATION=$(sed -n 's/.*--duration \([0-9]*\).*/\1/p' <<< "$PARAMS")
INTERVAL=$(sed -n 's/.*--interval \([0-9]*\).*/\1/p' <<< "$PARAMS")
NCONFIG=4    # campaign.py DEFAULT_CONFIGS
EXPECT=$(( NCONFIG * (DURATION / INTERVAL + 1) ))

bad=0; nfiles=0
for f in "$OUTDIR"/*.csv; do
    [ -e "$f" ] || continue
    nfiles=$((nfiles+1))
    rows=$(( $(wc -l < "$f") - 1 ))
    if [ "$rows" -lt "$EXPECT" ]; then
        echo "  SHORT  $(basename "$f"): $rows rows, expected $EXPECT"
        bad=$((bad+1))
    else
        echo "  ok     $(basename "$f"): $rows rows"
    fi
done
echo "files: $nfiles   short: $bad   expected rows/file: $EXPECT"
[ "$nfiles" -eq 0 ] && fail "no CSVs produced in $OUTDIR"
[ "$bad" -gt 0 ] && fail "$bad of $nfiles CSVs are short -- some runs failed; see $RUNLOG"

echo "all CSVs complete"
