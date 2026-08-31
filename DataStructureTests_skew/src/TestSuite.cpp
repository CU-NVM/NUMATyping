/*Testsuite implementation which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "Node.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearch.hpp"
#include "LinkedList.hpp"
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <syncstream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <atomic>
#include "umf_numa_allocator.hpp"

#define MEGABYTE 1048576

#define NODE_ZERO 0

#ifndef MAX_NODE
    #warning "MAX_NODE not defined! Defaulting to 1."
    #define MAX_NODE 1
#endif


using namespace std::chrono;


int64_t ops0=0;
int64_t ops1=0;

int sharedCounter = 0;


pthread_barrier_t bar ;
pthread_barrier_t init_bar;

std::mutex* printLK;
std::mutex* globalLK;
std::vector<int64_t> globalOps0;
std::vector<int64_t> globalOps1;
BinarySearchTree** BSTs0;
BinarySearchTree** BSTs1;
std::vector<mutex*> BST_lk0;
std::vector<mutex*> BST_lk1;
std::vector<mutex*> BST_reader_lk0;
std::vector<mutex*> BST_reader_lk1;


int checkNUMANode(void* ptr) {
    int node;
    unsigned long nodemask;

    if (get_mempolicy(&node, &nodemask, sizeof(nodemask) * 8, ptr, MPOL_F_NODE) == 0) {
        // std::cout << "Pointer at " << ptr << " is allocated on NUMA Node " << node << std::endl;
		return node;
    } else {
        std::cerr << "Failed to get NUMA node for pointer at " << ptr << std::endl;
    }
	return 0;
}



// Number of sampling buckets for a run. Integer division alone truncates to 0
// whenever duration < interval (e.g. -D 5 with the default -i 20), which left
// the ops vectors empty and every localOps[]/globalOps[] access out of bounds.
int numIntervals(int duration, int interval){
	if(interval <= 0){ return 1; }
	int n = (duration + interval - 1) / interval;   // round up
	return n > 0 ? n : 1;
}

void global_init(int num_threads, int duration, int interval){
	pthread_barrier_init(&bar, NULL, num_threads);
	pthread_barrier_init(&init_bar, NULL, 2);
	globalOps0.resize(numIntervals(duration, interval));
	globalOps1.resize(numIntervals(duration, interval));
	ops0 = 0;
	ops1 = 0;
	printLK = new std::mutex();
	globalLK = new std::mutex();
}



// ---------------------------------------------------------------- txn mix
// DataStructureTests picked the transaction kind with `opDist(gen) % 4`, which
// over a uniform 1..100 draw is exactly 25 values per residue -- a fixed
// 25/25/25/25 split of 0->0, 0->1, 1->0, 1->1.  Here the four weights are
// configurable so the mix can be skewed, either toward cross-node traffic or
// asymmetrically (a net flow from one node to the other).
//
//   index 0 = 0->0   index 1 = 0->1   index 2 = 1->0   index 3 = 1->1
//
// txn_mix defaults to 25,25,25,25, which reproduces DataStructureTests exactly.
int txnWeights[4] = {25, 25, 25, 25};
static int txnCum[4] = {25, 50, 75, 100};

void set_txn_mix(int w00, int w01, int w10, int w11){
    txnWeights[0]=w00; txnWeights[1]=w01; txnWeights[2]=w10; txnWeights[3]=w11;
    int acc = 0;
    for(int i=0;i<4;i++){ acc += txnWeights[i]; txnCum[i] = acc; }
    if(acc <= 0){   // degenerate input: fall back to the uniform mix
        txnWeights[0]=txnWeights[1]=txnWeights[2]=txnWeights[3]=25;
        txnCum[0]=25; txnCum[1]=50; txnCum[2]=75; txnCum[3]=100;
    }
}
int txn_mix_total(){ return txnCum[3]; }

// r is a uniform draw in [1, txn_mix_total()]
static inline int txn_kind(int r){
    if(r <= txnCum[0]) return 0;
    if(r <= txnCum[1]) return 1;
    if(r <= txnCum[2]) return 2;
    return 3;
}

void numa_BST_init(std::string DS_config, int num_DS, int keyspace, int node){
	pthread_barrier_wait(&init_bar);
	std::mt19937 gen(123);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> dist(0, keyspace);
	if(node == 0){
        if(DS_config=="numa"){
            BSTs0 = reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*,NODE_ZERO>[num_DS]);
            BST_lk0.resize(num_DS);
            BST_reader_lk0.resize(num_DS);
            for(int i= 0; i < num_DS; i++){
                BSTs0[i]= reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,NODE_ZERO>());
                BST_lk0[i]= new mutex();
                BST_reader_lk0[i]= new mutex();
            }
        

        }else{
            BSTs0 = new BinarySearchTree*[num_DS];
            BST_lk0.resize(num_DS);
            BST_reader_lk0.resize(num_DS);
            for(int i= 0; i < num_DS; i++){
                BSTs0[i]= new BinarySearchTree();
                BST_lk0[i]= new mutex();
                BST_reader_lk0[i]= new mutex(); 
            }
        }
        for(int i = 0; i < keyspace/2 ; i++){	
			for(int j=0; j < num_DS; j++){
				BSTs0[j]->insert(dist(gen));
			}
		}
        

    }
    else{
        if(DS_config=="numa"){
            BSTs1= reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*,MAX_NODE>[num_DS]);
            BST_lk1.resize(num_DS);
            BST_reader_lk1.resize(num_DS);
            for(int i= 0; i < num_DS; i++){
                BSTs1[i]= reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,MAX_NODE>());
                BST_lk1[i]= new mutex();
                BST_reader_lk1[i]= new mutex();
            }  
    

        }else{
            BSTs1 = new BinarySearchTree*[num_DS];
            BST_lk1.resize(num_DS);
            BST_reader_lk1.resize(num_DS);
            for(int i= 0; i < num_DS; i++){
                BSTs1[i]= new BinarySearchTree();
                BST_lk1[i]= new mutex();
                BST_reader_lk1[i]= new mutex(); 
            }

        }
              for(int i = 0; i < keyspace/2 ; i++){	
			for(int j=0; j < num_DS; j++){
				BSTs1[j]->insert(dist(gen));
			}
		}

    }
    pthread_barrier_wait(&init_bar);
}
	

void BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int keyspace, int interval)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);
	std::mt19937 gen(tid);
	std::uniform_int_distribution<> dist(0, num_DS-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> keyDist(0,keyspace);

	int64_t ops;
	thread_local vector<int64_t> localOps;
	localOps.assign(numIntervals(duration, interval), 0);
	int x = xDist(gen);
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
	int intervalIdx = 0;

	while (true) {
		int ds = dist(gen);


		int key = keyDist(gen);
		if(node==0){
			if(opDist(gen)<=80)
			{

				BST_lk0[ds]->lock();
				int level = BSTs0[ds]->lookup(key);
				// globalLK->lock();
				// std::cout<<"Look up traversed "<<level<<" levels"<<std::endl;
				// globalLK->unlock();
				BST_lk0[ds]->unlock();
			
			}
			else {
				int ds_a= dist(gen);
				int ds_b = dist(gen);
				std::uniform_int_distribution<> mixDist(1, txn_mix_total());
				int txn = txn_kind(mixDist(gen));   // 0:0->0  1:0->1  2:1->0  3:1->1
				if(txn==1){          // 0 -> 1  (cross-node)
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn==2){    // 1 -> 0  (cross-node)
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs1[ds_b]->remove(key);
					int level = BSTs0[ds_a]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn==0){    // 0 -> 0  (same node)
					int lk1 = (ds_a<ds_b)?ds_a:ds_b;
					int lk2 = (ds_a<ds_b)?ds_b:ds_a;
					if(ds_a==ds_b){continue;}
					BST_lk0[lk1]->lock();
					BST_lk0[lk2]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs0[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[lk1]->unlock();
					BST_lk0[lk2]->unlock();
				}else{
					int lk1 = (ds_a<ds_b)?ds_a:ds_b;
					int lk2 = (ds_a<ds_b)?ds_b:ds_a;
					if(ds_a==ds_b){continue;}
					BST_lk1[lk1]->lock();
					BST_lk1[lk2]->lock();
					BSTs1[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk1[lk1]->unlock();
					BST_lk1[lk2]->unlock();
				}
			}
		}
		else{
			if(opDist(gen)<=80)
			{
				BST_lk1[ds]->lock();
				int level = BSTs1[ds]->lookup(key);
				// globalLK->lock();
				// std::cout<<"Look up traversed "<<level<<" levels"<<std::endl;
				// globalLK->unlock();
				BST_lk1[ds]->unlock();
			}
			else {
				int ds_a= dist(gen);
				int ds_b = dist(gen);
				std::uniform_int_distribution<> mixDist(1, txn_mix_total());
				int txn = txn_kind(mixDist(gen));   // 0:0->0  1:0->1  2:1->0  3:1->1
				if(txn==1){          // 0 -> 1  (cross-node)
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn==2){    // 1 -> 0  (cross-node)
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs1[ds_b]->remove(key);
					int level= BSTs0[ds_a]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();
					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn==0){    // 0 -> 0  (same node)
					int lk1 = ds_a<ds_b?ds_a:ds_b;
					int lk2 = ds_a<ds_b?ds_b:ds_a;
					if(ds_a==ds_b){continue;}
					BST_lk0[lk1]->lock();
					BST_lk0[lk2]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs0[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();
					BST_lk0[lk1]->unlock();
					BST_lk0[lk2]->unlock();
				}else{
					int lk1 = ds_a<ds_b?ds_a:ds_b;
					int lk2 = ds_a<ds_b?ds_b:ds_a;
					if(ds_a==ds_b){continue;}
					BST_lk1[lk1]->lock();
					BST_lk1[lk2]->lock();
					BSTs1[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();
					BST_lk1[lk1]->unlock();
					BST_lk1[lk2]->unlock();
				}
			}
		}
		ops++;
		if(ops % 1024 == 0){
			if(std::chrono::steady_clock::now() >= nextLogTime){
				// Clamp: a slow final interval can fire more often than there
				// are buckets.
				if(intervalIdx < (int)localOps.size() - 1){
					localOps[intervalIdx] = ops;
					intervalIdx++;
				}
				nextLogTime += std::chrono::seconds(interval);
			}
			if(std::chrono::steady_clock::now() >= endTimer){
				localOps[intervalIdx] = ops;
				break;
			}
		}
		
	}


	globalLK->lock();
	if(node==0)
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



void global_cleanup(){
}

