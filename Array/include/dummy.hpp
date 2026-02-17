#ifndef _DUMMY_HPP_
#define _DUMMY_HPP_



//#include "BinaryNode.hpp"

#include <iostream>
#include "numatype.hpp" 
using namespace std;

class Dummy{
    int x;
    public:
    Dummy(int);
    ~Dummy();
};

Dummy::Dummy(int a){
    x=a;
}

Dummy::~Dummy(){

}

#endif
