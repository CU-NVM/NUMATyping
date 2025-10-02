#include <iostream>
#include "numatype.hpp"
#include <cstring>

class HashNode {
public:
    char* key;
    int count;
    HashNode* next;

    HashNode(const char* word);

    ~HashNode();
};

HashNode::HashNode(const char* word) : count(1), next(nullptr) {
        key = new char[strlen(word) + 1];
        strcpy(key, word);
    }
HashNode::~HashNode() {
        delete[] key;
    }