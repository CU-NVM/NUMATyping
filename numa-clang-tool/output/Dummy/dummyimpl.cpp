#include "dummyheader.h"
#include "anotherdummyheader.h"
//#include "numatype.hpp"

class SomeClass{
    public:
    SomeClass(){};
    int a;
    MyVector mv;

    virtual void someFunction(){
        return;
    }
};

template<>
class numa<SomeClass,3>{
int main(){
    numa<MyVector, 3>* v1 = new numa<MyVector, 3>();
    numa<SomeClass, 3>* v3 = new numa<SomeClass, 3>();
    int* dummyint = new int[10];
    //numa<SomeOtherClass, 3>* v4 = new numa<SomeOtherClass, 3>();
    //MyVector* v = new MyVector(10);
    dummyFunction();
    anotherdummyFunction();
    return 0;
}