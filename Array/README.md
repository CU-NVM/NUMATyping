# Array Test

## Running

```shell
    numactl --cpunodebind=0,1 --membind=0,1 ./bin/array --th_config=numa --DS_config=numa -t 40 -n 1000 -u 120 -s 10000000 -i 10 
```

```shell
    python3 meta.py  numactl --cpunodebind=0,1 --membind=0,1 ./bin/array --meta th_config:numa:regular --meta DS_config:numa:regular --meta t:40 --meta n:1000 --meta u:120 --meta s:100000 --meta i:10
```