"""
benchmarks.py -- the ONLY file to touch when adding a benchmark.

Each entry fully describes a benchmark: its binary, CSV header, the command to
run one config, its default workloads, its working dir, and -- crucially -- the
list of command-line parameters it needs.  campaign.py and run.py are
benchmark-independent: they add these params to their CLI dynamically (via
add_bench_args) and read them back (via extract_params), so they never mention
--threads/--buckets/etc. themselves.

A param spec is: param(name, type, default, help).
"""


def param(name, typ, default, help=""):
    return {"name": name, "type": typ, "default": default, "help": help}


# ---------------------------------------------------------------------- YCSB
YCSB_HEADER = ("Date, Time, Num_Tables, Num_Threads, Thread_Config, DS_Config, "
               "Mix, Buckets, Workload, Duration, Num_Keys, Interval, "
               "Ops_Node0, Ops_Node1, Total_Ops")

YCSB_WORKLOADS = [
    "A-50-50-50,A-100-0-50", "B-50-50-50,B-100-0-50", "C-50-50-50,C-100-0-50",
    "D-50-50-50,D-100-0-50", "E-50-50-50,E-100-0-50", "F-50-50-50,F-100-0-50",
    "A-50-50-50,D-100-0-50",
]

# DataStructureTests emits: date, time, DS_name, num_DS, num_threads, th_config,
# DS_config, duration, keyspace, interval, ops per node..., total.
DS_HEADER = ("Date, Time, DS_Name, Num_DS, Num_Threads, Thread_Config, DS_Config, "
             "Duration, Keyspace, Interval, Ops_Node0, Ops_Node1, Total_Ops")

# DataStructureTests_four splits four ways, so print_function() emits four op
# columns rather than two (main.cpp loops over perNode[NUM_NUMA_NODES]).
DS4_HEADER = ("Date, Time, DS_Name, Num_DS, Num_Threads, Thread_Config, DS_Config, "
              "Duration, Keyspace, Interval, Ops_Node0, Ops_Node1, Ops_Node2, "
              "Ops_Node3, Total_Ops")


def ycsb_argv(binary, th, ds, p):
    return [binary,
            f"--th_config={th}", f"--DS_config={ds}",
            f"--mix={p['mix']}", f"--hash={p['hash']}", "-z", str(p["theta"]),
            "-t", str(p["threads"]), "-b", str(p["buckets"]), "-a", str(p["tables"]),
            f"--workload={p['workload']}", "-u", str(p["duration"]),
            "-k", str(p["keys"]), "-i", str(p["interval"]), "-p", str(p["payload"]),
            "-W", str(p["warmup"])]


# ------------------------------------------------------- BST (DataStructureTests)
# NOTE: partial -- BST's CSV header has dynamic op columns; fill in when we wire
# BST graphs.  argv mirrors runExperiments.py's meta invocation.
def skew_argv(binary, th, ds, p):
    """DataStructureTests_skew: bst_argv plus the transaction-mix weights.

    --txn_mix=w00,w01,w10,w11 replaces the fixed 25/25/25/25 split that
    DataStructureTests hard-codes via `opDist(gen) % 4`.  The default value
    reproduces that split exactly, so DS_SKEW at 25,25,25,25 should match DS.
    """
    return bst_argv(binary, th, ds, p) + [f"--txn_mix={p['txn_mix']}"]


def bst_argv(binary, th, ds, p):
    # the swept "workload" is the data-structure name for BST
    return [binary,
            "-n", str(p["numDS"]), "-t", str(p["threads"]), "-D", str(p["duration"]),
            f"--DS_name={p['workload']}", f"--th_config={th}", f"--DS_config={ds}",
            "-k", str(p["keys"]), "-i", str(p["interval"])]


BENCHES = {
    "ycsb": {
        "suite":     "ycsb",          # directory numafy.py transforms
        "binary":    "Output/ycsb/bin/ycsb",
        "header":    YCSB_HEADER,
        "argv":      ycsb_argv,
        "workloads": YCSB_WORKLOADS,
        "cwd":       None,
        "params": [
            param("mix",      str,   "uniform", "key distribution: uniform | zipfian"),
            param("hash",     str,   "djb2",    "placement hash: djb2 | mix"),
            param("theta",    float, 0.99,      "zipfian skew exponent (only used when mix=zipfian)"),
            param("payload",  int, 64,        "per-record payload bytes (char* value)"),
            param("warmup",   int, 60,        "untimed warmup seconds before measuring"),
            param("threads",  int, 80,        "worker threads"),
            param("buckets",  int, 133300,    "hash buckets per table"),
            param("tables",   int, 1000,      "number of tables"),
            param("keys",     int, 100000000, "keyspace size"),
            param("duration", int, 1200,      "seconds per config"),
            param("interval", int, 20,        "reporting interval seconds"),
        ],
    },
    # DataStructureTests -- the transactional BST benchmark.  Bench key is "DS",
    # so campaigns land in Campaigns/DS/<slug>/.  Column names deliberately match
    # the ycsb header (Thread_Config / DS_Config / Duration / Total_Ops) so
    # an_comparison.py and campaign_comparison.py work on DS campaigns unchanged.
    # Four logical partitions (thread_numa<0..3>) mapped onto the CPU-bearing
    # nodes by numa_node_map(k) = order[k % order.size()].  On a 2-node machine
    # that is 0,1,0,1 -- i.e. two partitions per physical node, half and half.
    "DS4": {
        "suite":     "DataStructureTests_four",
        "binary":    "Output/DataStructureTests_four/bin/datastructures",
        "header":    DS4_HEADER,
        "argv":      bst_argv,               # identical CLI to DS
        "workloads": ["BinarySearchTree"],
        "cwd":       "Output/DataStructureTests_four",
        "params": [
            # num_DS and num_threads are divided by 4 inside the benchmark, so
            # these are TOTALS, matching DS -- keep threads a multiple of 4.
            param("numDS",    int, 1000000,   "number of data structures (indices), split 4 ways"),
            param("threads",  int, 80,        "worker threads, split 4 ways"),
            param("keys",     int, 80,        "keyspace per tree"),
            param("duration", int, 600,       "seconds per config"),
            param("interval", int, 60,        "reporting interval seconds"),
        ],
    },

    # Same as DS, but the four transaction kinds (0->0, 0->1, 1->0, 1->1) are
    # weighted rather than fixed at 25/25/25/25.  Two independent axes:
    # the cross-node share (w01+w10), and directional asymmetry (w01 vs w10).
    "DS_SKEW": {
        "suite":     "DataStructureTests_skew",
        "binary":    "Output/DataStructureTests_skew/bin/datastructures",
        "header":    DS_HEADER,              # same two op columns as DS
        "argv":      skew_argv,
        "workloads": ["BinarySearchTree"],
        "cwd":       "Output/DataStructureTests_skew",
        "params": [
            param("numDS",    int, 1000000,   "number of data structures (indices)"),
            param("threads",  int, 80,        "worker threads"),
            param("keys",     int, 80,        "keyspace per tree"),
            param("duration", int, 600,       "seconds per config"),
            param("interval", int, 60,        "reporting interval seconds"),
            param("txn_mix",  str, "25,25,25,25",
                  "transaction weights w(0->0),w(0->1),w(1->0),w(1->1); "
                  "relative, need not sum to 100. Default reproduces DS."),
        ],
    },

    "DS": {
        "suite":     "DataStructureTests",   # bench key != suite dir, so numafy
                                             # and Output/ must use this name
        "binary":    "Output/DataStructureTests/bin/datastructures",
        "header":    DS_HEADER,
        "argv":      bst_argv,
        "workloads": ["BinarySearchTree"],   # sweep dimension = data-structure name
        "cwd":       "Output/DataStructureTests",
        "params": [
            # defaults follow paper section 6.3.1: 1M indices x 80 keys (~2 GB),
            # measured over 10 minutes.
            param("numDS",    int, 1000000,   "number of data structures (indices)"),
            param("threads",  int, 80,        "worker threads"),
            param("keys",     int, 80,        "keyspace per tree"),
            param("duration", int, 600,       "seconds per config"),
            param("interval", int, 60,        "reporting interval seconds"),
        ],
    },
}


# -------------------------------------------------- dynamic CLI (used by runners)
def add_bench_args(parser, bench):
    """Add the chosen benchmark's parameters to an argparse parser."""
    for s in BENCHES[bench]["params"]:
        parser.add_argument(f"--{s['name']}", type=s["type"],
                            default=s["default"], help=s["help"])


def extract_params(args, bench):
    """Pull the benchmark's parameter values out of parsed args into a dict."""
    return {s["name"]: getattr(args, s["name"]) for s in BENCHES[bench]["params"]}
 