#include <iostream>
#include "numatype.hpp"
#include <cstring>

class HashNode {
public:
    char* key;
    int count;
    HashNode* next;

    HashNode(const char* word);

    virtual ~HashNode();
};

template<>
class numa<HashNode,0>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(0 ,sizeof(HashNode),alignof(HashNode));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashNode), 0);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(0 ,sizeof(HashNode),alignof(HashNode));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashNode), 0);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(0,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashNode));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(0,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashNode));
        #endif
    }
public:
numa<char*,0> key;
numa<int,0> count;
numa<HashNode*,0> next;
numa (const char * word){
    this->count = 1;
    this->next = nullptr;
    this->key = reinterpret_cast<char *>(reinterpret_cast<char *>(new numa<char,0>[strlen(word) + 1]));
    strcpy(this->key, word);
}
virtual ~numa()
{
        delete[] key;
    }
private:
};

template<>
class numa<HashNode,1>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(1 ,sizeof(HashNode),alignof(HashNode));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashNode), 1);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(1 ,sizeof(HashNode),alignof(HashNode));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashNode), 1);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(1,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashNode));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(1,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashNode));
        #endif
    }
public:
numa<char*,1> key;
numa<int,1> count;
numa<HashNode*,1> next;
numa (const char * word){
    this->count = 1;
    this->next = nullptr;
    this->key = reinterpret_cast<char *>(reinterpret_cast<char *>(new numa<char,1>[strlen(word) + 1]));
    strcpy(this->key, word);
}
virtual ~numa()
{
        delete[] key;
    }
private:
};

HashNode::HashNode(const char* word){
        count = 1;
        next = nullptr;
        key = new char[strlen(word) + 1];
        strcpy(key, word);
    }
HashNode::~HashNode() {
        delete[] key;
    }