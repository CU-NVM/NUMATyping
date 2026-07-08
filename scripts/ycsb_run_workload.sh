#!/bin/bash
# ycsb_run_workload.sh "<WORKLOAD_STRING>" [uniform|zipfian]
#
# Runs the transformed YCSB for ONE workload across all FOUR thread/DS configs:
#   numa/numa, numa/regular, regular/numa, regular/regular
# all into a single CSV. Between each config it refreshes memory WITHOUT a reboot
# and WITHOUT touching AutoNUMA. Every step writes a persistent, timestamped
# breadcrumb so a mid-run reboot or crash is fully diagnosable afterward.
#
# Output : Result/AN_off/ycsb_<WL>_<MIX>.csv   (commas in WL -> underscores)
# Log    : Result/AN_off/ycsb_<WL>_<MIX>.log
#
# Default config (env-overridable): 80 threads, 30001 buckets (coprime -> no coset),
# 2000 tables, 60M keys, 10 min/run, --hash=mix, bound to CPU nodes 0,1. AutoNUMA OFF.
# Override via env vars: THREADS BUCKETS TABLES KEYS DURATION INTERVAL HASH.

set -u

WL="${1:?usage: $0 \"<WORKLOAD_STRING>\" [uniform|zipfian] [an_mode 0|1]}"
MIX="${2:-zipfian}"
AN_MODE="${3:-0}"               # 0 = AutoNUMA off (AN_off), 1 = AutoNUMA on (AN_on)

ROOT=/home/kiwo9430/NUMATyping
BIN="$ROOT/Output/ycsb/bin/ycsb"
TAG="${WL//,/_}"
if [ "$AN_MODE" = "1" ]; then
    AN_FOLDER="AN_on"
    NUMACTL="numactl --balancing --cpunodebind=0,1 --membind=0,1"
else
    AN_FOLDER="AN_off"
    NUMACTL="numactl --cpunodebind=0,1 --membind=0,1"
fi
# Revision runs land here regardless of AN mode; mix + payload are in the filename, not columns.
# Override the directory with the OUTDIR env var.
OUTDIR="${OUTDIR:-$ROOT/Result/Revision/ycsb}"
PAYLOAD="${PAYLOAD:-0}"          # value-payload size in bytes for the filename; 0 = no payload
mkdir -p "$OUTDIR"
OUT="$OUTDIR/ycsb_${TAG}_${MIX}_${PAYLOAD}.csv"
LOG="$OUTDIR/ycsb_${TAG}_${MIX}_${PAYLOAD}.log"
HEADER="Date, Time, Num_Tables, Num_Threads, Thread_Config, DS_Config, Buckets, Workload, Duration, Num_Keys, Interval, Ops_Node0, Ops_Node1, Total_Ops"

# Run parameters (env-overridable; defaults = safe fixed-coset + payload config).
THREADS="${THREADS:-80}"
BUCKETS="${BUCKETS:-30001}"     # coprime to TABLES -> all buckets reachable (no coset)
TABLES="${TABLES:-2000}"
KEYS="${KEYS:-60000000}"        # ~33 GB/node with the VALUE_SIZE payload
DURATION="${DURATION:-600}"     # 10 min per config
INTERVAL="${INTERVAL:-10}"
HASH="${HASH:-mix}"

# All four configs, in order.
CONFIGS=("numa numa" "numa regular" "regular numa" "regular regular")

log(){ echo "$(date '+%F %T') | $*" | tee -a "$LOG"; }

# Append mode: write the header only if the file is new/empty, then append configs.
if [ ! -s "$OUT" ]; then echo "$HEADER" > "$OUT"; fi
log "================ START workload=$WL mix=$MIX hash=$HASH t=$THREADS b=$BUCKETS a=$TABLES k=$KEYS u=$DURATION i=$INTERVAL an_mode=$AN_MODE -> $AN_FOLDER/$(basename "$OUT") | boot=$(uptime -s) ================"

run_cfg(){
    local TH=$1 DS=$2
    # numa_balancing must equal AN_MODE (a reboot resets it to 1). Refuse to write
    # mislabeled data: AN_off needs balancing=0, AN_on needs balancing=1.
    local AN; AN=$(cat /proc/sys/kernel/numa_balancing 2>/dev/null || echo '?')
    if [ "$AN" != "$AN_MODE" ]; then
        log "ABORT [$TH/$DS]: numa_balancing=$AN but expected $AN_MODE (AN_MODE). Likely a reboot. Fix with sudo and rerun."
        return 2
    fi
    log "[START $TH/$DS $MIX] AN=$AN uptime=$(uptime -p)"
    $NUMACTL "$BIN" --th_config=$TH --DS_config=$DS --mix=$MIX --hash=$HASH \
        -t $THREADS -b $BUCKETS -a $TABLES --w="$WL" -u $DURATION -k $KEYS -i $INTERVAL >> "$OUT" 2>>"$LOG"
    local rc=$?
    if [ $rc -eq 0 ]; then log "[DONE  $TH/$DS $MIX] rc=0"; else log "[FAIL  $TH/$DS $MIX] rc=$rc"; fi
    return $rc
}

declare -A RC
i=0
for cfg in "${CONFIGS[@]}"; do
    set -- $cfg
    run_cfg "$1" "$2"; RC["$1/$2"]=$?
    i=$((i+1))
    if [ $i -lt ${#CONFIGS[@]} ]; then
        # Refresh memory between configs (no reboot, no AutoNUMA change).
        # The prior process's data is freed on exit; sync + settle lets the OS
        # reclaim it so the next config starts cold and fair.
        # (Hard page-cache drop would need: sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches')
        log "Refreshing memory before next config: sync + settle 30s"
        sync; sleep 30
    fi
done

log "================ END workload=$WL : ${RC[numa/numa]:-?}/nn ${RC[numa/regular]:-?}/nr ${RC[regular/numa]:-?}/rn ${RC[regular/regular]:-?}/rr (rc codes) ================"
