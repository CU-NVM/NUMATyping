#!/usr/bin/env python3
"""
run.py -- quick, scratch benchmark run (no git gate, no manifest).

You name the output; results are bare CSVs, by default under
Runs/<bench>/AN_<mode>/.  For archival, provenance-tracked sweeps use campaign.py.

Benchmark-independent: the benchmark's own parameters are pulled from
benchmarks.py at parse time.  The generic helpers live in campaign.py.

Example:
    python3 scripts/run.py --name quicktest --workload C-50-50-50,C-100-0-50 \\
        --mix uniform --keys 1000000 --duration 15 --configs numa/numa numa/regular
"""
import argparse, subprocess, time
from pathlib import Path
from campaign import autonuma, run_config, build     # generic helpers now live in campaign.py
import benchmarks

ROOT = Path(__file__).resolve().parent.parent


def parse_args():
    pre = argparse.ArgumentParser(add_help=False)
    pre.add_argument("--bench", default="ycsb", choices=list(benchmarks.BENCHES))
    bench = pre.parse_known_args()[0].bench

    p = argparse.ArgumentParser(description="Quick scratch benchmark run.")
    p.add_argument("--bench", default="ycsb", choices=list(benchmarks.BENCHES))
    p.add_argument("--name", required=True, help="output CSV base name (no extension)")
    p.add_argument("--outdir", default=None,
                   help="output directory (default: Runs/<bench>/AN_<mode>/)")
    p.add_argument("--numafy", action="store_true",
                   help="numafy the suite and (re)compile Output/<bench> before running")
    p.add_argument("--no-umf", action="store_true", help="compile without UMF")
    p.add_argument("--workload", default=None, help="single workload (default: the bench's first)")
    p.add_argument("--configs", nargs="+", default=["numa/numa", "numa/regular"])
    p.add_argument("--refresh", type=int, default=0, help="settle seconds between configs")
    benchmarks.add_bench_args(p, bench)            # <-- the only benchmark-specific args
    return p.parse_args(), bench


def main():
    args, bench_name = parse_args()
    bench = benchmarks.BENCHES[bench_name]

    if args.numafy:
        build(ROOT, bench_name, umf=not args.no_umf, do_numafy=True)

    an_value, an_folder = autonuma()
    binary = str(ROOT / bench["binary"])
    outdir = Path(args.outdir) if args.outdir else ROOT / "Runs" / bench_name / an_folder
    outdir.mkdir(parents=True, exist_ok=True)
    out = outdir / f"{args.name}.csv"
    log = outdir / f"{args.name}.log"
    out.write_text(bench["header"] + "\n")

    params = benchmarks.extract_params(args, bench_name)
    params["workload"] = args.workload or bench["workloads"][0]
    cwd = str(ROOT / bench["cwd"]) if bench["cwd"] else None

    print(f"Run -> {out}  (AutoNUMA {an_folder})")
    for i, cfg in enumerate(args.configs):
        th, ds = cfg.split("/")
        print(f"  {th}/{ds} ...", flush=True)
        rc = run_config(bench["argv"](binary, th, ds, params), an_value, out, log, cwd=cwd)
        print(f"     rc={rc}" + ("  FAILED (see log)" if rc else ""))
        if i < len(args.configs) - 1 and args.refresh > 0:
            subprocess.run(["sync"]); time.sleep(args.refresh)
    print(f"Done -> {out}")


if __name__ == "__main__":
    main()
