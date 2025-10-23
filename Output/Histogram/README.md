# Histogram Test for numa types




## How to run the benchmark
```
    make clean
    make UMF=1
    ./bin/histogram --th_config=regular --DS_config=regular  -f 1 -t 20 --book_title=litrature
```

## How to compile and test the hashtable data structure
```
`clang++ test_hashtable.cpp -I ../include/ -o test
./test`

```

# NOTE
Please include numatype.hpp library in the headers you expect a transformation