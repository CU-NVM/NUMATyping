#include "Array.h"
#include "numatype.hpp"
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
#include <cstring>
#include <map>
#include <atomic>

using namespace std;
using namespace std::chrono;

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 1
#endif
int global_successful_inserts;
int global_successful_init_inserts;
std::vector<char**> array_node0;
std::vector<char**> array_node1;
std::vector<std::mutex*> array_node0_locks;
std::vector<std::mutex*> array_node1_locks;

std::mutex* printLK;
std::mutex* globalLK;

std::vector<int64_t> globalOps0;
std::vector<int64_t> globalOps1;
int64_t ops0=0;
int64_t ops1=0;
pthread_barrier_t bar;
pthread_barrier_t init_bar;



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
void numa_array_init(int thread_id, int num_total_threads, std::string DS_config, int64_t array_size, int node, int num_arrays)
{
    int threads_per_node = num_total_threads / 2;

    // ------------------ GLOBAL RESIZE (ONCE) ------------------
    if (thread_id == 0) {
        array_node0.resize(num_arrays);
        array_node1.resize(num_arrays);
        
        array_node0_locks.resize(num_arrays);
        array_node1_locks.resize(num_arrays); 
        for (int i = 0; i < num_arrays; i++) {
            array_node0_locks[i] = new std::mutex();
            array_node1_locks[i] = new std::mutex();
        }
    }
    pthread_barrier_wait(&init_bar);

    // ------------------ GLOBAL ALLOCATION (ONCE) ------------------
    if(node == 0 && thread_id % threads_per_node == 0) {
        for (int i = 0; i < num_arrays; i++) {
            if (DS_config != "regular") {
                array_node0[i] = reinterpret_cast<char**> (new numa<char*, NODE_ZERO>[array_size]);
                
            } else if(DS_config == "regular"){
                array_node0[i] = new char*[array_size];
            }
        }
    }
   else if (node == 1 && thread_id % threads_per_node == 0) {
        for (int i = 0; i < num_arrays; i++) {
            if (DS_config != "regular") {
                array_node1[i] = reinterpret_cast<char**> (new numa<char*, NODE_ONE>[array_size]);
            }
            else if(DS_config == "regular"){
                array_node1[i] = new char*[array_size];
            }
        }
    }
    pthread_barrier_wait(&init_bar);


    // ------------------ SANITY CHECK ------------------
    if (thread_id == 0) {
        for (int i = 0; i < num_arrays; i++) {
            if (array_node0[i] == nullptr || array_node1[i] == nullptr || array_node0_locks[i] == nullptr || array_node1_locks[i] == nullptr) {
                std::cerr << "Hash table allocation error!" << std::endl;
                return;
            }
        }
    }
    pthread_barrier_wait(&init_bar);

    //random number generator for keys
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)) + thread_id);
    std::uniform_int_distribution<int> key_dist(1, array_size*10);

    int local_thread_id = thread_id % threads_per_node;
    long long chunk = num_arrays / threads_per_node;
    long long start = local_thread_id * chunk;
    long long end = (local_thread_id == threads_per_node - 1) ? num_arrays : start + chunk;
    int64_t iterations = array_size;
    char* word;
    const char* letters;

    for(long long i=start; i < end; ++i){
        for (long long j = 0; j < iterations; ++j) {
            if (node == 0) {
                if(DS_config == "regular"){
                    std::string temp = "key" + std::to_string(key_dist(rng));
                    letters= temp.c_str();
                    word = new char[strlen(letters) + 1];
                }
                else {
                    std::string temp = "key" + std::to_string(key_dist(rng));
                    letters= temp.c_str();
                    word = reinterpret_cast<char *>(reinterpret_cast<char *>(new numa<char,0>[strlen(letters) + 1]));
                }
                array_node0[i][j] = word;
            } else if (node == 1) {
                if(DS_config == "regular"){
                    std::string temp = "key" + std::to_string(key_dist(rng));
                    letters= temp.c_str();
                    word = new char[strlen(letters) + 1];
                }
                else {
                    std::string temp = "key" + std::to_string(key_dist(rng));
                    letters= temp.c_str();
                    word = reinterpret_cast<char *>(reinterpret_cast<char *>(new numa<char,1>[strlen(letters) + 1]));
                }
                array_node1[i][j] = word;
            }
        }
    }

}


void array_test(int tid, int duration, std::string DS_config, int node, int num_threads,  int64_t array_size, int num_arrays, int interval)
{
   
	pthread_barrier_wait(&bar);

	int64_t ops = 0;
	thread_local vector<int64_t> localOps;
	localOps.resize(duration/interval);
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
    auto nextLogTime = startTimer + std::chrono::seconds(interval);
	int intervalIdx = 0;

    std::random_device rd;
    std::mt19937_64 rng(tid);
    std::uniform_int_distribution<long long> op_dist(1, 100);
    std::uniform_int_distribution<long long> array_dist(0, num_arrays - 1);
    std::uniform_int_distribution<long long> word_dist(0, array_size - 1);

    const char* letters;
    char* word;
    while (duration_cast<seconds>(steady_clock::now() - startTimer).count() < duration) {
        int array_choice = array_dist(rng);
        int word_choice = word_dist(rng);
        int op_choice = op_dist(rng);

        if (node == 0) {
            if (op_choice <= 80) {
                array_node0_locks[array_choice]->lock();
                word = array_node0[array_choice][word_choice];
                array_node0_locks[array_choice]->unlock();
            } 
            else {
                // 1. Prepare the new string OUTSIDE the lock to keep the critical section small
                std::string temp = "key" + std::to_string(word_dist(rng));
                const char* letters = temp.c_str();
                size_t len = strlen(letters) + 1;
                char* new_word;

                if (DS_config != "regular") {
                    new_word = reinterpret_cast<char*>(new numa<char, 0>[len]);
                } else {
                    new_word = new char[len];
                }
                std::strcpy(new_word, letters);

                // 2. Lock, swap pointers, and capture the old one
                array_node0_locks[array_choice]->lock();
                array_node0[array_choice][word_choice] = new_word;      // Update with new pointer
                array_node0_locks[array_choice]->unlock();
            }
        } else {
            if (op_choice <= 80) {
                array_node1_locks[array_choice]->lock();
                word = array_node1[array_choice][word_choice];
                array_node1_locks[array_choice]->unlock();
            } 
            else{
                // 1. Prepare the new string OUTSIDE the lock to keep the critical section small
                std::string temp = "key" + std::to_string(word_dist(rng));
                const char* letters = temp.c_str();
                size_t len = strlen(letters) + 1;
                char* new_word;

                if (DS_config != "regular") {
                    new_word = reinterpret_cast<char*>(new numa<char, 1>[len]);
                } else {
                    new_word = new char[len];
                }
                std::strcpy(new_word, letters);

                // 2. Lock, swap pointers, and capture the old one
    
                array_node1_locks[array_choice]->lock();
                array_node1[array_choice][word_choice] = new_word;      // Update with new pointer
                array_node1_locks[array_choice]->unlock();
            }
        }


        ops++;
        if(std::chrono::steady_clock::now() >= nextLogTime){
            localOps[intervalIdx] = ops;
            intervalIdx++;
            nextLogTime += std::chrono::seconds(interval);
        }
    }


    pthread_barrier_wait(&bar);   


    globalLK->lock();
    if(node==0)
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

}