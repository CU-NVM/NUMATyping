#include "Array.h"
#include "numatype.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <cstring>
#include <cstdio> 
#include <algorithm> 
#include <shared_mutex>

using namespace std;
using namespace std::chrono;

#define NODE_ZERO 0
#ifndef MAX_NODE 
    #warning "MAX_NODE_ID not defined! Defaulting to 1."
    #define MAX_NODE 1
#endif

// ---------------- INTERNAL STATE ----------------


static char** array_node0;
static char** array_node1;
// Lock-based variant: each read takes a per-node shared lock (read_lk0/1).
// See the array_lk_free benchmark for the lock-free version.

// Synchronization
static std::mutex* globalLK = nullptr;
static pthread_barrier_t bar;
static pthread_barrier_t init_bar;

// Results Storage
static std::vector<int64_t> globalOps0;
static std::vector<int64_t> globalOps1;

static std::shared_mutex* read_lk0;
static std::shared_mutex* read_lk1;

// ---------------- HELPER FUNCTIONS ----------------

void global_init(int num_threads, int duration, int interval) {
    pthread_barrier_init(&bar, NULL, num_threads);
    pthread_barrier_init(&init_bar, NULL, 2);
    read_lk0 = new std::shared_mutex();
    read_lk1 = new std::shared_mutex();
    if (globalLK) delete globalLK;
    globalLK = new std::mutex();
}

size_t get_num_intervals() {
    return globalOps0.size();
}

int64_t get_ops(int node_id, size_t interval_idx) {
    if (node_id == 0) {
        if (interval_idx < globalOps0.size()) return globalOps0[interval_idx];
    } else {
        if (interval_idx < globalOps1.size()) return globalOps1[interval_idx];
    }
    return 0;
}

// ---------------- THREAD FUNCTIONS ----------------

void numa_array_init(int thread_id, int num_total_threads, std::string DS_config, int64_t array_size, int node, int num_arrays, int duration, int interval)
{
    // --- 1. SETUP PHASE ---
    // One result slot per node (the per-interval time series was removed).
    if (node == 0) globalOps0.assign(1, 0);
    else           globalOps1.assign(1, 0);

    pthread_barrier_wait(&init_bar);

    // --- 2. ALLOCATION PHASE ---
    // We allocate arrays of std::atomic<char*> instead of raw char*

    if (DS_config == "regular") {
        if(node == 0)
            array_node0 = new char*[array_size];
        else
            array_node1 = new char*[array_size];
    } else {
        if (node == 0)
            array_node0 = reinterpret_cast<char**>(new numa<char*, NODE_ZERO>[array_size]);
        else
            array_node1 = reinterpret_cast<char**>(new numa<char*, MAX_NODE>[array_size]);
    }

    
    pthread_barrier_wait(&init_bar);

    // --- 3. PRE-FILL PHASE ---
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)) + thread_id);
    std::uniform_int_distribution<int64_t> key_dist(1, array_size-1);
    
    char temp_buf[64];
    

    for(int64_t j = 0; j < array_size; ++j){ 
       if(DS_config == "regular"){
            char* new_word = new char[10];
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            if(node == 0)
                array_node0[j]= new_word;
            else
                array_node1[j]= new_word;
       }
       else if(node == 0){
            char* new_word = reinterpret_cast<char*>(new numa<char, 0>[10]);
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            array_node0[j]= new_word;
       }
       else{
            char* new_word = reinterpret_cast<char*>(new numa<char, MAX_NODE>[10]);
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            array_node1[j]= new_word;
       }
    }
    

    pthread_barrier_wait(&init_bar);
}

void array_test(int tid, int duration, std::string DS_config, int node, int num_threads, int64_t array_size, int num_arrays, int interval)
{
    pthread_barrier_wait(&bar);

    int64_t ops = 0;

    auto startTimer = std::chrono::steady_clock::now();
    auto endTimer   = startTimer + std::chrono::seconds(duration);

    std::mt19937_64 rng(tid);
    std::uniform_int_distribution<long long> word_dist(0, array_size - 1);

    int64_t curr_idx = word_dist(rng);
    volatile char* word;
    while (true) {
        // Dependent read under a per-node shared lock: the pointer read at
        // curr_idx determines the next index, so reads can't be prefetched and
        // the loop is memory-latency bound.
        if (node == 0) {
            read_lk0->lock_shared();
            word = array_node0[curr_idx];
            read_lk0->unlock_shared();
        } else {
            read_lk1->lock_shared();
            word = array_node1[curr_idx];
            read_lk1->unlock_shared();
        }

        // The stored pointer's value drives the walk; jitter breaks up sequential runs.
        int64_t next_idx = reinterpret_cast<int64_t>(word);
        curr_idx = (next_idx + word_dist(rng)) % array_size;
        ops++;

        if (ops > 1000000 && std::chrono::steady_clock::now() >= endTimer)
            break;
    }

    pthread_barrier_wait(&bar);

    // Aggregate this thread's op count into its node's total.
    globalLK->lock();
    if (node == 0) globalOps0[0] += ops;
    else           globalOps1[0] += ops;
    globalLK->unlock();

    pthread_barrier_wait(&bar);
}