#include "StdAfx.h"
#include "geometry.h"
#include <cmath>

using namespace std;

namespace 
{
	inline double teilverhaeltnis (const Vec& v1, const Vec& v2, const Vec& p)
	{
		if (v1.x!=v2.x)
		  return (p.x-v1.x)/(v2.x-v1.x);
		else
		  return (p.y-v1.y)/(v2.y-v1.y);
	}

}

Line::Line () throw () : p1 (Vec::zero_vector), p2 (Vec::unit_vector_x) {;}

Line::Line (const Vec pp1, const Vec pp2) throw (std::invalid_argument) : p1(pp1), p2(pp2) 
{
  if (pp1==pp2)
    throw std::invalid_argument ("invalid line definition in Line::Line");
}

Line::Line (const Line& ln) throw () : p1(ln.p1), p2(ln.p2) {;}

const Line& Line::operator= (const Line& ln) throw () 
{
  p1=ln.p1;
  p2=ln.p2;
  return (*this);
}

double Line::distance (const Vec p) throw () 
{
  Vec d=p2-p1;
  Vec l=p1+((p*d-p1*d)/(d.squared_length()))*d;
  return (p-l).length();
}

int Line::side (const Vec p) throw () 
{
  Vec d = p2-p1;
  Vec d_ortho (d.y, -d.x);
  double sp = d_ortho*(p-p1);
  return (sp>0 ? 1 : (sp<0 ? -1 : 0 ));
}
 
Line& Line::s_rotate (const Angle& a) throw () 
{
  p1=p1.rotate(a); p2=p2.rotate(a);   
  return *this;
}

Line& Line::s_translate (const Vec& v) throw () 
{
  p1+=v; p2+=v;                       
  return *this;
}

LineSegment& LineSegment::s_rotate (const Angle& a) throw () 
{
  p1=p1.rotate(a); p2=p2.rotate(a);  
  return *this;
}

LineSegment& LineSegment::s_translate (const Vec& v) throw () 
{
  p1+=v; p2+=v;                      
  return *this;
}

Line Line::rotate (const Angle a) const throw () 
{
  Line dest (*this);
  dest.s_rotate (a);
  return dest;
}

Line Line::translate (const Vec v) const throw () 
{
  Line dest (*this);
  dest.s_translate (v);
  return dest;
}

Line Line::makeLineFromPositionAndAngle( Vec p, Angle h)
{
  return Line( p, p + Vec(1,h.get_rad(),POLAR) );
}

LineSegment LineSegment::rotate (const Angle a) const throw () 
{
  LineSegment dest (*this);
  dest.s_rotate (a);
  return dest;
}

LineSegment LineSegment::translate (const Vec v) const throw () 
{
  LineSegment dest (*this);
  dest.s_translate (v);
  return dest;
}


Circle::Circle () throw () : center (Vec::zero_vector), radius(1) {;}

Circle::Circle (const Vec c, double r) throw () : center(c), radius( fabs(r) ) {;}

Circle::Circle (const Vec p1, const Vec p2, const Vec p3) throw (std::invalid_argument) 
{
  Line ln1 (0.5*(p1+p2), 0.5*(p1+p2)+(p2-p1).rotate_quarter());
  Line ln2 (0.5*(p2+p3), 0.5*(p2+p3)+(p3-p2).rotate_quarter());
  center = intersect (ln1, ln2);   
  radius = (center-p1).length();
}

Circle::Circle (const Circle& cc) throw () : center(cc.center), radius(cc.radius) {;}

const Vec& Circle::get_center () const throw ()
 {
  return center;
}

double Circle::get_radius () const throw () 
{
  return radius;
}

double Circle::distance (const Vec p) const throw () 
{
  return (p-center).length()-radius;
}


const Circle& Circle::operator= (const Circle& cc) throw () 
{
  center=cc.center;
  radius=cc.radius;
  return (*this);
}

bool Circle::is_inside ( const Vec p) const throw () 
{
  Vec d = p-center;
  return (d.squared_length()<=radius*radius);
}

Vec intersect (const Line& ln1, const Line& ln2) throw (std::invalid_argument) 
{
  Vec pos;
  Vec d1=ln1.p2-ln1.p1;
  Vec d2=ln2.p2-ln2.p1;
  double det=d1.x*d2.y-d2.x*d1.y;
  if (det==0)
    return pos; // throw std::invalid_argument("parallel lines in intersect");
  Vec dp=ln2.p1-ln1.p1;
  double tau=(d2.y*dp.x-d2.x*dp.y)/det;
  return (1.0-tau)*ln1.p1+tau*ln1.p2;
}

std::vector<Vec> intersect (const Line& ln, const Circle& cc) throw (std::bad_alloc) 
{
  double d_len2 = (ln.p1-ln.p2).squared_length();
  double p1_len2 = ln.p1.squared_length();
  double c_len2 = cc.center.squared_length();
  double p1_p2 = ln.p1*ln.p2;
  double p1_c = ln.p1*cc.center;
  double p2_c = ln.p2*cc.center;

  double l_term = 2.0*(-p1_len2+p1_p2+p1_c-p2_c);
  double c_term = p1_len2+c_len2-2.0*p1_c-cc.radius*cc.radius;

  double rad = l_term*l_term-4.0*d_len2*c_term;
  if (rad<0)
  {
    std::vector<Vec> ret (0);
    return ret;
  } 
  else if (rad==0)
  {
    std::vector<Vec> ret (1);
    double tau = -l_term/(2.0*d_len2);
    ret[0]=(1.0-tau)*ln.p1+tau*ln.p2;
    return ret;
  } 
  else 
  {
    std::vector<Vec> ret (2);
    double root =  sqrt(rad);
    double tau = (-l_term+root)/(2.0*d_len2);
    ret[0]=(1.0-tau)*ln.p1+tau*ln.p2;
    tau = (-l_term-root)/(2.0*d_len2);
    ret[1]=(1.0-tau)*ln.p1+tau*ln.p2;
    return ret;
  }
}

std::vector<Vec> intersect (const Circle& cc1, const Circle& cc2) throw (std::bad_alloc) 
{
  Vec d = (cc2.center-cc1.center);
  double d_len2 = d.squared_length();
  double d_len =  sqrt (d_len2);
  if ((d_len>(cc1.radius+cc2.radius)) || (d_len<fabs(cc1.radius-cc2.radius))) 
  {
    std::vector<Vec> ret (0);
    return ret;
  } 
  else if (d_len==(cc1.radius+cc2.radius)) 
  {
    std::vector<Vec> ret (1);
    if ((cc1.radius==0)&&(cc2.radius==0))
      ret[0]=cc1.center;
    else
      ret[0]=cc1.center+(cc1.radius/(cc1.radius+cc2.radius))*d;
    return ret;
  } 
  else if (d_len==(cc1.radius-cc2.radius)) 
  {
    std::vector<Vec> ret (1);
    ret[0]=cc1.center+(cc1.radius/d_len)*d;
    return ret;
  } 
  else if (d_len==(cc2.radius-cc1.radius))
  {
    std::vector<Vec> ret (1);
    ret[0]=cc1.center-(cc1.radius/d_len)*d;
    return ret;
  }
  else
  {
    std::vector<Vec> ret (2);
    Vec d_norm = (1.0/d_len)*d;
    Vec d_ortho (-d_norm.y, d_norm.x);
    double tau = (cc1.radius*cc1.radius+d_len2-cc2.radius*cc2.radius)/(2.0*d_len);
    double rho =  sqrt(cc1.radius*cc1.radius-tau*tau);
    ret[0]=cc1.center+tau*d_norm+rho*d_ortho;
    ret[1]=cc1.center+tau*d_norm-rho*d_ortho;
    return ret;
  }
}

std::vector<Vec> tangent_point (const Circle& cc, const Vec& p) throw (std::bad_alloc, std::invalid_argument) 
{
  std::vector<Vec> ret(0);
  Vec d = (p-cc.center);
  double d_len = d.length();
  if (d_len<cc.radius)
    return ret;//throw std::invalid_argument ("no tangent possible in tangent_point");
  else if (d_len==cc.radius) 
  {
    std::vector<Vec> ret(1);
    ret[0]=p;
    return ret;
  }
  else 
  {
    std::vector<Vec> ret(2);
    Vec d_norm = (1.0/d_len)*d;
    Vec d_ortho (-d_norm.y,d_norm.x);
    double tau = (cc.radius*cc.radius)/d_len;
    double rho =  sqrt(cc.radius*cc.radius-tau*tau);
    ret[0]=cc.center+tau*d_norm+rho*d_ortho;
    ret[1]=cc.center+tau*d_norm-rho*d_ortho;
    return ret;
  }
}

Vec Line::perpendicular_point (const Vec& p) throw () 
{
  Vec d=p2-p1;
  return p1+((p*d-p1*d)/(d.squared_length()))*d;
}


LineSegment::LineSegment () throw () : p1 (Vec::zero_vector), p2 (Vec::unit_vector_x) {;}

LineSegment::LineSegment (const Vec start, const Vec ende) throw (std::invalid_argument) : p1 (start), p2 (ende) {;}

LineSegment::LineSegment (const LineSegment& ls) throw () : p1(ls.p1), p2(ls.p2) {;}

const LineSegment& LineSegment::operator= (const LineSegment& ls) throw () 
{
  p1=ls.p1;
  p2=ls.p2;
  return (*this);
}

double LineSegment::distance (const Vec p) throw () 
{
  Vec f = Line (p1, p2).perpendicular_point (p);
  double tv;
  if (p1.x!=p2.x)
    tv = (f.x-p1.x)/(p2.x-p1.x);
  else
    tv = (f.y-p1.y)/(p2.y-p1.y);
  if (tv>=1)
    return (p2-p).length();
  else if (tv<=0)
    return (p1-p).length();
  else
    return (f-p).length();
}

const Vec& LineSegment::getStart() const throw ()
{ return p1; }

const Vec& LineSegment::getEnd() const throw ()
{ return p2; }


MyArc::MyArc () throw () : center (Vec::zero_vector), radius (1), start(Angle::zero), end(Angle::half) {;}

MyArc::MyArc (Vec c, double r, Angle s, Angle e) throw () : center(c), radius(r), start(s), end(e) {;}

MyArc::MyArc (const MyArc& a) throw () : center(a.center), radius(a.radius), start(a.start), end(a.end) {;}

const MyArc& MyArc::operator= (const MyArc& a) throw () 
{
  center=a.center;
  radius=a.radius;
  start=a.start;
  end=a.end;
  return (*this);
}

double MyArc::distance (const Vec p) throw ()
{
  Vec v = p-center;
  if (v.angle().in_between (start, end))
    return abs(v.length()-radius);
  else 
  {
    double d1 = (Vec::unit_vector (start)-p).squared_length();
    double d2 = (Vec::unit_vector (end)-p).squared_length();
    if (d1<d2)
      return sqrt(d1);
    else
      return sqrt(d2);
  }
}

std::vector<Vec> intersect (const Line& l, const MyArc& a) throw (std::bad_alloc) 
{
  std::vector<Vec> res = intersect (l, Circle(a.center, a.radius) );
  unsigned int i=0;
  while (i<res.size())
    if (!(res[i]-a.center).angle().in_between (a.start, a.end))
      res.erase (res.begin()+i);
    else
      i++;
  return res;
}

std::vector<Vec> intersect (const MyArc& a, const Line& l) throw (std::bad_alloc)
{
  return intersect (l,a);
}

std::vector<Vec> intersect (const LineSegment& l, const MyArc& a) throw (std::bad_alloc) 
{
  std::vector<Vec> res = intersect (Line (l.p1, l.p2), a);
  unsigned int i=0;
  while (i<res.size()) 
  {
    double tv;
    if (l.p1.x!=l.p2.x)
      tv = (res[i].x-l.p1.x)/(l.p2.x-l.p1.x);
    else
      tv = (res[i].y-l.p1.y)/(l.p2.y-l.p1.y);
    if (tv>1 || tv<0)
      res.erase (res.begin()+i);
    else
      i++;
  }
  return res;
}

std::vector<Vec> intersect (const MyArc& a, const LineSegment& l) throw (std::bad_alloc) 
{
  return intersect (l,a);
}
    
std::vector<Vec> intersect (const LineSegment& l1, const Line& l2) throw (std::bad_alloc)
 {
  Vec is;
  try
  {
    is = intersect (Line (l1.p1, l1.p2), l2);
  }
  catch(std::invalid_argument)
  {
    std::vector<Vec> leer (0);
    return leer;
  }
  double tv = teilverhaeltnis (l1.p1, l1.p2, is);
  if (tv<0 || tv>1) 
  {
    std::vector<Vec> leer (0);
    return leer;
  }
  std::vector<Vec> nleer (1);
  nleer[0]=is;
  return nleer;
}

std::vector<Vec> intersect (const Line& l1, const LineSegment& l2) throw (std::bad_alloc) 
{
  return intersect (l2,l1);
}

std::vector<Vec> intersect (const LineSegment& l1, const LineSegment& l2) throw (std::bad_alloc) 
{
  Vec is;
  try
  {
    is = intersect (Line (l1.p1,l1.p2), Line(l2.p1,l2.p2));
  }catch(std::invalid_argument)
  {
    std::vector<Vec> leer (0);
    return leer;
  }
  double tv1 = teilverhaeltnis (l1.p1, l1.p2, is);
  double tv2 = teilverhaeltnis (l2.p1, l2.p2, is);
  if (tv1<0 || tv1>1 || tv2<0 || tv2>1) 
  {
    std::vector<Vec> leer (0);
    return leer;
  }
  std::vector<Vec> nleer (1);
  nleer[0]=is;
  return nleer;
}


XYRectangle::XYRectangle () throw () 
{
	size_x = 0;
    size_y = 0;
}

XYRectangle::XYRectangle (Vec c1,Vec c2) throw () 
{ 
  center.x = 0.5 * c1.x + 0.5* c2.x;
  center.y = 0.5 * c1.y + 0.5* c2.y;
  if ( c2.x > c1.x )
	  size_x= c2.x - c1.x;
  else
	  size_x= c1.x - c2.x;
  
  if ( c2.y > c1.y )
	  size_y= c2.y - c1.y;
  else
	  size_y= c1.y - c2.y;

  Vec center = 0.5*(c1+c2);
  double w = 0.5*fabs(c1.x-c2.x);
  double h = 0.5*fabs(c1.y-c2.y);
  p1 = Vec (center.x-w,center.y-h);
  p2 = Vec (center.x+w,center.y+h);
}

XYRectangle::XYRectangle (const XYRectangle& r) throw () : p1 (r.p1), p2 (r.p2),
    center(r.center),size_x(r.size_x),size_y(r.size_y) {;}

bool XYRectangle::is_inside (const Vec z) const throw () 
{
  //return (p1.x<=z.x && p1.y<=z.y && z.x<=p2.x && z.y<=p2.y);
  double max_x= center.x + 0.5*size_x;
  double min_x= center.x - 0.5*size_x;
  double max_y= center.y + 0.5*size_y;
  double min_y= center.y - 0.5*size_y;
  return ( z.x <= max_x && z.x >= min_x && z.y <= max_y && z.y >= min_y );
}


Triangle::Triangle () throw () {;}

Triangle::Triangle (Vec c1,Vec c2,Vec c3) throw () : p1(c1), p2(c2), p3(c3) {;}

Triangle::Triangle (const Triangle& t) throw () : p1(t.p1), p2(t.p2), p3(t.p3) {;}

bool Triangle::is_inside (Vec p) const throw () 
{
 
  double A= p2.x - p1.x;
  double B= p3.x - p1.x;
  double C= p2.y - p1.y;
  double D= p3.y - p1.y;

  double det= A * D - C * B;
  if ( fabs(det) < 0.000001 ) 
  { 
    return false;
  }

  double x= p.x - p1.x;
  double y= p.y - p1.y;

  double a= D * x - B * y;
  double b= -C * x + A * y;

  a/= det;
  b/= det;

  if (a < 0 || b < 0) 
  {
    return false;
  }
  if ( a + b > 1.0) 
  {
    return false;
  }

  return true;
}


Quadrangle::Quadrangle () throw () {;}

Quadrangle::Quadrangle (const Quadrangle& q) throw () : p1(q.p1), p2(q.p2), p3(q.p3), p4(q.p4) {;}

Quadrangle::Quadrangle (const Vec & q1, const Vec & q2, double width) throw ()
{
  Vec tmp= q2-q1;
  Vec norm;
  norm.x= -tmp.y;
  norm.y= tmp.x;
  
  double n= (0.5*width)/(sqrt(norm.squared_length()));
  norm.x*=n;
  norm.y*=n;

  p1= q1+ norm;
  p2= q1- norm;
  p3= q2- norm;
  p4= q2+ norm;
}

Quadrangle::Quadrangle (const Vec & q1, const Vec & q2, double width, double width2) throw () 
{
  Vec tmp= q2-q1;
  Vec norm;
  Vec norm2;
  norm.x= -tmp.y;
  norm.y= tmp.x;
   
  double n= (0.5)/(sqrt(norm.squared_length()));
  norm2.x= width2*norm.x*n;
  norm2.y= width2*norm.y*n;
  
  norm.x*=width*n;
  norm.y*=width*n;

  p1= q1+ norm;
  p2= q1- norm;
  p3= q2- norm2;
  p4= q2+ norm2;
}

Quadrangle::Quadrangle (Vec c1, Vec c2) throw () 
{
  p1=c1;
  p2=Vec(c2.x, c1.y);
  p3=c2;
  p4=Vec(c1.x, c2.y);
}

Quadrangle::Quadrangle (Vec c1, Vec c2, Vec c3, Vec c4) throw () : p1(c1), p2(c2), p3(c3), p4(c4) {;}

bool Quadrangle::is_inside (const Vec p) const throw () 
{
  return (Triangle(p1,p2,p3).is_inside( p ) || Triangle(p1,p3,p4).is_inside( p ) );
}
 
Halfplane::Halfplane () throw () {;}

Halfplane::Halfplane (const Halfplane& h) throw () : p1(h.p1), norm(h.norm) {;}

Halfplane::Halfplane (Vec c, Vec n) throw () : p1(c), norm(n) {;}

Halfplane::Halfplane (Vec c, Angle a) throw () : p1(c), norm(Vec::unit_vector (a+Angle::quarter)) {;}//

bool Halfplane::is_inside (Vec p) const throw () 
{
  return ((p-p1)*norm>=0);
}
 
Line operator* (const Frame2d& f, const Line& l) throw () 
{
  return Line (f*l.p1, f*l.p2);
}

LineSegment operator* (const Frame2d& f, const LineSegment& l) throw () 
{
  return LineSegment (f*l.p1, f*l.p2);
}

MyArc operator* (const Frame2d& f, const MyArc& a) throw () 
{
  return MyArc (f*a.center, a.radius, a.start+f.get_angle(), a.end+f.get_angle());
}

Circle operator* (const Frame2d& f, const Circle& c) throw () 
{
  return Circle (f*c.center, c.radius);
}

Triangle operator* (const Frame2d& f , const Triangle& t) throw () 
{
  return Triangle (f*t.p1, f*t.p2, f*t.p3);
}

Quadrangle operator* (const Frame2d& f, const Quadrangle& q) throw () 
{
  return Quadrangle (f*q.p1, f*q.p2, f*q.p3, f*q.p4);
}

Halfplane operator* (const Frame2d& f, const Halfplane& h) throw ()
{
  return Halfplane (f*h.p1, h.norm.rotate (f.get_angle()));
}

/******************************************************************************
  Cone2d
*******************************************************************************/

Cone2d::Cone2d(Vec const & p, Angle const & ang1, Angle const & ang2) {
  pos= p;
  dir1 = Vec::getVecPositionFromPolar(1.0,ang1);
  dir2 = Vec::getVecPositionFromPolar(1.0,ang2);
}

Cone2d::Cone2d(Vec const & p, Vec const & dir, Angle const & ang) {
  double a= 0.5*ang.get_rad(); //here a half of the angle is taken
  double c= cos( a );
  double s= sin( a );

  pos= p;
  /* Rotation Matrix (counterclockwise)    Rotation Matrix (clockwise) 
     [ cos   -sin]                         [  cos    sin]                         
     [ sin    cos]                         [ -sin    cos]                         
  */                                                                           

  dir1= Vec( c*dir.x + s*dir.y, c*dir.y - s*dir.x); //rotate counterclockwise
  dir2= Vec( c*dir.x - s*dir.y, s*dir.x + c*dir.y); //rotate clockwise  
}


bool Cone2d::is_inside(Vec point) const throw()
{
  Vec tmp1,tmp2;
  tmp1.x= -dir1.y;
  tmp1.y= dir1.x;

  tmp2.x= dir2.y;
  tmp2.y= -dir2.x;

  if ( Halfplane(pos,tmp1).is_inside(point) &&
       Halfplane(pos,tmp2).is_inside(point) )
    return true;
  return false;
}