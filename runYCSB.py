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
YCSB NUMA Experiment Runner
---------------------------
Automates the lifecycle of a NUMA-aware YCSB experiment.

USAGE:
    python3 runYCSB.py [OPTIONS]

CORE OPTIONS:
    --ROOT_DIR PATH        Path to NUMATyping root (Default: ~/NUMATyping).
    --numafy               Trigger the 'numafy.py' transformation pass. 
    --UMF                  Enable Unified Memory Framework support.
    --AN [0|1]             Set AutoNUMA (Default: 1).
    -d, --output PATH      Output directory (Default: ROOT_DIR/Result).
    --graph                Generate plots after the run finishes. (TODO: May not work properly)

WORKFLOW EXAMPLE:
    python3 runYCSB.py --ROOT_DIR=$SCRATCH/NUMATyping --numafy --UMF
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

def set_autonuma(desired: int) -> None:
    try:
        with open("/proc/sys/kernel/numa_balancing", "r") as f:
            cur = int(f.read().strip())
    except FileNotFoundError:
        return

    if cur == desired: return

    try:
        with open("/proc/sys/kernel/numa_balancing", "w") as f:
            f.write(str(desired))
    except PermissionError:
        # Fallback to sysctl if direct write fails
        subprocess.run(["sudo", "sysctl", f"kernel.numa_balancing={desired}"], check=False)

# ============================================================================
# Execution Pipeline
# ============================================================================

def compile_experiment(UMF: bool, do_numafy: bool, root_dir: str, jemalloc_root: str, experiment_folder: str) -> None:
    if do_numafy:
        numafy_script = os.path.join(root_dir, "numafy.py")
        numafy_cmd = ["python3", numafy_script, f"--ROOT_DIR={root_dir}", "ycsb", f"--umf={1 if UMF else 0}"]
        if jemalloc_root:
            numafy_cmd.append(f"--jemalloc-root={jemalloc_root}")
        
        print(f"\n--- Running Transformation ---")
        subprocess.run(numafy_cmd, check=True)

    print(f"\n--- Compiling in {experiment_folder} ---")
    subprocess.run(f"make -C {experiment_folder} clean", shell=True, check=False)
    
    make_vars = f"ROOT_DIR={root_dir}"
    if jemalloc_root: make_vars += f" JEMALLOC_ROOT={jemalloc_root}"
    if UMF: make_vars += " UMF=1"
    
    subprocess.run(f"make -C {experiment_folder} {make_vars}", shell=True, check=True)

def run_experiment(output_csv: Path, experiment_folder: str) -> None:
    cmd = (f'cd {experiment_folder} && python3 meta.py '
           f'numactl --cpunodebind=0,7 --membind=0,7 ./bin/ycsb '
           f'--meta th_config:numa:regular --meta DS_config:numa:regular '
           f'--meta t:80 --meta b:1333 --meta w:D --meta u:20 '
           f'--meta k:15000000 --meta l:80-20 --meta i:20 --meta a:1000 >> "{output_csv}"')
    subprocess.run(cmd, shell=True, check=True)

# ============================================================================
# Main
# ============================================================================

if __name__ == "__main__":
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--ROOT_DIR', default=os.path.expanduser("~/NUMATyping"))
    parser.add_argument('--numafy', action='store_true')
    parser.add_argument('--UMF', action='store_true')
    parser.add_argument("-d", "--output")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=1)
    parser.add_argument('--graph', action='store_true')
    parser.add_argument('--jemalloc-root')

    try:
        args = parser.parse_args()
    except:
        show_help()

    # Fixed: Define variable inside scope
    OUTPUT_FILENAME = "ycsb_experiments.csv"
    ROOT_DIR = os.path.abspath(args.ROOT_DIR)
    
    if not os.path.exists(ROOT_DIR):
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = os.path.join(ROOT_DIR, "Output/ycsb")
    OUT_BASE = Path(ensure_dir(args.output)) if args.output else Path(ensure_dir(os.path.join(ROOT_DIR, "Result")))
    
    JEMALLOC_ROOT = args.jemalloc_root or get_spack_path("jemalloc")
    
    set_autonuma(args.AN)
    
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    output_file_path = OUT_BASE / an_folder / OUTPUT_FILENAME
    
    # Ensure header
    if not output_file_path.exists() or output_file_path.stat().st_size == 0:
        output_file_path.parent.mkdir(parents=True, exist_ok=True)
        with output_file_path.open("w") as f:
            f.write("Date, Time, num_tables, num_threads, thread_config, DS_config, buckets, workload, duration, num_keys, locality, interval, ops_node0, ops_node1, total_ops\n")

    compile_experiment(args.UMF, args.numafy, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER)
    run_experiment(output_file_path.absolute(), EXPERIMENT_FOLDER)

    if args.graph:
        plot_script = os.path.join(ROOT_DIR, "Result/plots/plot_histogram.py")
        subprocess.run(f'python3 {plot_script} "{output_file_path}" --show --save {OUT_BASE}/{an_folder}/figs', shell=True)

    print(f"\nCOMPLETE. Results: {output_file_path}")
