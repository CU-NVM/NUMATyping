#!/usr/bin/env python3
# plot_histogram.py  (X-axis = bucket_count; bars = min/max num_threads)
# Usage:
#   python plot_histogram.py your_data.csv --show
#   python plot_histogram.py your_data.csv --save ./figs

import argparse
import os
import pandas as pd
import matplotlib.pyplot as plt

METRICS = [
    "FileRead(ms)",
    "PerNodeMerge(ms)",
    "SingleThreadMerge(ms)",
    "SingleNodeMerge(ms)",
    "MultiNodeMerge(ms)",
]

DROP_COLS = ["Date", "Time", "run_freq", "TotalLat(ms)"]

COMBOS = [
    ("numa", "numa"),
    ("numa", "regular"),
    ("regular", "numa"),
    ("regular", "regular"),
]

COLOR_MAP = {
    ("numa", "numa"): "tab:blue",
    ("numa", "regular"): "tab:orange",
    ("regular", "numa"): "tab:green",
    ("regular", "regular"): "tab:red",
}



def load_and_clean(csv_path: str) -> pd.DataFrame:
    df = pd.read_csv(csv_path, skipinitialspace=True)
    # Drop columns we don't want to display or use
    for c in DROP_COLS:
        if c in df.columns:
            df = df.drop(columns=c)
    # Ensure numeric types
    df["num_threads"] = pd.to_numeric(df["num_threads"], errors="coerce")
    df["bucket_count"] = pd.to_numeric(df["bucket_count"], errors="coerce")

    needed = {"num_threads", "thread_config", "DS_config", "bucket_count"}
    if not needed.issubset(df.columns):
        missing = needed - set(df.columns)
        raise ValueError(f"CSV missing required columns: {missing}")
    df = df.dropna(subset=list(needed))
    return df

def grouped_bar_positions(x_vals, n_groups, bar_width):
    """
    Given unique sorted x values and number of groups per x, return the center positions
    for each group index (0..n_groups-1). Returns a dict:
        positions[group_index] = list of x positions aligned to each x_val
    """
    x_vals = list(x_vals)
    total_width = n_groups * bar_width
    offsets = [(-total_width/2) + (i + 0.5) * bar_width for i in range(n_groups)]
    pos = {}
    for gi, off in enumerate(offsets):
        pos[gi] = [x + off for x in x_vals]
    return pos

def plot_metric_grouped_bars(df: pd.DataFrame, metric: str, save_dir: str = None, show: bool = False):
    """
    X-axis: bucket_count (evenly spaced categories).
    For each (thread_config, DS_config) combo and each bucket_count:
      - faded bar: metric at the *lowest* num_threads present
      - solid bar: metric at the *highest* num_threads present
    Bars are grouped by bucket_count and laid side-by-side with no overlap.
    """
    # Evenly spaced categorical x-axis (0..N-1) regardless of bucket numeric value
    x_levels = sorted(df["bucket_count"].unique())
    x_idx = list(range(len(x_levels)))

    n_combos = len(COMBOS)
    bars_per_group = n_combos * 2  # min + max for each combo
    group_width = 0.84             # total visual width per bucket group
    bar_width = group_width / bars_per_group

    fig, ax = plt.subplots(figsize=(12, 6))

    # helper to compute offset slot (0..bars_per_group-1) within the group
    def slot_offset(slot: int) -> float:
        # center group around the integer tick (x_idx)
        return -group_width / 2 + (slot + 0.5) * bar_width

    # For legend assembly once
    plotted_combos = set()
    have_min = False
    have_max = False

    for gi, combo in enumerate(COMBOS):
        tcfg, dcfg = combo
        color = COLOR_MAP[combo]
        combo_df = df[(df["thread_config"] == tcfg) & (df["DS_config"] == dcfg)]
        if combo_df.empty:
            continue

        # Slots: even slot = min thread bar, odd slot = max thread bar
        min_slot = gi * 2
        max_slot = gi * 2 + 1

        # Precompute y values in x_idx order
        y_min = []
        y_max = []

        for b in x_levels:
            at_b = combo_df[combo_df["bucket_count"] == b]
            if at_b.empty or metric not in at_b.columns:
                y_min.append(None)
                y_max.append(None)
                continue

            nts = sorted(at_b["num_threads"].unique())
            nt_min, nt_max = nts[0], nts[-1]

            # metric at min num_threads
            v_min = None
            rows_min = at_b[at_b["num_threads"] == nt_min]
            if not rows_min.empty:
                v_min = float(rows_min[metric].iloc[0])

            # metric at max num_threads
            v_max = None
            rows_max = at_b[at_b["num_threads"] == nt_max]
            if not rows_max.empty:
                v_max = float(rows_max[metric].iloc[0])

            if nt_min == nt_max:
                # only one bar (max) if there is a single num_threads at this bucket
                v_min = None

            y_min.append(v_min)
            y_max.append(v_max)

        # Compute actual x positions for each bar in this combo
        x_min = [xi + slot_offset(min_slot) for xi in x_idx]
        x_max = [xi + slot_offset(max_slot) for xi in x_idx]

        # Plot min (faded)
        for xp, y in zip(x_min, y_min):
            if y is not None:
                ax.bar(xp, y, width=bar_width, color=color, alpha=0.4)
                have_min = True

        # Plot max (solid)
        for xp, y in zip(x_max, y_max):
            if y is not None:
                ax.bar(xp, y, width=bar_width, color=color, alpha=1.0)
                have_max = True

        plotted_combos.add(combo)

    ax.set_title(f"{metric} vs bucket_count (min/max num_threads, grouped by bucket)")
    ax.set_xlabel("bucket_count")
    ax.set_ylabel(metric)
    ax.set_xticks(x_idx)
    ax.set_xticklabels([str(b) for b in x_levels])
    ax.grid(axis="y", linestyle="--", alpha=0.6)

    # Legend: combos + style meaning
    from matplotlib.patches import Patch
    legend_patches = [Patch(facecolor=COLOR_MAP[c], edgecolor="none", alpha=1.0, label=f"{c[0]}–{c[1]}")
                      for c in COMBOS if c in plotted_combos]
    if have_max:
        legend_patches.append(Patch(facecolor="gray", edgecolor="none", alpha=1.0, label="max num_threads (solid)"))
    if have_min:
        legend_patches.append(Patch(facecolor="gray", edgecolor="none", alpha=0.4, label="min num_threads (faded)"))
    if legend_patches:
        ax.legend(handles=legend_patches, bbox_to_anchor=(1.02, 1), loc="upper left")

    plt.tight_layout()

    if save_dir:
        os.makedirs(save_dir, exist_ok=True)
        out_path = os.path.join(
            save_dir,
            f"{metric.replace('(', '_').replace(')', '').replace('/', '_')}_bars_bucketcount.png"
        )
        plt.savefig(out_path, dpi=150)
        print(f"Saved: {out_path}")

    if show:
        plt.show()
    else:
        plt.close(fig)

def main():
    ap = argparse.ArgumentParser(description="Plot grouped bar charts for NUMA metrics (bucket_count on X).")
    ap.add_argument("csv", help="Path to the CSV file")
    ap.add_argument("--save", help="Directory to save figures (optional)")
    ap.add_argument("--show", action="store_true", help="Display figures interactively")
    args = ap.parse_args()

    df = load_and_clean(args.csv)

    for metric in METRICS:
        if metric not in df.columns:
            print(f"Warning: '{metric}' not found in CSV; skipping.")
            continue
        plot_metric_grouped_bars(df, metric, save_dir=args.save, show=args.show)

if __name__ == "__main__":
    main()