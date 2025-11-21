import subprocess
import argparse
import os
from pathlib import Path

experiment_folder = "./Output/ycsb/"
output_filename = "ycsb_experiments.csv"

def ensure_dir(path: str) -> str:
    """Ensure path is a directory; create it if missing; return absolute path."""
    os.makedirs(path, exist_ok=True)
    return os.path.abspath(path)

def set_autonuma(desired: int) -> None:
    """
    Set AutoNUMA globally to 0 or 1.
    Tries direct /proc write, then falls back to 'sudo sysctl -w'.
    Verifies and raises on failure.
    """
    
    try:
        with open("/proc/sys/kernel/numa_balancing", "r") as f:
            cur =int(f.read().strip())
    except FileNotFoundError:
        raise RuntimeError("This kernel doesn't expose /proc/sys/kernel/numa_balancing (no AutoNUMA support?)")

    if desired not in (0, 1):
        raise ValueError("AutoNUMA value must be 0 or 1")

    if cur == desired:
        print(f"AutoNUMA already {cur} (no change).")
        return

    # 1) Try direct write (works if running as root)
    try:
        with open("/proc/sys/kernel/numa_balancing", "w") as f:
            f.write(str(desired))
    except PermissionError:
        # 2) Fall back to sudo sysctl
        cmd = ["sudo", "sysctl", f"kernel.numa_balancing={desired}"]
        r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode != 0:
            msg = r.stdout.strip()
            raise RuntimeError(f"Failed to set AutoNUMA via sysctl (exit {r.returncode}). Output:\n{msg}")

    # Verify
    try:
        with open("/proc/sys/kernel/numa_balancing", "r") as f:
            new_val =int(f.read().strip())
    except FileNotFoundError:
        raise RuntimeError("This kernel doesn't expose /proc/sys/kernel/numa_balancing (no AutoNUMA support?)")
    if new_val != desired:
        raise RuntimeError(f"Tried to set AutoNUMA to {desired}, but kernel reports {new_val}.")
    print(f"AutoNUMA set to {new_val} successfully.")


def compile_experiment(UMF: bool) -> None:
    # Clean previous builds
    subprocess.run(f"cd {experiment_folder} && make clean", shell=True, check=False)

    # Compile with or without UMF
    if UMF:
        subprocess.run(f"cd {experiment_folder} && make UMF=1", shell=True, check=False)
    else:
        subprocess.run(f"cd {experiment_folder} && make", shell=True, check=False)

def write_header_once(csv_path: Path) -> None:
    """Write header only if file doesn't exist or is empty."""
    header = (
        "Date, Time, num_tables,  num_threads, thread_config, DS_config, buckets, workload, duration, "
        "num_keys, locality, interval, ops_node0, ops_node1, total_ops\n"
    )
    if not csv_path.exists() or csv_path.stat().st_size == 0:
        # Ensure parent dir exists
        csv_path.parent.mkdir(parents=True, exist_ok=True)
        with csv_path.open("w") as f:
            f.write(header)

def run_experiment(output_csv: Path) -> None:
    # Append results under the header
    # We’ll redirect stdout of meta.py to the SAME csv file
    cmd = (
        f'cd {experiment_folder} && '
        f'python3 meta.py '
        f'numactl --cpunodebind=0,1 --membind=0,1 '
        f'./bin/ycsb '
        f'--meta th_config:numa:regular '
        f'--meta DS_config:numa:regular '
        f'--meta t:40:80 '
        f'--meta b:1333 '
        f'--meta w:A:B:C:D:E:F '
        f'--meta u:120 '
        f'--meta k:10000000 '
        f'--meta l:80-20:50-50:90-10:95-5:98-2 '
        f'--meta i:10 '
        f'--meta a:1000 '
        f'>> "{output_csv}"'
    )
    subprocess.run(cmd, shell=True, check=False)




def graph_data(should_graph: bool, autonuma) -> None:
    if should_graph:
        if (autonuma == 1):
            output_dir = Path(args.output) / "AN_on"
        elif (autonuma == 0):
            output_dir = Path(args.output) / "AN_off"
        plot_cmd = f'cd Result/plots && python3 plot_histogram.py "{output_dir}/{output_filename}" --show --save {output_dir}/figs'
        subprocess.run(plot_cmd, shell=True, check=False)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run ycsb experiment and append results to CSV.")
    parser.add_argument('--UMF', action='store_true', help="Build with UMF=1.")
    parser.add_argument(
        "-d", "--output", type=ensure_dir, required=True, help="Directory where the CSV will be written (will be created if missing)."
    )
    parser.add_argument('--graph', action='store_true', help="Generate graphs after running experiments.")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=1, help="Set autonuma flag (0 or 1)")

    args = parser.parse_args()

    set_autonuma(args.AN)

    output_dir = Path(args.output)
    output_file_path = ""

    if (args.AN == 1):
        output_file_path = output_dir / "AN_on" / output_filename
    elif (args.AN == 0):
        output_file_path = output_dir / "AN_off" / output_filename

    # 1) Ensure header exists (first line)
    write_header_once(output_file_path)

    # 2) Build
    compile_experiment(args.UMF)

    # 3) Run and append results
    run_experiment(output_file_path)

    #graph_data(args.graph, args.AN)
    # 4) Graph experiments
    print(f"Results appended to: {output_file_path}")
