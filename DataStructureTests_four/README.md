# DataStructureTests_four — 4-node NUMA Data-Structure Benchmark

A multithreaded binary-search-tree (BST) benchmark that places **both the data
structures and the worker threads on specific NUMA nodes** and measures
throughput (operations/sec) over time. This is the **four-node** version of
`DataStructureTests` (which splits work across two nodes); here the work is
split across **four logical NUMA nodes**.

It is a modified version of https://github.com/nmante/Data-Structures and is
meant to mimic a NUMA-annotated data-structure/thread layout that the
`numa-clang-tool` compiler can then recursively specialize for maximum local
access.

## What it does

* `numa_BST_init` runs one **pinned** init thread per node (`thread_numa<0..3>`).
  Each builds `num_DS / 4` BSTs on its node using `numa<BinarySearchTree, K>`
  (so the trees live in that node's memory), gives each tree a lock, and
  prefills it with `keyspace/2` random keys.
* `BinarySearchTest` runs `num_threads / 4` **pinned** worker threads per node.
  Each worker loops for `duration` seconds doing:
  * **80%** — a `lookup` on one of its *own* node's trees (local access).
  * **20%** — a cross-node transaction (`remove` + `insert`) with its **ring
    neighbour** (0→1→2→3→0). Locks are always taken in ascending node order, so
    the ring cannot deadlock.
* Throughput is sampled every `interval` seconds and reported per node.

## Logical vs physical nodes

Node ids in the code are **logical**. `numaLib/numa_nodemap.hpp` maps each
logical id onto a **CPU-bearing physical node** (`logical % #cpu_nodes`),
skipping memory-only nodes, and **both** the allocator and the thread pinner use
it — so a node's threads and its memory always land on the same physical node.

Example: on a machine whose CPU-bearing nodes are `{0,1}` (with memory-only
nodes `{2,3}`), logical nodes `0,1,2,3` map to physical `0,1,0,1`. You can
override per id with `NUMA_NODE_MAP="2=0,3=1"`.

## How to compile

Requires the Unified Memory Framework and jemalloc pool libs to be built under
`$ROOT_DIR/unified-memory-framework/build/lib`.

```sh
make clean
make UMF=1 NUM_NODES=4 ROOT_DIR=$HOME/NUMATyping
```

* `NUM_NODES=4` (default) sets both the number of logical nodes the test spreads
  across (`-DNUM_NUMA_NODES`) and the number of allocator pools
  (`-DNUMA_NODE_NUM`).
* `UMF=1` links the jemalloc-backed UMF allocator. Without it, allocation falls
  back to `numa_alloc_onnode`.
* Optional: `JEMALLOC_ROOT=/path/to/jemalloc`, `DEBUG=1`.

> Note: the build does not track header dependencies. After editing any `.hpp`
> (including the numaLib headers), run `make clean` or `rm src/*.o` before
> rebuilding.

## How to run

Use the configuration from the paper's transactional benchmark (§6.3.1) on the
small dual-socket two-node machine: **1 million BSTs, 80 keys each, 80 threads,
80% local / 20% mixed transactions, run for 10 minutes** (~2 GB footprint):

```sh
numactl --cpunodebind=0,1 --membind=0,1 ./bin/datastructures \
    --th_config=numa --DS_config=numa --DS_name=bst \
    -n 1000000 -t 80 -D 600 -k 160 -i 10
```

How the paper's parameters map to the flags:

| Paper | Flag | Notes |
|-------|------|-------|
| 1,000,000 indices | `-n 1000000` | total BSTs, split `/4` across nodes (250k each) |
| 80 keys per index  | `-k 160`     | each tree is prefilled with `keyspace/2 = 80` keys |
| all 80 threads     | `-t 80`      | split `/4` across nodes (20 each; on a 2-CPU-node box that fills both sockets) |
| 10 minutes         | `-D 600`     | duration in seconds |
| 80% local / 20% mixed | (built in) | the worker loop's read/transaction split |

`-i 10` samples throughput every 10 s. For a quick smoke test, shrink the run,
e.g. `-n 8 -t 16 -D 4 -i 2 -k 2000`.

### Options

| Flag | Meaning |
|------|---------|
| `--th_config`  | `numa` (pin threads per node) or `regular` (unpinned) |
| `--DS_config`  | `numa` (place trees per node) or anything else (plain heap) |
| `--DS_name`    | data-structure name label (e.g. `bst`) |
| `-n, --num_DS` | total number of BSTs (split `/4` across nodes) |
| `-t, --num_threads` | total worker threads (split `/4` across nodes) |
| `-D, --duration`    | run length in seconds |
| `-i, --interval`    | sampling interval in seconds |
| `-k, --keyspace`    | key range; each tree is prefilled with `keyspace/2` keys |
| `-f, --run_freq`    | repeat the test this many times |
| `-x, --crossover`   | crossover parameter (currently forced to -1 in init) |

`num_threads` and `num_DS` are rounded down to a multiple of 4.

### Output

One CSV row per sampling interval:

```
date, time, DS_name, num_DS, num_threads, th_config, DS_config, duration,
keyspace, interval, ops_node0, ops_node1, ops_node2, ops_node3, total_ops
```

## Layout

* `src/main.cpp` — option parsing; spawns the per-node init and worker threads
  with explicit `thread_numa<0..3>`; prints the per-node CSV.
* `src/TestSuite.cpp` — `global_init`, `numa_BST_init`, `BinarySearchTest`, and
  the per-node named globals (`BSTs0..3`, `BST_lk0..3`, `ops0..3`).
* `include/` — the data-structure classes (`BinarySearch.hpp`, etc.). This is
  the directory the `numa-clang-tool` recursively types: it finds the literal
  `new numa<BinarySearchTree, K>` expressions and generates the
  `template<> class numa<BinarySearchTree, K>{…}` specializations into
  `BinarySearch.hpp`.
* `Makefile` — compile/link flags. **Set `ROOT_DIR` to your NUMATyping checkout.**
