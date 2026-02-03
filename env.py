import subprocess
import sys
import os

def get_max_numa_node():
    """Detects the maximum NUMA node index available on the system."""
    try:
        # Read the 'online' file which usually returns a range like "0-1" or just "0"
        with open('/sys/devices/system/node/online', 'r') as f:
            content = f.read().strip()
            if '-' in content:
                # Returns the high end of the range (e.g., '1' from '0-1')
                return content.split('-')[-1]
            else:
                # Single node system (e.g., '0')
                return content
    except Exception:
        # Fallback: Default to 0 if sysfs is inaccessible
        return "0"

def get_spack_path(package):
    """Queries spack for the installation prefix with full environment sourcing."""
    cmd = f"spack location -i {package}"
    try:
        return subprocess.check_output(
            cmd, 
            shell=True, 
            executable='/bin/bash', 
            universal_newlines=True,
            stderr=subprocess.DEVNULL
        ).strip()
    except Exception:
        return None

def get_clang_bin_path():
    """Detects the current Clang binary directory using the resource-dir."""
    try:
        resource_dir = subprocess.check_output(
            "clang++ -print-resource-dir", 
            shell=True, 
            universal_newlines=True,
            stderr=subprocess.DEVNULL
        ).strip()
        bin_path = os.path.abspath(os.path.join(resource_dir, "..", "..", "..", "bin"))
        if os.path.exists(os.path.join(bin_path, "clang++")):
            return bin_path
        return None
    except Exception:
        return None

def main():
    exports = []
    
    # 1. Resolve JEMALLOC path via Spack
    jemalloc_path = get_spack_path("jemalloc")
    if jemalloc_path:
        exports.append(f"export JEMALLOC_ROOT='{jemalloc_path}'")
    else:
        sys.stderr.write("Warning: jemalloc not found via spack.\n")
        
    # 2. Resolve Compiler paths dynamically
    clang_bin = get_clang_bin_path()
    if clang_bin:
        exports.append(f"export CXX='{clang_bin}/clang++'")
        exports.append(f"export CC='{clang_bin}/clang'")
    
    # 3. Detect and export NUMA Topology
    max_node = get_max_numa_node()
    exports.append(f"export MAX_NODE_ID={max_node}")
    
    # 4. Add any other project-specific variables
    exports.append("export BUILD_TYPE=Release")
    
    # Output the string for the shell to 'eval'
    if exports:
        print(" ; ".join(exports))

if __name__ == "__main__":
    main()
