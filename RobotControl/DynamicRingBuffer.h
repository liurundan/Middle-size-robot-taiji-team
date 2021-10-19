#ifndef _DynamicRingBuffer_h_
#define _DynamicRingBuffer_h_

#include <stdexcept>

template<class T> class DynamicRingBuffer 
{
	struct Node 
	{
		T elem;
		Node* next;  // DynamicRingBuffer<T>::Node* next;
	};
	private:
		unsigned int cur_size;        
		
		Node* anchor;                
		Node* pred_anchor;            
		Node* empty_nodes;            
		
	public:
		DynamicRingBuffer (unsigned int =1) throw (std::bad_alloc);
		~DynamicRingBuffer () throw ();
		unsigned int size () const throw ();
		void step (unsigned int =1) throw ();
		const T& get (unsigned int =0) const throw ();
		void set (const T&, unsigned int =0);
		void insert (const T&);
		void erase () throw ();
};


template<class T> DynamicRingBuffer<T>::DynamicRingBuffer (unsigned int n) 
throw (std::bad_alloc) : empty_nodes(NULL) 
{
	if (n==0) 
		n=1;
	cur_size = n;
	anchor = new DynamicRingBuffer<T>::Node;
	pred_anchor=anchor;
	while ((--n)>0) 
	{
		DynamicRingBuffer<T>::Node* new_node = new DynamicRingBuffer<T>::Node;
		new_node->next=anchor;
		anchor=new_node;
	}
	pred_anchor->next=anchor;
}

template<class T> DynamicRingBuffer<T>::~DynamicRingBuffer () throw () 
{
	pred_anchor->next = NULL;
	while (anchor->next) 
	{
		DynamicRingBuffer<T>::Node* new_node = anchor;
		anchor = anchor->next;
		delete new_node;
	}
	while (empty_nodes) 
	{
		DynamicRingBuffer<T>::Node* new_node = empty_nodes;
		empty_nodes=empty_nodes->next;
		delete new_node;
	}
}

template<class T> unsigned int DynamicRingBuffer<T>::size () const throw () 
{
	return cur_size;
}

template<class T> void DynamicRingBuffer<T>::step (unsigned int n) throw () 
{
	while (n-->0)
		pred_anchor=pred_anchor->next;
	anchor=pred_anchor->next;
}

template<class T> const T&  DynamicRingBuffer<T>::get (unsigned int n) const throw () 
{
	DynamicRingBuffer<T>::Node* ptr = anchor;
	while ((n--)>0)
		ptr=ptr->next;
	return ptr->elem;
}

template<class T> void DynamicRingBuffer<T>::set (const T& e, unsigned int n) 
{
	DynamicRingBuffer<T>::Node* ptr = anchor;
	while ((n--)>0)
		ptr=ptr->next;
	ptr->elem = e;
}

template<class T> void DynamicRingBuffer<T>::insert (const T& e) 
{
	if (empty_nodes) 
	{
		pred_anchor->next=empty_nodes;
		empty_nodes=empty_nodes->next;
	} 
	else 
		pred_anchor->next=new DynamicRingBuffer<T>::Node;   
	cur_size++;
	pred_anchor->next->next = anchor;
	pred_anchor->next->elem = e;
	anchor=pred_anchor->next;
}

template<class T> void DynamicRingBuffer<T>::erase () throw () 
{
	if (cur_size>1) 
	{
		cur_size--;
		pred_anchor->next=anchor->next;
		anchor->next=empty_nodes;
		empty_nodes=anchor;
		anchor=pred_anchor->next;
	}
}

#endif
