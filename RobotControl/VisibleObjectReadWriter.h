#ifndef _VisibleObjectReadWriter_h
#define _VisibleObjectReadWriter_h

#include <iostream>
#include "VisibleObject.h"

class VisibleObjectWriter
{
public:
    VisibleObjectWriter (std::ostream&) throw ();
    ~VisibleObjectWriter () throw ();
    void write (unsigned long int, unsigned long int, const VisibleObject&) throw ();
    void write (unsigned long int, unsigned long int, const VisibleObjectList&) throw ();
private:
	std::ostream& dest;

};

class VisibleObjectReader
{
public:
    VisibleObjectReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, unsigned long int&, VisibleObjectList&, unsigned long int) throw ();
private:
	std::istream& src;
    unsigned long int next;
};

#endif