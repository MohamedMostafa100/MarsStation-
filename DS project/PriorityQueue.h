#pragma once


/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					-----------
					|item| next| -->NULL
					-----------

*/


#include "NodePri.h"
#include "QueueADT.h"
#include <vector>
using namespace std;


template <typename T>
class PriorityQueue :public QueueADT<T>
{
private:

	NodePri<T>* backPtr;
	NodePri<T>* frontPtr;
	int count;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool enqueue(const T& newEntry,int priority);
	bool enqueuelowest(const T& newEntry, int priority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	void PrintQueue();
	int getcount();
	~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue<T>& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	frontPtr = nullptr;
	backPtr = nullptr;
	count = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template<typename T>
inline bool PriorityQueue<T>::enqueue(const T& newEntry)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry,int priority)
{
	NodePri<T>* p=frontPtr;
	NodePri<T>* newNodePtr = new NodePri<T>(newEntry,priority);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
	{
		frontPtr = newNodePtr; // The queue is empty
		p = frontPtr;
		
	}
	else
	{
		if (frontPtr->getpriority() < priority)
		{
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
			
		}
		else
		{
			while (p->getNext() && p->getNext()->getpriority() > priority) //traverse
			{
				p = p->getNext();
			}


			newNodePtr->setNext(p->getNext());
			p->setNext(newNodePtr);
			
		}
	}
	count++;
	
	return true;
} // end enqueue

template<typename T>
inline bool PriorityQueue<T>::enqueuelowest(const T& newEntry, int priority)
{
	NodePri<T>* p = frontPtr;
	NodePri<T>* newNodePtr = new NodePri<T>(newEntry, priority);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
	{
		frontPtr = newNodePtr; // The queue is empty
		p = frontPtr;
		
	}
	else
	{
		if (frontPtr->getpriority() > priority)
		{
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
		
		}
		else
		{
			while (p->getNext() && p->getNext()->getpriority() < priority)
			{
				p = p->getNext();
			}


			newNodePtr->setNext(p->getNext());
			p->setNext(newNodePtr);
			
		}
	}
	count++;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	NodePri<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	count--;
	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
template <typename T>
void PriorityQueue<T>::PrintQueue()
{
	T K;
	cout << "\nQueue contents: ";
	while (dequeue(K))
		cout << K << " ";
	cout << endl;
}
template<typename T>
inline int PriorityQueue<T>::getcount()
{
	return count;
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy,
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& LQ)
{
	count = 0;
	NodePri<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	NodePri<T>* ptr = new NodePri<T>(NodePtr->getItem(),NodePtr->getpriority());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();
	count++;

	//insert remaining nodes
	while (NodePtr)
	{
		NodePri<T>* ptr = new NodePri<T>(NodePtr->getItem(), NodePtr->getpriority());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
		count++;
	}
}


