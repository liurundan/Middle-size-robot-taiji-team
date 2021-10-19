#ifndef _Mytime_
#define _Mytime_

#include <windows.h>
 
class MyTime
{
public:
     MyTime () throw ();
     MyTime (const MyTime&) throw ();
     const MyTime& operator= (const MyTime&) throw ();

     long int get_usec () const throw ();
     long int get_msec () const throw ();
     long int get_sec () const throw ();

     void set_usec (const long int&) throw ();
     void set_msec (const long int&) throw ();
     void set_sec (const long int&) throw ();

     void update () throw ();

     long int elapsed_usec () const throw ();
     long int elapsed_msec () const throw ();
     long int elapsed_sec () const throw ();

     void add_usec (long int) throw ();
     void add_msec (long int) throw ();
     void add_sec (long int) throw ();

     long int diff_usec (const MyTime&) const throw ();
     long int diff_msec (const MyTime&) const throw ();
     long int diff_sec (const MyTime&) const throw ();

     bool operator== (const MyTime&) const throw ();
     bool operator!= (const MyTime&) const throw ();
     bool operator<= (const MyTime&) const throw ();
     bool operator< (const MyTime&) const throw ();
     bool operator>= (const MyTime&) const throw ();
     bool operator> (const MyTime&) const throw ();

     void set (const SYSTEMTIME&) throw ();
     void get (SYSTEMTIME&) const throw ();
protected:
private:
     long int sec;   
     long int usec;   
     static const MyTime starting_time;    
};

#endif
