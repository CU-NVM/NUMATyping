/*! \file hist_main.cpp
 *  \brief Main file for histogram test
 *  \author Kidus Workneh
 *  \date 2025 
*/

#include "Histogram.hpp"
#include "timing.h"
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
int bucket_count = 10;
string book_title = "";

std::string merge_strat = "ALL";


std::vector <thread_numa<NODE_ZERO>*> numa_thread0;
std::vector <thread_numa<NODE_ONE>*> numa_thread1;
std::vector <thread*> regular_thread0;
std::vector <thread*> regular_thread1;
thread_numa<NODE_ZERO>* init_thread0;
thread_numa<NODE_ONE>* init_thread1;
std::thread* init_thread_regular0;
std::thread* init_thread_regular1;




void compute_histogram(){
	numa_thread0.assign(num_threads/2, nullptr);
	numa_thread1.assign(num_threads/2, nullptr);
	regular_thread0.assign(num_threads/2, nullptr);
	regular_thread1.assign(num_threads/2, nullptr);

	//Initialization
	#ifdef PIN_INIT
		if (numa_num_configured_nodes() == 1 || (DS_config == "regular" && thread_config == "regular")) {
			init_thread_regular0 = new thread(numa_histogram_init, num_threads/2, "regular", bucket_count, 0);
			init_thread_regular1 = new thread(numa_histogram_init, num_threads/2, "regular", bucket_count, 1);
		}
		else{
			init_thread0 = new thread_numa<NODE_ZERO>(numa_histogram_init, num_threads/2, DS_config,  bucket_count, 0);
			init_thread1 = new thread_numa<NODE_ONE>(numa_histogram_init, num_threads/2, DS_config,  bucket_count, 1);
		}
	#else
		std::cout<< "single threaded initialization running" <<std::endl;
		numa_histogram_single_init(num_threads, DS_config, bucket_count);
	#endif

	//Test
	for(int i=0; i < num_threads/2; i++){
		int node = 0;
		int tid = i;
        string filename = "../book/per_thread/" + book_title + "_" + to_string(tid) + ".txt"; 
		if(thread_config == "numa"){
			numa_thread0[i] = new thread_numa<NODE_ZERO>(histogram_test, tid, duration, DS_config, node, num_threads/2, filename, merge_strat, bucket_count);
		}
		else if(thread_config == "regular"){
			regular_thread0[i] = new thread(histogram_test, tid, duration, DS_config, node, num_threads/2, filename, merge_strat, bucket_count);
		}
		else{
			numa_thread0[i] = new thread_numa<NODE_ZERO>(histogram_test, tid, duration, DS_config, 1, num_threads/2, filename, merge_strat, bucket_count);
		}
	}

	for(int i=0; i < num_threads/2; i++){
		int node = 1;
		int tid = i + num_threads/2;
		string filename = "../book/per_thread/" + book_title + "_" + to_string(tid) + ".txt"; 
		if(thread_config == "numa"){
			numa_thread1[i] = new thread_numa<NODE_ONE>(histogram_test, tid, duration, DS_config, node, num_threads/2, filename, merge_strat, bucket_count);
		}
		else if(thread_config == "regular"){
			regular_thread1[i] = new thread(histogram_test, tid, duration, DS_config, node, num_threads/2, filename, merge_strat, bucket_count);
		}
		else{
			numa_thread1[i] = new thread_numa<NODE_ONE>(histogram_test, tid, duration, DS_config, 0, num_threads/2, filename, merge_strat, bucket_count);
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
		{"bucket_count", required_argument, nullptr, 'b'},      // -k
		{"interval", required_argument, nullptr, 'i'},      // -i
		{"book_title", required_argument, nullptr, 'a'},      // -a
		{"merge_strategy", required_argument, nullptr, 'm'}, // -m
		{"verbose", no_argument, nullptr, 'v'},        // --verbose
		{"help", no_argument, nullptr, 'h'},           // --help
		{nullptr, 0, nullptr, 0}                            // End of array
	};

 int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:b:f:", long_options, &option_index)) != -1) {
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
			case 'm': // -m option for merge strategy
				if(optarg){
					merge_strat = optarg;
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


bool process_file(string book_title, int num_threads){
	std::string home = std::getenv("HOME"); 
	string filename = home + "/NUMATyping/book/" + book_title + ".txt";
	if (num_threads <= 0) { std::cerr << "num_threads must be > 0\n"; return false; }

	std::ifstream in(filename);
	if (!in) { std::cerr << "Failed to open: " << filename << "\n"; return false; }

	// Pass 1: count lines
	std::size_t total_lines = 0;
	std::string line;
	while (std::getline(in, line)) ++total_lines;

	// Rewind to start
	in.clear();
	in.seekg(0, std::ios::beg);

	// Chunk sizes (first 'rem' chunks get one extra line)
	std::vector<std::size_t> chunk_sizes(num_threads, total_lines / num_threads);
	std::size_t rem = total_lines % static_cast<std::size_t>(num_threads);
	for (int i = 0; i < num_threads && rem > 0; ++i, --rem) ++chunk_sizes[i];

	namespace fs = std::filesystem;
	fs::path inpath(filename);
	std::string stem = inpath.stem().string();   // "filename"
	const std::string ext = ".txt";              // force .txt

	// Ensure $HOME/NUMATyping/book exists, then delete all *.txt inside it
	fs::path out_dir = fs::path(home + "/NUMATyping/book") / "per_thread";
	try {
		fs::create_directories(out_dir); // no-op if exists
		for (const auto& entry : fs::directory_iterator(out_dir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".txt") {
				std::error_code del_ec;
				fs::remove(entry.path(), del_ec);
				if (del_ec) {
					std::cerr << "Warning: couldn't remove " << entry.path()
							<< " : " << del_ec.message() << "\n";
				}
			}
		}
	} catch (const fs::filesystem_error& e) {
		std::cerr << "Error preparing $HOME/NUMATyping/book: " << e.what() << "\n";
		return false;
	}

	// Pass 2: write chunks
	for (int i = 0; i < num_threads; ++i) {
		fs::path outpath = out_dir / (stem + "_" + std::to_string(i) + ext);
		std::ofstream out(outpath.string());
		if (!out) { std::cerr << "Failed to create: " << outpath << "\n"; return false; }

		for (std::size_t written = 0; written < chunk_sizes[i] && std::getline(in, line); ++written) {
			out << line << '\n';
		}
	}

	return true;
}

void clean_files(){
	std::string home = std::getenv("HOME"); 
    const std::filesystem::path dir = home + "/NUMATyping/book/per_thread/";

    try {
        if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
            std::filesystem::remove_all(dir); // removes everything in it (including the directory)
            std::filesystem::create_directory(dir); // recreate the empty directory if you want to keep it
            // std::cout << "Directory contents deleted successfully.\n";
        } else {
            	std::cerr << "Directory does not exist or is not valid.\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

}

void print_stat(bool verbose, bool print_header){
	auto latmap = get_latency_map_copy();
	auto totalmap = get_total_latency_map();

	#ifdef PER_THREAD_TIMING
		if(verbose){ 
			//orderd print per thread
			for (int i= 0; i < latmap.size(); i++) {
				const auto& m = latmap[i];
				std::cout << "Thread " << i << " Latencies (ns):\n";
				for (const auto& phase : m) {
					std::cout << "  " << phase.first << ": " << phase.second << "\n";
				}
			}
		}
		long long p1=0, p2=0, p31=0, p32=0, p33=0, total=0;
		//sum up all threads latencies per phase
		for (const auto& tid : latmap) {
			const auto& m = tid.second;
			for (const auto& phase : m) {
					if (phase.first == "Phase1Lat") p1 += phase.second;
					else if (phase.first == "Phase2Lat") p2 += phase.second;
					else if (phase.first == "Phase3.1Lat") p31 += phase.second;
					else if (phase.first == "Phase3.2Lat") p32 += phase.second;
					else if (phase.first == "Phase3.3Lat") p33 += phase.second;
					else if (phase.first == "TotalLat") total += phase.second;
				}
			}
		
	#endif

	// Date and Time (local)
	auto now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm = *std::localtime(&tt);
	std::ostringstream date_ss, time_ss;
	date_ss << std::put_time(&local_tm, "%Y-%m-%d");
	time_ss << std::put_time(&local_tm, "%H:%M:%S");

	// Convert ns to milliseconds as floating point
	auto ns_to_ms = [](long long ns)->double { return ns / 1e6; };

	//Header (optional): print once
	if (print_header) {
		std::cout << "Date, Time, num_threads, thread_config, DS_config, bucket_count, run_freq, "
					<< "FileRead, PerNodeMerge, SingleThreadMerge, SingleNodeMerge, MultiNodeMerge, TotalLat\n";
	}


	std::cout << date_ss.str() << ", "
				<< time_ss.str() << ", "
				<< num_threads << ", "
				<< thread_config << ", "
				<< DS_config << ", "
				<< bucket_count << ", "
				<< run_freq << ", "
				<< std::fixed << std::setprecision(3)
				<< ns_to_ms(totalmap["Phase1Total"]) << ", "
				<< ns_to_ms(totalmap["Phase2Total"]) << ", "
				<< ns_to_ms(totalmap["Phase3.1Total"]) << ", "
				<< ns_to_ms(totalmap["Phase3.2Total"]) << ", "
				<< ns_to_ms(totalmap["Phase3.3Total"]) << ", "
				<< ns_to_ms(totalmap["Phase1Total"] + totalmap["Phase2Total"] + totalmap["Phase3.1Total"] + totalmap["Phase3.2Total"] + totalmap["Phase3.3Total"])
				<< "\n";	
//todo: Implement per thread timing printing if it is ever necessary
}


int main(int argc, char *argv[])
{
    compile_options(argc, argv);

	global_init(num_threads, duration, interval);

	// Check if NUMA is available and the node is allowed	
	if (thread_config == "numa" || DS_config == "numa") {
		if (numa_num_configured_nodes() == 1) {
			std::cout << "NUMA not available.\n";
		}
	}
	bool stat = process_file(book_title, num_threads);
	if(!stat){
		return -1;
	}
	compute_histogram();
	bool print_header = false;
	print_stat(verbose, print_header);
	//std::cout << "Histogram computation completed." << std::endl;
	numa_thread0.clear();
	numa_thread1.clear();
	regular_thread0.clear();
	regular_thread1.clear();	
	global_cleanup(num_threads, duration, interval);
	clean_files();
	return 0;
}