#!/bin/bash
#SBATCH --account=m1302
#SBATCH --constraint=cpu
#SBATCH --qos=regular
#SBATCH --time=48:00:00
#SBATCH --nodes=1
#SBATCH --exclusive



# 1. Change to your specific directory
# The job will now execute all subsequent commands from this folder
cd $SCRATCH/NUMATyping

# 2. Load modules
module load python
eval $(python3 load.py)
eval $(python3 env.py)

echo "=== CPU INFO ==="
lscpu


# 3. Verify (Optional, but good for logs)
echo "Working directory is: $(pwd)"


echo "=== STARTING MY SCRIPT ==="
# 4. Run your workload
python3 runExperiments.py --ROOT_DIR=/pscratch/sd/k/kiwo9430/NUMATyping --DS=bst --numafy --AN=1  --UMF --graph --numDS=3000000 --numKeys=240
