#ifndef _RobotLocationReadWriter_h
#define _RobotLocationReadWriter_h

#include <iostream>
#include "RobotLocation.h"

class RobotLocationWriter
{
public:
    RobotLocationWriter (std::ostream&) throw ();
    ~RobotLocationWriter () throw ();
    void write (unsigned long int, unsigned long int, 
		const RobotLocation&, unsigned long int, 
		const RobotLocation&) throw ();
private:
	    std::ostream& dest;

};

class RobotLocationReader
{
public:
    RobotLocationReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, unsigned long int&, 
		RobotLocation&, unsigned long int&, RobotLocation&, 
		unsigned long int) throw ();

private:
    std::istream& src;
    unsigned long int next;

};

#endif
