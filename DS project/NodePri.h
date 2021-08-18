#pragma once


template < typename T>
class NodePri
{
private:
	T item; // A data item
	NodePri<T>* next; // Pointer to next node
	int priority;
public:
	NodePri();
	NodePri(const T& r_Item,int pr);
	NodePri(const T& r_Item, NodePri<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(NodePri<T>* nextNodePtr);
	T getItem() const;
	int getpriority();
	NodePri<T>* getNext() const;

}; // end Node


template < typename T>
NodePri<T>::NodePri()
{
	next = nullptr;
}

template < typename T>
NodePri<T>::NodePri(const T& r_Item,int pr)
{
	item = r_Item;
	priority = pr;
	next = nullptr;
}

template < typename T>
NodePri<T>::NodePri(const T& r_Item, NodePri<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void NodePri<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void NodePri<T>::setNext(NodePri<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T NodePri<T>::getItem() const
{
	return item;
}

template < typename T>
int NodePri<T>::getpriority()
{
	return priority;
}

template < typename T>
NodePri<T>* NodePri<T>::getNext() const
{
	return next;
}
