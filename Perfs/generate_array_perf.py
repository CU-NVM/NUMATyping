import subprocess
import argparse
import os
from pathlib import Path
import csv
import re
import sys

# ----------------------------------------------------
# Utilities
# ----------------------------------------------------

def ensure_dir(path: Path) -> Path:
    """Ensure 'path' exists; return absolute path."""
    path.mkdir(parents=True, exist_ok=True)
    return path.resolve()

# ----------------------------------------------------
# AutoNUMA toggle
# ----------------------------------------------------

def set_autonuma(desired: int) -> None:
    """
    Enable/disable AutoNUMA (0/1).
    """
    numa_path = Path("/proc/sys/kernel/numa_balancing")

    if not numa_path.exists():
        # Just a warning instead of a crash, in case running on non-Linux for testing
        print("Warning: /proc/sys/kernel/numa_balancing not found. Skipping AutoNUMA toggle.")
        return

    if desired not in (0, 1):
        raise ValueError("AutoNUMA must be 0 or 1.")

    try:
        cur = int(numa_path.read_text().strip())
    except PermissionError:
        # If we can't read, we likely can't write, so proceed to sudo try
        cur = -1 

    if cur == desired:
        print(f"AutoNUMA already {cur} (no change).")
        return

    # Try direct write
    try:
        numa_path.write_text(str(desired))
    except PermissionError:
        # Fallback to sudo sysctl
        cmd = ["sudo", "sysctl", "-w", f"kernel.numa_balancing={desired}"]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            raise RuntimeError(
                f"Failed to set AutoNUMA via sysctl.\nOutput:\n{r.stdout}\n{r.stderr}"
            )

    print(f"AutoNUMA set to {desired} successfully.")

# ----------------------------------------------------
# Build
# ----------------------------------------------------

def compile_experiment(experiment_folder: Path, root_dir: Path, UMF: bool) -> None:
    if not experiment_folder.exists():
        raise RuntimeError(f"Experiment folder does not exist: {experiment_folder}")

    print(f"Compiling in: {experiment_folder}")
    print(f"Using ROOT_DIR: {root_dir}")

    # Clean
    subprocess.run(["make", "clean"], cwd=experiment_folder, check=True)

    # Build command
    # We pass ROOT_DIR to make so it overrides the internal default
    build_cmd = ["make", f"ROOT_DIR={root_dir}"]
    
    if UMF:
        build_cmd.append("UMF=1")

    # Run the build
    subprocess.run(build_cmd, cwd=experiment_folder, check=True)

def convert_perf_to_csv(input_file):
    """Converts a perf stat output file into a CSV file."""
    input_path = Path(input_file)
    output_csv = input_path.with_suffix(".csv")

    if not input_path.exists():
        print(f"Error: Perf file {input_path} not found.")
        return None

    with open(input_path, "r") as f:
        lines = f.readlines()

    pattern = re.compile(r"^\s*(\d+\.\d+)\s+([\d,]+)\s+ocr\.demand_data_rd\.(remote_dram|local_dram)")

    time_to_values = {}

    for line in lines:
        match = pattern.match(line)
        if match:
            time_str, count, event_type = match.groups()
            time_val = float(time_str)
            count_val = int(count.replace(",", ""))

            if time_val not in time_to_values:
                time_to_values[time_val] = {"remote_dram": 0, "local_dram": 0}

            time_to_values[time_val][event_type] = count_val

    with open(output_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "Remote DRAM Accesses", "Local DRAM Accesses"])
        for t, vals in sorted(time_to_values.items()):
            writer.writerow([t, vals["remote_dram"], vals["local_dram"]])

    print(f"Converted {input_file} -> {output_csv}")
    return output_csv

# ----------------------------------------------------
# Perf run
# ----------------------------------------------------

def perf_array_experiment(experiment_folder: Path, output_dir: Path) -> None:
    ensure_dir(output_dir)

    # Fixed array parameters
    th_config = "reverse"
    ds_config = "numa"
    t = 80
    n = 1000
    u = 120
    s = 1000000
    i = 10

    array_bin = experiment_folder / "bin" / "array"

    if not array_bin.exists():
        raise FileNotFoundError(f"Binary not found at {array_bin}. Did compilation fail?")

    # Full array command
    # Using 'int(t)' etc ensures we don't accidentally pass floats if variables change type later
    array_cmd_str = (
        f"numactl --cpunodebind=0,1 --membind=0,1 "
        f"{array_bin} "
        f"--th_config={th_config} "
        f"--DS_config={ds_config} "
        f"-t {t} -n {n} -u {u} -s {s} -i {i}"
    )

    perf_output_file = output_dir / f"{th_config}_{ds_config}_{n}_{s}_perf.data"

    # Construct the perf command
    # -I 2000 means print stats every 2000ms
    perf_cmd = [
        "sudo", "perf", "stat",
        "-e", "ocr.demand_data_rd.remote_dram",
        "-e", "ocr.demand_data_rd.local_dram",
        "-I", "2000",
        "-o", str(perf_output_file),
        "--", "bash", "-c", array_cmd_str
    ]

    print("Running perf stat on array...")
    print(f"Command: {' '.join(perf_cmd)}")
    
    try:
        subprocess.run(perf_cmd, check=True)
        print(f"Perf output saved to {perf_output_file}")
        # Convert perf output to CSV
        convert_perf_to_csv(perf_output_file)
    except subprocess.CalledProcessError as e:
        print(f"Error running perf: {e}")

# ----------------------------------------------------
# main
# ----------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run array perf experiment.")
    parser.add_argument("--UMF", action="store_true", help="Compile with UMF=1")
    parser.add_argument("--AN", type=int, choices=[0, 1], default=1, help="AutoNUMA (0 or 1)")
    
    # 1. New Argument for ROOT_DIR
    # Defaults to $HOME/NUMATyping if not provided
    default_root = Path.home() / "NUMATyping"
    parser.add_argument(
        "--root_dir", 
        type=Path, 
        default=default_root, 
        help=f"Path to project root (default: {default_root})"
    )

    args = parser.parse_args()

    # Resolve paths based on args.root_dir
    root_path = args.root_dir.resolve()
    experiment_path = root_path / "Array"
    
    if not root_path.exists():
        print(f"Error: The provided root directory does not exist: {root_path}")
        sys.exit(1)

    # 1) AutoNUMA
    set_autonuma(args.AN)

    # Determine Output path based on AutoNUMA state
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    output_dir = root_path / "Perfs" / an_folder / "array"

    # 2) Build
    print("Compiling experiment...")
    try:
        compile_experiment(experiment_path, root_path, args.UMF)
    except Exception as e:
        print(f"Compilation failed: {e}")
        sys.exit(1)

    # 3) Run Perf
    print("Running array + Perf...")
    perf_array_experiment(experiment_path, output_dir)

    print(f"All done. Results saved under: {output_dir}")