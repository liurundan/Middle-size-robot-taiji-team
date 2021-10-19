#ifndef _DriveVectorReadWriter_h
#define _DriveVectorReadWriter_h

#include <iostream>
#include "DriveVector.h"

class DriveVectorWriter
{
public:
    DriveVectorWriter (std::ostream&) throw ();
    ~DriveVectorWriter () throw ();
    void write (unsigned long int, unsigned long int, const DriveVector&) throw ();
private:
	std::ostream& dest;

};

class DriveVectorReader
{
public:
    DriveVectorReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, unsigned long int&, DriveVector&, unsigned long int) throw ();
private:
	std::istream& src;
    unsigned long int next;
};

#endif