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

HashTable* ht_node0 = nullptr;
HashTable* ht_node1 = nullptr;
std::mutex* printLK;
std::mutex* globalLK;

std::vector<int64_t> globalOps0;
std::vector<int64_t> globalOps1;
int64_t ops0=0;
int64_t ops1=0;
pthread_barrier_t bar;
pthread_barrier_t init_bar;

void global_init(int num_threads, int duration, int interval){
	pthread_barrier_init(&bar, NULL, num_threads);
	pthread_barrier_init(&init_bar, NULL, 2);
	globalOps0.resize(duration/interval);
	globalOps1.resize(duration/interval);
	ops0 = 0;
	ops1 = 0;
	printLK = new std::mutex();
	globalLK = new std::mutex();
}



void numa_hash_table_init(int node, std::string DS_config, int buckets){
    if(node == 0){
        if (DS_config == "numa") {
            ht_node0 = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ZERO>(buckets));
        }else{
            ht_node0 = new HashTable(buckets);
        }
    }
    else if(node == 1) {
        if (DS_config == "numa"){
            ht_node1 = reinterpret_cast<HashTable*>(new numa<HashTable, NODE_ONE>(buckets));
        } else {
            ht_node1 = new HashTable(buckets);
        }
    }
    pthread_barrier_wait(&init_bar);
    if (ht_node0 == nullptr || ht_node1 == nullptr) {
        cerr << "Failed to allocate hash tables!" << endl;
        cout<<ht_node0<<endl;
        cout<<ht_node1<<endl;
        return;
    }
     pthread_barrier_wait(&init_bar);
    
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
    int interval)
{

  #ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);
	//std::cout<<"crossover value from test is "<<crossover<<std::endl;

	int64_t ops;
	thread_local vector<int64_t> localOps;
	localOps.resize(duration/interval);
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
	int intervalIdx = 0;
     mt19937 rng(random_device{}());
    uniform_int_distribution<int> op_dist(1, 100);
    uniform_int_distribution<int> locality_dist(1, 100);

	while (duration_cast<seconds>(steady_clock::now() - startTimer).count() < duration) {
		uint64_t key_id = gen->Next();
        string key = "key" + to_string(key_id);
        int locality_choice = locality_dist(rng);

        if (numa_node == 0) {
            if (locality_choice <= local_pct) {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node0->getCount(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node0->updateCount(key.c_str(), 1);
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node0->insert(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node0->getCount(skey.c_str());
                    }
                } else {
                    ht_node0->getCount(key.c_str());
                    ht_node0->updateCount(key.c_str(), 1);
                }
            }
            else {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node1->getCount(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node1->updateCount(key.c_str(), 1);
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node1->insert(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node1->getCount(skey.c_str());
                    }
                } else {
                    ht_node1->getCount(key.c_str());
                    ht_node1->updateCount(key.c_str(), 1);
                }
            }
        }

        else if (numa_node == 1)
        {
            if (locality_choice <= local_pct) {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node1->getCount(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node1->updateCount(key.c_str(), 1);
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node1->insert(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node1->getCount(skey.c_str());
                    }
                } else {
                    ht_node1->getCount(key.c_str());
                    ht_node1->updateCount(key.c_str(), 1);
                }
            }
            else {
                int op_choice = op_dist(rng);
                if (op_choice <= cfg->read_pct) {
                    ht_node0->getCount(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct) {
                    ht_node0->updateCount(key.c_str(), 1);
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct) {
                    ht_node0->insert(key.c_str());
                } else if (op_choice <= cfg->read_pct + cfg->update_pct + cfg->insert_pct + cfg->scan_pct) {
                    for (int j = 0; j < 10 && (key_id + j) < (uint64_t)num_keys; j++) {
                        string skey = "key" + to_string(key_id + j);
                        ht_node0->getCount(skey.c_str());
                    }
                } else {
                    ht_node0->getCount(key.c_str());
                    ht_node0->updateCount(key.c_str(), 1);
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

