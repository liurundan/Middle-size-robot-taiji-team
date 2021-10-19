
#ifndef _ANGLE_H_
#define _ANGLE_H_

#include <math.h>
#include <iostream>

using namespace std;

#ifndef M_PI
#define M_PI  3.1415926535897932384626433832795029
#endif

/**
   This class is used for unambiguous representation of angles. 
   
   Angles are given in radians, and normalized to be in [0,...,2*PI). 
   It is allowed to use values not from [0,...,2*PI) as parameters, they
   will be always normalized to fit into this interval.

            ^  
            | 0.5 Pi
            |
   Pi       |          0
     -------O--------> 
            |
            |
            | 1.5 Pi
*/
class Angle 
{
  friend ostream& operator<< (ostream&,const Angle&);
  friend istream& operator>> (istream&,Angle&);
  friend double cos(const Angle &);
  friend double sin(const Angle &);
  friend Angle operator+(const Angle &, const Angle &);
  friend Angle operator-(const Angle &, const Angle &);
  friend Angle operator-(const Angle &);
  double ang; 
  static const double TwoPi;
  //important class invariant:  0.0 <= ang < 2*PI
 public:
  Angle();
  Angle( const Angle& );
  Angle( double );
  double get_value() const { return ang; }
  double get_value_0_p2PI() const { return ang; }
  double get_value_mPI_pPI() const { return ang<= M_PI ? ang: ang-TwoPi; }
  void set_value(double);

  void    operator +=(const Angle & a);
  void    operator -=(const Angle & a);
};

#endif

