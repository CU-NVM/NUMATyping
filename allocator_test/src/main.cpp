#include<iostream>
#include "umf_numa_allocator.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <cstdlib>


size_t NUM_ALLOCS = 100000; // 100k objects
size_t ALLOC_SIZE = 64;     // 64 bytes each


size_t get_current_rss() {
    long rss = 0L;
    FILE* fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL)
        return 0;
    if (fscanf(fp, "%*s%ld", &rss) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
}

void test_numa() {
    // Check if NUMA is available on this system
    if (numa_available() == -1) {
        std::cerr << "NUMA not supported on this system." << std::endl;
        return;
    }

    // We'll allocate on Node 0 for this test
    int target_node = 0;
    std::cout << "Testing NUMA (numa_alloc_onnode) on Node " << target_node << std::endl;

    void **ptr_array = new void*[NUM_ALLOCS];
    auto start_time = std::chrono::high_resolution_clock::now();
    size_t start_mem = get_current_rss();

    for (size_t i = 0; i < NUM_ALLOCS; ++i) {
        // numa_alloc_onnode is similar to malloc but pins to a specific NUMA node
        void* ptr = numa_alloc_onnode(ALLOC_SIZE, target_node);
        if (ptr == nullptr) {
            std::cerr << "Allocation failed at index " << i << std::endl;
            break;
        }
        ((char*)ptr)[0] = 1; // Touch memory to ensure physical page backing
        ptr_array[i] = ptr;
    }

    size_t end_mem = get_current_rss();
    auto end_time = std::chrono::high_resolution_clock::now();
    
    double time_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    size_t mem_diff = (end_mem > start_mem) ? (end_mem - start_mem) : 0;
    size_t requested_total = NUM_ALLOCS * ALLOC_SIZE;

    std::cout << "Time Elapsed:       " << time_ms << " ms" << std::endl;
    std::cout << "RSS Growth:         " << mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << ((double)mem_diff - (double)requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;

    // Clean up
    for (size_t i = 0; i < NUM_ALLOCS; ++i) {
        // Use numa_free for memory allocated with numa_alloc functions
        numa_free(ptr_array[i], ALLOC_SIZE);
    }
    delete[] ptr_array;
}

void test_malloc(){
    std::cout<<"Testing Malloc allocator"<<std::endl;
    void **ptr_array = new void*[NUM_ALLOCS];
    auto start_time = std::chrono::high_resolution_clock::now();
	size_t start_mem = get_current_rss();
    for (size_t i = 0; i < NUM_ALLOCS; ++i) {
        void* ptr = malloc(ALLOC_SIZE);
        ((char*)ptr)[0]= 1;
        ptr_array[i] = ptr;
    }
	
	size_t end_mem = get_current_rss();
    auto end_time = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    size_t mem_diff = (end_mem > start_mem) ? (end_mem - start_mem) : 0;
    size_t requested_total = NUM_ALLOCS * ALLOC_SIZE;
    std::cout << "Time Elapsed:       " << time_ms << " ms" << std::endl;
	std::cout << "RSS Growth:         " << mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << ((double)mem_diff - (double)requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;

	
    // Clean up
    for(size_t i=0; i<NUM_ALLOCS; i++){
        free(ptr_array[i]);
    }
    delete[] ptr_array;
}


void test_umf(){
    std::cout<<"Testing UMF allocator" <<std::endl;
    umf_alloc_init();
    void **ptr_array = new void*[NUM_ALLOCS];
    auto start_time = std::chrono::high_resolution_clock::now();
	size_t start_mem = get_current_rss();

    for(size_t i=0; i < NUM_ALLOCS; ++i) {
        void * ptr = umf_alloc(0, ALLOC_SIZE, 8);
        ((char*)ptr)[0]=1 ;
        ptr_array[i]= ptr;
    }
	
	size_t end_mem = get_current_rss(); 
    auto end_time = std::chrono::high_resolution_clock::now();
    size_t mem_diff = (end_mem > start_mem) ? (end_mem - start_mem) : 0;                                                                                 
    size_t requested_total = NUM_ALLOCS * ALLOC_SIZE;
    double time_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    std::cout << "Time Elapsed:       " << time_ms << " ms" << std::endl;


	std::cout << "RSS Growth:         " << mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << ((double)mem_diff - (double)requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;
    // Clean up
    for(size_t i=0; i<NUM_ALLOCS; ++i) {
        umf_free(0,ptr_array[i]);
    }
    delete[] ptr_array;
}




void test_jemalloc(){
    std::cout<<"Testing jemalloc allocator (mallocx)"<<std::endl;
    void **ptr_array = new void*[NUM_ALLOCS];
    auto start_time = std::chrono::high_resolution_clock::now();
    size_t start_mem = get_current_rss();
    for(size_t i=0; i < NUM_ALLOCS; ++i) {
        void* ptr = mallocx(ALLOC_SIZE, 0);   // jemalloc's explicit API (unambiguously jemalloc)
        ((char*)ptr)[0] = 1;
        ptr_array[i] = ptr;
    }
    size_t end_mem = get_current_rss();
    auto end_time = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    size_t mem_diff = (end_mem > start_mem) ? (end_mem - start_mem) : 0;
    size_t requested_total = NUM_ALLOCS * ALLOC_SIZE;
    std::cout << "Time Elapsed:       " << time_ms << " ms" << std::endl;
    std::cout << "RSS Growth:         " << mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << ((double)mem_diff - (double)requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;

    // Clean up
    for(size_t i=0; i<NUM_ALLOCS; ++i) {
        dallocx(ptr_array[i], 0);
    }
    delete[] ptr_array;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <malloc|jemalloc|umf|numa> [count] [size]" << std::endl;
        return 1;
    }
    
    std::string mode = argv[1];
    if (argc >= 3) NUM_ALLOCS = std::atol(argv[2]);
    if (argc >= 4) ALLOC_SIZE = std::atol(argv[3]);

    if (mode == "malloc") {
        test_malloc();
    } else if (mode == "jemalloc") {
        test_jemalloc();
    } else if (mode == "umf") {
        test_umf();
    } else if (mode == "numa") {
        test_numa();
    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
    }

    return 0;
}
