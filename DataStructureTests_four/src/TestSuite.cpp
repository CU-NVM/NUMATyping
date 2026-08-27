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
#include "numathreads.hpp"
#include "umf_numa_allocator.hpp"

#define MEGABYTE 1048576

#ifndef NUM_NUMA_NODES
    #warning "NUM_NUMA_NODES not defined! Defaulting to 4."
    #define NUM_NUMA_NODES 4
#endif


using namespace std::chrono;


// One named global per node, exactly like DataStructureTests but extended to 4.
int64_t ops0 = 0, ops1 = 0, ops2 = 0, ops3 = 0;
std::vector<int64_t> globalOps0, globalOps1, globalOps2, globalOps3;
BinarySearchTree **BSTs0 = nullptr, **BSTs1 = nullptr, **BSTs2 = nullptr, **BSTs3 = nullptr;
std::vector<mutex*> BST_lk0, BST_lk1, BST_lk2, BST_lk3;
std::vector<mutex*> BST_reader_lk0, BST_reader_lk1, BST_reader_lk2, BST_reader_lk3;

int sharedCounter = 0;

pthread_barrier_t bar;
pthread_barrier_t init_bar;

std::mutex* printLK;
std::mutex* globalLK;


int checkNUMANode(void* ptr) {
    int node;
    unsigned long nodemask;

    if (get_mempolicy(&node, &nodemask, sizeof(nodemask) * 8, ptr, MPOL_F_NODE) == 0) {
        return node;
    } else {
        std::cerr << "Failed to get NUMA node for pointer at " << ptr << std::endl;
    }
    return 0;
}


// Ported from DataStructureTests: duration/interval truncates, so any duration
// that is not a multiple of interval under-sizes the ops vectors and every
// worker writes past the end.  duration < interval sized them at ZERO.
int numIntervals(int duration, int interval){
    if(interval <= 0){ return 1; }
    int n = (duration + interval - 1) / interval;   // round up
    return n > 0 ? n : 1;
}

void global_init(int num_threads, int duration, int interval){
    pthread_barrier_init(&bar, NULL, num_threads);
    pthread_barrier_init(&init_bar, NULL, NUM_NUMA_NODES);
    const int n = numIntervals(duration, interval);
    globalOps0.assign(n, 0);
    globalOps1.assign(n, 0);
    globalOps2.assign(n, 0);
    globalOps3.assign(n, 0);
    ops0 = ops1 = ops2 = ops3 = 0;
    printLK = new std::mutex();
    globalLK = new std::mutex();
}


void numa_BST_init(std::string DS_config, int num_DS, int keyspace, int node, int crossover){
    pthread_barrier_wait(&init_bar);
    crossover = -1;
    std::mt19937 gen(123);
    std::uniform_int_distribution<> xDist(1, 100);
    std::uniform_int_distribution<> dist(0, keyspace);

    if(node == 0){
        if(DS_config == "numa"){
            BSTs0 = reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*, 0>[num_DS]);
            BST_lk0.resize(num_DS);
            BST_reader_lk0.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs0[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, 0>());
                BST_lk0[i] = new mutex();
                BST_reader_lk0[i] = new mutex();
            }
        }else{
            BSTs0 = new BinarySearchTree*[num_DS];
            BST_lk0.resize(num_DS);
            BST_reader_lk0.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs0[i] = new BinarySearchTree();
                BST_lk0[i] = new mutex();
                BST_reader_lk0[i] = new mutex();
            }
        }
        for(int i = 0; i < keyspace/2; i++){
            for(int j = 0; j < num_DS; j++){
                BSTs0[j]->insert(dist(gen));
            }
        }
    }
    else if(node == 1){
        if(DS_config == "numa"){
            BSTs1 = reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*, 1>[num_DS]);
            BST_lk1.resize(num_DS);
            BST_reader_lk1.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs1[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, 1>());
                BST_lk1[i] = new mutex();
                BST_reader_lk1[i] = new mutex();
            }
        }else{
            BSTs1 = new BinarySearchTree*[num_DS];
            BST_lk1.resize(num_DS);
            BST_reader_lk1.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs1[i] = new BinarySearchTree();
                BST_lk1[i] = new mutex();
                BST_reader_lk1[i] = new mutex();
            }
        }
        for(int i = 0; i < keyspace/2; i++){
            for(int j = 0; j < num_DS; j++){
                BSTs1[j]->insert(dist(gen));
            }
        }
    }
    else if(node == 2){
        if(DS_config == "numa"){
            BSTs2 = reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*, 2>[num_DS]);
            BST_lk2.resize(num_DS);
            BST_reader_lk2.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs2[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, 2>());
                BST_lk2[i] = new mutex();
                BST_reader_lk2[i] = new mutex();
            }
        }else{
            BSTs2 = new BinarySearchTree*[num_DS];
            BST_lk2.resize(num_DS);
            BST_reader_lk2.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs2[i] = new BinarySearchTree();
                BST_lk2[i] = new mutex();
                BST_reader_lk2[i] = new mutex();
            }
        }
        for(int i = 0; i < keyspace/2; i++){
            for(int j = 0; j < num_DS; j++){
                BSTs2[j]->insert(dist(gen));
            }
        }
    }
    else{
        if(DS_config == "numa"){
            BSTs3 = reinterpret_cast<BinarySearchTree**>(new numa<BinarySearchTree*, 3>[num_DS]);
            BST_lk3.resize(num_DS);
            BST_reader_lk3.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs3[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree, 3>());
                BST_lk3[i] = new mutex();
                BST_reader_lk3[i] = new mutex();
            }
        }else{
            BSTs3 = new BinarySearchTree*[num_DS];
            BST_lk3.resize(num_DS);
            BST_reader_lk3.resize(num_DS);
            for(int i = 0; i < num_DS; i++){
                BSTs3[i] = new BinarySearchTree();
                BST_lk3[i] = new mutex();
                BST_reader_lk3[i] = new mutex();
            }
        }
        for(int i = 0; i < keyspace/2; i++){
            for(int j = 0; j < num_DS; j++){
                BSTs3[j]->insert(dist(gen));
            }
        }
    }

    pthread_barrier_wait(&init_bar);
}


void BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int crossover, int keyspace, int interval)
{
    pthread_barrier_wait(&bar);

    std::mt19937 gen(tid);
    std::uniform_int_distribution<> dist(0, num_DS-1);
    std::uniform_int_distribution<> opDist(1, 100);
    std::uniform_int_distribution<> xDist(1, 100);
    std::uniform_int_distribution<> keyDist(0, keyspace);

    int64_t ops = 0;
    thread_local vector<int64_t> localOps;
    localOps.assign(numIntervals(duration, interval), 0);   // see numIntervals(): truncation sized this at 0

    auto startTimer  = std::chrono::steady_clock::now();
    auto endTimer    = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
    int  intervalIdx = 0;

    while (true) {
        int ds  = dist(gen);
        int key = keyDist(gen);

        // Each node does 80% local lookups and 20% cross-node transactions with
        // its ring neighbour (0->1->2->3->0). Locks are always taken in
        // ascending node order so the ring can never deadlock.
        if(node == 0){
            if(opDist(gen) <= 80){
                BST_lk0[ds]->lock();
                int level = BSTs0[ds]->lookup(key);
                BST_lk0[ds]->unlock();
            }else{
                int ds_a = dist(gen);
                int ds_b = dist(gen);
                int txn = opDist(gen);
                if(txn%4==0){          // remove local(0), insert neighbour(1)
                    BST_lk0[ds_a]->lock();
                    BST_lk1[ds_b]->lock();
                    BSTs0[ds_a]->remove(key);
                    int level = BSTs1[ds_b]->insert(key);
                    BST_lk0[ds_a]->unlock();
                    BST_lk1[ds_b]->unlock();
                }else if(txn%4==1){    // remove neighbour(1), insert local(0)
                    BST_lk0[ds_a]->lock();
                    BST_lk1[ds_b]->lock();
                    BSTs1[ds_b]->remove(key);
                    int level = BSTs0[ds_a]->insert(key);
                    BST_lk0[ds_a]->unlock();
                    BST_lk1[ds_b]->unlock();
                }else if(txn%4==2){    // both local(0)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk0[lk1]->lock();
                    BST_lk0[lk2]->lock();
                    BSTs0[ds_a]->remove(key);
                    int level = BSTs0[ds_b]->insert(key);
                    BST_lk0[lk1]->unlock();
                    BST_lk0[lk2]->unlock();
                }else{                 // both neighbour(1)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk1[lk1]->lock();
                    BST_lk1[lk2]->lock();
                    BSTs1[ds_a]->remove(key);
                    int level = BSTs1[ds_b]->insert(key);
                    BST_lk1[lk1]->unlock();
                    BST_lk1[lk2]->unlock();
                }
            }
        }
        else if(node == 1){
            if(opDist(gen) <= 80){
                BST_lk1[ds]->lock();
                int level = BSTs1[ds]->lookup(key);
                BST_lk1[ds]->unlock();
            }else{
                int ds_a = dist(gen);
                int ds_b = dist(gen);
                int txn = opDist(gen);
                if(txn%4==0){          // remove local(1), insert neighbour(2)
                    BST_lk1[ds_a]->lock();
                    BST_lk2[ds_b]->lock();
                    BSTs1[ds_a]->remove(key);
                    int level = BSTs2[ds_b]->insert(key);
                    BST_lk1[ds_a]->unlock();
                    BST_lk2[ds_b]->unlock();
                }else if(txn%4==1){    // remove neighbour(2), insert local(1)
                    BST_lk1[ds_a]->lock();
                    BST_lk2[ds_b]->lock();
                    BSTs2[ds_b]->remove(key);
                    int level = BSTs1[ds_a]->insert(key);
                    BST_lk1[ds_a]->unlock();
                    BST_lk2[ds_b]->unlock();
                }else if(txn%4==2){    // both local(1)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk1[lk1]->lock();
                    BST_lk1[lk2]->lock();
                    BSTs1[ds_a]->remove(key);
                    int level = BSTs1[ds_b]->insert(key);
                    BST_lk1[lk1]->unlock();
                    BST_lk1[lk2]->unlock();
                }else{                 // both neighbour(2)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk2[lk1]->lock();
                    BST_lk2[lk2]->lock();
                    BSTs2[ds_a]->remove(key);
                    int level = BSTs2[ds_b]->insert(key);
                    BST_lk2[lk1]->unlock();
                    BST_lk2[lk2]->unlock();
                }
            }
        }
        else if(node == 2){
            if(opDist(gen) <= 80){
                BST_lk2[ds]->lock();
                int level = BSTs2[ds]->lookup(key);
                BST_lk2[ds]->unlock();
            }else{
                int ds_a = dist(gen);
                int ds_b = dist(gen);
                int txn = opDist(gen);
                if(txn%4==0){          // remove local(2), insert neighbour(3)
                    BST_lk2[ds_a]->lock();
                    BST_lk3[ds_b]->lock();
                    BSTs2[ds_a]->remove(key);
                    int level = BSTs3[ds_b]->insert(key);
                    BST_lk2[ds_a]->unlock();
                    BST_lk3[ds_b]->unlock();
                }else if(txn%4==1){    // remove neighbour(3), insert local(2)
                    BST_lk2[ds_a]->lock();
                    BST_lk3[ds_b]->lock();
                    BSTs3[ds_b]->remove(key);
                    int level = BSTs2[ds_a]->insert(key);
                    BST_lk2[ds_a]->unlock();
                    BST_lk3[ds_b]->unlock();
                }else if(txn%4==2){    // both local(2)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk2[lk1]->lock();
                    BST_lk2[lk2]->lock();
                    BSTs2[ds_a]->remove(key);
                    int level = BSTs2[ds_b]->insert(key);
                    BST_lk2[lk1]->unlock();
                    BST_lk2[lk2]->unlock();
                }else{                 // both neighbour(3)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk3[lk1]->lock();
                    BST_lk3[lk2]->lock();
                    BSTs3[ds_a]->remove(key);
                    int level = BSTs3[ds_b]->insert(key);
                    BST_lk3[lk1]->unlock();
                    BST_lk3[lk2]->unlock();
                }
            }
        }
        else{
            // node 3 wraps to neighbour 0; lock node 0 first to keep ascending order.
            if(opDist(gen) <= 80){
                BST_lk3[ds]->lock();
                int level = BSTs3[ds]->lookup(key);
                BST_lk3[ds]->unlock();
            }else{
                int ds_a = dist(gen);
                int ds_b = dist(gen);
                int txn = opDist(gen);
                if(txn%4==0){          // remove local(3), insert neighbour(0)
                    BST_lk0[ds_b]->lock();
                    BST_lk3[ds_a]->lock();
                    BSTs3[ds_a]->remove(key);
                    int level = BSTs0[ds_b]->insert(key);
                    BST_lk0[ds_b]->unlock();
                    BST_lk3[ds_a]->unlock();
                }else if(txn%4==1){    // remove neighbour(0), insert local(3)
                    BST_lk0[ds_b]->lock();
                    BST_lk3[ds_a]->lock();
                    BSTs0[ds_b]->remove(key);
                    int level = BSTs3[ds_a]->insert(key);
                    BST_lk0[ds_b]->unlock();
                    BST_lk3[ds_a]->unlock();
                }else if(txn%4==2){    // both local(3)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk3[lk1]->lock();
                    BST_lk3[lk2]->lock();
                    BSTs3[ds_a]->remove(key);
                    int level = BSTs3[ds_b]->insert(key);
                    BST_lk3[lk1]->unlock();
                    BST_lk3[lk2]->unlock();
                }else{                 // both neighbour(0)
                    if(ds_a==ds_b){continue;}
                    int lk1 = (ds_a<ds_b)?ds_a:ds_b;
                    int lk2 = (ds_a<ds_b)?ds_b:ds_a;
                    BST_lk0[lk1]->lock();
                    BST_lk0[lk2]->lock();
                    BSTs0[ds_a]->remove(key);
                    int level = BSTs0[ds_b]->insert(key);
                    BST_lk0[lk1]->unlock();
                    BST_lk0[lk2]->unlock();
                }
            }
        }

        ops++;
        if(ops % 1024 == 0){
            if(std::chrono::steady_clock::now() >= nextLogTime){
                localOps[intervalIdx] = ops;
                intervalIdx++;
                nextLogTime += std::chrono::seconds(interval);
            }
            if(std::chrono::steady_clock::now() >= endTimer){
                localOps[intervalIdx] = ops;
                break;
            }
        }
    }

    globalLK->lock();
    if(node == 0){
        for(int i = 0; i < (int)localOps.size(); i++){ globalOps0[i] += localOps[i]; }
        ops0 = globalOps0[globalOps0.size()-1];
    }
    else if(node == 1){
        for(int i = 0; i < (int)localOps.size(); i++){ globalOps1[i] += localOps[i]; }
        ops1 = globalOps1[globalOps1.size()-1];
    }
    else if(node == 2){
        for(int i = 0; i < (int)localOps.size(); i++){ globalOps2[i] += localOps[i]; }
        ops2 = globalOps2[globalOps2.size()-1];
    }
    else{
        for(int i = 0; i < (int)localOps.size(); i++){ globalOps3[i] += localOps[i]; }
        ops3 = globalOps3[globalOps3.size()-1];
    }
    globalLK->unlock();

    pthread_barrier_wait(&bar);
}


void global_cleanup(){
}
