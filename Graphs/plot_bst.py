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
Benchmark Result Plotting Tool
---------------------------
Generates line graphs from benchmark results.
Reads '[ds_name]_[numDS]_[numKeys]_experiments.csv' from the AN_on/ or AN_off/ 
directory relative to this script's location. Automatically generates plots 
for both 40 and 80 threads.

USAGE:
    python3 plot_benchmark.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1]             AutoNUMA setting. 1 to read from AN_on/, 0 for AN_off/ (Required).
    --ds_name STR          Data structure name to plot (e.g., HashTrie, Skiplist) (Required).
    --numDS INT            Number of data structure elements used in the experiment (Required).
    --numKeys INT          Keyspace size used in the experiment (Required).

WORKFLOW EXAMPLE:
    python3 plot_benchmark.py --AN 1 --ds_name "bst" --numDS 1000000 --numKeys 80
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    parser = argparse.ArgumentParser(description="Plot Benchmark Results", add_help=False)
    parser.add_argument('--AN', type=int, choices=[0, 1], required=True)
    parser.add_argument('--ds_name', type=str, required=True)
    parser.add_argument('--numDS', type=str, required=True)
    parser.add_argument('--numKeys', type=str, required=True)
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
    
    # Dynamically format the CSV filename based on user inputs
    safe_ds_name = args.ds_name.replace(' ', '_').replace('/', '_')
    csv_filename = f"{safe_ds_name}_{args.numDS}_{args.numKeys}_experiments.csv"
    csv_path = base_dir / csv_filename 
    
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

    # 3. Robust column extraction (resolves any header misalignment)
    if 'duration' in df.columns:
        cols_after_duration = df.columns[df.columns.get_loc('duration')+1:]
        # Get the last non-NaN numeric value in the row (which should be TotalOps)
        df['real_total_ops'] = pd.to_numeric(df[cols_after_duration].ffill(axis=1).iloc[:, -1], errors='coerce')
    else:
        print("Error: 'duration' column not found in the CSV!")
        sys.exit(1)

    df['duration'] = pd.to_numeric(df['duration'], errors='coerce')
    df['num_threads'] = pd.to_numeric(df['num_threads'], errors='coerce')

    # 4. Loop through target threads (40 and 80)
    target_threads = [40, 80]
    
    for thread_count in target_threads:
        print(f"\n--- Processing data for {thread_count} threads ---")
        
        # Filter the Dataframe by DS_name and thread count
        plot_df = df[(df['DS_name'] == args.ds_name) & (df['num_threads'] == thread_count)].copy()

        if plot_df.empty:
            print(f"No data found for DS_name '{args.ds_name}' with {thread_count} threads in {an_folder}. Skipping plot.")
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

            # Plot smooth, dot-less lines
            ax.plot(group['duration'], throughput_millions, 
                    label=f"TH: {th_conf} | DS: {ds_conf}",
                    color=color_map[th_conf],
                    linestyle=ls,
                    linewidth=2.5) 

        # 6. Graph Formatting
        ax.set_xlabel("Duration (Seconds)", fontsize=12)
        ax.set_ylabel("Total Operations (Millions)", fontsize=12)
        ax.set_title(f"Performance: {args.ds_name} N:{args.numDS} K:{args.numKeys} ({thread_count} Threads) - {an_folder}", fontsize=14)
        
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
        # Include numDS and numKeys in the figure filename so plots aren't overwritten
        filename = f"{safe_ds_name}_{args.numDS}_{args.numKeys}_{thread_count}.png"
        out_path = figs_dir / filename
        
        plt.savefig(out_path, dpi=300)
        print(f"Success! Plot saved to {out_path}")
        
        plt.close(fig)

if __name__ == "__main__":
    main()
