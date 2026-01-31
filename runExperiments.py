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
    --DS NAME               Specify the data structure name (Required).
    --ROOT_DIR PATH         Path to NUMATyping root (Default: ~/NUMATyping).
    --numafy                Trigger the 'numafy.py' transformation pass. 
    --UMF                   Enable Unified Memory Framework support.
    --AN [0|1]              Set AutoNUMA (Default: 1).
    -d, --output PATH       Output directory (Default: ROOT_DIR/Result).
    --graph                 Generate plots after the run finishes.
    --jemalloc-root PATH    Manual path to jemalloc (Default: auto-detect via spack).

WORKFLOW EXAMPLE:
    python3 runExperiments.py --DS stack --numafy --UMF
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
        subprocess.run(["sudo", "sysctl", f"kernel.numa_balancing={desired}"], check=False)

# ============================================================================
# Execution Pipeline
# ============================================================================

def compile_experiment(UMF: bool, do_numafy: bool, root_dir: str, jemalloc_root: str, experiment_folder: str) -> None:
    if do_numafy:
        numafy_script = os.path.join(root_dir, "numafy.py")
        numafy_cmd = ["python3", numafy_script, f"--ROOT_DIR={root_dir}", "DataStructureTests", f"--umf={1 if UMF else 0}"]
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

def run_experiment(output_csv: Path, experiment_folder: str, DS_name: str) -> None:
    # Constructing the command using the specific logic for DataStructureTest
    cmd = (
        f'cd {experiment_folder} && python3 meta.py '
        'numactl --cpunodebind=0,7 --membind=0,7 '
        './bin/datastructures '
        '--meta n:30000000 '
        '--meta t:128:256 '
        '--meta D:300 '
        f'--meta DS_name:{DS_name} '
        '--meta th_config:numa:regular:reverse '
        '--meta DS_config:numa:regular '
        '--meta k:1600 '
        '--meta i:10 '
        f'>> "{output_csv}"'
    )
    print(f"--- Running Experiment ---\n{cmd}\n")
    subprocess.run(cmd, shell=True, check=True)

# ============================================================================
# Main
# ============================================================================

if __name__ == "__main__":
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--DS', type=str, required=True)
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

    ROOT_DIR = os.path.abspath(args.ROOT_DIR)
    if not os.path.exists(ROOT_DIR):
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = os.path.join(ROOT_DIR, "Output/DataStructureTests")
    OUT_BASE = Path(ensure_dir(args.output)) if args.output else Path(ensure_dir(os.path.join(ROOT_DIR, "Result")))
    
    JEMALLOC_ROOT = args.jemalloc_root or get_spack_path("jemalloc")
    
    #set_autonuma(args.AN)
    
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    output_file_path = OUT_BASE / an_folder / f"{args.DS}_Transactions.csv"
    
    # Ensure header
    if not output_file_path.exists() or output_file_path.stat().st_size == 0:
        output_file_path.parent.mkdir(parents=True, exist_ok=True)
        with output_file_path.open("w") as f:
            f.write("Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, keyspace, interval, Op0, Op1, TotalOps\n")

    # Wrap execution in try/except to catch runtime errors
    try:
        compile_experiment(args.UMF, args.numafy, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER)
        run_experiment(output_file_path.absolute(), EXPERIMENT_FOLDER, args.DS)

        if args.graph:
            plot_script = os.path.join(ROOT_DIR, "Result/plots/plot_histogram.py")
            # Note: adjust plot script path or arguments as needed for DS experiments
            subprocess.run(f'python3 {plot_script} "{output_file_path}" --show --save {OUT_BASE}/{an_folder}/figs', shell=True, check=True)

        print(f"\nCOMPLETE. Results: {output_file_path}")

    except subprocess.CalledProcessError as e:
        print(f"\n[FATAL ERROR] Experiment failed during execution (Exit Code: {e.returncode})")
        # Ensure we exit with error status so calling scripts know it failed
        sys.exit(e.returncode)
    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected runtime error occurred: {e}")
        sys.exit(1)

