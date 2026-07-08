#!/usr/bin/env python3
import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.lines import Line2D
import matplotlib.colors as mcolors
from pathlib import Path
import sys
import os

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
YCSB Bar Plotting Tool (Normalized Cumulative Throughput)
---------------------------------------------------------
Generates grouped bar charts comparing the normalized cumulative throughput of 
NUMA configurations relative to the 'regular-regular' baseline across workloads.
Uses the true average (Total Ops / Total Time) right before the last interval.

USAGE:
    python3 bar_plot_ycsb.py [OPTIONS]

CORE OPTIONS:
    --AN [0|1|both]        AutoNUMA setting. 1 for AN_on/, 0 for AN_off/, 
                           or 'both' to place them side-by-side (Required).
    --workloads STR [STR]  List of workloads to plot. 
                           (Default: AD, A, B, C, D, E, F configurations)
    --perlmutter           Read from/save to 'Perlmutter/' directory, use 128 threads, 
                           and look for files ending in '_perl.csv'.
    --short                Read from files ending with '_short.csv' and append 
                           '_short' to output figure names.
    --ROOT_DIR PATH        Path to NUMATyping root (Default: ~/NUMATyping).
    --stop INT             Stop sampling at this duration in seconds (Default: 0 / all points).

WORKFLOW EXAMPLE:
    python3 bar_plot_ycsb.py --AN 1
    python3 bar_plot_ycsb.py --AN both --perlmutter --short --stop 3600
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Data Extraction Function
# ============================================================================

def get_data(an_folder, base_dir, target_threads, workloads, is_perlmutter, stop_time, is_short, is_zipfian=False, baseline_config='regular-regular', suffix=''):
    configs = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']
    raw_data = {c: [] for c in configs}
    x_labels = []

    for orig_wl in workloads:
        wl = orig_wl
        # Clean off any extensions if user passed the literal filename
        if wl.startswith("ycsb_"): wl = wl[5:]
        if wl.endswith("_perl_short.csv"): wl = wl[:-15]
        elif wl.endswith("_short.csv"): wl = wl[:-10]
        elif wl.endswith("_perl.csv"): wl = wl[:-9]
        elif wl.endswith(".csv"): wl = wl[:-4]

        blocks = wl.replace('_', ',').split(',')
        letters = [b.split('-')[0] for b in blocks if '-' in b]
        unique_letters = []
        for l in letters:
            if l not in unique_letters: unique_letters.append(l)
            
        label = "".join(unique_letters)
        if not label: label = wl 
        x_labels.append(label)

        safe_wl = wl.replace(',', '_')
        
        # Build the proper file suffix based on flags
        file_suffix = "_perl" if is_perlmutter else ""
        if is_short:
            file_suffix += "_short"
        if is_zipfian:
            file_suffix += "_zipfian"
        file_suffix += suffix
        file_suffix += ".csv"
        
        csv_path = base_dir / an_folder / f"ycsb_{safe_wl}{file_suffix}"

        if not csv_path.exists() or csv_path.stat().st_size == 0:
            print(f"Warning: File {csv_path} does not exist or is empty. Using 0 values.")
            for c in configs: raw_data[c].append(0)
            continue

        try:
            df = pd.read_csv(csv_path, skipinitialspace=True)
        except pd.errors.EmptyDataError:
            print(f"Warning: File {csv_path} is empty. Using 0 values.")
            for c in configs: raw_data[c].append(0)
            continue
            
        df.columns = df.columns.str.strip().str.lower()  # handle Capitalized headers from manual-run CSVs
        for col in df.columns:
            if df[col].dtype == 'object':
                df[col] = df[col].astype(str).str.strip()

        if 'duration' in df.columns:
            cols_after_duration = df.columns[df.columns.get_loc('duration')+1:]
            df['real_total_ops'] = pd.to_numeric(df[cols_after_duration].ffill(axis=1).iloc[:, -1], errors='coerce')
        else:
            print(f"Error: 'duration' column not found in {csv_path}!")
            for c in configs: raw_data[c].append(0)
            continue

        df['duration'] = pd.to_numeric(df['duration'], errors='coerce')
        df['num_threads'] = pd.to_numeric(df['num_threads'], errors='coerce')

        df_filtered = df[df['num_threads'] == target_threads].copy()

        for c in configs:
            th_conf, ds_conf = c.split('-')
            group = df_filtered[(df_filtered['thread_config'] == th_conf) & (df_filtered['ds_config'] == ds_conf)].copy()

            # Ensure sorted by duration
            group = group.dropna(subset=['duration', 'real_total_ops']).sort_values('duration')
            
            # --- CAP END SECONDS ---
            if stop_time > 0:
                group = group[group['duration'] <= stop_time]

            if len(group) < 2:
                raw_data[c].append(0)
                continue
            
            # --- CUMULATIVE THROUGHPUT CALCULATION ---
            # Grab the second-to-last row to avoid any partial/truncated data at the very end
            target_row = group.iloc[-2] if len(group) > 1 else group.iloc[-1]
            
            cum_ops = target_row['real_total_ops']
            cum_time = target_row['duration']
            
            if cum_time > 0:
                # Dynamically find the interval length (usually 10s)
                interval_sec = group['duration'].diff().median()
                if pd.isna(interval_sec) or interval_sec <= 0:
                    interval_sec = 10 # Fallback
                
                # Calculate true operations per second
                ops_per_sec = cum_ops / cum_time
                
                # Scale back up to the interval size so the Y-axis scale matches your previous charts
                ops_per_interval = ops_per_sec * interval_sec
                raw_data[c].append(ops_per_interval / 1_000_000)
            else:
                raw_data[c].append(0)

    # Normalization Step (baseline_config passed in; default regular-regular)
    normalized_data = {c: [] for c in configs}
    
    for i in range(len(x_labels)):
        baseline_val = raw_data[baseline_config][i]
        for c in configs:
            if baseline_val == 0:
                normalized_data[c].append(0) 
            else:
                normalized_data[c].append(raw_data[c][i] / baseline_val)
                
    # --- Always Calculate Geometric Mean ---
    for c in configs:
        valid_vals = [v for v in normalized_data[c] if v > 0]
        if valid_vals:
            gmean = np.exp(np.mean(np.log(valid_vals)))
        else:
            gmean = 0
        normalized_data[c].append(gmean)
        
    x_labels.append("Geomean")
                
    return normalized_data, x_labels

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    default_workloads = [
        "A-50-50-50_D-100-0-50", "A-50-50-50_A-100-0-50",
        "B-50-50-50_B-100-0-50", "C-50-50-50_C-100-0-50",
        "D-50-50-50_D-100-0-50", "E-50-50-50_E-100-0-50",
        "F-50-50-50_F-100-0-50"
    ]
    parser = argparse.ArgumentParser(description="Plot YCSB Bar Chart", add_help=False)
    parser.add_argument('--AN', type=str, choices=['0', '1', 'both'], required=True)
    parser.add_argument('--workloads', type=str, nargs='+', default=default_workloads)
    parser.add_argument('--perlmutter', action='store_true')
    parser.add_argument('--short', action='store_true')
    parser.add_argument('--zipfian', action='store_true', help="Read ycsb_<wl>_zipfian.csv files and prefix output with 'zipfian_'.")
    parser.add_argument('--baseline', type=str, default='regular-regular',
                        choices=['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular'],
                        help="Config to normalize against (default: regular-regular).")
    parser.add_argument('--ROOT_DIR', type=str, default=os.path.expanduser("~/NUMATyping"))
    parser.add_argument('--stop', type=int, default=0, help="Stop sampling at this many seconds (0 = end of data)")
    parser.add_argument('--suffix', type=str, default='', help="Extra filename suffix before .csv, e.g. _uniform_0")
    return parser.parse_args()

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    try:
        args = parse_args()
    except Exception:
        show_help()

    root_dir = Path(args.ROOT_DIR).resolve()
    target_threads = 128 if args.perlmutter else 80
    
    # Rigidly define the base path using the standard NUMATyping hierarchy
    if args.perlmutter:
        base_dir = root_dir / "Graphs" / "Perlmutter"
    else:
        base_dir = root_dir / "Graphs"

    configs = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']
    custom_colors = ["#133bcb", "#f9d405", "#5ab057", "#5E5959"]

    # --- Fetch Data ---
    if args.AN == 'both':
        data_on, x_labels = get_data("AN_on", base_dir, target_threads, args.workloads, args.perlmutter, args.stop, args.short, args.zipfian, args.baseline, args.suffix)
        data_off, _ = get_data("AN_off", base_dir, target_threads, args.workloads, args.perlmutter, args.stop, args.short, args.zipfian, args.baseline, args.suffix)
        an_folder_name = "AN_both"
    else:
        an_folder = "AN_on" if args.AN == '1' else "AN_off"
        data, x_labels = get_data(an_folder, base_dir, target_threads, args.workloads, args.perlmutter, args.stop, args.short, args.zipfian, args.baseline, args.suffix)
        an_folder_name = an_folder

    # --- Plotting ---
    # Fixes the faded diagonal lines issue in PDF exports
    plt.rcParams['hatch.linewidth'] = 1.5 
    
    x = np.arange(len(x_labels))
    width = 0.1 if args.AN == 'both' else 0.2
    
    # Custom display map for the legends
    legend_map = {
        'numa-numa': 'numa th. / numa ds.',
        'numa-regular': 'numa th. / regular ds.',
        'regular-numa': 'regular th. / numa ds.',
        'regular-regular': 'regular th. / regular ds.'
    }
    
    # We always have Geomean now, so keep it consistently wide
    fig, ax = plt.subplots(figsize=(13.5, 5.5))

    if args.AN == 'both':
        for i, config in enumerate(configs):
            offset_on = (2 * i - 3.5) * width
            offset_off = (2 * i - 2.5) * width
            
            # Map the config to the readable legend string
            ax.bar(x + offset_on, data_on[config], width, label=legend_map[config], 
                   color=custom_colors[i], edgecolor='black', zorder=2)
            
            faded_bg = mcolors.to_rgba(custom_colors[i], alpha=0.4)
            ax.bar(x + offset_off, data_off[config], width, 
                   facecolor=faded_bg, edgecolor='black', 
                   hatch='////', linewidth=1.2, zorder=2)
            
    else:
        for i, config in enumerate(configs):
            offset = (i - 1.5) * width
            # Map the config to the readable legend string
            ax.bar(x + offset, data[config], width, label=legend_map[config], 
                   color=custom_colors[i], edgecolor='black', zorder=2)
            
    ax.axhline(y=1.0, color='black', linestyle='--', linewidth=1, alpha=0.6, label='Baseline (1.0x)', zorder=1)

    # --- Geomean Annotations & Separation ---
    sep_x = len(x_labels) - 1.5
    ax.axvline(x=sep_x, color='gray', linestyle='-.', linewidth=1.2, alpha=0.8, zorder=1)

    gm_idx = len(x_labels) - 1
    gm_x = x[-1]
    
    # Now annotating ALL four configurations above their Geomean bar
    configs_to_annotate = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']

    if args.AN == 'both':
        for i, config in enumerate(configs):
            if config not in configs_to_annotate:
                continue
            
            offset_on = (2 * i - 3.5) * width
            val_on = data_on[config][gm_idx]
            if val_on > 0:
                ax.text(gm_x + offset_on, val_on + 0.05, f"{val_on:.2f}x", 
                        ha='center', va='bottom', fontsize=10, fontweight='bold', rotation=90)
                
            offset_off = (2 * i - 2.5) * width
            val_off = data_off[config][gm_idx]
            if val_off > 0:
                ax.text(gm_x + offset_off, val_off + 0.05, f"{val_off:.2f}x", 
                        ha='center', va='bottom', fontsize=10, fontweight='bold', rotation=90)
    else:
        for i, config in enumerate(configs):
            if config not in configs_to_annotate:
                continue
                
            offset = (i - 1.5) * width
            val = data[config][gm_idx]
            if val > 0:
                ax.text(gm_x + offset, val + 0.05, f"{val:.2f}x", 
                        ha='center', va='bottom', fontsize=10, fontweight='bold', rotation=90)

    # --- Legend Management ---
    handles, labels = ax.get_legend_handles_labels()
    
    if args.AN == 'both':
        blank_line = Line2D([0], [0], color='none', linestyle='none', marker='none')
        handles.append(blank_line)
        labels.append('AutoNUMA ON (Solid)')
        
        faded_gray_bg = mcolors.to_rgba('gray', alpha=0.4)
        handles.append(mpatches.Patch(facecolor=faded_gray_bg, edgecolor='black', hatch='////'))
        labels.append('AutoNUMA OFF (Striped)')
        
    # Bumped height from 1.35 to 1.45 to make room for the stacked legend rows
    ax.set_ylim(0, ax.get_ylim()[1] * 1.45)
    
    # Reduced ncol to 2 (or 3 for 'both') to prevent the long labels from crossing the Y-axis
    num_cols = 3 if args.AN == 'both' else 2
    ax.legend(handles=handles, labels=labels, loc='upper center', ncol=num_cols, frameon=False, fontsize=13)

    # --- Formatting ---
    ax.set_xlabel("Workload Configs", fontsize=12, fontweight='bold')
    
    # Update label depending on if data was truncated
    ylab = "Normalized Cumulative Throughput" 
    if args.stop > 0:
        ylab += f" (Up to {args.stop}s)"
    ax.set_ylabel(ylab, fontsize=12, fontweight='bold')
    
    ax.set_xticks(x)
    labels_obj = ax.set_xticklabels(x_labels, fontsize=12)
    
    # Bold the Geomean label to make it stand out
    labels_obj[-1].set_fontweight('bold')
    
    ax.grid(axis='y', linestyle=':', alpha=0.7, zorder=0)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()

    # --- Save Logic ---
    joined_labels = "_".join(x_labels[:-1]) # Don't include "Geomean" in the filename
    
    short_str = "_short" if args.short else ""
    zipfian_prefix = "zipfian_" if args.zipfian else ""
    base_suffix = "" if args.baseline == 'regular-regular' else f"_norm-{args.baseline}"
    base_out_name = f"{zipfian_prefix}bar_plot_normalized_{joined_labels}_{target_threads}_{an_folder_name}{short_str}{base_suffix}"
    
    if args.AN == 'both':
        target_dirs = [base_dir / "AN_both" / "figs"]
    else:
        target_dirs = [base_dir / an_folder_name / "figs"]

    print("\nSaving plots...")
    for t_dir in target_dirs:
        t_dir.mkdir(parents=True, exist_ok=True)
        
        out_path_png = t_dir / f"{base_out_name}.png"
        out_path_pdf = t_dir / f"{base_out_name}.pdf"
        
        plt.savefig(out_path_png, dpi=300)
        print(f" -> Saved PNG to: {out_path_png}")
        
        plt.savefig(out_path_pdf, format='pdf', bbox_inches='tight')
        print(f" -> Saved PDF to: {out_path_pdf}")

if __name__ == "__main__":
    main()