# `scripts/` — the experiment toolchain

Five scripts carry the normal workflow, in this order:

```
numafy.py  ──►  campaign.py  ──►  an_comparison.py
 transform      run the sweep      tables + AutoNUMA figures
 + compile          │
                    └──►  bar_plot_ycsb.py
                          normalized bar charts

run.py  — the quick, no-provenance version of campaign.py, for scratch runs
```

All commands are run **from the repository root**, not from `scripts/`.

---

## `numafy.py` — transform a benchmark for NUMA typing

Runs the Clang tool (`numa-clang-tool`) over a suite and writes the transformed,
ready-to-compile copy into `Output/<SUITE>/`. This is what turns `numa<T,k>`
annotations into per-node specializations that recursively pin every member type.

```shell
python3 scripts/numafy.py --ROOT_DIR=$PWD --umf=1 ycsb
```

| option | meaning |
|---|---|
| `SUITE` (positional) | `ycsb`, `DataStructureTests`, `Histogram`, … (default `Histogram`) |
| `--ROOT_DIR` | path to the NUMATyping root |
| `--umf` | `1` = UMF/jemalloc allocator, `0` = `numa_alloc_onnode` (default `1`) |
| `--debug` | build for GDB |
| `--jemalloc-root` | jemalloc prefix, if it isn't on the default path |

Then compile the transformed suite:

```shell
make -C Output/ycsb clean
make -C Output/ycsb ROOT_DIR=$PWD UMF=1
```

**You rarely call this directly** — `campaign.py --numafy` and `run.py --numafy`
do the transform *and* the compile for you, which is the safer habit (see the
warning under `campaign.py`).

---

## `campaign.py` — an archival, provenance-tracked sweep

Runs every workload × every config, and records exactly what produced the numbers.
**It refuses to start on a dirty git tree** — you commit first, so the manifest is
always reproducible.

```shell
python3 scripts/campaign.py --slug campaign05 --numafy \
  --purpose "uniform, 4 KB payload, reworked allocator" \
  --mix uniform --hash mix --payload 4096 --warmup 60 \
  --threads 80 --buckets 13337 --tables 1000 --keys 20000000 \
  --duration 300 --interval 20
```

Output layout — the slug is the experiment, and both AutoNUMA runs share it:

```
Campaigns/ycsb/campaign05/
    manifest.md          identity: commit, machine, every parameter, run list
    git_diff.txt         the committed diff at that commit
    AN_off/  ycsb_<workload>.csv + .log      data only
    AN_on/   ycsb_<workload>.csv + .log
```

**AutoNUMA is not a flag.** The script reads `/proc/sys/kernel/numa_balancing`
and writes into `AN_off/` or `AN_on/` accordingly. To get both halves, run the
*identical* command twice with the sysctl flipped in between:

```shell
echo 1 | sudo tee /proc/sys/kernel/numa_balancing     # then re-run the same command
```

The second run appends to the same `manifest.md`. Commit **and** every parameter
must match the first run, or it refuses — that guard is what makes the AN_off vs
AN_on comparison trustworthy.

| option | meaning |
|---|---|
| `--slug` | experiment name; the folder under `Campaigns/<bench>/` (required) |
| `--purpose` | one-line description written into the manifest |
| `--bench` | `ycsb` (default) or `bst` |
| `--numafy` | transform + recompile before running |
| `--no-umf` | compile without UMF |
| `--configs` | default: `numa/numa numa/regular regular/numa regular/regular` |
| `--workloads` | default: the benchmark's full list (7 for ycsb) |
| `--refresh` | seconds to settle memory between configs (default 30) |
| `--force` | overwrite an existing AN folder / allow a commit mismatch |

Benchmark parameters come from `benchmarks.py` and are added to the CLI
automatically — for ycsb: `--mix --hash --theta --payload --warmup --threads
--buckets --tables --keys --duration --interval`.

> ⚠️ **Always pass `--numafy`.** The UMF static libraries are build artifacts:
> editing allocator source changes nothing until `unified-memory-framework/build`
> is rebuilt. Campaigns 01–03 unknowingly ran a months-old allocator this way.
> Verify with `nm Output/ycsb/bin/ycsb | grep BindThread`.

Wall time for the default ycsb sweep: 28 runs × (warmup + duration + refresh).
At `--warmup 60 --duration 300` that is about **3 hours per AutoNUMA mode**, so
run it under `screen`.

---

## `run.py` — a quick scratch run

Same machinery, no git gate and no manifest. Use it to check a parameter before
committing three hours to it.

```shell
python3 scripts/run.py --name pilot_C --workload "C-50-50-50,C-100-0-50" \
  --mix zipfian --theta 0.7 --payload 64 --buckets 30011 --tables 1000 \
  --keys 100000000 --warmup 60 --duration 180 --interval 20 \
  --configs numa/numa numa/regular
```

Writes a bare CSV to `Runs/<bench>/AN_<mode>/<name>.csv` (plus `.log`). Same
AutoNUMA auto-detection as `campaign.py`.

| option | meaning |
|---|---|
| `--name` | output CSV base name (required) |
| `--workload` | a single workload (default: the benchmark's first) |
| `--outdir` | override the output directory |
| `--configs`, `--bench`, `--numafy`, `--no-umf`, `--refresh` | as in `campaign.py` |

Plus the same benchmark parameters from `benchmarks.py`.

---

## `bar_plot_ycsb.py` — normalized throughput bar charts

Grouped bars, one cluster per workload, every config normalized to a chosen
baseline. Solid bars are AutoNUMA on, striped are AutoNUMA off.

```shell
# both AutoNUMA modes, normalized to the paper's baseline (numa/regular)
python3 scripts/bar_plot_ycsb.py --campaign campaign04 --AN both

# the same data against the NUMA-unaware baseline
python3 scripts/bar_plot_ycsb.py --campaign campaign04 --AN both \
  --normalize regular/regular
```

With `--campaign`, figures land in `Campaigns/ycsb/<slug>/` as
`<baseline>_normalized_<workloads>.png` and `.pdf`.

| option | meaning |
|---|---|
| `--AN` | `on`, `off`, or `both` (required) |
| `--campaign` | read `Campaigns/ycsb/<slug>/` and write figures there |
| `--normalize` | baseline: `numa/regular` (default), `regular/regular`, `regular/numa` |
| `--workloads` | which workloads to plot (default: all 7) |
| `--stop` | ignore intervals past N seconds |
| `--perlmutter`, `--zipfian`, `--suffix` | legacy `Graphs/` layout selectors |

> ⚠️ **Each AutoNUMA mode is normalized to its own baseline**, so solid and
> striped bars are **not comparable to each other**. A taller solid bar means the
> gap widened, not that AutoNUMA was faster. For cross-mode comparisons use
> `an_comparison.py`.

---

## `an_comparison.py` — AutoNUMA comparison tables and figures

The cross-AutoNUMA analysis the bar charts structurally cannot show.

```shell
python3 scripts/an_comparison.py --campaign campaign04 --graph
```

Writes into `Campaigns/ycsb/<slug>/comparisons/`:

| file | contents |
|---|---|
| `AN_comparison.csv` | all 4 configs × both modes; the AutoNUMA effect per config; and how each config-vs-baseline gap shifts between modes |
| `AN_ON_comparison.csv` | AN_on throughput, then one table per config used as the baseline |
| `AN_OFF_comparison.csv` | the same for AN_off |
| `autonuma_effect.png/.pdf` | one labelled circle per workload per config — did AutoNUMA help or hurt |
| `gap_shift_numa-numa_vs_numa-regular.png/.pdf` | dumbbells: the numa/numa lead, AutoNUMA off → on |

| option | meaning |
|---|---|
| `--campaign` | campaign slug (required) |
| `--graph` | also render the two figures (needs both AutoNUMA modes) |
| `--baseline` | baseline for the gap-shift figure (default `numa/regular`) |
| `--compare` | config whose lead is plotted (default `numa/numa`) |
| `--bench`, `--ROOT_DIR` | as elsewhere |

Values are the **mean operations per reporting interval**, in millions —
`Total_Ops` is cumulative in the CSVs, so this is the final cumulative value
divided by the number of intervals. Percentages are `(config / baseline − 1)×100`,
positive meaning the config is faster. Every table ends with a **geomean** row
(geometric mean of the ratios, the correct aggregate for ratios).

Tables are stacked in one file with `== TITLE ==` separators, so they open cleanly
in Excel or Sheets.

---

## A full session, start to finish

```shell
# 1. commit — campaign.py refuses a dirty tree
git add -A && git commit -m "..."

# 2. AutoNUMA off half (~3 h)
echo 0 | sudo tee /proc/sys/kernel/numa_balancing
screen -dmS camp bash -c 'python3 scripts/campaign.py --slug campaign05 --numafy \
    --purpose "..." --mix uniform --payload 64 --duration 300'

# 3. AutoNUMA on half — the identical command (~3 h)
echo 1 | sudo tee /proc/sys/kernel/numa_balancing
# ... re-run exactly the same campaign.py line ...

# 4. analyse
python3 scripts/an_comparison.py --campaign campaign05 --graph
python3 scripts/bar_plot_ycsb.py  --campaign campaign05 --AN both
```

---

## The rest of `scripts/`

| script | role |
|---|---|
| `benchmarks.py` | the **only** file to edit when adding a benchmark or a parameter — `campaign.py` and `run.py` build their CLIs from it |
| `runExperiments.py` | the older BST/DataStructureTests runner |
| `runYCSB.py` | the older ycsb runner, superseded by `campaign.py` |
| `bar_plot_bst.py`, `line_plot_bst.py` | BST bar and time-series charts |
| `perfBST.py`, `perfYCSB.py`, `plot_perfs.py` | `perf`-based profiling runs and their plots |
| `detect_machine.sh` | probes the NUMA layout into `machine.env` |
| `env.py`, `load.py` | NERSC module/environment helpers |
| `fix_workload_commas.sh` | one-off repair for CSVs whose workload cell contained a comma |
