#include "Array.h"
#include "numatype.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <atomic> 
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


std::atomic<char*>* array_node0;
std::atomic<char*>* array_node1;

static std::mutex* globalLK = nullptr;
static pthread_barrier_t bar;
static pthread_barrier_t init_bar;

static std::vector<int64_t> globalOps0;
static std::vector<int64_t> globalOps1;


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
    std::uniform_int_distribution<int64_t> key_dist(0, array_size - 1);
    
    for(int64_t j = 0; j < array_size; ++j){
        int64_t next_hop = key_dist(rng);
            
        char* fake_ptr = reinterpret_cast<char*>(next_hop);
  
        if(node == 0) {
            array_node0[j].store(fake_ptr, std::memory_order_relaxed);
        }
        else {
            array_node1[j].store(fake_ptr, std::memory_order_relaxed);
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

    // Start at a random valid index
    int64_t current_idx = word_dist(rng);

    int64_t last_snapshot_ops = 0;
    
    while (true) {
        volatile char* val;
        if(node ==0){
            val =array_node0[current_idx].load(std::memory_order_relaxed);
        }else{
            val =array_node1[current_idx].load(std::memory_order_relaxed);
        }
        int64_t next_idx = reinterpret_cast<int64_t>(val);
        
        if (next_idx < 0 || next_idx >= array_size) {
            next_idx = 0; 
        }
        current_idx = (next_idx+word_dist(rng))% array_size; 
        ops++;

        if((ops % 1024)==0){
            auto now = std::chrono::steady_clock::now();
            if (now >= endTimer){
                local_interval_ops[0]= ops;
                break;
            }
        }
    }
    
    pthread_barrier_wait(&bar);


    // Aggregate
    globalLK->lock();
    if(node == 0) {
             globalOps0[0] += local_interval_ops[0];
    } else {
             globalOps1[0] += local_interval_ops[0];
    }
    globalLK->unlock();

    pthread_barrier_wait(&bar);
}
