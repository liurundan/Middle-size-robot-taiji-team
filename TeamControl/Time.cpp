#include "StdAfx.h"
#include "Time.h"

/************************************************************************/
/*   testing time                                                       */
/************************************************************************/

const Time Time::starting_time;

Time::Time() throw()
{
  QueryPerformanceFrequency(&litmp);
  dfFreq = (double)litmp.QuadPart; // 获得计数器的时钟频率
   
  update();
}

Time::Time (const  Time& src) throw () : QPart1( src.QPart1 ),dfFreq(src.dfFreq),
										 sec (src.sec), usec (src.usec) {;}
const  Time&  Time::operator= (const  Time& src) throw () 
{
  QPart1 = src.QPart1;
  dfFreq = src.dfFreq;
  sec=src.sec;
  usec=src.usec;
  return *this;
}

void  Time::set_usec (const long int& d) throw () 
{
  QPart1=starting_time.QPart1;
  dfFreq=starting_time.dfFreq;

  sec=starting_time.sec;
  usec=starting_time.usec;
  add_usec (d);
}

void  Time::set_msec (const long int& d) throw () 
{
  QPart1=starting_time.QPart1;
  dfFreq=starting_time.dfFreq;
  sec=starting_time.sec;
  usec=starting_time.usec;
  add_msec( d );
}

void Time::set_sec (const long int& d) throw () 
{
  QPart1=starting_time.QPart1;
  dfFreq=starting_time.dfFreq;
  sec=starting_time.sec;
  usec=starting_time.usec;
  
  add_sec (d);
}

void  Time::add_usec( long int n ) throw () 
{
  double dQPart = n * dfFreq * 1e-6;
  QPart1 += static_cast<__int64>(dQPart);
  
  usec+=n;
  if (usec<0) 
  {
    long int k=-usec/1000000+1;
    sec-=k;
    usec+=1000000*k;
  } 
  else if (usec>=1000000) 
  {
    long int k=usec/1000000;
    sec+=k;
    usec-=1000000*k;
  }

}

void  Time::add_msec (long int n) throw () 
{
  add_usec (1e3*n);
}

void  Time::add_sec (long int n) throw () 
{
  add_usec (1e6*n); 
   
}

void Time::update() throw() 
{
 	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;          // 获得初始值

	SYSTEMTIME sys;
    GetLocalTime(&sys);
    sec  = sys.wSecond;
    usec = sys.wMilliseconds*1e3;
}

long int Time::elapsed_sec () const throw () 
{
  Time now;
  return now.diff_sec (*this);
}

long int  Time::elapsed_usec () const throw () 
{
  Time now;
  return now.diff_usec (*this);
}

long int  Time::elapsed_msec () const throw () 
{
  Time now;
  return now.diff_msec (*this);
}

long int  Time::diff_usec (const Time& src) const throw () 
{
   return  (long int)((QPart1-src.QPart1)/ dfFreq*1e6);
}

long int  Time::diff_msec (const Time& src) const throw () 
{
   return  (long int)((QPart1-src.QPart1)/ dfFreq*1e3);
}

long int  Time::diff_sec (const Time& src) const throw () 
{
   return  (long int)((QPart1-src.QPart1)/ dfFreq);
}

bool  Time::operator== (const Time& src) const throw () 
{
  return (QPart1==src.QPart1);
}

bool  Time::operator!= (const Time& src) const throw ()
 {
  return !operator==(src);
}

bool  Time::operator<= (const Time& src) const throw () 
{
  return (diff_usec (src)<=0);
}

bool  Time::operator< (const Time& src) const throw () 
{
  return (diff_usec (src)<0);
}

bool  Time::operator>= (const Time& src) const throw () 
{
  return (diff_usec (src)>=0);
}

bool  Time::operator> (const Time& src) const throw () 
{
  return (diff_usec (src)>0);
}

long int  Time::get_usec () const throw () 
{
  return  (long int)((QPart1-Time::starting_time.QPart1)/ dfFreq*1e6);
}

long int Time::get_msec () const throw () 
{
  return (long int)((QPart1-Time::starting_time.QPart1)/ dfFreq*1e3);
}

long int  Time::get_sec () const throw () 
{
  return (long int)((QPart1-Time::starting_time.QPart1)/ dfFreq);
}

void Time::set (const SYSTEMTIME& sys) throw () 
{
  sec  = sys.wSecond;
  usec = sys.wMilliseconds*1e3;
}

void Time::get (SYSTEMTIME& sys) const throw () 
{
  sys.wSecond=sec;
  sys.wMilliseconds=usec*1e-3;
}

std::ostream& operator<< (std::ostream& os, const Time& tt) throw()
{
  os << tt.get_msec();
  return os;
} 