#include "numatype.hpp"
#ifndef _NODE_HPP_
#define _NODE_HPP_

class Node
{
private:
	int data;
	Node *link;

public:
	Node() : data(0)
	{}
	Node(int initData);
	Node(int, Node*);
	virtual Node *getLink();
	virtual void setLink(Node *n);

	virtual int getData();
	virtual void setData(int n);

	virtual ~Node();
};

template<>
class numa<Node,0>{
public: 
    static void* operator new(std::size_t sz){
        std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof(Node), 0);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
		std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof(Node), 0);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof(Node));
    }

    static void operator delete[](void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof(Node));
    }
public:
numa (): data(0){
}
numa (int initData){
    this->data = initData;
}
numa (int initData, Node * node){
    this->data = initData;
    this->link = node;
}
virtual Node * getLink(){
    return this->link;
}
virtual void setLink(Node * n){
    this->link = n;
}
virtual int getData(){
    return this->data;
}
virtual void setData(int n){
    this->data = n;
}
virtual ~numa()
{
	link = nullptr;
}
private:
numa<int,0> data;
numa<Node*,0> link;
};


Node::Node(int initData)
{
	data = initData;
}

Node::Node(int initData, Node *node)
{
	data = initData;
	link = node;
}
 
Node* Node::getLink()
{
	return link;
}
void Node::setLink(Node *n)
{
	link = n;
}

int Node::getData()
{
	return data;
}

void Node::setData(int n)
{
	data = n;
}

Node::~Node()
{
	link = nullptr;
}



#endif //_NODE_HPP_