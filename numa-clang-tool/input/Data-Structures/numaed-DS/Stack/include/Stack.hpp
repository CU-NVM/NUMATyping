/*! \file Stack.hpp
 * \brief Interface for a simple Stack class
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _STACK_HPP_
#define _STACK_HPP_


#include "Node.hpp"
#include "numatype.hpp"
#include <iostream>

/*!
 * \class Stack
 *
 * \brief Interface for a simple Stack class.
 *
 * This is the interface for a simple stack class.
 * The class utilizes the \class Node class as stack objects.
 * Thus, the stack is implemented as a Linked List.
 *
 */


class Stack
{

private:
	Node *top; //< Pointer to the top of the Stack


public:

	Stack();

	~Stack();
	
	int pop();

	virtual void push(int);

	virtual void display();


};

Stack::Stack()
{
	top = nullptr;
}


Stack::~Stack()
{
	
	if(top == nullptr)
	{
		return;
	}
	Node *temp;
	while(top != nullptr)
	{
		temp = top;
		top = top->getLink();
		delete temp;
	}

}


int Stack::pop()
{
	if(top == nullptr)
	{
		return -1;
	}
	Node *retN = top;
	top = top->getLink();
	int data = retN->getData();
	delete retN;
	retN = nullptr;

	return data;
}


void Stack::push(int data)
{
	Node *newN = new Node(data);
	if(newN == nullptr)
	{
		std::cerr << "Stack full" << std::endl;
		return;
	}

	//Node *newNext = newN->getLink();
	newN->setLink(top);
	top = newN;
}

void Stack::display()
{
	if(top == nullptr)
	{
		std::cout << "Stack Empty!!" << std::endl;
		return;
	}

	int i = 0;
	
	Node *temp = top;
	while (temp != nullptr)
	{    
		
		if(i == 0)
			std::cout << "TOP ";

		std::cout << temp->getData() << std::endl;
		temp = temp->getLink();
		i++;
	}
}

template<>
class numa<Stack, 1>:public Stack{
private:
	// Node* top;
    // numa<Node, 1>* top;
	numa<Node*,1> top;
public:
	numa(){
		top = nullptr;
	}
	 
	~numa(){
		if(top == nullptr)
		{
			return;
		}
		Node* temp;
		while(top != nullptr)
		{
			temp = top;
			top = top->getLink();
			delete temp;
		}
	}
	int pop();
	virtual void push(int data){
		Node* newN = reinterpret_cast<Node*>(new numa<Node,1>(data));
		std::cerr << "Pushing " << data << std::endl;
		if(newN == nullptr)
		{
			std::cerr << "Stack full" << std::endl;
			return;
		}

		Node *newNext = newN->getLink();
		newN->setLink(top);
		top = newN;
	}

	virtual void display(){
		if(top == nullptr)
		{
			std::cout << "Stack Empty!!" << std::endl;
			return;
		}

		int i = 0;
		
		Node* temp = top;
		while (temp != nullptr)
		{
			
			if(i == 0)
				std::cout << "TOP ";

			std::cout << temp->getData() << std::endl;
			temp = temp->getLink();
			i++;
		}
}

};

int numa<Stack,1> :: pop(){
	if(top == nullptr)
	{
		return -1;
	}
	Node* retN = top;
	top = top->getLink();
	int data = retN->getData();
	delete retN;
	retN = nullptr;

	return data;
}


#endif //_STACK_HPP_