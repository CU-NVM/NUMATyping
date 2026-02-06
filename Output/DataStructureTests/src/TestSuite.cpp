/*! \file TestSuite.cpp
 * \brief Testsuite implementation which allows for testing of various data structures
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
    #warning "MAX_NODE not defined! Defaulting to 0."
    #define MAX_NODE 1
#endif


using namespace std::chrono;
std::vector<Stack*> Stacks0;
std::vector<Stack*> Stacks1;
// int64_t ops0=0;
// int64_t ops1=0;
int64_t ops0=0;
int64_t ops1=0;

int sharedCounter = 0;


std::vector<mutex*> Stack_lk0;
std::vector<mutex*> Stack_lk1;
pthread_barrier_t bar ;
pthread_barrier_t init_bar;

std::mutex* printLK;
std::mutex* globalLK;



std::vector<int64_t> globalOps0;
std::vector<int64_t> globalOps1;

std::vector<Queue*> Queues0;
std::vector<Queue*> Queues1;
std::vector<mutex*> Queue_lk0;
std::vector<mutex*> Queue_lk1;


std::vector<BinarySearchTree*> BSTs0;
std::vector<BinarySearchTree*> BSTs1;
std::vector<mutex*> BST_lk0;
std::vector<mutex*> BST_lk1;
std::vector<mutex*> BST_reader_lk0;
std::vector<mutex*> BST_reader_lk1;

std::vector<LinkedList*> LLs0;
std::vector<LinkedList*> LLs1;
std::vector<mutex*> LL_lk0;
std::vector<mutex*> LL_lk1;

// chrono::high_resolution_clock::time_point startTimer;
// chrono::high_resolution_clock::time_point endTimer;


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


void numa_BST_single_init(std::string DS_config, int num_DS, int keyspace, int node, int crossover){
	BSTs0.resize(num_DS);
	BSTs1.resize(num_DS);
	BST_lk0.resize(BSTs0.size());
	BST_lk1.resize(BSTs1.size());

	std::mt19937 gen(123);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> dist(0, keyspace/2);
	for(int i = 0; i < num_DS; i++)
	{
		int x = xDist(gen);
		
		if(DS_config=="numa"){
			BSTs0[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, NODE_ZERO>());
			BSTs1[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,MAX_NODE>());
		}
		else{
			BSTs0[i] = new BinarySearchTree();
			BSTs1[i] = new BinarySearchTree();
		}
	}

	
	for(int i = 0; i < num_DS; i++)
	{
		BST_lk0[i] = new mutex();
		BST_lk1[i] = new mutex();
	}	

	
	for(int i = 0; i < num_DS/2 ; i++)
	{
		for(int j=0; j < keyspace/2; j++)
		{
			BSTs0[i]->insert(dist(gen));
			BSTs1[i]->insert(dist(gen));
		}
	}	

}

void numa_BST_init(std::string DS_config, int num_DS, int keyspace, int node, int crossover){
	pthread_barrier_wait(&init_bar);
	crossover = -1;
	//std::cout<<"crossover value from here is "<<crossover<<std::endl;
	if(node==0 ){
		BSTs0.resize(num_DS);
		BSTs1.resize(num_DS);
		BST_lk0.resize(BSTs0.size());
		BST_lk1.resize(BSTs1.size());
		BST_reader_lk0.resize(BSTs0.size());
		BST_reader_lk1.resize(BSTs1.size());
	}
	pthread_barrier_wait(&init_bar);
	std::mt19937 gen(node);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> dist(0, keyspace);
	if(node == 0){
		for(int i = 0; i < num_DS; i++)
		{
			int x = xDist(gen);
			if(x <= crossover){
				if(DS_config=="numa"){
					BSTs1[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,MAX_NODE>());
				}
				else{
					BSTs1[i] = new BinarySearchTree();
				}
			}else{
				if(DS_config=="numa"){
					BSTs0[i] =reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, NODE_ZERO>());
				}
				else{
					BSTs0[i] = new BinarySearchTree();
				}
			}
		}

		
		for(int i = 0; i < num_DS; i++)
		{
			int x = xDist(gen);
			if(x<=crossover){
				BST_lk1[i] = new mutex();
				BST_reader_lk1[i] = new mutex();
			}else{
				BST_lk0[i] = new mutex();
				BST_reader_lk0[i] = new mutex();
			}
		}

    }

	if(node == 1){
		
		for(int i = 0; i < num_DS; i++)
		{
			int x = xDist(gen);
			if(x <= crossover){
				if(DS_config=="numa"){
					BSTs0[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, NODE_ZERO>());
				}
				else{
					BSTs0[i] = new BinarySearchTree();
				}
			}else{
				if(DS_config=="numa"){
					BSTs1[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,MAX_NODE>());
				}
				else{
					BSTs1[i] = new BinarySearchTree();
				}
			}
		}

		for(int i = 0; i < num_DS; i++)
		{	
			int x = xDist(gen);
			if(x<=crossover){
				BST_lk0[i] = new mutex();
			}else{
				BST_lk1[i] = new mutex();
			}
		}	
    }
    pthread_barrier_wait(&init_bar);
    
    if(node == 1){
		
		for(int i = 0; i < keyspace/2 ; i++)
		{
			int x = xDist(gen);
			if(x <= crossover){
				for(int j=0; j < num_DS; j++){
					BSTs0[j]->insert(dist(gen));
				}
			}else{
				for(int j=0; j < num_DS; j++){
					BSTs1[j]->insert(dist(gen));
				}
			}
		}
    }


    if(node == 0){
		for(int i = 0; i < keyspace/2 ; i++)
		{	
			int x = xDist(gen);
			if(x <= crossover){
				for(int j=0; j < num_DS; j++){
					BSTs1[j]->insert(dist(gen));
				}
			}else{
				for(int j=0; j < num_DS; j++){
					BSTs0[j]->insert(dist(gen));
				}
			}
		}	
    }

	pthread_barrier_wait(&init_bar);
	
	std::cout<<"BSTs initialized"<<std::endl<<std::endl<<std::endl<<std::endl;
    pthread_barrier_wait(&init_bar);
}

void BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int crossover, int keyspace, int interval)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);
	//std::cout<<"crossover value from test is "<<crossover<<std::endl;
	std::mt19937 gen(tid);
	std::uniform_int_distribution<> dist(0, BSTs0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> keyDist(0,keyspace);

	int64_t ops;
	thread_local vector<int64_t> localOps;
	localOps.resize(duration/interval);
	int x = xDist(gen);
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
	int intervalIdx = 0;

	while (duration_cast<seconds>(steady_clock::now() - startTimer).count() < duration) {
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
				int txn = opDist(gen);
				if(txn%4==0){
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn%4==1){
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs1[ds_b]->remove(key);
					int level = BSTs0[ds_a]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn%4==2){
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
				int txn = opDist(gen);
				if(txn%4==0){
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs0[ds_a]->remove(key);
					int level = BSTs1[ds_b]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();

					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn%4==1){
					BST_lk0[ds_a]->lock();
					BST_lk1[ds_b]->lock();
					BSTs1[ds_b]->remove(key);
					int level= BSTs0[ds_a]->insert(key);
					// globalLK->lock();
					// std::cout<<"Insert traversed "<<level<<" levels"<<std::endl;
					// globalLK->unlock();
					BST_lk0[ds_a]->unlock();
					BST_lk1[ds_b]->unlock();
				}else if(txn%4==2){
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
		if(std::chrono::steady_clock::now() >= nextLogTime){
			localOps[intervalIdx] = ops;
			intervalIdx++;
			nextLogTime += std::chrono::seconds(interval);
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
