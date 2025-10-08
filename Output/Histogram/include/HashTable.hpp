#include "HashNode.hpp"
#include "numatype.hpp"
#include <iostream>
#include <cstring>
#include <vector>


using namespace std;
class HashTable{
    HashNode** table;
    int bucket_count;

    virtual int hash(const char* key);
public:
    HashTable(int buckets);
    virtual ~HashTable();

    virtual void insert(const char* key);
    virtual void remove(const char* key);
    virtual int getCount(const char* key);
    virtual bool updateCount(const char* key, int count);
    virtual bool exists(const char* key);
    virtual void printAll();
    virtual std::vector<char*> getAllKeys();
};

template<>
class numa<HashTable,0>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(0 ,sizeof(HashTable),alignof(HashTable));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashTable), 0);
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
            p= umf_alloc(0 ,sizeof(HashTable),alignof(HashTable));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashTable), 0);
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
		    numa_free(ptr, 1 * sizeof(HashTable));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(0,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashTable));
        #endif
    }
public:
numa (int buckets){
    this->table = reinterpret_cast<HashNode **>(reinterpret_cast<HashNode **>(new numa<HashNode *,0>[this->bucket_count]));
    for (int i = 0; i < this->bucket_count; i++) {
        this->table[i] = nullptr;
    }
}
virtual ~numa()
{
    for(int i = 0; i < bucket_count; i++) {
        HashNode* curr = table[i];
        while(curr) {
            HashNode* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
    }
    delete[] table;
}
virtual void insert(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                curr->count++;
                return;
            }
            curr = curr->next;
        }
    HashNode *newNode = reinterpret_cast<HashNode *>(reinterpret_cast<HashNode *>(new numa<HashNode,0>(word)));
    newNode->next = this->table[idx];
    this->table[idx] = newNode;
}
virtual void remove(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    HashNode *prev = nullptr;
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    this->table[idx] = curr->next;
                }
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
}
virtual int getCount(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                return curr->count;
            }
            curr = curr->next;
        }
    return 0;
}
virtual bool updateCount(const char * word, int count){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                curr->count += count;
                return true;
            }
            curr = curr->next;
        }
    return false;
}
virtual bool exists(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                return true;
            }
            curr = curr->next;
        }
    return false;
}
virtual void printAll(){
    for (int i = 0; i < this->bucket_count; i++) {
        HashNode *curr = this->table[i];
        while (curr)
            {
                std::cout << curr->key << ": " << curr->count << std::endl;
                curr = curr->next;
            }
    }
}
virtual std::vector<char *> getAllKeys(){
    std::vector<char *> keys;
    for (int i = 0; i < this->bucket_count; i++) {
        HashNode *curr = this->table[i];
        while (curr)
            {
                keys.push_back(curr->key);
                curr = curr->next;
            }
    }
    return keys;
}
private:
numa<HashNode **,0> table;
numa<int,0> bucket_count;
virtual int hash(const char * key){
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        {
            hash = ((hash << 5) + hash) + c;
        }
    return hash % this->bucket_count;
}
};

template<>
class numa<HashTable,1>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(1 ,sizeof(HashTable),alignof(HashTable));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashTable), 1);
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
            p= umf_alloc(1 ,sizeof(HashTable),alignof(HashTable));
        #else
            p = numa_alloc_onnode(sz* sizeof(HashTable), 1);
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
		    numa_free(ptr, 1 * sizeof(HashTable));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(1,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(HashTable));
        #endif
    }
public:
numa (int buckets){
    this->table = reinterpret_cast<HashNode **>(reinterpret_cast<HashNode **>(new numa<HashNode *,1>[this->bucket_count]));
    for (int i = 0; i < this->bucket_count; i++) {
        this->table[i] = nullptr;
    }
}
virtual ~numa()
{
    for(int i = 0; i < bucket_count; i++) {
        HashNode* curr = table[i];
        while(curr) {
            HashNode* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
    }
    delete[] table;
}
virtual void insert(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                curr->count++;
                return;
            }
            curr = curr->next;
        }
    HashNode *newNode = reinterpret_cast<HashNode *>(reinterpret_cast<HashNode *>(new numa<HashNode,1>(word)));
    newNode->next = this->table[idx];
    this->table[idx] = newNode;
}
virtual void remove(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    HashNode *prev = nullptr;
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    this->table[idx] = curr->next;
                }
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
}
virtual int getCount(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                return curr->count;
            }
            curr = curr->next;
        }
    return 0;
}
virtual bool updateCount(const char * word, int count){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                curr->count += count;
                return true;
            }
            curr = curr->next;
        }
    return false;
}
virtual bool exists(const char * word){
    int idx = this->hash(word);
    HashNode *curr = this->table[idx];
    while (curr)
        {
            if (strcmp(curr->key, word) == 0) {
                return true;
            }
            curr = curr->next;
        }
    return false;
}
virtual void printAll(){
    for (int i = 0; i < this->bucket_count; i++) {
        HashNode *curr = this->table[i];
        while (curr)
            {
                std::cout << curr->key << ": " << curr->count << std::endl;
                curr = curr->next;
            }
    }
}
virtual std::vector<char *> getAllKeys(){
    std::vector<char *> keys;
    for (int i = 0; i < this->bucket_count; i++) {
        HashNode *curr = this->table[i];
        while (curr)
            {
                keys.push_back(curr->key);
                curr = curr->next;
            }
    }
    return keys;
}
private:
numa<HashNode **,1> table;
numa<int,1> bucket_count;
virtual int hash(const char * key){
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        {
            hash = ((hash << 5) + hash) + c;
        }
    return hash % this->bucket_count;
}
};


HashTable::HashTable(int buckets) : bucket_count(buckets) {
    table = new HashNode*[bucket_count];
    for(int i = 0; i < bucket_count; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for(int i = 0; i < bucket_count; i++) {
        HashNode* curr = table[i];
        while(curr) {
            HashNode* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTable::hash(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % bucket_count;
}

void HashTable::insert(const char* word){
    int idx = hash(word);
    HashNode* curr = table[idx];
    while(curr){
        if(strcmp(curr->key, word)==0){
            curr->count++;
            return;
        }
        curr = curr->next;
    }
    HashNode* newNode = new HashNode(word);
    newNode->next = table[idx];
    table[idx] = newNode;
}

void HashTable::remove(const char* word){
    int idx = hash(word);
    HashNode* curr = table[idx];
    HashNode* prev = nullptr;
    while(curr){
        if(strcmp(curr->key, word)==0){
            if(prev){
                prev->next = curr->next;
            } else {
                table[idx] = curr->next;
            }
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int HashTable::getCount(const char* word){
    int idx = hash(word);
    HashNode* curr = table[idx];
    while(curr){
        if(strcmp(curr->key, word)==0){
            return curr->count;
        }
        curr = curr->next;
    }
    return 0;
}

bool HashTable::updateCount(const char* word, int count){
    int idx = hash(word);
    HashNode* curr = table[idx];
    while(curr){
        if(strcmp(curr->key, word)==0){
            curr->count += count;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool HashTable::exists(const char* word){
    int idx = hash(word);
    HashNode* curr = table[idx];
    while(curr){
        if(strcmp(curr->key, word)==0){
            return true;
        }
        curr = curr->next;
    }
    return false;
}


void HashTable::printAll(){
    for(int i = 0; i < bucket_count; i++) {
        HashNode* curr = table[i];
        while(curr){
            std::cout << curr->key << ": " << curr->count << std::endl;
            curr = curr->next;
        }
    }
}

std::vector<char*> HashTable::getAllKeys(){
    std::vector<char*> keys;
    for(int i = 0; i < bucket_count; i++) {
        HashNode* curr = table[i];
        while(curr){
            keys.push_back(curr->key);
            curr = curr->next;
        }
    }
    return keys;
}