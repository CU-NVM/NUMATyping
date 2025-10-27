#include "ycsb_benchmark.hpp"
#include "zipfian_generator.h"
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <chrono>
#include <iomanip>
#include "numathreads.hpp"

using namespace std;
using namespace ycsbc;

#ifdef NUMA_MACHINE
	#define NODE_ZERO 0
	#define NODE_ONE 1
#else
	#define NODE_ZERO 0
	#define NODE_ONE 1
#endif

int num_threads = 2;
int bucket_count = 1024;
string workload_key = "A";
int num_keys = 10000;
double theta = 0.99;
string locality_key = "80-20";
string th_config = "regular";
string DS_config = "regular";
int duration = 20;
int interval = 10;
int num_tables = 10;

extern int64_t ops0;
extern int64_t ops1;
extern std::vector<int64_t> globalOps0;
extern std::vector<int64_t> globalOps1;
std::vector <int64_t> num_ops1;
std::vector <int64_t> num_ops0;
std::vector <int64_t> total_ops;

vector<thread_numa<NODE_ZERO>*> numa_thread0;
vector<thread_numa<NODE_ONE>*> numa_thread1;
vector<thread*> regular_thread0;
vector<thread*> regular_thread1;
thread_numa<NODE_ZERO>* init_thread0;
thread_numa<NODE_ONE>* init_thread1;
std::thread* init_thread_regular0;
std::thread* init_thread_regular1;

vector<ZipfianGenerator*> generators;

void print_function(int duration, int64_t ops0, int64_t ops1, int64_t totalOps) {
	auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::cout<<std::put_time(local_time, "%Y-%m-%d") << ", ";
	std::cout<<std::put_time(local_time, "%H:%M:%S") <<", ";
	std::cout<<num_tables << ", ";
	std::cout<<num_threads << ", ";
	std::cout<<th_config << ", ";
	std::cout<<DS_config << ", ";
    std::cout<<bucket_count<<", ";
    std::cout<<workload_key<<", ";
	std::cout<<duration << ", ";
	std::cout<<num_keys<<", ";
	std::cout<<interval<<", ";
	std::cout<<ops0 << ", ";
	std::cout<<ops1 << ", ";
	std::cout<<totalOps << "\n";
}

void print_header() {
    std::cout << "Date, Time, Num_Tables, Num_Threads, Thread_Config, DS_Config, Buckets, Workload, Duration(s), Num_Keys, Interval(s), Ops_Node0, Ops_Node1, Total_Ops\n";
}

void compile_options(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"threads",    required_argument, nullptr, 't'},
        {"buckets",    required_argument, nullptr, 'b'},
        {"workload",   required_argument, nullptr, 'w'},
        {"duration",   required_argument, nullptr, 'u'},
        {"keys",       required_argument, nullptr, 'k'},
        {"theta",      required_argument, nullptr, 'z'},
        {"locality",   required_argument, nullptr, 'l'},
        {"th_config",  required_argument, nullptr, 'c'},
        {"DS_config",  required_argument, nullptr, 'd'},
        {"interval",   required_argument, nullptr, 'i'},
        {"tables",     required_argument, nullptr, 'a'},
        {"help",       no_argument,       nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:b:w:u:k:z:l:c:d:i:a:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 't': num_threads = std::stoi(optarg); break;
            case 'b': bucket_count = std::stoi(optarg); break;
            case 'w': workload_key = optarg; break;
            case 'u': duration = std::stoi(optarg); break;
            case 'k': num_keys = std::stoi(optarg); break;
            case 'z': theta = std::stod(optarg); break;
            case 'l': locality_key = optarg; break;
            case 'c': th_config = optarg; break;
            case 'd': DS_config = optarg; break;
            case 'i': interval = std::stoi(optarg); break;
            case 'a': num_tables = std::stoi(optarg); break;
            case 'h':
                cout << "Usage: ./runner [options]\n";
                cout << "Options:\n";
                cout << "  -t, --threads <num>      Number of threads (default: 2)\n";
                cout << "  -b, --buckets <num>      Number of hash table buckets (default: 1024)\n";
                cout << "  -w, --workload <A-F>     YCSB workload type (default: A)\n";
                cout << "  -o, --ops <num>          Total operations (default: 1000000)\n";
                cout << "  -k, --keys <num>         Number of keys (default: 10000)\n";
                cout << "  -z, --theta <float>      Zipfian theta (default: 0.99)\n";
                cout << "  -l, --locality <split>   Locality (80-20, 50-50, 20-80) (default: 80-20)\n";
                cout << "  -c, --th_config <cfg>    Thread config (regular, numa) (default: regular)\n";
                cout << "  -d, --DS_config <cfg>    Data structure config (regular, numa) (default: regular)\n";
                exit(0);
            case '?':
                cerr << "Unknown option or missing argument.\n";
                exit(1);
            default: break;
        }
    }
}

WorkloadConfig selectWorkload(const string &w) {
    WorkloadConfig workloadA = {50, 50, 0, 0, 0};
    WorkloadConfig workloadB = {95, 5, 0, 0, 0};
    WorkloadConfig workloadC = {100, 0, 0, 0, 0};
    WorkloadConfig workloadD = {95, 0, 5, 0, 0};
    WorkloadConfig workloadE = {0, 0, 5, 95, 0};
    WorkloadConfig workloadF = {50, 0, 0, 0, 50};
    if (w == "A") return workloadA;
    if (w == "B") return workloadB;
    if (w == "C") return workloadC;
    if (w == "D") return workloadD;
    if (w == "E") return workloadE;
    if (w == "F") return workloadF;
    throw runtime_error("Unknown workload " + w);
}

int selectLocality(const string &l) {
    if (l == "80-20") return 80;
    if (l == "50-50") return 50;
    if (l == "20-80") return 20;
    throw runtime_error("Unknown locality split. Use 80-20, 50-50, or 20-80." + l);
}

void run_ycsb_benchmark(
    const string& workload_key,
    int duration,
    int num_keys,
    double theta,
    int buckets,
    const string& locality_key,
    int num_threads,
    const string& th_config,
    const string& DS_config,
    int num_tables
) 
{
    if (num_threads <= 0) {
        cerr << "Number of threads must be greater than 0.\n";
        return;
    }
    
    for (int i = 0; i < num_threads; ++i)
        generators.push_back(new ZipfianGenerator(0, num_keys - 1, theta));

    WorkloadConfig cfg = selectWorkload(workload_key);
    int local_pct = selectLocality(locality_key);
    global_init(num_threads, duration, interval);

//Initialization
	#ifdef PIN_INIT
    if (th_config == "numa")
    {
        init_thread0 = new thread_numa<NODE_ZERO>(numa_hash_table_init, NODE_ZERO , DS_config, buckets, num_tables/2);
        init_thread1 = new thread_numa<NODE_ONE>(numa_hash_table_init,  NODE_ONE , DS_config, buckets, num_tables/2);
    }
    else 
    {
        init_thread_regular0 = new thread(numa_hash_table_init, NODE_ZERO , DS_config, buckets, num_tables/2);
        init_thread_regular1 = new thread(numa_hash_table_init, NODE_ONE , DS_config, buckets, num_tables/2);
    }
	#endif

    if (th_config == "numa") {
        init_thread0->join();
        init_thread1->join();
    } else {
        init_thread_regular0->join();
        init_thread_regular1->join();
    }

    int threads_per_node = num_threads / 2;
    if (th_config == "numa") {
        numa_thread0.resize(threads_per_node);
        numa_thread1.resize(threads_per_node);
    } else {
        regular_thread0.resize(threads_per_node);
        regular_thread1.resize(threads_per_node);
    }

    auto start = chrono::high_resolution_clock::now();
    int tables_per_node = num_tables/2;

    for (int i = 0; i < threads_per_node; ++i) {
        int thread_id = i;
        int numa_node = 0;
        if (th_config == "numa") 
        {
            numa_thread0[i] = new thread_numa<NODE_ZERO>(
                ycsb_test,
                thread_id, threads_per_node, numa_node, duration, &cfg, 
                generators[thread_id], num_keys, local_pct, interval, tables_per_node
            );
        } else {
            regular_thread0[i] = new thread(
                ycsb_test,
                thread_id, threads_per_node, numa_node, duration, &cfg, 
                generators[thread_id], num_keys, local_pct, interval, tables_per_node
            );
        }
    }

    for (int i = 0; i < threads_per_node; ++i) {
        int thread_id = i + threads_per_node;
        int numa_node = 1;
        if (th_config == "numa") {
            numa_thread1[i] = new thread_numa<NODE_ONE>(
                ycsb_test,
                thread_id, threads_per_node, numa_node, duration, &cfg, 
                generators[thread_id], num_keys, local_pct, interval, tables_per_node
            );
        } else {
            regular_thread1[i] = new thread(
                ycsb_test,
                thread_id, threads_per_node, numa_node, duration, &cfg, 
                generators[thread_id], num_keys, local_pct, interval, tables_per_node
            );
        }
    }

    if (th_config == "numa") {
        for (auto th : numa_thread0) th->join();
        for (auto th : numa_thread1) th->join();
    } else {
        for (auto th : regular_thread0) th->join();
        for (auto th : regular_thread1) th->join();
    }

    if (th_config == "numa") {
        for (auto th : numa_thread0) delete th;
        for (auto th : numa_thread1) delete th;
    } else {
        for (auto th : regular_thread0) delete th;
        for (auto th : regular_thread1) delete th;
    }

    num_ops0.push_back(ops0);
	num_ops1.push_back(ops1);
	total_ops.push_back(ops0 + ops1);
}

int main(int argc, char** argv) {

    compile_options(argc, argv);

    if (th_config == "numa" || DS_config == "numa") {
        if (numa_num_configured_nodes() == 1) {
            std::cout << "NUMA not available or only one node configured. Running in regular mode.\n";
            th_config = "regular";
            DS_config = "regular";
        }
    }

    print_header();
    print_function(0, 0, 0, 0); // Print header
    run_ycsb_benchmark(
        workload_key,
        duration,
        num_keys,
        theta,
        bucket_count,
        locality_key,
        num_threads,
        th_config,
        DS_config,
        num_tables
    );

    /*
    cout << "YCSB Benchmark Report:\n";
    cout << "----------------------\n";
    cout << "Workload: " << workload_key << ", Threads: " << num_threads 
        << ", Locality: " << locality_key << endl;
    cout << "Thread Config: " << th_config << ", DS Config: " << DS_config << endl;
    */

    for (auto gen : generators) {
        delete gen;
    }
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