# perlmutter.md — target machine profile and shared setup contract

**Purpose.** This file is the counterpart to `stormbreaker.md`. That file describes
the machine the repo was developed on; this one describes **Perlmutter (NERSC)**,
what had to change to make the repo run here, and — most importantly — the
**single setup path that now works on both machines**.

**Contract.** Same rule as `stormbreaker.md`, in the other direction: treat this
file as a description of Perlmutter, not of your machine. Do not "fix" these
values to match somewhere else. Everything machine-specific lives behind
`scripts/machine_profile.sh`; add a profile rather than editing values here.

Captured 2026-08-24, at repo commit `0a951209`, after the first full campaign
(`campaign01-perl`, both AutoNUMA arms) completed successfully.

---

## 0. The one command (read this first)

Both machines are now configured by the same script, which auto-detects where it
is running:

```shell
bash scripts/configure_machine.sh            # auto-detect
bash scripts/configure_machine.sh perlmutter-cpu
bash scripts/configure_machine.sh stormbreaker
```

It runs four stages — `topo`, `clangtool`, `umf`, `suites` — and each can be run
alone:

```shell
bash scripts/configure_machine.sh --stages=topo            # just re-derive machine.env
bash scripts/configure_machine.sh --stages=suites --suites=ycsb,DataStructureTests
```

Underneath it is `scripts/machine_profile.sh`, which is **sourceable** and is what
batch jobs and interactive shells use:

```shell
source scripts/machine_profile.sh            # auto-detect
source scripts/machine_profile.sh perlmutter-cpu
numatyping_profile_summary                   # print what was resolved
numatyping_verify_topology                   # check the live machine matches the profile
```

Machine is resolved as: **argument > `$NUMATYPING_MACHINE` > auto-detect**.
Auto-detect uses `$NERSC_HOST` / `$LMOD_SYSTEM_NAME` = `perlmutter`, else hostname
`stormbreaker*`, else `generic`.

**What each profile does:**

| profile | modules | topology |
|---|---|---|
| `perlmutter-cpu` | loads python, PrgEnv-llvm, spack, cmake (pinned versions) | probes, then reconciles against pinned expectations |
| `stormbreaker` | none — system toolchain, as before | pure probe |
| `generic` | none | pure probe |

The `stormbreaker` and `generic` profiles have **no pinned topology and load no
modules**, so behaviour there is byte-identical to running `detect_machine.sh`
directly, which is what happened before this layer existed. Nothing about
stormbreaker's path changed.

---

## 1. Hardware

**There are two different machines here, and confusing them will cost you a
campaign.** Login nodes and CPU compute nodes do not share a NUMA layout.

### 1.1 CPU compute node (where campaigns run)

```
example nodes  nid005645, nid004673
OS             SLES 15-SP6
kernel         6.4.0-150600.23.125_15.0.27-cray_shasta_c
CPU            2 x AMD EPYC 7763 (Milan), 64 cores/socket
               128 physical cores / 256 hardware threads
NUMA           NPS=4 -> 4 domains per socket, 8 per node
RAM            512 GB total, ~64 GB per NUMA domain
select with    --constraint=cpu
```

### 1.2 Login node (where you build, and must NOT measure)

```
example        login23
CPU            2 x AMD EPYC 7713 (Milan)
RAM            512 GB
GPU            1 x A100 (irrelevant here)
shared         ~20 concurrent users, load average 20-40 typical
```

Two differences that matter:

- **cgroup CPU restriction.** `Cpus_allowed_list` on login23 was `6-127,134-255`
  — CPUs 0-5 and 128-133 withheld, **all of them on node 0**. So node 0 offers 20
  usable threads against node 7's 32. A benchmark run there shows
  `Ops_Node0/Ops_Node1 ~ 0.67`; on an exclusive compute node it is **1.000-1.008**
  (measured, campaign01). Any per-node number taken on a login node is skewed.
- **THP differs** — see §2.2.

NERSC's own documentation states login nodes are NPS=1. **The login nodes we saw
report 8 NUMA domains**, i.e. NPS=4, same shape as compute. Do not rely on either
claim: this is exactly why `machine_profile.sh` verifies rather than assumes.

### 1.3 NUMA topology (compute node)

```
available: 8 nodes (0-7)
node 0 cpus: 0-15,128-143      size: ~64205 MB
node 7 cpus: 112-127,240-255   size: ~63927 MB

node distances:
        0    1    2    3    4    5    6    7
   0:  10   12   12   12   32   32   32   32
   ...
   7:  32   32   32   32   12   12   12   10
```

Three tiers, unlike stormbreaker's flat matrix: **10 local, 12 within-socket, 32
cross-socket**. Nodes 0 and 7 are on different sockets, so every remote access in
our binding pays the full **32 — a 3.2x penalty**, against stormbreaker's uniform
remote of 20 (**2.0x**). The paper's "about 3x maximum latency difference" matches.

### 1.4 The bandwidth consequence of NPS4 — easy to miss

Both machines bind **two** NUMA nodes, each with its own memory. What differs is
what fraction of a socket each node commands:

| | stormbreaker node | Perlmutter node |
|---|---|---|
| what it is | a **whole socket** | a **quarter socket** |
| memory channels | all 8 of the socket's | **2** of the socket's 8 |
| bandwidth | full socket | 204.8 / 4 = **51.2 GB/s** |
| capacity | ~128 GB | 64 GB |

NERSC documents 204.8 GB/s per CPU and NPS=4; NPS splits the memory controllers
evenly, and node capacity corroborates it (512 / 8 = 64 GB).

So binding nodes 0+7 gives **4 of the node's 16 memory channels** for 64 of 256
threads — roughly **4x less bandwidth per thread than stormbreaker's nodes 0+1**,
which are two whole sockets and all 16 channels. This is the same trap as §9.2 of
`stormbreaker.md`, one level down: 2 of 8 nodes costs you three-quarters of the
bandwidth as well as three-quarters of the capacity.

---

## 2. Kernel / runtime state

### 2.1 AutoNUMA — no root, so `numactl -b` is the only control

```
/proc/sys/kernel/numa_balancing = 1     (and you cannot change it)
```

There is no root here. The kernel knob stays at 1. The contrast is per-process,
via `numactl --balancing`, selected by `campaign.py --an-mode`:

```
--an-mode off  ->  numactl      --cpunodebind=0,7 --membind=0,7
--an-mode on   ->  numactl -b   --cpunodebind=0,7 --membind=0,7
```

`campaign.py` records in the manifest that the arm was forced and what the kernel
actually reported, so a per-process contrast is never mistaken for a system-wide
one. Both arms of `campaign01-perl` carry:

```
FORCED via --an-mode (kernel numa_balancing=1; numactl --balancing only, not a kernel toggle)
```

**Verified `-b` is a real switch here** (pilot, login node, 2M keys, 30 s,
`--DS_config=regular`, idle baseline sampled over an equal window):

| | pte_updates | pages_migrated |
|---|---|---|
| idle baseline | 5,874 | 3,388 |
| AN off | 4,112 | 3,098 |
| **AN on** | **2,279,624** | **685,828** |

The off arm is *below* the idle baseline — nothing attributable. This matches
stormbreaker's finding that `-b` is a hard switch, not a soft one.

**Caveat, unresolved:** that pilot ran on a **login node under THP=never**. The
campaign ran on compute under **THP=always**, where migration granularity is 2 MB.
AutoNUMA's measured effect in `campaign01-perl` was **null** (§9.7), and the data
shows no migration transient — but that has *not* been confirmed by `vmstat`
under campaign conditions. See §9.7 for the open question and the cheap experiment
that would close it.

### 2.2 THP — compute matches stormbreaker, login does not

```
compute (nid005645, nid004673) : [always] madvise never
stormbreaker                   : [always] madvise never
login23                        : always madvise [never]
```

**Compute nodes match stormbreaker exactly**, so THP is not a confounder between
the two machines' campaigns. It *is* a confounder between login-node pilots and
compute-node campaigns — do not carry migration counters across that line.

Note you can *disable* THP per process (`prctl(PR_SET_THP_DISABLE)`) but you
cannot enable it when the system says `never`.

---

## 3. Software dependencies

### 3.1 Modules — pinned, not defaults

`machine_profile.sh` loads these for `perlmutter-cpu`:

| module | version | override env var |
|---|---|---|
| python | `python/3.13-26.8.0` | `NUMATYPING_PYTHON` |
| LLVM | `PrgEnv-llvm/21.1.4` | `NUMATYPING_LLVM` |
| spack | `spack/1.1.1` | `NUMATYPING_SPACK` |
| cmake | `cmake/3.30.2` | `NUMATYPING_CMAKE` |
| spack env | `NUMATyping` | `NUMATYPING_SPACK_ENV` |

Versions are pinned deliberately. `PrgEnv-llvm` currently defaults to 21.1.4,
which happens to match stormbreaker's LLVM 21 major — a default that moves to 22
would drag `numa-clang-tool`'s AST-matcher APIs with it.

**Two module traps:**

1. **`module load python` with no version does not exist here** and fails. The
   stale root `*.slurm` files use exactly that. The versioned name is required.
2. **The system `python3` is 3.6.15**, and `campaign.py` calls
   `subprocess.run(capture_output=True)`, which is 3.7+. Without the python
   module every campaign dies instantly in `git_gate()` with
   `TypeError: __init__() got an unexpected keyword argument 'capture_output'`.
   This is easy to miss because **`run.py` survives on 3.6** — it never calls
   `git_status()` — so a passing smoke test proves nothing about `campaign.py`.
   The profile now hard-fails if the interpreter is older than 3.7.

Also note **`module load` inside a shell pipeline is a no-op** (it runs in a
subshell and the environment changes are discarded). `module load X 2>&1 | tail`
appears to succeed and changes nothing.

### 3.2 Libraries

```
jemalloc  5.3.0, spack: /global/homes/k/kiwo9430/.spack/opt/spack/linux-zen3/
                        jemalloc-5.3.0-rd7q2icc4g7g2bsbdhaxbbv4wkdug7w3
hwloc     system: /usr/lib64/libhwloc.so
libnuma   system: /usr/lib64/libnuma.so, headers /usr/include/numa.h
```

There is **no system jemalloc** — spack supplies it, which is the reverse of
stormbreaker (apt jemalloc, no spack). `scripts/env.py` resolves it via
`spack location -i jemalloc`; the `if jr:` graceful-empty fallbacks that make
stormbreaker work without spack are still required and unchanged.

### 3.3 Filesystems and session persistence

```
/global/homes/...  GPFS, mounted on every login node
$SCRATCH           /pscratch/sd/k/kiwo9430, Lustre, PURGED periodically
work in            $SCRATCH/NUMATyping
```

`Campaigns/` lives on **scratch and is subject to purge**. Copy anything you care
about to `$HOME` or CFS once a campaign finishes. This is the operational
difference from stormbreaker, where the risk was a full disk rather than a purge.

---

## 4. Build chain — what had to be fixed

Four defects blocked the port. All are fixed and committed; all fixes are
conditional so stormbreaker's path is unchanged.

### 4.1 `machine.env` was tracked despite `.gitignore` (commit `6140a132`)

`.gitignore` has no effect on already-tracked files. Regenerating `machine.env`
on a new machine dirtied the tree, and `campaign.py`'s `git_gate()` refuses to
run dirty — so **every campaign was blocked before it started**. Untracked with
`git rm --cached`. On stormbreaker, re-run `bash scripts/detect_machine.sh $PWD`
after pulling; the regenerated values are byte-identical to the committed ones.

### 4.2 `numa-clang-tool` aborted at exit (commit `473ce150`)

**This is the one `stormbreaker.md` §4.4 warned about, but not for the predicted
reason.** The AST-matcher APIs were fine — PrgEnv-llvm is 21.1.4, the same major
as stormbreaker's 21.0.0git, and the tool compiled unmodified.

The real fault was linking. LLVM ships in two shapes:

- `libclang-cpp.so` **depends on** `libLLVM.so` — one shared copy (stormbreaker's
  hand-built trunk LLVM).
- `libclang-cpp.so` **statically embeds** its own complete copy of LLVM and has no
  `DT_NEEDED` on `libLLVM.so` (**NERSC PrgEnv-llvm/21.1.4**).

`src/CMakeLists.txt` linked both unconditionally. In the second shape that loads
**two full sets of LLVM globals**; they interpose, both run their static
destructors at exit, and the second frees the first's storage:

```
munmap_chunk(): invalid pointer
~vector<llvm::TensorSpec>() -> __cxa_finalize -> _dl_fini -> exit
```

The passes finish *before* this fires, so it reads as harmless teardown noise —
but the process still aborts, and `numafy.py` runs the tool with `check=True`, so
**every transform failed with SIGABRT and wrote no output**.

The CMake now inspects `libclang-cpp.so` with `readelf` and links `libLLVM.so`
only when it is genuinely a separate copy. Override with
`-DLINK_LLVM_DYLIB=ON/OFF`.

### 4.3 Login/compute topology divergence (commit `473ce150`)

`detect_machine.sh` probes whatever machine it runs on, which is wrong when you
configure on a login node and run on compute. `machine_profile.sh` still probes
(the probing logic is unchanged and remains the source of truth) but then
reconciles against the profile's pinned expectations, warns loudly on a
mismatch, and pins. `numatyping_verify_topology` re-checks on the compute node and
**fails the job rather than measuring the wrong nodes**.

### 4.4 Plotting assumed stormbreaker's thread count (commit `0a951209`)

`bar_plot_ycsb.py` filters rows by `num_threads == target_threads`, hardcoded to
**80** (stormbreaker) or **128** under `--perlmutter` (the paper's number that
§9.2 says not to use). A correctly configured Perlmutter run uses
`PARTITION_THREADS = 64`, so the filter matched **zero rows**, every config fell
through to `append(0)`, and the script emitted a fully rendered chart with axes,
legend and labels but **no bars — exiting 0**. An empty chart rather than an
error.

There is now `--threads`, and without it the count is auto-detected from the data
when unambiguous. Stormbreaker is unaffected (its data is 80, so detection
returns the same value). The same commit fixes figures overwriting each other in
campaign mode, where the filename did not encode `--AN`.

### 4.5 Build artifacts (rebuild per machine; all gitignored)

```
numa-clang-tool/build/bin/clang-tool          13.8 MB
unified-memory-framework/build/lib/libumf.a   233 KB
unified-memory-framework/build/lib/libjemalloc_pool.a  15.7 KB
Output/ycsb/bin/ycsb                          860 KB
```

UMF is built here with `-DCMAKE_BUILD_TYPE=Release` and tests/examples/benchmarks
off, which is why `libumf.a` is smaller than stormbreaker's 331 KB. Both are
linked statically into the benchmarks, as before.

`numaLib/numa_nodemap.hpp` needed **no changes** — its runtime detection is
genuinely portable, exactly as `stormbreaker.md` §4.1 promised.

---

## 5. machine.env on Perlmutter

Generated by `detect_machine.sh`, reconciled by `machine_profile.sh`:

```shell
export NUM_PHYS_NODES=8
export CPU_NODES=0,1,2,3,4,5,6,7
export NUM_CPU_NODES=8
export TOTAL_THREADS=244          # see caveat
export NUM_PARTITIONS=2
export NUMA_NODE_ORDER=0,7
export PARTITION_THREADS=64
export NUMACTL_BIND="--cpunodebind=0,7 --membind=0,7"
```

The outside-in rule in `detect_machine.sh` yields `0,7` on an 8-node machine with
no special-casing — leave it alone, as `stormbreaker.md` §8 instructs.

**`TOTAL_THREADS=244` is a login-node artifact.** It comes from `nproc`, which the
login cgroup restricts; a compute node reports 256. It is informational only —
nothing derives binding or thread count from it — but do not quote it as a machine
fact. `PARTITION_THREADS` is derived from sysfs cpulists, not `nproc`, so it is
correct at 64 regardless of where it was generated.

### `numa<T,1>` really does land on node 7 — verified

Not inferred from the header. A test allocated from each UMF pool, faulted the
pages in, and asked the kernel via `get_mempolicy(MPOL_F_NODE|MPOL_F_ADDR)`:

```
logical 0 -> physical node 0   OK
logical 1 -> physical node 7   OK
```

A control run with `NUMA_NODE_ORDER=7,0` swapped the placement, ruling out
coincidence. Since `MAX_NODE` defaults to 1, `numa<T,MAX_NODE>` is `numa<T,1>` ->
node 7.

---

## 6. Running campaigns

### 6.1 The scripts

```
scripts/ycsb_campaigns.sh     sourceable table of all six campaign definitions
scripts/ycsb_campaign.slurm   one campaign, one AutoNUMA arm
scripts/submit_campaign.sh    submits both arms, chained
scripts/watch_campaign.sh     live monitoring from a login node
scripts/analyze_duration.py   how long a config must run to separate configs
```

Submit:

```shell
bash scripts/submit_campaign.sh campaign01          # both arms, chained
bash scripts/submit_campaign.sh campaign01 --dry    # validate, submit nothing
```

Monitor:

```shell
bash scripts/watch_campaign.sh            # queue + per-job status + runs finished N/28
bash scripts/watch_campaign.sh <jobid>    # follow one job
bash scripts/watch_campaign.sh --check    # one-shot, exits non-zero on trouble
```

### 6.2 Why the arms are chained by default

Arm `on` is submitted with `--dependency=afterok` on arm `off`:

1. **If the first arm fails the second never starts**, saving ~3.6 node-hours.
   The wrapper exits non-zero on a failed run, a short CSV, a topology mismatch
   or a memory floor breach, so `afterok` catches all of them.
2. **Both arms append to the same `manifest.md`.** Run concurrently they race:
   both can find no manifest, both create one, and the commit/parameter guard
   that makes the AN contrast trustworthy is silently bypassed.

`--parallel` opts out of both.

### 6.3 DO NOT COMMIT between the two arms

`campaign.py` compares HEAD against the commit recorded in the slug's manifest
and **hard-exits** on a mismatch:

```
Refusing: experiment '<slug>' was started at commit X, but HEAD is Y.
```

That includes the gap between arm 1 finishing and arm 2 starting. Land all code
changes *before* launching arm 1. Stage edits outside the repo (`/tmp`) while a
campaign is in flight — an untracked file also trips the `git_gate()`.

### 6.4 Failure handling — `campaign.py` does not abort on a failed run

It prints `rc=N  FAILED (see log)` and continues (`campaign.py:400`). A job can
consume its full walltime, write empty CSVs and **exit 0**. The slurm wrapper
therefore adds four layers:

| layer | what it catches |
|---|---|
| **preflight** | ~40 s run at 1/1000th keyspace before committing hours — missing binary, broken UMF link, unbindable node, jemalloc rpath |
| **live trip** | first `FAILED` / `ABORT [` / `Refusing:` / `Traceback` cancels the job (`ABORT_ON_RUN_FAILURE=0` opts out) |
| **memory watchdog** | samples per-node free memory every 30 s; aborts below `MEM_LOW_WATER_MB` (default 6144) with a diagnosis rather than being OOM-killed |
| **validation** | row counts vs `configs x (duration/interval + 1)`; short files exit non-zero |

Plus an always-printed `STATUS: OK` / `STATUS: FAILED rc=N` banner with reason,
elapsed minutes and log tail, and a `USR1` trap warning 10 minutes before the
walltime.

`DRY_RUN=1` validates the whole path without running the campaign.

---

## 7. Reproducing the build on Perlmutter

```shell
cd $SCRATCH/NUMATyping

# everything, auto-detected
bash scripts/configure_machine.sh

# or stage by stage
bash scripts/configure_machine.sh --stages=topo
bash scripts/configure_machine.sh --stages=clangtool -j16
bash scripts/configure_machine.sh --stages=umf -j16
bash scripts/configure_machine.sh --stages=suites --suites=ycsb -j16

# smoke test
source scripts/machine_profile.sh
python3 scripts/run.py --bench ycsb --name smoke --mix uniform --hash djb2 \
    --keys 2000000 --tables 100 --buckets 1009 \
    --threads $PARTITION_THREADS --warmup 5 --duration 30 --interval 10 \
    --configs numa/numa numa/regular

# campaign
bash scripts/submit_campaign.sh campaign01
```

Login-node etiquette: building with `-j16` is fine. **Running a multi-minute
64-thread benchmark on a login node is not** — it is outside NERSC policy and the
numbers are skewed anyway (§1.2). Use `--constraint=cpu` compute nodes.

---

## 8. Porting checklist (Perlmutter side)

Machine-specific, re-derive here:
- [ ] `bash scripts/configure_machine.sh` — does all of the below
- [ ] `machine.env` — gitignored, does NOT arrive with a clone
- [ ] `numa-clang-tool/build/`, `unified-memory-framework/build/`, `Output/<SUITE>/`
- [ ] python module ≥ 3.7 (system python3 is 3.6, and `module load python` fails)
- [ ] `--constraint=cpu` on every job, or you get a 4-NUMA-domain GPU node

Portable, leave alone:
- [ ] `numaLib/numa_nodemap.hpp` — needed no changes
- [ ] `detect_machine.sh` probing and its outside-in rule (yields `0,7` unaided)
- [ ] the `if jr:` empty-spack fallbacks (still required for stormbreaker)
- [ ] `benchmarks.py` bench→suite mapping

Still outstanding:
- [ ] `perfYCSB.py:106` still hardcodes `0,1` with no machine_env lookup — will
      bind wrongly here (inherited from `stormbreaker.md` §4.3, not yet fixed)
- [ ] the 13 stale `ycsb_job*.slurm` files in the repo root are superseded by
      `scripts/ycsb_campaign.slurm` but still present
- [ ] AutoNUMA null result not yet confirmed by `vmstat` under campaign
      conditions (§9.7)

---

## 9. YCSB on Perlmutter

### 9.1 Sizing — 3x, load factor preserved

Definitions live in `scripts/ycsb_campaigns.sh`. `LF = (keys/2) / (tables x buckets)`.

| campaign | keys | buckets | LF | mix | theta | payload |
|---|---|---|---|---|---|---|
| campaign01 | 300M | 200000 | 0.750 | uniform | 0.7 | 128 |
| campaign01.1 | 300M | 90011 | 1.666 | uniform | 0.7 | 128 |
| campaign02 | 300M | 200000 | 0.750 | zipfian | 0.9 | 128 |
| campaign02.1 | 300M | 90011 | 1.666 | zipfian | 0.9 | 128 |
| campaign03 | 300M | 90011 | 1.666 | zipfian | 0.7 | 64 |
| **campaign04** | **10M** | **6669** | 0.750 | uniform | 0.7 | 4096 |

Shared: `tables=1000`, `warmup=60`, `duration=300`, `interval=20`, `hash=mix`,
4 configs x 7 workloads, `--threads 64`.

### 9.2 Threads: 64 IS the maximum

Nodes 0+7 have 16 physical cores each x 2 SMT = **64 hardware threads**. Asking
for 128 oversubscribes 2x — that is what the archived Perlmutter runs did, and
`stormbreaker.md` §9.8 warns against treating `t=128` as validated for a two-node
binding. `--threads 64` is "all the threads the bound nodes have", not a
compromise.

### 9.3 Working-set growth — measured, and smaller than the 4x bound implies

RSS sampled during real runs:

| keys | prefill | peak seen | ratio | note |
|---|---|---|---|---|
| 20M | 1628 MB | 6434 MB | 3.95x | plateaued at t=35 s |
| 60M | 4883 MB | 9356 MB | 1.92x | still climbing at t=340 s |
| **300M (campaign01)** | 24.4 GB | **44.6 GB** | 1.83x | measured peak, both arms |

Small keyspaces saturate fast; larger ones do not saturate within a run at all.
The insert rate is roughly scale-invariant — half the keyspace is absent at every
scale — so growth is about 9 GB per 360 s window regardless of `keys`.

**Consequence: the 4x steady-state figures in `stormbreaker.md` §9.2 are an
asymptote a 300 s run never approaches.** campaign01 peaked at **44.6 GiB against
125.6 GiB available**, not the 95 GiB the 4x bound predicts. Memory fully reclaims
between runs (measured: free returns to baseline), so there is no accumulation
across the 28 runs.

### 9.4 Warmup stays at 60 s

Steady state is unreachable at 3x scale in any acceptable run length, so a longer
warmup would shift the measurement from ~1.15x to ~1.25x prefill and break
protocol match with stormbreaker for no real gain. Measured throughput drift
across a 300 s window is **0.0-0.2%**, and every config runs a fresh process from
identical prefill, so configs stay comparable.

### 9.5 Runtime and cost — measured

```
per run       ~10.0 min  (28 runs: 300 s + 60 s warmup + ~150 s prefill + settle)
per arm       4:38:00    (both arms, identically)
both arms     ~9.2 node-hours
walltime      6 h requested; ~1.4 h margin
```

`--time=06:00:00` is right. QOS `regular` allows 2 days; the partition caps at 12 h.

### 9.6 campaign01-perl results (2026-08-24)

56 runs, zero failures, all 14 CSVs at 64/64 rows.

`numa/numa` vs `numa/regular`, per workload:

| workload | AN_off | AN_on |
|---|---|---|
| A | +13.65% | +14.44% |
| B | +22.35% | +16.76% |
| C | +25.61% | +13.26% |
| D | +17.77% | +21.95% |
| E | +16.45% | +15.41% |
| F | +14.14% | +13.47% |
| AD | +15.72% | +17.01% |
| **Geomean** | **+17.88%** | **+16.01%** |

Node balance `Ops_Node0/Ops_Node1` = **0.998-1.004** across all configs.

### 9.7 The AutoNUMA null result — and the open question

AutoNUMA effect (AN_on vs AN_off, geomean): `numa/numa` **-1.03%**,
`numa/regular` **+0.56%**, `regular/numa` **-2.40%**, `regular/regular` **+1.33%**.
Every config within ±2.5% — noise.

There is also **no migration transient**: AN_on tracks AN_off from the first
interval and stays flat, where real migration work would show an early cost
and/or a late improvement.

Working hypothesis: AutoNUMA has **no affinity signal to act on** here. Uniform
random access over 300M keys and a 40 GiB working set means any page is touched
rarely and from either node with ~equal probability, and AutoNUMA's two-stage
filter needs consecutive faults from the same node. THP `always` makes it worse —
a 2 MB page holds ~13,000 records and is certain to be touched from both nodes.

**This is not confirmed.** The `-b` pilot (§2.1) ran on a login node under
THP=never at 4 KB granularity, where pages *are* re-touched constantly — which is
why it saw 685,828 migrations. To close the gap, run one campaign01 config under
`--an-mode on` on an exclusive compute node with THP=always, sampling
`/proc/vmstat` around it (~10 min, ~0.2 node-hours, `debug` QOS). Near-zero
counters would establish the null as a mechanism rather than an absence.

### 9.8 Why the gaps are larger here than on stormbreaker

Two structural reasons, both from §1.3 and §1.4:

- **Remote access costs 3.2x local here vs 2.0x on stormbreaker** (nodes 0 and 7
  are cross-socket, distance 32).
- **Each node commands a quarter of its socket's memory bandwidth**, so a
  misplaced access consumes scarce cross-socket bandwidth as well as paying
  latency. Roughly 4x less bandwidth per thread than stormbreaker's nodes 0+1.

Note `regular/numa` (1.03-1.06x) beats `numa/regular` (1.00x): **numa-typing the
data structure is the more valuable half**; typing only the threads buys almost
nothing. A plausible additional mechanism — untested — is that numa-typed
allocation guarantees an even split across the two nodes' memory controllers,
whereas first-touch can skew, which is expensive when a node has only 2 channels.

### 9.9 Analysis

```shell
python3 scripts/an_comparison.py --campaign campaign01-perl --graph
python3 scripts/bar_plot_ycsb.py --AN both --campaign campaign01-perl --ROOT_DIR $PWD
python3 scripts/bar_plot_ycsb.py --AN both --baseline-from off --campaign campaign01-perl --ROOT_DIR $PWD
```

`--baseline-from off` puts both AutoNUMA modes on one scale so solid and striped
bars are directly comparable. Output lands in
`Campaigns/ycsb/<slug>/` and `Campaigns/ycsb/<slug>/comparisons/`.

`scripts/analyze_duration.py` answers "how long must a config run": it differences
cumulative ops into per-interval throughput and runs Welch's t-test over growing
windows, reporting the shortest window that is significant and stays significant.
