import subprocess
import argparse
import os
from pathlib import Path


# ----------------------------------------------------
# Clean path setup
# ----------------------------------------------------

HOME = Path.home()
ROOT = HOME / "NUMATyping"
EXPERIMENT_FOLDER = ROOT / "Output" / "ycsb"
OUTPUT_FILENAME = "ycsb_experiments.csv"


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


# ----------------------------------------------------
# FlameGraph run
# ----------------------------------------------------

def flame_ycsb_experiment(output_dir: Path) -> None:
    output_dir = ensure_dir(output_dir)

    # Fixed parameters
    th_config = "numa"
    ds_config = "numa"
    t = 40
    b = 1333
    w = "B"
    u =180
    k = 10000000
    l = "80-20"
    i = 10
    a = 1000

    ycsb_bin = EXPERIMENT_FOLDER / "bin" / "ycsb"
    flamegraph_path = ROOT / "FlameGraphs" / "FlameGraph"

    ycsb_cmd = (
        f"numactl --cpunodebind=0,1 --membind=0,1 "
        f"{ycsb_bin} "
        f"--th_config={th_config} "
        f"--DS_config={ds_config} "
        f"-t {t} -b {b} --w={w} -u {u} -k {k} --l={l} -i {i} -a {a}"
    )

    flame_output = output_dir / f"{th_config}_{ds_config}_{w}_flamegraph.svg"

    flame_cmd = (
        f"perf record -F 99 -g {ycsb_cmd} && "
        f"perf script > out.perf && {flamegraph_path}/stackcollapse-perf.pl out.perf > out.folded && "
        f"{flamegraph_path}/flamegraph.pl out.folded > {flame_output}"
    )

    print("Running FlameGraph command...")
    subprocess.run(flame_cmd, shell=True)
    print(f"FlameGraph written to {flame_output}")


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
    flame_ycsb_experiment(output_dir)

    print(f"Results saved under: {output_dir.resolve()}")
