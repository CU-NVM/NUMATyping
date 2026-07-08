# Array — lock-based NUMA pointer-chase latency benchmark

A multithreaded microbenchmark that measures **memory-access latency under a
lock** for local versus remote NUMA placement. It is the lock-based counterpart
to the `array_lk_free` benchmark (which runs the same idea without a lock), so
comparing the two isolates the cost of the read lock.

## What it does

Each NUMA node gets one array of `array_size` pointers (`char*`), pre-filled so
every slot holds a valid pointer. Worker threads (half pinned to node 0, half to
node 1) run a **dependent-read pointer chase**:

```cpp
read_lk->lock_shared();
word = array[curr_idx];          // read a pointer (under a shared lock)
read_lk->unlock_shared();
curr_idx = (int64_t(word) + jitter) % array_size;   // the value read picks the next index
```

Because each read's index depends on the *previous* read's result, the reads
cannot be prefetched — they serialize, so throughput is bound by **memory-access
latency**. Every read also takes a per-node `shared_mutex` (`read_lk0`/`read_lk1`),
which is the point of this variant: it includes lock overhead in the measurement.

For the result to reflect DRAM latency rather than cache, make the pointer array
larger than the last-level cache: `array_size * 8 bytes` should exceed the LLC
(e.g. on a 37.5 MB/node L3, use `--array_size` well above ~5,000,000).

### Configurations
- `--th_config`  `numa` (threads pinned with `thread_numa<k>`) or `regular` (unpinned).
- `--DS_config`  `numa` (arrays placed with `numa<char*,k>`) or `regular` (plain heap).

The logical nodes (0 and 1) are mapped to physical CPU nodes at runtime by
`numaLib/numa_nodemap.hpp`, so the same binary runs on any machine.

## Build

```sh
make clean
make UMF=1 ROOT_DIR=$HOME/NUMATyping
```

Produces `./bin/array`. (Two logical NUMA partitions; no node-count macros needed.)

## Run

Bind to the CPU nodes so threads and memory stay where intended:

```sh
numactl --cpunodebind=0,1 --membind=0,1 ./bin/array \
    --th_config=numa --DS_config=numa --num_threads=80 --array_size=10000000 --duration=120
```

### Sweep multiple configs with meta.py

```sh
python3 meta.py numactl --cpunodebind=0,1 --membind=0,1 ./bin/array \
    --meta th_config:numa:regular --meta DS_config:numa:regular \
    --meta t:80 --meta s:10000000 --meta D:120
```

## Output

One CSV row:

```
Timestamp, Th_Config, DS_Config, Threads, Time(s), Ops_Node0, Ops_Node1, Total_Ops, Throughput(ops/s)
```

Higher throughput = lower average access latency. The `numa`/`numa` configuration
should lead when the array is DRAM-bound, since both the array and the threads are
co-located on the same node.

## Debugging

Watch per-node memory placement while it runs:

```sh
watch -n 1 'numastat -p $(pgrep -n array)'
```
