#pragma once
#ifndef NUMATHREADS_HPP
#define NUMATHREADS_HPP

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

#include <numa.h>
#include <numaif.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

// Optional: cap if you really want a fixed upper bound; we now compute ncpus at runtime.
#ifndef MAX_CPUS
#define MAX_CPUS 80
#endif

// #define NUMA_THREADS_DEBUG 1

template <int NodeID>
class thread_numa : public std::thread {
public:
    using thread_type = std::thread;

    template <typename Func, typename... Args>
    thread_numa(Func&& func, Args&&... args)
        : std::thread(std::forward<Func>(func), std::forward<Args>(args)...) {
        // Pin the new thread to the specified NUMA node
        pin_thread_to_node(this, NodeID);
    }

    ~thread_numa() = default;

    thread_numa(const thread_numa&)            = delete;
    thread_numa& operator=(const thread_numa&) = delete;

    thread_numa(thread_numa&& other) noexcept : std::thread(std::move(other)) {}
    thread_numa& operator=(thread_numa&& other) noexcept {
        if (this != &other) std::thread::operator=(std::move(other));
        return *this;
    }

private:
    // Store BOTH the cpu_set_t* and its size (bytes) so pthread_setaffinity_np gets the exact size.
    static inline std::map<int, std::pair<cpu_set_t*, size_t>> node_to_cpumask;

    static inline decltype(node_to_cpumask)::iterator convert_bitmask_to_cpuset(int node) {
        if (numa_available() == -1) {
            throw std::runtime_error("NUMA is not available on this system.");
        }

        // Get the NUMA node's CPU bitmask
        bitmask* bm = numa_allocate_cpumask();
        if (!bm) throw std::runtime_error("numa_allocate_cpumask failed");
        if (numa_node_to_cpus(node, bm) != 0) {
            numa_free_cpumask(bm);
            throw std::system_error(errno, std::generic_category(), "numa_node_to_cpus");
        }

        // Use actual CPU count; fall back to MAX_CPUS if sysconf fails.
        long nconf = sysconf(_SC_NPROCESSORS_CONF);
        int  ncpus = (nconf > 0) ? static_cast<int>(nconf) : MAX_CPUS;

        size_t sz = CPU_ALLOC_SIZE(ncpus);
        cpu_set_t* set = CPU_ALLOC(ncpus);
        if (!set) {
            numa_free_cpumask(bm);
            throw std::runtime_error("CPU_ALLOC failed");
        }
        CPU_ZERO_S(sz, set);

        // Copy bits from libnuma bitmask cpu_set_t
        for (int cpu = 0; cpu < static_cast<int>(bm->size); ++cpu) {
            if (numa_bitmask_isbitset(bm, cpu) && cpu < ncpus) {
                CPU_SET_S(cpu, sz, set);
            }
        }
        numa_free_cpumask(bm);

        auto [it, inserted] = node_to_cpumask.insert({node, {set, sz}});
        if (!inserted) {
            // Already had one; free the new allocation and return existing
            CPU_FREE(set);
        }
        return node_to_cpumask.find(node);
    }

    static inline void pin_thread_to_node(std::thread* t, int node) {
        if (numa_available() == -1) {
            std::cerr << "NUMA is not available on this system.\n";
            return;
        }

        // Build or get cached mask for this node
        auto it = node_to_cpumask.find(node);
        if (it == node_to_cpumask.end()) {
            it = convert_bitmask_to_cpuset(node);
        }
        cpu_set_t* mask = it->second.first;
        size_t     sz   = it->second.second;

        // Intersect with allowed CPUs to avoid EPERM under cgroups/cpuset
        cpu_set_t* allowed = static_cast<cpu_set_t*>(malloc(sz));
        if (allowed && sched_getaffinity(0, sz, allowed) == 0) {
            for (int cpu = 0; cpu < static_cast<int>(8 * sz); ++cpu) {
                if (!CPU_ISSET_S(cpu, sz, allowed)) {
                    CPU_CLR_S(cpu, sz, mask);
                }
            }
        }
        free(allowed);

        // Ensure the mask isn't empty
        bool any = false;
        for (int cpu = 0; cpu < static_cast<int>(8 * sz); ++cpu) {
            if (CPU_ISSET_S(cpu, sz, mask)) { any = true; break; }
        }
        if (!any) {
            throw std::runtime_error("Affinity mask is empty for node " + std::to_string(node));
        }

        // debug_print_mask("binding mask", mask, sz);

        pthread_t pid = t->native_handle();
\
        int rc = pthread_setaffinity_np(pid, sz, mask);
        if (rc != 0) {
            throw std::system_error(rc, std::generic_category(),
                                    "pthread_setaffinity_np (node " + std::to_string(node) + ")");
        }

    }
};

#endif // NUMATHREADS_HPP