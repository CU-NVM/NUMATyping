/*! \file TestSuite.cpp
 * \brief Testsuite implementation which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "Node.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearch.hpp"
#include "LinkedList.hpp"
#include "numatype.hpp"

#include <iostream>
#include <thread>
// #include <barrier>
#include <mutex>
#include <syncstream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <pthread.h>


mutex* Stack_lk;
mutex* Queue_lk;
mutex* BST_lk;
mutex* LL_lk;



numa<Stack,0> *stack;
numa<Queue,0> *queue;
numa<BinarySearchTree,0> *bst;
numa<LinkedList,0> *ll;
pthread_barrier_t bar;

chrono::high_resolution_clock::time_point startTime;
chrono::high_resolution_clock::time_point endTime;


void DS_init(){
	stack = new numa<Stack,0>();
	queue = new numa<Queue,0>();
	bst = new numa<BinarySearchTree,0>();
	ll = new numa<LinkedList,0>();
	
}

void sync_init(int num_threads){
    Stack_lk = new mutex();
	Queue_lk = new mutex();
	BST_lk = new mutex();
	LL_lk = new mutex();
	pthread_barrier_init(&bar, nullptr, num_threads);
	//Zbar = new std::barrie(num_threads);
}



void* StackTest(int tid, int num_threads)
{	
	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			startTime = chrono::high_resolution_clock::now();
			std::cout << "Only thread "<< tid << " will print this." << std::endl;
			
		}
	#endif

	pthread_barrier_wait(&bar);

	Stack_lk->lock();
	stack->push(tid*10);
	Stack_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
		std::cout << "----------------------------------" << std::endl;
		std::cout << "Display Stack after adding " << num_threads << " nodes." << std::endl;
		std::cout << "----------------------------------" << std::endl;
		stack->display();
		}
	#endif

	pthread_barrier_wait(&bar);

	Stack_lk->lock();
	stack->push(tid*100);
	Stack_lk->unlock();

	#ifdef DEBUG
	pthread_barrier_wait(&bar);
		if(tid == 1)
		{
		std::cout << "----------------------------------" << std::endl;
		std::cout << "Display Stack after adding " << num_threads << " more nodes." << std::endl;
		std::cout << "----------------------------------" << std::endl;
		stack->display();
		}
	#endif
	pthread_barrier_wait(&bar);

	Stack_lk->lock();
	stack->pop();
	Stack_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
		std::cout << "----------------------------------" << std::endl;
		std::cout << "Display Stack after removing " << num_threads << " nodes." << std::endl;
		std::cout << "----------------------------------" << std::endl;
		stack->display();
		}
	#endif
	pthread_barrier_wait(&bar);

	Stack_lk->lock();
	stack->pop();
	Stack_lk->unlock();

	pthread_barrier_wait(&bar);
	if(tid == 1)
	{
		#ifdef DEBUG
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display Stack after removing " << num_threads << " nodes.(empty)" << std::endl;
			std::cout << "----------------------------------" << std::endl;
			stack->display();
		#endif
		endTime = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = endTime - startTime;
		std::cout << "Time taken for StackTest: " << elapsed.count() << "s\n";
	}
	pthread_barrier_wait(&bar);
	return nullptr;
}

void* QueueTest(int tid, int num_threads)
{
	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			startTime = chrono::high_resolution_clock::now();
			std::cout << "Only thread "<< tid << " will print this." << std::endl;
			
		}
	#endif

	pthread_barrier_wait(&bar);
	Queue_lk->lock();
	queue->add(tid*10);
	Queue_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display Queue after adding " << num_threads << " nodes." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			queue->display();
		}
	#endif

	pthread_barrier_wait(&bar);
	Queue_lk->lock();
	queue->add(tid* 100);
	Queue_lk->unlock();
	
	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display Queue after adding "<< num_threads << " more node." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			queue->display();
		}
	#endif

	pthread_barrier_wait(&bar);
	Queue_lk->lock();
	queue->del();
	Queue_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display Queue after removing " << num_threads << " nodes." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			queue->display();
		}
	#endif
	pthread_barrier_wait(&bar);

	Queue_lk->lock();
	queue->del();
	Queue_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display Queue after removing "<< num_threads<< " more node. (empty)" << std::endl;
			std::cout << "----------------------------------" << std::endl;
			queue->display();
		}
	#endif
	
	pthread_barrier_wait(&bar);
	if(tid == 1)
	{
		endTime = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = endTime - startTime;
		std::cout << "Time taken for QueueTest: " << elapsed.count() << "s\n";
	}

	pthread_barrier_wait(&bar);
	return nullptr;
}

void* BinarySearchTest(int tid, int num_threads)
{
	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			startTime = chrono::high_resolution_clock::now();
			std::cout << "Only thread "<< tid << " will print this." << std::endl;
			
		}
	#endif

	pthread_barrier_wait(&bar);
	BST_lk->lock();
	bst->insert(tid*10);
	BST_lk->unlock();

	#ifdef DEBUG
	pthread_barrier_wait(&bar);

	if(tid == 1)
	{
		std::cout << "----------------------------------" << std::endl;
		std::cout << "Display BinarySearchTree after adding "<<num_threads<<" nodes." << std::endl;
		std::cout << "----------------------------------" << std::endl;

		std::cout << "Pre Order Print" << std::endl;
		bst->preOrderPrint();
		std::cout << std::endl;

		std::cout << "Post Order Print" << std::endl;
		bst->postOrderPrint();
		std::cout << std::endl;

		std::cout << "In Order Print" << std::endl;
		bst->inOrderPrint();
		std::cout << std::endl;

		std::cout << "----------------------------------" << std::endl;
		std::cout << "On the look out for 10" << std::endl;
		std::cout << "----------------------------------" << std::endl;
	}
	#endif 
	thread_local bool found = false;
	pthread_barrier_wait(&bar);
	if(bst->lookup(20))
	{
		BST_lk->lock();
		found = true;
		BST_lk->unlock();
	}
	else
	{
		BST_lk->lock();
		found = false;
		//std::cout << "Did not find 10" << std::endl;
		BST_lk->unlock();
	}

	pthread_barrier_wait(&bar);
	if(tid == 1)
	{
		endTime = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = endTime - startTime;
		std::cout << "Time taken for BinarySearchTest: " << elapsed.count() << "s\n";
	}
	pthread_barrier_wait(&bar);
	return nullptr;
}

void* LinkedListTest(int tid, int num_threads)
{
	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			startTime = chrono::high_resolution_clock::now();
			std::cout << "Only thread "<< tid << " will print this." << std::endl;
			
		}
	#endif

	pthread_barrier_wait(&bar);
	LL_lk->lock();
	ll->append(tid*10);
	LL_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display LinkedList after appending "<<num_threads<<" nodes." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			ll->display();
		}
	#endif

	pthread_barrier_wait(&bar);
	LL_lk->lock();
	ll->prepend(tid*100);
	LL_lk->unlock();

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display LinkedList after prepending "<< num_threads <<" nodes." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			ll->display();
		}
	#endif

	pthread_barrier_wait(&bar);

	ll->insertAfter(10, 20);

	#ifdef DEBUG
		pthread_barrier_wait(&bar);
		if(tid == 1)
		{
			std::cout << "----------------------------------" << std::endl;
			std::cout << "Display LinkedList after "<< num_threads<< " numbers of threads insert 20 after 10." << std::endl;
			std::cout << "----------------------------------" << std::endl;
			ll->display();
		}
	#endif
	pthread_barrier_wait(&bar);

	if(tid == 1)
	{
		endTime = chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = endTime - startTime;
		std::cout << "Time taken for LinkedListTest: " << elapsed.count() << "s\n";
	}
	pthread_barrier_wait(&bar);
	return nullptr;
}

void global_cleanup(){
	delete Stack_lk;
	delete Queue_lk;
	delete BST_lk;
	delete LL_lk;

	delete stack;
	delete queue;
	delete bst;
	delete ll;
}