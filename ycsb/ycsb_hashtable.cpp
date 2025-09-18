#include "zipf_base/src/zipfian_generator.h"
#include "zipf_base/src/ycsbutils.h"
#include "include/HashTable.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <random>
#include <string>

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

int main(int argc, char** argv) {
    if (argc != 6) {
        cerr << "Usage: ./ycsb_hashtable [workload] [num_ops] [num_keys] [zipf_theta] [buckets]\n";
        return 1;
    }

    // command line args
    // note: too many ops breaks stoi
    string workload = argv[1];
    int num_ops   = stoi(argv[2]);
    int num_keys  = stoi(argv[3]);
    double theta  = stod(argv[4]);
    int buckets   = stoi(argv[5]);

    // set workload
    WorkloadConfig cfg = selectWorkload(workload);

    // declare this stuff
    HashTable ht(buckets); // from HashTable.hpp
    ZipfianGenerator gen(0, num_keys - 1, theta); // from zipfian_generator.hpp

    // rng to decide what ops to do
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);

    auto start = chrono::high_resolution_clock::now();

    // use workload percentages to determine what op to use
    for (int i = 0; i < num_ops; i++) {
        // get next key from zipfian generator
        uint64_t key_id = gen.Next();
        string key = "key" + to_string(key_id);

        // choose op
        int op_choice = op_dist(rng);

        // read case
        if (op_choice <= cfg.read_pct) {
            ht.getCount(key.c_str());
        }
        // update case
        else if (op_choice <= cfg.read_pct + cfg.update_pct) {
            ht.updateCount(key.c_str(), 1);
        }
        // insert case
        else if (op_choice <= cfg.read_pct + cfg.update_pct + cfg.insert_pct) {
            ht.insert(key.c_str());
        }
        // scan case (short sequential read) (sequential keys in j loop but will stop if goes out of bounds) 
        else if (op_choice <= cfg.read_pct + cfg.update_pct + cfg.insert_pct + cfg.scan_pct) {
            for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                string skey = "key" + to_string(key_id + j);
                ht.getCount(skey.c_str());
            }
        }
        // rmw case
        else if (op_choice <= cfg.read_pct + cfg.update_pct + cfg.insert_pct + cfg.scan_pct + cfg.rmw_pct) {
            int old = ht.getCount(key.c_str());
            ht.updateCount(key.c_str(), 1);
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << num_ops << " ops in " << elapsed.count() << "s\n";
    return 0;
}
