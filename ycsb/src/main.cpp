#include "../include/ycsb_benchmark.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "../../numaLib/numathreads.hpp"
#include <numa.h> 

using namespace std;

int num_threads = 2;
int bucket_count = 1024;
string workload_key = "A";
int total_ops = 1000000;
int num_keys = 10000;
double theta = 0.99;
string locality_key = "80-20";
string th_config = "regular";
string DS_config = "regular";


void compile_options(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"threads",    required_argument, nullptr, 't'},
        {"buckets",    required_argument, nullptr, 'b'},
        {"workload",   required_argument, nullptr, 'w'},
        {"ops",        required_argument, nullptr, 'o'},
        {"keys",       required_argument, nullptr, 'k'},
        {"theta",      required_argument, nullptr, 'z'},
        {"locality",   required_argument, nullptr, 'l'},
        {"th_config",  required_argument, nullptr, 'c'},
        {"DS_config",  required_argument, nullptr, 'd'},
        {"help",       no_argument,       nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "t:b:w:o:k:z:l:c:d:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 't': num_threads = std::stoi(optarg); break;
            case 'b': bucket_count = std::stoi(optarg); break;
            case 'w': workload_key = optarg; break;
            case 'o': total_ops = std::stoi(optarg); break;
            case 'k': num_keys = std::stoi(optarg); break;
            case 'z': theta = std::stod(optarg); break;
            case 'l': locality_key = optarg; break;
            case 'c': th_config = optarg; break;
            case 'd': DS_config = optarg; break;
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

int main(int argc, char** argv) {

    compile_options(argc, argv);

    if (th_config == "numa" || DS_config == "numa") {
        if (numa_num_configured_nodes() == 1) {
            std::cout << "NUMA not available or only one node configured. Running in regular mode.\n";
            th_config = "regular";
            DS_config = "regular";
        }
    }

    run_ycsb_benchmark(
        workload_key,
        total_ops,
        num_keys,
        theta,
        bucket_count,
        locality_key,
        num_threads,
        th_config,
        DS_config
    );

    return 0;
}