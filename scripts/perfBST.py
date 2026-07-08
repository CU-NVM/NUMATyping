#!/usr/bin/env python3
import subprocess
import argparse
import os
from pathlib import Path
import csv
import sys
import threading
import time

# ----------------------------------------------------
# Clean path setup
# ----------------------------------------------------

def ensure_dir(path: Path) -> Path:
    """Ensure 'path' exists; return absolute path."""
    path.mkdir(parents=True, exist_ok=True)
    return path.resolve()

# ----------------------------------------------------
# Sudo Keep-Alive
# ----------------------------------------------------

def keep_sudo_alive():
    """
    Runs `sudo -v` periodically in the background to keep the sudo ticket 
    alive, preventing password prompts during long experiments.
    """
    while True:
        time.sleep(60)  # Refresh the token every 60 seconds
        subprocess.run(["sudo", "-v"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# ----------------------------------------------------
# Build
# ----------------------------------------------------

def compile_experiment(UMF: bool, experiment_folder: Path) -> None:
    if not experiment_folder.exists():
        raise RuntimeError(f"Experiment folder does not exist: {experiment_folder}")

    print(f"\n--- Compiling in {experiment_folder} ---")
    
    subprocess.run(["make", "clean"], cwd=experiment_folder, check=False)

    build_cmd = ["make"]
    if UMF:
        build_cmd.append("UMF=1")

    subprocess.run(build_cmd, cwd=experiment_folder, check=True)

# ----------------------------------------------------
# CSV Conversion
# ----------------------------------------------------

def convert_perf_to_csv(input_file):
    """Converts a raw perf stat output file into a clean CSV file."""
    input_path = Path(input_file)
    output_csv = input_path.with_suffix(".csv")

    with open(input_path, "r") as f:
        lines = f.readlines()

    time_to_values = {}

    for line in lines:
        line = line.strip()
        if line.startswith("#") or not line:
            continue
            
        parts = line.split()
        if len(parts) >= 3:
            try:
                time_val = float(parts[0])
                count_str = parts[1].replace(",", "")
                
                # Handle <not counted> or <not supported>
                if count_str == "<not":
                    count = 0
                    event_name = parts[3].lower()
                else:
                    count = int(count_str)
                    event_name = parts[2].lower()
                
                if time_val not in time_to_values:
                    time_to_values[time_val] = {"remote": 0, "local": 0}
                    
                if "remote" in event_name:
                    time_to_values[time_val]["remote"] = count
                elif "local" in event_name:
                    time_to_values[time_val]["local"] = count
            except ValueError:
                continue

    with open(output_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "Remote Accesses", "Local Accesses"])
        for t, vals in sorted(time_to_values.items()):
            writer.writerow([t, vals["remote"], vals["local"]])

    print(f"Converted {input_file} → {output_csv}")
    return output_csv

# ----------------------------------------------------
# Perf run
# ----------------------------------------------------

def perf_ds_experiment(output_dir: Path, data_structures: list, experiment_folder: Path, an_setting: int, numDS: str, numKeys: str, is_perlmutter: bool, stop_time: int) -> None:
    output_dir = ensure_dir(output_dir)

    configs = [
        # ("numa", "numa"),
        # ("numa", "regular"),
        # ("regular", "numa"),
        ("regular", "regular")
    ]

    # Dynamically build numactl flags and threads based on system
    if is_perlmutter:
        numactl_base = "--cpunodebind=0,7 --membind=0,7"
        t_val = "128"
        event_remote = "ls_any_fills_from_sys.mem_io_remote"
        event_local = "ls_any_fills_from_sys.mem_io_local"
    else:
        numactl_base = "--cpunodebind=0,1 --membind=0,1"
        t_val = "80"
        event_remote = "ocr.demand_data_rd.remote_dram"
        event_local = "ocr.demand_data_rd.local_dram"

    if an_setting == 1:
        numactl_flags = f"--balancing {numactl_base}"
    else:
        numactl_flags = numactl_base

    for ds in data_structures:
        print(f"\n=======================================================")
        print(f"Starting Perf Collection for Data Structure: {ds}")
        print(f"=======================================================")

        for th_config, ds_config in configs:
            print(f"\n--- Testing Config: TH={th_config}, DS={ds_config} ---")

            # Wrapping the execution inside Python meta.py to maintain the DataStructure environment variables and arguments
            ds_cmd = (
                f"cd {experiment_folder} && python3 meta.py "
                f"numactl {numactl_flags} "
                f"./bin/datastructures "
                f"--meta n:{numDS} "
                f"--meta t:{t_val} "
                f"--meta D:{stop_time} "
                f"--meta DS_name:{ds} "
                f"--meta th_config:{th_config} "
                f"--meta DS_config:{ds_config} "
                f"--meta k:{numKeys} "
                f"--meta i:10"
            )

            file_suffix = "_perl" if is_perlmutter else ""
            perf_output = output_dir / f"perf_{ds}_{numDS}_{numKeys}_{th_config}_{ds_config}{file_suffix}.data"

            perf_cmd = (
                f"sudo perf stat "
                f"-e {event_remote} "
                f"-e {event_local} "
                f"-I 2000 "
                f"-o {perf_output} "
                f"-- bash -c \"{ds_cmd}\""
            )

            print(f"Executing: {perf_cmd}")
            subprocess.run(perf_cmd, shell=True, check=True)
            
            # Convert the resulting .data file to the final .csv
            if perf_output.exists():
                convert_perf_to_csv(str(perf_output))
                # Optional: Uncomment the line below to delete the raw .data file after conversion
                # os.remove(perf_output)
                
            print("-> Run complete. Sleeping 10s to clear memory caches...")
            time.sleep(10)

# ----------------------------------------------------
# main
# ----------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run Data Structure perf experiments.")
    parser.add_argument("--DS", type=str, nargs='+', required=True, help="List of data structures to test")
    parser.add_argument("--UMF", action="store_true", help="Compile with UMF=1")
    parser.add_argument("--AN", type=int, choices=[0, 1], default=1, help="AutoNUMA (0 or 1)")
    parser.add_argument("--perlmutter", action="store_true", help="Use Perlmutter config (AMD events, 128 threads)")
    parser.add_argument("--numDS", type=str, default="1000000", help="Number of elements")
    parser.add_argument("--numKeys", type=str, default="80", help="Keyspace size")
    parser.add_argument("--stop", type=int, default=1200, help="Duration to run each combination in seconds (Default: 1200)")
    parser.add_argument("--ROOT_DIR", type=str, default=os.path.expanduser("~/NUMATyping"), help="Path to root directory")
    
    try:
        args = parser.parse_args()
    except Exception as e:
        parser.print_help()
        sys.exit(1)

    # Perlmutter specific defaults fallback
    if args.perlmutter:
        if '--numDS' not in sys.argv:
            args.numDS = "3000000"
        if '--numKeys' not in sys.argv:
            args.numKeys = "240"

    ROOT_DIR = Path(args.ROOT_DIR).resolve()
    if not ROOT_DIR.exists():
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = ROOT_DIR / "Output" / "DataStructureTests"

    # --- Authenticate sudo once and keep it alive ---
    print("\n--- Initializing Sudo Access ---")
    print("You may be prompted for your password to allow 'perf stat' to run without interruption.")
    try:
        subprocess.run(["sudo", "-v"], check=True)
    except subprocess.CalledProcessError:
        print("Error: Sudo authentication failed. Exiting.")
        sys.exit(1)
        
    # Start the daemon thread to keep sudo active
    sudo_thread = threading.Thread(target=keep_sudo_alive, daemon=True)
    sudo_thread.start()
    print("Sudo ticket initialized and keep-alive daemon started.")
    # -----------------------------------------------------

    # 1) Route output directly to the Graphs directory
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    output_dir = ROOT_DIR / "Graphs" / "perf" / an_folder

    # 2) Build execution binary
    compile_experiment(args.UMF, EXPERIMENT_FOLDER)

    # 3) Run Experiments
    perf_ds_experiment(output_dir, args.DS, EXPERIMENT_FOLDER, args.AN, args.numDS, args.numKeys, args.perlmutter, args.stop)

    print(f"\nALL COMPLETE. Results saved under: {output_dir.resolve()}")