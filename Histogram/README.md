# Histogram Test for numa types




## How to run the benchmark
```
    make clean
    make UMF=1
    ./bin/histogram --th_config=regular --DS_config=regular -D 10 -f 1 -t 20 -i 10 --book_title=paper
```

## How to compile and test the hashtable data structure
```
`clang++ test_hashtable.cpp -I ../include/ -o test
./test`

```