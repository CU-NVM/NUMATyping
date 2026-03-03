#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from pathlib import Path
import sys

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
YCSB Result Plotting Tool
---------------------------
Generates line graphs from YCSB benchmark results.
Reads 'ycsb_experiments.csv' from the AN_on/ or AN_off/ directory
relative to this script's location. Automatically generates plots 
for both 40 and 80 threads.

USAGE:
    python3 plot_ycsb.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1]             AutoNUMA setting. 1 to read from AN_on/, 0 for AN_off/ (Required).
    --workload STR         Workload name to plot (e.g., A-50-50-50,D-100-0-50) (Required).

WORKFLOW EXAMPLE:
    python3 plot_ycsb.py --AN 1 --workload "A-50-50-50,D-100-0-50"
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    parser = argparse.ArgumentParser(description="Plot YCSB Benchmark Results", add_help=False)
    parser.add_argument('--AN', type=int, choices=[0, 1], required=True)
    parser.add_argument('--workload', type=str, required=True)
    return parser.parse_args()

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    try:
        args = parse_args()
    except Exception:
        show_help()

    # Setup directories
    script_dir = Path(__file__).resolve().parent
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    base_dir = script_dir / an_folder
    
    csv_path = base_dir / "ycsb_experiments.csv"
    figs_dir = base_dir / "figs"
    figs_dir.mkdir(parents=True, exist_ok=True)

    if not csv_path.exists():
        print(f"Error: Could not find results file at {csv_path}")
        sys.exit(1)

    # 1. Read CSV and strictly clean column names
    df = pd.read_csv(csv_path, skipinitialspace=True)
    df.columns = df.columns.str.strip()

    # 2. Force clean string columns and enforce numeric types
    for col in df.columns:
        if df[col].dtype == 'object':
            df[col] = df[col].astype(str).str.strip()

    # 3. Robust column extraction (resolves header misalignment)
    if 'duration' in df.columns:
        cols_after_duration = df.columns[df.columns.get_loc('duration')+1:]
        # Get the last non-NaN numeric value in the row
        df['real_total_ops'] = pd.to_numeric(df[cols_after_duration].ffill(axis=1).iloc[:, -1], errors='coerce')
    else:
        print("Error: 'duration' column not found in the CSV!")
        sys.exit(1)

    df['duration'] = pd.to_numeric(df['duration'], errors='coerce')
    df['num_threads'] = pd.to_numeric(df['num_threads'], errors='coerce')

    # Reconstruct workload string safely (ignoring empty pandas 'nan's)
    cols = list(df.columns)
    try:
        idx_buckets = cols.index('buckets')
        idx_duration = cols.index('duration')
        workload_cols = cols[idx_buckets + 1 : idx_duration]
        
        def clean_workload(row):
            parts = [str(val).strip() for val in row if str(val).strip() and str(val).strip().lower() != 'nan']
            return ','.join(parts)
            
        df['full_workload'] = df[workload_cols].apply(clean_workload, axis=1)
    except ValueError:
        print("Warning: Standard 'buckets' or 'duration' columns not found. Assuming 'workload' column exists.")
        df['full_workload'] = df['workload'].astype(str).str.strip()

    # 4. Loop through target threads (40 and 80)
    target_threads = [40, 80]
    
    for thread_count in target_threads:
        print(f"\n--- Processing data for {thread_count} threads ---")
        
        # Filter the Dataframe
        plot_df = df[(df['full_workload'] == args.workload) & (df['num_threads'] == thread_count)].copy()

        if plot_df.empty:
            print(f"No data found for workload '{args.workload}' with {thread_count} threads in {an_folder}. Skipping plot.")
            continue

        print(f"Found {len(plot_df)} total matching rows. Generating plot...")

        # 5. Plotting Setup
        fig, ax = plt.subplots(figsize=(10, 6))
        th_configs = plot_df['thread_config'].unique()
        
        # Use specific colors (Green and Blue first, with backups just in case)
        custom_colors = ['#2ca02c', '#1f77b4', '#ff7f0e', '#d62728'] 
        color_map = {th: custom_colors[i % len(custom_colors)] for i, th in enumerate(th_configs)}

        # Group and plot lines
        for (th_conf, ds_conf), group in plot_df.groupby(['thread_config', 'DS_config']):
            # Drop rows where duration or real_total_ops couldn't be converted to a number
            group = group.dropna(subset=['duration', 'real_total_ops']).sort_values('duration')
            
            print(f" -> Plotted TH: {th_conf} | DS: {ds_conf} with {len(group)} data points.")

            ds_str = str(ds_conf).lower()
            if 'numa' in ds_str: ls = '-'
            elif 'reg' in ds_str: ls = '--'
            else: ls = ':'

            # Divide by 1 million for throughput
            throughput_millions = group['real_total_ops'] / 1_000_000

            # Removed marker='o' to get smooth, dot-less lines
            ax.plot(group['duration'], throughput_millions, 
                    label=f"TH: {th_conf} | DS: {ds_conf}",
                    color=color_map[th_conf],
                    linestyle=ls,
                    linewidth=2.5) # Slightly thicker line looks better without dots

        # 6. Graph Formatting
        ax.set_xlabel("Duration (Seconds)", fontsize=12)
        ax.set_ylabel("Total Operations (Millions)", fontsize=12)
        ax.set_title(f"YCSB Performance: {args.workload} ({thread_count} Threads) - {an_folder}", fontsize=14)
        
        # Force X and Y to start at exactly 0
        ax.set_xlim(left=0)
        ax.set_ylim(bottom=0)
        
        # Split both axes into exactly 10 units
        if not plot_df['duration'].isna().all():
            ax.xaxis.set_major_locator(ticker.MaxNLocator(nbins=10))
            ax.yaxis.set_major_locator(ticker.MaxNLocator(nbins=10))
            
        ax.grid(True, linestyle=':', alpha=0.7)
        ax.legend(title="Configurations", bbox_to_anchor=(1.05, 1), loc='upper left')
        plt.tight_layout()

        # 7. Save the Figure and close
        safe_workload_name = args.workload.replace(',', '_')
        filename = f"{safe_workload_name}_{thread_count}.png"
        out_path = figs_dir / filename
        
        plt.savefig(out_path, dpi=300)
        print(f"Success! Plot saved to {out_path}")
        
        plt.close(fig)

if __name__ == "__main__":
    main()
