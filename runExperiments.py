#!/usr/bin/python

import shutil
import subprocess
import sys
import argparse
from pathlib import Path
import os
#threads we want to run for = t:4:8:12:16:20:24:28:32:36:40

experiment_folder = "./Output/DataStructureTests/"


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

        
def run_command(command):
    # Start the command with stdout and stderr redirected to subprocess.PIPE
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, shell=True)

    # Read and print each line from stdout as it becomes available
    while True:
        output = process.stdout.readline()
        if output == "" and process.poll() is not None:
            break
        if output:
            print(output.strip())  # Print output line by line

    # Wait for the process to complete
    process.wait()

def run_experiment(output_file_path, DS_name):
    """
    Run the experiment inside experiment_folder.
    If successful, append output to CSV file.
    If failed, dump all output to terminal.
    """
    cmd = (
        "python3 meta.py "
        "numactl --cpunodebind=0,1 --membind=0,1 "
        "./bin/DataStructureTest "
        "--meta n:10240 "
        "--meta t:40:80 "
        "--meta D:800 "
        f"--meta DS_name:{DS_name} "
        "--meta th_config:numa:regular:reverse "
        "--meta DS_config:numa:regular "
        "--meta k:160 "
        "--meta i:10"
    )

    print(f"Running experiment in {experiment_folder}\n{cmd}\n")

    # Run and capture both stdout and stderr
    result = subprocess.run(
        cmd,
        shell=True,
        cwd=os.path.abspath(experiment_folder),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    if result.returncode == 0:
        # Success — append output to CSV file
        with open(output_file_path, "a") as out:
            out.write(result.stdout)
        print(f"Experiment completed successfully. Output appended to {output_file_path}\n")
    else:
        print("\nExperiment failed!")
        print(f"Exit code: {result.returncode}")
        print(f"Command: {cmd}")
        print(result.stdout)
        # Optionally re-raise or exit non-zero
        raise subprocess.CalledProcessError(result.returncode, cmd)
def write_header_once(output_file_path: Path) -> None:
    """Write header only if file doesn't exist or is empty."""
    header = (
        "Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration,keyspace, interval, Op0, Op1, TotalOps\n"
    )
    if not output_file_path.exists() or output_file_path.stat().st_size == 0:
        # Ensure parent dir exists
        output_file_path.parent.mkdir(parents=True, exist_ok=True)
        with output_file_path.open("w") as f:
            f.write(header)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Copy folder and run a command with an additional argument.")
    parser.add_argument('--DS', type=str, required=True, help="Specify the data structure name")
    parser.add_argument('--UMF', action='store_true', help="Enable verbose mode")
    parser.add_argument("-d", "--output", type=ensure_dir, required=True, help="Output directory")
    parser.add_argument('--graph', action='store_true', help="Generate graphs after experiment")
    parser.add_argument('--AN', type=int, choices=[0, 1], default=1, help="Set autonuma flag (0 or 1)")
    parser.add_argument('--verbose', action='store_true', help="Enable verbose mode")
    
    args = parser.parse_args()
    set_autonuma(args.AN)
    output_dir = Path(args.output)
    output_file_path = ""
    if (args.AN == 1):
        output_file_path = output_dir / "AN_on" / f"{args.DS}_Transactions.csv"
    elif (args.AN == 0):
        output_file_path = output_dir / "AN_off" / f"{args.DS}_Transactions.csv"


    write_header_once(output_file_path)
    compile_experiment(args.UMF)

    run_experiment(output_file_path, args.DS)


    # run_command_and_redirect_output(command, f"./Result/stack.csv")
    
    
    