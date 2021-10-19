#ifndef _BallLocationReadWriter_h
#define _BallLocationReadWriter_h

#include <iostream>
#include "BallLocation.h"

class BallLocationWriter
{
public:
    BallLocationWriter (std::ostream&) throw ();
    ~BallLocationWriter () throw ();
    void write (unsigned long int, unsigned long int, const BallLocation&, 
		unsigned long int, const BallLocation&) throw ();

protected:
private:
    std::ostream& dest;

};

class BallLocationReader
{
public:
    BallLocationReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, unsigned long int&, BallLocation&, unsigned long int&, BallLocation&, unsigned long int) throw ();

protected:
private:
	std::istream& src;
    unsigned long int next;
};

#endif