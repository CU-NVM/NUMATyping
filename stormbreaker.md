# stormbreaker.md — reference machine profile and porting contract

**Purpose.** This file describes the machine the NUMATyping repo was developed and
validated on (`stormbreaker`), and the assumptions the build and run scripts
inherit from it. It exists so that an agent configuring the repo on a *different*
machine (Perlmutter, or anything else) knows which values are machine-specific,
which are load-bearing, and what must not be changed.

**Contract for the porting agent:** treat this file as read-only. It documents the
reference machine, not the target. Do not "fix" the values here to match a new
machine. Add machine-specific handling behind a machine name/detection layer
instead, and leave every stormbreaker path working exactly as it does today.

Captured 2026-08-23 at repo commit `e4e5b86a`.

---

## 1. Hardware

```
hostname   stormbreaker
kernel     6.11.5cxleak  (custom CXL kernel — see §1.2)
OS         Ubuntu 22.04.5 LTS (jammy)
CPU        2 × Intel Xeon Silver 4416+ (Sapphire Rapids)
           20 cores/socket, 2 threads/core = 80 logical CPUs
           800–3900 MHz; L3 75 MiB (2 instances); AVX-512, AMX
RAM        331 GiB total
swap       1 GiB
```

### 1.1 NUMA topology

```
available: 4 nodes (0-3)
node 0 cpus: 0-19,40-59      size: 128611 MB     <- socket 0, DRAM
node 1 cpus: 20-39,60-79     size: 129003 MB     <- socket 1, DRAM
node 2 cpus: (none)          size:  16384 MB     <- CXL, memory-only
node 3 cpus: (none)          size:  65536 MB     <- CXL, memory-only

node distances:
        0    1    2    3
   0:  10   20   20   20
   1:  20   10   20   20
   2:  20   20   10   20
   3:  20   20   20   10
```

**This is the single most important thing to understand about this machine.**
There are 4 NUMA nodes but only **2 of them have CPUs**. Nodes 2 and 3 are
CXL-attached memory-only tiers, which is why the kernel is a custom `cxleak`
build. Every benchmark in this repo binds to nodes 0 and 1 only; the CXL nodes
are not used by the current campaigns.

Note the distance matrix is uniform (all remote = 20), so it does *not*
distinguish socket-to-socket from socket-to-CXL. Do not rely on it for tiering.

### 1.2 Consequences for porting

A target machine will almost certainly differ in ways that matter:

| Property | stormbreaker | What to check on the target |
|---|---|---|
| CPU-bearing nodes | 2 (nodes 0,1) | `numactl -H` — count nodes with a non-empty cpulist |
| Memory-only nodes | 2 (nodes 2,3, CXL) | likely **none** on a standard HPC node |
| Total threads | 80 | `nproc` |
| Threads per node | 40 | derive; do not assume 40 |
| Node numbering | CPU nodes are 0,1 | on NPS4-style parts CPU nodes may be 0..7 with no gaps |

If the target has more than 2 CPU-bearing nodes, the `numa<T,k>` logical→physical
mapping changes meaning. See §4.1.

---

## 2. Kernel / runtime state the experiments depend on

```
/proc/sys/kernel/numa_balancing        = 1        (AutoNUMA ON at capture time)
/sys/kernel/mm/transparent_hugepage/enabled = [always] madvise never
```

### 2.1 AutoNUMA is the independent variable — and it needs root

The whole campaign design is an **AN_on vs AN_off** comparison. `scripts/campaign.py`
*reads* `/proc/sys/kernel/numa_balancing` to decide which folder to write into
(`autonuma()`, `campaign.py:47`), and **aborts** if the value changes mid-run
(`campaign.py:293`). It never sets it. Flipping it is a manual, privileged step:

```shell
sudo sysctl -w kernel.numa_balancing=0     # or =1
```

**Porting hazard.** On a shared HPC system you will not have root, so this exact
mechanism cannot work — and because `autonuma()` drove *both* the folder name and
the `--balancing` flag, a machine stuck at `numa_balancing=1` could only ever
produce AN_on runs.

**The per-process mechanism is `numactl --balancing` (`-b`)**, and it is a real
control, not a fudge. From `numactl(8)` (verified, numactl 2.0.19):

> `--balancing, -b` — Enable Linux kernel NUMA balancing for the process if it is
> supported by kernel. **This should only be used with `--membind`**, otherwise
> ignored.

It sets `MPOL_F_NUMA_BALANCING` on the process's `MPOL_BIND` policy. An explicit
`--membind` policy without that flag means the kernel does **not** apply NUMA
balancing to the bound memory; with it, balancing is enabled. So the two arms are
a genuine contrast:

```
AN off:  numactl      --cpunodebind=0,7 --membind=0,7   ...
AN on:   numactl -b   --cpunodebind=0,7 --membind=0,7   ...
```

Our commands always pass `--membind`, so the flag applies. This is the *same*
mechanism `campaign.py` already used for the AN_on arm on stormbreaker — the
difference is only that stormbreaker moved the global knob as well, so both moved
together. On Perlmutter the global knob stays on and `-b` is the only variable,
which is arguably the better-isolated experiment.

`campaign.py --an-mode {auto,on,off}` selects it:

- **`auto`** (default) reads `/proc/sys/kernel/numa_balancing` — stormbreaker
  behaviour, unchanged.
- **`on` / `off`** force the arm where you cannot touch the kernel knob.

When forced, the manifest run line records that it was forced and what the kernel
reported, so a per-process contrast is never mistaken for a system-wide one:

```
- AN_off -- 2026-08-23 15:40 -- kernel 6.11... -- FORCED via --an-mode
  (kernel numa_balancing=1; numactl --balancing only, not a kernel toggle)
```

#### Measured on stormbreaker, 2026-08-23 (90 s, -t 40, 20M keys, payload 128)

`-b` is a hard switch, not a soft one — without it the kernel does essentially no
balancing work for the process at all:

| config | arm | pages migrated | PTE updates |
|---|---|---|---|
| numa/regular | off | 14 | 41 |
| numa/regular | **on** | **612,688** | **3,806,342** |
| numa/numa | off | 216 | 20,963 |
| numa/numa | **on** | 26,305 | 265,031 |

Three things follow:

1. **The AN_off arm pays no scan overhead.** It is not scanning and declining to
   migrate; it is not scanning. So `--an-mode off` (global knob on, process not
   opted in) is equivalent *in effect* to stormbreaker's global
   `numa_balancing=0`, and the two machines' arms stay comparable:

   | | stormbreaker | Perlmutter |
   |---|---|---|
   | AN_on | global=1 + `numactl -b` | global=1 + `numactl -b` |
   | AN_off | global=0, no `-b` | global=1, no `-b` |

   The AN_on arms are identical; the AN_off arms differ on paper but measure the
   same. Task-placement balancing is off in both too, since it is driven by the
   same hinting-fault scan.

2. **Migration is confined to the bound nodemask.** Under `MPOL_BIND` over
   `{0,7}`, AutoNUMA can only move pages between 0 and 7 — never to nodes 1-6.
   That is exactly the two-partition experiment.

3. **`numa<T,k>` pages are never balanced, in either arm.** UMF gives them their
   own `mbind` policy without `MPOL_F_NUMA_BALANCING`, so they stay pinned. That
   is the pinning under study, not a gap — and it is why `numa/numa` shows ~23x
   less migration than `numa/regular`. The 26,305 residual is the non-numa-typed
   memory (locks, globals, jemalloc metadata) that the paper describes as
   polluting cross-node pages.

**Pilot with `--DS_config=regular`, never `numa/numa`.** The numa/numa config is
the one where a perfectly working `-b` looks broken. Judge the pilot by the
`/proc/vmstat` counters above, not by throughput — a short run has no statistical
power on ops (the throughput deltas in the runs above were 1.2% and 0.1%, i.e.
noise at n=1).

### 2.2 THP — set at runtime, not recorded until 2026-08-24

THP materially affects NUMA page-migration behaviour and therefore the AN_on
results. On this machine it is **toggled by hand and reverts on reboot**:

```
/proc/cmdline           no transparent_hugepage= override
compiled-in default     madvise   (what a fresh boot gives)
observed 2026-08-23     always    (someone had set it that boot)
observed 2026-08-24      madvise   (after the 05:02 reboot)
```

Nothing on the machine records when it changes — no service writes it, and it
leaves no trace in shell history.

**The Jul-Aug 2026 campaigns did not record it, so their THP value is not
recoverable.** What *is* established, from `wtmp`: there was **no reboot between
Wed 2026-07-29 17:25 and Thu 2026-08-13 11:52**, and every campaign
(campaign03, campaign04, campaign01, campaign01.1, campaign02, campaign02.1,
DS campaign01) ran inside that single boot session. So THP was necessarily
**identical across all of them and across both AN arms of each** — the
AN_on/AN_off comparisons are internally valid whatever the value was. Only the
absolute value, and hence cross-machine comparison, is unverifiable. Do not
assert a THP setting for those campaigns in a paper.

`campaign.py` now records `THP=`, `defrag=` and `numa_balancing=` in the manifest
identity block **and in every run line** — per-run because THP can be changed
between a campaign's AN_off and AN_on runs, which are separate invocations often
hours or days apart.

---

## 3. Software dependencies (as actually installed here)

### 3.1 Toolchain

| Tool | Version on stormbreaker | Where |
|---|---|---|
| clang / clang++ | **21.0.0git** (`123c0040d4e6`) | `/usr/local/bin` — **source-built, not apt** |
| llvm-config | 21.0.0git, prefix `/usr/local` | `/usr/local/bin` |
| gcc / g++ | 12.3.0 | `/usr/bin` (Ubuntu) |
| cmake | 3.26.5 | |
| make | 4.3 | |
| ninja | 1.10.1 | |
| python3 | 3.10.12 | system python, **no venv** |
| spack | **not installed** | intentionally — see §3.3 |

The LLVM at `/usr/local` is a **hand-built trunk LLVM**, not a distro package.
This matters because `numa-clang-tool` links against `libclang-cpp` / `libLLVM`
and is sensitive to the version it was built against.

### 3.2 Libraries (all from Ubuntu apt)

```
libnuma-dev      2.0.14-3ubuntu2
libnuma1         2.0.14-3ubuntu2
libhwloc-dev     2.7.0-2ubuntu1
libhwloc15       2.7.0-2ubuntu1
libjemalloc-dev  5.2.1-4ubuntu1
libjemalloc2     5.2.1-4ubuntu1
libtbb-dev       2021.5.0-7ubuntu2
```

Resolved by UMF's CMake to:
```
jemalloc  /usr/lib/x86_64-linux-gnu/libjemalloc.so   (5.2.1_0, headers /usr/include)
hwloc     /usr/lib/x86_64-linux-gnu/libhwloc.so
```

### 3.3 The spack/module split — this is the existing portability seam

The repo **already** has a two-machine story, and it is worth understanding before
adding a third mechanism:

- `scripts/load.py` emits `module load PrgEnv-llvm spack cmake ; spack env activate NUMATyping`.
  This is **already NERSC/Perlmutter-specific** and is a no-op-by-absence here.
- `scripts/env.py` resolves jemalloc via `spack location -i jemalloc`, resolves
  `CXX`/`CC` from `clang++ -print-resource-dir`, and exports `MAX_NODE_ID` /
  `NUM_NUMA_NODES` from `/sys/devices/system/node/online`.
- `scripts/numafy.py` has hardcoded **Perlmutter fallback paths** baked in
  (`numafy.py:95-96`):
  ```
  CLANG_INC fallback: /global/common/software/nersc/pe/gpu/llvm/20.1.3/lib/clang/20/include/
  JEMALLOC  fallback: /global/homes/k/kiwo9430/.spack/opt/spack/linux-zen3/jemalloc-5.3.0-rd7q...
  ```
  Note that fallback implies **LLVM 20.1.3** on Perlmutter vs **21.0.0git** here.

On stormbreaker, `spack location -i jemalloc` returns empty and the callers handle
that gracefully (`campaign.py` guards with `if jr:`), falling through to the apt
jemalloc. **Preserve that graceful-empty behavior.** It is what makes the repo work
here without spack.

---

## 4. Build chain and what is machine-specific

### 4.1 numaLib is portable — do not "fix" it

`numaLib/numa_nodemap.hpp` already auto-detects CPU-bearing nodes at runtime via
`numa_node_to_cpus()`, and orders them **outside-in** (`[a,z,b,y,...]`) so that
consecutive logical partitions land on the farthest-apart physical nodes. Logical
`k` maps to `order[k % N]`.

- On stormbreaker: CPU nodes `[0,1]` → order `[0,1]`.
- On an 8-CPU-node machine: `[0..7]` → order `[0,7,1,6,2,5,3,4]`.

It honors a `NUMA_NODE_ORDER` env override. **The C++ side is already portable.**
The same header is duplicated into each suite (`DataStructureTests/include/`,
`Array/include/`, `allocator_test/include/`, `DataStructureTests_four/include/`) —
if you change one, change all, or they will silently diverge.

### 4.2 machine.env

`scripts/detect_machine.sh` probes the topology and writes `machine.env`. Current
contents (generated 2026-06-29):

```shell
NUM_PHYS_NODES=4        # includes the 2 memory-only CXL nodes
CPU_NODES=0,1
NUM_CPU_NODES=2
TOTAL_THREADS=80
NUMA_NODE_ORDER=0,1
NUMACTL_BIND="--cpunodebind=0,1 --membind=0,1"
```

This is the **right place** to put per-machine values. It is hand-editable by
design. Re-running `detect_machine.sh` on a new machine regenerates it correctly.

### 4.3 The numactl binding (FIXED 2026-08-23 — read this before changing it)

This *was* the main portability bug: `campaign.py` hardcoded
`numactl --cpunodebind=0,1 --membind=0,1`, stormbreaker's topology, and nothing
in Python read `machine.env`.

It now resolves the binding from `machine.env` at run time:

```python
# scripts/campaign.py
MACHINE_ENV_DEFAULTS = {"NUMACTL_BIND": "--cpunodebind=0,1 --membind=0,1", ...}

def numactl_prefix(an_value, root=ROOT):
    bind = machine_env(root)["NUMACTL_BIND"]
    return f"numactl {'--balancing ' if an_value == 1 else ''}{bind}"
```

`run_config()` additionally exports `NUMA_NODE_ORDER` into the benchmark's
environment, so thread pinning (`numathreads.hpp:65`) and the UMF pools
(`umf_numa_allocator.hpp:52,104`) land on exactly the nodes numactl bound us to.
**If those two ever disagree, allocation on an unbound node fails at runtime** —
loudly, which is the behavior we want.

`campaign.py` prints the resolved binding at startup and records it in the
manifest, and warns if `machine.env` is missing (in which case it falls back to
the stormbreaker literals above).

#### How the node pair is chosen

The benchmarks use exactly **two logical partitions**. This is not configurable at
run time: `main.cpp`/`TestSuite.cpp` instantiate `numa<T,k>` and `thread_numa<k>`
at `k = NODE_ZERO (0)` and `k = MAX_NODE`, and `MAX_NODE` defaults to `1` because
the `-DMAX_NODE=` injection is **commented out** in both `numafy.py:119` and
`Output/*/Makefile:3`. Likewise `NUMA_NODE_NUM` defaults to `2`, sizing the UMF
pool array. Both logical ids go through `numa_node_map()`.

So the question is only *which two physical nodes* those partitions land on.
`detect_machine.sh` now answers it: it orders the CPU-bearing nodes outside-in
and takes the first `NUM_PARTITIONS` (default 2) — i.e. the farthest-apart pair:

| CPU nodes | outside-in spread | NUMA_NODE_ORDER | numactl bind |
|---|---|---|---|
| 0,1 (stormbreaker) | `0,1` | `0,1` | `--cpunodebind=0,1 --membind=0,1` |
| 0–3 | `0,3,1,2` | `0,3` | `--cpunodebind=0,3 --membind=0,3` |
| 0–7 (Perlmutter CPU node) | `0,7,1,6,2,5,3,4` | `0,7` | `--cpunodebind=0,7 --membind=0,7` |

Stormbreaker's generated values are **byte-identical to what was hardcoded
before**, so nothing changed here. Override with
`NUM_PARTITIONS=4 bash scripts/detect_machine.sh`, or hand-edit `machine.env`.

#### `0,7` on Perlmutter is established precedent, not a new choice

Three older scripts already switch to `0,7` behind an `is_perlmutter` flag —
`runExperiments.py:89`, `runYCSB.py:83`, `perfBST.py:136` — so this matches how
Perlmutter runs were done before. Two loose ends there:

- **`perfYCSB.py:106` has no Perlmutter branch** and is still hardcoded to `0,1`.
  It will bind wrongly on Perlmutter. Port it to `machine_env()` or give it the
  same flag.
- Those scripts also **double every size parameter** on `--perlmutter`
  (threads 128, keys 200M, buckets 266600, interval 10). Those are the *paper's*
  numbers, not the current campaigns' — reusing them would double the working
  set. See §9.2: pass the campaign values unchanged, and take the thread count
  from `PARTITION_THREADS` (§9.3).

#### machine.env is no longer tracked by git

`machine.env` used to be committed. It is now in `.gitignore`, because
`campaign.py` **refuses to run on a dirty tree** (`git_gate()`, `campaign.py:261`)
— so on any new machine, re-running `detect_machine.sh` would modify a tracked
file and block every campaign. Provenance is not lost: the resolved binding and
node order are written into each campaign manifest.

**On a fresh clone you must run `bash scripts/detect_machine.sh` before the first
campaign**, or you silently inherit stormbreaker's `0,1` (with a warning printed).

### 4.4 numa-clang-tool

```
built with: CMAKE_CXX_COMPILER=/usr/bin/c++   (gcc 12)
links against: LLVM/Clang 21 at /usr/local
artifact: numa-clang-tool/build/bin/clang-tool
```

`CMakeLists.txt` auto-detects the clang version and the builtin include dir, and
accepts overrides: `-DCLANG_VER=`, `-DCLANG_BUILTIN_INCLUDE_DIR=`,
`-DLLVM_SHARED_LIB=`, `-DCLANG_CPP_SHARED_LIB=`, `-DLLVM_LDFLAGS=`. Run
`cmake -DHELP=ON ..` for the built-in guidance.

**Version sensitivity is the main risk.** Built here against trunk 21; Perlmutter's
`PrgEnv-llvm` is 20.1.3 per the numafy fallback. The tool uses clang AST-matcher
APIs, which are explicitly unstable across major versions. Expect the tool to need
source fixes, not just flags, if the major version differs. Budget for that.

`numa-clang-tool/build/` is **gitignored** — it never transfers. It must be rebuilt
on every machine.

### 4.5 UMF (unified-memory-framework)

```
built with: CMAKE_C_COMPILER=/usr/local/bin/clang   (21.0.0git)
artifacts:  build/lib/libumf.a  (331 KB)
            build/lib/libjemalloc_pool.a (19.6 KB)
```

Both are linked **statically** into the benchmarks. `build/` is gitignored; the
**sources are tracked** (347 files), so UMF travels with the repo and is rebuilt
per machine. Same for numa-clang-tool (237 tracked files). **There are no git
submodules** — `.gitmodules` does not exist. A plain `git pull` gets everything.

### 4.6 Include/link contract

Both `numafy.py` and the per-suite Makefiles hardcode the same UMF include list.
If you change one, change both, or you get a header/ABI mismatch that shows up as
a link error or, worse, silent struct-layout corruption:

```
-I$(UMF)/src/utils -I$(UMF)/include -I$(UMF)/examples/common -I$(UMF)/src
-I$(UMF)/src/ravl -I$(UMF)/src/critnib -I$(UMF)/src/provider
-I$(UMF)/src/memspaces -I$(UMF)/src/memtargets -DUMF
-lhwloc -lrt -ldl -ljemalloc
$(UMF)/build/lib/libumf.a $(UMF)/build/lib/libjemalloc_pool.a
```

Suite Makefiles (`Output/<SUITE>/Makefile`) use `clang++`, `-O3 -g -std=c++20
-pthread -fno-omit-frame-pointer -D_NODE_HPP=1 -DPIN_INIT=1`, and take
`ROOT_DIR=`, `UMF=1`, optional `JEMALLOC_ROOT=` (which adds `-L` and `-Wl,-rpath`).

---

## 5. Known-stale things (do not treat as working examples)

These are already broken *on this machine* and will mislead you if you copy them:

1. **All `*.slurm` files at the repo root are stale.** They call `python3 load.py`,
   `python3 env.py`, `python3 runYCSB.py`, `python3 runExperiments.py` from the
   repo root, but commit `29f5e7db` ("Prune scripts/") moved every one of those
   into `scripts/`. Every slurm script needs its paths updated. They do still carry
   the correct NERSC preamble, which is worth keeping:
   ```
   #SBATCH --account=m5308
   #SBATCH --constraint=cpu
   #SBATCH --qos=regular
   #SBATCH --nodes=1 --exclusive
   cd $SCRATCH/NUMATyping
   ```
2. **`README.md` §Build has the same stale paths** (`python3 load.py`,
   `python3 numafy.py` at root — both now under `scripts/`).
3. **`DataStructureTests_four/` still has the `duration`/`interval` truncation
   segfault** that was fixed in `DataStructureTests/`. Do not run the `_four`
   suite until that fix is ported across.
4. **`line_plot_bst.py --AN both` in legacy (non-campaign) mode** raises
   `NameError`: `an_folder_name` is only bound in the non-`both` branch but is
   used to build the output path. Pre-existing; campaign mode is unaffected.

---

## 6. Data that must not be touched

`Campaigns/`, `Runs/`, `Old_Results/`, `FlameGraphs/` are all **gitignored** and
exist **only on this machine's disk**. They are the accumulated experimental
results. They will not appear on any other machine via git, and they cannot be
regenerated without re-running multi-hour campaigns.

Do not delete them and do not add them to git (they are large; a previous attempt
to commit `FlameGraphs/` hit GitHub's file-size limit).

Note this machine's root filesystem is a **single 879 GB `/dev/sda2` with no
separate `/home` or `/tmp`**, and it has been at 100% before. Disk headroom is a
real operational constraint here; check `df -h /` before starting a campaign.

---

## 7. Reproducing the reference build on stormbreaker

For comparison, this is what a from-scratch build looks like here — no modules, no
spack, everything from apt plus the `/usr/local` LLVM:

```shell
cd ~/NUMATyping

# 1. topology
bash scripts/detect_machine.sh ~/NUMATyping     # writes machine.env

# 2. compiler tool  (needs LLVM/Clang >= 20 dev libs)
cmake -S numa-clang-tool -B numa-clang-tool/build
cmake --build numa-clang-tool/build -j

# 3. allocator
cmake -S unified-memory-framework -B unified-memory-framework/build
cmake --build unified-memory-framework/build -j

# 4. transform + compile a suite
python3 scripts/numafy.py --ROOT_DIR=$PWD --umf=1 DataStructureTests
make -C Output/DataStructureTests ROOT_DIR=$PWD UMF=1

# 5. run a campaign (reads numa_balancing to pick AN_on/AN_off)
python3 scripts/campaign.py --bench DS ...
```

Valid suite names for `numafy.py`: `DataStructureTests`, `DataStructureTests_four`,
`ycsb`, `Histogram`, `Array`.

Note the bench key and suite directory differ for DS: bench `DS` → suite
`DataStructureTests`. `benchmarks.py` now carries an explicit `"suite"` field for
exactly this reason; use `BENCHES[bench]["suite"]` for any path construction.

---

## 8. Summary: the porting checklist

Machine-specific, **must** be re-derived on the target:
- [ ] `machine.env` — re-run `scripts/detect_machine.sh`
- [ ] `numa-clang-tool/build/` — rebuild (gitignored, version-sensitive)
- [ ] `unified-memory-framework/build/` — rebuild (gitignored)
- [ ] `Output/<SUITE>/` — re-numafy and recompile (gitignored)
- [ ] jemalloc / hwloc / libnuma resolution (apt here, spack there)
- [ ] clang version and builtin include dir (21 trunk here, 20.1.3 there)
- [ ] AutoNUMA: no root on Perlmutter, so use `--an-mode on|off` — the
      contrast is `numactl --balancing` only, and the manifest says so (§2.1)
- [ ] run `bash scripts/detect_machine.sh` (writes machine.env; gitignored,
      so it does NOT arrive with the clone) — §4.3
- [ ] `perfYCSB.py:106` still hardcodes `0,1`, no Perlmutter branch — §4.3
- [ ] pass `--threads $PARTITION_THREADS` (from machine.env); the
      `benchmarks.py` default of 80 is a stormbreaker value — §9.3
- [ ] **ycsb: `numactl -H` the memory of nodes 0+7 before sizing.** Five of six
      campaigns fit with margin; only campaign04 (4 KB payload) is at risk — §9.2
- [ ] pass the campaign parameters unchanged; do NOT use `runYCSB.py`'s doubled
      `--perlmutter` defaults, and pass `--duration 300` (the default is 1200,
      which no campaign used) — §9.2, §9.4
- [ ] `*.slurm` paths (already stale even here — §5)

Portable, **leave alone**:
- [ ] `numaLib/numa_nodemap.hpp` runtime node detection (§4.1)
- [ ] `scripts/detect_machine.sh` probing logic
- [ ] the `if jr:` empty-spack fallbacks in `campaign.py` / `numafy.py`
- [ ] `detect_machine.sh`'s outside-in rule — it already yields 0,7 on an
      8-node machine; do not replace it with a hand-written node list
- [ ] `benchmarks.py` bench→suite mapping
- [ ] everything in `Campaigns/` (§6)
- [ ] the 11 identical `numa_nodemap.hpp` copies — edit all or none (§9.6)

---

## 9. YCSB specifics (read this before running ycsb anywhere else)

### 9.1 What to keep, what to scale, what to re-derive

| keep identical | scale up deliberately | re-derive per machine |
|---|---|---|
| `duration`, `warmup`, `interval` | `keys` **and** `buckets` together, so the load factor is preserved (§9.2) | numactl binding: 0,1 -> 0,7 (automatic, §4.3) |
| `tables`, `mix`, `theta`, `hash` | | `threads` — 64 on nodes 0+7, not 80 (§9.3) |
| configs and workloads | | AutoNUMA arm — `--an-mode`, not the kernel knob (§2.1) |

The trap: `membind` restricts the run to the **bound nodes'** RAM, so a bigger
machine can give you a *smaller* budget. Nodes 0+7 are 128 GB and 64 threads
against stormbreaker's 257 GB and 80 threads. Size against 128 GB. Details in
§9.2.

### 9.2 Scaling the working set up for the target machine

The stormbreaker campaigns are sized for stormbreaker. On a bigger machine you
should **scale the working set up** — but the amount of room you actually have is
not what "512 GB node" suggests, and this is the trap:

```
stormbreaker, nodes 0+1  ->  ~257 GB   and  80 hardware threads
Perlmutter,   nodes 0+7  ->   128 GB   and  64 hardware threads
```

`membind` restricts the run to the **bound nodes'** RAM. Because the experiment
uses only 2 of Perlmutter's 8 NUMA nodes, it gets **less** memory and **fewer**
threads than stormbreaker, on a machine that is four times larger overall. Scale
the working set to fill 128 GB, not 512 GB, and do not try to scale threads up at
all — 64 is what nodes 0+7 have (§9.3).

(Using more of the machine would mean more than two NUMA partitions. That is a
code change, not a config change: `MAX_NODE` and `NUMA_NODE_NUM` are compile-time
and default to 1 and 2, and the `-D` injection is commented out. See §9.9.)

**Do NOT reuse `runYCSB.py`'s `--perlmutter` defaults** (`keys 200000000`,
`buckets 266600`, `threads 128`, `interval 10`, `runYCSB.py:82-87`). Those are the
*paper's* numbers, and they are not reproducible with today's code anyway: commit
`f5dcf278` changed the prefill from the whole keyspace to **every other key** and
added the payload option, so the same `keys` value no longer means the same record
count. Scale deliberately from the current campaign values instead.

#### Recommended scaling: 3x

The paper set the precedent — *"We increased our memory footprint by 3x and ran it
on our large dual-socket (8 NUMA node) AMD EPYC processor to see how our benchmark
scales."* 3x fits comfortably, keeping the load factor fixed by scaling `buckets`
with `keys`:

| campaign | keys | buckets | prefill | steady 4x | LF | |
|---|---|---|---|---|---|---|
| campaign01 x3 | 300M | 200000 | 22.4 G | 90.9 G | 0.75 | safe |
| campaign01.1 x3 | 300M | 90011 | 22.4 G | 90.1 G | 1.67 | safe |
| campaign02 x3 | 300M | 200000 | 22.4 G | 90.9 G | 0.75 | safe |
| campaign02.1 x3 | 300M | 90011 | 22.4 G | 90.1 G | 1.67 | safe |
| campaign03 x3 | 300M | 90011 | 13.4 G | 54.3 G | 1.67 | safe |
| **campaign04 /2** | **10M** | **6669** | 19.2 G | 76.9 G | 0.75 | **scaled DOWN** |

Halve the scaling to 2x (`keys 200M`, `buckets 133367` / `60013`) if you want more
headroom; that lands around 60 G at the 4x bound.

**campaign04 is the exception and must scale DOWN.** At payload 4096 its
stormbreaker size (20M keys) reaches **154 G** at the 4x bound — over the 128 GB
budget before any scaling at all. Halve it to 10M keys / 6669 buckets.

Keep everything else identical: `tables=1000`, `warmup=60`, `duration=300`,
`interval=20`, `mix`, `theta`, `hash`, configs and workloads. Changing the
working set is deliberate; changing the rest would make the runs incomparable to
the stormbreaker campaigns for no benefit.

#### Why the load factor must be scaled with the keyspace

`LF = (keys/2) / (tables x buckets)`. Load factor is the variable campaigns 01 vs
01.1 and 02 vs 02.1 were built to isolate (0.75 vs 1.67 — shallow vs deep hash
chains), so scaling `keys` without scaling `buckets` would silently change the
thing under study. Every `buckets` value above preserves its campaign's LF.

#### The campaigns to reproduce

Every ycsb campaign shares `tables=1000`, `threads=80`, `warmup=60`,
`duration=300`, `interval=20`, all four configs and all seven workloads. They
differ only in mix/payload/buckets/keys:

| campaign | mix | theta | payload | buckets | keys |
|---|---|---|---|---|---|
| campaign01 | uniform | 0.7 | 128 | 66713 | 100M |
| campaign01.1 | uniform | 0.7 | 128 | 30011 | 100M |
| campaign02 | zipfian | 0.9 | 128 | 66713 | 100M |
| campaign02.1 | zipfian | 0.9 | 128 | 30011 | 100M |
| campaign03 | zipfian | 0.7 | 64 | 30011 | 100M |
| campaign04 | uniform | 0.7 | 4096 | 13337 | 20M |

(all use `hash=mix`; `short-test` is the 30 s smoke config: 2M keys, 100 tables,
1009 buckets, `hash=djb2`.)

#### Baseline: the stormbreaker sizes these scale from

```
record bytes  = payload + 32           (32 B hash-node overhead)
prefill recs  = keys / 2               (only the even half is prefilled)
prefill bytes = (keys/2) x (payload + 32)  +  tables x buckets x 8
steady state  = 2x to 4x prefill
```

The load factor and working set **drift upward mid-run**: update and insert fill
the absent odd half of the keyspace, and cross-node remote accesses duplicate keys
into the other node's tables. Manifests state **prefill-only** figures, so the
number in a manifest is not the peak. campaign01 measured steady state at ~4x the
prefill record count (50M -> 200M); campaign04's note counts only the odd-half
fill (2x). **Workload C is read-only and stays at prefill.**

| campaign | prefill | steady 2x | steady 4x | fits ~128 GiB? |
|---|---|---|---|---|
| campaign01 / 02 | 7.5 G | 15.4 G | 30.3 G | yes |
| campaign01.1 / 02.1 | 7.5 G | 15.1 G | 30.0 G | yes |
| campaign03 | 4.5 G | 9.2 G | 18.1 G | yes |
| **campaign04** | **38.4 G** | **77.0 G** | **153.9 G** | **2x only — at risk** |
| short-test | 0.1 G | 0.2 G | 0.4 G | yes |

Five of the six fit with large margin. **campaign04 (4 KB payload) is the only one
at risk**: it fits if growth is 2x (77 G) and OOMs if it reaches 4x (154 G). If
you run it on Perlmutter, watch RSS, or halve `keys` to 10M and set
`buckets=6669` to hold the load factor at 0.75.

Why the budget is tight there: `membind` restricts the run to the **bound nodes'**
RAM, not the node total. Per the paper's own description of the large machine
(§9.8), each of its 8 NUMA nodes has **64 GB**, so nodes 0+7 give **128 GB** —
about **half** of stormbreaker's ~257 GB, on a machine whose 512 GB total looks
twice as big. Still confirm on the actual allocation:

```shell
numactl -H | grep -E '^node (0|7) size'
```

### 9.3 Threads: the one parameter that cannot simply be copied

`ycsb/src/main.cpp:180` does `threads_per_node = num_threads / 2` — integer
division across exactly two partitions — so `--threads` must be **even**, and it
should not exceed the CPUs of the **bound nodes**.

The campaigns used `threads=80`, which on stormbreaker is every hardware thread of
nodes 0+1. On the large machine the paper states 128 physical cores / 256 threads
across 8 NUMA nodes, i.e. **16 cores / 32 threads per node**, so nodes 0+7 are
**32 physical cores / 64 hardware threads** — and `threads=80` would be ~1.25x
oversubscribed. `detect_machine.sh` now exports `PARTITION_THREADS` (hardware
threads on the bound nodes, forced even; **80 on stormbreaker**, identical to the
existing default; expect **64** on Perlmutter nodes 0+7). `campaign.py` warns on a
mismatch but does **not** override, because thread count is a recorded experiment
parameter.

**Recommendation: use `PARTITION_THREADS`, not 80.** Cross-machine throughput is
not directly comparable anyway (Xeon Sapphire Rapids vs EPYC Milan), so matching
the thread count buys nothing, while oversubscription adds scheduler noise to the
AN_on/AN_off contrast that *is* the measurement. What must stay identical is the
working set and the duration — and `campaign.py`'s guards already force the two AN
runs on Perlmutter to share a thread count, which is what makes that contrast
valid. Record the value in the manifest either way.

If you would rather keep 80 for continuity, that is defensible — just do it
deliberately and note it in `--purpose`.

### 9.4 Runtime budget

Every ycsb campaign ran `duration=300` with `warmup=60`, over 4 configs x 7
workloads = 28 runs:

```
28 x (300 s + 60 s)  =  2.8 h per AutoNUMA mode,  plus prefill per run
both AN modes        =  ~5.6 h + prefill
```

Prefill of 50M records is minutes per run, so budget roughly **4 h per AN mode /
8 h total**. The DS campaign01 (`duration=600`, `interval=60`, numDS 1M, keys 80)
is ~4.7 h per AN mode.

Note `benchmarks.py` defaults `duration` to **1200**, which **no campaign used** —
pass `--duration 300` explicitly or you will run 4x longer than intended.

The two AutoNUMA modes are **separate jobs** (`campaign.py` writes one AN folder
per invocation; the manifest guards enforce that both share commit and config).
The existing `ycsb_job*.slurm` files ask for `--time=48:00:00`; that is ample for
these durations — confirm it is allowed under your QOS.

#### Canonical Perlmutter invocation

```shell
cd $SCRATCH/NUMATyping
bash scripts/detect_machine.sh $PWD        # -> NUMA_NODE_ORDER=0,7, bind 0,7
source machine.env

python3 scripts/campaign.py --bench ycsb --slug campaign01-perl \
    --purpose "campaign01 scaled 3x (keys 100M->300M, buckets 66713->200000, LF 0.75 held); AN arm via numactl -b" \
    --mix uniform --hash mix --theta 0.7 \
    --payload 128 --buckets 200000 --tables 1000 --keys 300000000 \
    --warmup 60 --duration 300 --interval 20 \
    --threads $PARTITION_THREADS --an-mode off
```

Then run the same command again with `--an-mode on`. That is the whole AutoNUMA
contrast on Perlmutter: `--an-mode off` omits `numactl -b`, `--an-mode on` adds it
(§2.1). Everything else — commit, parameters, binding — must be identical, and
`campaign.py`'s manifest guards enforce that.

### 9.5 ycsb build specifics

- ycsb links **four** objects, not two: `src/main.o src/ycsb_benchmark.o
  zipf_base/src/ycsbutils.o zipf_base/src/zipfian_generator.o`, and needs
  `-Izipf_base/src`. `zipf_base/` is tracked source (84 KB), so it travels with
  the clone — there is no external YCSB dataset to download.
- Same UMF include/link block and same `clang++ -O3 -std=c++20 -D_NODE_HPP=1
  -DPIN_INIT=1` flags as the DS suite.
- `MAX_NODE` defaults to 1 with a `#warning` (`ycsb_benchmark.cpp:29`,
  `main.cpp:19`) because `-DMAX_NODE=` is commented out. Expect that warning in
  the build log; it is normal, not a misconfiguration.

### 9.6 The duplicated header hazard

`numa_nodemap.hpp` exists in **11 copies** (one per suite plus `numaLib/`). They
are byte-identical today — I verified all 11. But the suite Makefiles put
`-Iinclude/` **before** `-I$(ROOT_DIR)/numaLib`, so **the suite's own copy wins**.
Editing `numaLib/numa_nodemap.hpp` alone changes nothing for ycsb. If you touch
that header, propagate it to every copy:

```shell
for f in $(grep -rl NUMA_NODE_ORDER --include=numa_nodemap.hpp .); do
    cp numaLib/numa_nodemap.hpp "$f"
done
```
(then re-run numafy, since `Output/` holds yet another transformed copy.)

### 9.7 Workload string format

`--workload=A-50-50-50,A-100-0-50` is a comma-separated list of thread groups,
each `<workload>-<local%>-<remote%>-<thread%>`. The example reads: half the
threads run workload A with a 50/50 local/remote split, the other half run A
fully local. `parse_mixed_workload()` divides `num_threads` by these percentages,
so a thread count that does not divide cleanly changes the group sizes — another
reason to keep `--threads` even and matched to `PARTITION_THREADS`.

The seven default workload strings live in `YCSB_WORKLOADS` in `benchmarks.py`.
In the CSV the commas become dashes so the value stays one cell
(`main.cpp:63`) — `an_comparison.py` and the plotting scripts depend on that.

### 9.8 The target machine, as the paper describes it

From `paper_final.pdf` (the "large" machine — the NERSC/Perlmutter-class node):

> dual socket AMD EPYC 7763 64-Core NUMA machine with 128 physical cores
> (256 threads with Hyper-Threading enabled) split across 8 NUMA nodes. Each node
> has a 64 GB main memory attached to it bringing the total available memory to
> 512 GB. The maximum latency difference between local and remote memory across
> the 8 nodes is about 3x.

Derived per-node figures, which are what the sizing above depends on:

| | whole machine | per NUMA node | nodes 0+7 |
|---|---|---|---|
| physical cores | 128 | 16 | 32 |
| hardware threads | 256 | 32 | **64** |
| memory | 512 GB | 64 GB | **128 GB** |

**On "256 threads":** that is the machine's hardware-thread count, not a run
parameter. Every row of the archived Perlmutter data —
`Old_Results/Graphs/Perlmutter/`, both AN modes, ycsb and BST — records
`num_threads = 128`, i.e. one thread per physical core with SMT unused. There is
no 256-thread run anywhere in the repo.

**An open question worth resolving before you rely on the old numbers:** 128
threads on nodes 0+7 is only 64 hardware threads, so those runs were ~2x
oversubscribed *if* they were bound to `0,7`. Either the March-2026 runs were not
CPU-bound to two nodes, or they were and accepted the oversubscription. The paper
says the large-machine experiment ran "across its two physical socket[s]", which
is consistent with one node per socket (0 and 7). This does not affect the plan
above — `PARTITION_THREADS` sidesteps it — but do not treat `t=128` as a
validated setting for a 2-node binding.

Two further notes from the paper, relevant to comparisons:

- **The paper's Intel machine is not today's stormbreaker.** The paper describes
  it as 20 cores/socket, 80 logical cores, and **128 GB RAM with 64 GB per NUMA
  node**. Today's stormbreaker reports **331 GB with ~128 GB per node** (§1.1),
  so it has been expanded since. The core/thread counts still match.
- **The paper's ycsb config is the `benchmarks.py` default**: "1000 hash tables
  with 133300 buckets each ... up to 100 million, 32 Byte entries ... around
  10 GB ... each workload for 20 minutes" — i.e. `tables=1000 buckets=133300
  keys=100000000 duration=1200`. That is where the 1200 s default comes from. The
  current campaigns deliberately use `duration=300` and different bucket counts,
  which is why §9.4 tells you to pass `--duration 300` explicitly.

### 9.9 Why you cannot simply use more of the machine

The obvious way to exploit an 8-node machine is more NUMA partitions. That is a
**code change, not a config change**:

- `MAX_NODE` and `NUMA_NODE_NUM` are compile-time macros defaulting to `1` and
  `2`, and the `-DMAX_NODE=` / `-DNUMA_NODE_NUM=` injection is **commented out**
  in `numafy.py:119` and every `Output/*/Makefile:3`.
- ycsb hard-codes two partitions structurally: `ht_node0`/`ht_node1`,
  `threads_per_node = num_threads / 2`, and a two-column CSV
  (`Ops_Node0, Ops_Node1`). Four partitions means changing the schema and every
  consumer of it.
- `DataStructureTests_four/` is a 4-partition BST variant (`thread_numa<0..3>`),
  but its `NUMA_NODE_NUM` still defaults to **2** while it indexes logical nodes
  0-3 — so `NUMA_HANDLES[]` and `jemalloc_pool[]`, both sized `NUM_NODES`, are
  **indexed out of bounds** unless it is built with `-DNUMA_NODE_NUM=4`, which
  nothing currently passes. It also still carries the `duration`/`interval`
  segfault (§5). Do not run it without fixing both.

So for this trip: keep two partitions, scale the working set (§9.2), and treat
4-partition support as separate work.
