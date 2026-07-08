/*! \file main.cpp
 * \brief Test Suite for classic Data Structures.
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "numathreads.hpp"
#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <string>
#include <vector>
#include <cmath>
#include <getopt.h>
#include <chrono>
#include <iomanip>
#include <unordered_map>

using namespace std;

#ifndef NUM_NUMA_NODES
    #warning "NUM_NUMA_NODES not defined! Defaulting to 4."
    #define NUM_NUMA_NODES 4
#endif


std::string thread_config;
std::string DS_config;
int64_t num_DS = 0;
int num_threads = 0;
int duration = 0;
std::string DS_name;
bool prefill_set = false;
int crossover = 0;
int keyspace = 80000;
int run_freq = 1;
int interval = 20;
int bucket_count = 10;

extern int64_t ops0, ops1, ops2, ops3;
extern std::vector<int64_t> globalOps0, globalOps1, globalOps2, globalOps3;


void print_function(int duration, const int64_t* perNode, int64_t total){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::cout << std::put_time(local_time, "%Y-%m-%d") << ", ";
    std::cout << std::put_time(local_time, "%H:%M:%S") << ", ";
    std::cout << DS_name << ", ";
    std::cout << num_DS << ", ";
    std::cout << num_threads << ", ";
    std::cout << thread_config << ", ";
    std::cout << DS_config << ", ";
    std::cout << duration << ", ";
    std::cout << keyspace << ", ";
    std::cout << interval << ", ";
    for (int nd = 0; nd < NUM_NUMA_NODES; nd++) {
        std::cout << perNode[nd] << ", ";
    }
    std::cout << total << "\n";
}


void main_BST_test(int duration, int64_t num_DS, int num_threads, int crossover, int keyspace){
    int per_node_DS      = num_DS / NUM_NUMA_NODES;
    int per_node_threads = num_threads / NUM_NUMA_NODES;

    std::vector<std::thread*> init_threads;
    std::vector<std::thread*> workers;

    // Initialization: one init thread per node, pinned with an explicit node id.
    if(thread_config == "numa"){
        init_threads.push_back(new thread_numa<0>(numa_BST_init, DS_config, per_node_DS, keyspace, 0, crossover));
        init_threads.push_back(new thread_numa<1>(numa_BST_init, DS_config, per_node_DS, keyspace, 1, crossover));
        init_threads.push_back(new thread_numa<2>(numa_BST_init, DS_config, per_node_DS, keyspace, 2, crossover));
        init_threads.push_back(new thread_numa<3>(numa_BST_init, DS_config, per_node_DS, keyspace, 3, crossover));
    }
    else{
        for(int nd = 0; nd < NUM_NUMA_NODES; nd++){
            init_threads.push_back(new std::thread(numa_BST_init, DS_config, per_node_DS, keyspace, nd, crossover));
        }
    }
    for (auto* t : init_threads) { if (t) t->join(); }

    // Test: per_node_threads workers per node, pinned with an explicit node id.
    int tid = 0;
    if(thread_config == "numa"){
        for(int i = 0; i < per_node_threads; i++)
            workers.push_back(new thread_numa<0>(BinarySearchTest, tid++, duration, 0, per_node_DS, per_node_threads, crossover, keyspace, interval));
        for(int i = 0; i < per_node_threads; i++)
            workers.push_back(new thread_numa<1>(BinarySearchTest, tid++, duration, 1, per_node_DS, per_node_threads, crossover, keyspace, interval));
        for(int i = 0; i < per_node_threads; i++)
            workers.push_back(new thread_numa<2>(BinarySearchTest, tid++, duration, 2, per_node_DS, per_node_threads, crossover, keyspace, interval));
        for(int i = 0; i < per_node_threads; i++)
            workers.push_back(new thread_numa<3>(BinarySearchTest, tid++, duration, 3, per_node_DS, per_node_threads, crossover, keyspace, interval));
    }
    else{
        for(int nd = 0; nd < NUM_NUMA_NODES; nd++){
            for(int i = 0; i < per_node_threads; i++){
                workers.push_back(new std::thread(BinarySearchTest, tid++, duration, nd, per_node_DS, per_node_threads, crossover, keyspace, interval));
            }
        }
    }
    for (auto* t : workers) { if (t) t->join(); }
}


int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"th_config", required_argument, nullptr, 'c'},
        {"DS_config", required_argument, nullptr, 'd'},
        {"DS_name", required_argument, nullptr, 's'},
        {"num_DS", required_argument, nullptr, 'n'},
        {"num_threads", required_argument, nullptr, 't'},
        {"duration", required_argument, nullptr, 'D'},
        {"run_freq", optional_argument, nullptr, 'f'},
        {"crossover", optional_argument, nullptr, 'x'},
        {"keyspace", required_argument, nullptr, 'k'},
        {"interval", required_argument, nullptr, 'i'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "n:t:D:x:k:f:i:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': thread_config = optarg; break;
            case 'd': DS_config = optarg; break;
            case 'n': num_DS = std::stoll(optarg); break;
            case 't': num_threads = std::stoll(optarg); break;
            case 'D': duration = std::stoi(optarg); break;
            case 's': DS_name = optarg; break;
            case 'f': if (optarg) run_freq = std::stoi(optarg); break;
            case 'x': if (optarg) crossover = std::stoi(optarg); break;
            case 'i': if (optarg) interval = std::stoi(optarg); break;
            case 'k': if (optarg) keyspace = std::stoi(optarg); break;
            case '?': std::cerr << "Unknown option or missing argument.\n"; return 1;
            default: break;
        }
    }

    // Actual worker count (num_threads rounded down to a multiple of NUM_NUMA_NODES).
    int total_workers = (num_threads / NUM_NUMA_NODES) * NUM_NUMA_NODES;

    global_init(total_workers, duration, interval);

    if (thread_config == "numa" || DS_config == "numa") {
        if (numa_num_configured_nodes() == 1) {
            std::cout << "NUMA not available.\n";
        }
    }

    for (int i = 0; i < run_freq; i++) {
        main_BST_test(duration, num_DS, num_threads, crossover, keyspace);
    }

    // Per-interval CSV output: one column per node plus a total.
    int newDuration = interval;
    int num_intervals = (int)globalOps0.size();
    for (int i = 0; i < num_intervals; i++) {
        int64_t perNode[NUM_NUMA_NODES] = {
            globalOps0[i], globalOps1[i], globalOps2[i], globalOps3[i]
        };
        int64_t total = globalOps0[i] + globalOps1[i] + globalOps2[i] + globalOps3[i];
        print_function(newDuration, perNode, total);
        newDuration += interval;
    }

    global_cleanup();
}
