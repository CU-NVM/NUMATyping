#include "Array.h"
#include "numatype.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <atomic> // REQUIRED for Zero-Contention
#include <cstring>
#include <cstdio> 
#include <algorithm> 

using namespace std;
using namespace std::chrono;

#define NODE_ZERO 0
#ifndef MAX_NODE 
    #warning "MAX_NODE_ID not defined! Defaulting to 1."
    #define MAX_NODE 1
#endif

// ---------------- INTERNAL STATE ----------------

// Changed to Atomic pointers to allow lock-free access
std::atomic<char*>* array_node0;
std::atomic<char*>* array_node1;

// Locks removed entirely - we rely on atomic hardware operations now

// Synchronization
static std::mutex* globalLK = nullptr;
static pthread_barrier_t bar;
static pthread_barrier_t init_bar;

// Results Storage
static std::vector<int64_t> globalOps0;
static std::vector<int64_t> globalOps1;

// ---------------- HELPER FUNCTIONS ----------------

void global_init(int num_threads, int duration, int interval) {
    pthread_barrier_init(&bar, NULL, num_threads);
    pthread_barrier_init(&init_bar, NULL, 2);
    
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
    size_t num_intervals = (interval > 0) ? (duration / interval) : 1;
    if (node == 0) {
        globalOps0.assign(num_intervals, 0);
    } else {
        globalOps1.assign(num_intervals, 0);
    }

    pthread_barrier_wait(&init_bar);

    // --- 2. ALLOCATION PHASE ---
    // We allocate arrays of std::atomic<char*> instead of raw char*

    if (DS_config == "regular") {
        if(node == 0)
            array_node0 = new std::atomic<char*>[array_size];
        else
            array_node1 = new std::atomic<char*>[array_size];
    } else {
        if (node == 0)
            array_node0 = reinterpret_cast<std::atomic<char*>*>(new numa<std::atomic<char*>, NODE_ZERO>[array_size]);
        else
            array_node1 = reinterpret_cast<std::atomic<char*>*>(new numa<std::atomic<char*>, MAX_NODE>[array_size]);
    }

    
    pthread_barrier_wait(&init_bar);

    // --- 3. PRE-FILL PHASE ---
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)) + thread_id);
    std::uniform_int_distribution<int64_t> key_dist(1, std::max((int64_t)1, array_size / 100));
    
    char temp_buf[64];

    for(int64_t j = 0; j < array_size; ++j){ 
        if(DS_config == "regular"){
            char* new_word = new char[10];
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            if(node == 0)
                array_node0[j].store(new_word, std::memory_order_relaxed);
            else
                array_node1[j].store(new_word, std::memory_order_relaxed);
    }else if(node == 0){
            char* new_word = reinterpret_cast<char*>(new numa<char, 0>[10]);
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            array_node0[j].store(new_word, std::memory_order_relaxed);
       }
       else{
            char* new_word = reinterpret_cast<char*>(new numa<char, MAX_NODE>[10]);
            sprintf(temp_buf, "%ld", key_dist(rng));
            std::strcpy(new_word, temp_buf);
            array_node1[j].store(new_word, std::memory_order_relaxed);
       }
    }


    pthread_barrier_wait(&init_bar);
}

void array_test(int tid, int duration, std::string DS_config, int node, int num_threads, int64_t array_size, int num_arrays, int interval)
{
    pthread_barrier_wait(&bar);

    int64_t ops = 0;
    
    // Interval Setup
    size_t num_intervals = (interval > 0) ? (duration / interval) : 1;
    std::vector<int64_t> local_interval_ops(num_intervals, 0);

    auto startTimer = std::chrono::steady_clock::now();
    auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextIntervalTime = startTimer + std::chrono::seconds(interval > 0 ? interval : duration);
    int interval_idx = 0;

    std::mt19937_64 rng(tid);
    std::uniform_int_distribution<long long> op_dist(1, 100);
    std::uniform_int_distribution<long long> array_dist(0, num_arrays - 1);
    std::uniform_int_distribution<long long> word_dist(0, array_size - 1);


    int curr_idx = word_dist(rng);
    char* word;
    while (true) {
        int array_choice = array_dist(rng);
        int word_choice = word_dist(rng);
        int op_choice = op_dist(rng);

        if(node == 0){
            word = array_node0[curr_idx].load(std::memory_order_relaxed);
        }
        else{
            word = array_node1[curr_idx].load(std::memory_order_relaxed);
        }
        
        int64_t next_idx = reinterpret_cast<int64_t>(word); // Use the content to determine next index (introduces data dependency)
        curr_idx = (next_idx + word_dist(rng)) % array_size; // Ensure we stay within bound
        ops++;

 
        if(ops > 1000000) { // Periodically check time to avoid overhead of checking every iteration
            auto now = std::chrono::steady_clock::now();
            
            // if (interval > 0 && now >= nextIntervalTime && interval_idx < num_intervals) {
            //     local_interval_ops[interval_idx] = ops;
            //     interval_idx++;
            //     nextIntervalTime += std::chrono::seconds(interval);
            // }

            if (now >= endTimer) {
                local_interval_ops[0] = ops;
                break;
            }

        }

        
        // if (interval > 0 && now >= nextIntervalTime && interval_idx < num_intervals) {
        //     local_interval_ops[interval_idx] = ops;
        //     last_snapshot_ops = ops;
        //     interval_idx++;
        //     nextIntervalTime += std::chrono::seconds(interval);
        // }

    }
    
    // Capture remaining ops
    // if (interval > 0 && interval_idx < num_intervals) {
    //      local_interval_ops[interval_idx] += (ops - last_snapshot_ops);
    // } else if (interval == 0) {
    //     local_interval_ops[0] = ops;
    // }

    pthread_barrier_wait(&bar);   

    // Aggregate
    globalLK->lock();
    if(node == 0) {
       // for(size_t i=0; i<local_interval_ops.size(); i++) {
        globalOps0[0] += local_interval_ops[0];
        //}
    } else {
        //for(size_t i=0; i<local_interval_ops.size(); i++) {
        globalOps1[0] += local_interval_ops[0];
        //}
    }
    globalLK->unlock();

    pthread_barrier_wait(&bar);
}