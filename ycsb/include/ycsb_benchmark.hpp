#ifndef YCSB_BENCHMARK_HPP
#define YCSB_BENCHMARK_HPP

#include <string>

/**
 *
 * @param workload_key (A, B, C, D, E)
 * @param total_ops
 * @param num_keys
 * @param theta
 * @param buckets
 * @param locality_key (80-20, 50-50, 20-80)
 * @param num_threads 
 * @param th_config (regular, numa)
 * @param DS_config (regular, numa)
 */

void run_ycsb_benchmark(
    const std::string& workload_key,
    int total_ops,
    int num_keys,
    double theta,
    int buckets,
    const std::string& locality_key,
    int num_threads,
    const std::string& th_config,
    const std::string& DS_config
);

#endif