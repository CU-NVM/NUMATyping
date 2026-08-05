# UMF allocator changes — 2026-07-20

What changed in the UMF/jemalloc allocator, why, and what it bought. Machine:
stormbreaker, 4 NUMA nodes (0/1 have CPUs, 2/3 are memory-only), 80 threads,
jemalloc 5.2.1, THP = `madvise`.

## TL;DR

The per-thread tcaches we added were all being created on the main thread, which
serialised every `free()` through one arena's bin locks. Fixing that plus two
smaller things took the allocator from **408 ns to 43 ns per alloc/free pair at
20 threads** and closed the numa/numa vs numa/regular gap on the BST benchmark
**from 11.3% behind to 2.5% behind**.

## Background: what our fork already did

Against upstream `oneapi-src/unified-memory-framework` v0.9.0 we had three
changes, all in the jemalloc pool:

1. **Per-thread tcaches** instead of upstream's `MALLOCX_TCACHE_NONE`.
2. **160 arenas per pool**, with the arena rotated on every allocation.
3. **`umfFastJemallocMalloc/Free`** — inline fast paths in the public header that
   skip the `umf_memory_pool_ops_t` vtable.

Change 1 was the important one and it stays. Upstream's `TCACHE_NONE` costs
**42,305 ns/alloc at 20 threads** — every single allocation takes an arena bin
lock. Do not go back to it.

## The bug: tcaches were bound to the wrong arena

`mallctl("tcache.create")` binds the new tcache to **whatever arena the calling
thread is currently using**, permanently. There is no way to rebind it later.

`op_initialize()` created all `MAX_JEMALLOC_THREADS` tcaches up front, in a loop,
on the main thread. So every worker thread's tcache was bound to the *main
thread's* arena, and every `free()` flushed into that one arena's bins.

Measured (32 B objects, steady state, pinned threads), before the fix:

| threads | alloc | free |
|---|---|---|
| 1 | 32.0 ns | 25.8 ns |
| 8 | 30.8 ns | 38.9 ns |
| 20 | 30.8 ns | **377.4 ns** |

Alloc is flat; free explodes. This is invisible in any single-threaded
micro-benchmark, which is why it survived so long.

### The fix

Create each thread's tcache **lazily, in that thread**, after pointing
`thread.arena` at the arena that thread will allocate from — so the tcache and
the arena match and each thread flushes into its own bins. See
`umfJemallocBindThread()` in `src/pool/pool_jemalloc.c`.

One trap: **`-ljemalloc` interposes the global `malloc`**, so `thread.arena` also
governs ordinary non-numa-typed allocations in that thread. If you set it and
walk away, plain `malloc()` silently starts returning node-bound memory. We save
it and put it back once the tcache has taken its snapshot.

### The tempting wrong fix

Dropping to jemalloc's automatic tcache (`dallocx(ptr, 0)`) is faster still
(20.5 / 18.2 ns) — and **wrong**. jemalloc's tcache bins are per size class, not
per arena, so a node-1 request gets handed whatever the bin happens to hold.
Measured: **200 of 4096 node-1 allocations landed on node 0.** Our entire result
depends on that not happening. There is a regression test for this in
`allocator_test/src/verify_allocator.cpp` (see "How to re-measure" below).

A TLS magazine (batch freelist) layered on top was also tried: **slower**
(24.6 vs 21.7 ns). jemalloc's tcache already is one. Don't bother.

## What was changed

### `unified-memory-framework/src/pool/pool_jemalloc.c` + `include/umf/pools/pool_jemalloc.h`

- **Tcaches are created lazily per thread** (`umfJemallocBindThread`), bound to
  that thread's arena, with `thread.arena` saved and restored. Replaces the
  `tcaches[MAX_JEMALLOC_THREADS]` array that used to live in
  `jemalloc_memory_pool_t`.
- **A `pthread_key` destructor** (`destroy_thread_tcaches`) reclaims them on
  thread exit. Explicit tcaches are not garbage collected by jemalloc, so
  without this a thread-churning process leaks one tcache per (thread, pool).
- **No more arena rotation.** Each thread gets one fixed arena,
  `arena_index + (tid % num_arenas)`. The rotation bought nothing — `tid()`
  already seeded a distinct starting arena per thread — and cost ~9 ns/alloc by
  scattering tcache refills across cold arenas. (~5 ns of the win; the rest is
  the flag caching below.)
- **Flags are cached in TLS.** `umf_je_alloc_flags[slot]` / `umf_je_free_flags[slot]`
  are computed once per (thread, pool). The fast path is now a TLS bit test plus
  `mallocx()`. New `umfFastJemallocMallocSlot(slot, size)` takes the slot
  directly; for `numa<T,k>` the slot is `k`, a compile-time constant, so the
  whole thing folds to `mallocx(size, <constant-indexed TLS word>)`.
- **`num_arenas` now defaults to the online CPU count** (was hardcoded 160),
  overridable with `UMF_JE_ARENAS_PER_POOL`. One arena per thread is all the
  scheme needs; 160 was just RSS and page-table bloat.
- **`op_finalize` fixed.** It destroyed `arena_index` `num_arenas` times instead
  of `arena_index + i`, leaking 159 arenas per pool.
- **`MAX_JEMALLOC_THREADS` is gone.** It was a fixed 200-thread ceiling with no
  bounds check — thread 200 walked off the end of `tcaches[]`. It was also *200*
  in the built library but *256* in `numaLib/` and the `Array*` copies, i.e. two
  different `sizeof(jemalloc_memory_pool_t)` on either side of the ABI. There is
  now no thread limit; `UMF_JE_MAX_POOLS` (16) caps NUMA nodes instead, with a
  `static_assert`.

### `numaLib/umf_numa_allocator.hpp`

- **`umf_alloc` / `umf_free` are now `inline`.** They were ordinary out-of-line
  definitions in a header — a call per allocation, and no constant folding.
- They call the `*Slot` fast path directly, skipping the
  `hPool->pool_priv->pool_slot` chase.
- **`jemalloc_pool` / `NUMA_HANDLES` are `inline` globals** and `umf_alloc_init()`
  is guarded, so multiple TUs including this header share one set of pools
  instead of racing to create duplicates. The `__attribute__((constructor))`
  moved to an anonymous-namespace shim so pools are still up before `main()`.

### `numa-clang-tool/src/transformer/RecursiveNumaTyper.cc`

- **`operator new[]` allocated `sizeof(T)` regardless of the element count** — a
  heap overflow on any array-new of a numa-typed object. Now uses `sz`, which is
  the total byte count the compiler asks for. `operator new` switched to `sz`
  too (equivalent, but no reason for the two to differ).
- Already-generated headers under `Output/` were patched the same way, but
  **re-run `numafy.py` to regenerate them properly.**

### `DataStructureTests/src/TestSuite.cpp` (and the other copies)

Unrelated pre-existing crash, found while trying to measure. `duration/interval`
truncates to 0 whenever `duration < interval` — e.g. `-D 5` with the default
`-i 20` — leaving `localOps`/`globalOps` empty and every access out of bounds.
**This segfaults at HEAD**, before any of the allocator work. Fixed with
`numIntervals()` (rounds up, floor of 1) plus a bound on `intervalIdx`.

### Header copies

`umf_numa_allocator.hpp`, `numa_nodemap.hpp` and `pool_jemalloc.h` were
duplicated across ten suite directories and had drifted (DataStructureTests still
had a pre-`numa_nodemap` copy with hardcoded identity node mapping, which shadows
`numaLib/` because `-Iinclude/` comes first). All synced. **This will drift
again** — the real fix is to drop the per-suite copies and rely on `-InumaLib`.

## Results

Allocator micro-benchmark, 32 B objects, steady state, ns per operation:

| threads | | alloc | free | pair |
|---|---|---|---|---|
| 1 | before | 32.0 | 25.8 | 57.8 |
| | **after** | **22.2** | **20.4** | **42.6** |
| | jemalloc `malloc` | 16.7 | 16.7 | 33.4 |
| 20 | before | 30.8 | 377.4 | 408.2 |
| | **after** | **22.7** | **20.4** | **43.1** |
| | jemalloc `malloc` | 16.4 | 16.6 | 33.1 |

Flat across thread counts now, and within 1.3× of a plain `malloc`/`free` pair.

BST benchmark, `--num_DS=4 --duration=10 --keyspace=200000 --num_threads=32`,
mean of 3 runs (spread was under 1%):

| config | before | after | |
|---|---|---|---|
| numa / numa | 1,868,800 | **2,043,221** | **+9.3%** |
| numa / regular | 2,105,685 | 2,096,128 | unchanged |

numa/numa went from **11.3% behind** numa/regular to **2.5% behind**.

### YCSB, write-heavy (added after the allocator work)

Workload A (50% read / 50% update), `--mix=uniform --hash=mix`, 80 threads,
1000 tables, 30011 buckets, 100M keys, 128 B payload, 300 s per config, 20 s
intervals, AutoNUMA **on**. Steady-state = mean of intervals from t=60 s (the
first two are warm-up: both configs start near 44 M ops/s and settle).

| config | steady throughput | total ops |
|---|---|---|
| numa / numa | **38.42 M ops/s** | 11,660,763,136 |
| numa / regular | 36.76 M ops/s | 11,186,569,216 |
| | **+4.5%** | |

Unlike the BST, this one is a clean win, and the separation is unambiguous:
every numa/numa interval (38.24–38.55) beats every numa/regular interval
(36.46–36.91) — the two ranges do not overlap. Node 0 and node 1 stay balanced
throughout in both configs.

Caveat: one run per config. Interval-level stability is excellent (<1.2% spread
within a run) but that does not capture run-to-run variance from a different
prefill and page placement. Repeat before it goes in the paper.

Reproduce:

```shell
python3 scripts/run.py --bench ycsb --name wheavy_A \
  --workload "A-50-50-50,A-100-0-50" --mix uniform --hash mix \
  --threads 80 --tables 1000 --buckets 30011 --keys 100000000 \
  --duration 300 --payload 128 --interval 20
```

Note the CSV columns are **cumulative** ops per interval, not per-interval rates
— difference consecutive rows to get throughput.

Two build prerequisites that were not in place:

- `numa-clang-tool` needs a `libLLVM*.so` its CMake could not find (only static
  LLVM libs are installed here). `libclang-cpp.so` has LLVM linked in
  statically, so `cmake .. -DLLVM_SHARED_LIB=/usr/local/lib/libclang-cpp.so`
  builds it.
- `Output/ycsb` did not exist and is not in git; `scripts/numafy.py --umf=1 ycsb`
  generates it.

### Speedup vs `numa_alloc_onnode`

`numa_alloc_onnode` is the libnuma call in the `#else` (non-UMF) branch of the
generated `numa<T,k>::operator new` — the naive way to get node-local memory. It
`mmap`s + `mbind`s + `munmap`s every object, so it is slow, doesn't scale (the
per-process `mmap_lock` serialises threads), and wastes a whole 4 KB page per
small object. Full alloc + first-touch + free cycle, 32 B objects, node 0,
measured by `allocator_test/bin/throughput_compare` (ns/op, and speedup):

| threads | `numa_alloc_onnode` | advisor's UMF | this UMF | advisor vs numa | **this vs numa** |
|--:|--:|--:|--:|--:|--:|
| 1 | 5,394 | 42 | 26 | 127× | **205×** |
| 8 | 88,257 | 50 | 33 | 1,758× | **2,683×** |
| 20 | 366,142 | 345 | 45 | 1,061× | **8,191×** |

So both UMF versions beat `numa_alloc_onnode` by 100×–8000×. The advisor-vs-this
gap at 20 threads (1,061× vs 8,191×) is the free-path fix: the advisor's own free
stalls at ~318 ns/op there, this one holds ~45 ns.

Reminder from above: in the actual `UMF=1` benchmarks, numa/**regular** is *not*
`numa_alloc_onnode` — it's interposed jemalloc `malloc`, which is fast. So this
table answers "was building a real pooled allocator worth it vs. the naive
libnuma call" (emphatically yes), a different question from the few-percent
numa/numa-vs-numa/regular margin.

## Honest read

The allocator handicap is gone — that was worth ~9% and it was the thing making
numa/numa lose. **On YCSB write-heavy, numa/numa now wins by 4.5%.** On the BST
it reached parity but not a win.

The BST and YCSB disagreeing is informative, not noise. On the BST the `txn%4`
cases in `BinarySearchTest` deliberately touch both nodes' trees, so a large
share of the work is cross-node by construction, and numa/regular already gets
good locality free from first-touch with pinned threads — there is little
locality left for numa-typing to win. YCSB's mixed workload spec
(`A-50-50-50,A-100-0-50`: half the threads 100% local) leaves real locality on
the table, and numa-typing collects it.

That suggests the BST result is a property of that benchmark's access pattern,
not of the allocator, and that the paper's story is better told on workloads
where placement genuinely differs from first-touch.

Things worth trying next, roughly in order of expected value:

1. **Huge pages.** Nothing gets THP today — the box is `madvise` and UMF's OS
   provider never calls `MADV_HUGEPAGE`, so the BST is 100% 4 KB pages. A
   `madvise(addr, size, MADV_HUGEPAGE)` after the `mbind` in `os_alloc()`
   (`src/provider/provider_os_memory.c`) should help the pointer chase, and
   should help numa/numa more than numa/regular because our extents are large
   and contiguous while the regular heap is fragmented. Needs its own
   measurement pass — mbind/THP interaction is worth checking, and extents must
   be 2 MB aligned and sized for it to apply at all.
2. **A workload where placement actually differs from first-touch.** As long as
   the allocating thread is the one that reads the data, first-touch gives
   numa/regular the same locality for free. The reverse-numa configurations are
   the interesting comparison.
3. **`-flto`** on the benchmark builds, so the now-`inline` `umf_alloc` folds
   into the generated `operator new`.

## How to re-measure

The tests live in `allocator_test/` and are built by `make UMF=1`:

- **`bin/verify_allocator`** — ns/op vs thread count (catches a free-path
  regression) plus three NUMA placement checks (single-thread interleaved,
  plain-`malloc`-still-unbound, 8-thread interleaved). Exits non-zero on failure.
  Run this after any change to the jemalloc pool.
- **`bin/throughput_compare`** — the UMF-vs-`numa_alloc_onnode` speedup sweep at
  1/8/20 threads (the numbers in "Speedup vs numa_alloc_onnode" below).

```shell
cd allocator_test && make UMF=1 ROOT_DIR=$HOME/NUMATyping-umfAlloc
numactl --cpunodebind=0,1 --membind=0,1 ./bin/verify_allocator
numactl --cpunodebind=0,1 --membind=0,1 ./bin/throughput_compare
```

The placement checks in `verify_allocator` are the real regression test for the
tcache isolation property — keep them green.

Rebuild after any change here:

```shell
cd unified-memory-framework/build && make -j16
cd ../../Output/DataStructureTests && make UMF=1 ROOT_DIR=$HOME/NUMATyping-umfAlloc
```
