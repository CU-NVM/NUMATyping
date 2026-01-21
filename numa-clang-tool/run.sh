#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <Exprs>"
  exit 1
fi

# Run different commands based on the argument
case "$1" in
  Exprs)
    echo "Running Exprs"
    # Place the actual command for 'command2' here
    # For example: df -h

    ./build/bin/clang-tool  --numa input/Exprs/Examples/main.cpp input/Exprs/Examples/TestSuite.cpp -- -I input/Exprs/include/ -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma -pthread


    ./build/bin/clang-tool  --cast output/Exprs/Examples/main.cpp output/Exprs/Examples/TestSuite.cpp -- -I output/Exprs/include/ -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma -pthread
    ;;

  *)
    echo "Invalid argument. Usage: $0 <DS|dummy|Exprs|STExprs>"
    exit 1
    ;;
esac