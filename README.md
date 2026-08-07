# NUMATyping

**A C++ type extension and introspective-typing compiler for NUMA-aware data structures.**

Annotate a data structure with `numa<T, k>` and a thread with `thread_numa<k>`; a Clang-based source-to-source compiler then recursively rewrites the type so that *every* sub-object (down to each tree node) is allocated on NUMA node `k`, co-located with the threads that use it.

---

## Overview

On a multi-socket (NUMA) machine, remote memory accesses cost roughly 1.5x–3x a local access. Achieving locality by hand is tedious and error-prone. NUMATyping turns it into a type annotation:

| You write | It means |
|---|---|
| `numa<BinarySearchTree, 2>` | allocate this tree (and, after transformation, *every node inside it*) on NUMA node 2 |
| `thread_numa<2>` | run this thread pinned to node 2's CPUs |

A plain `numa<T,k>` only pins the *outer* object. The `numa-clang-tool` compiler reads your code, finds these annotations, and generates per-node specializations that recursively pin member types as well, so a tree's millions of nodes all land on the correct node for the entire run, not merely at first-touch. On a two-node machine this typically yields roughly **12–13% throughput over thread-pinning alone, and about 28% over a NUMA-unaware baseline** (more on larger machines).

---

## Repository layout

| Path | Description |
|---|---|
| `numaLib/` | Header-only type library (`numa<T,k>`, `thread_numa<k>`, allocator, node map). |
| `numa-clang-tool/` | The introspective-typing compiler (Clang LibTooling). |
| `unified-memory-framework/` | UMF — the jemalloc-backed, NUMA-bound allocator backend. |
| `numafy.py` | Driver that runs the compiler over a benchmark and writes the transformed code to `Output/`. |
| `Output/` | Transformed (numafied) copies of each benchmark. |
| `DataStructureTests/` | BST transactional benchmark, two NUMA nodes. |
| `DataStructureTests_four/` | The same benchmark spread across four logical nodes. |
| `ycsb/` | NUMA-adapted Yahoo! Cloud Serving Benchmark. |
| `Histogram/`, `Array/`, `Array_lk_free/`, `allocator_test/` | Additional micro-benchmarks. |
| `run*.py`, `perf*.py` | Experiment and profiling drivers (build, run, CSV, graphs). |
| `Result/`, `Graphs/` | CSV results and plots, split into `AN_on/` and `AN_off/`. |

### The core library (`numaLib/`)

| Header | Role |
|---|---|
| `numatype.hpp` | `numa<T, NodeID>` — the NUMA-placing type wrapper plus `NumaAllocator`. |
| `numathreads.hpp` | `thread_numa<NodeID>` — a `std::thread` pinned to node `NodeID`'s CPUs. |
| `numa_nodemap.hpp` | Maps a *logical* node id to a *CPU-bearing physical* node (`logical % #cpu_nodes`), skipping memory-only nodes. Shared by the allocator and the thread pinner so data and threads always agree. Override with `NUMA_NODE_MAP="2=0,3=1"`. |
| `umf_numa_allocator.hpp` | One jemalloc pool per node (via UMF), bound with `MPOL_BIND`. |

---

## The allocator rework (2026-07, merged 2026-08)

The jemalloc-backed UMF pool was reworked in commit `a45515bd` (merged from the
`NUMATyping-umfAlloc` fork). Full rationale and measurements: [`allocator.md`](allocator.md).

Two companion pages *(private Claude artifacts — visible to this account unless shared)*:

- **[Why the allocator changed](https://claude.ai/code/artifact/0dd4446d-090d-43b5-b86b-225422e61cf9)** —
  arenas, thread caches and the binding bug explained from scratch, with the reasoning behind each
  individual change. Start here.
- **[The staged diff](https://claude.ai/code/artifact/ec7ccf72-2ab6-4b6d-b3aa-728beda37801)** —
  every changed file, red/green, grouped by role.

**Before** (the fork's original customization of upstream UMF v0.9.0):

- Per-thread tcaches instead of upstream's `MALLOCX_TCACHE_NONE` (upstream costs
  ~42,000 ns/alloc at 20 threads -- every allocation takes an arena bin lock).
- **But** all tcaches were created up front on the *main* thread, permanently binding
  them to the main thread's arena: every `free()` from every worker flushed into one
  arena's bins. Free cost exploded from 26 ns (1 thread) to **377 ns (20 threads)**.
- 160 arenas per pool, rotated every allocation (~9 ns/alloc of cold-arena refills).
- `MAX_JEMALLOC_THREADS` fixed at 200 with no bounds check -- and set to *256* in some
  header copies, giving two different pool-struct sizes across the ABI.
- `op_finalize` leaked 159 of the 160 arenas per pool.
- The clang transformer's generated `operator new[]` allocated `sizeof(T)` regardless
  of element count -- a heap overflow on any array-new of a numa-typed class.

**After** (`unified-memory-framework/src/pool/pool_jemalloc.c`, `numaLib/`):

- Tcaches are created **lazily, in the owning thread**, bound to that thread's fixed
  arena (`umfJemallocBindThread`); a `pthread_key` destructor reclaims them on exit.
- No arena rotation; one fixed arena per thread; allocation flags cached in TLS
  (`umfFastJemallocMallocSlot` -- for `numa<T,k>` the slot is a compile-time constant).
- Arena count defaults to the online CPU count; no thread ceiling; leaks fixed;
  header copies synced (including `ycsb/include/`, missed by the fork itself);
  transformer emits `sz` so array-new is correct.

**Measured effect** (stormbreaker, 32 B objects; see `allocator_test/`):

| metric | before | after |
|---|---|---|
| alloc+free pair, 20 threads | 408 ns | **43 ns** (flat 1->20 threads) |
| BST numa/numa vs numa/regular | -11.3% | -2.5% |
| node-1 allocations on wrong node | -- | **0 / 4096** (`verify_allocator`) |
| vs `numa_alloc_onnode`, 20 threads | 1,061x | **7,142x** |

After touching the pool, always rebuild `unified-memory-framework/build` **and** re-run
`allocator_test/bin/verify_allocator` -- it guards the free-path flatness and the
placement property the whole approach depends on. Note the static libs are build
artifacts: a source merge alone does *not* update an existing `build/lib/*.a`
(campaigns 01-03 unknowingly ran the old allocator this way; see the corrected
provenance notes in `Campaigns/ycsb/campaign0*/manifest.md`).

---

## Prerequisites

- Clang/LLVM 20 or newer (the tool links `libclang`/`libLLVM`) and CMake
- `libnuma`, `hwloc`, `jemalloc` (development headers)
- A C++20 compiler (`clang++`)
- On NERSC machines only: the `python` module

If CMake cannot find LLVM/Clang, build the tool with `-DHELP=ON` for guidance, and locate libraries with, for example, `find /usr -name 'libLLVM*.so'`.

---

## Build (one-time setup)

**0. NERSC only** — load modules and environment:
```shell
module load python
eval $(python3 load.py)   # loads required modules
eval $(python3 env.py)    # sets environment variables
```

**1. Build the compiler tool:**
```shell
cd numa-clang-tool && mkdir -p build && cd build
cmake ..
cmake --build .
cd ../..
```

**2. Build the UMF allocator:**
```shell
cd unified-memory-framework && mkdir -p build && cd build
cmake ..
cmake --build .
cd ../..
```

---

## Numafy: transforming a benchmark

`numafy.py` stages a suite into `numa-clang-tool/input/`, runs the two compiler passes, and writes the result to `Output/<SUITE>/`.

```shell
python3 numafy.py --ROOT_DIR=$HOME/NUMATyping <SUITE> [--umf=1] [--jemalloc-root=PATH]
```

- `<SUITE>` is one of `DataStructureTests`, `DataStructureTests_four`, `ycsb`, `Histogram`, `Array`.
- `--ROOT_DIR` defaults to `$HOME/NUMATyping`; run with `--help` for all options.

The two passes:

1. **`recurse`** — finds `new numa<T, k>` sites and generates `template<> class numa<T, k>{...}` specializations in the headers. It recurses into member types, so `numa<BinarySearchTree,k>` also produces `numa<BinaryNode,k>`; the entire structure is pinned.
2. **`cast`** — inserts the `reinterpret_cast`s that wire the specialized types together.

The transformed, ready-to-compile suite is written to `Output/<SUITE>/`.

---

## Benchmarks: compile and run

Every suite shares the same flags — `--th_config` (`numa`/`regular`), `--DS_config` (`numa`/`regular`) — and a `Makefile` taking `UMF=1` and `ROOT_DIR`. Bind to the CPU nodes with `numactl --cpunodebind=0,1 --membind=0,1`.

### Data-structure (BST) benchmark

A large array of `BinarySearchTree`s, each under its own lock; threads perform 80% local lookups and 20% cross-node transactions, reporting per-interval throughput.

```shell
# Native (untransformed) build and run
cd Output/DataStructureTests        # or DataStructureTests_four (four nodes, NUM_NODES=4)
make clean && make UMF=1 ROOT_DIR=$HOME/NUMATyping
numactl --cpunodebind=0,1 --membind=0,1 ./bin/datastructures \
    --th_config=numa --DS_config=numa --DS_name=bst \
    -n 1000000 -t 80 -D 600 -k 80 -i 20
```

Each output row is a per-interval CSV record: `date, time, DS_name, num_DS, num_threads, th_config, DS_config, elapsed, keyspace, interval, Op0..OpN, TotalOps`.

### YCSB

```shell
cd Output/ycsb        # or ./ycsb for the native version
make clean && make UMF=1 ROOT_DIR=$HOME/NUMATyping
numactl --cpunodebind=0,1 --membind=0,1 ./bin/ycsb \
    --th_config=numa --DS_config=numa -t 80 -b 1333 --w=D -u 120 -k 10000000 --l=80-20 -i 10 -a 1000
```

### Sweeping configurations with `meta.py`

Each suite ships a `meta.py` that runs a command over all combinations of swept options (colon-separated values):

```shell
numactl --cpunodebind=0,1 --membind=0,1 python3 meta.py ./bin/datastructures \
    --meta th_config:numa:regular --meta DS_config:numa:regular \
    --meta n:1000000 --meta t:80 --meta D:600 --meta k:80 --meta i:20
```

`--meta th_config:numa:regular` runs both values; pin a single value (`th_config:numa`) to fix it. Use `--printOnly` to preview commands, and `--help` for details.

---

## Experiment runners (build, run, CSV, graphs)

These scripts wrap the full lifecycle: optional numafy, compile, configuration sweep, and writing results to `Result/<AN_on|AN_off>/` (with plots to `Graphs/`).

| Script | Drives |
|---|---|
| `runExperiments.py` | the BST data-structure suite (`--suite DataStructureTests` / `DataStructureTests_four`) |
| `runYCSB.py` | the YCSB suite |
| `perfBST.py`, `perfYCSB.py` | `perf`-based profiling runs |
| `campaign.py`, `run.py` | git-gated archival sweeps / quick scratch runs (see `Campaigns/`) |
| `an_comparison.py` | AutoNUMA comparison tables and figures for a campaign |

**Example — BST, numa/numa vs numa/regular, AutoNUMA off, four logical nodes:**
```shell
python3 runExperiments.py --DS bst --ROOT_DIR $HOME/NUMATyping --UMF --AN 0 \
    --suite DataStructureTests_four --duration 600 --thConfig numa --dsConfig numa:regular \
    --nodes 4 --numDS 1000000 --numKeys 80 \
    --outfile bst_keyspace_threads_4_experiments.csv
```

**Example — YCSB with graphs:**
```shell
python3 runYCSB.py --ROOT_DIR=$HOME/NUMATyping --numafy --AN=1 --UMF --graph \
    --workload A-50-50-50,D-100-0-50 A-50-50-50,A-100-0-50
```

Useful flags (see each script's `--help`): `--numafy` (transform first), `--UMF`, `--AN {0,1}` (AutoNUMA label plus numactl `--balancing`), `--graph`, `--perlmutter`, `--numDS`, `--numKeys`, and, for `runExperiments.py`, `--suite`, `--duration`, `--thConfig`/`--dsConfig`, `--nodes`, `--outfile`. Per-run output files are append-safe, so existing results are preserved.

---

## Measurement notes

**AutoNUMA.** The Linux page-migration daemon partly closes the locality gap, so experiments compare it on versus off. The `--AN` flag only sets the label and the `numactl` option; to truly disable the kernel feature:

```shell
sudo sh -c 'echo 0 > /proc/sys/kernel/numa_balancing'   # 0 = off, 1 = on
cat /proc/sys/kernel/numa_balancing
```

Results are written under `Result/AN_off/` or `Result/AN_on/` accordingly.

**Logical versus physical nodes.** Node ids in code are *logical*. `numa_nodemap.hpp` maps them onto CPU-bearing physical nodes only, so on a machine with CPU nodes `{0,1}` and memory-only nodes `{2,3}`, logical `0,1,2,3` map to physical `0,1,0,1`. The allocator and the thread pinner use the same map, so memory and threads never drift apart.

---

## Results

CSV time-series are written to `Result/AN_off/` and `Result/AN_on/` (for example, `bst_<numDS>_<numKeys>_experiments.csv`), with per-node operation columns and a total, sampled each interval. Pass `--graph` to a runner to render plots into `Graphs/`.

Representative result (two-node machine, 80 threads, AutoNUMA off): `numa` threads with `numa` data is roughly **12% faster** than `numa` threads with `regular` data, and about **28% faster** than the NUMA-unaware `regular`/`regular` baseline. The `numa`/`numa` throughput stays flat while `numa`/`regular` degrades over time as first-touch locality erodes.
