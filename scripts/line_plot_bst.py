#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib.lines import Line2D
from pathlib import Path
import sys
import os

# ============================================================================
# Data Extraction Function
# ============================================================================

def get_data(an_folder, root_dir, target_threads, args):
    if args.perlmutter:
        base_dir = root_dir / "Graphs" / "Perlmutter" / an_folder
        if not base_dir.exists():
            base_dir = root_dir / "Perlmutter" / an_folder
    else:
        base_dir = root_dir / "Graphs" / an_folder

    # Build the proper file suffix based on flags
    file_suffix = "_perl" if args.perlmutter else ""
    if args.short:
        file_suffix += "_short"
    file_suffix += ".csv"

    csv_filename = f"{args.ds_name}_{args.numDS}_{args.numKeys}_experiments{file_suffix}"
    csv_path = base_dir / csv_filename
    
    data_dict = {}
    configs = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']
    
    if not csv_path.exists() or csv_path.stat().st_size == 0:
        print(f"Warning: File not found at {csv_path}")
        return data_dict

    df = pd.read_csv(csv_path, skipinitialspace=True)
    df.columns = df.columns.str.strip()
    
    rename_map = {col: 'DS_config' for col in df.columns if col.lower() in ['ds_config', 'ds config']}
    rename_map.update({col: 'thread_config' for col in df.columns if col.lower() in ['th_config', 'thread_config']})
    df.rename(columns=rename_map, inplace=True)
        
    df['duration'] = pd.to_numeric(df['duration'], errors='coerce')
    df['num_threads'] = pd.to_numeric(df['num_threads'], errors='coerce')

    if 'duration' in df.columns:
        cols_after_duration = df.columns[df.columns.get_loc('duration')+1:]
        df['real_total_ops'] = pd.to_numeric(df[cols_after_duration].ffill(axis=1).iloc[:, -1], errors='coerce')
    
    plot_df = df[df['num_threads'] == target_threads].copy()

    for c in configs:
        th_conf, ds_conf = c.split('-')
        group = plot_df[(plot_df['thread_config'] == th_conf) & (plot_df['DS_config'] == ds_conf)].copy()
        if group.empty: continue
            
        group = group.dropna(subset=['duration', 'real_total_ops']).sort_values('duration')

        # --- CALCULATE THROUGHPUT BEFORE CUTTING DATA ---
        # This ensures the first point after the cut diffs correctly against its preceding hidden interval
        diffs = group['real_total_ops'].diff()
        diffs.iloc[0] = group['real_total_ops'].iloc[0]
        group['throughput_millions'] = diffs / 1_000_000

        # --- CUT INITIAL SECONDS ---
        if args.start > 0:
            group = group[group['duration'] >= args.start]

        # --- CAP END SECONDS ---
        if args.stop > 0:
            group = group[group['duration'] <= args.stop]

        # --- EXPLICIT CONDITIONAL SAMPLING ---
        if args.sample > 0:
            group = group[(group['duration'].round() > 0) & (group['duration'].round() % args.sample == 0)]
        
        if group.empty: continue
        data_dict[c] = group

    return data_dict

# ============================================================================
# Main Logic
# ============================================================================

def parse_args():
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--AN', type=str, choices=['0', '1', 'both'], required=True)
    parser.add_argument('--ds_name', type=str, default="bst")
    parser.add_argument('--numDS', type=str, default="1000000")
    parser.add_argument('--numKeys', type=str, default="80")
    parser.add_argument('--perlmutter', action='store_true')
    parser.add_argument('--short', action='store_true', help="Read from '_short' files and append '_short' to outputs")
    parser.add_argument('--sample', type=int, default=0, help="Downsample data to this interval in seconds")
    parser.add_argument('--start', type=int, default=300, help="Skip the first X seconds of data (Default: 300)")
    parser.add_argument('--stop', type=int, default=0, help="Stop graphing at this many seconds (0 = end of data)")
    parser.add_argument('--ROOT_DIR', type=str, default=os.path.expanduser("~/NUMATyping"))
    return parser.parse_args()

def main():
    args = parse_args()

    # Perlmutter specific defaults
    if args.perlmutter:
        if '--numDS' not in sys.argv:
            args.numDS = "3000000"
        if '--numKeys' not in sys.argv:
            args.numKeys = "240"

    root_dir = Path(args.ROOT_DIR).resolve()
    target_threads = 128 if args.perlmutter else 80
    custom_colors = ["#133bcb", "#f9d405", "#5ab057", "#5E5959"]
    configs = ['numa-numa', 'numa-regular', 'regular-numa', 'regular-regular']

    if args.AN == 'both':
        an_folder_name = "AN_both"
        data_on = get_data("AN_on", root_dir, target_threads, args)
        data_off = get_data("AN_off", root_dir, target_threads, args)
    else:
        an_folder_name = "AN_on" if args.AN == '1' else "AN_off"
        data = get_data(an_folder_name, root_dir, target_threads, args)

    # --- ADJUSTED VERTICAL HEIGHT ---
    fig, ax = plt.subplots(figsize=(10, 4.5))
    all_y = []

    # Slightly increased markersize to match the bigger legend fonts
    marker_style = {'marker': 'o', 'markersize': 5, 'markerfacecolor': 'white', 'markevery': 0.05}

    if args.AN == 'both':
        for i, c in enumerate(configs):
            if 'data_off' in locals() and c in data_off:
                ax.plot(data_off[c]['duration'], data_off[c]['throughput_millions'], 
                        color=custom_colors[i], linestyle='--', linewidth=1.5, **marker_style)
                all_y.extend(data_off[c]['throughput_millions'].dropna().tolist())
            if 'data_on' in locals() and c in data_on:
                ax.plot(data_on[c]['duration'], data_on[c]['throughput_millions'], 
                        color=custom_colors[i], linestyle='-', linewidth=2)
                all_y.extend(data_on[c]['throughput_millions'].dropna().tolist())
    else:
        for i, c in enumerate(configs):
            if c in data:
                line_s = '--' if args.AN == '0' else '-'
                ax.plot(data[c]['duration'], data[c]['throughput_millions'], 
                        color=custom_colors[i], linestyle=line_s, 
                        **(marker_style if args.AN=='0' else {'linewidth': 2}))
                all_y.extend(data[c]['throughput_millions'].dropna().tolist())

    # --- ADVANCED SCALING ---
    if all_y:
        y_min, y_max = min(all_y), max(all_y)
        y_range = y_max - y_min
        if y_range == 0:
            ax.set_ylim(y_min * 0.9, y_max * 1.5)
        else:
            # Provide 55% overhead space so the stacked legend has plenty of vertical clearance
            ax.set_ylim(y_min - (y_range * 0.05), y_max + (y_range * 0.55))
    
    # Start the X-axis at the cut time
    ax.set_xlim(left=args.start)
    
    # Cap the X-axis right side if --stop was explicitly passed
    if args.stop > 0:
        ax.set_xlim(right=args.stop)
    
    ax.set_xlabel("Time (Seconds)", fontsize=14, fontweight='bold')
    # Label reflects the explicit sampling method if requested
    y_label = f"Ops per {args.sample}s (Millions)" if args.sample > 0 else "Throughput (Millions of Ops)"
    ax.set_ylabel(y_label, fontsize=14, fontweight='bold')

    # --- BIGGER LEGEND ---
    # Custom display map for the legends
    legend_map = {
        'numa-numa': 'numa th. / numa ds.',
        'numa-regular': 'numa th. / regular ds.',
        'regular-numa': 'regular th. / numa ds.',
        'regular-regular': 'regular th. / regular ds.'
    }

    handles = [Line2D([0], [0], color=custom_colors[i], lw=2) for i in range(len(configs))]
    # Apply the mapping to the labels
    labels = [legend_map[c] for c in configs]
    
    if args.AN == 'both':
        handles.extend([
            Line2D([0], [0], color='black', linestyle='-', lw=2),
            Line2D([0], [0], color='black', linestyle='--', marker='o', markersize=5, markerfacecolor='white')
        ])
        labels.extend(['AutoNUMA ON', 'AutoNUMA OFF'])

    # Fontsize set to 13 to balance readability with physical space limits
    # Columns dynamically switch between 2 and 3 based on how many items are being plotted
    num_cols = 3 if args.AN == 'both' else 2
    ax.legend(handles=handles, labels=labels, loc='upper center', 
              ncol=num_cols, frameon=False, fontsize=13, columnspacing=1.5)
    
    ax.grid(True, linestyle=':', alpha=0.6)
    
    # Clean up right/top spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()

    # Directory/Save Logic
    path_base = root_dir / "Graphs"
    if args.perlmutter:
        path_base = path_base / "Perlmutter"
    
    target_dir = path_base / an_folder_name / "figs"
    target_dir.mkdir(parents=True, exist_ok=True)
    
    # Generate the base name based on the presence of --short
    short_str = "_short" if args.short else ""
    
    if args.AN == 'both':
        base_out_name = f"{args.ds_name}_{args.numDS}_{args.numKeys}_AN_Both_Result{short_str}"
    else:
        base_out_name = f"{args.ds_name}_{args.numDS}_{args.numKeys}_Result{short_str}"
        
    save_path_png = target_dir / f"{base_out_name}.png"
    save_path_pdf = target_dir / f"{base_out_name}.pdf"
    
    plt.savefig(save_path_png, dpi=300)
    plt.savefig(save_path_pdf, format='pdf', bbox_inches='tight')
    
    print(f"--- Processing Done ---")
    print(f"Skipped Initial: {args.start}s")
    if args.stop > 0:
        print(f"Stopped At: {args.stop}s")
    print(f"Sampling: {f'Every {args.sample}s' if args.sample > 0 else 'Disabled (Graphing all points)'}")
    print(f"Saved PNG to: {save_path_png}")
    print(f"Saved PDF to: {save_path_pdf}")
    plt.close(fig)

if __name__ == "__main__":
    main()