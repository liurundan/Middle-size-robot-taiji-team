#ifndef _ObstacleLocationReadWriter_h
#define _ObstacleLocationReadWriter_h

#include <iostream>
#include "ObstacleLocation.h"

class ObstacleLocationWriter
{
public:
    ObstacleLocationWriter (std::ostream&) throw ();
    ~ObstacleLocationWriter () throw ();
    void write (unsigned long int, const ObstacleLocation&) throw ();
private:
    std::ostream& dest;

};

class ObstacleLocationReader
{
public:
    ObstacleLocationReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, ObstacleLocation&, unsigned long int) throw ();
private:
	std::istream& src;
    unsigned long int next;
};

#endif

