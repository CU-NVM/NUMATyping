#!/usr/bin/env python3
"""
Find the minimum per-config --duration at which numa/numa separates cleanly
from numa/regular.

The campaign CSV reports CUMULATIVE ops at each interval, so per-interval
throughput is the first difference. With one run per config we have no
between-run variance, but we do have the within-run interval-to-interval
spread, which is what a short run is actually fighting. So:

  1. per-interval throughput series for each (arm, config)
  2. for each candidate window length W (in intervals), take the first W
     post-warmup intervals and ask whether numa/numa > numa/regular by more
     than the noise, via Welch's t-test on the two interval samples
  3. report the smallest W that is significant AND stays significant for every
     longer window (no flapping), which is the duration to use

Caveat stated plainly: a shared login node inflates the interval variance, so
the W this reports is an OVER-estimate of what a quiet compute node needs.
That is the safe direction to err in.
"""
import csv, sys, math
from collections import defaultdict
from pathlib import Path


def welch(a, b):
    """Welch's t and two-sided p (normal approx; n>=10 here so it's fine)."""
    na, nb = len(a), len(b)
    if na < 2 or nb < 2:
        return 0.0, 1.0
    ma, mb = sum(a) / na, sum(b) / nb
    va = sum((x - ma) ** 2 for x in a) / (na - 1)
    vb = sum((x - mb) ** 2 for x in b) / (nb - 1)
    se = math.sqrt(va / na + vb / nb)
    if se == 0:
        return 0.0, 1.0
    t = (ma - mb) / se
    # two-sided p from the normal CDF
    p = 2.0 * (1.0 - 0.5 * (1.0 + math.erf(abs(t) / math.sqrt(2.0))))
    return t, p


def load(path):
    """-> {(arm, config): [(elapsed, cumulative_total_ops), ...]}"""
    series = defaultdict(list)
    arm = Path(path).parent.name              # AN_off / AN_on
    with open(path) as fh:
        for row in csv.DictReader(fh, skipinitialspace=True):
            try:
                cfg = f"{row['Thread_Config'].strip()}/{row['DS_Config'].strip()}"
                series[(arm, cfg)].append((int(row['Duration']), int(row['Total_Ops'])))
            except (KeyError, ValueError):
                continue
    return series


def per_interval(points):
    """cumulative -> [(t_end, ops_per_sec), ...]"""
    pts = sorted(set(points))
    out = []
    for (t0, c0), (t1, c1) in zip(pts, pts[1:]):
        if t1 > t0 and c1 >= c0:
            out.append((t1, (c1 - c0) / (t1 - t0)))
    return out


def main(paths):
    series = defaultdict(list)
    for p in paths:
        for k, v in load(p).items():
            series[k].extend(v)

    if not series:
        print("no data found"); return 1

    rates = {k: per_interval(v) for k, v in series.items()}

    print("=" * 78)
    print("PER-INTERVAL THROUGHPUT (ops/s), after the first interval")
    print("=" * 78)
    print(f"{'arm':<8} {'config':<16} {'n':>3} {'mean':>14} {'stdev':>12} {'cv%':>7}")
    for k in sorted(rates):
        r = [x for _, x in rates[k][1:]]        # drop first interval (ramp)
        if not r:
            continue
        m = sum(r) / len(r)
        sd = math.sqrt(sum((x - m) ** 2 for x in r) / (len(r) - 1)) if len(r) > 1 else 0.0
        print(f"{k[0]:<8} {k[1]:<16} {len(r):>3} {m:>14,.0f} {sd:>12,.0f} {100*sd/m if m else 0:>7.1f}")

    for arm in sorted({k[0] for k in rates}):
        nn = [x for _, x in rates.get((arm, 'numa/numa'), [])[1:]]
        nr = [x for _, x in rates.get((arm, 'numa/regular'), [])[1:]]
        if not nn or not nr:
            continue
        interval = rates[(arm, 'numa/numa')][0][0]

        print()
        print("=" * 78)
        print(f"{arm}: numa/numa vs numa/regular as the window grows")
        print("=" * 78)
        print(f"{'window':>8} {'numa/numa':>14} {'numa/regular':>14} {'advantage':>11} {'p':>9}  verdict")

        ok_from = None
        rows = []
        for w in range(2, min(len(nn), len(nr)) + 1):
            a, b = nn[:w], nr[:w]
            ma, mb = sum(a) / w, sum(b) / w
            adv = (ma / mb - 1.0) * 100 if mb else 0.0
            t, p = welch(a, b)
            sig = (p < 0.05) and (ma > mb)
            rows.append((w, ma, mb, adv, p, sig))

        # smallest window that is significant and never flaps back afterwards
        for i, (w, *_rest, sig) in enumerate(rows):
            if sig and all(r[-1] for r in rows[i:]):
                ok_from = w
                break

        for w, ma, mb, adv, p, sig in rows:
            mark = "OK" if sig else "--"
            if ok_from is not None and w == ok_from:
                mark = "<== stable from here"
            print(f"{w*interval:>7}s {ma:>14,.0f} {mb:>14,.0f} {adv:>10.1f}% {p:>9.2}  {mark}")

        print()
        final_adv = (sum(nn)/len(nn)) / (sum(nr)/len(nr)) - 1.0
        print(f"  full-run advantage : {final_adv*100:+.1f}%")
        if ok_from is None:
            print("  MINIMUM DURATION  : not reached within this run --")
            print("                      numa/numa never separated from numa/regular "
                  "at p<0.05 for the rest of the run.")
        else:
            print(f"  MINIMUM DURATION  : {ok_from*interval}s of measured time "
                  f"(plus warmup), on this contended login node.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
