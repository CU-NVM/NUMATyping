#include "HashNode.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <mutex>

using namespace std;

class HashTable {
    HashNode** table;
    int bucket_count;
    std::vector<std::mutex> bucket_locks; // lock for each bucket

    int hash(const char* key);
public:
    HashTable(int buckets);
    ~HashTable();

    void insert(const char* key);
    void remove(const char* key);
    int getCount(const char* key);
    bool updateCount(const char* key, int count);
    void printAll();
};

HashTable::HashTable(int buckets) : bucket_count(buckets), bucket_locks(buckets) { // had to init locks here or breaks everything
    // keeping this here to change original data structure as little as possible
    table = new HashNode*[buckets];
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
    std::lock_guard<std::mutex> lock(bucket_locks[idx]); // lock (lock_guard will unlock when function completes)

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
    std::lock_guard<std::mutex> lock(bucket_locks[idx]); // lock

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
    std::lock_guard<std::mutex> lock(bucket_locks[idx]); // lock

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
    std::lock_guard<std::mutex> lock(bucket_locks[idx]); // lock
    
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


void HashTable::printAll(){
    for(int i = 0; i < bucket_count; i++) {
        std::lock_guard<std::mutex> lock(bucket_locks[i]); // lock
        HashNode* curr = table[i];
        while(curr){
            std::cout << curr->key << ": " << curr->count << std::endl;
            curr = curr->next;
        }
    }
}