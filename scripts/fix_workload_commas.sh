#!/bin/bash
# Replace the comma inside the YCSB workload value (X-50-50-50,Y-100-0-50) with a
# dash so the workload is a single CSV cell. Idempotent + precise (only the
# workload comma is touched). Skips files written in the last 25 min (a config
# may still be dumping rows to them).
set -u
DIR=/home/kiwo9430/NUMATyping/Result/AN_off
now=$(date +%s)
for f in "$DIR"/ycsb_*.csv; do
  [ -e "$f" ] || continue
  if [ $(( now - $(stat -c %Y "$f") )) -lt 1500 ]; then
    echo "SKIP (recently written, may be active): $(basename "$f")"; continue
  fi
  if grep -q -- '-50-50-50,[A-Za-z]*-100-0-50' "$f"; then
    sed -i 's/-50-50-50,\([A-Za-z][A-Za-z]*-100-0-50\)/-50-50-50-\1/g' "$f"
    echo "FIXED: $(basename "$f")"
  else
    echo "ok (already clean): $(basename "$f")"
  fi
done
