# clang-tool
clang-tool is a simple and powerful project template for clang-based tools using libtooling[1]. It helps getting started to write standalone tools useful for refactoring, static code analysis, auto-completion etc. In our case we use it to recursivley numa type libraries so they get allocate memory according to the users usage of the thread and numa types

## Building the tool
Install the necessary llvm and clang headers and libraries for you system an run:

```bash
cd numa-clang-tool
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

```bash
cp -rf ../Exprs ./input
sudo run.sh Exprs
```
The ```run.sh``` script has the command on how to run the specified  benchmark in this case ```Exprs```.


## Description
* The compiler binary is in ./build/bin/clang-tool
* The  --numa flag is a pass to recursivley add specialized numa types according to programmer annotaiton
* The --cast flag is a pass to force new allocations in numa typed classes to be on a the desired numa node and finally cast the pointer returned to the appropriate type
