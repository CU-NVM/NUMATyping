#pragma once
/*! \file Array.hpp
 * \brief Array program which allows for testing of an array
 * \author Kidus
 * \date 2026
 *
 */

#ifndef _ARRAY_HPP_
#define _ARRAY_HPP__HPP_



#include <string>
#include <jemalloc/jemalloc.h>
#include <umf/pools/pool_jemalloc.h>

using namespace std;

void global_init(int num_threads, int duration, int interval);

void numa_array_init(int tid, int num_threads, std::string DS_config, int64_t array_size, int node, int num_arrays);
void numa_array_single_init(int num_threads, std::string DS_config, int64_t array_size, int num_arrays);
void array_test(int tid, int duration, std::string DS_config, int node, int num_threads,  int64_t array_size, int num_arrays, int interval);

void global_cleanup(int num_threads, int num_arrays, int duration, int interval);

#endif 