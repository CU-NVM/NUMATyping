#!/usr/bin/env python3
import subprocess
import argparse
import os
import sys
from pathlib import Path

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
Data Structure Experiment Runner
--------------------------------
Automates the lifecycle of NUMA-aware Data Structure tests.

USAGE:
    python3 runExperiments.py --DS [name] [OPTIONS]

CORE OPTIONS:
    --DS NAME [NAME..]      Specify one or more data structure names (Required).
    --ROOT_DIR PATH         Path to NUMATyping root (Default: ~/NUMATyping).
    --numafy                Trigger the 'numafy.py' transformation pass. 
    --UMF                   Enable Unified Memory Framework support.
    --AN [0|1]              Set AutoNUMA. 1 adds '--balancing' to numactl (Default: 1).
    -d, --output PATH       Output directory (Default: ROOT_DIR/Result).
    --graph                 Generate plots after the run finishes.
    --perlmutter            Use Perlmutter config (128 threads, node 0/7 binding, '_perl' suffix).
    --jemalloc-root PATH    Manual path to jemalloc (Default: auto-detect via spack).
    --numDS INT             Number of data structure elements (Default: 1000000)
    --numKeys INT           Keyspace size (Default: 80)
    -v, --verbose           Stream results to the terminal instead of writing CSVs.

WORKFLOW EXAMPLE:
    python3 runExperiments.py --DS HashTrie Skiplist --numafy --UMF --perlmutter --AN 1
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Helpers
# ============================================================================

def get_spack_path(package):
    cmd = "source /etc/profile.d/modules.sh && module load spack && spack location -i " + package
    try:
        return subprocess.check_output(cmd, shell=True, executable='/bin/bash', 
                                       universal_newlines=True, stderr=subprocess.DEVNULL).strip()
    except Exception:
        return None

def ensure_dir(path: str) -> str:
    os.makedirs(path, exist_ok=True)
    return os.path.abspath(path)

# ============================================================================
# Execution Pipeline
# ============================================================================

def compile_experiment(UMF: bool, do_numafy: bool, root_dir: str, jemalloc_root: str, experiment_folder: str, suite: str = "DataStructureTests") -> None:
    max_node = os.environ.get("MAX_NODE_ID", "0")

    if do_numafy:
        numafy_script = os.path.join(root_dir, "scripts", "numafy.py")
        # Always emit both #ifdef UMF / #else numa_alloc_onnode branches so the
        # runtime allocator can be switched purely via the Makefile UMF flag.
        numafy_cmd = ["python3", numafy_script, f"--ROOT_DIR={root_dir}", suite, "--umf=1"]
        if jemalloc_root:
            numafy_cmd.append(f"--jemalloc-root={jemalloc_root}")

        print(f"\n--- Running Transformation (MAX_NODE_ID={max_node}) ---")
        subprocess.run(numafy_cmd, check=True)

    print(f"\n--- Compiling in {experiment_folder} ---")
    subprocess.run(f"make -C {experiment_folder} clean", shell=True, check=False)
    
    make_vars = f"ROOT_DIR={root_dir} "
    if jemalloc_root: make_vars += f" JEMALLOC_ROOT={jemalloc_root}"
    if UMF: make_vars += " UMF=1"
    
    subprocess.run(f"make -C {experiment_folder} {make_vars}", shell=True, check=True)

def run_experiment(output_csv, experiment_folder: str, DS_name: str, numDS: str, numKeys: str, an_setting: int, is_perlmutter: bool, duration: str = "1200", th_config: str = "numa:regular", ds_config: str = "numa:regular", verbose: bool = False) -> None:
    max_node = os.environ.get("MAX_NODE_ID", "0")

    # Adjust NUMA binding targets based on the system
    if is_perlmutter:
        numactl_base = "--cpunodebind=0,7 --membind=0,7"
        t_val = "128"
    else:
        numactl_base = "--cpunodebind=0,1 --membind=0,1"
        t_val = "80"

    # Dynamically build numactl flags based on the AN setting
    if an_setting == 1:
        numactl_flags = f"--balancing {numactl_base}"
    else:
        numactl_flags = numactl_base

    print(f"--- Configuring NUMA Binding: {numactl_flags} ---")

    base_cmd = (
        f'cd {experiment_folder} && python3 meta.py '
        f'numactl {numactl_flags} '
        './bin/datastructures '
        f'--meta n:{numDS} '
        f'--meta t:{t_val} '
        f'--meta D:{duration} '
        f'--meta DS_name:{DS_name} '
        f'--meta th_config:{th_config} '
        f'--meta DS_config:{ds_config} '
        f'--meta k:{numKeys} '
        '--meta i:20 '
    )

    if verbose:
        cmd = base_cmd
    else:
        cmd = base_cmd + f'>> "{output_csv}"'

    print(f"--- Running Experiment ---\n{cmd}\n")
    subprocess.run(cmd, shell=True, check=True)

# ============================================================================
# Main
# ============================================================================

if __name__ == "__main__":
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--DS', type=str, nargs='+', required=True)
    parser.add_argument('--ROOT_DIR', default=os.path.expanduser("~/NUMATyping"))
    parser.add_argument('--numafy', action='store_true')
    parser.add_argument('--UMF', action='store_true')
    parser.add_argument("-d", "--output")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=1)
    parser.add_argument('--graph', action='store_true')
    parser.add_argument('--perlmutter', action='store_true')
    parser.add_argument('--jemalloc-root')
    
    # Extract num_DS and keyspace into variables so they can dictate the file names
    parser.add_argument('--numDS', type=str, default="1000000")
    parser.add_argument('--numKeys', type=str, default="80")
    parser.add_argument('-v', '--verbose', action='store_true',
                        help="Stream results to the terminal instead of writing CSVs.")

    # Suite / run customization (defaults preserve the original 2-node behavior).
    parser.add_argument('--suite', default="DataStructureTests",
                        help="Suite folder under Output/ to build & run (e.g. DataStructureTests_four).")
    parser.add_argument('--duration', default="1200", help="Per-config run duration in seconds.")
    parser.add_argument('--thConfig', default="numa:regular", help="Thread config sweep (colon-separated).")
    parser.add_argument('--dsConfig', default="numa:regular", help="Data-structure config sweep (colon-separated).")
    parser.add_argument('--nodes', type=int, default=2, help="Number of NUMA nodes (controls Op columns in the header).")
    parser.add_argument('--outfile', default=None, help="Override the per-run CSV filename.")

    try:
        args = parser.parse_args()
    except:
        show_help()

    ROOT_DIR = os.path.abspath(args.ROOT_DIR)
    if not os.path.exists(ROOT_DIR):
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = os.path.join(ROOT_DIR, f"Output/{args.suite}")
    OUT_BASE = Path(ensure_dir(args.output)) if args.output else Path(ensure_dir(os.path.join(ROOT_DIR, "Result")))
    GRAPH_BASE = Path(ensure_dir(os.path.join(ROOT_DIR, "Graphs")))
    
    JEMALLOC_ROOT = args.jemalloc_root or get_spack_path("jemalloc")
    
    an_folder = "AN_on" if args.AN == 1 else "AN_off"

    op_cols = ", ".join(f"Op{i}" for i in range(args.nodes))
    header_str = f"Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, keyspace, interval, {op_cols}, TotalOps\n"

    try:
        # Compile only once before looping through data structures
        compile_experiment(args.UMF, args.numafy, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER, args.suite)

        for ds in args.DS:
            print(f"\n=======================================================")
            print(f"Starting Experiment Phase for Data Structure: {ds}")
            print(f"=======================================================")

            if args.verbose:
                # Stream results straight to the terminal; skip all CSV bookkeeping.
                run_experiment(None, EXPERIMENT_FOLDER, ds, args.numDS, args.numKeys, args.AN, args.perlmutter,
                               duration=args.duration, th_config=args.thConfig, ds_config=args.dsConfig, verbose=True)
                print(f"COMPLETE. Results for {ds} streamed to terminal (verbose mode, no files written).")
                continue

            # 1. Build Filenames (appending _perl if running on Perlmutter)
            file_suffix = "_perl.csv" if args.perlmutter else ".csv"
            specific_filename = args.outfile or f"{ds}_{args.numDS}_{args.numKeys}_experiments{file_suffix}"
            exp_filename = f"{ds}_experiments.csv"

            # Define all 4 output paths
            out_specific_path = OUT_BASE / an_folder / specific_filename
            graph_specific_path = GRAPH_BASE / an_folder / specific_filename

            out_exp_path = OUT_BASE / an_folder / exp_filename
            graph_exp_path = GRAPH_BASE / an_folder / exp_filename

            # Ensure headers exist (append-safe) in ALL targets, including the
            # per-run "specific" files. Existing data is preserved, not clobbered.
            for target_path in [out_exp_path, graph_exp_path, out_specific_path, graph_specific_path]:
                target_path.parent.mkdir(parents=True, exist_ok=True)
                if not target_path.exists() or target_path.stat().st_size == 0:
                    with target_path.open("w") as f:
                        f.write(header_str)

            # Record the existing line count so we can isolate only the rows this
            # run appends (the binary writes via '>>'), even when the file already
            # held prior results.
            pre_lines = sum(1 for _ in open(out_specific_path))

            # ---------------------------------------------------------
            # EXECUTION FLOW:
            # Append directly to the isolated file first to avoid race conditions
            # ---------------------------------------------------------
            run_experiment(out_specific_path.absolute(), EXPERIMENT_FOLDER, ds, args.numDS, args.numKeys, args.AN, args.perlmutter,
                           duration=args.duration, th_config=args.thConfig, ds_config=args.dsConfig)

            # Read back ONLY the rows produced by this run
            with open(out_specific_path, "r") as f:
                lines = f.readlines()
                data_lines = lines[pre_lines:]

            # Copy this run's rows to the Graphs directory
            with open(graph_specific_path, "a") as f:
                f.writelines(data_lines)

            # Append this run's rows to the aggregate master files
            with open(out_exp_path, "a") as f:
                f.writelines(data_lines)
            with open(graph_exp_path, "a") as f:
                f.writelines(data_lines)

            print(f"--- Data safely written to isolated files and appended to master aggregates ---")

            if args.graph:
                # Call the specific plot_benchmark script
                plot_script = os.path.join(ROOT_DIR, "scripts", "plot_bst.py")
                subprocess.run(f'python3 {plot_script} --AN {args.AN} --ds_name "{ds}" --numDS {args.numDS} --numKeys {args.numKeys}', shell=True)

            print(f"COMPLETE. Primary Results for {ds} saved to: {out_specific_path}")

    except subprocess.CalledProcessError as e:
        print(f"\n[FATAL ERROR] Experiment failed during execution (Exit Code: {e.returncode})")
        sys.exit(e.returncode)
    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected runtime error occurred: {e}")
        sys.exit(1)