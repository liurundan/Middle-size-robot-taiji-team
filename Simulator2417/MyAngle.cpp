#include "stdafx.h"
#include "MyAngle.h"
#include "math.h"
 
#define CONSTZWEIPI 6.283185307179586232
#define CONSTPI180 0.017453292519943295474
#define CONST180PI 57.295779513082322865

const MyAngle MyAngle::zero (0);
const MyAngle MyAngle::twelvth (M_PI/6);
const MyAngle MyAngle::eighth (M_PI/4);
const MyAngle MyAngle::sixth (M_PI/3);
const MyAngle MyAngle::quarter (M_PI/2);
const MyAngle MyAngle::three_eighth (3*M_PI/4);
const MyAngle MyAngle::half (M_PI);
const MyAngle MyAngle::five_eighth (5*M_PI/4);
const MyAngle MyAngle::three_quarters (1.5*M_PI);
const MyAngle MyAngle::five_sixth (5*M_PI/3);
const MyAngle MyAngle::seven_eighth (7*M_PI/4);
const MyAngle MyAngle::eleven_twelvth (11*M_PI/6);

bool MyAngle::operator== (const MyAngle& a) const throw () {
  return the_angle==a.the_angle;
}

bool MyAngle::operator!= (const MyAngle& a) const throw () {
  return the_angle!=a.the_angle;
}

bool MyAngle::in_between (const MyAngle a, const MyAngle b) const throw () {
  if (b.the_angle>=a.the_angle)
    return (a.the_angle<=the_angle) && (the_angle<=b.the_angle);
  else
    return (a.the_angle<=the_angle) || (the_angle<=b.the_angle);
}

void MyAngle::set_rad (double a) throw () {
  if (a<0) {
    double k = ceil (-a/(CONSTZWEIPI));
    the_angle = a+k*CONSTZWEIPI;
    return;
  }
  if (a>=CONSTZWEIPI) {
    double k = floor (a/(CONSTZWEIPI));
    the_angle = a-k*CONSTZWEIPI;
    return;
  }
  the_angle=a;
}

MyAngle MyAngle::rad_angle (double d) throw () {
  MyAngle res (d);
  return res;
}

MyAngle MyAngle::deg_angle (double d) throw () {
  MyAngle res;
  res.set_deg (d);
  return res;
}

void MyAngle::set_deg (double a) throw () {
  set_rad (a*CONSTPI180);
}

double MyAngle::get_rad () const throw () {
  return the_angle;
}

double MyAngle::get_deg () const throw () {
  return the_angle*CONST180PI;
}

double MyAngle::get_rad_pi () const throw () {
  return the_angle - (the_angle<M_PI ? 0.0: CONSTZWEIPI);
}

double MyAngle::get_deg_180 () const throw () {
  return get_deg() - (the_angle<M_PI ? 0.0: 360.0);
}

MyAngle MyAngle::operator+ (const MyAngle a) const throw () {
  MyAngle res (*this);
  res+=a;
  return res;
}

const MyAngle& MyAngle::operator+= (const MyAngle a) throw () {
  the_angle+=a.the_angle;
  if (the_angle>=CONSTZWEIPI)
    the_angle-=CONSTZWEIPI;
  return *this;
}

MyAngle MyAngle::operator- (const MyAngle a) const throw () {
  MyAngle res (*this);
  res-=a;
  return res;
}

const MyAngle& MyAngle::operator-= (const MyAngle a) throw () {
  the_angle-=a.the_angle;
  if (the_angle<0)
    the_angle+=CONSTZWEIPI;
  return *this;
}

MyAngle MyAngle::operator- () const throw () {
  return MyAngle (CONSTZWEIPI-the_angle);
}

const MyAngle& MyAngle::operator*= (double s) throw () {
  set_rad (s*get_rad_pi());
  return *this;
}

MyAngle operator* (const MyAngle a, double s) throw () {
  MyAngle res (a);
  res*=s;
  return res;
}

MyAngle  operator* (double s, const MyAngle a) throw () {
  MyAngle res (a);
  res*=s;
  return res;
}


