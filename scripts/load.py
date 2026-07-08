# load.py
import sys

def get_modules():
    # 1. Define the modules to load
    modules = ["PrgEnv-llvm", "spack", "cmake"]
    
    # 2. Build the command chain
    # We load the modules, then activate the specific spack environment
    commands = [
        "module load {0}".format(' '.join(modules)),
        "spack env activate NUMATyping"
    ]
    
    return " ; ".join(commands)

if __name__ == "__main__":
    print(get_modules())
