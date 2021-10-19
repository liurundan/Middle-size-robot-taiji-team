#include "StdAfx.h"
#include "MyTime.h"

const MyTime MyTime::starting_time;
MyTime::MyTime() throw()
{
  update();
}

MyTime::MyTime (const  MyTime& src) throw () : sec (src.sec), usec (src.usec) {;}

const  MyTime&  MyTime::operator= (const  MyTime& src) throw () 
{
  sec=src.sec;
  usec=src.usec;
  return *this;
}

void  MyTime::set_usec (const long int& d) throw () 
{
  sec=starting_time.sec;
  usec=starting_time.usec;
  add_usec (d);
}

void  MyTime::set_msec (const long int& d) throw () 
{
  sec=starting_time.sec;
  usec=starting_time.usec;
  add_msec (d);
}

void   MyTime::set_sec (const long int& d) throw () 
{
  sec=starting_time.sec;
  usec=starting_time.usec;
  add_sec (d);
}

long int  MyTime::elapsed_usec () const throw () 
{
  MyTime now;
  return now.diff_usec (*this);
}

long int  MyTime::elapsed_msec () const throw () 
{
  MyTime now;
  return now.diff_msec (*this);
}

long int  MyTime::elapsed_sec () const throw () 
{
  MyTime now;
  return now.diff_sec (*this);
}

void  MyTime::add_usec (long int n) throw () 
{
  usec+=n;
  if (usec<0) {
    long int k=-usec/1000000+1;
    sec-=k;
    usec+=1000000*k;
  } else if (usec>=1000000) {
    long int k=usec/1000000;
    sec+=k;
    usec-=1000000*k;
  }
}

void  MyTime::add_msec (long int n) throw () 
{
  add_usec (1000*n);
}

void  MyTime::add_sec (long int n) throw () 
{
  sec+=n;
}

long int  MyTime::diff_usec (const  MyTime& src) const throw () 
{
  return 1000000*(sec-src.sec)+(usec-src.usec);
}

long int  MyTime::diff_msec (const  MyTime& src) const throw () 
{
  return 1000*(sec-src.sec)+(usec-src.usec)/1000;
}

long int  MyTime::diff_sec (const  MyTime& src) const throw () 
{
  return sec-src.sec;
}

bool  MyTime::operator== (const   MyTime& src) const throw () 
{
  return ((sec==src.sec) && (usec==src.usec));
}

bool  MyTime::operator!= (const  MyTime& src) const throw ()
 {
  return !operator==(src);
}

bool  MyTime::operator<= (const  MyTime& src) const throw () 
{
  return (diff_usec (src)<=0);
}

bool  MyTime::operator< (const  MyTime& src) const throw () 
{
  return (diff_usec (src)<0);
}

bool  MyTime::operator>= (const MyTime& src) const throw () 
{
  return (diff_usec (src)>=0);
}

bool  MyTime::operator> (const  MyTime& src) const throw () 
{
  return (diff_usec (src)>0);
}

long int  MyTime::get_usec () const throw () 
{
  return (sec-MyTime::starting_time.sec)*1000000+usec-MyTime::starting_time.usec;
}

long int MyTime::get_msec () const throw () 
{
  return (sec-MyTime::starting_time.sec)*1000+(usec-MyTime::starting_time.usec)/1000;
}

long int  MyTime::get_sec () const throw () 
{
  return (sec-MyTime::starting_time.sec);
}

void  MyTime::set (const SYSTEMTIME& sys) throw () 
{
  sec=sys.wSecond;
  usec=sys.wMilliseconds;
}

void  MyTime::get (SYSTEMTIME& sys) const throw () 
{
  sys.wSecond=sec;
  sys.wMilliseconds=usec;
}

void MyTime::update() throw() 
{
  SYSTEMTIME sys;
  GetLocalTime(&sys);
   sec  = sys.wSecond;
  usec = sys.wMilliseconds;
}