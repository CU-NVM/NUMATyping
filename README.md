# NUMA TYPING 
A compiler tool project for recursivley typing NUMA annotated objects for increased local accesses. This repository contains a data structure benchmark which has numa data structures and numa threads that access these data structures. The benchmark reports thread throughput of different configurations of these data structures.


## How to run
1. If you are on the NERSC machines, navigate to the `$SCRATCH` directory. Make sure you have cloned this repo in `$SCRATCH`. If not, stay in `$HOME`

2. If you are not on NERSC machines, skip this step. If you are on the NERSC machines, you will start by loading the python module.Then run these two scripts in order. The first loads necessary modules, and the later adjusts environment variables.
    ```shell
        module load python
        eval $(python3 load.py)
        eval $(python3 env.py)
    ```

3. Navigate to the tool and compile it. 
    ```shell 
        cd numa-clang-tool
        mkdir build
        cd build 
        cmake ..
        cmake --build .
    ```
    NOTE: If for some reason compilation gives you trouble, pass the `-DHELP=ON` command when you run. There are multiple ways to specify common library paths
          If for some reason you have to find dependencies in your system, use `find`. Example usage `$ find /usr -name libLLVM*.so`
3. Navigate to the custom memory allocator and compile it.
    ```shell
        cd unified-memory-framework
        mkdir build
        cd build
        cmake ..
        cmake --build .
    ```

## NUMAFYING

```shell
    python3 numafy.py --ROOT_DIR=[PATH] [SUITE] [OPTIONS]
```
* ROOT_DIR will default to `$HOME/NUMATyping` if not specified.
* SUITE is the benchmarks name i.e. ycsb, Histogram, DataStructures etc
* To see other options and help message run with `--help`

## RUNNING BENCHMARKS

The are multiple ways to run the different benchmarks.
### YCSB
* Running native: 
```shell 
    cd ycsb
    make clean
    make ROOT_DIR=[PATH] UMF=1
    numactl --cpunodebind=0,1 --membind=0,1 ./bin/ycsb --th_config=numa --DS_config=numa -t 40 -b 1333 --w=D -u 120 -k 10000000 --l=80-20 -i 10 -a 1000
```

* Running numafied version (Make sure to run it after NUMAFYING: [Jump to Subtitle](#NUMAFYING)
```shell
    cd Output/ycsb
    make clean
    make ROOT_DIR=[PATH] UMF=1
    numactl --cpunodebind=0,1 --membind=0,1 ./bin/ycsb --th_config=numa --DS_config=numa -t 40 -b 1333 --w=D -u 120 -k 10000000 --l=80-20 -i 10 -a 1000
```
 
 NOTE: There is also a way to run with multiple configurations at once using the meta.py script. 
    Example usage:
    ```shell
        numactl --cpunodebind=0,1 --membind=0,1 ./bin/ycsb --meta th_config:numa:regular --meta DS_config:numa:regular \
        --meta t:80 --meta b:1333 --meta w:D --meta u:30 --meta k:15000000 --meta l:80-20 --meta i:20 --meta a:1000
    ```
    More help can be found running meta.py with `--help`

* Running numafied version with csv files and graphs
```shell
    python3 runYCSB.py --ROOT_DIR=[PATH] --UMF --numafy [OPTIONS]
```
    **`--numafy` is optional and is only there if you want to numafy the native code before you begin. For more opitons, run with --help.

### DATASTRUCTURES
