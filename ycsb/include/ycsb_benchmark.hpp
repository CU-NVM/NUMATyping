#ifndef YCSB_BENCHMARK_HPP
#define YCSB_BENCHMARK_HPP

#include <string>
#include "zipfian_generator.h"
#include <stdexcept>
#include <jemalloc/jemalloc.h>
#include <umf/pools/pool_jemalloc.h>
using namespace ycsbc;
using namespace std;

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

struct WorkloadConfig {
    int read_pct;
    int update_pct;
    int insert_pct;
    int scan_pct;
    int rmw_pct;
};

WorkloadConfig selectWorkload(const string &w);

void global_init(int num_threads, int duration, int interval);

void numa_hash_table_init(int numa_node, std::string DS_config, int buckets, int num_tables);

void ycsb_test(
    int thread_id,
    int num_total_threads,
    int numa_node,
    int duration,
    const WorkloadConfig* cfg,
    ZipfianGenerator* gen,
    int num_keys,
    int local_pct,
    int interval,
    int num_tables
);

void run_ycsb_benchmark(
    const std::string& workload_key,
    int duration,
    int num_keys,
    double theta,
    int buckets,
    const std::string& locality_key,
    int num_threads,
    const std::string& th_config,
    const std::string& DS_config,
    int num_tables
);

void prefill_hash_tables(int num_keys_to_fill, int total_num_tables);

#endif