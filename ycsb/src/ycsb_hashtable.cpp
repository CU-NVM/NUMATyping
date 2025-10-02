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
WorkloadConfig workloadA = {50, 50, 0, 0, 0}; // balanced read/update
WorkloadConfig workloadB = {95, 5, 0, 0, 0}; // read heavy
WorkloadConfig workloadC = {100, 0, 0, 0, 0}; // read only
WorkloadConfig workloadD = {95, 0, 5, 0, 0}; // read latest, not sure on this one
WorkloadConfig workloadE = {0, 0, 5, 95, 0}; // scan heavy
WorkloadConfig workloadF = {50, 0, 0, 0, 50}; // read-modify-write

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

// convert functionality to be contained in threads
void worker_thread(
    int num_ops,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    int num_keys,
    HashTable* local_ht,
    HashTable* remote_ht,
    int local_pct)
{
    // seed rng
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);
    uniform_int_distribution<int> locality_dist(1, 100);

    for (int i = 0; i < num_ops; i++) {
        // next key
        uint64_t key_id = gen->Next();
        string key = "key" + to_string(key_id);

        // roll for locality
        int locality_choice = locality_dist(rng);

        // define if we're targeting the local or remote hash table
        HashTable* target_ht;
        if (locality_choice <= local_pct)
            target_ht = local_ht;
        else
            target_ht = remote_ht;

        // roll for ycsb operation
        int op_choice = op_dist(rng);

        // perform operation
        // read case
        if (op_choice <= cfg->read_pct) {
            target_ht->getCount(key.c_str());
        }
        // update case
        else if (op_choice <= cfg->read_pct + cfg->update_pct) {
            target_ht->updateCount(key.c_str(), 1);
        }
        // insert case
        else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
            target_ht->insert(key.c_str());
        }
        // scan case
        else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
            for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                string skey = "key" + to_string(key_id + j);
                target_ht->getCount(skey.c_str());
            }
        }
        // rmw case
        else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct + cfg->rmw_pct) {
            int old = target_ht->getCount(key.c_str());
            target_ht->updateCount(key.c_str(), 1);
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 7) {
        cerr << "Usage: ./ycsb_hashtable [workload] [num_ops] [num_keys] [zipf_theta] [buckets] [locality]\n";
        return 1;
    }

    // command line args
    string workload_key = argv[1];
    int total_ops       = stoi(argv[2]);
    int num_keys        = stoi(argv[3]);
    double theta        = stod(argv[4]);
    int buckets         = stoi(argv[5]);
    string locality_key = argv[6];

    // configure workload and locality choices
    WorkloadConfig cfg = selectWorkload(workload_key);
    int local_pct = selectLocality(locality_key);

    HashTable ht1(buckets);
    HashTable ht2(buckets);
    
    // zipfian generator for each thread to avoid contention
    ZipfianGenerator gen1(0, num_keys - 1, theta);
    ZipfianGenerator gen2(0, num_keys - 1, theta);

    int ops_per_thread = total_ops / 2;
    auto start = chrono::high_resolution_clock::now();

    // launch and join threads
    thread t1(worker_thread, ops_per_thread, &cfg, &gen1, num_keys, &ht1, &ht2, local_pct);
    thread t2(worker_thread, ops_per_thread, &cfg, &gen2, num_keys, &ht2, &ht1, local_pct);
    t1.join();
    t2.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << total_ops << " ops on 2 threads with " << locality_key << " locality completed in " << elapsed.count() << "s\n";
    return 0;
}