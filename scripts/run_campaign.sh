#!/bin/bash
# run_campaign.sh — run a YCSB config sweep and auto-write a provenance manifest.
#
# Everything (CSVs, logs, manifest.md) lands in one campaign directory:
#     Result/<BENCH>/<YYYY-MM-DD>_<TAG>/
#
# The manifest records the git commit, machine, AutoNUMA state, all parameters,
# and the exact command to reproduce -- so a result is never ambiguous later.
#
# All knobs are env-overridable; defaults below are a small, fast example.
set -u
ROOT=/home/kiwo9430/NUMATyping

# ---- knobs -----------------------------------------------------------------
BENCH="${BENCH:-ycsb}"                          # results go to Result/<BENCH>/
TAG="${TAG:-example}"
PURPOSE="${PURPOSE:-small demonstration run}"
WL="${WL:-C-50-50-50,C-100-0-50}"
MIX="${MIX:-uniform}"
HASH="${HASH:-djb2}"
PAYLOAD="${PAYLOAD:-0}"                          # VALUE_SIZE compiled into the binary (label only)
THREADS="${THREADS:-80}"
BUCKETS="${BUCKETS:-10007}"
TABLES="${TABLES:-10}"
KEYS="${KEYS:-1000000}"
DURATION="${DURATION:-15}"
INTERVAL="${INTERVAL:-5}"
CONFIGS="${CONFIGS:-numa/numa numa/regular}"     # space-separated th/ds pairs

# ---- derived ---------------------------------------------------------------
BIN="$ROOT/Output/$BENCH/bin/$BENCH"
AN=$(cat /proc/sys/kernel/numa_balancing 2>/dev/null || echo '?')
NUMACTL="numactl --cpunodebind=0,1 --membind=0,1"
[ "$AN" = "1" ] && NUMACTL="numactl --balancing --cpunodebind=0,1 --membind=0,1"

DATE=$(date '+%Y-%m-%d')
AN_FOLDER=$([ "$AN" = "1" ] && echo AN_on || echo AN_off)   # auto from numa_balancing
DIR="$ROOT/Result/$BENCH/$AN_FOLDER/${DATE}_${TAG}"
mkdir -p "$DIR"
TAG_WL="${WL//,/_}"
OUT="$DIR/${BENCH}_${TAG_WL}_${MIX}_${PAYLOAD}.csv"
LOG="$DIR/${BENCH}_${TAG_WL}_${MIX}_${PAYLOAD}.log"
MANIFEST="$DIR/manifest.md"

COMMIT=$(cd "$ROOT" && git rev-parse --short HEAD 2>/dev/null || echo 'n/a')
TREE=$(cd "$ROOT" && [ -n "$(git status --porcelain 2>/dev/null)" ] && echo 'dirty' || echo 'clean')
CPU=$(awk -F: '/model name/{gsub(/^ +/,"",$2); print $2; exit}' /proc/cpuinfo)
NODES=$(numactl -H 2>/dev/null | awk '/available:/{print $2}')

# ---- manifest --------------------------------------------------------------
cat > "$MANIFEST" <<EOF
# ${BENCH} campaign — ${TAG}

- **date:** $(date '+%Y-%m-%d %H:%M:%S')
- **purpose:** ${PURPOSE}
- **benchmark:** ${BENCH}  (\`${BIN}\`)
- **git commit:** ${COMMIT} (${TREE})
- **machine:** $(hostname) · ${CPU} · ${NODES} NUMA nodes · kernel $(uname -r)
- **AutoNUMA:** numa_balancing=${AN} ($([ "$AN" = 1 ] && echo on || echo off))

## Parameters
| param | value |
|-------|-------|
| mix | ${MIX} |
| hash | ${HASH} |
| payload VALUE_SIZE | ${PAYLOAD} |
| threads | ${THREADS} |
| buckets | ${BUCKETS} |
| tables | ${TABLES} |
| num_keys | ${KEYS} |
| duration (s) | ${DURATION} |
| interval (s) | ${INTERVAL} |
| workload | ${WL} |
| configs | ${CONFIGS} |

## Reproduce
\`\`\`sh
BENCH=${BENCH} TAG=${TAG} WL='${WL}' MIX=${MIX} HASH=${HASH} PAYLOAD=${PAYLOAD} \\
  THREADS=${THREADS} BUCKETS=${BUCKETS} TABLES=${TABLES} KEYS=${KEYS} \\
  DURATION=${DURATION} INTERVAL=${INTERVAL} CONFIGS='${CONFIGS}' \\
  bash scripts/run_campaign.sh
\`\`\`

## Files
EOF

# ---- run -------------------------------------------------------------------
HEADER="Date, Time, Num_Tables, Num_Threads, Thread_Config, DS_Config, Buckets, Workload, Duration, Num_Keys, Interval, Ops_Node0, Ops_Node1, Total_Ops"
echo "$HEADER" > "$OUT"
log(){ echo "$(date '+%F %T') | $*" | tee -a "$LOG"; }
log "START campaign=$(basename "$DIR") commit=$COMMIT AN=$AN"
for cfg in $CONFIGS; do
    TH="${cfg%%/*}"; DS="${cfg##*/}"
    log "[START $TH/$DS]"
    $NUMACTL "$BIN" --th_config=$TH --DS_config=$DS --mix=$MIX --hash=$HASH \
        -t $THREADS -b $BUCKETS -a $TABLES --w="$WL" -u $DURATION -k $KEYS -i $INTERVAL >> "$OUT" 2>>"$LOG"
    log "[DONE  $TH/$DS] rc=$?"
done
{ echo "- \`$(basename "$OUT")\` — results"; echo "- \`$(basename "$LOG")\` — run log"; } >> "$MANIFEST"
log "DONE -> $DIR"
echo ">>> campaign at: $DIR"
