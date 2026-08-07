#!/usr/bin/env python3
"""
an_comparison.py -- steady-state throughput comparison tables for one campaign.

Reads   Campaigns/<bench>/<slug>/AN_{off,on}/*.csv
Writes  Campaigns/<bench>/<slug>/comparisons/
            AN_comparison.csv       every config under both AutoNUMA modes, plus
                                    the AutoNUMA effect (AN_on vs AN_off) per config
            AN_ON_comparison.csv    AN_on throughput, then one table per config
                                    taken as the baseline, showing how much faster
                                    every other config is
            AN_OFF_comparison.csv   the same for AN_off

The reported value is the mean number of operations per reporting interval, in
millions.  Total_Ops in the CSV is cumulative, so per-interval counts are the
successive differences and their mean is the final cumulative value divided by the
number of intervals.  Percentages are (other / baseline - 1)*100, so positive means
the other config is faster than the baseline.

    python3 scripts/an_comparison.py --campaign campaign04 [--graph]
"""
import argparse
import csv
import math
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIGS = ["numa/numa", "numa/regular", "regular/numa", "regular/regular"]
MODES = [("AN_off", "AN_off"), ("AN_on", "AN_on")]


# ----------------------------------------------------------------- loading
def workload_label(path):
    """ycsb_A-50-50-50_D-100-0-50.csv -> 'AD'   (matches the bar-chart labels)."""
    name = Path(path).stem
    if name.startswith("ycsb_"):
        name = name[5:]
    letters = []
    for block in name.replace("_", ",").split(","):
        if "-" in block:
            letter = block.split("-")[0]
            if letter not in letters:
                letters.append(letter)
    return "".join(letters) or name


def wl_sort_key(w):
    """Single-letter workloads first (A..F), then the mixed ones (AD)."""
    return (len(w), w)


def load_mode(mode_dir):
    """-> ({workload: {config: mean_ops_per_interval_in_M}}, interval_seconds)

    Total_Ops in the CSV is cumulative, so the per-interval counts are the
    successive differences and their mean is simply the final cumulative value
    divided by the number of interval rows.
    """
    data, interval_lengths = {}, []
    for path in sorted(mode_dir.glob("*.csv")):
        rows_by_cfg = {}                            # config -> [(duration, cumulative)]
        with open(path, newline="") as fh:
            reader = csv.reader(fh)
            try:
                header = [h.strip().lower() for h in next(reader)]
            except StopIteration:
                continue
            idx = {h: i for i, h in enumerate(header)}
            if not all(k in idx for k in ("thread_config", "ds_config",
                                          "duration", "total_ops")):
                print(f"  ! {path.name}: unexpected header, skipped")
                continue
            for row in reader:
                if len(row) < len(header):
                    continue
                try:
                    dur = float(row[idx["duration"]].strip())
                    total = float(row[idx["total_ops"]].strip())
                except ValueError:
                    continue
                if dur <= 0 or total <= 0:          # the dur=0 header row, or a failed run
                    continue
                cfg = (f'{row[idx["thread_config"]].strip()}'
                       f'/{row[idx["ds_config"]].strip()}')
                rows_by_cfg.setdefault(cfg, []).append((dur, total))
        per_cfg = {}
        for cfg, rows in rows_by_cfg.items():
            rows.sort()
            final_dur, final_total = rows[-1]
            n = len(rows)                           # one row per completed interval
            per_cfg[cfg] = final_total / n / 1e6
            interval_lengths.append(final_dur / n)
        if per_cfg:
            data[workload_label(path)] = per_cfg
    interval = (round(sorted(interval_lengths)[len(interval_lengths) // 2])
                if interval_lengths else 0)
    return data, interval


# ------------------------------------------------------------------ tables
def geomean(values):
    vals = [v for v in values if v > 0]
    return math.exp(sum(math.log(v) for v in vals) / len(vals)) if vals else None


def fmt_tp(v):
    return f"{v:.2f}" if v is not None else ""


def fmt_pct(v):
    return f"{v:+.2f}%" if v is not None else ""


def throughput_table(title, data_by_col, workloads, col_labels):
    """data_by_col: {col_label: {workload: value}}."""
    rows = []
    for wl in workloads:
        rows.append([wl] + [fmt_tp(data_by_col[c].get(wl)) for c in col_labels])
    gm = [geomean([data_by_col[c][w] for w in workloads if data_by_col[c].get(w)])
          for c in col_labels]
    rows.append(["Geomean"] + [fmt_tp(v) for v in gm])
    return (title, ["Workload"] + col_labels, rows)


def ratio_table(title, numerators, denominator, workloads, col_labels):
    """Percentage tables: (numerator / denominator - 1) * 100, per workload."""
    rows = []
    ratios = {c: [] for c in col_labels}
    for wl in workloads:
        cells = []
        for c in col_labels:
            num = numerators[c].get(wl)
            den = denominator.get(wl)
            if num and den:
                ratios[c].append(num / den)
                cells.append(fmt_pct((num / den - 1) * 100))
            else:
                cells.append("")
        rows.append([wl] + cells)
    gm = [geomean(ratios[c]) for c in col_labels]
    rows.append(["Geomean"] + [fmt_pct((v - 1) * 100) if v else "" for v in gm])
    return (title, ["Workload"] + col_labels, rows)


def gap_shift_table(title, off, on, base, others, workloads):
    """How a config-vs-baseline gap changes between the two AutoNUMA modes.

    For each compared config: its gap over `base` with AutoNUMA off, the same gap
    with AutoNUMA on, and the shift between them in percentage points.
    """
    header = ["Workload"]
    for c in others:
        header += [f"{c} AN_off", f"{c} AN_on", f"{c} shift(pp)"]
    ratios = {c: {"off": [], "on": []} for c in others}
    rows = []
    for wl in workloads:
        cells = []
        for c in others:
            ob, oc = off.get(wl, {}).get(base), off.get(wl, {}).get(c)
            nb, nc = on.get(wl, {}).get(base), on.get(wl, {}).get(c)
            if ob and oc and nb and nc:
                ratios[c]["off"].append(oc / ob)
                ratios[c]["on"].append(nc / nb)
                g_off, g_on = (oc / ob - 1) * 100, (nc / nb - 1) * 100
                cells += [fmt_pct(g_off), fmt_pct(g_on), f"{g_on - g_off:+.2f}"]
            else:
                cells += ["", "", ""]
        rows.append([wl] + cells)
    cells = []
    for c in others:
        go, gn = geomean(ratios[c]["off"]), geomean(ratios[c]["on"])
        if go and gn:
            a, b = (go - 1) * 100, (gn - 1) * 100
            cells += [fmt_pct(a), fmt_pct(b), f"{b - a:+.2f}"]
        else:
            cells += ["", "", ""]
    rows.append(["Geomean"] + cells)
    return (title, header, rows)


def write_tables(path, tables, preamble):
    with open(path, "w", newline="") as fh:
        w = csv.writer(fh)
        for line in preamble:
            w.writerow([line])
        for title, header, rows in tables:
            w.writerow([])
            w.writerow([f"== {title} =="])
            w.writerow(header)
            w.writerows(rows)


def per_mode_file(out_path, data, mode_name, slug, interval):
    """Throughput table + one baseline table per config, for a single AN mode."""
    workloads = sorted(data, key=wl_sort_key)
    by_cfg = {c: {w: data[w].get(c) for w in workloads} for c in CONFIGS}
    tables = [throughput_table(
        f"MEAN OPS PER INTERVAL (M ops / {interval}s) -- AutoNUMA "
        f"{mode_name.split('_')[1].upper()}",
        by_cfg, workloads, CONFIGS)]
    for base in CONFIGS:
        others = [c for c in CONFIGS if c != base]
        tables.append(ratio_table(
            f"BASELINE: {base}  (+ = that config is faster than {base})",
            {c: by_cfg[c] for c in others}, by_cfg[base], workloads, others))
    write_tables(out_path, tables, [
        f"campaign: {slug}   mode: {mode_name}",
        f"value = mean operations per {interval}s interval, in millions "
        f"(final cumulative Total_Ops / number of intervals)",
        "percentages = (config / baseline - 1) * 100",
    ])


def combined_file(out_path, off, on, slug, interval):
    """Both modes side by side, then the AutoNUMA effect per config."""
    workloads = sorted(set(off) | set(on), key=wl_sort_key)
    cols, by_col = [], {}
    for data, suffix in ((off, "AN_off"), (on, "AN_on")):
        for c in CONFIGS:
            label = f"{c} {suffix}"
            cols.append(label)
            by_col[label] = {w: data.get(w, {}).get(c) for w in workloads}
    tables = [throughput_table(
        f"MEAN OPS PER INTERVAL (M ops / {interval}s) -- both AutoNUMA modes",
        by_col, workloads, cols)]
    # AutoNUMA effect: each config's AN_on against its own AN_off
    rows, ratios = [], {c: [] for c in CONFIGS}
    for wl in workloads:
        cells = []
        for c in CONFIGS:
            a, b = off.get(wl, {}).get(c), on.get(wl, {}).get(c)
            if a and b:
                ratios[c].append(b / a)
                cells.append(fmt_pct((b / a - 1) * 100))
            else:
                cells.append("")
        rows.append([wl] + cells)
    gm = [geomean(ratios[c]) for c in CONFIGS]
    rows.append(["Geomean"] + [fmt_pct((v - 1) * 100) if v else "" for v in gm])
    tables.append(("AUTONUMA EFFECT: AN_on vs AN_off  (+ = AutoNUMA ON is faster)",
                   ["Workload"] + CONFIGS, rows))
    # how each config-vs-baseline gap shifts between the two AutoNUMA modes
    for base in CONFIGS:
        others = [c for c in CONFIGS if c != base]
        tables.append(gap_shift_table(
            f"GAP vs {base} UNDER EACH AutoNUMA MODE  "
            f"(+ = that config faster than {base}; shift = AN_on gap - AN_off gap)",
            off, on, base, others, workloads))
    write_tables(out_path, tables, [
        f"campaign: {slug}",
        f"value = mean operations per {interval}s interval, in millions "
        f"(final cumulative Total_Ops / number of intervals)",
        "AutoNUMA effect = (AN_on / AN_off - 1) * 100 for the same config",
        "gap tables = (config / baseline - 1) * 100 within each mode; "
        "shift = AN_on gap minus AN_off gap, in percentage points",
    ])


# ------------------------------------------------------------------ graphs
# Diverging pair for "AutoNUMA helped / hurt" and one accent for the dumbbells.
# Checked with the dataviz validator: lightness band, chroma floor, CVD
# separation (dE 20.8 protan), normal-vision floor and >=3:1 contrast all pass.
# (The bar-chart palette in bar_plot_ycsb.py does NOT pass -- its yellow sits at
# 1.42:1 against white -- so these figures do not reuse it.)
C_POS, C_NEG, C_ACCENT = "#2a5db0", "#b3452c", "#2a5db0"
INK, MUTED, GRID = "#1a1a1a", "#6b6b6b", "#d9d9d9"

# Per-workload fill colours.  Seven categorical hues in one dot cluster do NOT
# clear the validator's all-pairs gates (green/orange dE 3.2 under protanopia;
# magenta/orange 12.9 for normal vision), so the workload name is written inside
# each circle -- identity never rests on hue, and the colour is a fast visual
# index rather than the thing you have to decode.
WL_COLORS = ["#2a78d6", "#eb6834", "#1baf7a", "#eda100",
             "#e87ba4", "#008300", "#4a3aa7"]


def _on_color(hex_color):
    """Black or white label text, whichever reads on that fill."""
    r, g, b = (int(hex_color[i:i + 2], 16) / 255 for i in (1, 3, 5))
    lin = [c / 12.92 if c <= 0.04045 else ((c + 0.055) / 1.055) ** 2.4 for c in (r, g, b)]
    lum = 0.2126 * lin[0] + 0.7152 * lin[1] + 0.0722 * lin[2]
    return "#101010" if lum > 0.42 else "#ffffff"


def _style(ax):
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    for s in ("left", "bottom"):
        ax.spines[s].set_color(GRID)
    ax.tick_params(colors=MUTED, labelsize=9, length=3)


def plot_autonuma_effect(out_base, off, on, slug, workloads):
    """Each workload is drawn as its own name: how much AutoNUMA changed each config."""
    import matplotlib.pyplot as plt
    import matplotlib.patheffects as pe
    halo = [pe.withStroke(linewidth=2.6, foreground="white")]
    fig, ax = plt.subplots(figsize=(9.0, 4.6))
    # Gather first: text artists do not drive autoscaling, so the limits must be
    # set explicitly or the labels land outside the axes.
    per_cfg, all_vals = [], [0.0]
    for cfg in CONFIGS:
        pairs = [(w, on[w][cfg] / off[w][cfg]) for w in workloads
                 if off.get(w, {}).get(cfg) and on.get(w, {}).get(cfg)]
        per_cfg.append(pairs)
        all_vals += [(r - 1) * 100 for _, r in pairs]
    lo, hi = min(all_vals), max(all_vals)
    pad = max((hi - lo) * 0.13, 0.12)
    ax.set_ylim(lo - pad, hi + pad)

    for i, pairs in enumerate(per_cfg):
        if not pairs:
            continue
        span = 0.30 if len(pairs) > 1 else 0
        for k, (wl, ratio) in enumerate(pairs):
            x = i + (-span + 2 * span * k / (len(pairs) - 1) if len(pairs) > 1 else 0)
            colour = WL_COLORS[workloads.index(wl) % len(WL_COLORS)]
            y = (ratio - 1) * 100
            ax.scatter([x], [y], marker="o", s=235, c=colour,
                       edgecolors="white", linewidths=1.2, zorder=4)
            ax.text(x, y, wl, ha="center", va="center", zorder=5,
                    fontsize=6.6 if len(wl) > 1 else 7.4,
                    fontweight="bold", color=_on_color(colour))
        gm = (geomean([r for _, r in pairs]) - 1) * 100
        ax.hlines(gm, i - 0.36, i + 0.36, color=INK, lw=2.4, zorder=3)
        ax.annotate(f"{gm:+.2f}%", (i + 0.40, gm), va="center", ha="left",
                    fontsize=9.5, fontweight="bold", color=INK, zorder=5)
    ax.axhline(0, color=INK, lw=1.1, zorder=2)
    ax.set_xlim(-0.55, len(CONFIGS) - 0.15)
    ax.set_xticks(range(len(CONFIGS)))
    ax.set_xticklabels([c.replace("/", " th. /\n") + " ds." for c in CONFIGS],
                       fontsize=9.5, color=INK)
    ax.set_ylabel("throughput change when AutoNUMA is enabled (%)",
                  fontsize=10, color=INK)
    ax.set_title(f"{slug}: effect of enabling AutoNUMA, by configuration",
                 fontsize=12, color=INK, pad=10)
    ax.grid(axis="y", color=GRID, lw=0.7, alpha=0.6, zorder=0)
    ax.set_axisbelow(True)
    _style(ax)
    handles = [plt.Line2D([], [], ls="", marker="o", ms=8.5,
                          mfc=WL_COLORS[k % len(WL_COLORS)], mec="white", mew=1.0,
                          label=wl)
               for k, wl in enumerate(workloads)]
    handles.append(plt.Line2D([], [], color=INK, lw=2.4, label="geomean"))
    ax.legend(handles=handles, frameon=False, fontsize=9, loc="upper center",
              bbox_to_anchor=(0.5, -0.13), ncol=len(handles), labelcolor=MUTED,
              handletextpad=0.4, columnspacing=1.4)
    fig.tight_layout()
    for ext in ("png", "pdf"):
        fig.savefig(f"{out_base}.{ext}", dpi=300, bbox_inches="tight")
    plt.close(fig)


def plot_gap_shift(out_base, off, on, slug, workloads, base, compared):
    """Dumbbell: the compared config's lead over `base`, AutoNUMA off -> on."""
    import matplotlib.pyplot as plt
    rows, r_off, r_on = [], [], []
    for w in workloads:
        ob, oc = off.get(w, {}).get(base), off.get(w, {}).get(compared)
        nb, nc = on.get(w, {}).get(base), on.get(w, {}).get(compared)
        if ob and oc and nb and nc:
            rows.append(w)
            r_off.append(oc / ob)
            r_on.append(nc / nb)
    if not rows:
        return
    rows.append("Geomean")
    r_off.append(geomean(r_off))
    r_on.append(geomean(r_on))
    g_off = [(r - 1) * 100 for r in r_off]
    g_on = [(r - 1) * 100 for r in r_on]
    ys = list(range(len(rows)))[::-1]

    fig, ax = plt.subplots(figsize=(7.4, 0.34 * len(rows) + 1.7))
    for y, a, b in zip(ys, g_off, g_on):
        ax.plot([a, b], [y, y], color=C_ACCENT, lw=2, zorder=2, alpha=.75)
        ax.scatter([a], [y], s=74, facecolors="white", edgecolors=C_ACCENT,
                   linewidths=2, zorder=3)
        ax.scatter([b], [y], s=74, facecolors=C_ACCENT, edgecolors="white",
                   linewidths=1.1, zorder=4)
    lo, hi = min(g_off + g_on), max(g_off + g_on)
    pad = (hi - lo) * 0.18 or 0.5
    for y, a, b in zip(ys, g_off, g_on):
        ax.annotate(f"{b - a:+.2f}pp", (hi + pad * 0.35, y), va="center",
                    fontsize=9, color=INK,
                    fontweight="bold" if rows[len(rows) - 1 - y] == "Geomean" else "normal")
    if lo > 0:
        ax.axvline(0, color=GRID, lw=1)
    ax.set_xlim(lo - pad * 0.6, hi + pad * 1.5)
    ax.set_yticks(ys)
    ax.set_yticklabels(rows, fontsize=10, color=INK)
    for lbl in ax.get_yticklabels():
        if lbl.get_text() == "Geomean":
            lbl.set_fontweight("bold")
    ax.set_xlabel(f"{compared} lead over {base} (%)", fontsize=10, color=INK)
    ax.set_title(f"{slug}: how the {compared} lead over {base} "
                 f"changes when AutoNUMA is enabled", fontsize=11, color=INK, pad=10)
    ax.grid(axis="x", color=GRID, lw=0.7, alpha=0.6, zorder=0)
    ax.set_axisbelow(True)
    _style(ax)
    handles = [plt.Line2D([], [], marker="o", ls="", ms=8, mfc="white",
                          mec=C_ACCENT, mew=2, label="AutoNUMA off"),
               plt.Line2D([], [], marker="o", ls="", ms=8, mfc=C_ACCENT,
                          mec="white", label="AutoNUMA on")]
    ax.legend(handles=handles, frameon=False, fontsize=9, loc="upper center",
              bbox_to_anchor=(0.5, -0.16), ncol=2, labelcolor=MUTED,
              handletextpad=0.5, columnspacing=2.5)
    fig.tight_layout()
    for ext in ("png", "pdf"):
        fig.savefig(f"{out_base}.{ext}", dpi=300, bbox_inches="tight")
    plt.close(fig)


def make_graphs(out_dir, off, on, slug, baseline, compared):
    try:
        import matplotlib
        matplotlib.use("Agg")
    except ImportError:
        print("  ! matplotlib not available -- skipping --graph")
        return []
    workloads = sorted(set(off) & set(on), key=wl_sort_key)
    if not workloads:
        print("  ! both AutoNUMA modes are needed for the graphs -- skipping")
        return []
    written = []
    base = out_dir / "autonuma_effect"
    plot_autonuma_effect(base, off, on, slug, workloads)
    written += [Path(f"{base}.png"), Path(f"{base}.pdf")]
    if compared != baseline:
        tag = f"{compared}_vs_{baseline}".replace("/", "-")
        b = out_dir / f"gap_shift_{tag}"
        plot_gap_shift(b, off, on, slug, workloads, baseline, compared)
        written += [Path(f"{b}.png"), Path(f"{b}.pdf")]
    else:
        print("  ! --compare equals --baseline -- skipping the gap-shift figure")
    return written


# -------------------------------------------------------------------- main
def main():
    p = argparse.ArgumentParser(
        description="Generate AutoNUMA throughput comparison tables for a campaign.")
    p.add_argument("--campaign", required=True, help="campaign slug, e.g. campaign04")
    p.add_argument("--bench", default="ycsb", help="benchmark folder (default: ycsb)")
    p.add_argument("--ROOT_DIR", default=str(ROOT), help="path to the NUMATyping root")
    p.add_argument("--graph", action="store_true",
                   help="also render figures into comparisons/ (needs both AN modes)")
    p.add_argument("--baseline", default="numa/regular", choices=CONFIGS,
                   help="baseline config for the gap-shift figure (default: numa/regular)")
    p.add_argument("--compare", default="numa/numa", choices=CONFIGS,
                   help="config whose lead over the baseline is plotted (default: numa/numa)")
    args = p.parse_args()

    exp_dir = Path(args.ROOT_DIR).resolve() / "Campaigns" / args.bench / args.campaign
    if not exp_dir.is_dir():
        sys.exit(f"No such campaign: {exp_dir}")

    modes, interval = {}, 0
    for folder, _ in MODES:
        d = exp_dir / folder
        modes[folder], iv = load_mode(d) if d.is_dir() else ({}, 0)
        interval = interval or iv
        n = sum(len(v) for v in modes[folder].values())
        print(f"  {folder}: {len(modes[folder])} workloads, {n} config runs")

    out_dir = exp_dir / "comparisons"
    out_dir.mkdir(parents=True, exist_ok=True)
    written = []

    for folder, name in MODES:
        if modes[folder]:
            out = out_dir / f"AN_{'ON' if folder == 'AN_on' else 'OFF'}_comparison.csv"
            per_mode_file(out, modes[folder], name, args.campaign, interval)
            written.append(out)

    if modes["AN_off"] and modes["AN_on"]:
        out = out_dir / "AN_comparison.csv"
        combined_file(out, modes["AN_off"], modes["AN_on"], args.campaign, interval)
        written.append(out)
    else:
        print("  ! only one AutoNUMA mode present -- skipping AN_comparison.csv")

    if args.graph:
        written += make_graphs(out_dir, modes["AN_off"], modes["AN_on"],
                               args.campaign, args.baseline, args.compare)

    print()
    for w in written:
        print(f"  wrote {w}")


if __name__ == "__main__":
    main()
