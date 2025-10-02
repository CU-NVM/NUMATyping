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

using namespace std;

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 0
#endif

string thread_config;
string DS_config;
int num_threads = 0;
int duration = 0;
int run_freq = 1;
int interval =20;
bool verbose = false;
int bucket_count = 10;
string book_title = "";


std::vector <thread_numa<NODE_ZERO>*> numa_thread0;
std::vector <thread_numa<NODE_ONE>*> numa_thread1;
std::vector <thread*> regular_thread0;
std::vector <thread*> regular_thread1;
thread_numa<NODE_ZERO>* init_thread0;
thread_numa<NODE_ONE>* init_thread1;
std::thread* init_thread_regular0;
std::thread* init_thread_regular1;

void compile_options(int argc, char *argv[]){
static struct option long_options[] = {
		{"th_config", required_argument, nullptr, 'c'},     // --th_config=NUMA/REGULAR
		{"DS_config", required_argument, nullptr, 'd'},     // --DS_config=NUMA/REGULAR
		{"num_threads", required_argument, nullptr, 't'},   // -t
		{"duration", required_argument, nullptr, 'D'},      // -d
		{"run_freq", optional_argument, nullptr, 'f'},       // -p
		{"bucket_count", required_argument, nullptr, 'b'},      // -k
		{"interval", required_argument, nullptr, 'i'},      // -i
		{"book_title", required_argument, nullptr, 'a'},      // -a
		{"verbose", no_argument, nullptr, 'v'},        // --verbose
		{"help", no_argument, nullptr, 'h'},           // --help
		{nullptr, 0, nullptr, 0}                            // End of array
	};

 int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:D:b:f:i:", long_options, &option_index)) != -1) {
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
            case 'D':  // -D option for duration
                duration = std::stoi(optarg);
                break;
			case 'f':
				if(optarg){
					run_freq = std::stoi(optarg);
				}
				break;
			case 'i':
				if(optarg){
					interval = std::stoi(optarg);
				}
				break;
			case 'b':
				if(optarg){
					bucket_count = std::stoi(optarg);
				}
				break;
			case 'a':
				if(optarg){
					book_title = optarg;
				}
				break;
			case 'v':  // --verbose flag
				verbose = true;
				break;				
            case '?':  // Unknown option
                std::cerr << "Unknown option or missing argument.\n";
                return ;
            default:
                break;
        }
    }
}