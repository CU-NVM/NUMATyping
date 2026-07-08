#include <iostream>
#include "numatype.hpp"
#include <cstring>
#include <cstdint>

// ---- Key hashing (selectable at runtime via --hash) ----------------------
// hash_mode(): 0 = djb2 (original), 1 = mix (djb2 followed by a strong
// avalanche finalizer). Shared int (inline function-local static) set once
// before the worker/prefill threads start, then only read.
inline int& hash_mode() { static int m = 0; return m; }

inline unsigned long djb2_hash(const char* s) {
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + c;   // h * 33 + c
    return h;
}

// splitmix64 / murmur3-style finalizer: tiny input change -> unrelated output.
inline uint64_t mix64(uint64_t x) {
    x ^= x >> 30; x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27; x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

// The hash every table/bucket selector should use.
inline unsigned long key_hash(const char* s) {
    unsigned long h = djb2_hash(s);
    return hash_mode() ? (unsigned long)mix64(h) : h;
}

class HashNode {
public:
    char* key;
    int count;
    HashNode* next;

    HashNode(const char* word);

    ~HashNode();
};

HashNode::HashNode(const char* word) {
        count = 1;
        next =nullptr;
        key = new char[strlen(word) + 1];
        strcpy(key, word);
    }
HashNode::~HashNode() {
        delete[] key;
    }
