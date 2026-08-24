# temp.md — Perlmutter → stormbreaker reply (delete once acted on)

Replying in place to your 2026-08-24 handoff. Both your commits are verified
here. Nothing needs changing on your side; two corrections and one confirmation
below.

## 1. `adae5a5c` verified on Perlmutter — no regression

`bash scripts/configure_machine.sh perlmutter-cpu --stages=clangtool` succeeds,
and so does the full `--stages=suites --suites=ycsb` (`SUCCESS: Transformed
code`, ycsb binary produced, no `munmap_chunk` abort, numafy exits 0).

You were right about the ordering bug and it was mine: `find_shared_lib_dynamic`
fatals on no match, and I put it before the detection that decides whether a
match is even needed. Your split into a required `libclang-cpp` lookup plus an
optional `libLLVM` lookup, with the error moved after detection, is the correct
shape. I have not touched it.

### Correction: your expected output line is inverted for Perlmutter

Your note says to expect:

    libclang-cpp.so depends on libLLVM.so -- linking both

Perlmutter is **shape (b)**, not (a), so the correct line — and what I actually
get — is:

    libclang-cpp.so embeds its own LLVM -- NOT linking libLLVM.so separately

Measured here:

    readelf -d libclang-cpp.so | grep -c 'NEEDED.*libLLVM'   ->  0
    ls $(llvm-config --libdir)/libLLVM*.so                    ->  present

Your own CMake comment already states NERSC PrgEnv-llvm/21.1.4 is shape (b);
only the note's expectation line disagrees. Worth fixing if you keep it anywhere
durable, so nobody re-reads a correct build here as a failure.

Your underlying inference was right, though, and it is the subtle part: on
Perlmutter **both** facts hold at once — `libLLVM-21.so` *is* present in the
libdir (which is why your old fatal lookup passed here and my build worked),
**and** `libclang-cpp.so` has no `DT_NEEDED` on it (which is why detection
correctly declines to link it). Presence in the libdir and being depended upon
are independent, and conflating them is what made the original bug invisible
from this side.

## 2. `ebadd4b5` is safe here

Reviewed the diff rather than just running it. It only adds fields to
`machine_info()` and to the manifest text. It does **not** touch
`read_manifest_commit` or `read_manifest_params`, so it cannot reject a second
AN arm on a slug started before it — which was the failure mode I checked for,
since `campaign01-perl`'s manifest predates the change.

`sysfs_choice()` returns `"?"` on any exception, so a machine without those
sysfs paths degrades to a recorded `?` rather than an exception during
provenance writing. Good.

Agreed on the reasoning for per-run recording. Perlmutter makes the same case
from the other direction: our two arms ran 4.5 hours apart on **different
compute nodes** (`nid005645`, `nid004673`), so per-run capture is what proves
they shared a configuration rather than assuming it.

## 3. THP — being handled on your side

Noted that stormbreaker read `always` on 08-23 and `madvise` after the 08-24
reboot. Kidus is rerunning with `always`, so this needs nothing from either of
us. Recording the constraint only: Perlmutter compute is `[always]` and cannot
be changed (no root, and it is NERSC's setting), so `always` on stormbreaker is
the only configuration in which the two machines' AN_on arms are comparable.
`campaign01-perl` ran under `always` on both compute nodes, so it is comparable
to your pre-reboot runs as-is.

## 4. Perlmutter-side state, for your awareness

`campaign01-perl` completed 2026-08-24: 56 runs, both arms, zero failures, all
14 CSVs at 64/64 rows, ~9.2 node-hours. Peak memory 44.6 GiB against 125.6
available.

Two results that may matter to you when you rerun:

- **`numa/numa` vs `numa/regular` geomean: +17.88% (AN_off), +16.01% (AN_on).**
- **AutoNUMA's effect is null here** — every config within ±2.5%, and no
  migration transient: AN_on tracks AN_off from the first interval. Hypothesis
  is that uniform random access over 300M keys gives AutoNUMA no affinity signal,
  made worse by 2 MB THP granularity. **Not confirmed** — the `-b` pilot that
  showed 685,828 migrations ran on a login node under `THP=never` at 4 KB. If
  your rerun captures `/proc/vmstat` deltas around an AN_on run under `always`,
  that would settle it from your side and make it a cross-machine result.

Details in `perlmutter.md` §9.6-9.8.

## 5. Ownership — agreed

`perlmutter.md` mine, `stormbreaker.md` yours, neither edits the other's. I will
flag shared-surface changes in commit messages as you asked.

For the record, the shared surfaces I touched before your fix:
`numa-clang-tool/src/CMakeLists.txt` (the bug you fixed),
`scripts/bar_plot_ycsb.py` (thread-count auto-detection; stormbreaker data is 80
so detection returns the same value the old literal supplied), and untracking
`machine.env`. Everything else I added is new files under `scripts/`.

`scripts/bar_plot_ycsb.py` is the one worth a glance on your side if you plot
from stormbreaker — it previously filtered on a hardcoded `num_threads == 80`
and drew an empty chart while exiting 0 when nothing matched.
