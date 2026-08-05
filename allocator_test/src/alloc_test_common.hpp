// Shared helpers for the allocator test harnesses. Header-only.
//
// The point of these helpers is portability: the benchmarks pin worker threads
// to the CPUs of the physical node that logical node 0 maps to, discovered at
// runtime from libnuma, so they run correctly on any machine rather than
// assuming this box's CPU numbering.
#pragma once

#include <numa.h>
#include <numaif.h>
#include <pthread.h>
#include <sched.h>
#include <vector>

// CPUs belonging to the physical node that logical node `logical` maps to.
// (numa_node_map() comes from numaLib/numa_nodemap.hpp, pulled in by
// umf_numa_allocator.hpp.)
inline const std::vector<int> &node_cpus(unsigned logical) {
    static std::vector<std::vector<int>> cache;
    if (logical >= cache.size()) {
        cache.resize(logical + 1);
    }
    std::vector<int> &cpus = cache[logical];
    if (!cpus.empty()) {
        return cpus;
    }
    unsigned phys = numa_node_map(logical);
    bitmask *bm = numa_allocate_cpumask();
    if (numa_node_to_cpus(phys, bm) == 0) {
        for (int c = 0; c < numa_num_configured_cpus(); ++c) {
            if (numa_bitmask_isbitset(bm, c)) {
                cpus.push_back(c);
            }
        }
    }
    numa_free_cpumask(bm);
    if (cpus.empty()) {
        cpus.push_back(0);
    }
    return cpus;
}

// Pin the calling thread to the i-th CPU of logical node `logical` (round-robin).
inline void pin_to_node(unsigned logical, int i) {
    const std::vector<int> &cpus = node_cpus(logical);
    cpu_set_t s;
    CPU_ZERO(&s);
    CPU_SET(cpus[i % cpus.size()], &s);
    pthread_setaffinity_np(pthread_self(), sizeof(s), &s);
}

// Physical NUMA node backing a virtual address (page must be faulted in first).
inline int node_of(void *p) {
    int status = -1;
    if (move_pages(0, 1, &p, NULL, &status, 0)) {
        return -2;
    }
    return status;
}
