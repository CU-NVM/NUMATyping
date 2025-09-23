#pragma once
/*! \file TestSuite.hpp
 * \brief Testsuite program which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _HISTOGRAM_HPP_
#define _HISTOGRAM_HPP__HPP_



#include <string>
#include "numatype.hpp"
#include "numathreads.hpp"
#include <jemalloc/jemalloc.h>
#include <umf/pools/pool_jemalloc.h>
using namespace std;

void global_init(int num_threads, int duration, int interval);

void numa_histogram_init(int num_threads, std::string DS_config, int bucket_count, int node);
void numa_histogram_single_init(int num_threads, std::string DS_config, int bucket_count);
void histogram_test(int tid, int duration, int node, int num_threads, std::string filename);



#endif 