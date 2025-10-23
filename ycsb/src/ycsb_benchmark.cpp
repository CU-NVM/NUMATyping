#include "../include/ycsb_benchmark.hpp"
#include "../zipf_base/src/zipfian_generator.h"
#include "../zipf_base/src/ycsbutils.h"
#include "../include/HashTable.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <stdexcept>
#include <functional>
#include "../../numaLib/numatype.hpp"
#include "../../numaLib/numathreads.hpp"
#include <pthread.h>

using namespace std;
using namespace ycsbc;

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 1
#endif

struct WorkloadConfig {
    int read_pct;
    int update_pct;
    int insert_pct;
    int scan_pct;
    int rmw_pct;
};

WorkloadConfig workloadA = {50, 50, 0, 0, 0};
WorkloadConfig workloadB = {95, 5, 0, 0, 0};
WorkloadConfig workloadC = {100, 0, 0, 0, 0};
WorkloadConfig workloadD = {95, 0, 5, 0, 0};
WorkloadConfig workloadE = {0, 0, 5, 95, 0};
WorkloadConfig workloadF = {50, 0, 0, 0, 50};

HashTable* ht_node0 = nullptr;
HashTable* ht_node1 = nullptr;
pthread_barrier_t bar;


WorkloadConfig selectWorkload(const string &w) {
    if (w == "A") return workloadA;
    if (w == "B") return workloadB;
    if (w == "C") return workloadC;
    if (w == "D") return workloadD;
    if (w == "E") return workloadE;
    if (w == "F") return workloadF;
    throw runtime_error("Unknown workload " + w);
}

int selectLocality(const string &l) {
    if (l == "80-20") return 80;
    if (l == "50-50") return 50;
    if (l == "20-80") return 20;
    throw runtime_error("Unknown locality split. Use 80-20, 50-50, or 20-80." + l);
}

void worker_thread(
    int thread_id,
    int num_total_threads,
    int numa_node,
    int num_ops,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    int num_keys,
    int local_pct)
{
    pthread_barrier_wait(&bar);
    
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);
    uniform_int_distribution<int> locality_dist(1, 100);

    HashTable* local_ht = (numa_node == 0) ? ht_node0 : ht_node1;
    HashTable* remote_ht = (numa_node == 0) ? ht_node1 : ht_node0;

    for (int i = 0; i < num_ops; i++) {
        uint64_t key_id = gen->Next();
        string key = "key" + to_string(key_id);
        int locality_choice = locality_dist(rng);
        HashTable* target_ht;

        if (locality_choice <= local_pct || num_total_threads == 1) {
            target_ht = local_ht;
        } else {
            target_ht = remote_ht;
        }

        int op_choice = op_dist(rng);
        if (op_choice <= cfg->read_pct) {
            target_ht->getCount(key.c_str());
        } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
            target_ht->updateCount(key.c_str(), 1);
        } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
            target_ht->insert(key.c_str());
        } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
            for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                string skey = "key" + to_string(key_id + j);
                target_ht->getCount(skey.c_str());
            }
        } else {
            target_ht->getCount(key.c_str());
            target_ht->updateCount(key.c_str(), 1);
        }
    }
}

void run_ycsb_benchmark(
    const string& workload_key,
    int total_ops,
    int num_keys,
    double theta,
    int buckets,
    const string& locality_key,
    int num_threads,
    const string& th_config,
    const string& DS_config
) {
    if (num_threads <= 0) {
        cerr << "Number of threads must be greater than 0.\n";
        return;
    }
    
    WorkloadConfig cfg = selectWorkload(workload_key);
    int local_pct = selectLocality(locality_key);

    pthread_barrier_init(&bar, NULL, num_threads);

    if (DS_config == "numa") {
        ht_node0 = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ZERO>(buckets));
        ht_node1 = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ONE>(buckets));
    } else {
        ht_node0 = new HashTable(buckets);
        ht_node1 = new HashTable(buckets);
    }

    if (ht_node0 == nullptr || ht_node1 == nullptr) {
        cerr << "Failed to allocate hash tables!" << endl;
        return;
    }
    
    vector<thread_numa<NODE_ZERO>*> numa_thread0;
    vector<thread_numa<NODE_ONE>*> numa_thread1;
    vector<thread*> regular_thread0;
    vector<thread*> regular_thread1;

    int threads_per_node = num_threads / 2;
    int threads_node1 = num_threads - threads_per_node;

    if (th_config == "numa") {
        numa_thread0.resize(threads_per_node);
        numa_thread1.resize(threads_node1);
    } else {
        regular_thread0.resize(threads_per_node);
        regular_thread1.resize(threads_node1);
    }

    vector<ZipfianGenerator*> generators;
    for (int i = 0; i < num_threads; ++i) {
        generators.push_back(new ZipfianGenerator(0, num_keys - 1, theta));
    }

    int ops_per_thread = total_ops / num_threads;
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < threads_per_node; ++i) {
        int thread_id = i;
        int numa_node = 0;
        if (th_config == "numa") {
            numa_thread0[i] = new thread_numa<NODE_ZERO>(
                worker_thread,
                thread_id, num_threads, numa_node, ops_per_thread, &cfg, 
                generators[thread_id], num_keys, local_pct
            );
        } else {
            regular_thread0[i] = new thread(
                worker_thread,
                thread_id, num_threads, numa_node, ops_per_thread, &cfg, 
                generators[thread_id], num_keys, local_pct
            );
        }
    }

    for (int i = 0; i < threads_node1; ++i) {
        int thread_id = i + threads_per_node;
        int numa_node = 1;
        if (th_config == "numa") {
            numa_thread1[i] = new thread_numa<NODE_ONE>(
                worker_thread,
                thread_id, num_threads, numa_node, ops_per_thread, &cfg, 
                generators[thread_id], num_keys, local_pct
            );
        } else {
            regular_thread1[i] = new thread(
                worker_thread,
                thread_id, num_threads, numa_node, ops_per_thread, &cfg, 
                generators[thread_id], num_keys, local_pct
            );
        }
    }

    if (th_config == "numa") {
        for (auto th : numa_thread0) th->join();
        for (auto th : numa_thread1) th->join();
    } else {
        for (auto th : regular_thread0) th->join();
        for (auto th : regular_thread1) th->join();
    }


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "YCSB Benchmark Report:\n";
    cout << "----------------------\n";
    cout << "Workload: " << workload_key << ", Threads: " << num_threads 
         << ", Locality: " << locality_key << endl;
    cout << "Thread Config: " << th_config << ", DS Config: " << DS_config << endl;
    cout << total_ops << " ops completed in " << elapsed.count() << "s\n";
    cout << "Throughput: " << (double)total_ops / elapsed.count() << " OPS/sec" << endl;

    pthread_barrier_destroy(&bar);
    
    delete ht_node0;
    delete ht_node1;

    for (auto gen : generators) {
        delete gen;
    }

    if (th_config == "numa") {
        for (auto th : numa_thread0) delete th;
        for (auto th : numa_thread1) delete th;
    } else {
        for (auto th : regular_thread0) delete th;
        for (auto th : regular_thread1) delete th;
    }
}