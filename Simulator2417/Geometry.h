#ifndef _Geometry_
#define _Geometry_
 
#include "math.h"
#include <vector>
#include "MyAngle.h"
using namespace std;

typedef double AngRad;   
typedef double AngDeg;
typedef double Angle;

#define PI 3.14159265358979323846
#define EPSILON      0.0001    
#define FLOAT_ERROR  0.001
#define EPS          1.0e-10
#define INF          1.0e10

#define CONSTWURZELDREIHALBE 0.86602540378443859659
#define CONSTWURZELZWEIHALBE 0.70710678118654757274

#define QUANTIZESTEP      0.20
#define QUANTIZESTEP_LINE 0.11

#define QUANTIZESTEP_vel 0.001


inline double
Quantize( const double &d,
		  const double &q )
{		  
   return  ( floor( (d)/(q) + 0.5 )  * (q) );
}

/*****************************************************************************/
/********************   RANDOM FUNCTION   **********************************/
/*****************************************************************************/
inline void   random_seed( const unsigned int& s) {  srand(s); }
inline double urandom()                           { return static_cast<double>( rand())/RAND_MAX; } // 0-1
inline double urandom (const double& f, const double& t) { return (t-f)*urandom()+f; } // f-t
inline bool   brandom (const double& p)                  { return (urandom()<p); }
inline double nrandom()  // 标准正态分布u＝0，sigma＝1
{
    double u1 = urandom();
    double u2 = urandom();
	if( u1 == 0 ) { u1 = 1e-50; }
    return (sqrt(-2.0*log(u1))*cos(6.2831853071*u2));
}
inline double nrandom (const double& mu, const double& sigma) { return nrandom()*sigma+mu; }

// auxiliary numeric functions for determining the
// maximum and minimum of two given double values and the sign of a value
int    sign    ( double d1            );
double diff_angle(double ang1, double ang2);
bool   equal_float(double a,double b);
double Pow(double x);
double Max(double d1,double d2);
double Min(double d1,double d2);

AngDeg Rad2Deg ( AngRad x             );
AngRad Deg2Rad ( AngDeg x             ); 
double cosDeg  ( AngDeg x             );
double sinDeg  ( AngDeg x             );
double tanDeg  ( AngDeg x             );
AngDeg atanDeg ( double x             );
double atan2Deg( double x,  double y  );
AngDeg acosDeg ( double x             );
AngDeg asinDeg ( double x             );
double GetLinearFuzzyValue(double value, double left, double right, int up=0 );
double getDiffAngle(double ang1, double ang2);
  
// various goniometric functions
bool   isAngInInterval     ( AngDeg ang,    AngDeg angMin,    AngDeg angMax );
AngDeg getBisectorTwoAngles( AngDeg angMin, AngDeg angMax );
double CalcQuanDist(const double &dist, const double &qstep);

enum CoordSystemT 
{
  CARTESIAN,
  POLAR
};

class Vector
{
public:
	double X;
	double Y;
public:
	Vector(double x1 = 0,double y1=0,CoordSystemT cs=CARTESIAN);
	Vector        operator -             (                                );
	Vector        operator +             ( const double      &d           );
	Vector        operator +             ( const Vector &p           );
	Vector        operator -             ( const double      &d           );
	Vector        operator -             ( const Vector &p           );
	Vector        operator *             ( const double      &d           );
	Vector        operator *             ( const Vector &p           );
  	Vector        operator /             ( const double      &d           );
	Vector        operator /             ( const Vector &p           );
	void               operator =             ( const double      &d           );
	void               operator +=            ( const Vector &p           );
	void               operator +=            ( const double      &d           );
	void               operator -=            ( const Vector &p           );
	void               operator -=            ( const double      &d           );
	void               operator *=            ( const Vector &p           );
	void               operator *=            ( const double      &d           );
	void               operator /=            ( const Vector &p           );
	void               operator /=            ( const double      &d           );
	bool               operator !=            ( const Vector &p           );
	bool               operator !=            ( const double      &d           );
	bool               operator ==            ( const Vector &p           );
	bool               operator ==            ( const double      &d           );
	double getY( ) const;
	bool   setY( double dY );
	double getX( ) const;
	bool   setX( double dX );
	bool   isBetweenY( const double &d1, const double &d2 );
	bool   isBetweenY( const Vector &p1, const Vector &p2 );
	bool isBetweenX( const double &d1, const double &d2 );
	bool isBetweenX( const Vector &p1, const Vector &p2 );
	bool isRightOf( const double &d );
	bool isRightOf( const Vector &p );
	bool isLeftOf( const double &d );
	bool isLeftOf( const Vector &p );
	bool isBehindOf( const double &d );
	bool isBehindOf( const Vector &p );
	bool isInFrontOf( const double &d );
	bool isInFrontOf( const Vector &p );
	Vector relativeToGlobal( Vector origin, AngDeg ang );
	Vector normalize( );
	Vector setMagnitude( double d );
	Vector globalToRelative( Vector origin, AngDeg ang );
	Vector rotate( AngDeg angle );
	Vector rotate( const MyAngle angle) const throw();
	bool   setVecPosition( double dX, double dY, CoordSystemT cs=CARTESIAN);
	Vector getVecPositionOnLineFraction( Vector &p,double  dFrac );
	double getDistanceTo( const Vector p );
	double getMagnitude() const;
	double getSqrMagnitude() const;

	double getDirection();

	static AngDeg normalizeAngle( AngDeg angle );
    static Vector getVecPositionFromPolar( double dMag, AngDeg ang );

    Vector rotate_twelvth () const  ;          ///< Rotation um 30 Grad
    Vector rotate_eleven_twelvth () const  ;   ///< Rotation um -30=330 Grad
    Vector rotate_eighth () const  ;           ///< Rotation um 45 Grad
    Vector rotate_seven_eighth () const ;     ///< Rotation um -45=315 Grad
    Vector rotate_sixth () const  ;            ///< Rotation um 60 Grad
    Vector rotate_five_sixth ()  const;       ///< Rotation um -60=300 Grad
    Vector rotate_quarter () const  ;          ///< Rotation um 90 Grad
    Vector rotate_three_quarters () const  ;   ///< Rotation um -90 Grad
    Vector rotate_half () const  ;             ///< Rotation um 180 Grad=Punktspiegelung am Ursprung

    Vector s_rotate (const Angle)  ;           ///< Rotation von this
    
    Vector mirror (const Vector) const ;         ///< Spiegelung an einer Achse mit gegebener Richtung wenn ||Arg1|| > 0, ansonsten Punktspiegelung 
    Vector mirror_x () const  ;                ///< Spiegelung an x-Achse
    Vector mirror_y () const  ;                ///< Spiegelung an y-Achse
    Vector mirror_eighth () const  ;           ///< Spiegelung an 1. Winkelhalbierender
    Vector mirror_three_eighth () const  ;
	Angle  angle( const Vector ) const throw();
	MyAngle angle() const throw();

	static Vector unit_vector(MyAngle) throw();
};

inline Vector n2random() 
{
    double u1 = urandom();
	if( u1 == 0 ) { u1 = 1e-50; }
    double u2 = urandom();
    double s =  sqrt(-2.0*log(u1));
    return Vector(s*cos(6.2831853071*u2), s*sin(6.2831853071*u2));
}

class MyLine;
class Circle;
class MyArc;
class LineSegment;
class Frame2d;
/*****************************************************************************/
/*********************   CLASS GEOMETRY   ************************************/
/*****************************************************************************/

/*! This class contains several static methods dealing with geometry.*/
class Geometry
{
 public:
   // geometric series
  static double getLengthGeomSeries(double dFirst,double dRatio,double dSum  );
  static double getSumGeomSeries   (double dFirst,double dRatio,double dLen  );
  static double getSumInfGeomSeries(double dFirst,double dRatio              );
  static double getFirstGeomSeries (double dSum,  double dRatio,double dLen  );
  static double getFirstInfGeomSeries(double dSum,double dRatio              );

  // abc formula
  static int    abcFormula(double a,double b, double c, double *s1,double *s2);
};

class MyLine  
{
private:
	double A;
	double B;
	double C;
public:
	Vector p1;
	Vector p2;
public:
	static MyLine makeLineFromTwoPoints( Vector pos1, Vector pos2 );
	static MyLine makeLineFromPositionAndAngle( Vector vec,AngDeg angle);
	double        getDistanceWithPoint( Vector pos );
	Vector        getPointOnLineClosestTo( Vector pos );
	MyLine        getTangentLine( Vector pos );
	bool          isInBetween( Vector pos, Vector point1,Vector point2);
	double        getXGivenY( double y );
	double        getYGivenX( double x );
	double        getCCoefficient() const;
	double        getBCoefficient() const;
	double        getACoefficient() const;
	Vector        getIntersection( MyLine line );
 	MyLine(double a,double b,double c){A=a;B=b;C=c;};
	MyLine();
	MyLine(Vector pp1,Vector pp2);
	MyLine(const MyLine& ln);
	const MyLine& operator = (const MyLine&);
	int           side( Vector p);
    Vector        perpendicular_point (  Vector& p);
 
	MyLine& s_rotate   (const Angle&)  ;
    /** Translation */
    MyLine& s_translate(const Vector&) throw();
    MyLine rotate (const Angle) const  ;
    MyLine translate (const Vector) const  ;
	double distance(  Vector);
protected:
	friend Vector intersect(  MyLine&,  MyLine&) throw(invalid_argument);
    friend vector<Vector> intersect(  MyLine&,  Circle&) throw(bad_alloc);
    friend vector<Vector> intersect( MyLine&  ,  MyArc&    ) throw(bad_alloc);
    friend MyLine operator* (const Frame2d&, const MyLine&) throw ();

};

class LineSegment
{	
	friend vector<Vector> intersect(  LineSegment&,  MyArc&)  throw(bad_alloc);
 	friend vector<Vector> intersect(  LineSegment&,  MyLine&) throw(bad_alloc);
    friend vector<Vector> intersect(  LineSegment&,  LineSegment&) throw(bad_alloc);
    friend LineSegment operator* (const Frame2d&, const LineSegment&) throw ();

public:
	Vector p1;
	Vector p2;
	LineSegment();
	LineSegment(const Vector, const Vector);
    LineSegment(const LineSegment&);
    const LineSegment& operator= (const LineSegment&);
     double distance ( Vector);

     const Vector& getStart() const;
    
     const Vector& getEnd() const;

     LineSegment& s_rotate   (const MyAngle&);
     LineSegment& s_translate(const Vector&);
     LineSegment rotate (const MyAngle) const;
     LineSegment translate (const Vector) const;
protected:
private:
};

class Circle
{
    Vector      m_posCenter;            /*!< Center of the circle  */
    double      m_dRadius;              /*!< Radius of the circle  */
 public:
	double distance(  Vector p) const;
    Circle( );
    Circle( Vector pos, double dR );

	Circle(  Vector,  Vector,   Vector) throw(invalid_argument);
	Circle(  const Circle&) throw();
	const Circle& operator= (  Circle& ) throw();
     // get and set methods
    bool        setCircle             ( Vector pos,
                                        double      dR  );
    bool        setRadius             ( double dR       );
    double      getRadius             (                 );
    bool        setCenter             ( Vector pos );
    Vector      getCenter             (                 );
    double      getCircumference      (                 );
    double      getArea               (                 );

    // calculate intersection points and area with other circle
    bool        isInside              ( Vector pos );
    int         getIntersectionPoints ( Circle      c,
                                        Vector *p1,
                                        Vector *p2 );
    double      getIntersectionArea   ( Circle c        );
 private:
	friend vector<Vector> intersect(  MyLine&,  Circle&) throw(bad_alloc);
    friend vector<Vector> intersect(  Circle&,  Circle&) throw(bad_alloc);
	friend vector<Vector> tangent_point(  Circle&,  Vector&) throw(bad_alloc,invalid_argument);   
    friend Circle operator* (const Frame2d&, const Circle&) throw ();

};

class MyArc
{
public:
	MyArc();
	MyArc(Vector,double,MyAngle,MyAngle);
 	MyArc(const MyArc&);
	const MyArc& operator= (const MyArc&);
	double distance(Vector);
	Vector getCenter() {return center;	}
	double getRadius() {return radius;	}
	MyAngle getStart() {return start;	}
	MyAngle getEnd()   {	return end;}
 
protected:
private:
	friend vector<Vector> intersect( MyLine&  ,  MyArc&    ) throw(bad_alloc);
    friend vector<Vector> intersect( LineSegment&,  MyArc& )  throw(bad_alloc);
    friend MyArc operator* (const Frame2d&, const MyArc&) throw ();

	Vector center;
	double radius;
	MyAngle  start;
	MyAngle  end;

};

class Rect
{
  Vector m_posLeftTop;     /*!< top left position of the rectangle      */
  Vector m_posRightBottom; /*!< bottom right position of the rectangle  */
public:
  Rect                          ( Vector pos, Vector pos2 );
 
  bool        isInside          ( Vector pos                   );

  // standard get and set methosd
  void        setRectanglePoints( Vector pos1,
                                  Vector pos2                  );
  bool        setPosLeftTop     ( Vector pos                   );
  Vector      getPosLeftTop     (                                   );
  bool        setPosRightBottom ( Vector pos                   );
  Vector      getPosRightBottom (                                   );
};
 

 
class Frame2d
{
  friend Vector operator*(const Frame2d &, const Vector &);
 public:
  Frame2d(); //init as identity frame
  Frame2d(Vector, Angle);
  static Frame2d Translation(double, double);
  static Frame2d Rotation(const Angle&);
  Angle get_angle() const;
  Vector get_heading() const;

  Vector get_x_axis() const { return Vector(n_x,n_y); }
  Vector get_y_axis() const { return Vector(-n_y,n_x); }
  Vector get_pos() const { return Vector(p_x,p_y); }
  void set_angle(const Angle&);
  void set_angle(Vector);
  void set_position(double,double);
  void set_position(Vector);
  void set_scale(double,double);
  void set_scale(double);

  double get_scale() const { return scale; }

  void invert();
  double n_x, n_y, p_x, p_y;
protected:
private:
	double scale;
}; 
 
Vector intersect(  MyLine&,  MyLine&) throw(invalid_argument);
vector<Vector> intersect(  LineSegment&,  MyLine&) throw(bad_alloc);
vector<Vector> intersect(  MyLine&,  LineSegment&) throw(bad_alloc);
vector<Vector> intersect(  LineSegment&,  LineSegment&) throw(bad_alloc);
vector<Vector> intersect(  MyLine&,  Circle&) throw(bad_alloc);
inline vector<Vector> intersect(  Circle& c,  MyLine& l) { return intersect(l,c);} 
vector<Vector> intersect(  Circle&,  Circle&) throw(bad_alloc);

vector<Vector> intersect( MyLine&  ,  MyArc&    ) throw(bad_alloc);
vector<Vector> intersect( MyArc&  ,  MyLine& )    throw(bad_alloc);
vector<Vector> intersect( LineSegment&,  MyArc&)  throw(bad_alloc);
vector<Vector> intersect(  MyArc&,  LineSegment&) throw(bad_alloc);
vector<Vector> tangent_point(  Circle&,  Vector&) throw(bad_alloc,invalid_argument);
   
MyLine operator* (const Frame2d&, const MyLine&) throw ();
LineSegment operator* (const Frame2d&, const LineSegment&) throw ();
MyArc operator* (const Frame2d&, const MyArc&) throw ();
Circle operator* (const Frame2d&, const Circle&) throw ();
 
#endif