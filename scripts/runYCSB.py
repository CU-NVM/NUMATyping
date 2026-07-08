#!/usr/bin/env python3
import subprocess
import argparse
import os
import sys
import time
from pathlib import Path

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
YCSB NUMA Experiment Runner
---------------------------
Automates the lifecycle of a NUMA-aware YCSB experiment.

USAGE:
    python3 runYCSB.py [OPTIONS]

CORE OPTIONS:
    --ROOT_DIR PATH        Path to NUMATyping root (Default: ~/NUMATyping).
    --numafy               Trigger the 'numafy.py' transformation pass. 
    --UMF                  Enable Unified Memory Framework support.
    --jemalloc             Enable Jemalloc support during compilation.
    --AN [0|1]             Set AutoNUMA. 1 adds '--balancing' to numactl (Default: 0).
    -d, --output PATH      Output directory (Default: ROOT_DIR/Result).
    --graph                Generate plots after the run finishes.
    --perlmutter           Append '_perl', bind to 0,7, use 128 threads & 200M keys.
    --workload STR [STR..] One or more workload configs. 
                           (Default: AD, AA, BB, CC, DD, EE, FF mixes)

WORKFLOW EXAMPLE:
    python3 runYCSB.py --ROOT_DIR=$SCRATCH/NUMATyping --numafy --UMF --AN 1
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Helpers
# ============================================================================

def get_spack_path(package):
    try:
        return subprocess.check_output(f"spack location -i {package}", shell=True, executable='/bin/bash', 
                                       universal_newlines=True, stderr=subprocess.DEVNULL).strip()
    except Exception:
        return None

def ensure_dir(path: str) -> str:
    os.makedirs(path, exist_ok=True)
    return os.path.abspath(path)

# ============================================================================
# Execution Pipeline
# ============================================================================

def compile_experiment(UMF: bool, do_numafy: bool, use_jemalloc: bool, root_dir: str, jemalloc_root: str, experiment_folder: str) -> None:
    max_node = os.environ.get("MAX_NODE_ID", "0")

    if do_numafy:
        numafy_script = os.path.join(root_dir, "scripts", "numafy.py")
        numafy_cmd = f"python3 {numafy_script} --ROOT_DIR=./ --umf=1 --jemalloc-root=$(spack location -i jemalloc) ycsb"
        
        print(f"\n--- Running Transformation (MAX_NODE_ID={max_node}) ---")
        subprocess.run(numafy_cmd, shell=True, executable='/bin/bash', cwd=root_dir, check=True)

    print(f"\n--- Compiling in {experiment_folder} ---")
    subprocess.run(f"make -C {experiment_folder} clean", shell=True, check=False)
    
    make_vars = f"ROOT_DIR={root_dir} "
    if UMF: make_vars += " UMF=1 "
    
    subprocess.run(f"make -C {experiment_folder} {make_vars}", shell=True, check=True)

def run_experiment(output_csv: Path, experiment_folder: str, workload: str, an_setting: int, is_perlmutter: bool,
                   combinations, mixes, duration: str,
                   keys=None, buckets=None, tables=None, interval=None, threads=None, hash_fn: str = "djb2") -> None:

    # Dynamically scale parameters based on the system
    if is_perlmutter:
        numactl_base = "--cpunodebind=0,7 --membind=0,7"
        t_val = "128"
        b_val = "266600"
        k_val = "200000000"
        i_val = "10"
    else:
        numactl_base = "--cpunodebind=0,1 --membind=0,1"
        t_val = "80"
        b_val = "133300"
        k_val = "100000000"
        i_val = "20"

    # Explicit CLI overrides take precedence over the machine-scaled defaults above.
    if threads  is not None: t_val = str(threads)
    if buckets  is not None: b_val = str(buckets)
    if keys     is not None: k_val = str(keys)
    if interval is not None: i_val = str(interval)
    a_val = str(tables) if tables is not None else "1000"

    if an_setting == 1:
        numactl_flags = f"--balancing {numactl_base}"
    else:
        numactl_flags = numactl_base

    print(f"--- Configuring NUMA Binding: {numactl_flags} ---")

    for th, ds in combinations:
        for mix in mixes:
            cmd_list = [
                "python3", "meta.py",
                "numactl"
            ] + numactl_flags.split() + [
                "./bin/ycsb",
                "--meta", f"th_config:{th}",
                "--meta", f"DS_config:{ds}",
                "--meta", f"m:{mix}",
                "--meta", f"t:{t_val}",
                "--meta", f"b:{b_val}",
                "--meta", f"w:{workload}",
                "--meta", f"u:{duration}",
                "--meta", f"k:{k_val}",
                "--meta", f"i:{i_val}",
                "--meta", f"a:{a_val}",
                "--meta", f"H:{hash_fn}"
            ]

            cmd_str = ' '.join(cmd_list)
            tag = f"th={th} ds={ds} mix={mix}"
            print(f"\n[CMD-START] {tag} | duration={duration}s @ {time.strftime('%H:%M:%S')}", flush=True)
            print(f"[CMD] {cmd_str}", flush=True)
            try:
                with open(output_csv, "a") as f:
                    subprocess.run(cmd_list, cwd=experiment_folder, stdout=f, text=True, check=True)
            except subprocess.CalledProcessError as e:
                # Record exactly which command failed, both to the log and a dedicated file.
                msg = f"[CMD-FAIL] exit={e.returncode} {tag} :: {cmd_str}"
                print(msg, flush=True)
                fail_path = Path(output_csv).parent / "ycsb_sweep_FAILED.txt"
                with open(fail_path, "a") as ff:
                    ff.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')}  {msg}\n")
                raise
            print(f"[CMD-OK] {tag} @ {time.strftime('%H:%M:%S')}", flush=True)

            print("-> Run complete. Sleeping 10s to clear memory caches...", flush=True)
            time.sleep(10)

# ============================================================================
# Main
# ============================================================================

if __name__ == "__main__":
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    # Define the 7 target workloads as the default list
    default_workloads = [
        "A-50-50-50,D-100-0-50",
        "A-50-50-50,A-100-0-50",
        "B-50-50-50,B-100-0-50",
        "C-50-50-50,C-100-0-50",
        "D-50-50-50,D-100-0-50",
        "E-50-50-50,E-100-0-50",
        "F-50-50-50,F-100-0-50"
    ]

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--ROOT_DIR', default=os.path.expanduser("~/NUMATyping"))
    parser.add_argument('--numafy', action='store_true')
    parser.add_argument('--UMF', action='store_true')
    parser.add_argument('--jemalloc', action='store_true')
    parser.add_argument("-d", "--output")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=0)
    parser.add_argument('--graph', action='store_true')
    parser.add_argument('--perlmutter', action='store_true')
    parser.add_argument('--jemalloc-root')
    parser.add_argument('--workload', type=str, nargs='+', default=default_workloads)
    parser.add_argument('--mix', type=str, nargs='+', default=['uniform', 'zipfian'],
                        choices=['uniform', 'zipfian'],
                        help="Key distribution(s) to sweep (default: both uniform and zipfian).")
    parser.add_argument('--duration', type=str, default='1200',
                        help="Per-run duration in seconds (default: 1200 = 20 min, paper config).")
    parser.add_argument('--configs', type=str, default='typing', choices=['typing', 'all'],
                        help="typing = numa/numa + numa/regular (default); all = 4 thread/DS combos.")
    parser.add_argument('--keys', type=int, default=None, help="num_keys (-k). Overrides machine default.")
    parser.add_argument('--buckets', type=int, default=None, help="bucket_count per table (-b). Overrides machine default.")
    parser.add_argument('--tables', type=int, default=None, help="total num_tables (-a). Default 1000.")
    parser.add_argument('--interval', type=int, default=None, help="report interval seconds (-i). Overrides machine default.")
    parser.add_argument('--threads', type=int, default=None, help="num_threads (-t). Overrides machine default.")
    parser.add_argument('--hash', type=str, default='djb2', choices=['djb2', 'mix'], help="key placement hash (-H). Default djb2.")

    try:
        args = parser.parse_args()
    except:
        show_help()

    ROOT_DIR = os.path.abspath(args.ROOT_DIR)
    
    if not os.path.exists(ROOT_DIR):
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = os.path.join(ROOT_DIR, "Output/ycsb")
    OUT_BASE = Path(ensure_dir(args.output)) if args.output else Path(ensure_dir(os.path.join(ROOT_DIR, "Result")))
    GRAPH_BASE = Path(ensure_dir(os.path.join(ROOT_DIR, "Graphs")))
    
    use_jemalloc = args.jemalloc or bool(args.jemalloc_root)
    JEMALLOC_ROOT = args.jemalloc_root or get_spack_path("jemalloc") if use_jemalloc else ""
    
    an_folder = "Revision/ycsb"   # revision runs; AN mode is applied via numactl, not the path

    if args.configs == 'all':
        combinations = [("numa", "numa"), ("numa", "regular"), ("regular", "numa"), ("regular", "regular")]
    else:
        combinations = [("numa", "numa"), ("numa", "regular")]

    try:
        compile_experiment(args.UMF, args.numafy, use_jemalloc, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER)

        for wl in args.workload:
            print(f"\n=======================================================")
            print(f"Starting Experiment Phase for Workload: {wl}")
            print(f"=======================================================")

            # Replace commas with underscores to keep filenames clean and safe.
            # Tag with the mix when a single one is requested (e.g. _zipfian) to match
            # the ycsb_<WL>_<MIX>.csv convention of ycsb_run_workload.sh.
            file_suffix = "_perl.csv" if args.perlmutter else ".csv"
            mix_suffix = f"_{args.mix[0]}" if len(args.mix) == 1 else ""
            safe_filename = f"ycsb_{wl.replace(',', '_')}{mix_suffix}{file_suffix}"
            exp_filename = f"ycsb_experiments{file_suffix}"
            
            # Define all 4 output paths
            out_exp_path = OUT_BASE / an_folder / exp_filename
            out_wl_path = OUT_BASE / an_folder / safe_filename
            graph_exp_path = GRAPH_BASE / an_folder / exp_filename
            graph_wl_path = GRAPH_BASE / an_folder / safe_filename
            
            # 1. Dynamic Header Formatting
            base_header = "Date, Time, num_tables, num_threads, thread_config, DS_config, buckets, workload, duration, num_keys, interval, ops_node0, ops_node1, total_ops\n"
            
            workload_count = wl.count(",") + 1
            if workload_count > 1:
                workload_cols = ", ".join([f"workload{i+1}" for i in range(workload_count)]) + ","
                header_to_write = base_header.replace("workload,", workload_cols)
            else:
                header_to_write = base_header

            # Ensure headers exist in BOTH append files
            for exp_path in [out_exp_path, graph_exp_path]:
                if not exp_path.exists() or exp_path.stat().st_size == 0:
                    exp_path.parent.mkdir(parents=True, exist_ok=True)
                    with exp_path.open("w") as f:
                        f.write(header_to_write)

            # Note the number of lines in the main experiment file BEFORE running
            with open(out_exp_path, "r") as f:
                lines_before_run = len(f.readlines())

            # Path 1: Append to main ycsb_experiments.csv in the Result directory
            run_experiment(out_exp_path.absolute(), EXPERIMENT_FOLDER, wl, args.AN, args.perlmutter,
                           combinations, args.mix, args.duration,
                           keys=args.keys, buckets=args.buckets, tables=args.tables,
                           interval=args.interval, threads=args.threads, hash_fn=args.hash)

            # Extract ONLY the newest results
            with open(out_exp_path, "r") as f:
                all_lines = f.readlines()
                
            latest_run_lines = all_lines[lines_before_run:]

            # Path 2: Append this run's rows to the workload+mix-specific file
            out_wl_path.parent.mkdir(parents=True, exist_ok=True)
            new_wl_file = (not out_wl_path.exists()) or out_wl_path.stat().st_size == 0
            with open(out_wl_path, "a") as f:
                if new_wl_file:
                    f.write(header_to_write)
                f.writelines(latest_run_lines)

            # Path 3: Append to ycsb_experiments.csv in the Graphs directory
            graph_exp_path.parent.mkdir(parents=True, exist_ok=True)
            with open(graph_exp_path, "a") as f:
                f.writelines(latest_run_lines)
                
            # Path 4: Overwrite workload-specific file in the Graphs directory
            graph_wl_path.parent.mkdir(parents=True, exist_ok=True)
            with open(graph_wl_path, "w") as f:
                f.write(header_to_write)
                f.writelines(latest_run_lines)
                
            print(f"--- Data distributed successfully to {an_folder} directories ---")

            if args.graph:
                plot_script = os.path.join(ROOT_DIR, "scripts", "bar_plot_ycsb.py")
                subprocess.run(f'python3 {plot_script} --AN {args.AN} --perlmutter' if args.perlmutter else f'python3 {plot_script} --AN {args.AN}', shell=True)
                
            print(f"COMPLETE. Primary Results for {wl} appended to: {out_exp_path}")

    except subprocess.CalledProcessError as e:
        print(f"\n[FATAL ERROR] Experiment failed during execution (Exit Code: {e.returncode})")
        sys.exit(e.returncode)
    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected runtime error occurred: {e}")
        sys.exit(1)