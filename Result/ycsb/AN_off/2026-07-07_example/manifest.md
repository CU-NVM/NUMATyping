# ycsb campaign — example

- **date:** 2026-07-07 19:45:26
- **purpose:** small demonstration run
- **benchmark:** ycsb  (`/home/kiwo9430/NUMATyping/Output/ycsb/bin/ycsb`)
- **git commit:** 4ba01aef (dirty)
- **machine:** stormbreaker · Intel(R) Xeon(R) Silver 4416+ · 4 NUMA nodes · kernel 6.11.5cxleak
- **AutoNUMA:** numa_balancing=0 (off)

## Parameters
| param | value |
|-------|-------|
| mix | uniform |
| hash | djb2 |
| payload VALUE_SIZE | 0 |
| threads | 80 |
| buckets | 10007 |
| tables | 10 |
| num_keys | 1000000 |
| duration (s) | 15 |
| interval (s) | 5 |
| workload | C-50-50-50,C-100-0-50 |
| configs | numa/numa numa/regular |

## Reproduce
```sh
BENCH=ycsb TAG=example WL='C-50-50-50,C-100-0-50' MIX=uniform HASH=djb2 PAYLOAD=0 \
  THREADS=80 BUCKETS=10007 TABLES=10 KEYS=1000000 \
  DURATION=15 INTERVAL=5 CONFIGS='numa/numa numa/regular' \
  bash scripts/run_campaign.sh
```

## Files
- `ycsb_C-50-50-50_C-100-0-50_uniform_0.csv` — results
- `ycsb_C-50-50-50_C-100-0-50_uniform_0.log` — run log
