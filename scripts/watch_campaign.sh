#!/bin/bash
# watch_campaign.sh [jobid] -- live view of running ycsb campaign jobs.
#
#   bash scripts/watch_campaign.sh          # summarise every queued/running job
#   bash scripts/watch_campaign.sh 1234567  # follow one job's output
#   bash scripts/watch_campaign.sh --check  # one-shot health check, exits non-zero on trouble
#
# The point is to notice a broken job in minutes rather than at the walltime
# limit.  campaign.py keeps going after an individual run fails, so "the job is
# still running" does not mean "the job is still healthy".
set -uo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

BAD_PAT='FAILURE MARKER|STATUS: FAILED|ERROR:|Traceback|Refusing:|FAILED \(see log\)|ABORT \[|slurmstepd|DUE TO TIME LIMIT|Killed|oom'
GOOD_PAT='STATUS: OK|all CSVs complete|preflight OK|=== RUNNING'

summary() {
    echo "=== queue ==="
    squeue --me -o "%.10i %.22j %.9T %.11M %.11l %.6D %R" 2>/dev/null || echo "(squeue unavailable)"
    echo
    echo "=== recent job outputs ==="
    local any=0
    for f in $(ls -t Runs/slurm/*.out 2>/dev/null | head -12); do
        any=1
        local status
        if   grep -qE 'STATUS: OK'     "$f"; then status="OK"
        elif grep -qE 'STATUS: FAILED' "$f"; then status="FAILED"
        elif grep -qE "$BAD_PAT"       "$f"; then status="TROUBLE"
        else status="running?"
        fi
        printf '%-9s %-52s %s\n' "$status" "$(basename "$f")" "$(date -r "$f" '+%H:%M:%S')"
        if [ "$status" = "FAILED" ] || [ "$status" = "TROUBLE" ]; then
            grep -nE "$BAD_PAT" "$f" | head -4 | sed 's/^/            /'
        fi
        # progress: how far into the sweep is it
        local done_runs
        done_runs=$(grep -cE '^      rc=' "$f" 2>/dev/null || echo 0)
        [ "$done_runs" -gt 0 ] && echo "            runs finished: $done_runs / 28"
    done
    [ "$any" = 0 ] && echo "(no job outputs in Runs/slurm/)"
}

case "${1:-}" in
    "")
        summary
        ;;
    --check)
        summary
        if ls Runs/slurm/*.out >/dev/null 2>&1 && grep -qlE "$BAD_PAT" Runs/slurm/*.out; then
            echo; echo "TROUBLE FOUND -- see above"; exit 1
        fi
        echo; echo "no failure markers found"; exit 0
        ;;
    *)
        f=$(ls -t Runs/slurm/*"$1"*.out 2>/dev/null | head -1)
        [ -z "$f" ] && { echo "no output file for job $1 in Runs/slurm/"; exit 1; }
        echo "following $f  (ctrl-C to stop)"
        tail -f -n 200 "$f" | grep --line-buffered -E "$BAD_PAT|$GOOD_PAT|^      rc=|^  \[|STATUS:"
        ;;
esac
