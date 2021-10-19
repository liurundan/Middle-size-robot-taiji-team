#ifndef _stringconvert_h
#define _stringconvert_h

#include <string>
#include <deque>
#include <stdexcept>

void split_string (std::deque<std::string>&, const std::string&) throw (std::bad_alloc);

bool prefix (const std::string&, const std::string) throw ();

bool string2double (double&, const std::string&) throw ();

bool string2float (float&, const std::string&) throw ();


bool string2int (int&, const std::string&) throw ();


bool string2uint (unsigned int&, const std::string&) throw ();


bool string2lint (long int&, const std::string&) throw ();


bool string2ulint (unsigned long int&, const std::string&) throw ();


bool string2bool (bool&, const std::string&) throw ();

#endif