#!/usr/bin/env python3
import argparse
import os
import shutil
import sys
import subprocess

# ============================================================================
# Help Function
# ============================================================================

def show_help():
    help_text = """
NUMA-aware C++ Transformation Tool
Author: Kidus Workneh

USAGE:
    python3 numafy.py --ROOT_DIR=[PATH] [SUITE] [OPTIONS]

ARGUMENTS:
    SUITE           Target suite (Histogram, DataStructureTests, ycsb)

OPTIONS:
    --ROOT_DIR      Path to the NUMATyping root directory
    --umf           UMF support (1=on, 0=off, default: 1)
    --debug         GDB Debug mode (1=on, 0=off, default: 0)
    --help          Show this help message

EXAMPLE:
    python3 numafy.py --ROOT_DIR=$SCRATCH/NUMATyping ycsb
"""
    print(help_text)
    sys.exit(0)

# ============================================================================
# Detection Helpers
# ============================================================================

def get_spack_path(package):
    cmd = "source /etc/profile.d/modules.sh && module load spack && spack location -i " + package
    try:
        return subprocess.check_output(cmd, shell=True, executable='/bin/bash', 
                                       universal_newlines=True, stderr=subprocess.DEVNULL).strip()
    except: return None

def get_clang_include_path():
    try:
        res = subprocess.check_output("clang++ -print-resource-dir", shell=True, universal_newlines=True).strip()
        return os.path.join(res, "include")
    except: return None

# ============================================================================
# Main Script
# ============================================================================

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        show_help()

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("suite", nargs="?", default="Histogram")
    parser.add_argument("--ROOT_DIR", required=True)
    parser.add_argument("--umf", type=int, default=1)
    parser.add_argument("--debug", type=int, default=0)
    parser.add_argument("--clang-includes")
    parser.add_argument("--jemalloc-root")

    try:
        args = parser.parse_args()
    except:
        show_help()

    # --- Direct Path Setup ---
    ROOT_DIR = os.path.abspath(args.ROOT_DIR)
    SUITE = args.suite
    SRC_PATH = f"{ROOT_DIR}/{SUITE}"
    
    if not os.path.exists(SRC_PATH):
        print(f"Error: Source {SRC_PATH} not found!")
        sys.exit(1)
        
    # --- Retrieve MAX_NODE_ID from Environment ---
    # This assumes you ran 'eval $(python3 env.py)' before this script
    MAX_NODE_ID = os.environ.get("MAX_NODE_ID", "0")
    print(f"--- Configuration Detected: MAX_NODE_ID={MAX_NODE_ID} ---")

    # Tool and Working Dirs
    TOOL_BIN = f"{ROOT_DIR}/numa-clang-tool/build/bin/clang-tool"
    INPUT_DIR = f"{ROOT_DIR}/numa-clang-tool/input/{SUITE}"
    OUTPUT_DIR = f"{ROOT_DIR}/numa-clang-tool/output/{SUITE}"
    OUTPUT2_DIR = f"{ROOT_DIR}/numa-clang-tool/output2/{SUITE}"

    # --- System Detection ---
    CLANG_INC = args.clang_includes or get_clang_include_path() or "/global/common/software/nersc/pe/gpu/llvm/20.1.3/lib/clang/20/include/"
    JEMALLOC = args.jemalloc_root or get_spack_path("jemalloc") or "/global/homes/k/kiwo9430/.spack/opt/spack/linux-zen3/jemalloc-5.3.0-rd7q2icc4g7g2bsbdhaxbbv4wkdug7w3"

    # ============================================================================
    # 1. Directory Setup
    # ============================================================================
    print(f"--- Preparing working directories for {SUITE} ---")
    for d in [INPUT_DIR, OUTPUT_DIR, OUTPUT2_DIR]:
        if os.path.exists(d): shutil.rmtree(d)
        os.makedirs(os.path.dirname(d), exist_ok=True)
        shutil.copytree(SRC_PATH, d)

    # ============================================================================
    # 2. Flag Configuration
    # ============================================================================
    UMF_FLAGS = ""
    if args.umf == 1:
        U = f"{ROOT_DIR}/unified-memory-framework"
        UMF_FLAGS = (f"-I{U}/src/utils -I{U}/include -I{U}/examples/common -I{U}/src "
                     f"-I{U}/src/ravl -I{U}/src/critnib -I{U}/src/provider "
                     f"-I{U}/src/memspaces -I{U}/src/memtargets -DUMF "
                     f"-lhwloc -lrt -ldl -ljemalloc {U}/build/lib/libumf.a {U}/build/lib/libjemalloc_pool.a")

    # Inject MAX_NODE_ID directly into BASE_FLAGS here
    BASE_FLAGS = f"-I{ROOT_DIR}/numaLib/ -I{CLANG_INC} -I{JEMALLOC}/include -lnuma -pthread -DMAX_NODE={MAX_NODE_ID} {UMF_FLAGS}"

    # ============================================================================
    # 3. Execution
    # ============================================================================
    suite_map = {
        "Histogram": "main.cpp,Histogram.cpp",
        "DataStructureTests": "main.cpp,TestSuite.cpp",
        "ycsb": "main.cpp,ycsb_benchmark.cpp"
    }
    
    FILES = suite_map.get(SUITE)
    if not FILES:
        print(f"Unknown suite: {SUITE}"); sys.exit(1)

    def run_pass(pass_name, work_dir):
        # Format the file list correctly for the tool
        file_list = ",".join([f"{work_dir}/src/{f}" for f in FILES.split(",")])
        
        # NOTE: removed -DMAX_NODE=$MAX_NODE_ID from here because it's now in BASE_FLAGS
        cmd = (f"{TOOL_BIN} --pass={pass_name} --input={file_list} dummy.cpp -- "
               f"-I {work_dir}/include/ -I {work_dir}/util/ {BASE_FLAGS}")
        
        final_cmd = cmd.split()
        if args.debug == 1: final_cmd = ["gdb", "--args"] + final_cmd
        
        print(f"\n--- Running {pass_name.upper()} ---")
        subprocess.run(final_cmd, check=True)

    # Run Passes
    run_pass("recurse", INPUT_DIR)
    
    if os.path.exists(OUTPUT2_DIR): shutil.rmtree(OUTPUT2_DIR)
    shutil.copytree(OUTPUT_DIR, OUTPUT2_DIR)
    
    run_pass("cast", OUTPUT_DIR)

    # ============================================================================
    # 4. Finalize
    # ============================================================================
    FINAL_DEST = f"{ROOT_DIR}/Output/{SUITE}"
    if os.path.exists(FINAL_DEST): shutil.rmtree(FINAL_DEST)
    os.makedirs(os.path.dirname(FINAL_DEST), exist_ok=True)
    shutil.copytree(OUTPUT2_DIR, FINAL_DEST)

    print(f"\nSUCCESS: Transformed code in {FINAL_DEST}")

if __name__ == "__main__":
    main()
