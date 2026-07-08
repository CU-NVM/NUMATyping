#!/usr/bin/env python3
"""
campaign.py -- run a git-gated, benchmark-independent campaign with provenance.

A campaign is an archival, for-the-record sweep.  It refuses to run on a dirty
tree: you commit your changes yourself first (so you can watch `git status`),
then this writes everything needed to reproduce the run.

This script knows nothing benchmark-specific: it reads benchmarks.py for the
binary, CSV header, per-config command, and the benchmark's own CLI parameters
(added dynamically).  Add a benchmark by editing benchmarks.py only.

Output:
    Campaigns/<bench>/<slug>/AN_<mode>/
        manifest.md, git_diff.txt, <bench>_<workload>.csv/.log
    The experiment (slug) is the folder; AN_off/ and AN_on/ are siblings inside
    it, so an AutoNUMA on/off pair lives together.  A pair must share the same
    commit AND the same config -- both are enforced by refusing to write a run
    that disagrees with its sibling -- which is what makes the on/off comparison
    valid.  The run datetime lives in the manifest, not the folder name.
    plus a git tag  campaign/<slug>  on the current HEAD.

The generic helpers below (autonuma/git/machine/build/run_config/manifest) are
also imported by run.py.

Example:
    python3 scripts/campaign.py --slug uniform-paper --numafy \\
        --purpose "paper config, uniform, no payload" \\
        --mix uniform --hash djb2 --buckets 133300 --tables 1000 \\
        --keys 100000000 --duration 1200 --interval 20
"""
import argparse, subprocess, socket, platform, sys, time, datetime
from pathlib import Path
import benchmarks

ROOT = Path(__file__).resolve().parent.parent          # scripts/ -> repo root
DEFAULT_CONFIGS = ["numa/numa", "numa/regular", "regular/numa", "regular/regular"]


# ===================================================================== helpers
# (benchmark-independent; also imported by run.py)

def autonuma():
    """Return (value:int, folder:str) from /proc/sys/kernel/numa_balancing."""
    try:
        v = int(Path("/proc/sys/kernel/numa_balancing").read_text().strip())
    except Exception:
        v = -1
    return v, ("AN_on" if v == 1 else "AN_off")


def git_status(root):
    """Return (short_hash, commit_subject, dirty_lines[])."""
    def g(*a):
        return subprocess.run(["git", "-C", str(root), *a],
                              capture_output=True, text=True).stdout.strip()
    return (g("rev-parse", "--short", "HEAD") or "n/a",
            g("log", "-1", "--pretty=%s"),
            [l for l in g("status", "--porcelain").splitlines() if l.strip()])


def machine_info():
    cpu = ""
    for line in Path("/proc/cpuinfo").read_text().splitlines():
        if line.startswith("model name"):
            cpu = line.split(":", 1)[1].strip()
            break
    nodes = "?"
    try:
        for l in subprocess.run(["numactl", "-H"], capture_output=True,
                                text=True).stdout.splitlines():
            if l.startswith("available:"):
                nodes = l.split()[1]
                break
    except Exception:
        pass
    return {"host": socket.gethostname(), "cpu": cpu,
            "nodes": nodes, "kernel": platform.release()}


def build(root, bench, umf=True, do_numafy=False):
    """Optionally numafy the suite, then (re)compile Output/<bench>."""
    root = Path(root)
    if do_numafy:
        jr = subprocess.run("spack location -i jemalloc", shell=True,
                            capture_output=True, text=True).stdout.strip()
        cmd = f"python3 {root/'scripts'/'numafy.py'} --ROOT_DIR={root} --umf=1"
        if jr:
            cmd += f" --jemalloc-root={jr}"
        cmd += f" {bench}"
        print(f"--- numafy: {cmd}")
        subprocess.run(cmd, shell=True, executable="/bin/bash", check=True)
    folder = root / "Output" / bench
    print(f"--- compile: make -C {folder} {'UMF=1' if umf else ''}")
    subprocess.run(["make", "-C", str(folder), "clean"])
    subprocess.run(["make", "-C", str(folder), f"ROOT_DIR={root}"]
                   + (["UMF=1"] if umf else []), check=True)


def numactl_prefix(an_value):
    base = "numactl --cpunodebind=0,1 --membind=0,1"
    return ("numactl --balancing --cpunodebind=0,1 --membind=0,1"
            if an_value == 1 else base)


def run_config(argv, an_value, out_path, log_path, cwd=None):
    """Run a pre-built argv under numactl; append stdout->CSV, stderr->log."""
    full = numactl_prefix(an_value).split() + argv
    with open(out_path, "a") as out, open(log_path, "a") as log:
        log.write("# " + " ".join(full) + "\n")
        log.flush()
        return subprocess.run(full, stdout=out, stderr=log, cwd=cwd).returncode


def write_manifest(path, *, bench, binary, purpose, git_hash, git_subject,
                   machine, an_value, params, configs, workloads, slug):
    an_str = "on" if an_value == 1 else "off"
    with open(path, "w") as f:
        f.write(f"# {bench} campaign -- {purpose or 'run'}\n\n")
        f.write(f"- **experiment (slug):** {slug}\n")
        f.write(f"- **date:** {datetime.datetime.now():%Y-%m-%d %H:%M:%S}\n")
        f.write(f"- **purpose:** {purpose}\n")
        f.write(f"- **benchmark:** {bench}  (`{binary}`)\n")
        f.write(f"- **git commit:** {git_hash} -- {git_subject}\n")
        f.write(f"- **machine:** {machine['host']} - {machine['cpu']} - "
                f"{machine['nodes']} NUMA nodes - kernel {machine['kernel']}\n")
        f.write(f"- **AutoNUMA:** numa_balancing={an_value} ({an_str})\n\n")
        f.write("## Parameters\n| param | value |\n|-------|-------|\n")
        for k, v in params.items():
            f.write(f"| {k} | {v} |\n")
        f.write(f"| configs | {' '.join(configs)} |\n")
        f.write(f"| workloads | {', '.join(workloads)} |\n\n")
        f.write("## Files\n- `manifest.md` -- this file\n"
                "- `git_diff.txt` -- `git show HEAD` of the committed code\n"
                "- `*.csv` / `*.log` -- results\n")


def read_manifest_commit(manifest_path):
    """Best-effort: pull the short commit hash out of an existing manifest.md."""
    try:
        for line in Path(manifest_path).read_text().splitlines():
            if "**git commit:**" in line:               # "- **git commit:** 98c476d6 -- subj"
                return line.split("**git commit:**", 1)[1].strip().split()[0]
    except FileNotFoundError:
        pass
    return None


def read_manifest_params(manifest_path):
    """Parse the '## Parameters' table of a manifest into {param: value(str)}."""
    out = {}
    try:
        lines = Path(manifest_path).read_text().splitlines()
    except FileNotFoundError:
        return out
    in_tbl = False
    for line in lines:
        if line.strip().startswith("## Parameters"):
            in_tbl = True
            continue
        if in_tbl:
            if line.startswith("## "):                  # next section ends the table
                break
            if line.startswith("|") and "---" not in line:
                cells = [c.strip() for c in line.strip().strip("|").split("|")]
                if len(cells) == 2 and cells[0].lower() != "param":
                    out[cells[0]] = cells[1]
    return out


# ================================================================ campaign entry
def parse_args():
    # A pre-pass reads --bench so the full parser can pull that benchmark's params.
    pre = argparse.ArgumentParser(add_help=False)
    pre.add_argument("--bench", default="ycsb", choices=list(benchmarks.BENCHES))
    bench = pre.parse_known_args()[0].bench

    p = argparse.ArgumentParser(description="Run a git-gated benchmark campaign.")
    p.add_argument("--bench", default="ycsb", choices=list(benchmarks.BENCHES))
    p.add_argument("--slug", required=True, help="short descriptive slug, e.g. uniform-paper")
    p.add_argument("--purpose", default="", help="one-line description for the manifest")
    p.add_argument("--numafy", action="store_true",
                   help="numafy the suite and (re)compile Output/<bench> before running")
    p.add_argument("--no-umf", action="store_true", help="compile without UMF")
    p.add_argument("--configs", nargs="+", default=DEFAULT_CONFIGS)
    p.add_argument("--workloads", nargs="+", default=None, help="default: the bench's full list")
    p.add_argument("--refresh", type=int, default=30, help="seconds to settle memory between configs")
    p.add_argument("--force", action="store_true",
                   help="overwrite an existing AN folder / allow a commit mismatch with the sibling")
    benchmarks.add_bench_args(p, bench)            # <-- the only benchmark-specific args
    return p.parse_args(), bench


def git_gate():
    """Refuse unless the tree is clean; you commit yourself so the manifest is exact."""
    git_hash, subject, dirty = git_status(ROOT)
    if dirty:
        print("Refusing to start: the working tree is not clean.\n")
        print("Uncommitted / untracked changes:")
        for l in dirty:
            print("   " + l)
        print("\nA campaign must run on committed code so the manifest is reproducible.")
        print("Stage and commit yourself, then re-run:\n   git add <files> && git commit -m '...'")
        sys.exit(1)
    return git_hash, subject


def main():
    args, bench_name = parse_args()
    bench = benchmarks.BENCHES[bench_name]

    git_hash, subject = git_gate()
    if args.numafy:
        build(ROOT, bench_name, umf=not args.no_umf, do_numafy=True)

    an_value, an_folder = autonuma()
    binary    = str(ROOT / bench["binary"])
    workloads = args.workloads or bench["workloads"]
    exp_dir   = ROOT / "Campaigns" / bench_name / args.slug   # experiment = slug
    outdir    = exp_dir / an_folder                           # AN_off/ and AN_on/ are siblings
    other_an  = "AN_on" if an_folder == "AN_off" else "AN_off"
    params    = benchmarks.extract_params(args, bench_name)

    # ---- guards: don't clobber; keep an on/off pair identical (commit + config) ----
    if outdir.exists() and any(outdir.iterdir()) and not args.force:
        sys.exit(f"Refusing: {outdir} already exists and is non-empty.\n"
                 f"Pick a new --slug, or pass --force to overwrite.")
    sib_manifest = exp_dir / other_an / "manifest.md"
    sib_commit   = read_manifest_commit(sib_manifest)
    if sib_commit and sib_commit != git_hash and not args.force:
        sys.exit(f"Refusing: {other_an} for '{args.slug}' ran at commit {sib_commit}, "
                 f"but HEAD is {git_hash}.\n"
                 f"An AutoNUMA on/off pair must share a commit to be comparable. "
                 f"Re-run on that commit, use a new --slug, or pass --force.")
    # every config parameter must match the sibling too -- not just the commit
    cur_sig = {k: str(v) for k, v in params.items()}
    cur_sig["configs"]   = " ".join(args.configs)
    cur_sig["workloads"] = ", ".join(workloads)
    sib_sig = read_manifest_params(sib_manifest)
    if sib_sig:
        diffs = sorted(k for k in set(sib_sig) | set(cur_sig)
                       if sib_sig.get(k) != cur_sig.get(k))
        if diffs and not args.force:
            detail = "\n".join(f"    {k}: {other_an}={sib_sig.get(k)!r} vs this={cur_sig.get(k)!r}"
                               for k in diffs)
            sys.exit(f"Refusing: config differs from the {other_an} sibling of '{args.slug}':\n"
                     f"{detail}\nAn AutoNUMA on/off pair must share config to be comparable. "
                     f"Use a new --slug, or pass --force.")
    outdir.mkdir(parents=True, exist_ok=True)

    # ---- provenance: manifest + committed diff + tag ----
    write_manifest(outdir / "manifest.md", bench=bench_name, binary=binary,
                   purpose=args.purpose or args.slug, git_hash=git_hash,
                   git_subject=subject, machine=machine_info(),
                   an_value=an_value, params=params,
                   configs=args.configs, workloads=workloads, slug=args.slug)
    (outdir / "git_diff.txt").write_text(
        subprocess.run(["git", "-C", str(ROOT), "show", "HEAD"],
                       capture_output=True, text=True).stdout)
    subprocess.run(["git", "-C", str(ROOT), "tag", "-f", f"campaign/{args.slug}"], capture_output=True)

    print(f"Campaign: {outdir}")
    print(f"  experiment '{args.slug}' | commit {git_hash} ({subject}) | AutoNUMA {an_folder}")
    print(f"  paired with {other_an}: commit + config match ({sib_commit})\n" if sib_commit
          else f"  (no {other_an} sibling yet)\n")

    # ---- the sweep ----
    cwd = str(ROOT / bench["cwd"]) if bench["cwd"] else None
    for wl in workloads:
        safe = wl.replace(",", "_")
        out  = outdir / f"{bench_name}_{safe}.csv"
        log  = outdir / f"{bench_name}_{safe}.log"
        out.write_text(bench["header"] + "\n")
        p = dict(params); p["workload"] = wl
        for i, cfg in enumerate(args.configs):
            if autonuma()[0] != an_value:                       # reboot safety
                print(f"  ABORT [{wl}] {cfg}: numa_balancing changed (reboot?). Re-run.")
                sys.exit(2)
            th, ds = cfg.split("/")
            print(f"  [{wl}] {th}/{ds} ...", flush=True)
            rc = run_config(bench["argv"](binary, th, ds, p), an_value, out, log, cwd=cwd)
            print(f"      rc={rc}" + ("  FAILED (see log)" if rc else ""))
            if i < len(args.configs) - 1 and args.refresh > 0:
                subprocess.run(["sync"]); time.sleep(args.refresh)
    print(f"\nDone -> {outdir}")


if __name__ == "__main__":
    main()
