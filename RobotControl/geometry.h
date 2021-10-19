#ifndef _geometry_h_
#define _geometry_h_

#include "Vec.h"
#include <stdexcept>
#include <vector>
#include "Frame2D.h"

class Area;
class Line;
class LineSegment;
class Circle;
class MyArc;
class Triangle;
class XYRectangle;
class Quadrangle;
class Halfplane;

Line operator* (const Frame2d&, const Line&) throw ();
LineSegment operator* (const Frame2d&, const LineSegment&) throw ();
MyArc operator* (const Frame2d&, const MyArc&) throw ();
Circle operator* (const Frame2d&, const Circle&) throw ();
Triangle operator* (const Frame2d&, const Triangle&) throw ();
Quadrangle operator* (const Frame2d&, const Quadrangle&) throw ();
Halfplane operator* (const Frame2d&, const Halfplane&) throw ();

Vec intersect (const Line&, const Line&) throw (std::invalid_argument);
std::vector<Vec> intersect (const LineSegment&, const Line&) throw (std::bad_alloc);
std::vector<Vec> intersect (const Line&, const LineSegment&) throw (std::bad_alloc);
std::vector<Vec> intersect (const LineSegment&, const LineSegment&) throw (std::bad_alloc);
std::vector<Vec> intersect (const Line&, const Circle&) throw (std::bad_alloc);
inline std::vector<Vec> intersect (const Circle& c, const Line& l) throw (std::bad_alloc) { return intersect (l,c); }
std::vector<Vec> intersect (const Circle&, const Circle&) throw (std::bad_alloc);
std::vector<Vec> intersect (const Line&, const MyArc&) throw (std::bad_alloc);
std::vector<Vec> intersect (const MyArc&, const Line&) throw (std::bad_alloc);
std::vector<Vec> intersect (const LineSegment&, const MyArc&) throw (std::bad_alloc);
std::vector<Vec> intersect (const MyArc&, const LineSegment&) throw (std::bad_alloc);
std::vector<Vec> tangent_point (const Circle&, const Vec&) throw (std::bad_alloc, std::invalid_argument);

class Area
{
public:
	virtual ~Area() throw () {;}
    virtual bool is_inside ( Vec) const throw () =0;
 
};

class Line
{
public:
	Vec p1;   
    Vec p2;
    Line () throw ();
    Line (const Vec, const Vec) throw (std::invalid_argument);
    Line (const Line&) throw ();
    const Line& operator= (const Line&) throw ();
    double distance (const Vec) throw ();
    int side (const Vec) throw ();
    Vec perpendicular_point (const Vec&) throw ();
    Line& s_rotate   (const Angle&) throw();
    Line& s_translate(const Vec&) throw();
    Line rotate (const Angle) const throw ();
    Line translate (const Vec) const throw ();
	static Line makeLineFromPositionAndAngle(  Vec p,  Angle h );
protected:
	friend Vec intersect (const Line&, const Line&) throw (std::invalid_argument);
    friend std::vector<Vec> intersect (const Line&, const Circle&) throw (std::bad_alloc);
    friend std::vector<Vec> intersect (const Line&, const MyArc&) throw (std::bad_alloc);
    friend Vec perpendicular_point (const Vec&, const Line&) throw ();
    friend Line operator* (const Frame2d&, const Line&) throw ();
 	
};

class LineSegment
{
	friend std::vector<Vec> intersect (const LineSegment&, const MyArc&) throw (std::bad_alloc);
    friend std::vector<Vec> intersect (const LineSegment&, const Line&) throw (std::bad_alloc);
    friend std::vector<Vec> intersect (const LineSegment&, const LineSegment&) throw (std::bad_alloc);
    friend LineSegment operator* (const Frame2d&, const LineSegment&) throw ();
public:
	Vec p1;    
    Vec p2;   

    LineSegment () throw ();
    LineSegment (const Vec, const Vec) throw (std::invalid_argument);
    LineSegment (const LineSegment&) throw ();
    const LineSegment& operator= (const LineSegment&) throw ();
    double distance (const Vec) throw ();
    const Vec& getStart() const throw();
    const Vec& getEnd() const throw();
    LineSegment& s_rotate   (const Angle&) throw();
    LineSegment& s_translate(const Vec&) throw();
    LineSegment rotate (const Angle) const throw ();
    LineSegment translate (const Vec) const throw ();
};

class MyArc
{
public:
	MyArc () throw ();
	MyArc (Vec, double, Angle, Angle) throw ();
	MyArc (const MyArc&) throw ();
	const MyArc& operator= (const MyArc&) throw ();

	double distance (const Vec) throw ();
 
private:
	friend std::vector<Vec> intersect (const Line&, const MyArc&) throw (std::bad_alloc);
    friend std::vector<Vec> intersect (const LineSegment&, const MyArc&) throw (std::bad_alloc);
    friend MyArc operator* (const Frame2d&, const MyArc&) throw ();

    Vec center;
    double radius;
    Angle start;
    Angle end;
};

class Circle : public Area
{
public:
	Vec    center;     
	double radius;   
	Circle () throw ();
	Circle (const Vec, double) throw ();
	Circle (const Vec, const Vec, const Vec) throw (std::invalid_argument);
	Circle (const Circle&) throw ();
	const Circle& operator= (const Circle&) throw ();

	const Vec& get_center () const throw ();
	double get_radius () const throw ();
	bool is_inside ( Vec) const throw ();  // const edit by lrd
	double distance (const Vec) const throw ();
	
private:
	friend std::vector<Vec> intersect (const Line&, const Circle&) throw (std::bad_alloc);
    friend std::vector<Vec> intersect (const Circle&, const Circle&) throw (std::bad_alloc);
    friend std::vector<Vec> tangent_point (const Circle&, const Vec&) throw (std::bad_alloc, std::invalid_argument);
    friend Circle operator* (const Frame2d&, const Circle&) throw ();
};

class Triangle : public Area
{
public:
	Triangle () throw ();
    Triangle (Vec,Vec,Vec) throw ();
    Triangle (const Triangle&) throw ();
    bool is_inside (Vec) const throw ();
    Vec p1, p2, p3;
    
    friend Triangle operator* (const Frame2d&, const Triangle&) throw ();
 
};

class XYRectangle : public Area
{
public:
	XYRectangle () throw ();
    XYRectangle (const XYRectangle&) throw ();
    XYRectangle (Vec,Vec) throw ();
	XYRectangle(Vec const& c, double s_x, double s_y) { center = c; size_x = s_x; size_y = s_y; }
    bool is_inside( Vec) const throw ();
    Vec p1;
    Vec p2;
    Vec center;
	double size_x;
	double size_y;
};

class Quadrangle : public Area
{
public: 
	Quadrangle () throw ();
    Quadrangle (const Quadrangle&) throw ();
    
    Quadrangle (Vec, Vec) throw ();
   
    Quadrangle (const Vec&, const Vec&, double) throw ();    
    
    Quadrangle (const Vec&, const Vec&, double,double) throw ();    
    
    Quadrangle (Vec, Vec, Vec, Vec) throw ();
    bool is_inside (Vec) const throw ();
    Vec p1, p2, p3, p4;

    friend Quadrangle operator* (const Frame2d&, const Quadrangle&) throw ();
};

class Halfplane : public Area
{
public:
	Halfplane () throw ();
    Halfplane (const Halfplane&) throw ();
     
    Halfplane (Vec, Vec) throw ();
     
    Halfplane (Vec, Angle) throw ();
    bool is_inside (Vec) const throw ();
private:
	Vec p1;
    Vec norm;

    friend Halfplane operator* (const Frame2d&, const Halfplane&) throw ();
};

class Cone2d : public Area 
{
public:
	Cone2d():dir1(1,0),dir2(1,0) {;}
	
	Cone2d(Vec const & p, Vec const & d1, Vec const & d2) { pos= p; dir1= d1; dir2= d2; }
	Cone2d(Vec const & p, Angle const & ang1, Angle const & ang2);
	
	
	Cone2d(Vec const & p, Vec const & dir, Angle const & ang);
    bool is_inside (Vec) const throw ();
private:
	Vec pos;
	Vec dir1;
	Vec dir2;
};

#endif