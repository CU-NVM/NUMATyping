#!/bin/bash
# all.sh [an_mode 0|1]
#   an_mode 0 -> AutoNUMA OFF, results in Result/AN_off/   (default)
#   an_mode 1 -> AutoNUMA ON,  results in Result/AN_on/
#
# Runs every workload's 4-config (numa/numa, numa/regular, regular/numa,
# regular/regular) zipfian sweep, one after another, with a memory refresh
# between workloads. Built to run overnight inside screen.
#
# IMPORTANT: set the kernel to match an_mode first:
#   an_mode 1: sudo sh -c 'echo 1 > /proc/sys/kernel/numa_balancing'
#   an_mode 0: sudo sh -c 'echo 0 > /proc/sys/kernel/numa_balancing'
set -u
AN_MODE="${1:-0}"
MIX="${MIX:-zipfian}"           # key distribution: uniform or zipfian (env-overridable)
ROOT=/home/kiwo9430/NUMATyping
if [ "$AN_MODE" = "1" ]; then AN_FOLDER="AN_on"; else AN_FOLDER="AN_off"; fi
OUTDIR="${OUTDIR:-$ROOT/Result/Revision/ycsb}"
mkdir -p "$OUTDIR"
ALLLOG="$OUTDIR/ycsb_all.log"

WORKLOADS=(
    "A-50-50-50,A-100-0-50"
    "B-50-50-50,B-100-0-50"
    "C-50-50-50,C-100-0-50"
    "D-50-50-50,D-100-0-50"
    "E-50-50-50,E-100-0-50"
    "F-50-50-50,F-100-0-50"
    "A-50-50-50,D-100-0-50"   # AD
)

log(){ echo "$(date '+%F %T') | ALL | $*" | tee -a "$ALLLOG"; }

# Sanity: kernel AutoNUMA state must match the requested mode.
AN_NOW=$(cat /proc/sys/kernel/numa_balancing 2>/dev/null || echo '?')
if [ "$AN_NOW" != "$AN_MODE" ]; then
    log "REFUSING TO START: numa_balancing=$AN_NOW but an_mode=$AN_MODE. Set it first (sudo) and relaunch."
    exit 1
fi

log "######## ALL START (an_mode=$AN_MODE -> $AN_FOLDER, mix=$MIX) : ${#WORKLOADS[@]} workloads x 4 configs | boot=$(uptime -s) ########"
for wl in "${WORKLOADS[@]}"; do
    AN=$(cat /proc/sys/kernel/numa_balancing 2>/dev/null || echo '?')
    log ">>> workload $wl  (numa_balancing=$AN, uptime=$(uptime -p))"
    bash "$ROOT/scripts/ycsb_run_workload.sh" "$wl" "$MIX" "$AN_MODE"
    log "<<< workload $wl finished; refreshing memory (sync + 60s) before next workload"
    sync; sleep 60
done
log "######## ALL DONE (an_mode=$AN_MODE) ########"
