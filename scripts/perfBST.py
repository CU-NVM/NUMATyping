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
    --perf                  Run with 'perf stat' to track local vs remote NUMA accesses.

WORKFLOW EXAMPLE:
    python3 runExperiments.py --DS HashTrie Skiplist --numafy --UMF --perlmutter --AN 1 --perf
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

def clean_perf_csv(raw_csv_path: Path, clean_csv_path: Path):
    """
    Takes the raw, multi-line perf stat output and converts it into 
    a clean 3-column CSV: time, local_ops, remote_ops.
    """
    data = {}
    try:
        with open(raw_csv_path, 'r') as f:
            for line in f:
                line = line.strip()
                # Skip comments and empty lines
                if line.startswith('#') or not line:
                    continue
                
                parts = line.split(',')
                if len(parts) >= 4:
                    try:
                        time_val = float(parts[0].strip())
                        count_val_str = parts[1].strip()
                        event_name = parts[3].strip()

                        # Handle perf scenarios where events aren't counted
                        if count_val_str == '<not counted>' or count_val_str == '<not supported>':
                            count_val = 0
                        else:
                            count_val = int(count_val_str)

                        if time_val not in data:
                            data[time_val] = {'local': 0, 'remote': 0}

                        if 'mem_io_local' in event_name:
                            data[time_val]['local'] = count_val
                        elif 'mem_io_remote' in event_name:
                            data[time_val]['remote'] = count_val

                    except ValueError:
                        continue

        # Write the cleanly formatted CSV
        with open(clean_csv_path, 'w') as f:
            f.write("time,local_ops,remote_ops\n")
            for t in sorted(data.keys()):
                f.write(f"{t},{data[t]['local']},{data[t]['remote']}\n")
                
        # Clean up the raw perf file so we don't clutter the directory
        if clean_csv_path.exists():
            os.remove(raw_csv_path)

    except Exception as e:
        print(f"[Warning] Failed to process perf output: {e}")

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

def run_experiment(output_csv: Path, graph_dir: Path, experiment_folder: str, DS_name: str, numDS: str, numKeys: str, an_setting: int, is_perlmutter: bool, run_perf: bool) -> None:
    if is_perlmutter:
        numactl_base = "--cpunodebind=0,7 --membind=0,7"
        t_val = "128"
    else:
        numactl_base = "--cpunodebind=0,1 --membind=0,1"
        t_val = "40:80"

    if an_setting == 1:
        numactl_flags = f"--balancing {numactl_base}"
    else:
        numactl_flags = numactl_base

    print(f"--- Configuring NUMA Binding: {numactl_flags} ---")

    combinations = [
        ("numa", "numa"),
        ("numa", "regular"),
        ("regular", "numa"),
        ("regular", "regular")
    ]

    for th, ds in combinations:
        cmd_list = [
            "python3", "meta.py",
            "numactl"
        ] + numactl_flags.split() + [
            "./bin/datastructures",
            "--meta", f"n:{numDS}",
            "--meta", f"t:{t_val}",
            "--meta", "D:7200",
            "--meta", f"DS_name:{DS_name}",
            "--meta", f"th_config:{th}",
            "--meta", f"DS_config:{ds}",
            "--meta", f"k:{numKeys}",
            "--meta", "i:10"
        ]

        if run_perf:
            file_suffix = "_perl" if is_perlmutter else ""
            raw_perf_filename = f"raw_perf_{DS_name}_{numDS}_{numKeys}_{th}_{ds}{file_suffix}.csv"
            clean_perf_filename = f"perf_{DS_name}_{numDS}_{numKeys}_{th}_{ds}{file_suffix}.csv"
            
            perf_path = graph_dir / "perf"
            perf_path.mkdir(parents=True, exist_ok=True)
            
            raw_perf_out = perf_path / raw_perf_filename
            clean_perf_out = perf_path / clean_perf_filename
            
            perf_prefix = [
                "perf", "stat",
                "-e", "ls_any_fills_from_sys.mem_io_local,ls_any_fills_from_sys.mem_io_remote",
                "-I", "2000",
                "-x,",
                "-o", str(raw_perf_out)
            ]
            
            cmd_list = perf_prefix + cmd_list
            print(f"-> Perf Profiling Enabled. Processing output to: {clean_perf_out}")

        print(f"\n-> Running Combination: Thread={th} | DataStructure={ds}")
        print(f"Executing: {' '.join(cmd_list)}")

        with open(output_csv, "a") as f:
            subprocess.run(cmd_list, cwd=experiment_folder, stdout=f, text=True, check=True)

        # Trigger Perf Output cleanup immediately after the command finishes
        if run_perf:
            clean_perf_csv(raw_perf_out, clean_perf_out)

        print("-> Run complete. Sleeping 10s to clear memory caches...")
        time.sleep(10)

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
    parser.add_argument('--perf', action='store_true')
    
    parser.add_argument('--numDS', type=str, default="3000000")
    parser.add_argument('--numKeys', type=str, default="240")

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
    
    an_folder = "AN_on" if args.AN == 1 else "AN_off"

    header_str = "Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, keyspace, interval, Op0, Op1, TotalOps\n"

    try:
        compile_experiment(args.UMF, args.numafy, ROOT_DIR, JEMALLOC_ROOT, EXPERIMENT_FOLDER)

        for ds in args.DS:
            print(f"\n=======================================================")
            print(f"Starting Experiment Phase for Data Structure: {ds}")
            print(f"=======================================================")

            file_suffix = "_perl.csv" if args.perlmutter else ".csv"
            specific_filename = f"{ds}_{args.numDS}_{args.numKeys}_experiments{file_suffix}"
            exp_filename = f"{ds}_experiments.csv"
            
            out_specific_path = OUT_BASE / an_folder / specific_filename
            graph_specific_path = GRAPH_BASE / an_folder / specific_filename
            
            out_exp_path = OUT_BASE / an_folder / exp_filename
            graph_exp_path = GRAPH_BASE / an_folder / exp_filename

            for target_path in [out_exp_path, graph_exp_path]:
                if not target_path.exists() or target_path.stat().st_size == 0:
                    target_path.parent.mkdir(parents=True, exist_ok=True)
                    with target_path.open("w") as f:
                        f.write(header_str)

            for spec_path in [out_specific_path, graph_specific_path]:
                spec_path.parent.mkdir(parents=True, exist_ok=True)
                with spec_path.open("w") as f:
                    f.write(header_str)

            run_experiment(out_specific_path.absolute(), graph_specific_path.parent, EXPERIMENT_FOLDER, ds, args.numDS, args.numKeys, args.AN, args.perlmutter, args.perf)

            with open(out_specific_path, "r") as f:
                lines = f.readlines()
                data_lines = lines[1:] 

            with open(graph_specific_path, "a") as f:
                f.writelines(data_lines)

            with open(out_exp_path, "a") as f:
                f.writelines(data_lines)
            with open(graph_exp_path, "a") as f:
                f.writelines(data_lines)
                
            print(f"--- Data safely written to isolated files and appended to master aggregates ---")

            if args.graph:
                plot_script = os.path.join(ROOT_DIR, "scripts/line_plot_bst.py")
                subprocess.run(f'python3 {plot_script} --AN {args.AN} --ds_name "{ds}" --numDS {args.numDS} --numKeys {args.numKeys}', shell=True)
                
            print(f"COMPLETE. Primary Results for {ds} saved to: {out_specific_path}")

    except subprocess.CalledProcessError as e:
        print(f"\n[FATAL ERROR] Experiment failed during execution (Exit Code: {e.returncode})")
        sys.exit(e.returncode)
    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected runtime error occurred: {e}")
        sys.exit(1)
