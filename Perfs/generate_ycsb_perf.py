import subprocess
import argparse
import os
from pathlib import Path
import csv
import re

# ----------------------------------------------------
# Clean path setup
# ----------------------------------------------------

HOME = Path.home()
ROOT = HOME / "NUMATyping"
EXPERIMENT_FOLDER = ROOT / "Output" / "ycsb"



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
        raise RuntimeError("This kernel does not expose /proc/sys/kernel/numa_balancing.")

    if desired not in (0, 1):
        raise ValueError("AutoNUMA must be 0 or 1.")

    cur = int(numa_path.read_text().strip())
    if cur == desired:
        print(f"AutoNUMA already {cur} (no change).")
        return

    # Try direct write
    try:
        numa_path.write_text(str(desired))
    except PermissionError:
        # Fallback to sudo sysctl
        cmd = ["sudo", "sysctl", f"kernel.numa_balancing={desired}"]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            raise RuntimeError(
                f"Failed to set AutoNUMA via sysctl.\nOutput:\n{r.stdout}\n{r.stderr}"
            )

    # Verify
    new_val = int(numa_path.read_text().strip())
    if new_val != desired:
        raise RuntimeError(f"AutoNUMA verification failed: expected {desired}, got {new_val}")

    print(f"AutoNUMA set to {new_val} successfully.")


# ----------------------------------------------------
# Build
# ----------------------------------------------------

def compile_experiment(UMF: bool) -> None:
    if not EXPERIMENT_FOLDER.exists():
        raise RuntimeError(f"Experiment folder does not exist: {EXPERIMENT_FOLDER}")

    # Clean
    subprocess.run(["make", "clean"], cwd=EXPERIMENT_FOLDER)

    # Build
    build_cmd = ["make"]
    if UMF:
        build_cmd.append("UMF=1")

    subprocess.run(build_cmd, cwd=EXPERIMENT_FOLDER)

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
def perf_ycsb_experiment(output_dir: Path) -> None:
    output_dir = ensure_dir(output_dir)

    # Fixed YCSB parameters
    th_config = "numa"
    ds_config = "numa"
    t = 40
    b = 1333
    w = "D"
    u = 180
    k = 10000000
    l = "90-10"
    i = 10
    a = 1000

    ycsb_bin = EXPERIMENT_FOLDER / "bin" / "ycsb"

    # Full YCSB command
    ycsb_cmd = (
        f"numactl --cpunodebind=0,1 --membind=0,1 "
        f"{ycsb_bin} "
        f"--th_config={th_config} "
        f"--DS_config={ds_config} "
        f"-t {t} -b {b} --w={w} -u {u} -k {k} --l={l} -i {i} -a {a}"
    )

    perf_output = output_dir / f"{th_config}_{ds_config}_{w}_{l}_perf.data"

    # *** perf_cmd is now ONE STRING ***
    perf_cmd = (
        f"sudo perf stat "
        f"-e ocr.demand_data_rd.remote_dram "
        f"-e ocr.demand_data_rd.local_dram "
        f"-I 2000 "
        f"-o {perf_output} "
        f"-- bash -c \"{ycsb_cmd}\""
    )

    print("Running perf stat on YCSB…")
    subprocess.run(perf_cmd, shell=True)
    print(f"Perf output saved to {perf_output}")
    # Convert perf output to CSV
    convert_perf_to_csv(str(perf_output))

# ----------------------------------------------------
# main
# ----------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run YCSB flamegraph experiment.")
    parser.add_argument("--UMF", action="store_true", help="Compile with UMF=1")
    parser.add_argument("--graph", action="store_true", help="Generate FlameGraphs")
    parser.add_argument("--AN", type=int, choices=[0, 1], default=1, help="AutoNUMA (0 or 1)")
    args = parser.parse_args()

    # 1) AutoNUMA
    set_autonuma(args.AN)

    # Output path based on AutoNUMA state
    if args.AN == 1:
        output_dir = Path("AN_on") / "ycsb"
    else:
        output_dir = Path("AN_off") / "ycsb"

    # 2) Build
    print("Compiling experiment...")
    compile_experiment(args.UMF)

    # 3) Run FlameGraph
    print("Running YCSB + FlameGraph...")
    perf_ycsb_experiment(output_dir)

    print(f"Results saved under: {output_dir.resolve()}")
