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
    --AN [0|1]              Set AutoNUMA (Default: 1).
    -d, --output PATH       Output directory (Default: ROOT_DIR/Result).
    --graph                 Generate plots after the run finishes.
    --jemalloc-root PATH    Manual path to jemalloc (Default: auto-detect via spack).
    --numDS INT             Number of data structure elements (Default: 1000000)
    --numKeys INT           Keyspace size (Default: 80)

WORKFLOW EXAMPLE:
    python3 runExperiments.py --DS HashTrie Skiplist --numafy --UMF
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
    max_node = os.environ.get("MAX_NODE_ID", "0")
    
    if do_numafy:
        numafy_script = os.path.join(root_dir, "numafy.py")
        numafy_cmd = ["python3", numafy_script, f"--ROOT_DIR={root_dir}", "DataStructureTests", f"--umf={1 if UMF else 0}"]
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

def run_experiment(output_csv: Path, experiment_folder: str, DS_name: str, numDS: str, numKeys: str) -> None:
    max_node = os.environ.get("MAX_NODE_ID", "0")
    
    if max_node == "0":
        bind_str = "0"
    else:
        bind_str = f"0,{max_node}"

    print(f"--- Configuring NUMA Binding: 0,1 ---")

    cmd = (
        f'cd {experiment_folder} && python3 meta.py '
        f'numactl --cpunodebind=0,7 --membind=0,7 '
        './bin/datastructures '
        f'--meta n:{numDS} '
        '--meta t:64:128 '
        '--meta D:14400 '
        f'--meta DS_name:{DS_name} '
        '--meta th_config:numa '
        '--meta DS_config:numa:regular '
        f'--meta k:{numKeys} '
        '--meta i:200 '
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
    parser.add_argument('--DS', type=str, nargs='+', required=True)
    parser.add_argument('--ROOT_DIR', default=os.path.expanduser("~/NUMATyping"))
    parser.add_argument('--numafy', action='store_true')
    parser.add_argument('--UMF', action='store_true')
    parser.add_argument("-d", "--output")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=1)
    parser.add_argument('--graph', action='store_true')
    parser.add_argument('--jemalloc-root')
    
    # Extract num_DS and keyspace into variables so they can dictate the file names
    parser.add_argument('--numDS', type=str, default="1000000")
    parser.add_argument('--numKeys', type=str, default="80")

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
    GRAPH_BASE = Path(ensure_dir(os.path.join(ROOT_DIR, "Graphs")))
    
    JEMALLOC_ROOT = args.jemalloc_root or get_spack_path("jemalloc")
    
    #set_autonuma(args.AN)
    an_folder = "AN_on" if args.AN == 1 else "AN_off"

    header_str = "Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, keyspace, interval, Op0, Op1, TotalOps\n"

    try:
        # Compile only once before looping through data structures
        compile_experiment(args.UMF, args.numafy, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER)

        for ds in args.DS:
            print(f"\n=======================================================")
            print(f"Starting Experiment Phase for Data Structure: {ds}")
            print(f"=======================================================")

            # Define filenames dynamically based on args
            exp_filename = f"{ds}_experiments.csv"
            specific_filename = f"{ds}_{args.numDS}_{args.numKeys}_experiments.csv"
            
            # Define all 4 output paths
            out_exp_path = OUT_BASE / an_folder / exp_filename
            out_specific_path = OUT_BASE / an_folder / specific_filename
            graph_exp_path = GRAPH_BASE / an_folder / exp_filename
            graph_specific_path = GRAPH_BASE / an_folder / specific_filename

            # Ensure headers exist in BOTH append files
            for target_path in [out_exp_path, graph_exp_path]:
                if not target_path.exists() or target_path.stat().st_size == 0:
                    target_path.parent.mkdir(parents=True, exist_ok=True)
                    with target_path.open("w") as f:
                        f.write(header_str)

            # Note the number of lines in the main experiment file BEFORE running
            with open(out_exp_path, "r") as f:
                lines_before_run = len(f.readlines())

            # Path 1: Append to main DSname_experiments.csv in the Result directory
            run_experiment(out_exp_path.absolute(), EXPERIMENT_FOLDER, ds, args.numDS, args.numKeys)

            # Extract ONLY the newest results
            with open(out_exp_path, "r") as f:
                all_lines = f.readlines()
                
            latest_run_lines = all_lines[lines_before_run:]

            # Path 2: Overwrite DSname_numDS_numKeys_experiments.csv in the Result directory
            out_specific_path.parent.mkdir(parents=True, exist_ok=True)
            with open(out_specific_path, "w") as f:
                f.write(header_str)
                f.writelines(latest_run_lines)

            # Path 3: Append to DSname_experiments.csv in the Graphs directory
            graph_exp_path.parent.mkdir(parents=True, exist_ok=True)
            with open(graph_exp_path, "a") as f:
                f.writelines(latest_run_lines)
                
            # Path 4: Overwrite DSname_numDS_numKeys_experiments.csv in the Graphs directory
            graph_specific_path.parent.mkdir(parents=True, exist_ok=True)
            with open(graph_specific_path, "w") as f:
                f.write(header_str)
                f.writelines(latest_run_lines)
                
            print(f"--- Data distributed successfully to {an_folder} directories ---")

            if args.graph:
                # Call the new plot_benchmark script
                plot_script = os.path.join(ROOT_DIR, "Graphs/plot_bst.py")
                subprocess.run(f'python3 {plot_script} --AN {args.AN} --ds_name "{ds}" --numDS {args.numDS} --numKeys {args.numKeys}', shell=True)
                
            print(f"COMPLETE. Primary Results for {ds} appended to: {out_exp_path}")

    except subprocess.CalledProcessError as e:
        print(f"\n[FATAL ERROR] Experiment failed during execution (Exit Code: {e.returncode})")
        sys.exit(e.returncode)
    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected runtime error occurred: {e}")
        sys.exit(1)
