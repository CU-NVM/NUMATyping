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
#
# One ycsb campaign, one AutoNUMA arm.  Submit as:
#
#   mkdir -p Runs/slurm
#   sbatch --export=ALL,CAMPAIGN=campaign01,AN_MODE=off \
#          --job-name=campaign01-off scripts/ycsb_campaign.slurm
#   sbatch --export=ALL,CAMPAIGN=campaign01,AN_MODE=on  \
#          --job-name=campaign01-on  scripts/ycsb_campaign.slurm
#
# The two arms MUST be separate jobs -- campaign.py writes one AN folder per
# invocation, and its manifest guards check that both arms share a commit and
# an identical parameter set.
#
# --constraint=cpu is load-bearing, not decoration: it selects the 2x EPYC 7763
# CPU nodes (256 CPUs, NPS=4 -> 8 NUMA domains, ~64 GB each).  Without it you
# can land on a GPU node, which is single-socket with 4 NUMA domains, and the
# 0,7 node pair would mean something entirely different.
#
# Replaces the stale ycsb_job*.slurm files in the repo root, which call
# `python3 load.py` / `python3 runYCSB.py` from the root -- those moved into
# scripts/ in commit 29f5e7db -- and which would use runYCSB.py's --perlmutter
# defaults (200M keys / 266600 buckets / 128 threads), the paper's numbers
# rather than these campaigns' (stormbreaker.md sections 5 and 9.2).
set -euo pipefail

CAMPAIGN="${CAMPAIGN:?set CAMPAIGN, e.g. --export=ALL,CAMPAIGN=campaign01}"
AN_MODE="${AN_MODE:?set AN_MODE to on or off}"
case "$AN_MODE" in on|off) ;; *) echo "AN_MODE must be on|off"; exit 2 ;; esac

cd "$SCRATCH/NUMATyping"

echo "=== JOB ==============================================================="
echo "job            : ${SLURM_JOB_ID:-interactive}  on $(hostname)"
echo "campaign       : $CAMPAIGN"
echo "AutoNUMA arm   : $AN_MODE"
echo "started        : $(date)"

# --- environment -------------------------------------------------------------
module load python 2>/dev/null || true

# Do NOT let the profile rewrite machine.env here: it was derived at configure
# time, concurrent jobs share this directory, and we would rather VERIFY the
# compute node matches than silently re-derive and run on whatever we got.
export NUMATYPING_SKIP_TOPOLOGY=1
source scripts/machine_profile.sh perlmutter-cpu
source machine.env

echo
echo "=== MACHINE ==========================================================="
numatyping_profile_summary
echo
echo "--- topology check (fails the job rather than measuring the wrong nodes)"
numatyping_verify_topology
echo
echo "--- numactl -H"
numactl -H | grep -E '^node [0-7] (size|cpus)|^available'
echo
# THP and the kernel balancing knob are recorded because they change how AN_on
# behaves and are NOT comparable across machines (stormbreaker.md section 2.2).
echo "THP            : $(cat /sys/kernel/mm/transparent_hugepage/enabled)"
echo "numa_balancing : $(cat /proc/sys/kernel/numa_balancing)  (kernel knob; we cannot change it without root)"
echo "                 the AN contrast here is numactl --balancing per process, via --an-mode"

# --- run ---------------------------------------------------------------------
source scripts/ycsb_campaigns.sh
PARAMS="$(ycsb_campaign_params "$CAMPAIGN")"
PURPOSE="$(ycsb_campaign_purpose "$CAMPAIGN"); AN arm via numactl -b (--an-mode $AN_MODE)"

echo
echo "=== CAMPAIGN =========================================================="
echo "params  : $PARAMS"
echo "threads : $PARTITION_THREADS  (PARTITION_THREADS, not the benchmarks.py default of 80)"
echo "purpose : $PURPOSE"
echo

# DRY_RUN=1 validates everything above -- modules, topology, campaign lookup,
# the exact campaign.py command -- without running the campaign.  Use it on a
# login node before submitting, so a typo costs seconds rather than node hours.
if [ "${DRY_RUN:-0}" = "1" ]; then
    echo "DRY_RUN=1 -- would run:"
    echo "  python3 scripts/campaign.py --bench ycsb --slug ${CAMPAIGN}-perl \\"
    echo "      --purpose \"$PURPOSE\" \\"
    echo "      $PARAMS \\"
    echo "      --threads $PARTITION_THREADS --an-mode $AN_MODE"
    echo
    echo "=== DRY RUN OK $(date) ================================================"
    exit 0
fi

# shellcheck disable=SC2086
python3 scripts/campaign.py --bench ycsb \
    --slug "${CAMPAIGN}-perl" \
    --purpose "$PURPOSE" \
    $PARAMS \
    --threads "$PARTITION_THREADS" \
    --an-mode "$AN_MODE"

echo
echo "=== DONE $(date) ======================================================"
