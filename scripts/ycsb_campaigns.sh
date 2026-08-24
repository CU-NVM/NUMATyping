#!/bin/bash
# ycsb_campaigns.sh -- the ycsb campaign definitions, scaled for Perlmutter.
#
# Sourceable table, so the slurm script and any manual invocation cannot drift
# apart.  Usage:
#
#   source scripts/ycsb_campaigns.sh
#   ycsb_campaign_params campaign01      # echoes the campaign.py argument string
#   ycsb_campaign_list                   # echoes all campaign names
#
# SCALING (stormbreaker.md section 9.2)
# ------------------------------------
# Working set scaled 3x from the stormbreaker campaigns, with `buckets` scaled
# alongside `keys` so the LOAD FACTOR is preserved:
#
#       LF = (keys/2) / (tables x buckets)
#
# Load factor is the variable campaigns 01 vs 01.1 (and 02 vs 02.1) exist to
# isolate -- 0.75 shallow chains vs 1.67 deep chains -- so scaling keys without
# scaling buckets would silently change the thing under study.
#
#   campaign      storm keys -> perl keys   storm buckets -> perl buckets   LF
#   campaign01        100M -> 300M              66713 -> 200000            0.75
#   campaign01.1      100M -> 300M              30011 ->  90011            1.67
#   campaign02        100M -> 300M              66713 -> 200000            0.75
#   campaign02.1      100M -> 300M              30011 ->  90011            1.67
#   campaign03        100M -> 300M              30011 ->  90011            1.67
#   campaign04         20M ->  10M              13337 ->   6669            0.75  <- DOWN
#
# campaign04 is the exception and scales DOWN.  At payload 4096 its stormbreaker
# size reaches 154 GiB at the 4x steady-state bound, over the ~125 GiB that
# nodes 0+7 actually provide.  Halved to 10M keys / 6669 buckets (19.2 G
# prefill, 76.9 G at 4x).
#
# Everything else is deliberately IDENTICAL to the stormbreaker campaigns:
# tables=1000, warmup=60, duration=300, interval=20, hash=mix, mix, theta, and
# the full 4 configs x 7 workloads.  --duration 300 is passed explicitly because
# benchmarks.py defaults it to 1200, which no campaign used.
#
#
# MEASURED 2026-08-23 on Perlmutter: the 4x steady-state bound above is an
# ASYMPTOTE that a 300 s run does not approach at these scales.  RSS sampled
# during real runs:
#
#     keys   prefill   peak seen   ratio   note
#      20M   1628 MB     6434 MB   3.95x   plateaued at t=35 s
#      60M   4883 MB     9356 MB   1.92x   still climbing at t=340 s
#
# Small keyspaces saturate fast; larger ones do not saturate at all within a
# run.  The insert rate is roughly scale-invariant -- half the keyspace is
# absent at every scale, so ops/s x P(absent) is similar -- which means growth
# is about 9 GB per 360 s window regardless of `keys`.  For campaign01 at 300M
# keys expect a peak near 33 GiB, not the 95 GiB the 4x bound suggests.
#
# Two things follow:
#   - Memory headroom is much larger than the 4x column implies. The slurm
#     job still runs a memory watchdog, because being wrong about this in the
#     other direction costs a whole reservation.
#   - Warmup stays at 60 s. Steady state is unreachable in an acceptable run
#     length at 3x scale, so a longer warmup would only shift the measurement
#     from ~1.15x to ~1.25x prefill while breaking protocol match with
#     stormbreaker. Throughput is stable during growth anyway (measured drift
#     0.0-0.2% across a 300 s window), and every config runs a fresh process
#     from identical prefill, so the configs stay comparable to each other.
#
# --threads comes from PARTITION_THREADS (64 on nodes 0+7), NOT the
# benchmarks.py default of 80, which is a stormbreaker value.  Do NOT use
# runYCSB.py's --perlmutter defaults (200M keys / 266600 buckets / 128 threads):
# those are the paper's numbers and are not reproducible with today's prefill
# semantics, since commit f5dcf278 changed prefill to every other key.

ycsb_campaign_list() {
    echo "campaign01 campaign01.1 campaign02 campaign02.1 campaign03 campaign04"
}

ycsb_campaign_params() {
    local c="$1"
    # shared by every campaign
    local common="--hash mix --tables 1000 --warmup 60 --duration 300 --interval 20"
    case "$c" in
        campaign01)
            echo "--mix uniform --theta 0.7 --payload 128 --buckets 200000 --keys 300000000 $common" ;;
        campaign01.1)
            echo "--mix uniform --theta 0.7 --payload 128 --buckets 90011  --keys 300000000 $common" ;;
        campaign02)
            echo "--mix zipfian --theta 0.9 --payload 128 --buckets 200000 --keys 300000000 $common" ;;
        campaign02.1)
            echo "--mix zipfian --theta 0.9 --payload 128 --buckets 90011  --keys 300000000 $common" ;;
        campaign03)
            echo "--mix zipfian --theta 0.7 --payload 64  --buckets 90011  --keys 300000000 $common" ;;
        campaign04)
            echo "--mix uniform --theta 0.7 --payload 4096 --buckets 6669  --keys 10000000  $common" ;;
        *)
            echo "ycsb_campaigns.sh: unknown campaign '$c'" >&2
            echo "  known: $(ycsb_campaign_list)" >&2
            return 1 ;;
    esac
}

ycsb_campaign_purpose() {
    local c="$1"
    case "$c" in
        campaign01)   echo "campaign01 scaled 3x (keys 100M->300M, buckets 66713->200000, LF 0.75 held)" ;;
        campaign01.1) echo "campaign01.1 scaled 3x (keys 100M->300M, buckets 30011->90011, LF 1.67 held)" ;;
        campaign02)   echo "campaign02 scaled 3x (keys 100M->300M, buckets 66713->200000, LF 0.75 held)" ;;
        campaign02.1) echo "campaign02.1 scaled 3x (keys 100M->300M, buckets 30011->90011, LF 1.67 held)" ;;
        campaign03)   echo "campaign03 scaled 3x (keys 100M->300M, buckets 30011->90011, LF 1.67 held)" ;;
        campaign04)   echo "campaign04 scaled DOWN 2x (keys 20M->10M, buckets 13337->6669, LF 0.75 held); 4KB payload would need 154G at stormbreaker size, over the 125G budget" ;;
        *) return 1 ;;
    esac
}
