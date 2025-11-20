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

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 1
#endif

std::vector<HashTable*> ht_node0;
std::vector<HashTable*> ht_node1;
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
	pthread_barrier_init(&init_bar, NULL, 2);
	globalOps0.resize(duration/interval);
	globalOps1.resize(duration/interval);
	ops0 = 0;
	ops1 = 0;
	printLK = new std::mutex();
	globalLK = new std::mutex();
}

void numa_hash_table_init(int node, std::string DS_config, int buckets, int num_tables, int num_keys) {

    if(node==0){
        ht_node0.resize(num_tables);
        ht_node0_locks.resize(num_tables);
        ht_node1.resize(num_tables);
        ht_node1_locks.resize(num_tables);
    }
    pthread_barrier_wait(&init_bar);

    if(node == 0) 
    {
        if (DS_config == "numa") 
        {
            for (int i = 0; i < num_tables; i++) 
            {
                ht_node0[i] = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ZERO>(buckets));
            }
        } 
        else 
        {
            for (int i = 0; i < num_tables; i++) 
            {
                ht_node0[i] = new HashTable(buckets);
            }
        }

        for (int i = 0; i < num_tables; i++) {
            ht_node0_locks[i] = new std::mutex();
        }
    }
    else if(node == 1) 
    {
        if (DS_config == "numa") 
        {
            for (int i = 0; i < num_tables; i++) 
            {
                ht_node1[i] = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ONE>(buckets));
            }
        } 
        else 
        {   
            for (int i = 0; i < num_tables; i++) 
            {
                ht_node1[i] = new HashTable(buckets);
            }
        }
        for(int i = 0; i < num_tables; i++) 
        {
            ht_node1_locks[i] = new std::mutex();
        }   
    }
    
    pthread_barrier_wait(&init_bar);

    for (int i = 0; i < num_tables; i++) 
    {
        if (ht_node0[i] == nullptr || ht_node1[i] == nullptr) 
        {
            cerr << "Hash table allocation error!" << endl;
            return;
        }
    }

    // 'num_tables' passed here is actually tables_per_node (main passes num_tables/2)
    int tables_per_node = num_tables; 
    int actual_total_tables = tables_per_node * 2;

    for (long long i = 0; i < num_keys; ++i) {
        std::string key = "key" + std::to_string(i);
        unsigned long key_hash = prefill_hash(key.c_str());
        int table_index = key_hash % actual_total_tables;

        if (node == 0) 
        {
            if (table_index < tables_per_node) 
            {
                ht_node0[table_index]->insert(key.c_str());
            }
        } 
        else if (node == 1) 
        {
            if (table_index >= tables_per_node) 
            {
                int local_index = table_index - tables_per_node;
                ht_node1[local_index]->insert(key.c_str());
            }
        }
    }

    pthread_barrier_wait(&init_bar);
}

void prefill_hash_tables(int num_keys_to_fill, int total_num_tables) {
    // num_tables from main.cpp is the total number of tables
    int tables_per_node = total_num_tables / 2;
    int actual_total_tables = tables_per_node * 2;

    if (actual_total_tables <= 0 || ht_node0.empty() || ht_node1.empty()) {
        std::cerr << "Prefill Error: Hash tables not initialized or num_tables < 2." << std::endl;
        return;
    }

    for (long long i = 0; i < num_keys_to_fill; ++i) {
        std::string key = "key" + std::to_string(i);
        
        // hash into a table
        unsigned long key_hash = prefill_hash(key.c_str());
        int table_index = key_hash % actual_total_tables;
        // insert key i into the table the same way as in ycsb_test
        if (table_index < tables_per_node) {
            ht_node0[table_index]->insert(key.c_str());
        } else {
            int node1_index = table_index - tables_per_node;
            ht_node1[node1_index]->insert(key.c_str());
        }
    }
}

void ycsb_test(
    int thread_id,
    int num_total_threads,
    int numa_node,
    int duration,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    int num_keys,
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

	while (duration_cast<seconds>(steady_clock::now() - startTimer).count() < duration) {
		uint64_t key_id = gen->Next();
        string key = "key" + to_string(key_id);
        int locality_choice = locality_dist(rng);
        int ht_choice = ht_dist(rng);

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
                    ht_node0[ht_choice]->insert(key.c_str());
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
                    ht_node1[ht_choice]->insert(key.c_str());
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
                    ht_node1[ht_choice]->insert(key.c_str());
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
                    ht_node0[ht_choice]->insert(key.c_str());
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
	if(numa_node==0)
	{
		for(int i=0; i<localOps.size(); i++){
			globalOps0[i] += localOps[i];
		}
		ops0 = globalOps0[globalOps0.size()-1];
	}
	else
	{
		for(int i=0; i<localOps.size(); i++){
			globalOps1[i] += localOps[i];
		}
		ops1 = globalOps1[globalOps1.size()-1];
	}
	globalLK->unlock();

	pthread_barrier_wait(&bar);

}

