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
std::vector<std::mutex*> HashTable_lk0;
std::vector<std::mutex*> HashTable_lk1;

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
    HashTable_lk0.resize(num_threads);
    HashTables1.resize(num_threads);
    HashTable_lk1.resize(num_threads);

    
    for(int i = 0; i < num_threads; i++) {
        if(DS_config == "numa") {
            // For NUMA allocation, create with default constructor
            HashTables0[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 0>(bucket_count));
            HashTables1[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 1>(bucket_count));
            HashTable_lk0[i] = new mutex();
            HashTable_lk1[i] = new mutex();
        }
        else {
            HashTables0[i] = new HashTable(bucket_count);
            HashTables1[i] = new HashTable(bucket_count);
            HashTable_lk0[i] = new mutex();
            HashTable_lk1[i] = new mutex();
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
        HashTable_lk0.resize(num_threads);

        for(int i = 0; i < num_threads; i++) {
            if(DS_config == "numa") {
                HashTables0[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 0>(bucket_count));
                HashTable_lk0[i] = new mutex();
            }
            else {
                HashTables0[i] = new HashTable(bucket_count);
                HashTable_lk0[i] = new mutex();
            }
        }
    }
    else if(node == 1) {
        HashTables1.resize(num_threads);
        HashTable_lk1.resize(num_threads);
        for(int i = 0; i < num_threads; i++) {
            if(DS_config == "numa") {
                HashTables1[i] = reinterpret_cast<HashTable*>(new numa<HashTable, 1>(bucket_count));
                HashTable_lk1[i] = new mutex();
            }
            else {
                HashTables1[i] = new HashTable(bucket_count);
                HashTable_lk1[i] = new mutex();
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
void histogram_test(int tid, int duration, int node, int num_threads, std::string filename) {
    using clock = std::chrono::high_resolution_clock;
    // auto __total_start = clock::now(); 

    #ifdef DEBUG
    if(tid == 1 && node == 0) {
        std::cout << "Only thread " << tid << " will print this." << std::endl;
    }
    #endif

    pthread_barrier_wait(&bar);

    //PHASE 1: Each thread reads its portion of the file

    std::string home = std::getenv("HOME"); 
    std::string file_name = home + "/NUMATyping/book/"+filename;
    std::ifstream file(file_name);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return;
    }
    auto __p1_start = clock::now();
    std::string word;
    while(file >> word) {
        if(node == 0) {
                HashTables0[tid]->insert(word.c_str());
        }
        else { 
                HashTables1[tid]->insert(word.c_str());
        }
    }
    file.close();

    time(tid, "Phase1Lat", __p1_start);

    pthread_barrier_wait(&bar);

    /******************************* PHASE 2: Merging the tables on each node ********************************/

    auto __p2_start = clock::now();

    //Merge the tables on each node into a single table on HashTables0[0] and HashTables1[0]
    if(tid == 0 || tid == num_threads/2) {
        if(node == 0) {
            //for all keys in all hash tables, check to see if key exists in HashTables0[0], if it does, update the count, if not insert the key
            for(int i = 1; i < num_threads; i++) {
                std::vector<char*> keys = HashTables0[i]->getAllKeys();
                for(auto key : keys) {
                    int count = HashTables0[i]->getCount(key);
                    if(HashTables0[0]->exists(key)) {
                        HashTables0[0]->updateCount(key, count);
                    }
                    else {
                        for(int j = 0; j < count; j++) {
                            HashTables0[0]->insert(key);
                        }
                    }
                }
            }
        }
        else {
            for(int i = 1; i < num_threads; i++) {
                std::vector<char*> keys = HashTables1[i]->getAllKeys();
                for(auto key : keys) {
                    int count = HashTables1[i]->getCount(key);
                    if(HashTables1[0]->exists(key)) {
                        HashTables1[0]->updateCount(key, count);
                    }
                    else {
                        for(int j = 0; j < count; j++) {
                            HashTables1[0]->insert(key);
                        }
                    }
                }
            }

        }
    }

    time(tid, "Phase2Lat", __p2_start);

    pthread_barrier_wait(&bar);
  
    /******************************** PHASE 3.1: Merging the tables across nodes using only thread 0 of node 0 *******************************/
    auto __p31_start = clock::now();
    //Only thread 0 of node 0 merges HashTables 1[0] into HashTables0[0]
    if(tid == 0 && node == 0) {
        for(int i = 1; i < num_threads; i++) {
            std::vector<char*> keys = HashTables1[i]->getAllKeys();
            for(auto key : keys) {
                int count = HashTables1[i]->getCount(key);
                if(HashTables0[0]->exists(key)) {
                    HashTables0[0]->updateCount(key, count);
                }
                else {
                    for(int j = 0; j < count; j++) {
                        HashTables0[0]->insert(key);
                    }
                }
            }
        }
    }

    time(tid, "Phase3.1Lat", __p31_start);

    pthread_barrier_wait(&bar);


    std::vector<char*> allKeys = HashTables1[0]->getAllKeys();
    thread_local std::vector<char*> myKeys;

    //divide the keys among threads equally and store them in a thread local vector]
    if(node == 0){
        for(int i = tid; i < allKeys.size(); i += num_threads) {
            myKeys.push_back(allKeys[i]);
        }
    }

    /******************************** PHASE 3.2: Merging the tables across nodes using threads on node 0 ********************************/
    auto __p32_start = clock::now();

    if(node == 0) {
        for(auto key : myKeys) {
            int count = HashTables1[0]->getCount(key);
            if(HashTables0[0]->exists(key)) {
                HashTable_lk0[0]->lock();
                HashTables0[0]->updateCount(key, count);
                HashTable_lk0[0]->unlock();
            }
            else {
                for(int j = 0; j < count; j++) {
                    HashTable_lk0[0]->lock();
                    HashTables0[0]->insert(key);
                    HashTable_lk0[0]->unlock();
                }
            }
        }
    }
    
    time(tid, "Phase3.2Lat", __p32_start);

    pthread_barrier_wait(&bar);

    //divide the keys among threads equally and store them in a thread local vector]
    myKeys.clear();
    for(int i = tid; i < allKeys.size(); i += num_threads*2) {
        myKeys.push_back(allKeys[i]);
    }

    /******************************** PHASE 3.3: Merging the tables across nodes using threads all threads on both nodes ********************************/
    auto __p33_start = clock::now();

    if(node == 0) {
        for(auto key : myKeys) {
            int count = HashTables1[0]->getCount(key);
            if(HashTables0[0]->exists(key)) {
                HashTable_lk0[tid]->lock();
                HashTables0[0]->updateCount(key, count);
                HashTable_lk0[0]->unlock();
            }
            else {
                for(int j = 0; j < count; j++) {
                    HashTable_lk0[0]->lock();
                    HashTables0[0]->insert(key);
                    HashTable_lk0[0]->unlock();
                }
            }
        }
    }
    else {
        for(auto key : myKeys) {
            int count = HashTables1[0]->getCount(key);
            if(HashTables0[0]->exists(key)) {
                HashTable_lk0[0]->lock();
                HashTables0[0]->updateCount(key, count);
                HashTable_lk0[0]->unlock();
            }
            else {
                for(int j = 0; j < count; j++) {
                    HashTable_lk0[0]->lock();
                    HashTables0[0]->insert(key);
                    HashTable_lk0[0]->unlock();
                }
            }
        }
    }

    time(tid, "Phase3.3Lat", __p33_start);
    pthread_barrier_wait(&bar);

    
    // if(tid == 0) {
    //     //print the histogram to std out
    //     HashTables0[0]->printAll();
    // }

}
