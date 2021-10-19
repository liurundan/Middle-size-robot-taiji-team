#ifndef _Table_h
#define _Table_h

#include <stdexcept>

template<class C> class Table 
{
  public:
	  Table (unsigned int =1, unsigned int =1) throw (std::bad_alloc);
	  Table (const Table<C>&) throw (std::bad_alloc);
	  ~Table () throw ();
	  const Table<C>& operator= (const Table<C>&) throw (std::bad_alloc);
	  void resize (unsigned int, unsigned int) throw (std::bad_alloc);
	  unsigned int rows () const throw ();
	  unsigned int columns () const throw (); 
	  const C& operator() (unsigned int, unsigned int) const throw ();
	  C& operator() (unsigned int, unsigned int) throw ();
  private:
	  unsigned int nr, nc;
	  C* array;
};

 
template<class C> Table<C>::Table (unsigned int r, unsigned int c) throw (std::bad_alloc) : array (NULL) 
{
	resize (r,c);
}

template<class C> Table<C>::~Table () throw () { delete [] array; }

template<class C> void Table<C>::resize (unsigned int r, unsigned int c) throw (std::bad_alloc) 
{
	nr = r;
	nc = c;
	if (array) delete [] array;
	array = new C [nr*nc];
}

template<class C> unsigned int Table<C>::rows () const throw () { return nr; }

template<class C> unsigned int Table<C>::columns () const throw () { return nc; }

template<class C> const C& Table<C>::operator() (unsigned int i, unsigned int j) const throw () { return array[i*nc+j]; }

template<class C> C& Table<C>::operator() (unsigned int i, unsigned int j) throw () { return array[i*nc+j]; }

template<class C> const Table<C>& Table<C>::operator= (const Table<C>& src) 
throw (std::bad_alloc) 
{
	delete [] array;
	nr = src.nr;
	nc = src.nc;
	array = new C [nr*nc];
	const C* sp = src.array;
	C* dp = array;
	unsigned int n=nr*nc;
	for (unsigned int i=0; i<n; i++)
		(*dp++)=(*sp++);
	return (*this);
}

template<class C> Table<C>::Table (const Table<C>& src) throw (std::bad_alloc) 
{
	nr = src.nr;
	nc = src.nc;
	array = new C [nr*nc];
	const C* sp = src.array;
	C* dp = array;
	unsigned int n=nr*nc;
	for (unsigned int i=0; i<n; i++)
		(*dp++)=(*sp++);
}

#endif