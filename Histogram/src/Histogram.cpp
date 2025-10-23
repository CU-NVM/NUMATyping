/*! \file Histogram.cpp
 * \brief Word Histogram test implementation for parallel NUMA-aware testing
 * \date 2025

 */

#include "Histogram.hpp"
#include "HashTable.hpp"
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <syncstream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <atomic>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "timing.h"
#include "numatype.hpp"
#include "numathreads.hpp"
// #include "umf_numa_allocator.hpp"


using namespace std::chrono;    


// Global variables for HashTable testing (following BST patterns)
std::vector<HashTable*> HashTables0;
std::vector<HashTable*> HashTables1;
std::vector<std::mutex> HashTable_lk0;
std::vector<std::mutex> HashTable_lk1;
std::mutex global_lk;
std::vector<const char*> allKeys;

pthread_barrier_t bar;
pthread_barrier_t init_bar;

int read_op0 = 0;
int read_op1 = 0;




void global_init(int num_threads, int duration, int interval){
	pthread_barrier_init(&bar, NULL, num_threads);
	pthread_barrier_init(&init_bar, NULL, 2);
}


/*!
 * \brief Initialize HashTables for NUMA-aware testing (single-threaded)
 * 
 * \param[in] DS_config Configuration ("numa" or "regular")
 * \param[in] num_threads Number of threads
 * \param[in] bucket_count Number of buckets in each hash table
 */

void numa_histogram_single_init(int num_threads, std::string DS_config, int bucket_count) {
    HashTables0.resize(num_threads);
    HashTable_lk0= std::vector<std::mutex>(num_threads);
    HashTables1.resize(num_threads);
    HashTable_lk1= std::vector<std::mutex>(num_threads);

    
    for(int i = 0; i < num_threads; i++) {
        if(DS_config == "numa") {
            // For NUMA allocation, create with default constructor
            HashTables0[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 0>(bucket_count));
            HashTables1[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 1>(bucket_count));
            // HashTable_lk0[i] = new mutex();
            // HashTable_lk1[i] = new mutex();
        }
        else {
            HashTables0[i] = new HashTable(bucket_count);
            HashTables1[i] = new HashTable(bucket_count);
            // HashTable_lk0[i] = new mutex();
            // HashTable_lk1[i] = new mutex();
        }
    }
}

/*!
 * \brief Initialize HashTables for NUMA-aware testing
 * 
 * \param[in] DS_config Configuration ("numa" or "regular")
 * \param[in] bucket_count Number of buckets in each hash table
 * \param[in] node NUMA node (for compatibility)
 */

void numa_histogram_init(int num_threads, std::string DS_config, int bucket_count, int node) {
    pthread_barrier_wait(&init_bar);

    if(node == 0) {
        HashTables0.resize(num_threads);
        HashTable_lk0= std::vector<std::mutex>(num_threads);

        for(int i = 0; i < num_threads; i++) {
            if(DS_config == "numa") {
                HashTables0[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 0>(bucket_count));
                // HashTable_lk0[i] = new mutex();
            }
            else {
                HashTables0[i] = new HashTable(bucket_count);
                // HashTable_lk0[i] = new mutex();
            }
        }
    }
    else if(node == 1) {
        HashTables1.resize(num_threads);
        HashTable_lk1= std::vector<std::mutex>(num_threads);
        for(int i = 0; i < num_threads; i++) {
            if(DS_config == "numa") {
                HashTables1[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 1>(bucket_count));
                // HashTable_lk1[i] = new mutex();
            }
            else {
                HashTables1[i] = new HashTable(bucket_count);
                // HashTable_lk1[i] = new mutex();
            }
        }
    }

    pthread_barrier_wait(&init_bar);
}

/*!
 * \brief HashTable parallel test function (Word Histogram)
 * 
 * This function performs parallel operations on word histograms following
 * the same patterns as BinarySearchTest but adapted for word operations.
 * 
 * \param[in] tid Thread ID
 * \param[in] duration Test duration in seconds
 * \param[in] node NUMA node (0 or 1)
 * \param[in] num_DS Number of data structures per node
 * \param[in] num_threads Number of threads per node
 * \param[in] crossover Percentage for cross-node operations
 * \param[in] keyspace Range for word selection (maps to sample word indices)
 * \param[in] interval Interval for logging operations
 */

void phase_one(int tid, int duration, int node, int num_threads, std::string filename)
{
    using clock = std::chrono::high_resolution_clock;
    //PHASE 1: Each thread reads its portion of the file

    std::string home = std::getenv("HOME"); 
    std::string file_name = home + "/NUMATyping/book/"+filename;
    std::ifstream file(file_name);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return;
    }

    #ifdef PER_THREAD_TIMING
        auto __p1_start = clock::now();
    #endif
    auto phase1_start = clock::now();
    thread_local int size = 0;
    std::string word;
    while(file >> word) {
        if(node == 0) {
                HashTables0[tid]->insert(word.c_str());
        }
        else { 
                HashTables1[(tid-num_threads)]->insert(word.c_str());
        }
    }
    

    #ifdef PER_THREAD_TIMING
        time(tid, "Phase1Lat", __p1_start);
    #endif

    pthread_barrier_wait(&bar);
    if(tid == 0 && node == 0) {
        stopwatch("Phase1Total", phase1_start);
    }
    pthread_barrier_wait(&bar);

    file.clear();                      // clear EOF/fail bits
    file.seekg(0, std::ios::beg);   
    global_lk.lock();
   // rewind to beginning
    while(file >> word) {
        allKeys.push_back(word.c_str());
    }
    global_lk.unlock();
    file.close();  
    pthread_barrier_wait(&bar);
}

void phase_two(int tid, int duration, int node, int num_threads, std::string filename) {
    using clock = std::chrono::high_resolution_clock;
    /******************************* PHASE 2: Merging the tables on each node ********************************/

    #ifdef PER_THREAD_TIMING
        auto __p2_start = clock::now();
    #endif
    auto phase2_start = clock::now();
    
    //Merge the tables on each node into a single table on HashTables0[0] and HashTables1[0]
    if(tid == 0 || tid == num_threads) {
        if(node == 0) {
            //for all keys in all hash tables, check to see if key exists in HashTables0[0], if it does, update the count, if not insert the key
            for(int i = 1; i < num_threads; i++) {
                std::vector<const char*> keys = HashTables0[i]->getAllKeys();
                for(auto key : keys) {
                    int count = HashTables0[i]->getCount(key);
                    if(HashTables0[0]->exists(key)) {
                        HashTables0[0]->updateCount(key, count);
                    }
                    else {
                        HashTables0[0]->insert(key, count);
                    }
                }
            }
        }
        else {
            for(int i = 1; i < num_threads; i++) {
                std::vector<const char*> keys = HashTables1[i]->getAllKeys();
                for(auto key : keys) {
                    int count = HashTables1[i]->getCount(key);
                    if(HashTables1[0]->exists(key)) {
                        HashTables1[0]->updateCount(key, count);
                    }
                    else {
                        HashTables1[0]->insert(key, count);
                    }
                }
            }

        }
    }

    #ifdef PER_THREAD_TIMING
        time(tid, "Phase2Lat", __p2_start);
    #endif

    pthread_barrier_wait(&bar);
    if(tid == 0 && node == 0) {
        stopwatch("Phase2Total", phase2_start);
    }
    pthread_barrier_wait(&bar);
}

void single_thread_merge(int tid, int duration, int node, int num_threads, std::string filename, std::string merge_strategy) {
    using clock = std::chrono::high_resolution_clock;
    /******************************** PHASE 3.1: Merging the tables across nodes using only thread 0 of node 0 *******************************/
    #ifdef PER_THREAD_TIMING
        auto __p31_start = clock::now();
    #endif
    auto phase31_start = clock::now();

    //Only thread 0 of node 0 merges HashTables1[0] into HashTables0[0]
    if(tid == 0 && node == 0) {
        std::vector<const char*> keys = HashTables1[0]->getAllKeys();
        for(auto key : keys) {
            int count = HashTables1[0]->getCount(key);
            if(HashTables0[0]->exists(key)) {
                HashTables0[0]->updateCount(key, count);
            }
            else {
                HashTables0[0]->insert(key, count);
            }
        }
    }

    #ifdef PER_THREAD_TIMING
        time(tid, "Phase3.1Lat", __p31_start);
    #endif

    pthread_barrier_wait(&bar);
    if(tid == 0 && node == 0) {
        stopwatch("Phase3.1Total", phase31_start);
    }
    pthread_barrier_wait(&bar);
}

void single_node_merge(int tid, int duration, int node, int num_threads, std::string filename, std::string merge_strategy){
    using clock = std::chrono::high_resolution_clock;

    /******************************** PHASE 3.2: Merging the tables across nodes using all threads on node 0 ********************************/
    thread_local std::vector<const char*> myKeys;
    myKeys.clear();

    pthread_barrier_wait(&bar);
    if (tid == 0 && node == 0) {
        allKeys.clear();
        allKeys = HashTables1[0]->getAllKeys();   // DO NOT free these later
    }

    pthread_barrier_wait(&bar);

    if (node == 0) {
        for (int i = tid; i < (int)allKeys.size(); i += num_threads) {
            myKeys.push_back(allKeys[i]);
        }
    }

#ifdef PER_THREAD_TIMING
    auto __p32_start = clock::now();
#endif
    auto phase32_start = clock::now();

    // Cross-node merge: read from node1, add to node0
    if (node == 0) {
        for (const char* key : myKeys) {
            int count = 0;
            HashTable_lk1[0].lock();
            count = HashTables1[0]->getCount(key);  // returns 0 if absent
            HashTable_lk1[0].unlock();

            if (count > 0) {
                HashTable_lk0[0].lock();
                if (!HashTables0[0]->updateCount(key, count)) {
                    HashTables0[0]->insert(key, count);
                }
                HashTable_lk0[0].unlock();
            }
        }
    }

#ifdef PER_THREAD_TIMING
    time(tid, "Phase3.2Lat", __p32_start);
#endif

    pthread_barrier_wait(&bar);
    if (tid == 0 && node == 0) {
        stopwatch("Phase3.2Total", phase32_start);
    }
    pthread_barrier_wait(&bar);

    myKeys.clear();
    if (tid == 0 && node == 0) {
        allKeys.clear();  
    }

    pthread_barrier_wait(&bar);
}

void multi_node_merge(int tid, int duration, int node, int num_threads, std::string filename, std::string merge_strategy) {
    using clock = std::chrono::high_resolution_clock;

    /********** PHASE 3.3: merge node1 -> node0 using threads on BOTH nodes **********/

    // Thread-local shard (clear every call so we don't accumulate across runs)
    thread_local std::vector<const char*> myKeys2;
    myKeys2.clear();
    
    // 1) Build one snapshot of keys from node1 (leader only), then sync.
    pthread_barrier_wait(&bar);
    if (node == 0 && tid == 0) {
        allKeys.clear();
        allKeys = HashTables1[0]->getAllKeys();
    }
    pthread_barrier_wait(&bar); // all threads can now read allKeys


    const int start = tid + node * num_threads;
    const int step  = num_threads * 2;
    for (int i = start; i < (int)allKeys.size(); i += step) {
        myKeys2.push_back(allKeys[i]);
    }
    pthread_barrier_wait(&bar);

#ifdef PER_THREAD_TIMING
    auto __p33_start = clock::now();
#endif
    auto phase33_start = clock::now();

    for (const char* key : myKeys2) {
        int count = 0;
        HashTable_lk1[0].lock();
        count = HashTables1[0]->getCount(key);
        HashTable_lk1[0].unlock();

        if (count > 0) {
            HashTable_lk0[0].lock();
            if (!HashTables0[0]->updateCount(key, count)) {
                HashTables0[0]->insert(key, count);
            }
            HashTable_lk0[0].unlock();
        }
    }

#ifdef PER_THREAD_TIMING
    time(tid, "Phase3.3Lat", __p33_start);
#endif

    pthread_barrier_wait(&bar);
    if (node == 0 && tid == 0) {
        stopwatch("Phase3.3Total", phase33_start);
    }
    pthread_barrier_wait(&bar);

    myKeys2.clear();       
    if (node == 0 && tid == 0) {
        allKeys.clear();  
    }

    pthread_barrier_wait(&bar);
}

void delete_HashTables(int num_threads) {
    for(int i = 0; i < num_threads; i++) {
        delete HashTables0[i];
        delete HashTables1[i];
    }
    HashTables0.clear();
    HashTables1.clear();
}

void histogram_test(int tid, int duration, std::string DS_config, int node, int num_threads, std::string filename, std::string merge_strategy, int bucket_count) {

    // auto __total_start = clock::now(); 
    pthread_barrier_wait(&bar);
    #ifdef DEBUG
    if(tid == 1 && node == 0) {
        std::cout << "Only thread " << tid << " will print this." << std::endl;
    }
    #endif
    phase_one(tid, duration, node, num_threads, filename);
    phase_two(tid, duration, node, num_threads, filename);
    pthread_barrier_wait(&bar);
    if(merge_strategy == "SINGLE_THREAD") {
        single_thread_merge(tid, duration, node, num_threads, filename, merge_strategy);
    }
    else if(merge_strategy == "SINGLE_NODE") {
        single_thread_merge(tid, duration, node, num_threads, filename, merge_strategy);
        single_node_merge(tid, duration, node, num_threads, filename, merge_strategy);
    }
    else if(merge_strategy == "MULTI_NODE") {
        single_thread_merge(tid, duration, node, num_threads, filename, merge_strategy);
        multi_node_merge(tid, duration, node, num_threads, filename, merge_strategy);
    }
    else if(merge_strategy == "ALL"){
        //Run single_thread_merge
        single_thread_merge(tid, duration, node, num_threads, filename, merge_strategy);
        pthread_barrier_wait(&bar);
        //Clear tables, reinitialize and run single_node_merge
        if(tid == 0){
            delete_HashTables(num_threads);
        }
        pthread_barrier_wait(&bar);
        if(tid == 0 || tid ==num_threads){
            numa_histogram_init(num_threads, DS_config, bucket_count, node);
        }
        pthread_barrier_wait(&bar);
        phase_one(tid, duration, node, num_threads, filename);
        phase_two(tid, duration, node, num_threads, filename);
        single_node_merge(tid, duration, node, num_threads, filename, merge_strategy);
        pthread_barrier_wait(&bar);
        // if(tid == 0 && node == 0) {
            //print_wordcounts();
        // }  
        //Clear tables, reinitialize and run multi_node_merge
        if(tid == 0){
            delete_HashTables(num_threads);
        }
        pthread_barrier_wait(&bar);
        if(tid == 0 || tid ==num_threads){
            numa_histogram_init(num_threads, DS_config, bucket_count, node);
        }
        pthread_barrier_wait(&bar);
        phase_one(tid, duration, node, num_threads, filename);
        phase_two(tid, duration, node, num_threads, filename);
        multi_node_merge(tid, duration, node, num_threads, filename, merge_strategy);
        pthread_barrier_wait(&bar);
    }
    else{}
    //Clear tables, reinitialize and run multi_node_merge
    // if(tid == 0){
    //     delete_HashTables(num_threads);
    // }
    //Print
    pthread_barrier_wait(&bar);
    if(tid == 0 && node == 0) {
        //print_wordcounts();
    }  
    pthread_barrier_wait(&bar);

}

void print_wordcounts(){
    size_t total_tokens1 = 0;
        auto keys1 = HashTables0[0]->getAllKeys();
        for (auto k : keys1) {
            total_tokens1 += HashTables0[0]->getCount(k);
        }
        std::cout << "unique=" << keys1.size()
                << " total_tokens=" << total_tokens1 << "\n";

        size_t total_tokens2 = 0;
        auto keys2 = HashTables1[0]->getAllKeys();
        for (auto k : keys2) {
            total_tokens2 += HashTables1[0]->getCount(k);
        }
        std::cout << "unique=" << keys2.size()
                << " total_tokens=" << total_tokens2 << "\n";
}

void global_cleanup(int num_threads, int duration, int interval){
    
    auto safe_delete_tables = [](std::vector<HashTable*>& v) {
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i]) {
                delete v[i];       // or your destroy_on_node(...) if NUMA placement-new
                v[i] = nullptr;
            }
        }
        v.clear();
    };
    HashTable_lk0.clear();
    HashTable_lk1.clear();

    // IMPORTANT: iterate by v.size(), not by num_threads
    safe_delete_tables(HashTables0);
    safe_delete_tables(HashTables1);
    pthread_barrier_destroy(&bar);
    pthread_barrier_destroy(&init_bar);
}
