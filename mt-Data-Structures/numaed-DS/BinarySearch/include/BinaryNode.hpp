#ifndef _BINARYNODE_HPP_
#define _BINARYNODE_HPP_


#include <cstddef>
#include "numatype.hpp"

class BinaryNode;
template<>
class numa<BinaryNode, 1>{
private:
	numa<int,1> data;
	numa<BinaryNode*,1> leftChild;
	numa<BinaryNode*,1> rightChild;

public:
	numa() : data(0), leftChild(NULL), rightChild(NULL)
	{}

	numa(int data) 
	: data(data), leftChild(NULL), rightChild(NULL)
	{}

	~numa()
	{
		
		leftChild = NULL;
		rightChild = NULL;
	}

	int getData() { return data; }
	void setData(int data) { data = data; }

	BinaryNode *getLeftChild() { return leftChild; }
	BinaryNode *getRightChild() { return rightChild; }

	void setLeftChild(BinaryNode *node) { leftChild = node; }
	void setRightChild(BinaryNode *node) { rightChild = node; }

};















/*!
 * \class BinaryNode
 *
 * \brief Interface for a simple BinaryNode.
 *
 * This is the interface for a binary node. The binary
 * node inherits from a Node object.  Members added to 
 * the BinaryNode are isRoot, leftChild, rightChild
 *
 */

class BinaryNode //: public Node
{
private:
	//< Inherits int data from Node
	int data;
	BinaryNode *leftChild;
	BinaryNode *rightChild;

public:
	//< Inherits int getData() from Node 


	BinaryNode() : data(0), leftChild(NULL), rightChild(NULL)
	{}

	BinaryNode(int data) 
	: data(data), leftChild(NULL), rightChild(NULL)
	{}

	~BinaryNode()
	{
		
		leftChild = NULL;
		rightChild = NULL;
	}

	int getData() { return data; }
	void setData(int data) { data = data; }

	BinaryNode *getLeftChild() { return leftChild; }
	BinaryNode *getRightChild() { return rightChild; }

	void setLeftChild(BinaryNode *node) { leftChild = node; }
	void setRightChild(BinaryNode *node) { rightChild = node; }



};

#endif /* _BINARYNODE_HPP_ */