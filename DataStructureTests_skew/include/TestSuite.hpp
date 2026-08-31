#pragma once
/*! \file TestSuite.hpp
 * \brief Testsuite program which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _TESTSUITE_HPP_
#define _TESTSUITE_HPP_


#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <string>
#include <vector>
#include <jemalloc/jemalloc.h>
#include <umf/pools/pool_jemalloc.h>
using namespace std;

void global_init(int num_threads, int duration, int interval);

void numa_BST_init(std::string DS_config, int num_DS, int keyspace, int node);



void numa_BST_single_init(std::string DS_config, int num_DS, int keyspace, int node);

void main_BST_test(int duration,  int64_t num_DS, int num_threads, int keyspace);

void BinarySearchTest(int t_id, int duration, int node, int64_t num_DS, int num_threads, int keyspace, int interval);

// Transaction mix: weights for 0->0, 0->1, 1->0, 1->1.  Defaults to
// 25,25,25,25, which is the fixed split DataStructureTests hard-coded.
void set_txn_mix(int w00, int w01, int w10, int w11);
int  txn_mix_total();
extern int txnWeights[4];


void global_cleanup();

#endif 