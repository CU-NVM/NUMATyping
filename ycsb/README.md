# ycsb implementation for numatyping
Uses zipfian key distribution from zipf_base files and HashTable as database.  
Uses two threads with two HashTables, one is "local" and one is "remote" for each thread.   
Simulates workloads A-F with num of ops, num of keys, theta, HashTable size and locality split decided at command line.  


compile:  
```g++ ycsb_hashtable.cpp -o ycsb_hashtable```

run:  
```./ycsb_hashtable {workload (A,B,C,D,E, or F)} {operations} {keys} {theta} {buckets} {locality (80-20, 50-50, 20-80)}```