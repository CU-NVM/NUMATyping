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
        print("Warning: /proc/sys/kernel/numa_balancing not found. Skipping AutoNUMA toggle.")
        return

    if desired not in (0, 1):
        raise ValueError("AutoNUMA must be 0 or 1.")

    try:
        cur = int(numa_path.read_text().strip())
    except PermissionError:
        cur = -1 

    if cur == desired:
        print(f"AutoNUMA already {cur} (no change).")
        return

    try:
        numa_path.write_text(str(desired))
    except PermissionError:
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
    subprocess.run(["make", "clean"], cwd=experiment_folder, check=True)

    build_cmd = ["make", f"ROOT_DIR={root_dir}"]
    if UMF:
        build_cmd.append("UMF=1")

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

    # UPDATED REGEX: Matches ls_dmnd_fills_from_sys.mem_io_local and .mem_io_remote
    pattern = re.compile(r"^\s*(\d+\.\d+)\s+([\d,]+)\s+ls_dmnd_fills_from_sys\.(mem_io_remote|mem_io_local)")

    time_to_values = {}

    for line in lines:
        match = pattern.match(line)
        if match:
            time_str, count, event_type = match.groups()
            time_val = float(time_str)
            count_val = int(count.replace(",", ""))

            if time_val not in time_to_values:
                time_to_values[time_val] = {"mem_io_remote": 0, "mem_io_local": 0}

            time_to_values[time_val][event_type] = count_val

    with open(output_csv, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Time", "Remote DRAM Accesses", "Local DRAM Accesses"])
        for t, vals in sorted(time_to_values.items()):
            # Map internal keys back to readable CSV columns
            writer.writerow([t, vals["mem_io_remote"], vals["mem_io_local"]])

    print(f"Converted {input_file} -> {output_csv}")
    return output_csv

# ----------------------------------------------------
# Perf run
# ----------------------------------------------------

def perf_array_experiment(experiment_folder: Path, output_dir: Path) -> None:
    ensure_dir(output_dir)

    th_config = "numa"
    ds_config = "numa"
    t = 128
    n = 1000
    u = 120
    s = 2000000
    i = 10

    array_bin = experiment_folder / "bin" / "array"

    if not array_bin.exists():
        raise FileNotFoundError(f"Binary not found at {array_bin}.")

    array_cmd_str = (
        f"numactl --cpunodebind=0,7 --membind=0,7 "
        f"{array_bin} "
        f"--th_config={th_config} "
        f"--DS_config={ds_config} "
        f"-t {t} -n {n} -u {u} -s {s} -i {i}"
    )

    perf_output_file = output_dir / f"{th_config}_{ds_config}_{n}_{s}_perf.data"

    # UPDATED EVENTS: Replaced Intel OCR with AMD ls_dmnd_fills_from_sys
    perf_cmd = [
        "perf", "stat",
        "-e", "ls_dmnd_fills_from_sys.mem_io_remote",
        "-e", "ls_dmnd_fills_from_sys.mem_io_local",
        "-I", "2000",
        "-o", str(perf_output_file),
        "--", "bash", "-c", array_cmd_str
    ]

    print("Running perf stat on array (AMD Zen Events)...")
    try:
        subprocess.run(perf_cmd, check=True)
        convert_perf_to_csv(perf_output_file)
    except subprocess.CalledProcessError as e:
        print(f"Error running perf: {e}")

# ----------------------------------------------------
# Main execution logic remains the same
# ----------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run array perf experiment.")
    parser.add_argument("--UMF", action="store_true", help="Compile with UMF=1")
    parser.add_argument("--AN", type=int, choices=[0, 1], default=1, help="AutoNUMA (0 or 1)")
    
    default_root = Path.home() / "NUMATyping"
    parser.add_argument("--root_dir", type=Path, default=default_root)

    args = parser.parse_args()
    root_path = args.root_dir.resolve()
    experiment_path = root_path / "Array"
    
    if not root_path.exists():
        print(f"Error: Root directory does not exist: {root_path}")
        sys.exit(1)

    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    output_dir = root_path / "Perfs" / an_folder / "array"

    compile_experiment(experiment_path, root_path, args.UMF)
    perf_array_experiment(experiment_path, output_dir)
