# ycsb stuff
## ycsb implementation for numatyping
Uses zipfian key distribution from zipf_base files and HashTable as database  
Simulates workloads A-F with num of ops, num of keys, theta and HashTable size decided at command line

compile:
```g++ ycsb_hashtable.cpp -o ycsb_hashtable```

run:
```./ycsb_hashtable {workload (A,B,C,D,E, or F)} {operations} {keys} {theta} {buckets}```