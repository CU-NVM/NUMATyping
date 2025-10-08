# NUMA TYPING 
* **Adjust HOME_DIR variable to your appropriate home directory in ```Exprs/Examples/Makefile```**
* **Adjust the clang version in ```numa-clang-tool/run.sh``` (Currently version 21)**



A compiler tool project for recursivley typing NUMA annotated objects for increased local accesses. This repository contains a data structure benchmark which has numa data structures and numa threads that access these data structures. The benchmark reports thread throughput of different configurations of these data structures.


## How to compile

```bash
cd numa-clang-tool

mkdir build 

cd build 

cmake ..

cmake --build .

cd ../../
```

This compiles the clang tool that is used to compile a source code.

```bash
cd unified-memory-framework

mkdir build

cd build 

sudo cmake ..

sudo cmake --build .

cd ../../
```
This compiles the custom numa allocator 

## How to run

1. **To run on a machine with two numa nodes,**

```sudo python3 runExperiments.py Exprs --DS=bst --UMF```

- runExperiments.py : python script to run desired data structures.
- Exprs: benchmark name in the main directory.
- DS=bst : type of data structure to run.
- UMF : type of numa allocator to use
- verbose (optional) : output result on command line

This command copies the Exprs benchmark into the ```numa-clang-tool``` folder, compiles it using the script under ```numa-clang-tool```(see readme under ```numa-clang-tool```), copies the numa-fied data structure back under the ```Output``` directory, compiles it using the make file and runs the experiment (see readme under ```Exprs```) finally writing the result in ```Result/BST_Transactions.csv``` directory.


2. **To run on a machine with more than two numa nodes (stormbreaker server),**

*to-do: make this part of the runExperiments.py script

```bash
./numafy [BENCHMARK] [CLANG-VERSION]

cd Output/[BENCHMARK]

make UMF=1 

numactl --cpunodebind=0,1 --membind=0,1 python3 meta.py ./Examples/bin/DSExample --meta n:1000000 --meta t:40:80 --meta D:800 --meta DS_name:bst --meta th_config:numa:regular:reverse --meta DS_config:numa:regular --meta k:160  --meta i:10 >> ../../Result/BST_Transactions.csv
```


This command copies the Exprs benchmark into the ```numa-clang-tool``` folder, compiles it using the script under ```numa-clang-tool```(see readme under ```numa-clang-tool```), copies the numa-fied data structure back under the ```Output``` directory, compiles it using the make file and runs the experiment (see readme under ```Exprs```) finally writing the result in ```Result/BST_Transactions.csv``` directory.



The first three arguments of the last command bind the process to be run on just two nodes since the Exprs benchmark only pins threads and data structures on two nodes. The meta script is part of the benchmark and is used to run different combinations of options for the benchmark (see readme under ```Exprs```). 
    

3. **To run a single configuration of all options (no cross product of options)**

```bash
cp -rf Exprs numa-clang-tool/input

cd numa-clang-tool

sudo ./run.sh Exprs

cp -rf output2/Exprs ../Output

cd ../Output/Exprs

sudo make UMF=1 

./Examples/bin/DSExample -n 10240 -t 40 -D 20 --DS_name=bst --th_config=numa --DS_config=numa -k 160 -i 10
```

### Result 
The result is a csv file with different fields. 

- DS_name: type of data structure run
- num_DS: numbers of data structures created
- num_threads: numbers of threads run 
- duration: time(sec) at which the corresponding throughput is recorded (time would be a better label)
- thread_config and DS_config : see paper (Section 6.3)
- keyspace: keyspace for binary search tree data structure
- interval: the interval between two successive duration values 
- Op0, Op1 and TotalOps: througput of threads on node 0, node 1 and total throughput.

(see readme in ```Result``` to understand more types of results on number of accesses etc.)

