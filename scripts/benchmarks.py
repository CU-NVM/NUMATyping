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
               "Buckets, Workload, Duration, Num_Keys, Interval, "
               "Ops_Node0, Ops_Node1, Total_Ops")

YCSB_WORKLOADS = [
    "A-50-50-50,A-100-0-50", "B-50-50-50,B-100-0-50", "C-50-50-50,C-100-0-50",
    "D-50-50-50,D-100-0-50", "E-50-50-50,E-100-0-50", "F-50-50-50,F-100-0-50",
    "A-50-50-50,D-100-0-50",
]

def ycsb_argv(binary, th, ds, p):
    return [binary,
            f"--th_config={th}", f"--DS_config={ds}",
            f"--mix={p['mix']}", f"--hash={p['hash']}",
            "-t", str(p["threads"]), "-b", str(p["buckets"]), "-a", str(p["tables"]),
            f"--w={p['workload']}", "-u", str(p["duration"]),
            "-k", str(p["keys"]), "-i", str(p["interval"]), "-p", str(p["payload"])]


# ------------------------------------------------------- BST (DataStructureTests)
# NOTE: partial -- BST's CSV header has dynamic op columns; fill in when we wire
# BST graphs.  argv mirrors runExperiments.py's meta invocation.
def bst_argv(binary, th, ds, p):
    # the swept "workload" is the data-structure name for BST
    return [binary,
            "-n", str(p["numDS"]), "-t", str(p["threads"]), "-D", str(p["duration"]),
            f"--DS_name={p['workload']}", f"--th_config={th}", f"--DS_config={ds}",
            "-k", str(p["keys"]), "-i", str(p["interval"])]


BENCHES = {
    "ycsb": {
        "binary":    "Output/ycsb/bin/ycsb",
        "header":    YCSB_HEADER,
        "argv":      ycsb_argv,
        "workloads": YCSB_WORKLOADS,
        "cwd":       None,
        "params": [
            param("mix",      str, "uniform", "key distribution: uniform | zipfian"),
            param("hash",     str, "djb2",    "placement hash: djb2 | mix"),
            param("payload",  int, 64,        "per-record payload bytes (char* value)"),
            param("threads",  int, 80,        "worker threads"),
            param("buckets",  int, 133300,    "hash buckets per table"),
            param("tables",   int, 1000,      "number of tables"),
            param("keys",     int, 100000000, "keyspace size"),
            param("duration", int, 1200,      "seconds per config"),
            param("interval", int, 20,        "reporting interval seconds"),
        ],
    },
    "bst": {
        "binary":    "Output/DataStructureTests/bin/datastructures",
        "header":    None,          # TODO: dynamic op columns; set when wiring BST
        "argv":      bst_argv,
        "workloads": ["BinarySearchTree"],   # sweep dimension = data-structure name
        "cwd":       "Output/DataStructureTests",
        "params": [
            param("numDS",    int, 1000,      "number of data structures"),
            param("threads",  int, 80,        "worker threads"),
            param("keys",     int, 100000000, "keyspace size"),
            param("duration", int, 1200,      "seconds per config"),
            param("interval", int, 20,        "reporting interval seconds"),
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
 