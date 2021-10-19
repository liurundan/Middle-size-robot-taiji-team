#ifndef Tribots_RingBuffer_h_
#define Tribots_RingBuffer_h_

#include <vector>
 
template<class T> class RingBuffer 
{
public:

	RingBuffer (unsigned int) throw (std::bad_alloc);

	~RingBuffer () throw ();

 	unsigned int size () const throw ();

	void resize (unsigned int) throw (std::bad_alloc);

	void step (int =1) throw ();

	const T& get () const throw ();

	T& get () throw ();

	const T& operator[] (int) const throw ();

	T& operator[] (int) throw ();

private:
	int n;                     
	std::vector<T> elem;       
	int anchor;               


};
  
template<class T> RingBuffer<T>::RingBuffer (unsigned int n1) throw (std::bad_alloc) : n(n1), elem(n1), anchor(0) {;}

template<class T> RingBuffer<T>::~RingBuffer () throw () {;}

template<class T> unsigned int RingBuffer<T>::size () const throw () { return n; }

template<class T> void RingBuffer<T>::resize (unsigned int n1) throw (std::bad_alloc) 
{ 
  n=n1;
  elem.resize (n);
  if (anchor>=n)
    anchor=0;
}

template<class T> void RingBuffer<T>::step (int n1) throw () 
{
  anchor+=n1;
  while (anchor<0)
    anchor+=n;
  while (anchor>=n)
    anchor-=n;
}

template<class T> const T& RingBuffer<T>::get () const throw () { return elem[anchor]; }

template<class T> T& RingBuffer<T>::get () throw () { return elem[anchor]; }

template<class T> const T& RingBuffer<T>::operator[] (int i) const throw () 
{
  i+=anchor;
  while (i<0)
    i+=n;
  while (i>=n)
    i-=n;
  return elem[i];
}

template<class T> T& RingBuffer<T>::operator[] (int i) throw () 
{
  i+=anchor;
  while (i<0)
    i+=n;
  while (i>=n)
    i-=n;
  return elem[i];
}


#endif