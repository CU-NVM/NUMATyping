/*in file for array test
 * \author Kidus Workneh
 * \date 2026 
*/

#include "Array.h"
#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <vector>
#include <cmath>
#include <getopt.h>
#include <chrono>
#include <iomanip>
#include "numathreads.hpp"

using namespace std;

#define NODE_ZERO 0
#ifndef MAX_NODE
    #warning "MAX_NODE_ID not defined! Defaulting to 1."
    #define MAX_NODE 1
#endif

// Configuration Globals
string thread_config;
string DS_config;
int num_threads = 0;
int duration = 10;
int interval = 0; 
bool verbose = false;
int64_t array_size = 1000000; 
int num_arrays = 1000;

// Thread Containers
std::vector <thread_numa<NODE_ZERO>*> numa_thread0;
std::vector <thread_numa<MAX_NODE>*> numa_thread1;
vector<thread*> regular_thread0;
vector<thread*> regular_thread1;
thread_numa<NODE_ZERO>* init0;
thread_numa<MAX_NODE>* init1;




void print_header(){
	std::cout << "Timestamp, Th_Config, DS_Config, Threads, Time(s), Ops_Node0, Ops_Node1, Total_Ops, Throughput(ops/s)\n";
}
void print_result(int elapsed_time, int64_t ops0, int64_t ops1) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    

    int64_t total = ops0 + ops1;
    int64_t throughput = (interval > 0) ? total / interval : 0;

    std::cout << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << ", "
			  << thread_config << ", "
			  << DS_config << ", "
			  << num_threads << ", "
			  << elapsed_time << ", "
			  << ops0 << ", "
			  << ops1 << ", "
			  << total << ", "
			  << throughput
			  << std::endl;
}

void spawn_threads() {
    numa_thread0.resize(num_threads/2);
    numa_thread1.resize(num_threads/2);
    regular_thread0.resize(num_threads/2);
    regular_thread1.resize(num_threads/2);

    // 1. Initialize Sync Primitives Only
    global_init(num_threads, duration, interval);
    
    // 2. Run Initialization Threads 
    init0 = new thread_numa<NODE_ZERO>(numa_array_init, 0, num_threads, DS_config, array_size, 0, num_arrays/2, duration, interval);
    init1 = new thread_numa<MAX_NODE>(numa_array_init, 1, num_threads, DS_config, array_size, 1, num_arrays/2, duration, interval);
    
    if(init0) init0->join();
    if(init1) init1->join();

    // 3. Spawn Test Threads
    for(int i = 0; i < num_threads/2; i++) {
        int tid0 = i;
        int tid1 = i + num_threads/2;

        if (thread_config == "numa") {
            numa_thread0[i] = new thread_numa<NODE_ZERO>(array_test, tid0, duration, DS_config, 0, num_threads, array_size, num_arrays/2, interval);
            numa_thread1[i] = new thread_numa<MAX_NODE>(array_test, tid1, duration, DS_config, 1, num_threads, array_size, num_arrays/2, interval);
        }
        else if (thread_config == "regular") {
            regular_thread0[i] = new thread(array_test, tid0, duration, DS_config, 0, num_threads, array_size, num_arrays/2, interval);
            regular_thread1[i] = new thread(array_test, tid1, duration, DS_config, 1, num_threads, array_size, num_arrays/2, interval);
        }
        else {
            numa_thread0[i] = new thread_numa<NODE_ZERO>(array_test, tid0, duration, DS_config, 1, num_threads, array_size, num_arrays/2, interval);
            numa_thread1[i] = new thread_numa<MAX_NODE>(array_test, tid1, duration, DS_config, 0, num_threads, array_size, num_arrays/2, interval);
        }
    }

    // 4. Join Test Threads
    if (thread_config == "regular") {
        for(auto& t : regular_thread0) if(t) { t->join(); delete t; }
        for(auto& t : regular_thread1) if(t) { t->join(); delete t; }
    } else {
        for(auto& t : numa_thread0) if(t) { t->join(); delete t; }
        for(auto& t : numa_thread1) if(t) { t->join(); delete t; }
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
			case 'i':  // -i interval option
				interval = std::stoll(optarg);
				break;	
            case '?':  // Unknown option
                std::cerr << "Unknown option or missing argument.\n";
                return ;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    compile_options(argc, argv);

    if (num_threads == 0) num_threads = 2; 

    if ((thread_config == "numa" || DS_config == "numa") && numa_num_configured_nodes() <= 1) {
        std::cout << "NUMA not available. Downgrading to regular.\n";
        thread_config = "regular";
        DS_config = "regular";
    }

    spawn_threads();
	print_header();
	
    // --- REPORTING PHASE ---
    size_t num_intervals = get_num_intervals();
		int64_t ops0 = get_ops(0, 0);
		int64_t ops1 = get_ops(1, 0);
		print_result(duration, ops0, ops1);


    return 0;
}
