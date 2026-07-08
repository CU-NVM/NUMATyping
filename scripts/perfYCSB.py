#!/usr/bin/env python3
import subprocess
import argparse
import os
from pathlib import Path
import csv
import re
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
    
    # cwd=experiment_folder ensures 'make' is executed directly inside ROOT/Output/ycsb
    subprocess.run(["make", "clean"], cwd=experiment_folder, check=False)

    build_cmd = ["make"]
    if UMF:
        build_cmd.append("UMF=1")

    subprocess.run(build_cmd, cwd=experiment_folder, check=True)

# ----------------------------------------------------
# CSV Conversion
# ----------------------------------------------------

def convert_perf_to_csv(input_file):
    """Converts a perf stat output file into a CSV file."""
    input_path = Path(input_file)
    output_csv = input_path.with_suffix(".csv")

    with open(input_path, "r") as f:
        lines = f.readlines()

    data = []
    pattern = re.compile(r"^\s*(\d+\.\d+)\s+([\d,]+)\s+ocr\.demand_data_rd\.(remote_dram|local_dram)")

    time_to_values = {}

    for line in lines:
        match = pattern.match(line)
        if match:
            time, count, event_type = match.groups()
            time = float(time)
            count = int(count.replace(",", ""))

            if time not in time_to_values:
                time_to_values[time] = {"remote_dram": 0, "local_dram": 0}

            time_to_values[time][event_type] = count

    with open(output_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "Remote DRAM Accesses", "Local DRAM Accesses"])
        for t, vals in sorted(time_to_values.items()):
            writer.writerow([t, vals["remote_dram"], vals["local_dram"]])

    print(f"Converted {input_file} → {output_csv}")
    return output_csv

# ----------------------------------------------------
# Perf run
# ----------------------------------------------------

def perf_ycsb_experiment(output_dir: Path, workloads: list, experiment_folder: Path, an_setting: int) -> None:
    output_dir = ensure_dir(output_dir)

    configs = [
        #("numa", "numa"),
        #("numa", "regular"),
        ("regular", "numa"),
        #("regular", "regular")
    ]

    # Dynamically build numactl flags based on the AN setting
    numactl_base = "--cpunodebind=0,1 --membind=0,1"
    if an_setting == 1:
        numactl_flags = f"--balancing {numactl_base}"
    else:
        numactl_flags = numactl_base

    for wl in workloads:
        # 1. Extract the clean label (e.g., "AD", "A", "B") from the workload string
        blocks = wl.replace('_', ',').split(',')
        letters = [b.split('-')[0] for b in blocks if '-' in b]
        unique_letters = []
        for l in letters:
            if l not in unique_letters: unique_letters.append(l)
        label = "".join(unique_letters)
        if not label: label = "WL"

        print(f"\n=======================================================")
        print(f"Starting Perf Collection for Workload: {wl} ({label})")
        print(f"=======================================================")

        for th_config, ds_config in configs:
            print(f"\n--- Testing Config: TH={th_config}, DS={ds_config} ---")

            # Execute binary cleanly inside the directory with dynamic numactl flags
            ycsb_cmd = (
                f"cd {experiment_folder} && "
                f"numactl {numactl_flags} "
                f"./bin/ycsb "
                f"--th_config {th_config} "
                f"--DS_config {ds_config} "
                f"-t 80 -b 133300 --workload {wl} -u 1200 -k 100000000 -i 20 -a 1000"
            )

            # The output directory path is absolute, so perf can safely write 
            # the data regardless of where the bash shell changes its directory.
            perf_output = output_dir / f"ycsb_{label}_{th_config}_{ds_config}.data"

            perf_cmd = (
                f"sudo perf stat "
                f"-e ocr.demand_data_rd.remote_dram "
                f"-e ocr.demand_data_rd.local_dram "
                f"-I 2000 "
                f"-o {perf_output} "
                f"-- bash -c \"{ycsb_cmd}\""
            )

            print(f"Executing: {perf_cmd}")
            subprocess.run(perf_cmd, shell=True, check=True)
            
            # Convert the resulting .data file to the final .csv
            convert_perf_to_csv(str(perf_output))

# ----------------------------------------------------
# main
# ----------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run YCSB perf experiments.")
    parser.add_argument("--UMF", action="store_true", help="Compile with UMF=1")
    parser.add_argument("--AN", type=int, choices=[0, 1], default=1, help="AutoNUMA (0 or 1)")
    parser.add_argument("--workloads", type=str, nargs='+', default=["A-50-50-50,D-100-0-50"], help="List of workloads")
    parser.add_argument("--ROOT_DIR", type=str, default=os.path.expanduser("~/NUMATyping"), help="Path to NUMATyping root directory")
    
    try:
        args = parser.parse_args()
    except Exception as e:
        parser.print_help()
        sys.exit(1)

    ROOT_DIR = Path(args.ROOT_DIR).resolve()
    if not ROOT_DIR.exists():
        print(f"Error: ROOT_DIR {ROOT_DIR} does not exist.")
        sys.exit(1)

    EXPERIMENT_FOLDER = ROOT_DIR / "Output" / "ycsb"

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

    # 3) Run Experiments (passing the AN argument to dictate the --balancing flag)
    perf_ycsb_experiment(output_dir, args.workloads, EXPERIMENT_FOLDER, args.AN)

    print(f"\nALL COMPLETE. Results saved under: {output_dir.resolve()}")