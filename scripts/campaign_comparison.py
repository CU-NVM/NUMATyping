#!/usr/bin/env python3
"""
campaign_comparison.py -- compare campaigns against each other, pairwise.

For every pair of the given campaigns and every AutoNUMA mode present in both,
writes into Campaigns/<bench>/comparisons/ (the bench level, next to the slugs):

    camp01v01.1_AN_off.csv         per-config throughput of A vs B with the
                                   change %, plus the compared-config gap
                                   (default numa/numa vs numa/regular) in each
                                   campaign and its shift in percentage points
    with --graph additionally:
    camp01v01.1_AN_off_throughput_change.png/.pdf
                                   one labelled circle per workload per config:
                                   how much B's throughput differs from A's
    camp01v01.1_AN_off_gap.png/.pdf
                                   dumbbell per workload: the gap in A (hollow)
                                   -> the gap in B (filled)

Values are mean operations per reporting interval in millions, as everywhere
else; if the two campaigns used different reporting intervals the values are
converted to millions of operations per second so they stay comparable.

Example:
    python3 scripts/campaign_comparison.py --campaigns campaign01 campaign01.1 --graph
"""
import argparse
import itertools
import sys
from pathlib import Path

import an_comparison as ac          # shared loaders, table writers, palette

ROOT = Path(__file__).resolve().parent.parent
MODES = ["AN_off", "AN_on"]


def short(slug):
    """campaign01 -> camp01 (used in file names and column headers)."""
    return slug.replace("campaign", "camp", 1)


def pair_tag(a, b):
    """(campaign01, campaign02.1) -> camp01v02.1"""
    sa, sb = short(a), short(b)
    return f"{sa}v{sb[4:] if sb.startswith('camp') else sb}"


def load_campaign(exp_dir):
    """-> {mode: ({workload: {config: value}}, interval)} for modes with data."""
    out = {}
    for mode in MODES:
        d = exp_dir / mode
        if d.is_dir():
            data, interval = ac.load_mode(d)
            if data:
                out[mode] = (data, interval)
    return out


# ------------------------------------------------------------------ tables
def compare_tables(na, nb, da, db, workloads, base, compared):
    """Build the stacked tables for one (pair, mode)."""
    tables = []
    for cfg in ac.CONFIGS:
        rows, ratios = [], []
        for wl in workloads:
            va, vb = da.get(wl, {}).get(cfg), db.get(wl, {}).get(cfg)
            if va and vb:
                ratios.append(vb / va)
                rows.append([wl, ac.fmt_tp(va), ac.fmt_tp(vb),
                             ac.fmt_pct((vb / va - 1) * 100)])
            else:
                rows.append([wl, ac.fmt_tp(va), ac.fmt_tp(vb), ""])
        gma = ac.geomean([da[w][cfg] for w in workloads if da.get(w, {}).get(cfg)])
        gmb = ac.geomean([db[w][cfg] for w in workloads if db.get(w, {}).get(cfg)])
        gmr = ac.geomean(ratios)
        rows.append(["Geomean", ac.fmt_tp(gma), ac.fmt_tp(gmb),
                     ac.fmt_pct((gmr - 1) * 100) if gmr else ""])
        tables.append((f"THROUGHPUT {cfg}: {na} vs {nb}",
                       ["Workload", na, nb, "change"], rows))

    rows, ra, rb = [], [], []
    for wl in workloads:
        a_ok = da.get(wl, {}).get(base) and da.get(wl, {}).get(compared)
        b_ok = db.get(wl, {}).get(base) and db.get(wl, {}).get(compared)
        ga = (da[wl][compared] / da[wl][base] - 1) * 100 if a_ok else None
        gb = (db[wl][compared] / db[wl][base] - 1) * 100 if b_ok else None
        if a_ok:
            ra.append(da[wl][compared] / da[wl][base])
        if b_ok:
            rb.append(db[wl][compared] / db[wl][base])
        rows.append([wl, ac.fmt_pct(ga), ac.fmt_pct(gb),
                     f"{gb - ga:+.2f}" if ga is not None and gb is not None else ""])
    gma, gmb = ac.geomean(ra), ac.geomean(rb)
    rows.append(["Geomean",
                 ac.fmt_pct((gma - 1) * 100) if gma else "",
                 ac.fmt_pct((gmb - 1) * 100) if gmb else "",
                 f"{(gmb - gma) * 100:+.2f}" if gma and gmb else ""])
    tables.append((f"GAP {compared} vs {base}: {na} vs {nb}",
                   ["Workload", na, nb, "shift(pp)"], rows))
    return tables


# ------------------------------------------------------------------ graphs
def plot_throughput_change(out_base, na, nb, da, db, workloads, an, unit):
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots(figsize=(9.0, 4.6))
    per_cfg, all_vals = [], [0.0]
    for cfg in ac.CONFIGS:
        pairs = [(w, db[w][cfg] / da[w][cfg]) for w in workloads
                 if da.get(w, {}).get(cfg) and db.get(w, {}).get(cfg)]
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
            colour = ac.WL_COLORS[workloads.index(wl) % len(ac.WL_COLORS)]
            y = (ratio - 1) * 100
            ax.scatter([x], [y], marker="o", s=235, c=colour,
                       edgecolors="white", linewidths=1.2, zorder=4)
            ax.text(x, y, wl, ha="center", va="center", zorder=5,
                    fontsize=6.6 if len(wl) > 1 else 7.4,
                    fontweight="bold", color=ac._on_color(colour))
        gm = (ac.geomean([r for _, r in pairs]) - 1) * 100
        ax.hlines(gm, i - 0.36, i + 0.36, color=ac.INK, lw=2.4, zorder=3)
        ax.annotate(f"{gm:+.2f}%", (i + 0.40, gm), va="center", ha="left",
                    fontsize=9.5, fontweight="bold", color=ac.INK, zorder=5)
    ax.axhline(0, color=ac.INK, lw=1.1, zorder=2)
    ax.set_xlim(-0.55, len(ac.CONFIGS) - 0.15)
    ax.set_xticks(range(len(ac.CONFIGS)))
    ax.set_xticklabels([c.replace("/", " th. /\n") + " ds." for c in ac.CONFIGS],
                       fontsize=9.5, color=ac.INK)
    ax.set_ylabel(f"throughput change, {nb} vs {na} (%)", fontsize=10, color=ac.INK)
    ax.set_title(f"{nb} vs {na} ({an}): throughput change by configuration",
                 fontsize=12, color=ac.INK, pad=10)
    ax.grid(axis="y", color=ac.GRID, lw=0.7, alpha=0.6, zorder=0)
    ax.set_axisbelow(True)
    ac._style(ax)
    handles = [plt.Line2D([], [], ls="", marker="o", ms=8.5,
                          mfc=ac.WL_COLORS[k % len(ac.WL_COLORS)], mec="white",
                          mew=1.0, label=wl) for k, wl in enumerate(workloads)]
    handles.append(plt.Line2D([], [], color=ac.INK, lw=2.4, label="geomean"))
    ax.legend(handles=handles, frameon=False, fontsize=9, loc="upper center",
              bbox_to_anchor=(0.5, -0.13), ncol=len(handles), labelcolor=ac.MUTED,
              handletextpad=0.4, columnspacing=1.4)
    fig.tight_layout()
    for ext in ("png", "pdf"):
        fig.savefig(f"{out_base}.{ext}", dpi=300, bbox_inches="tight")
    plt.close(fig)


def plot_gap(out_base, na, nb, da, db, workloads, an, base, compared):
    import matplotlib.pyplot as plt
    rows, ga, gb = [], [], []
    for w in workloads:
        if (da.get(w, {}).get(base) and da.get(w, {}).get(compared)
                and db.get(w, {}).get(base) and db.get(w, {}).get(compared)):
            rows.append(w)
            ga.append(da[w][compared] / da[w][base])
            gb.append(db[w][compared] / db[w][base])
    if not rows:
        return
    rows.append("Geomean")
    ga.append(ac.geomean(ga))
    gb.append(ac.geomean(gb))
    va = [(r - 1) * 100 for r in ga]
    vb = [(r - 1) * 100 for r in gb]
    ys = list(range(len(rows)))[::-1]
    fig, ax = plt.subplots(figsize=(7.4, 0.34 * len(rows) + 1.7))
    for y, a, b in zip(ys, va, vb):
        ax.plot([a, b], [y, y], color=ac.C_ACCENT, lw=2, zorder=2, alpha=.75)
        ax.scatter([a], [y], s=74, facecolors="white", edgecolors=ac.C_ACCENT,
                   linewidths=2, zorder=3)
        ax.scatter([b], [y], s=74, facecolors=ac.C_ACCENT, edgecolors="white",
                   linewidths=1.1, zorder=4)
    lo, hi = min(va + vb), max(va + vb)
    pad = (hi - lo) * 0.18 or 0.5
    for y, a, b in zip(ys, va, vb):
        ax.annotate(f"{b - a:+.2f}pp", (hi + pad * 0.35, y), va="center",
                    fontsize=9, color=ac.INK,
                    fontweight="bold" if rows[len(rows) - 1 - y] == "Geomean" else "normal")
    if lo > 0:
        ax.axvline(0, color=ac.GRID, lw=1)
    ax.set_xlim(lo - pad * 0.6, hi + pad * 1.5)
    ax.set_yticks(ys)
    ax.set_yticklabels(rows, fontsize=10, color=ac.INK)
    for lbl in ax.get_yticklabels():
        if lbl.get_text() == "Geomean":
            lbl.set_fontweight("bold")
    ax.set_xlabel(f"{compared} lead over {base} (%)", fontsize=10, color=ac.INK)
    ax.set_title(f"{compared} lead over {base} ({an}): {na} -> {nb}",
                 fontsize=11, color=ac.INK, pad=10)
    ax.grid(axis="x", color=ac.GRID, lw=0.7, alpha=0.6, zorder=0)
    ax.set_axisbelow(True)
    ac._style(ax)
    handles = [plt.Line2D([], [], marker="o", ls="", ms=8, mfc="white",
                          mec=ac.C_ACCENT, mew=2, label=na),
               plt.Line2D([], [], marker="o", ls="", ms=8, mfc=ac.C_ACCENT,
                          mec="white", label=nb)]
    ax.legend(handles=handles, frameon=False, fontsize=9, loc="upper center",
              bbox_to_anchor=(0.5, -0.16), ncol=2, labelcolor=ac.MUTED,
              handletextpad=0.5, columnspacing=2.5)
    fig.tight_layout()
    for ext in ("png", "pdf"):
        fig.savefig(f"{out_base}.{ext}", dpi=300, bbox_inches="tight")
    plt.close(fig)


# -------------------------------------------------------------------- main
def main():
    p = argparse.ArgumentParser(
        description="Compare campaigns pairwise; tables and figures per AutoNUMA mode.")
    p.add_argument("--campaigns", nargs="+", required=True,
                   help="two or more campaign slugs; every pair is compared")
    p.add_argument("--bench", default="ycsb")
    p.add_argument("--ROOT_DIR", default=str(ROOT))
    p.add_argument("--graph", action="store_true", help="also render the figures")
    p.add_argument("--baseline", default="numa/regular", choices=ac.CONFIGS,
                   help="baseline config for the gap table/figure (default: numa/regular)")
    p.add_argument("--compare", default="numa/numa", choices=ac.CONFIGS,
                   help="config whose lead over the baseline is compared (default: numa/numa)")
    args = p.parse_args()
    if len(args.campaigns) < 2:
        sys.exit("need at least two campaigns")

    bench_dir = Path(args.ROOT_DIR).resolve() / "Campaigns" / args.bench
    loaded = {}
    for slug in args.campaigns:
        d = bench_dir / slug
        if not d.is_dir():
            sys.exit(f"no such campaign: {d}")
        loaded[slug] = load_campaign(d)
        print(f"  {slug}: " + ", ".join(f"{m} ({len(v[0])} workloads)"
                                        for m, v in loaded[slug].items()))

    out_dir = bench_dir / "comparisons"
    out_dir.mkdir(parents=True, exist_ok=True)
    if args.graph:
        import matplotlib
        matplotlib.use("Agg")

    written = []
    for a, b in itertools.combinations(args.campaigns, 2):
        na, nb, tag = short(a), short(b), pair_tag(a, b)
        for mode in MODES:
            if mode not in loaded[a] or mode not in loaded[b]:
                print(f"  ! {tag} {mode}: missing in one campaign -- skipped")
                continue
            da, ia = loaded[a][mode]
            db, ib = loaded[b][mode]
            if ia != ib:                     # different reporting interval: use per-second
                da = {w: {c: v / ia for c, v in cf.items()} for w, cf in da.items()}
                db = {w: {c: v / ib for c, v in cf.items()} for w, cf in db.items()}
                unit = "M ops/s"
            else:
                unit = f"M ops / {ia}s interval"
            workloads = sorted(set(da) & set(db), key=ac.wl_sort_key)
            if not workloads:
                print(f"  ! {tag} {mode}: no common workloads -- skipped")
                continue
            tables = compare_tables(na, nb, da, db, workloads,
                                    args.baseline, args.compare)
            out = out_dir / f"{tag}_{mode}.csv"
            ac.write_tables(out, tables, [
                f"campaign comparison: {a} ({na}) vs {b} ({nb})   mode: {mode}",
                f"value = mean operations per reporting interval, in {unit}",
                f"change = ({nb} / {na} - 1) * 100; "
                f"gap = ({args.compare} / {args.baseline} - 1) * 100 within each campaign",
            ])
            written.append(out)
            if args.graph:
                gbase = out_dir / f"{tag}_{mode}_throughput_change"
                plot_throughput_change(gbase, na, nb, da, db, workloads, mode, unit)
                written += [Path(f"{gbase}.png"), Path(f"{gbase}.pdf")]
                gbase = out_dir / f"{tag}_{mode}_gap"
                plot_gap(gbase, na, nb, da, db, workloads, mode,
                         args.baseline, args.compare)
                written += [Path(f"{gbase}.png"), Path(f"{gbase}.pdf")]

    print()
    for w in written:
        print(f"  wrote {w}")


if __name__ == "__main__":
    main()
