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
	std::cout << "RSS Growth:         " << mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << (double)(mem_diff - requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;

	std::cout << "Time Elapsed:       " << time_ms << " ms" << std::endl;
    
    //Clean up
    auto free_start_time = std::chrono::high_resolution_clock::now();
    size_t free_start_mem = get_current_rss();
    for(int i=0; i<NUM_ALLOCS; i++){
        free(ptr_array[i]);
    }
    delete[] ptr_array;
    size_t free_end_mem = get_current_rss();
    auto free_end_time = std::chrono::high_resolution_clock::now();
    double free_time_ms = std::chrono::duration<double, std::milli>(free_end_time - free_start_time).count();
    size_t free_mem_diff = (free_start_mem > free_end_mem) ? (free_start_mem - free_end_mem) : 0;

    std::cout << "RSS Decreament:         " <<free_mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << (double)(free_mem_diff - requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;

	std::cout << "Time Elapsed:       " << free_time_ms << " ms" << std::endl;
 

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
    std::cout << "Est. Overhead:      " << (double)(mem_diff - requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;
    //Clean up
   

    auto free_start_time = std::chrono::high_resolution_clock::now();
    size_t free_start_mem = get_current_rss();

    for(int i=0; i<NUM_ALLOCS; ++i) {
        umf_free(0,ptr_array[i]);
    }
    delete[] ptr_array;
    size_t free_end_mem = get_current_rss();
    auto free_end_time = std::chrono::high_resolution_clock::now();
    double free_time_ms = std::chrono::duration<double, std::milli>(free_end_time - free_start_time).count();
    size_t free_mem_diff = (free_start_mem > free_end_mem) ? (free_start_mem - free_end_mem) : 0;

    std::cout << "RSS Decreament:         " <<free_mem_diff << " bytes" << std::endl;
    std::cout << "Requested Memory:   " << requested_total << " bytes" << std::endl;
    std::cout << "Est. Overhead:      " << (double)(free_mem_diff - requested_total) / NUM_ALLOCS << " bytes/object" << std::endl;
	std::cout << "Time Elapsed:       " << free_time_ms << " ms" << std::endl;
}






int main(int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <malloc|umf> [count] [size]" << std::endl;
        return 1;
    }
    std::string mode = argv[1];

    if (argc >= 3) NUM_ALLOCS = std::atol(argv[2]);
    if (argc >= 4) ALLOC_SIZE = std::atol(argv[3]);


    if(mode=="malloc"){
        test_malloc();
    }else if(mode=="umf"){
        test_umf();
    }
    
    umf_alloc_init();
    void* ptr =umf_alloc(0, 8, 8);
    umf_free(0, ptr);
    return 0;
}
