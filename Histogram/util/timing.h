
#ifndef TIMING_H
#define TIMING_H


#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <syncstream>
#include <unordered_map>
#include <chrono>



// ---- Latency instrumentation (per-thread, per-phase) ----
namespace __HistLatency {
    inline std::mutex lat_mu;
    inline std::unordered_map<int, std::unordered_map<std::string, long long>> per_thread_ns;
    inline std::unordered_map<std::string, long long > total_ns;

    inline void record_total(std::string phase, long long ns) {
       total_ns[phase] = ns; 
    }

    inline std::unordered_map<std::string, long long> get_total() {
        return total_ns;
    }

    inline void record(int tid, const char* phase, long long ns) {
        //std::lock_guard<std::mutex> lk(lat_mu);
        per_thread_ns[tid][phase] = ns;
        per_thread_ns[tid]["TotalLat"] += ns;
    }

    inline std::unordered_map<int, std::unordered_map<std::string, long long>> get_copy() {
        return per_thread_ns;
    }

    inline void reset() {
        per_thread_ns.clear();
        total_ns.clear();
    }
}

// Expose simple C++ linkage accessors for main.cpp
inline std::unordered_map<int, std::unordered_map<std::string, long long>> get_latency_map_copy() {
    return __HistLatency::get_copy();
}

inline std::unordered_map<std::string, long long> get_total_latency_map() {
    return __HistLatency::get_total();
}

inline void time(int tid, std::string phase, std::chrono::high_resolution_clock::time_point start_time) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    __HistLatency::record(tid, phase.c_str(), ns);
}

inline void stopwatch(std::string phase, std::chrono::high_resolution_clock::time_point start_time) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    __HistLatency::record_total(phase, ns);
}
// void reset_latency_map() { __HistLatency::reset(); }
#endif // TIMING_H