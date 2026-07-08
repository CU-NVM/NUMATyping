# Perf Analysis for Experiments

## YCSB

All the perf statistics that can be reported is in `out.txt`
To run numa for ycsb benchmark 

```shell
 perf stat -e numa_reads_addressed_to_local_dram -e numa_reads_addressed_to_remote_dram -I 2000 numactl --cpunodebind=0,1 --membind=0,1 ../Output/ycsb/bin/ycsb --th_config=numa --DS_config=numa -t 40 -b 1333 --w=D -u 120 -k 10000000 --l=80-20 -i=10 -a=1000
```
