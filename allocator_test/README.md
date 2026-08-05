# allocator_test

Three allocator test programs. `make UMF=1` builds all of them into `./bin/`:

| Binary | What it is |
|--------|-----------|
| `allocator_test`     | single-threaded microbenchmark of four allocators (below) |
| `verify_allocator`   | **regression test** — fast-path speed vs. thread count + NUMA placement correctness. Exits non-zero on failure. |
| `throughput_compare` | multithreaded sweep: how many times faster the UMF pool is than `numa_alloc_onnode`, at 1/8/20 threads. |

`verify_allocator` and `throughput_compare` were added alongside the UMF jemalloc
pool changes (see `../allocator.md`); they need `UMF=1` and a 2-node machine.
`allocator_test` is the original microbenchmark below.

Quick start (from a 2-node machine, UMF already built):

```sh
make UMF=1 ROOT_DIR=$HOME/NUMATyping
numactl --cpunodebind=0,1 --membind=0,1 ./bin/verify_allocator      # PASS/FAIL
numactl --cpunodebind=0,1 --membind=0,1 ./bin/throughput_compare    # speedup table
```

`verify_allocator` is the one to run after any change to the jemalloc pool: it
fails loudly if the free path regresses with thread count, or if a node-k
allocation ever lands on the wrong node.

---

## allocator_test (the original microbenchmark)

A small single-threaded microbenchmark that compares four allocators for many
small, same-node allocations. Its purpose is to show why a pooled NUMA allocator
(UMF + jemalloc) is preferable to raw `numa_alloc_onnode` when allocating large
numbers of small objects on a specific NUMA node.

## What it measures

For a chosen allocator, it allocates `count` objects of `size` bytes, touches the
first byte of each (to force a physical page to back it), and reports:

- **Time Elapsed** — wall-clock time for the allocation loop.
- **RSS Growth** — resident memory the process gained during the loop.
- **Est. Overhead** — `(RSS growth - requested bytes) / count`, i.e. bytes of
  real memory consumed per object beyond what was asked for.

The four allocators:

| Mode | Allocator |
|------|-----------|
| `malloc`   | the system `malloc` (whatever it resolves to) |
| `jemalloc` | jemalloc's explicit `mallocx` / `dallocx` |
| `numa`     | `numa_alloc_onnode(size, 0)` — libnuma, pinned to NUMA node 0 |
| `umf`      | `umf_alloc(0, size, 8)` — the UMF jemalloc pool bound to node 0 |

Each invocation runs exactly **one** mode (run the binary several times to compare).

Note: this build links `-ljemalloc`, which interposes `malloc`, so on most setups
the `malloc` and `jemalloc` modes report the same allocator. Run both to confirm —
they diverge only where jemalloc is not interposing the system `malloc` (then
`malloc` is glibc).

## Prerequisites

- The Unified Memory Framework built under
  `$ROOT_DIR/unified-memory-framework/build/lib` (for `UMF=1`).
- `clang++` (C++20), `libnuma`, `hwloc`, `jemalloc`.

## Build

This test only allocates on node 0, so no node-count macros are needed.

```sh
make clean
make UMF=1 ROOT_DIR=$HOME/NUMATyping
```

This produces the executable `./bin/allocator_test`.

## Run

```sh
./bin/allocator_test <malloc|jemalloc|umf|numa> [count] [size]
```

- `count` — number of objects (default 100000)
- `size`  — bytes per object (default 64)

Binding to the target node keeps the comparison clean:

```sh
numactl --cpunodebind=0 --membind=0 ./bin/allocator_test malloc   100000 64
numactl --cpunodebind=0 --membind=0 ./bin/allocator_test jemalloc 100000 64
numactl --cpunodebind=0 --membind=0 ./bin/allocator_test umf      100000 64
numactl --cpunodebind=0 --membind=0 ./bin/allocator_test numa     100000 64
```
