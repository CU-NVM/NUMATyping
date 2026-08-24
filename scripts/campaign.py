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
    Campaigns/<bench>/<slug>/
        manifest.md, git_diff.txt              <- shared experiment identity
        AN_off/  <bench>_<workload>.csv/.log   <- data only
        AN_on/   <bench>_<workload>.csv/.log
    The experiment (slug) owns ONE manifest: the shared identity (commit, params,
    configs, workloads) plus a "Runs" list that each AutoNUMA run appends to.
    Commit and config are enforced identical across every run of the experiment
    -- a run that disagrees with the existing manifest is refused -- which is what
    makes the AN_off/AN_on comparison valid.  The AN folders hold only data (the
    AutoNUMA state is the folder name; the run datetime lives in the Runs list).
    plus a git tag  campaign/<slug>  on the current HEAD.

The generic helpers below (autonuma/git/machine/build/run_config/manifest) are
also imported by run.py.

Example:
    python3 scripts/campaign.py --slug uniform-paper --numafy \\
        --purpose "paper config, uniform, no payload" \\
        --mix uniform --hash djb2 --buckets 133300 --tables 1000 \\
        --keys 100000000 --duration 1200 --interval 20
"""
import argparse, os, subprocess, socket, platform, sys, time, datetime
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
    # THP materially changes NUMA page-migration behaviour, so it belongs in the
    # provenance: it is set at runtime, reverts to the kernel default on reboot,
    # and nothing else on the machine records when it changed.
    def sysfs_choice(path):
        try:                                     # "always [madvise] never" -> "madvise"
            t = Path(path).read_text()
            return t[t.index("[") + 1:t.index("]")]
        except Exception:
            return "?"
    me = machine_env()
    return {"host": socket.gethostname(), "cpu": cpu,
            "nodes": nodes, "kernel": platform.release(),
            "bind": me["NUMACTL_BIND"],
            "node_order": me["NUMA_NODE_ORDER"] or "auto",
            "thp": sysfs_choice("/sys/kernel/mm/transparent_hugepage/enabled"),
            "thp_defrag": sysfs_choice("/sys/kernel/mm/transparent_hugepage/defrag"),
            "numa_balancing": autonuma()[0]}


def build(root, bench, umf=True, do_numafy=False):
    """Optionally numafy the suite, then (re)compile Output/<suite>.

    The bench key and the suite directory are not always the same -- bench "DS"
    lives in DataStructureTests/ -- so both numafy and the Makefile path use the
    suite name declared in benchmarks.py.
    """
    root = Path(root)
    suite = benchmarks.BENCHES[bench].get("suite", bench)
    if do_numafy:
        jr = subprocess.run("spack location -i jemalloc", shell=True,
                            capture_output=True, text=True).stdout.strip()
        cmd = f"python3 {root/'scripts'/'numafy.py'} --ROOT_DIR={root} --umf=1"
        if jr:
            cmd += f" --jemalloc-root={jr}"
        cmd += f" {suite}"
        print(f"--- numafy: {cmd}")
        subprocess.run(cmd, shell=True, executable="/bin/bash", check=True)
    folder = root / "Output" / suite
    print(f"--- compile: make -C {folder} {'UMF=1' if umf else ''}")
    subprocess.run(["make", "-C", str(folder), "clean"])
    subprocess.run(["make", "-C", str(folder), f"ROOT_DIR={root}"]
                   + (["UMF=1"] if umf else []), check=True)


MACHINE_ENV_DEFAULTS = {
    # stormbreaker literals -- what this script hardcoded before machine.env
    # existed.  Keeping them as the fallback means a checkout with no
    # machine.env behaves exactly as it always did.
    "NUMACTL_BIND":    "--cpunodebind=0,1 --membind=0,1",
    "NUMA_NODE_ORDER": "",
}


def machine_env(root=ROOT):
    """Parse ROOT/machine.env ('export K=V' / 'K=V') into a dict.

    machine.env is written by scripts/detect_machine.sh and pins the physical
    NUMA nodes this machine's experiment runs on.  Reading it here is what lets
    the same campaign run on stormbreaker (nodes 0,1) and on an 8-node machine
    (nodes 0,7) with no edit to this file.
    """
    env = dict(MACHINE_ENV_DEFAULTS)
    try:
        for line in (Path(root) / "machine.env").read_text().splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if line.startswith("export "):
                line = line[len("export "):]
            if "=" not in line:
                continue
            k, v = line.split("=", 1)
            v = v.split("#", 1)[0].strip().strip('"').strip("'")
            if v:
                env[k.strip()] = v
    except FileNotFoundError:
        pass
    return env


def numactl_prefix(an_value, root=ROOT):
    """numactl prefix; --balancing only when AutoNUMA is on."""
    bind = machine_env(root)["NUMACTL_BIND"]
    return f"numactl {'--balancing ' if an_value == 1 else ''}{bind}"


def run_config(argv, an_value, out_path, log_path, cwd=None):
    """Run a pre-built argv under numactl; append stdout->CSV, stderr->log."""
    full = numactl_prefix(an_value).split() + argv
    # numaLib/numa_nodemap.hpp reads NUMA_NODE_ORDER to map logical partition k
    # to a physical node.  Pass it explicitly so the thread pinning and the UMF
    # pools land on exactly the nodes numactl bound us to -- if these two ever
    # disagree, allocation on an unbound node fails at runtime.
    env = dict(os.environ)
    order = machine_env()["NUMA_NODE_ORDER"]
    if order:
        env["NUMA_NODE_ORDER"] = order
    with open(out_path, "a") as out, open(log_path, "a") as log:
        log.write("# " + " ".join(full) + "\n")
        if order:
            log.write(f"# NUMA_NODE_ORDER={order}\n")
        log.flush()
        return subprocess.run(full, stdout=out, stderr=log, cwd=cwd,
                              env=env).returncode


def write_or_append_manifest(path, *, bench, binary, purpose, git_hash, git_subject,
                             machine, an_value, params, configs, workloads, slug,
                             an_forced=False, kernel_an=None):
    """Shared experiment manifest at the slug level.  The first run writes the
    identity block + the Runs list; a later run (already verified by the guards to
    match on commit + config) just appends its own line to the Runs list."""
    an_str = "on" if an_value == 1 else "off"
    forced = (f" -- FORCED via --an-mode (kernel numa_balancing={kernel_an}; "
              f"numactl --balancing only, not a kernel toggle)" if an_forced else "")
    run_line = (f"- AN_{an_str} -- {datetime.datetime.now():%Y-%m-%d %H:%M:%S} "
                f"-- kernel {machine['kernel']} -- THP={machine['thp']} "
                f"-- numa_balancing={machine['numa_balancing']}{forced}\n")
    if path.exists():                                   # a sibling AN run already created it
        with open(path, "a") as f:
            f.write(run_line)
        return
    with open(path, "w") as f:
        f.write(f"# {bench} campaign -- {purpose or 'run'}\n\n")
        f.write(f"- **experiment (slug):** {slug}\n")
        f.write(f"- **purpose:** {purpose}\n")
        f.write(f"- **benchmark:** {bench}  (`{binary}`)\n")
        f.write(f"- **git commit:** {git_hash} -- {git_subject}\n")
        f.write(f"- **machine:** {machine['host']} - {machine['cpu']} - "
                f"{machine['nodes']} NUMA nodes - kernel {machine['kernel']}\n")
        f.write(f"- **numa binding:** `{machine['bind']}` - "
                f"NUMA_NODE_ORDER={machine['node_order']}\n")
        f.write(f"- **kernel tunables:** THP={machine['thp']} "
                f"(defrag={machine['thp_defrag']}) - "
                f"numa_balancing={machine['numa_balancing']}\n\n")
        f.write("## Parameters\n| param | value |\n|-------|-------|\n")
        for k, v in params.items():
            f.write(f"| {k} | {v} |\n")
        f.write(f"| configs | {' '.join(configs)} |\n")
        f.write(f"| workloads | {', '.join(workloads)} |\n\n")
        f.write("## Runs\n")                            # each AN run appends one line here
        f.write(run_line)


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
    p.add_argument("--an-mode", choices=["auto", "on", "off"], default="auto",
                   help="AutoNUMA mode. 'auto' (default) reads "
                        "/proc/sys/kernel/numa_balancing -- correct when you can "
                        "toggle the kernel knob. 'on'/'off' FORCE the mode where "
                        "you cannot (no root, e.g. a shared HPC system): the "
                        "contrast then comes from numactl --balancing alone, "
                        "which is weaker than a kernel toggle. The manifest "
                        "records that it was forced and what the kernel actually "
                        "reported, so the two never get confused.")
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

    # ---- announce the NUMA binding: it is machine-specific and silently wrong
    # bindings are the worst failure mode here (a full campaign of valid-looking
    # numbers taken on the wrong nodes).  Say out loud what we resolved.
    me = machine_env()
    if not (ROOT / "machine.env").exists():
        print("WARNING: no machine.env found -- falling back to "
              f"'{MACHINE_ENV_DEFAULTS['NUMACTL_BIND']}' (stormbreaker).\n"
              "         Run  bash scripts/detect_machine.sh  first on a new machine.\n")
    print(f"--- numa binding: {me['NUMACTL_BIND']}"
          f"   NUMA_NODE_ORDER={me['NUMA_NODE_ORDER'] or 'auto-detect'}")

    # The benchmarks split --threads evenly across the two partitions
    # (num_threads / 2, integer division), so an odd value silently drops one and
    # a value larger than the bound nodes' CPU count oversubscribes.  Warn rather
    # than override: the thread count is a recorded experiment parameter.
    pt = me.get("PARTITION_THREADS")
    th = getattr(args, "threads", None)
    if pt and th is not None:
        pt = int(pt)
        if th != pt:
            print(f"WARNING: --threads {th} != PARTITION_THREADS {pt} "
                  f"(hw threads on nodes {me['NUMA_NODE_ORDER']}).\n"
                  f"         {'Oversubscribed' if th > pt else 'Under-using the bound nodes'}"
                  f" -- pass --threads {pt} unless this is deliberate.")
    if th is not None and th % 2:
        print(f"WARNING: --threads {th} is odd; the benchmarks use num_threads/2 "
              f"per node, so one thread will be dropped.")

    if args.numafy:
        build(ROOT, bench_name, umf=not args.no_umf, do_numafy=True)

    kernel_an, _ = autonuma()                    # what the kernel actually reports
    if args.an_mode == "auto":
        an_value, an_folder = kernel_an, ("AN_on" if kernel_an == 1 else "AN_off")
    else:
        an_value = 1 if args.an_mode == "on" else 0
        an_folder = f"AN_{args.an_mode}"
        print(f"--- AutoNUMA FORCED {args.an_mode} "
              f"(kernel numa_balancing reports {kernel_an}); the contrast is "
              f"numactl --balancing only, not a kernel toggle -- recorded in the manifest.")
    binary    = str(ROOT / bench["binary"])
    workloads = args.workloads or bench["workloads"]
    exp_dir   = ROOT / "Campaigns" / bench_name / args.slug   # experiment = slug
    outdir    = exp_dir / an_folder                           # AN_off/ , AN_on/ : data only
    manifest  = exp_dir / "manifest.md"                       # shared, at the slug level
    params    = benchmarks.extract_params(args, bench_name)

    # ---- guards: don't clobber; every run of an experiment shares commit + config ----
    if outdir.exists() and any(outdir.iterdir()) and not args.force:
        sys.exit(f"Refusing: {outdir} already exists and is non-empty.\n"
                 f"Pick a new --slug, or pass --force to overwrite.")
    prev_commit = read_manifest_commit(manifest)              # None on the first run
    if prev_commit and prev_commit != git_hash and not args.force:
        sys.exit(f"Refusing: experiment '{args.slug}' was started at commit {prev_commit}, "
                 f"but HEAD is {git_hash}.\n"
                 f"Every run of an experiment must share a commit to be comparable. "
                 f"Re-run on that commit, use a new --slug, or pass --force.")
    # every config parameter must match the existing manifest too -- not just the commit
    cur_sig = {k: str(v) for k, v in params.items()}
    cur_sig["configs"]   = " ".join(args.configs)
    cur_sig["workloads"] = ", ".join(workloads)
    prev_sig = read_manifest_params(manifest)
    if prev_sig:
        diffs = sorted(k for k in set(prev_sig) | set(cur_sig)
                       if prev_sig.get(k) != cur_sig.get(k))
        if diffs and not args.force:
            detail = "\n".join(f"    {k}: manifest={prev_sig.get(k)!r} vs this={cur_sig.get(k)!r}"
                               for k in diffs)
            sys.exit(f"Refusing: config differs from the existing '{args.slug}' manifest:\n"
                     f"{detail}\nEvery run of an experiment must share config. "
                     f"Use a new --slug, or pass --force.")
    outdir.mkdir(parents=True, exist_ok=True)

    # ---- provenance: shared manifest (create or append a run) + committed diff + tag ----
    is_new = not manifest.exists()
    write_or_append_manifest(manifest, bench=bench_name, binary=binary,
                             purpose=args.purpose or args.slug, git_hash=git_hash,
                             git_subject=subject, machine=machine_info(),
                             an_value=an_value, params=params,
                             an_forced=(args.an_mode != "auto"), kernel_an=kernel_an,
                             configs=args.configs, workloads=workloads, slug=args.slug)
    diff_path = exp_dir / "git_diff.txt"
    if not diff_path.exists():                                # same commit for every run
        diff_path.write_text(subprocess.run(["git", "-C", str(ROOT), "show", "HEAD"],
                                            capture_output=True, text=True).stdout)
    subprocess.run(["git", "-C", str(ROOT), "tag", "-f", f"campaign/{args.slug}"], capture_output=True)

    print(f"Campaign: {outdir}")
    print(f"  experiment '{args.slug}' | commit {git_hash} ({subject}) | AutoNUMA {an_folder}")
    print("  (new experiment -- wrote manifest.md)\n" if is_new
          else "  (existing experiment -- appended run to manifest.md; commit + config match)\n")

    # ---- the sweep ----
    cwd = str(ROOT / bench["cwd"]) if bench["cwd"] else None
    for wl in workloads:
        safe = wl.replace(",", "_")
        out  = outdir / f"{bench_name}_{safe}.csv"
        log  = outdir / f"{bench_name}_{safe}.log"
        out.write_text(bench["header"] + "\n")
        p = dict(params); p["workload"] = wl
        for i, cfg in enumerate(args.configs):
            if autonuma()[0] != kernel_an:                      # reboot safety
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
