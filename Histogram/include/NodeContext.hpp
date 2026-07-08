#pragma once

#include <cstdint>
#include <iostream>
#include <syncstream>

namespace numactx {
    extern thread_local int thread_node;

    inline void  set_thread_node(int node) {
        thread_node = node;
        //std::osyncstream(std::cout) << "Thread set to NUMA node " << thread_node << "\n";
    }
    inline int get_thread_node() {
        return thread_node;
    }
}