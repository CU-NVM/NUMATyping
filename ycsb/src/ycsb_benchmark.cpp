#include "ycsb_benchmark.hpp"
#include "ycsbutils.h"
#include "HashTable.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <stdexcept>
#include <functional>
#include "numathreads.hpp"
#include <pthread.h>
#include <mutex>
#include <syncstream>
#include <cstdlib>

#include <map>
#include <atomic>

using namespace std;
using namespace ycsbc;
using namespace std::chrono;

#define NODE_ZERO 0
#ifndef MAX_NODE
    #warning "MAX_NODE not defined! Defaulting to 1."
    #define MAX_NODE 1
#endif

int global_successful_inserts;
int global_successful_init_inserts;
HashTable** ht_node0;
HashTable** ht_node1;
std::vector<std::mutex*> ht_node0_locks;
std::vector<std::mutex*> ht_node1_locks;

std::mutex* printLK;
std::mutex* globalLK;

std::vector<int64_t> globalOps0;
std::vector<int64_t> globalOps1;
int64_t ops0=0;
int64_t ops1=0;
pthread_barrier_t bar;
pthread_barrier_t init_bar;

unsigned long prefill_hash(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

void global_init(int num_threads, int duration, int interval) {
	pthread_barrier_init(&bar, NULL, num_threads);
	pthread_barrier_init(&init_bar, NULL, num_threads);
	globalOps0.resize(duration/interval);
	globalOps1.resize(duration/interval);
	ops0 = 0;
	ops1 = 0;
	printLK = new std::mutex();
	globalLK = new std::mutex();
    global_successful_init_inserts=0;
    global_successful_inserts=0;
}
void numa_hash_table_init(int thread_id,
                          int node,
                          std::string DS_config,
                          int buckets,
                          int num_tables,        // tables per node
                          uint64_t num_keys,
                          int num_total_threads)
{
    int threads_per_node = num_total_threads / 2;
    // ------------------ GLOBAL ALLOCATION (ONCE) ------------------
    if (thread_id == 0) {
        if(DS_config == "numa") {
            //std::cout << "Thread " << thread_id << " initializing NUMA hash tables on Node " << NODE_ZERO << std::endl;
            ht_node0 = reinterpret_cast<HashTable**>( new numa<HashTable*, NODE_ZERO>[num_tables]);
            ht_node0_locks.resize(num_tables);
            for(int i = 0; i < num_tables; i++) {
                ht_node0[i] = reinterpret_cast<HashTable*>( new numa<HashTable, NODE_ZERO>(buckets));
                ht_node0_locks[i] = new std::mutex();
            }
            //std::cout << "Thread " << thread_id << " finished initializing NUMA hash tables on Node " << NODE_ZERO << std::endl;
        }
        else {
            ht_node0 = reinterpret_cast<HashTable**>( new HashTable*[num_tables]);
            ht_node0_locks.resize(num_tables);
            for(int i = 0; i < num_tables; i++) {
                ht_node0[i] = new HashTable(buckets);
                ht_node0_locks[i] = new std::mutex();
            }
        }
    }
    if (thread_id == threads_per_node +1 && node == 1) {
        if(DS_config == "numa") {
            //std::cout << "Thread " << thread_id << " initializing NUMA hash tables on Node " << MAX_NODE<< std::endl;
            ht_node1 = reinterpret_cast<HashTable**>( new numa<HashTable*, MAX_NODE>[num_tables]);
            ht_node1_locks.resize(num_tables);
            for(int i = 0; i < num_tables; i++) {
                ht_node1[i] = reinterpret_cast<HashTable*>( new numa<HashTable, MAX_NODE    >(buckets));
                ht_node1_locks[i] = new std::mutex();
            }
            //std::cout << "Thread " << thread_id << " finished initializing NUMA hash tables on Node " << MAX_NODE << std::endl;
        }
        else {  
            ht_node1 = reinterpret_cast<HashTable**>( new HashTable*[num_tables]);
            ht_node1_locks.resize(num_tables);
            for(int i = 0; i < num_tables; i++) {
                ht_node1[i] = new HashTable(buckets);
                ht_node1_locks[i] = new std::mutex();
            }
        }   
    }
    pthread_barrier_wait(&init_bar);
    // ------------------ SANITY CHECK ------------------
    if (thread_id == 0) {
        for (int i = 0; i < num_tables; i++) {
            if (ht_node0[i] == nullptr || ht_node1[i] == nullptr || ht_node0_locks[i] == nullptr || ht_node1_locks[i] == nullptr) {
                std::cerr << "Hash table allocation error!" << std::endl;
                return;
            }
        }
    }
    pthread_barrier_wait(&init_bar);
    return;
    // ------------------ RNG (UNIQUE PER THREAD) ------------------
    std::random_device rd;
    std::mt19937_64 rng(rd() ^ (node << 16) ^ thread_id);
    std::uniform_int_distribution<uint64_t> dist(0, num_keys - 1);
    int tables_per_node = num_tables;
    long long local_successful_inserts = 0;
    int actual_total_tables = tables_per_node * 2;
    // ------------------ PREFILL LOOP ------------------
    long long iterations = (num_keys / 2);

    for (long long i = 0; i < iterations; ++i) {
        long long key_id = dist(rng); 
        std::string key = "key" + std::to_string(key_id);
        unsigned long key_hash = prefill_hash(key.c_str());
        int table_index = key_hash % actual_total_tables;

        if (node == 0) {
            // If random key belongs to Node 0, insert it.
            if (table_index < tables_per_node) {
                ht_node0_locks[table_index]->lock();
                bool result = ht_node0[table_index]->insert(key.c_str());
                if (result) {
                    local_successful_inserts++;
                }
                ht_node0_locks[table_index]->unlock();  
            }
            // If it belongs to Node 1, skip it.
        } 
        else if (node == 1) {
            // If random key belongs to Node 1, insert it.
            if (table_index >= tables_per_node) {
                int local_idx = table_index - tables_per_node;
                ht_node1_locks[local_idx]->lock();
                bool result = ht_node1[local_idx]->insert(key.c_str());
                if (result) {
                    local_successful_inserts++;
                }
                ht_node1_locks[local_idx]->unlock();
            }
            // If it belongs to Node 0, skip it.
        }
    }
    // ------------------ REDUCTION ------------------
    pthread_barrier_wait(&init_bar);

    globalLK->lock();
        global_successful_init_inserts += local_successful_inserts;
    globalLK->unlock();

    pthread_barrier_wait(&init_bar);
    #ifdef DEBUG
        if (thread_id == 0) {
            std::cout << "Prefill complete. Total inserts = "  << global_successful_init_inserts << std::endl;
        }
    #endif

    pthread_barrier_wait(&init_bar);
}


void prefill_hash_tables(int num_keys_to_fill, int total_num_tables) {
//    // num_tables from main.cpp is the total number of tables
//    int tables_per_node = total_num_tables / 2;
//    int actual_total_tables = tables_per_node * 2;
//
//    if (actual_total_tables <= 0 || ht_node0.empty() || ht_node1.empty()) {
//        std::cerr << "Prefill Error: Hash tables not initialized or num_tables < 2." << std::endl;
//        return;
//    }
//
//    for (long long i = 0; i < num_keys_to_fill; ++i) {
//        std::string key = "key" + std::to_string(i);
//        
//        // hash into a table
//        unsigned long key_hash = prefill_hash(key.c_str());
//        int table_index = key_hash % actual_total_tables;
//        // insert key i into the table the same way as in ycsb_test
//        if (table_index < tables_per_node) {
//            ht_node0[table_index]->insert(key.c_str());
//        } else {
//            int node1_index = table_index - tables_per_node;
//            ht_node1[node1_index]->insert(key.c_str());
//        }
//    }
//
}

void ycsb_test(
    int thread_id,
    int num_total_threads,
    int numa_node,
    int duration,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    uint64_t num_keys,
    int local_pct,
    int interval,
    int num_tables
)
{

  #ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);

	int64_t ops = 0;
	thread_local vector<int64_t> localOps;
	localOps.resize(duration/interval);
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
	int intervalIdx = 0;
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);
    uniform_int_distribution<int> locality_dist(1, 100);
    uniform_int_distribution<int> ht_dist(0, num_tables-1); // already passed in as num_tables/2 aka tables_per_node
    uniform_int_distribution<uint64_t> key_dist(0, num_keys-1);
    int successful_inserts = 0;
	while (duration_cast<seconds>(steady_clock::now() - startTimer).count() < duration) {
		//uint64_t key_id = gen->Next();
        uint64_t key_id = key_dist(rng);
        string key = "key" + to_string(key_id);
        int locality_choice = locality_dist(rng);
        int ht_choice = prefill_hash(key.c_str())%num_tables;

        if (numa_node == 0) {
            if (locality_choice <= local_pct) {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->getCount(key.c_str());
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    bool result = ht_node0[ht_choice]->insert(key.c_str());
                    if (result) {
                        successful_inserts++;
                    }
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node0_locks[ht_choice]->lock();
                        ht_node0[ht_choice]->getCount(skey.c_str());
                        ht_node0_locks[ht_choice]->unlock();
                    }
                } else {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->getCount(key.c_str());
                    ht_node0[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node0_locks[ht_choice]->unlock();
                }
            }
            else {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->getCount(key.c_str());
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    bool result = ht_node1[ht_choice]->insert(key.c_str());
                    if (result) {
                        successful_inserts++;
                    }
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node1_locks[ht_choice]->lock();
                        ht_node1[ht_choice]->getCount(skey.c_str());
                        ht_node1_locks[ht_choice]->unlock();
                    }
                } else {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->getCount(key.c_str());
                    ht_node1[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node1_locks[ht_choice]->unlock();
                }
            }
        }

        else if (numa_node == 1)
        {
            if (locality_choice <= local_pct) {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->getCount(key.c_str());
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node1_locks[ht_choice]->lock();
                    bool result = ht_node1[ht_choice]->insert(key.c_str());
                    if (result) {
                        successful_inserts++;
                    }
                    ht_node1_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node1_locks[ht_choice]->lock();
                        ht_node1[ht_choice]->getCount(skey.c_str());
                        ht_node1_locks[ht_choice]->unlock();
                    }
                } else {
                    ht_node1_locks[ht_choice]->lock();
                    ht_node1[ht_choice]->getCount(key.c_str());
                    ht_node1[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node1_locks[ht_choice]->unlock();
                }
            }
            else {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->getCount(key.c_str());
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node0_locks[ht_choice]->lock();
                    bool result = ht_node0[ht_choice]->insert(key.c_str());
                    if (result) {
                        successful_inserts++;
                    }
                    ht_node0_locks[ht_choice]->unlock();
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node0_locks[ht_choice]->lock();
                        ht_node0[ht_choice]->getCount(skey.c_str());
                        ht_node0_locks[ht_choice]->unlock();
                    }
                } else {
                    ht_node0_locks[ht_choice]->lock();
                    ht_node0[ht_choice]->getCount(key.c_str());
                    ht_node0[ht_choice]->updateCount(key.c_str(), 1);
                    ht_node0_locks[ht_choice]->unlock();
                }
            }
        }
		ops++;
		if(std::chrono::steady_clock::now() >= nextLogTime){
			localOps[intervalIdx] = ops;
			intervalIdx++;
			nextLogTime += std::chrono::seconds(interval);
		}
    }

    
     
	globalLK->lock();
    global_successful_inserts += successful_inserts;
	if(numa_node==0)
	{
		for(int i=0; i<localOps.size(); i++){
			globalOps0[i] += localOps[i];
		}
	}
	else
	{
		for(int i=0; i<localOps.size(); i++){
			globalOps1[i] += localOps[i];
		}
	}
	globalLK->unlock();

	pthread_barrier_wait(&bar);
    
    #ifdef DEBUG
        globalLK->lock();
            if(thread_id == 0 && numa_node==0) {
                std::cout<< "All successful inserts are " << global_successful_inserts << std::endl;  
            }
        globalLK->unlock();
    #endif
}

