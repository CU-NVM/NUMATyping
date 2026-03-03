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
YCSB Bar Plotting Tool (Normalized Throughput)
----------------------------------------------
Generates grouped bar charts comparing the normalized throughput of NUMA 
configurations relative to the 'regular-regular' baseline across YCSB workloads.
Reads files named 'ycsb_[workload].csv' from the AN_on/ or AN_off/ directory.

USAGE:
    python3 bar_plot_ycsb.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1]             AutoNUMA setting. 1 for AN_on/, 0 for AN_off/ (Required).
    --workloads STR [STR]  List of workloads to plot. 
                           (Default: AD, A, B, C, D, E, F configurations)

WORKFLOW EXAMPLE:
    python3 bar_plot_ycsb.py --AN 1
    python3 bar_plot_ycsb.py --AN 0 --workloads A-50-50-50_D-100-0-50 B-50-50-50_B-100-0-50
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    # Define the default workloads exactly as specified
    default_workloads = [
        "A-50-50-50_D-100-0-50",
        "A-50-50-50_A-100-0-50",
        "B-50-50-50_B-100-0-50",
        "C-50-50-50_C-100-0-50",
        "D-50-50-50_D-100-0-50",
        "E-50-50-50_E-100-0-50",
        "F-50-50-50_F-100-0-50"
    ]
    
    parser = argparse.ArgumentParser(description="Plot YCSB Bar Chart", add_help=False)
    parser.add_argument('--AN', type=int, choices=[0, 1], required=True)
    parser.add_argument('--workloads', type=str, nargs='+', default=default_workloads)
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

    for orig_wl in args.workloads:
        wl = orig_wl
        # Safeguard: if the user accidentally passed the exact filename with ycsb_ or .csv
        if wl.startswith("ycsb_"): wl = wl[5:]
        if wl.endswith(".csv"): wl = wl[:-4]

        # Extract X-axis label (e.g., AD, A, B, C...) from the workload blocks
        blocks = wl.replace('_', ',').split(',')
        letters = [b.split('-')[0] for b in blocks if '-' in b]
        unique_letters = []
        for l in letters:
            if l not in unique_letters: unique_letters.append(l)
            
        label = "".join(unique_letters)
        if not label: label = wl # Fallback just in case
        x_labels.append(label)

        # Build file path
        safe_wl = wl.replace(',', '_')
        csv_filename = f"ycsb_{safe_wl}.csv"
        csv_path = base_dir / csv_filename

        if not csv_path.exists():
            print(f"Warning: File {csv_path} does not exist. Using 0 values for {label}.")
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

        # 3. Robust extraction for YCSB TotalOps
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
            
            # Find the difference between intervals to calculate raw throughput
            diffs = group['real_total_ops'].diff().dropna()
            
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

    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Pre-defined nice colors matching earlier line graphs
    custom_colors = ["#133bcb", "#f9d405", "#5ab057", "#5E5959"]

    for i, config in enumerate(configs):
        offset = (i - 1.5) * width
        ax.bar(x + offset, data[config], width, label=config, color=custom_colors[i], edgecolor='black')

    # Add a horizontal dashed line at y=1.0 to clearly show the baseline
    ax.axhline(y=1.0, color='black', linestyle='--', linewidth=1, alpha=0.6, label='Baseline (1.0x)')

    # Formatting
    ax.set_xlabel("Workload Configs", fontsize=12, fontweight='bold')
    ax.set_ylabel("Normalized Throughput (vs regular-regular)", fontsize=12, fontweight='bold')
    ax.set_title(f"YCSB Workload Normalized Throughput (80 Threads) - {an_folder}", fontsize=14)
    ax.set_xticks(x)
    ax.set_xticklabels(x_labels, fontsize=12)
    
    # Move the legend outside to keep the bars from being covered
    ax.legend(title="Configurations", bbox_to_anchor=(1.05, 1), loc='upper left')
    ax.grid(axis='y', linestyle=':', alpha=0.7)
    
    # Remove top and right spines for a cleaner aesthetic
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()

    # Create dynamic filename based on the workloads parsed
    joined_labels = "_".join(x_labels)
    out_name = f"bar_plot_normalized_{joined_labels}_80.png"
    out_path = figs_dir / out_name
    
    plt.savefig(out_path, dpi=300)
    print(f"\nSuccess! Bar chart saved to {out_path}")

if __name__ == "__main__":
    main()
