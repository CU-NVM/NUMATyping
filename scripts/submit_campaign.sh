#!/bin/bash
# submit_campaign.sh <campaign> [--parallel] [--dry]
#
#   bash scripts/submit_campaign.sh campaign01           # chained (default)
#   bash scripts/submit_campaign.sh campaign01 --parallel
#   bash scripts/submit_campaign.sh campaign01 --dry     # validate, submit nothing
#
# Submits both AutoNUMA arms of one campaign.
#
# BY DEFAULT THE ARMS ARE CHAINED: arm "on" is submitted with
# --dependency=afterok on arm "off".  Two reasons, both about not wasting
# allocation:
#
#   1. If the first arm fails, the second never runs.  The wrapper exits
#      non-zero on a failed run, a short CSV, a topology mismatch or a memory
#      floor breach, so afterok catches all of those -- saving ~3.6 node-hours.
#   2. Both arms append to the SAME Campaigns/ycsb/<slug>/manifest.md.  Running
#      them concurrently races: both can see no manifest, both create one, and
#      the commit/param guard that makes the AN contrast trustworthy is
#      bypassed.  Chaining makes arm 2 read a manifest arm 1 finished writing.
#
# --parallel opts out (halves wall time, accepts both risks).
#
# DO NOT COMMIT between the two arms -- campaign.py hard-exits if HEAD differs
# from the commit recorded in the manifest.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

CAMPAIGN="${1:?usage: submit_campaign.sh <campaign> [--parallel] [--dry]}"
shift || true
PARALLEL=0; DRY=0
for a in "$@"; do
    case "$a" in
        --parallel) PARALLEL=1 ;;
        --dry)      DRY=1 ;;
        *) echo "unknown option: $a" >&2; exit 2 ;;
    esac
done

source scripts/ycsb_campaigns.sh
PARAMS="$(ycsb_campaign_params "$CAMPAIGN")" || exit 1

echo "campaign : $CAMPAIGN"
echo "params   : $PARAMS"
echo "mode     : $([ "$PARALLEL" = 1 ] && echo parallel || echo 'chained (arm on runs only if arm off succeeds)')"

# The tree must be clean now AND stay on this commit until both arms finish.
if [ -n "$(git status --porcelain)" ]; then
    echo "REFUSING: working tree is dirty. campaign.py's git gate will reject the job." >&2
    git status --porcelain >&2
    exit 1
fi
echo "commit   : $(git rev-parse --short HEAD)  $(git log -1 --pretty=%s)"

# Validate the whole job path before spending anything.
echo
echo "--- validating both arms (DRY_RUN, no allocation used) ---"
for arm in off on; do
    if DRY_RUN=1 SKIP_PREFLIGHT=1 CAMPAIGN="$CAMPAIGN" AN_MODE="$arm" \
         bash scripts/ycsb_campaign.slurm >/tmp/_val_$arm.log 2>&1; then
        echo "  arm $arm: OK"
    else
        echo "  arm $arm: FAILED"; tail -20 /tmp/_val_$arm.log; exit 1
    fi
done

[ "$DRY" = 1 ] && { echo; echo "--dry: validated only, nothing submitted."; exit 0; }

mkdir -p Runs/slurm
echo
J1=$(sbatch --parsable --job-name="${CAMPAIGN}-off" \
        --export=ALL,CAMPAIGN="$CAMPAIGN",AN_MODE=off \
        scripts/ycsb_campaign.slurm)
echo "submitted arm off : job $J1"

if [ "$PARALLEL" = 1 ]; then
    J2=$(sbatch --parsable --job-name="${CAMPAIGN}-on" \
            --export=ALL,CAMPAIGN="$CAMPAIGN",AN_MODE=on \
            scripts/ycsb_campaign.slurm)
    echo "submitted arm on  : job $J2  (parallel)"
else
    J2=$(sbatch --parsable --dependency=afterok:"$J1" --job-name="${CAMPAIGN}-on" \
            --export=ALL,CAMPAIGN="$CAMPAIGN",AN_MODE=on \
            scripts/ycsb_campaign.slurm)
    echo "submitted arm on  : job $J2  (starts only if $J1 succeeds)"
fi

echo
echo "watch with:  bash scripts/watch_campaign.sh"
echo "             bash scripts/watch_campaign.sh $J1"
echo "cancel with: scancel $J1 $J2"
