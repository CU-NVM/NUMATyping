/*! \file array_main.cpp
 *  \brief Main file for array test
 *  \author Kidus Workneh
 *  \date 2026 
*/

#include "Array.h"
#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <string>
#include <vector>
#include <cmath>
#include <getopt.h>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "numathreads.hpp"

using namespace std;

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 1
#endif



string thread_config;
string DS_config;
int num_threads = 0;
int duration = 0;
int run_freq = 1;
int interval =20;
bool verbose = false;
int64_t array_size = 100000000; //100 million
int num_arrays = 2;


std::vector <thread_numa<NODE_ZERO>*> numa_thread0;
std::vector <thread_numa<NODE_ONE>*> numa_thread1;
vector<thread*> regular_thread0;
vector<thread*> regular_thread1;
vector<thread_numa<NODE_ZERO>*> init_thread0;
vector<thread_numa<NODE_ONE>*> init_thread1;
std::thread* init_thread_regular0;
std::thread* init_thread_regular1;



extern int64_t ops0;
extern int64_t ops1;
extern std::vector<int64_t> globalOps0;
extern std::vector<int64_t> globalOps1;
std::vector <int64_t> num_ops1;
std::vector <int64_t> num_ops0;
std::vector <int64_t> total_ops;



void spawn_threads(){
	numa_thread0.assign(num_threads/2, nullptr);
	numa_thread1.assign(num_threads/2, nullptr);
	regular_thread0.assign(num_threads/2, nullptr);
	regular_thread1.assign(num_threads/2, nullptr);
	init_thread0.assign(num_threads/2, nullptr);
	init_thread1.assign(num_threads/2, nullptr);
    global_init(num_threads, duration, interval);
	//Initialization
	#ifdef PIN_INIT
		for(int i=0; i< num_threads/2; ++i)
		{   int thread_id = i;
			int numa_node = 0;
			init_thread0[i] = new thread_numa<NODE_ZERO>(numa_array_init, thread_id , num_threads, DS_config, array_size, numa_node, num_arrays/2);
		}

		for(int i=0; i< num_threads/2; ++i)
		{   
			int thread_id = i + num_threads/2;
			int numa_node = 1;
			init_thread1[i] = new thread_numa<NODE_ONE>(numa_array_init, thread_id , num_threads, DS_config, array_size, numa_node, num_arrays/2);
		}

	#else

	#endif


    #ifdef PIN_INIT 
        for(auto th : init_thread0) th->join();
        for(auto th : init_thread1) th->join();
        for(auto th : init_thread0) delete th;
        for(auto th : init_thread1) delete th;
    #else
        init_thread_regular0->join();
        init_thread_regular1->join();
        delete init_thread_regular0;
        delete init_thread_regular1;
    #endif
	//End Initialization
 
	// ------------------ TEST SPAWNING ------------------

	//Test
	for(int i=0; i < num_threads/2; i++){
		int node = 0;
		int tid = i;
		if(thread_config == "numa"){
			numa_thread0[i] = new thread_numa<NODE_ZERO>(array_test, tid, duration, DS_config, node, num_threads/2, array_size, num_arrays/2, interval);
		}
		else if(thread_config == "regular"){
			regular_thread0[i] = new thread(array_test, tid, duration, DS_config, node, num_threads/2, array_size, num_arrays/2, interval);
		}
		else{
			numa_thread0[i] = new thread_numa<NODE_ZERO>(array_test, tid, duration, DS_config, 1, num_threads/2, array_size, num_arrays/2, interval);
		}
	}

	for(int i=0; i < num_threads/2; i++){
		int node = 1;
		int tid = i + num_threads/2;
		if(thread_config == "numa"){
			numa_thread1[i] = new thread_numa<NODE_ONE>(array_test, tid, duration, DS_config, node, num_threads/2, array_size, num_arrays/2, interval);
		}
		else if(thread_config == "regular"){
			regular_thread1[i] = new thread(array_test, tid, duration, DS_config, node, num_threads/2, array_size, num_arrays/2, interval);
		}
		else{
			numa_thread1[i] = new thread_numa<NODE_ONE>(array_test, tid, duration, DS_config, 0, num_threads/2, array_size, num_arrays/2, interval);
		}
	}

	if(thread_config == "numa"){
		for(int i=0; i < numa_thread0.size(); i++){
			if(numa_thread0[i] == nullptr){
				continue;
			}
			numa_thread0[i]->join();
		}
		for(int i=0; i < numa_thread1.size(); i++){
			if(numa_thread1[i] == nullptr){
				continue;
			}
			numa_thread1[i]->join();
		}
	}
	else if(thread_config == "regular"){
		for(int i=0; i < regular_thread0.size(); i++){
			if(regular_thread0[i] == nullptr){
				continue;
			}else{
				regular_thread0[i]->join();
			}
			
		}
		for(int i=0; i < regular_thread1.size(); i++){
			if(regular_thread1[i] == nullptr){
				continue;
			}else{
				regular_thread1[i]->join();
			}
		}
	}
	else{
		for(int i=0; i < numa_thread0.size(); i++){
			if(numa_thread0[i] == nullptr){
				continue;
			}
			numa_thread0[i]->join();
		}
		for(int i=0; i < numa_thread1.size(); i++){
			if(numa_thread1[i] == nullptr){
				continue;
			}
			numa_thread1[i]->join();
		}
	}
}

void compile_options(int argc, char *argv[]){
static struct option long_options[] = {
		{"th_config", required_argument, nullptr, 'c'},     // --th_config=NUMA/REGULAR
		{"DS_config", required_argument, nullptr, 'd'},     // --DS_config=NUMA/REGULAR
		{"num_threads", required_argument, nullptr, 't'},   // -t
		{"duration", required_argument, nullptr, 'D'},      // -d
		{"run_freq", optional_argument, nullptr, 'f'},       // -p
		{"array_size", required_argument, nullptr, 's'},      // -k
		{"interval", required_argument, nullptr, 'i'},      // -i
		{"num_arrays", required_argument, nullptr, 'n'},      // -a
		{"verbose", no_argument, nullptr, 'v'},        // --verbose
		{"help", no_argument, nullptr, 'h'},           // --help
		{nullptr, 0, nullptr, 0}                            // End of array
	};

 int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:n:f:u:s:i:n:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':  // --th_config option
                thread_config = optarg;             
                break;
            case 'd':  // --DS_config option
                DS_config = optarg;
                break;
                break;
            case 't':  // -t option for num_threads
                num_threads = std::stoll(optarg);
				regular_thread0.resize(num_threads/2);
				regular_thread1.resize(num_threads/2);
				numa_thread0.resize(num_threads/2);
				numa_thread1.resize(num_threads/2);
                break;
			case 'f':
				if(optarg){
					run_freq = std::stoi(optarg);
				}
				break;
			case 's':
				if(optarg){
					array_size = std::stoll(optarg);
				}
				break;
			case 'n': // -n option for num_arrays
				if(optarg){
					num_arrays = std::stoll(optarg);
				}
				break;
			case 'v':  // --verbose flag
				verbose = true;
				break;		
			case 'u':  // -u duration option
				duration = std::stoll(optarg);
				break;		
            case '?':  // Unknown option
                std::cerr << "Unknown option or missing argument.\n";
                return ;
            default:
                break;
        }
    }
}


void print_header() {
    std::cout << "Date, Time, Num_Tables, Num_Threads, Thread_Config, DS_Config, Buckets, Workload, Duration(s), Num_Keys, Locality, Interval(s), Ops_Node0, Ops_Node1, Total_Ops\n";
}


void print_function(int duration, int64_t ops0, int64_t ops1, int64_t totalOps) {
	auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::cout<<std::put_time(local_time, "%Y-%m-%d") << ", ";
	std::cout<<std::put_time(local_time, "%H:%M:%S") <<", ";
	std::cout<<num_arrays << ", ";
	std::cout<<num_threads << ", ";
	std::cout<<thread_config << ", ";
	std::cout<<DS_config << ", ";
	std::cout<<duration << ", ";
	std::cout<<array_size << ", ";
	std::cout<<interval<<", ";
	std::cout<<ops0 << ", ";
	std::cout<<ops1 << ", ";
	std::cout<<totalOps << "\n";
}


int main(int argc, char *argv[])
{
    compile_options(argc, argv);

    if (thread_config == "numa" || DS_config == "numa") {
        if (numa_num_configured_nodes() == 1) {
            std::cout << "NUMA not available or only one node configured. Running in regular mode.\n";
            thread_config = "regular";
            DS_config = "regular";
        }
    }

    // print_header();
    print_function(0, 0, 0, 0); 

    spawn_threads();

    int64_t sum0 = 0;
	int64_t sum1 = 0;
	int64_t total_sum = 0;
	//get the average of the numbers in the array
	for(int i = 0; i < num_ops0.size(); i++){
		sum0 += num_ops0[i];	
	}
	for(int i = 0; i < num_ops1.size(); i++){
		sum1 += num_ops1[i];
	}
	for(int i = 0; i < total_ops.size(); i++){
		total_sum += total_ops[i];
	}
	int newDuration = interval;
	for(int i = 0; i< globalOps0.size(); i++){
		print_function(newDuration, globalOps0[i], globalOps1[i], globalOps0[i] + globalOps1[i]);
		newDuration += interval;
	}

    return 0;
}