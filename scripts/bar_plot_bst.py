#!/usr/bin/env python3
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import sys

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
Data Structure Bar Plotting Tool (Normalized Throughput)
--------------------------------------------------------
Generates grouped bar charts comparing the normalized throughput of NUMA 
configurations relative to the 'regular-regular' baseline across DS experiments.
Reads files named '[ds_name]_[numDS]_[numKeys]_experiments.csv' from the AN_on/ or AN_off/ directory.

USAGE:
    python3 bar_plot_ds.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1]             AutoNUMA setting. 1 for AN_on/, 0 for AN_off/ (Required).
    --ds_names STR [STR]   List of data structures to plot. (Default: bst)
    --numDS STR            Number of data structure elements (Default: 1000000).
    --numKeys STR          Keyspace size (Default: 80).

WORKFLOW EXAMPLE:
    python3 bar_plot_ds.py --AN 1
    python3 bar_plot_ds.py --AN 0 --ds_names bst hashtrie skiplist --numDS 1000000 --numKeys 80
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    parser = argparse.ArgumentParser(description="Plot DS Bar Chart", add_help=False)
    parser.add_argument('--AN', type=int, choices=[0, 1], required=True)
    parser.add_argument('--ds_names', type=str, nargs='+', default=["bst"])
    parser.add_argument('--numDS', type=str, default="1000000")
    parser.add_argument('--numKeys', type=str, default="80")
    return parser.parse_args()

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    try:
        args = parse_args()
    except Exception:
        show_help()

    script_dir = Path(__file__).resolve().parent
    an_folder = "AN_on" if args.AN == 1 else "AN_off"
    base_dir = script_dir / an_folder
    figs_dir = base_dir / "figs"
    figs_dir.mkdir(parents=True, exist_ok=True)

    # 4 distinct configurations we are tracking
    configs = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']
    
    # Dictionary to hold raw average throughputs per config
    raw_data = {c: [] for c in configs}
    x_labels = []

    for ds in args.ds_names:
        x_labels.append(ds)

        # Build file path
        safe_ds = ds.replace(' ', '_').replace('/', '_')
        csv_filename = f"{safe_ds}_{args.numDS}_{args.numKeys}_experiments.csv"
        csv_path = base_dir / csv_filename

        if not csv_path.exists():
            print(f"Warning: File {csv_path} does not exist. Using 0 values for {ds}.")
            for c in configs:
                raw_data[c].append(0)
            continue

        # 1. Read CSV and strictly clean column names
        df = pd.read_csv(csv_path, skipinitialspace=True)
        df.columns = df.columns.str.strip()

        # 2. Force clean string columns
        for col in df.columns:
            if df[col].dtype == 'object':
                df[col] = df[col].astype(str).str.strip()

        # 3. Robust extraction for TotalOps
        if 'duration' in df.columns:
            cols_after_duration = df.columns[df.columns.get_loc('duration')+1:]
            df['real_total_ops'] = pd.to_numeric(df[cols_after_duration].ffill(axis=1).iloc[:, -1], errors='coerce')
        else:
            print(f"Error: 'duration' column not found in {csv_path}!")
            for c in configs: raw_data[c].append(0)
            continue

        df['duration'] = pd.to_numeric(df['duration'], errors='coerce')
        df['num_threads'] = pd.to_numeric(df['num_threads'], errors='coerce')

        # Filter strictly for num_threads == 80
        df_80 = df[df['num_threads'] == 80].copy()

        # Iterate over the 4 configs
        for c in configs:
            th_conf, ds_conf = c.split('-')
            group = df_80[(df_80['thread_config'] == th_conf) & (df_80['DS_config'] == ds_conf)].copy()

            if len(group) < 2:
                raw_data[c].append(0)
                continue

            # Ensure sorted by duration to compute valid sequential differences
            group = group.dropna(subset=['duration', 'real_total_ops']).sort_values('duration')
            
            # Find the difference between intervals to calculate operations per interval
            diffs = group['real_total_ops'].diff()
            
            # First interval's throughput is just its raw value since it starts from 0
            diffs.iloc[0] = group['real_total_ops'].iloc[0]
            
            # Find average and normalize by 1,000,000
            avg_diff = diffs.mean() / 1_000_000
            raw_data[c].append(avg_diff)

    # ============================================================================
    # Normalization Step (Convert raw throughput to Normalized Throughput)
    # ============================================================================
    baseline_config = 'regular-regular'
    normalized_data = {c: [] for c in configs}
    
    for i in range(len(x_labels)):
        baseline_val = raw_data[baseline_config][i]
        
        for c in configs:
            if baseline_val == 0:
                normalized_data[c].append(0) # Avoid divide-by-zero if file is missing
            else:
                norm_throughput = raw_data[c][i] / baseline_val
                normalized_data[c].append(norm_throughput)

    data = normalized_data

    # ============================================================================
    # Plotting Logic
    # ============================================================================
    x = np.arange(len(x_labels))
    width = 0.2

    # Adjust width dynamically based on how many DS are being plotted
    fig_width = max(8, len(x_labels) * 2.5)
    fig, ax = plt.subplots(figsize=(fig_width, 6))
    
    # Pre-defined nice colors
    custom_colors = ["#133bcb", "#f9d405", "#5ab057", "#5E5959"]

    for i, config in enumerate(configs):
        offset = (i - 1.5) * width
        ax.bar(x + offset, data[config], width, label=config, color=custom_colors[i], edgecolor='black')

    # Add a horizontal dashed line at y=1.0 to clearly show the baseline
    ax.axhline(y=1.0, color='black', linestyle='--', linewidth=1, alpha=0.6, label='Baseline (1.0x)')

    # Formatting
    ax.set_xlabel("Data Structures", fontsize=12, fontweight='bold')
    ax.set_ylabel("Normalized Throughput (vs regular-regular)", fontsize=12, fontweight='bold')
    ax.set_title(f"Data Structure Normalized Throughput (80 Threads) - {an_folder}", fontsize=14)
    ax.set_xticks(x)
    
    # Capitalize the data structure names for cleaner axis labels
    ax.set_xticklabels([label.capitalize() for label in x_labels], fontsize=12)
    
    # Move the legend outside to keep the bars from being covered
    ax.legend(title="Configurations", bbox_to_anchor=(1.05, 1), loc='upper left')
    ax.grid(axis='y', linestyle=':', alpha=0.7)
    
    # Remove top and right spines for a cleaner aesthetic
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()

    # Create dynamic filename
    joined_labels = "_".join(x_labels)
    out_name = f"bar_plot_ds_{joined_labels}_{args.numDS}_{args.numKeys}_80.png"
    out_path = figs_dir / out_name
    
    plt.savefig(out_path, dpi=300)
    print(f"\nSuccess! Bar chart saved to {out_path}")

if __name__ == "__main__":
    main()