# temp.md — stormbreaker → Perlmutter handoff (delete once acted on)

Short-lived note from the stormbreaker side, written 2026-08-24 before pushing
two commits. Read this before running anything, then delete the file.

## What landed in this push

| commit | file | risk to you |
|---|---|---|
| `adae5a5c` | `numa-clang-tool/src/CMakeLists.txt` | **verify this one** |
| `ebadd4b5` | `scripts/campaign.py`, `stormbreaker.md` | none to the build |

### 1. `adae5a5c` — libLLVM discovery made optional

Your shape detection from `473ce150` is correct and I did not change its logic.
The problem was **ordering**: `find_shared_lib_dynamic(LLVM_SHARED_LIB …)`
`FATAL_ERROR`s when no `libLLVM*.so` matches, and that ran *before* the
detection could decide whether one was needed. stormbreaker's `/usr/local`
trunk LLVM ships only static archives plus an LLVM-embedding `libclang-cpp.so`
— there is no `libLLVM*.so` at all — so configure died before reaching 1b.

Fix: `libclang-cpp` stays required; `libLLVM.so` is now looked up without
failing, and the error moved to *after* detection, firing only if
`LINK_LLVM_DYLIB=ON` **and** nothing was found.

**Why this should be a no-op for you:** your clang-tool already built, so the
old fatal lookup must have found a `libLLVM*.so` in your `LLVM_LIB_DIR`. The
optional finder runs the identical glob and yields the identical path.

**Please verify cheaply before anything else — it takes ~2 minutes:**

```shell
bash scripts/configure_machine.sh perlmutter-cpu --stages=clangtool
```

Expect `libclang-cpp.so depends on libLLVM.so -- linking both`. Seeing that
line means detection ran rather than failing early, which is the whole point.

If it does break: `-DLLVM_SHARED_LIB=/path/to/libLLVM.so` and
`-DLINK_LLVM_DYLIB=ON` are both still honoured, or revert that one file and
tell me. Do not work around it by making the lookup fatal again — that is what
broke stormbreaker.

### 2. `ebadd4b5` — THP and numa_balancing in the manifest

`machine_info()` now captures `THP`, `THP defrag`, and `numa_balancing`, and
they appear in the manifest identity block **and in every run line**. Manifests
you generate will have fields older ones lack; that is expected, not drift.

Per-run recording matters because a campaign's two AN runs are separate
invocations — on stormbreaker campaign01's were 23 hours apart — so a
mid-campaign change is exactly what needs catching. THP is runtime-set and
reverts on reboot, so on a machine you cannot control, just record it.

Also repoints the missing-`machine.env` hint at
`configure_machine.sh --stages=topo` instead of `detect_machine.sh`.

## Things that are NOT problems (so you don't "fix" them)

- **`--an-mode` was already there.** It shipped in `ee8b4619`, before your work.
  Your `ycsb_campaign.slurm` calling `--an-mode $AN_MODE` works as written; the
  commits above are not a prerequisite for it. (My commit message originally
  claimed to add it — amended.)
- **`machine.env` has two writers**, `machine_profile.sh` and
  `detect_machine.sh`. That is deliberate and documented in
  `machine_profile.sh`, and `configure_machine.sh` skips the duplicate. Leave
  the arrangement alone — but if you edit topology logic, check whether the
  change belongs in both, because this is where drift would start.
- **`machine.env` is gitignored.** Yours will not travel; regenerate with the
  topo stage. This is required, not incidental: `campaign.py` refuses a dirty
  tree, so a tracked `machine.env` would block every campaign.

## Verified on stormbreaker after the push contents

Full `configure_machine.sh stormbreaker` completes all four stages:
`NUMA_NODE_ORDER=0,1`, `PARTITION_THREADS=80`, bind `0,1`; clang-tool builds,
links only `libclang-cpp.so.21.0git`, exits 0 with no `munmap_chunk` teardown
abort; UMF produces both static libs; ycsb numafies, compiles, and runs with
work balanced across both partitions.

## Ownership

`perlmutter.md` is yours, `stormbreaker.md` is mine — neither side edits the
other's. Anything you learn that applies to both machines goes in your file
with a note, and I will mirror it.

If you change anything under `scripts/` that stormbreaker also uses
(`campaign.py`, `benchmarks.py`, `numafy.py`, `detect_machine.sh`,
`numa-clang-tool/`, `numaLib/`), say so in your commit message so it gets
re-verified on stormbreaker. Those are the shared surfaces where one machine
can silently break the other — the libLLVM ordering above is exactly that class
of bug, and it was caught only because the build was re-run here.
