1. ~~make benchmark threaded~~
2. ~~have an array of hashtables concurrent hashtable with lock
hash into the hash table and theres a lock per hash table~~  
did not implement exactly like this but seems to work will change later if needed
3. ~~80/20 local vs remote ops~~
4. make ttas lock instead of mutex
5. remove lock from zipfian generator
6. make the generator thread local 
7. copy histogram structure
- all include files go into include directory
- include and src like histogram (what compiler looks for)
- structure file with compile options like histogram main.cpp
- include numalib header files, include everything in histogram includes
- split files into main and ycsb cpps, ycsb will have worker_thread function
8. change strings to c strings
9. also make mt generator thread local (maybe dont need to do this)