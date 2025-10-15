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

using namespace std;
using namespace ycsbc;

// distribution of each operation type
struct WorkloadConfig {
    int read_pct;
    int update_pct;
    int insert_pct;
    int scan_pct;
    int rmw_pct;
};

// workloads A-F
WorkloadConfig workloadA = {50, 50, 0, 0, 0};
WorkloadConfig workloadB = {95, 5, 0, 0, 0};
WorkloadConfig workloadC = {100, 0, 0, 0, 0};
WorkloadConfig workloadD = {95, 0, 5, 0, 0};
WorkloadConfig workloadE = {0, 0, 5, 95, 0};
WorkloadConfig workloadF = {50, 0, 0, 0, 50};

// helper that matches command line arg to workload
WorkloadConfig selectWorkload(const string &w) {
    if (w == "A") return workloadA;
    if (w == "B") return workloadB;
    if (w == "C") return workloadC;
    if (w == "D") return workloadD;
    if (w == "E") return workloadE;
    if (w == "F") return workloadF;
    throw runtime_error("Unknown workload " + w);
}

// helper that matches command line arg to locality split
int selectLocality(const string &l) {
    if (l == "80-20") return 80;
    if (l == "50-50") return 50;
    if (l == "20-80") return 20;
    throw runtime_error("Unknown locality split. Use 80-20, 50-50, or 20-80." + l);
}

void worker_thread(
    int thread_id,
    int num_total_threads,
    int num_ops,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    int num_keys,
    const vector<HashTable*>& all_hts,
    int local_pct)
{
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);
    uniform_int_distribution<int> locality_dist(1, 100);
    HashTable* local_ht = all_hts[thread_id];

    for (int i = 0; i < num_ops; i++) {
        uint64_t key_id = gen->Next();
        string key = "key" + to_string(key_id);
        int locality_choice = locality_dist(rng);
        HashTable* target_ht;

        if (locality_choice <= local_pct || num_total_threads == 1) {
            target_ht = local_ht;
        } else {
            uniform_int_distribution<int> remote_dist(0, num_total_threads - 1);
            int remote_id;
            do {
                remote_id = remote_dist(rng);
            } while (remote_id == thread_id);
            target_ht = all_hts[remote_id];
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
    int num_threads
) {
    if (num_threads <= 0) {
        cerr << "Number of threads must be greater than 0.\n";
        return;
    }
    
    WorkloadConfig cfg = selectWorkload(workload_key);
    int local_pct = selectLocality(locality_key);

    vector<HashTable*> hash_tables;
    hash_tables.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        hash_tables.push_back(new HashTable(buckets));
    }
    
    vector<thread> threads;
    threads.reserve(num_threads);
    vector<ZipfianGenerator*> generators;

    for (int i = 0; i < num_threads; ++i) {
        generators.push_back(new ZipfianGenerator(0, num_keys - 1, theta));
    }

    int ops_per_thread = total_ops / num_threads;
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(
            worker_thread,
            i,
            num_threads,
            ops_per_thread,
            &cfg,
            generators[i],
            num_keys,
            std::ref(hash_tables),
            local_pct
        );
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "YCSB Benchmark Report:\n";
    cout << "----------------------\n";
    cout << total_ops << " ops on " << num_threads << " threads with " 
         << locality_key << " locality completed in " << elapsed.count() << "s\n";

    for (auto ht : hash_tables) {
        delete ht;
    }
    for (auto gen : generators) {
        delete gen;
    }
}