#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from pathlib import Path
import sys
import os

# ============================================================================
# Configuration
# ============================================================================

# Define line colors here for easy adjustment
LOCAL_COLOR = "#e41919"   # Red
REMOTE_COLOR = "#0e41e7"  # Blue

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
Perf Result Plotting Tool (Square / LaTeX Optimized)
----------------------------------------------------
Generates square line graphs from perf memory access results.
Optimized for LaTeX integration (4 images across an A4 page).

USAGE:
    python3 plot_perf.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1]                AutoNUMA setting. 1 for AN_on/, 0 for AN_off/ (Required).
    --benchmark_workload STR  Prefix of the files, e.g., 'bst' or 'ycsb_AD' (Required).
    --perlmutter              Use Perlmutter paths and file suffixes (_perl.csv).
    --numDS STR               Number of elements (Default: 1000000 / 3000000 for perlmutter).
    --numKeys STR             Keyspace size (Default: 80 / 240 for perlmutter).
    --start_time INT          Seconds to skip at the beginning of the run (Default: 0).
    --stop INT                Stop graphing at this many seconds (Default: 0 [Graph all remaining points]).
    --sample INT              Interval in seconds to sample the data points (Default: 0 [Graph ALL points]).
    --ROOT_DIR PATH           Path to NUMATyping root (Default: ~/NUMATyping).
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    parser = argparse.ArgumentParser(description="Plot Perf Memory Access Results", add_help=False)
    parser.add_argument('--AN', type=int, choices=[0, 1], required=True)
    parser.add_argument('--benchmark_workload', type=str, required=True)
    parser.add_argument('--perlmutter', action='store_true')
    parser.add_argument('--numDS', type=str, default="1000000")
    parser.add_argument('--numKeys', type=str, default="80")
    parser.add_argument('--start_time', type=int, default=0, help="Skip initialization time")
    parser.add_argument('--stop', type=int, default=0, help="Stop graphing at this many seconds (0 = end of data)")
    parser.add_argument('--sample', type=int, default=0, help="Sampling interval in seconds")
    parser.add_argument('--ROOT_DIR', type=str, default=os.path.expanduser("~/NUMATyping"))
    return parser.parse_args()

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    try:
        args = parse_args()
    except Exception:
        show_help()

    # Dynamically update defaults if perlmutter is passed
    if args.perlmutter:
        if '--numDS' not in sys.argv:
            args.numDS = "3000000"
        if '--numKeys' not in sys.argv:
            args.numKeys = "240"

    root_dir = Path(args.ROOT_DIR).resolve()
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    an_suffix = "_ANon" if args.AN == 1 else "_ANoff"
    
    # ------------------------------------------------------------------------
    # UNIFIED DIRECTORY STRUCTURE
    # Both YCSB and DS perf scripts output to Graphs/perf/AN_folder
    # ------------------------------------------------------------------------
    if args.perlmutter:
        base_dir = root_dir / "Graphs" / "Perlmutter" / "perf" / an_folder
    else:
        base_dir = root_dir / "Graphs" / "perf" / an_folder
        
    figs_dir = base_dir / "figs"
    figs_dir.mkdir(parents=True, exist_ok=True)

    configs = [
        ("numa", "numa"),
        ("numa", "regular"),
        ("regular", "numa"),
        ("regular", "regular")
    ]

    for th_config, ds_config in configs:
        file_suffix = "_perl" if args.perlmutter else ""
        
        # --------------------------------------------------------------------
        # DYNAMIC FILENAME RESOLUTION
        # --------------------------------------------------------------------
        if args.benchmark_workload.startswith("ycsb"):
            filename = f"{args.benchmark_workload}_AD_{th_config}_{ds_config}{file_suffix}.csv"
        else:
            filename = f"perf_{args.benchmark_workload}_{args.numDS}_{args.numKeys}_{th_config}_{ds_config}{file_suffix}.csv"

        csv_path = base_dir / filename

        if not csv_path.exists():
            print(f"Warning: File {csv_path} does not exist. Skipping.")
            continue

        # 1. Read the CSV file
        df = pd.read_csv(csv_path, skipinitialspace=True)
        df.columns = df.columns.str.strip()

        # Handle legacy renaming just in case it reads an old uncleaned file
        rename_map = {
            'time': 'Time',
            'local_ops': 'Local Accesses',
            'remote_ops': 'Remote Accesses'
        }
        df.rename(columns=rename_map, inplace=True)

        # 2. Truncate the last 2 data entries to fix weird perf cutoffs
        if len(df) > 2:
            df = df.iloc[:-2].copy()

        expected_cols = ['Time', 'Remote Accesses', 'Local Accesses']
        if not all(col in df.columns for col in expected_cols):
            print(f"Warning: Missing columns in {filename}. Found: {list(df.columns)}. Skipping.")
            continue

        for col in expected_cols:
            df[col] = pd.to_numeric(df[col], errors='coerce')

        df = df.dropna(subset=expected_cols).copy()

        # 3. ANOMALY FILTER: Purge massive hardware counter glitches before processing
        if not df.empty:
            local_p99 = df['Local Accesses'].quantile(0.99)
            remote_p99 = df['Remote Accesses'].quantile(0.99)
            df = df[(df['Local Accesses'] <= local_p99 * 5) & 
                    (df['Remote Accesses'] <= remote_p99 * 5)].copy()
            
            if df.empty:
                print(f"Warning: Anomaly filter wiped out all data for {filename}. Skipping.")
                continue

        # 4. TIME BOUNDARIES: Apply start and stop limits
        if not df.empty and args.start_time > 0:
            df = df[df['Time'] >= args.start_time].copy()
            if df.empty:
                print(f"Warning: Start time filter (>= {args.start_time}s) wiped out all data for {filename}. Skipping.")
                continue

        if not df.empty and args.stop > 0:
            df = df[df['Time'] <= args.stop].copy()
            if df.empty:
                print(f"Warning: Stop time filter (<= {args.stop}s) wiped out all data for {filename}. Skipping.")
                continue

        # 5. DOWNSAMPLING: Take a sample every X seconds (if sample > 0)
        if not df.empty and args.sample > 0:
            df['time_bin'] = (df['Time'] // args.sample) * args.sample
            df = df.groupby('time_bin').first().reset_index(drop=True)

        if df.empty:
            print(f"Warning: No valid data left to plot for {filename}. Skipping.")
            continue

        # --- FORCE 1e8 SCALING ---
        df['Local Accesses'] = df['Local Accesses'] / 1e8
        df['Remote Accesses'] = df['Remote Accesses'] / 1e8

        # 6. Plotting Setup (ABSOLUTE FIXED SIZING)
        fig = plt.figure(figsize=(3, 3)) 
        
        # Hardcode the exact axes position [left, bottom, width, height] as fractions of the figure.
        # This guarantees the box size is identical across all plots mathematically.
        ax = fig.add_axes([0.22, 0.32, 0.72, 0.58]) 

        ax.plot(df['Time'], df['Local Accesses'], label='Local', color=LOCAL_COLOR, linewidth=1.5)
        ax.plot(df['Time'], df['Remote Accesses'], label='Remote', color=REMOTE_COLOR, linewidth=1.5)

        # --- DYNAMIC Y-AXIS LIMITS ---
        all_y_values = df['Local Accesses'].tolist() + df['Remote Accesses'].tolist()
        if all_y_values:
            y_max = max(df['Local Accesses'].quantile(0.995), df['Remote Accesses'].quantile(0.995))
            y_min = min(df['Local Accesses'].min(), df['Remote Accesses'].min())
            y_range = y_max - y_min if y_max > y_min else 1.0
            
            ax.set_ylim(max(0, y_min - (y_range * 0.1)), y_max + (y_range * 0.3))

        # 7. Graph Formatting
        ax.set_xlabel("Time (Seconds)", fontsize=11, fontweight='bold')
        ax.set_ylabel("Accesses ($10^8$)", fontsize=11, fontweight='bold')
        
        # REMOVED TITLE GENERATION HERE
        
        # Start and Stop the graph specifically at the requested limits
        ax.set_xlim(left=args.start_time)
        if args.stop > 0:
            ax.set_xlim(right=args.stop)
        
        ax.xaxis.set_tick_params(labelsize=10)
        ax.yaxis.set_tick_params(labelsize=10)
        
        # Force 1 decimal place on Y-axis so the text width is strictly consistent
        ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1f'))
        
        ax.grid(True, linestyle=':', alpha=0.7)
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.38), ncol=2, fontsize=11, frameon=False)
        
        # --- HARD BOX CREATION ---
        for spine in ax.spines.values():
            spine.set_visible(True)
            spine.set_linewidth(1.0)
            spine.set_edgecolor('black')
        
        # WE INTENTIONALLY REMOVED plt.tight_layout() HERE TO PREVENT DYNAMIC RESIZING

        # 8. Save the Figure
        if args.benchmark_workload.startswith("ycsb"):
            out_name = f"{args.benchmark_workload}_{th_config}_{ds_config}{file_suffix}{an_suffix}.png"
            out_pdf  = f"{args.benchmark_workload}_{th_config}_{ds_config}{file_suffix}{an_suffix}.pdf"
        else:
            out_name = f"perf_{args.benchmark_workload}_{th_config}_{ds_config}{file_suffix}{an_suffix}.png"
            out_pdf  = f"perf_{args.benchmark_workload}_{th_config}_{ds_config}{file_suffix}{an_suffix}.pdf"
            
        out_path_png = figs_dir / out_name
        out_path_pdf = figs_dir / out_pdf
        
        # WE INTENTIONALLY REMOVED bbox_inches='tight' SO THE PDF CANVAS IS EXACTLY 3x3
        plt.savefig(out_path_png, dpi=300)
        plt.savefig(out_path_pdf, format='pdf')
        print(f"Success! Plot saved to {out_path_png}")
        
        plt.close(fig)

if __name__ == "__main__":
    main()