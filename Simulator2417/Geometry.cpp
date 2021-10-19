#include "stdafx.h"
#include "Geometry.h"

inline double teilverhaeltnis (const Vector& v1, const Vector& v2, const Vector& p) 
{
    if (v1.X!=v2.X)
      return (p.X-v1.X)/(v2.X-v1.X);
    else
      return (p.Y-v1.Y)/(v2.Y-v1.Y);
} 

double diff_angle(double ang1, double ang2)
{
 return ( fabs( Vector::normalizeAngle( ang1 - ang2 ) ) );
}

bool equal_float(double a,double b)
{
  return ( fabs ( a - b ) < FLOAT_ERROR );//0.001 EPSILON
}

double Pow( double x )
{
  return ( (x) * (x) );
}

double Max( double d1, double d2 )
{
  return (d1>d2)?d1:d2;
}

double Min( double d1, double d2 )
{
  return (d1<d2)?d1:d2;
}

/*! This function returns the sign of a give double.
    1 is positive, -1 is negative
    \param d1 first parameter
    \return the sign of this double */
int sign( double d1 )
{
  return (d1>0)?1:-1;
}
 
AngDeg Rad2Deg( AngRad x )
{
  return ( x * 180 / PI );
}
 
AngRad  Deg2Rad( AngDeg x )
{
  return ( x * PI / 180 );
}
 
double  cosDeg( AngDeg x )
{
  return ( cos( Deg2Rad( x ) ) );
}
 
double  sinDeg( AngDeg x )
{
  return ( sin( Deg2Rad( x ) ) );
}
 
double  tanDeg( AngDeg x )
{
  return ( tan( Deg2Rad( x ) ) );
}
 
AngDeg  atanDeg( double x )
{
  return ( Rad2Deg( atan( x ) ) );
}
 
double  atan2Deg( double x, double y )
{
  if( fabs( x ) < EPSILON && fabs( y ) < EPSILON )
    return ( 0.0 );

  return ( Rad2Deg( atan2( x, y ) ) );
}
 
AngDeg  acosDeg( double x )
{
  if( x >= 1 )
    return ( 0.0 );
  else if( x <= -1 )
    return ( 180.0 );

  return ( Rad2Deg( acos( x ) ) );
}
  
AngDeg  asinDeg( double x )
{
  if( x >= 1 )
    return ( 90.0 );
  else if ( x <= -1 )
    return ( -90.0 );

  return ( Rad2Deg( asin( x ) ) );
}

double getDiffAngle(double ang1, double ang2)
{
	return ( fabs( Vector::normalizeAngle( ang1 - ang2 ) ) );
}
bool isAngInInterval( AngDeg ang, AngDeg angMin, AngDeg angMax )
{
  // convert all angles to interval 0..360
  if( ( ang    + 360 ) < 360 ) ang    += 360;
  if( ( angMin + 360 ) < 360 ) angMin += 360;
  if( ( angMax + 360 ) < 360 ) angMax += 360;

  if( angMin < angMax ) // 0 ---false-- angMin ---true-----angMax---false--360
    return angMin < ang && ang < angMax ;
  else                  // 0 ---true--- angMax ---false----angMin---true---360
    return !( angMax < ang && ang < angMin );
}

/*! This method returns the bisector (average) of two angles. It deals
    with the boundary problem, thus when 'angMin' equals 170 and 'angMax'
    equals -100, -145 is returned.
    \param angMin minimum angle [-180,180]
    \param angMax maximum angle [-180,180]
    \return average of angMin and angMax. */
AngDeg getBisectorTwoAngles( AngDeg angMin, AngDeg angMax )
{
  // separate sine and cosine part to circumvent boundary problem
  return Vector::normalizeAngle(
            atan2Deg( (sinDeg( angMin) + sinDeg( angMax ) )/2.0,
                      (cosDeg( angMin) + cosDeg( angMax ) )/2.0 ) );
}
   
double GetLinearFuzzyValue(double value, double left, double right, int up )
{
 	if( up )
	{
		if (value < left)
			return 0.0;
		else if (value > right)
			return 1.0;
		else
			return (value - left) / (right - left);
	}
	else
	{
		if (value < left)
			return 1.0;
		else if (value > right)
			return 0.0;
		else
			return (right - value) / (right - left);
	}
}

double CalcQuanDist(const double &dist, const double &qstep)
{
  return Quantize( exp ( Quantize ( log( dist + EPS ), qstep ) ), 0.1 );
}
///////////////////////////////////////////////////
///////////////_____Geometry______/////////////////
///////////////////////////////////////////////////
/*
double Vec::operator* (const Vec v) const throw () {
  return x*v.x+y*v.y;
}*/
Angle Vector::angle( const Vector v) const throw()
{
  double z = (X*v.X+Y*v.Y)/sqrt((X*X+Y*Y)*(v.X*v.X+v.Y*v.Y));   
  double z1=1-(z*z);
  if (z1<0) z1=0;   
  double phi = atan2 (sqrt(z1),z);   
  return Angle ( -Y*v.X+X*v.Y >= 0 ? phi : -phi);   	
}

MyAngle Vector::angle() const throw()
{
	return atan2(Y,X);
}

Vector intersect(  MyLine& ln1,  MyLine& ln2) throw( invalid_argument )
{
  Vector d1=ln1.p2-ln1.p1;
  Vector d2=ln2.p2-ln2.p1;
  double det=d1.X*d2.Y-d2.X*d1.Y;
  if (det==0)
    throw invalid_argument("parallel lines");
  Vector dp=ln2.p1-ln1.p1;
  double tau=(d2.Y*dp.X-d2.X*dp.Y)/det;
  return ln1.p1*(1.0-tau)+ln1.p2*tau;
}
 
vector<Vector> intersect(  LineSegment& l1,   MyLine& l2 ) throw(bad_alloc)
{
	Vector is;
	try
	{
		is = intersect( MyLine(l1.p1,l1.p2),l2);
	}catch (invalid_argument) {
		vector<Vector> leer(0);
		return leer;
	}
	double tv = teilverhaeltnis(l1.p1,l1.p2,is);
	if (tv<0|| tv>1) {
        	vector<Vector> leer(0);
		return leer;
	}
    vector<Vector> nleer(1);
    nleer[0] = is;
    return nleer;
}
 
vector<Vector> intersect(  MyLine&  l1, LineSegment&  l2  ) throw(bad_alloc)
{
  return intersect(l2,l1);
} 
 
vector<Vector> intersect(  LineSegment& l1,  LineSegment& l2) throw(bad_alloc)
{
  Vector is;
  try{
    is = intersect (MyLine (l1.p1,l1.p2), MyLine(l2.p1,l2.p2));
  }catch(invalid_argument){
     vector<Vector> leer (0);
    return leer;
  }
  double tv1 = teilverhaeltnis (l1.p1, l1.p2, is);
  double tv2 = teilverhaeltnis (l2.p1, l2.p2, is);
  if (tv1<0 || tv1>1 || tv2<0 || tv2>1) {
    vector<Vector> leer (0);
    return leer;
  }
  vector<Vector> nleer (1);
  nleer[0]=is;
  return nleer;	
} 
 
vector<Vector> intersect(  MyLine& ln,   Circle& cc ) throw(bad_alloc)
{
  double d_len2 = (ln.p1-ln.p2).getSqrMagnitude();
  double p1_len2 = ln.p1.getSqrMagnitude();
  double c_len2 = cc.getCenter().getSqrMagnitude();
  double p1_p2 = ln.p1.X*ln.p2.X+ln.p1.Y*ln.p2.Y;//ln.p1*ln.p2;
  double p1_c = ln.p1.X*cc.getCenter().X+ln.p1.Y*cc.getCenter().Y;
  double p2_c = ln.p2.X*cc.getCenter().X+ln.p2.Y*cc.getCenter().Y;

  double l_term = 2.0*(-p1_len2+p1_p2+p1_c-p2_c);
  double c_term = p1_len2+c_len2-2.0*p1_c-cc.getRadius()*cc.getRadius();

  double rad = l_term*l_term-4.0*d_len2*c_term;
  if (rad<0) 
  {
    vector<Vector> ret (0);
    return ret;
  } 
  else if (rad==0)
  {
     vector<Vector> ret (1);
    double tau = -l_term/(2.0*d_len2);
    ret[0]=ln.p1*(1.0-tau)+ln.p2*tau;
    return ret;
  } 
  else 
  {
    vector<Vector> ret(2);
    double root = sqrt(rad);
    double tau = (-l_term+root)/(2.0*d_len2);
    Vector resl1=ln.p1*(1.0-tau)+ln.p2*tau;
	ret[0] =  resl1;
    tau = (-l_term-root)/(2.0*d_len2);
	Vector resl2 = ln.p1*(1.0-tau)+ln.p2*tau;
    ret[1]=resl2;
    return ret;
  }
} 

vector<Vector> intersect(  Circle& cc1,  Circle& cc2) throw(bad_alloc)
{
  Vector d = (cc2.getCenter()-cc1.getCenter());
  double d_len2 = d.getSqrMagnitude();
  double d_len  =  sqrt (d_len2);
  if ((d_len>(cc1.getRadius()+cc2.getRadius())) || (d_len< fabs(cc1.getRadius()-cc2.getRadius()))) 
  {
    vector<Vector> ret (0);
    return ret;
  }
  else if (d_len==(cc1.getRadius()+cc2.getRadius())) 
  {
    vector<Vector> ret (1);
    if ((cc1.getRadius()==0)&&(cc2.getRadius()==0))
      ret[0]=cc1.getCenter();
    else
      ret[0]=cc1.getCenter()+d*(cc1.getRadius()/(cc1.getRadius()+cc2.getRadius()));
    return ret;
  } 
  else if (d_len==(cc1.getRadius()-cc2.getRadius())) 
  {
    vector<Vector> ret (1);
    ret[0]=cc1.getCenter()+d*(cc1.getRadius()/d_len);
    return ret;
  } 
  else if (d_len==(cc2.getRadius()-cc1.getRadius())) 
  {
    vector<Vector> ret (1);
    ret[0]=cc1.getCenter()-d*(cc1.getRadius()/d_len);
    return ret;
  } 
  else 
  {
    vector<Vector> ret (2);
    Vector d_norm = d*(1.0/d_len);
    Vector d_ortho (-d_norm.Y, d_norm.X);
    double tau = (cc1.getRadius()*cc1.getRadius()+d_len2-cc2.getRadius()*cc2.getRadius())/(2.0*d_len);
    double rho = sqrt(cc1.getRadius()*cc1.getRadius()-tau*tau);
    ret[0]=cc1.getCenter()+d_norm*tau+d_ortho*rho;
    ret[1]=cc1.getCenter()+d_norm*tau-d_ortho*rho;
    return ret;
  }
}

vector<Vector> intersect(  MyLine& l,  MyArc& a  ) throw(bad_alloc)
{
  vector<Vector> res = intersect (l, Circle(a.getCenter(), a.getRadius()));
  unsigned int i=0;
  while( i<res.size() )
    if (!isAngInInterval( (res[i]-a.getCenter()).getDirection(),a.getStart().get_deg(), a.getEnd().get_deg() ) )
      res.erase (res.begin()+i);
    else
      i++;
  return res;
} 

vector<Vector> intersect( MyArc& l ,  MyLine& a) throw(bad_alloc) 
{
  return intersect(l,a);
}

vector<Vector> intersect( LineSegment& l,  MyArc& a )  throw(bad_alloc) 
{
  vector<Vector> res = intersect (MyLine(l.p1, l.p2), a);
  unsigned int i=0;
  while (i<res.size()) 
  {
    double tv;
    if (l.p1.X!=l.p2.X)
      tv = (res[i].X-l.p1.X)/(l.p2.X-l.p1.X);
    else
      tv = (res[i].Y-l.p1.Y)/(l.p2.Y-l.p1.Y);
    if (tv>1 || tv<0)
      res.erase (res.begin()+i);
    else
      i++;
  }
  return res;	
}

vector<Vector> intersect( MyArc& a, LineSegment& l )  throw(bad_alloc) 
{
  return intersect(l,a);
}

vector<Vector> tangent_point(  Circle& cc,  Vector& p) throw(bad_alloc,invalid_argument)   
{
  Vector d = (p-cc.getCenter());
  double d_len = d.getMagnitude();
  if (d_len<cc.getRadius())
    throw  invalid_argument ("no tangent possible in tangent_point");
  else if (d_len==cc.getRadius()) 
  {
    vector<Vector> ret (1);
    ret[0]=p;
    return ret;
  } 
  else
  {
    vector<Vector> ret (2);
    Vector d_norm = d*(1.0/d_len);
    Vector d_ortho (-d_norm.Y,d_norm.X);
    double tau = (cc.getRadius()*cc.getRadius())/d_len;
    double rho =  sqrt(cc.getRadius()*cc.getRadius()-tau*tau);
    ret[0]=cc.getCenter()+d_norm*tau+d_ortho*rho;
    ret[1]=cc.getCenter()+d_norm*tau-d_ortho*rho;
    return ret;
  }
}

Vector::Vector(double x1,double y1,CoordSystemT cs)
{
	setVecPosition( x1, y1, cs );
}
 
bool Vector::setX(double dX)
{
  X = dX;
  return ( true );
}

double Vector::getX() const
{
 return ( X );
}

bool Vector::setY(double dY)
{
  Y = dY;
  return ( true );
}

double Vector::getY() const
{
  return ( Y );
}
 
double Vector::getDirection()
{
  return ( atan2Deg( Y, X ) );
}

double  Vector::getMagnitude() const
{
  return ( sqrt( X * X + Y * Y ) );
}
 

double  Vector::getSqrMagnitude() const
{
  return (  X * X + Y * Y  );
}

double Vector::getDistanceTo(const Vector p)
{
 return ( ( *this - p ).getMagnitude( ) );
}

Vector Vector::getVecPositionFromPolar(double dMag, AngDeg ang)
{
   double cosang =  cosDeg( ang );
   double sinang =  sinDeg( ang );
   return ( Vector( cosang * dMag  , sinang * dMag   ) );
}

Vector Vector::getVecPositionOnLineFraction(Vector &p, double dFrac)
{
  return ( ( *this ) * ( 1.0 - dFrac ) + ( p * dFrac ) );
}

bool Vector::setVecPosition(double dX, double dY, CoordSystemT cs)
{
  if( cs == CARTESIAN )
  {
    X = dX;
    Y = dY;
  }
  else
    *this = getVecPositionFromPolar( dX, dY );
  return true;
}

Vector Vector::rotate(AngDeg angle)
{
  double dMag    = this->getMagnitude( );
  double dNewDir = this->getDirection( ) + angle;  // add rotation angle to phi
  setVecPosition( dMag, dNewDir, POLAR );          // convert back to Cartesian
  return ( *this );
/*
  double sina = sinDeg(angle );
  double cosa = cosDeg(angle );
  return Vector(X*cosa-Y*sina, X*sina+Y*cosa);*/
}

Vector Vector::rotate( const MyAngle angle ) const throw()
{
  double sina = sin(angle.get_rad() );
  double cosa = cos(angle.get_rad() );
  return Vector(X*cosa-Y*sina, X*sina+Y*cosa); 
}

Vector Vector::globalToRelative(Vector origin, AngDeg ang)
{
  *this = *this - origin;
  return ( rotate( -ang ) );
}

Vector Vector::setMagnitude(double d)
{
 if( getMagnitude( ) > EPSILON )
     ( *this ) = ( *this ) * ( d / getMagnitude( ) );

  return ( *this );
}

Vector Vector::normalize()
{
return ( setMagnitude( 1.0 ) );
}

AngDeg Vector::normalizeAngle(AngDeg angle)
{
while( angle > 180.0  ) angle -= 360.0;
  while( angle < -180.0 ) angle += 360.0;

  return ( angle );
}

Vector Vector::relativeToGlobal(Vector origin, AngDeg ang)
{
  rotate( ang );
  *this = *this + origin;
  return ( *this );
}


Vector Vector::operator - ( )
{
  return ( Vector( -X, -Y ) );
}

/*! Overloaded version of the binary plus operator for adding a given double
    value to a Vector. The double value is added to both the x- and
    y-coordinates of the current Vector. The current Vector itself is
    left unchanged.
    \param d a double value which has to be added to both the x- and
    y-coordinates of the current Vector
    \return the result of adding the given double value to the current
    Vector */
Vector Vector::operator + ( const double &d )
{
  return ( Vector( X + d, Y + d ) );
}

/*! Overloaded version of the binary plus operator for Vectors. It returns
    the sum of the current Vector and the given Vector by adding their
    x- and y-coordinates. The Vectors themselves are left unchanged.
    \param p a Vector
    \return the sum of the current Vector and the given Vector */
Vector Vector::operator + ( const Vector &p )
{
  return ( Vector( X + p.X, Y + p.Y ) );
}

/*! Overloaded version of the binary minus operator for subtracting a
    given double value from a Vector. The double value is
    subtracted from both the x- and y-coordinates of the current
    Vector. The current Vector itself is left unchanged.
    \param d a double value which has to be subtracted from both the x- and
    y-coordinates of the current Vector
    \return the result of subtracting the given double value from the current
    Vector */
Vector Vector::operator - ( const double &d )
{
  return ( Vector( X - d, Y - d ) );
}

/*! Overloaded version of the binary minus operator for
    Vectors. It returns the difference between the current
    Vector and the given Vector by subtracting their x- and
    y-coordinates. The Vectors themselves are left unchanged.

    \param p a Vector
    \return the difference between the current Vector and the given
    Vector */
Vector Vector::operator - ( const Vector &p )
{
  return ( Vector( X - p.X, Y - p.Y ) );
}

/*! Overloaded version of the multiplication operator for multiplying a
    Vector by a given double value. Both the x- and y-coordinates of the
    current Vector are multiplied by this value. The current Vector
    itself is left unchanged.
    \param d the multiplication factor
    \return the result of multiplying the current Vector by the given
    double value */
Vector Vector::operator * (  const double &d  )
{
  return ( Vector( X * d, Y * d  ) );
}

/*! Overloaded version of the multiplication operator for
    Vectors. It returns the product of the current Vector
    and the given Vector by multiplying their x- and
    y-coordinates. The Vectors themselves are left unchanged.

    \param p a Vector
    \return the product of the current Vector and the given Vector */
Vector Vector::operator * ( const Vector &p )
{
  return ( Vector( X * p.X, Y * p.Y ) );
}

 
/*! Overloaded version of the division operator for dividing a
    Vector by a given double value. Both the x- and y-coordinates
    of the current Vector are divided by this value. The current
    Vector itself is left unchanged.

    \param d the division factor
    \return the result of dividing the current Vector by the given double
    value */
Vector Vector::operator / ( const double &d )
{
  return ( Vector( X / d, Y / d  ) );
}

/*! Overloaded version of the division operator for Vectors. It
    returns the quotient of the current Vector and the given
    Vector by dividing their x- and y-coordinates. The
    Vectors themselves are left unchanged.

    \param p a Vector
    \return the quotient of the current Vector and the given one */
Vector Vector::operator / ( const Vector &p )
{
  return ( Vector( X / p.X, Y / p.Y ) );
}

/*! Overloaded version of the assignment operator for assigning a given double
    value to both the x- and y-coordinates of the current Vector. This
    changes the current Vector itself.
    \param d a double value which has to be assigned to both the x- and
    y-coordinates of the current Vector */
void Vector::operator = ( const double &d )
{
  X = d;
  Y = d;
}

/*! Overloaded version of the sum-assignment operator for Vectors. It
    returns the sum of the current Vector and the given Vector by
    adding their x- and y-coordinates. This changes the current Vector
    itself.
    \param p a Vector which has to be added to the current Vector */
void Vector::operator +=( const Vector &p )
{
  X += p.X;
  Y += p.Y;
}

/*! Overloaded version of the sum-assignment operator for adding a given double
    value to a Vector. The double value is added to both the x- and
    y-coordinates of the current Vector. This changes the current
    Vector itself.
    \param d a double value which has to be added to both the x- and
    y-coordinates of the current Vector */
void Vector::operator += ( const double &d )
{
  X += d;
  Y += d;
}

/*! Overloaded version of the difference-assignment operator for
    Vectors.  It returns the difference between the current
    Vector and the given Vector by subtracting their x- and
    y-coordinates. This changes the current Vector itself.

    \param p a Vector which has to be subtracted from the current
    Vector */
void Vector::operator -=( const Vector &p )
{
  X -= p.X;
  Y -= p.Y;
}

/*! Overloaded version of the difference-assignment operator for
    subtracting a given double value from a Vector. The double
    value is subtracted from both the x- and y-coordinates of the
    current Vector. This changes the current Vector itself.

    \param d a double value which has to be subtracted from both the x- and
    y-coordinates of the current Vector */
void Vector::operator -=( const double &d )
{
  X -= d;
  Y -= d;
}

/*! Overloaded version of the multiplication-assignment operator for
    Vectors. It returns the product of the current Vector
    and the given Vector by multiplying their x- and
    y-coordinates. This changes the current Vector itself.

    \param p a Vector by which the current Vector has to be
    multiplied */
void Vector::operator *=( const Vector &p )
{
  X *= p.X;
  Y *= p.Y;
}

/*! Overloaded version of the multiplication-assignment operator for
    multiplying a Vector by a given double value. Both the x- and
    y-coordinates of the current Vector are multiplied by this
    value. This changes the current Vector itself.

    \param d a double value by which both the x- and y-coordinates of the
    current Vector have to be multiplied */
void Vector::operator *=( const double &d )
{
  X *= d;
  Y *= d;
}

/*! Overloaded version of the division-assignment operator for
    Vectors. It returns the quotient of the current Vector
    and the given Vector by dividing their x- and
    y-coordinates. This changes the current Vector itself.

    \param p a Vector by which the current Vector is divided */
void Vector::operator /=( const Vector &p )
{
  X /= p.X;
  Y /= p.Y;
}

/*! Overloaded version of the division-assignment operator for
    dividing a Vector by a given double value. Both the x- and
    y-coordinates of the current Vector are divided by this
    value. This changes the current Vector itself.

    \param d a double value by which both the x- and y-coordinates of the
    current Vector have to be divided */
void Vector::operator /=( const double &d )
{
  X /= d;
  Y /= d;
}

/*! Overloaded version of the inequality operator for Vectors. It
    determines whether the current Vector is unequal to the given
    Vector by comparing their x- and y-coordinates.

    \param p a Vector
    \return true when either the x- or y-coordinates of the given Vector
    and the current Vector are different; false otherwise */
bool Vector::operator !=( const Vector &p )
{
  return ( ( X != p.X ) || ( Y != p.Y ) );
}

/*! Overloaded version of the inequality operator for comparing a
    Vector to a double value. It determines whether either the x-
    or y-coordinate of the current Vector is unequal to the given
    double value.

    \param d a double value with which both the x- and y-coordinates of the
    current Vector have to be compared.
    \return true when either the x- or y-coordinate of the current Vector
    is unequal to the given double value; false otherwise */
bool Vector::operator !=( const double &d )
{
  return ( ( X != d ) || ( Y != d ) );
}

/*! Overloaded version of the equality operator for Vectors. It
    determines whether the current Vector is equal to the given
    Vector by comparing their x- and y-coordinates.

    \param p a Vector
    \return true when both the x- and y-coordinates of the given
    Vector and the current Vector are equal; false
    otherwise */
bool Vector::operator ==( const Vector &p )
{
  return ( ( X == p.X ) && ( Y == p.Y ) );
}

/*! Overloaded version of the equality operator for comparing a
    Vector to a double value. It determines whether both the x-
    and y-coordinates of the current Vector are equal to the
    given double value.

    \param d a double value with which both the x- and y-coordinates of the
    current Vector have to be compared.
    \return true when both the x- and y-coordinates of the current Vector
    are equal to the given double value; false otherwise */
bool Vector::operator ==( const double &d )
{
  return ( ( X == d ) && ( Y == d ) );
}

bool Vector::isInFrontOf(const Vector &p)
{
  return ( ( X > p.X ) ? true : false );
}
 
bool Vector::isInFrontOf(const double &d)
{
 return ( ( X > d ) ? true : false );
}
 
bool Vector::isBehindOf(const Vector &p)
{
  return ( ( X < p.X ) ? true : false );
}
 
bool Vector::isBehindOf(const double &d)
{
 return ( ( X < d ) ? true : false );
}

bool Vector::isLeftOf(const Vector &p)
{
  return ( ( Y < p.Y ) ? true : false );
}
 
bool Vector::isLeftOf(const double &d)
{
return ( ( Y < d ) ? true : false );
}
 
bool Vector::isRightOf(const Vector &p)
{
 return ( ( Y > p.Y ) ? true : false );
}
 
bool Vector::isRightOf(const double &d)
{
 return ( ( Y > d ) ? true : false );
}
 
bool Vector::isBetweenX(const Vector &p1, const Vector &p2)
{
 return ( ( isInFrontOf( p1 ) && isBehindOf( p2 ) ) ? true : false );
}

bool Vector::isBetweenX(const double &d1, const double &d2)
{
  return ( ( isInFrontOf( d1 ) && isBehindOf( d2 ) ) ? true : false );
}
 
bool Vector::isBetweenY(const Vector &p1, const Vector &p2)
{
 return ( ( isRightOf( p1 ) && isLeftOf( p2 ) ) ? true : false );
}
 
bool Vector::isBetweenY(const double &d1, const double &d2)
{
 return ( ( isRightOf( d1 ) && isLeftOf( d2 ) ) ? true : false );
}
 
 
Vector Vector::s_rotate (const Angle a)  
{
  (*this) = rotate (a);
  return (*this);
}

Vector Vector::rotate_twelvth () const  
{
  return Vector (X*CONSTWURZELDREIHALBE-Y*0.5, X*0.5+Y*CONSTWURZELDREIHALBE);
}
Vector Vector::rotate_eleven_twelvth () const  
{
  return Vector (X*CONSTWURZELDREIHALBE+Y*0.5, -0.5*X+Y*CONSTWURZELDREIHALBE);
}
Vector Vector::rotate_eighth () const 
{
  return Vector (CONSTWURZELZWEIHALBE*(X-Y), CONSTWURZELZWEIHALBE*(X+Y));
}

Vector Vector::rotate_seven_eighth () const 
{
  return Vector (CONSTWURZELZWEIHALBE*(X+Y), CONSTWURZELZWEIHALBE*(Y-X));
}
Vector Vector::rotate_sixth () const  
{
  return Vector (0.5*X-Y*CONSTWURZELDREIHALBE, X*CONSTWURZELDREIHALBE+Y*0.5);
}

Vector Vector::rotate_five_sixth () const  
{
  return Vector (0.5*X+Y*CONSTWURZELDREIHALBE, -X*CONSTWURZELDREIHALBE+Y*0.5);
}

Vector Vector::rotate_quarter () const  
{
  return Vector (-Y,X);
}

Vector Vector::rotate_three_quarters () const  
 {
  return Vector (Y,-X);
}

Vector Vector::rotate_half () const 
{
  return Vector (-X,-Y);
}

Vector Vector::mirror (const Vector v) const  
{
  double n=v.X*v.X+v.Y*v.Y;
  n = (n <= 0 ? 1 : n);    
  double f =2*(v.X*X+v.Y*Y)/n;
  return Vector (f*v.X-X, f*v.Y-Y);
}

Vector Vector::mirror_x () const 
{
  return Vector (X,-Y);
}
Vector Vector::mirror_y () const  
{
  return Vector (-X,Y);
}

Vector Vector::mirror_eighth () const   
{
  return Vector (Y,X);
}

Vector Vector::mirror_three_eighth () const   
{
  return Vector (-Y,-X);
}

Vector Vector::unit_vector (MyAngle a) throw () 
{
  return Vector(cos(a.get_rad()), sin(a.get_rad()));
}
/*****************************************************************************/
/***********************  CLASS LINE *****************************************/
/*****************************************************************************/
MyLine::MyLine():p1(Vector(0,0)),p2(Vector(1,0)) { ;}

MyLine::MyLine(Vector pp1,Vector pp2):p1(pp1),p2(pp2){ ;} 

MyLine::MyLine(const MyLine& ln):p1(ln.p1),p2(ln.p2){;}

const MyLine& MyLine::operator =(const MyLine& ln)
{ 
	p1=ln.p1;p2=ln.p2;
	makeLineFromTwoPoints( p1, p2);
	return (*this);
}
 
int MyLine::side( Vector p )
{
  Vector d = p2-p1;
  Vector d_ortho(d.Y, -d.X);
  double sp = d_ortho.X*(p-p1).X+d_ortho.Y+(p-p1).Y;
  return (sp>0 ? 1 : (sp<0 ? -1 : 0 ));
}

Vector MyLine::perpendicular_point(  Vector &p )
{
  Vector d=p2-p1;
  return p1+((p*d-p1*d)/(d.getSqrMagnitude()))*d;
} 

double MyLine::distance(  Vector p )
{
  Vector d=p2-p1;
  Vector l=p1+((p*d-p1*d)/(d.getSqrMagnitude()))*d;
  return (p-l).getMagnitude();	
}

MyLine& MyLine::s_rotate(const Angle& a)
{
  p1=p1.rotate(a); p2=p2.rotate(a);   
  return *this;
}

MyLine& MyLine::s_translate (const Vector& v)  
{
  p1+=v; p2+=v;                      
  return *this;
}

MyLine MyLine::rotate(const Angle a) const
{
  MyLine dest (*this);
  dest.s_rotate (a);
  return dest;
}

MyLine MyLine::translate (const Vector v) const 
{
  MyLine dest (*this);
  dest.s_translate (v);
  return dest;
}

MyLine operator* (const Frame2d& f, const MyLine& l) throw () 
{
  return MyLine(f*l.p1, f*l.p2);
}
 
Vector MyLine::getIntersection(MyLine line)
{
  Vector pos;
  double x, y;
  if( ( A / B ) ==  (line.getACoefficient() / line.getBCoefficient() ))
    return pos; // lines are parallel, no intersection
  if( A == 0 )            // bx + c = 0 and a2*y + b2*x + c2 = 0 ==> x = -c/b
  {                          // calculate x using the current line
    x = -C/B;                // and calculate the y using the second line
    y = line.getYGivenX(x);
  }
  else if( line.getACoefficient() == 0 )
  {                         // ay + bx + c = 0 and b2*x + c2 = 0 ==> x = -c2/b2
   x = -line.getCCoefficient()/line.getBCoefficient(); // calculate x using
   y = getYGivenX(x);       // 2nd line and calculate y using current line
  }
  // ay + bx + c = 0 and a2y + b2*x + c2 = 0
  // y = (-b2/a2)x - c2/a2
  // bx = -a*y - c =>  bx = -a*(-b2/a2)x -a*(-c2/a2) - c ==>
  // ==> a2*bx = a*b2*x + a*c2 - a2*c ==> x = (a*c2 - a2*c)/(a2*b - a*b2)
  // calculate x using the above formula and the y using the current line
  else
  {
    x = (A*line.getCCoefficient() - line.getACoefficient()*C)/
                    (line.getACoefficient()*B - A*line.getBCoefficient());
    y = getYGivenX(x);
  }

  return Vector( x, y );
}

double MyLine::getACoefficient() const
{
 return A;
}

double MyLine::getBCoefficient() const
{
 return B;
}

double MyLine::getCCoefficient() const
{
return C;
}

double MyLine::getYGivenX(double x)
{
 if( A == 0 )
 {
    
   return 0;
 }
  // ay + bx + c = 0 ==> ay = -(b*x + c)/a
  return -(B*x+C)/A;

}

double MyLine::getXGivenY(double y)
{
 if( B == 0 )
 {
    
   return 0;
 }
  // ay + bx + c = 0 ==> bx = -(a*y + c)/a
  return -(A*y+C)/B;
}

bool MyLine::isInBetween(Vector pos, Vector point1, Vector point2)
{
  pos          = getPointOnLineClosestTo( pos ); // get closest point
  double dDist = point1.getDistanceTo( point2 ); // get distance between 2 pos

  // if the distance from both points to the projection is smaller than this
  // dist, the pos lies in between.
  return pos.getDistanceTo( point1 ) <= dDist &&
         pos.getDistanceTo( point2 ) <= dDist;
}

MyLine MyLine::getTangentLine(Vector pos)// ´¹Ïß
{
 return MyLine( B, -A, A*pos.X - B*pos.Y );
}

Vector MyLine::getPointOnLineClosestTo(Vector pos)
{
  //MyLine l2 = getTangentLine( pos );  // get tangent line
  return getIntersection( getTangentLine( pos ) );
}

double MyLine::getDistanceWithPoint(Vector pos)
{
 return pos.getDistanceTo( getPointOnLineClosestTo( pos ) );
}

MyLine MyLine::makeLineFromTwoPoints(Vector pos1, Vector pos2)
{
  double dA, dB, dC;
  double dTemp = pos2.X - pos1.X; // determine the slope
  if( fabs(dTemp) < EPSILON )
  {
    // ay + bx + c = 0 with vertical slope=> a = 0, b = 1
    dA = 0.0;
    dB = 1.0;
  }
  else
  {
    // y = (-b)x -c with -b the slope of the line
    dA = 1.0;
    dB = -(pos2.Y - pos1.Y )/dTemp;
  }
  // ay + bx + c = 0 ==> c = -a*y - b*x
  dC =  - dA*pos2.Y  - dB * pos2.X;
  return MyLine( dA, dB, dC );
}

/*****************************************************************************/
/***********************  CLASS LINESEGMENT***********************************/
/*****************************************************************************/
LineSegment::LineSegment():p1(Vector(0,0)),p2(Vector(1,0)) {;}
LineSegment::LineSegment(const Vector start, const Vector end):p1(start),p2(end) {;}
LineSegment::LineSegment(const LineSegment& ls):p1(ls.p1),p2(ls.p2){;}
const LineSegment& LineSegment::operator =(const LineSegment& ls){ p1=ls.p1;p2=ls.p2;return (*this);}
double LineSegment::distance ( Vector p)  
{
  Vector f = MyLine(p1, p2).perpendicular_point(p);
  double tv;
  if (p1.X !=p2.X )
    tv = (f.X-p1.X)/(p2.X-p1.X);
  else
    tv = (f.Y-p1.Y)/(p2.Y-p1.Y);
  if ( tv>=1 )
    return (p2-p).getMagnitude();
  else if( tv<=0 )
    return (p1-p).getMagnitude();
  else
    return (f-p).getMagnitude();
}

const Vector&  LineSegment::getStart() const  
{ return p1; }

const Vector& LineSegment::getEnd() const 
{ return p2; }

LineSegment& LineSegment::s_rotate (const MyAngle& a) 
{
  p1=p1.rotate(a.get_deg()); p2=p2.rotate(a.get_deg());   
  return *this;
}

LineSegment& LineSegment::s_translate (const Vector& v)  
{
  p1+=v; p2+=v;                       
  return *this;
}
 
LineSegment LineSegment::rotate (const MyAngle a) const
{
  LineSegment dest (*this);
  dest.s_rotate (a);
  return dest;
}

LineSegment LineSegment::translate (const Vector v)  const
{
  LineSegment dest (*this);
  dest.s_translate (v);
  return dest;
} 

LineSegment operator* (const Frame2d& f, const LineSegment& l) throw () 
{
  return LineSegment (f*l.p1, f*l.p2);
} 


/*****************************************************************************/
/********************* CLASS CIRCLE ******************************************/
/*****************************************************************************/
Circle::Circle( Vector pos, double dR )
{
  setCircle( pos, dR );
}
   
Circle::Circle( )
{
  setCircle( Vector(-10000.0,-10000.0), 0);
}
 
Circle::Circle(  Vector p1,  Vector p2,   Vector p3) throw(invalid_argument)
{
  MyLine ln1((p1+p2)*0.5, (p1+p2)+(p2-p1).rotate_quarter()*0.5);
  MyLine ln2((p2+p3)*0.5, (p2+p3)+(p3-p2).rotate_quarter()*0.5);
  Vector center = intersect (ln1, ln2);  
  setCenter( center );
  double radius = (center-p1).getMagnitude();
  setRadius( radius  );
} 

Circle::Circle( const Circle& cc ) throw():m_posCenter(cc.m_posCenter),m_dRadius(cc.m_dRadius) {;}

const Circle& Circle::operator =(   Circle& cc ) throw()
{
  m_posCenter = cc.getCenter();
  m_dRadius   = cc.getRadius();
  return (*this);
}

bool Circle::setCircle( Vector pos, double dR )
{
  setCenter( pos );
  return setRadius( dR  );
}
 
bool Circle::setRadius( double dR )
{
  if( dR > 0 )
  {
    m_dRadius = dR;
    return true;
  }
  else
  {
    m_dRadius = 0.0;
    return false;
  }
}

double Circle::getRadius()
{
  return m_dRadius;
}
  
bool Circle::setCenter( Vector pos )
{
  m_posCenter = pos;
  return true;
}
  
Vector Circle::getCenter()
{
  return m_posCenter;
}
  
double Circle::getCircumference()
{
  return 2.0*PI*getRadius();
}
  
double Circle::getArea()
{
  return PI*getRadius()*getRadius();
}
  
bool Circle::isInside( Vector pos )
{
  return m_posCenter.getDistanceTo( pos ) < getRadius() ;
}
 
int Circle::getIntersectionPoints( Circle c, Vector *p1, Vector *p2)
{
    double x0, y0, r0;
    double x1, y1, r1;

    x0 = getCenter( ).X;
    y0 = getCenter( ).Y;
    r0 = getRadius( );
    x1 = c.getCenter( ).X;
    y1 = c.getCenter( ).Y;
    r1 = c.getRadius( );

    double      d, dx, dy, h, a, x, y, p2_x, p2_y;

     dx = x1 - x0;
    dy = y1 - y0;
    d = sqrt(dx*dx + dy*dy);
    dx /= d; dy /= d;
    a = (r0*r0 + d*d - r1*r1) / (2.0 * d);
    double      arg = r0*r0 - a*a;
    h = (arg > 0.0) ? sqrt(arg) : 0.0;

    // First calculate P2
    p2_x = x0 + a * dx;
    p2_y = y0 + a * dy;

   
    x =  p2_x - h * dy;
    y =  p2_y + h * dx;
    p1->setVecPosition( x, y );
    x =  p2_x + h * dy;
    y =  p2_y - h * dx;
    p2->setVecPosition( x, y );

    return (arg < 0.0) ? 0 : ((arg == 0.0 ) ? 1 :  2);
}
 
double Circle::getIntersectionArea( Circle c )
{
  Vector pos1, pos2, pos3;
  double d, h, dArea;
  AngDeg ang;

  d = getCenter().getDistanceTo( c.getCenter() ); // dist between two centers
  if( d > c.getRadius() + getRadius() )           // larger than sum radii
    return 0.0;                                   // circles do not intersect
  if( d <= fabs(c.getRadius() - getRadius() ) )   // one totally in the other
  {
    double dR = min( c.getRadius(), getRadius() );// return area smallest circ
    return PI*dR*dR;
  }

  int iNrSol = getIntersectionPoints( c, &pos1, &pos2 );
  if( iNrSol != 2 )
    return 0.0;
  pos3 = pos1.getVecPositionOnLineFraction( pos2, 0.5 );
  d = pos1.getDistanceTo( pos3 );
  h = pos3.getDistanceTo( getCenter() );
  ang = asin( d / getRadius() );

  dArea = ang*getRadius()*getRadius();
  dArea = dArea - d*h;

  // and now for the other segment the same story
  h = pos3.getDistanceTo( c.getCenter() );
  ang = asin( d / c.getRadius() );
  dArea = dArea + ang*c.getRadius()*c.getRadius();
  dArea = dArea - d*h;

  return dArea;
}

double Circle::distance( Vector p) const
{
  return (p - m_posCenter).getMagnitude()-m_dRadius;
}

Circle  operator* (const Frame2d& f, const Circle& c) throw () 
{
  return Circle (f*c.m_posCenter, c.m_dRadius);
}
/*****************************************************************************/
/********************* CLASS MyArc    ******************************************/
/*****************************************************************************/
MyArc::MyArc():center(Vector(0,0)),radius(1),start(0),end(180) {;}
MyArc::MyArc(Vector c,double r,MyAngle s,MyAngle e):center(c),radius(r),start(s),end(e) {;}
MyArc::MyArc(const MyArc& a):center(a.center),radius(a.radius),start(a.start),end(a.end) {;}
const MyArc& MyArc::operator= ( const MyArc& a )
{
	center = a.center;
	radius = a.radius;
	start = a.start;
	end   = a.end;
	return (*this);
}

double MyArc::distance( Vector p)
{
  Vector v = p - center;
 // if (isAngInInterval(v.getDirection(),start,end))
   // return abs(v.getMagnitude()-radius);
  if(isAngInInterval(v.getDirection(),start.get_deg(),end.get_deg()))
     return fabs(v.getMagnitude()-radius);
  else 
  {
    //double d1 = (Vector(1,start,POLAR)-p).getSqrMagnitude();
    //double d2 = (Vector(1,end,POLAR)-p).getSqrMagnitude();
	  double d1 = (Vector::unit_vector(start)-p).getSqrMagnitude();
     double d2 = (Vector::unit_vector(end)-p).getSqrMagnitude();
    if (d1<d2)
      return sqrt(d1);
    else
      return sqrt(d2);
  }
}

MyArc operator* (const Frame2d& f, const MyArc& a) throw () 
{
  return MyArc(f*a.center, a.radius, a.start+f.get_angle(), a.end+f.get_angle());
}
/*****************************************************************************/
/********************* CLASS RECTANGLE ***************************************/
/*****************************************************************************/

Rect::Rect( Vector pos, Vector pos2 )
{
  setRectanglePoints( pos, pos2 );
}
  
void Rect::setRectanglePoints( Vector pos1, Vector pos2 )
{
  m_posLeftTop.X  =  ( max( pos1.X, pos2.X ) );
  m_posLeftTop.Y  =  ( min( pos1.Y, pos2.Y ) );
  m_posRightBottom.X=( min( pos1.X, pos2.X ) );
  m_posRightBottom.Y=( max( pos1.Y, pos2.Y ) );
}
 
bool Rect::isInside( Vector pos )
{
  return pos.isBetweenX( m_posRightBottom.X, m_posLeftTop.X ) &&
         pos.isBetweenY( m_posLeftTop.Y,     m_posRightBottom.Y);

}
 
bool Rect::setPosLeftTop( Vector pos )
{
  m_posLeftTop = pos;
  return true;
}

/*! This method returns the top left position of the rectangle
    \return top left position of the rectangle */
Vector Rect::getPosLeftTop(  )
{
  return m_posLeftTop;
}

/*! This method sets the right bottom position of the rectangle
    \param pos new right bottom position of the rectangle
    \return true when update was succesfull */
bool Rect::setPosRightBottom( Vector pos )
{
  m_posRightBottom = pos;
  return true;
}

/*! This method returns the right bottom position of the rectangle
    \return top right bottom of the rectangle */
Vector Rect::getPosRightBottom(  )
{
  return m_posRightBottom;
}
 
MyLine MyLine::makeLineFromPositionAndAngle( Vector vec, AngDeg angle )
{
  // calculate point somewhat further in direction 'angle' and make
  // line from these two points.
  return makeLineFromTwoPoints( vec, vec+Vector(1,angle,POLAR));
} 


 
/*****************************************************************************/
/********************** CLASS GEOMETRY ***************************************/
/*****************************************************************************/

/*! A geometric series is one in which there is a constant ratio between each
    element and the one preceding it. This method determines the
    length of a geometric series given its first element, the sum of the
    elements in the series and the constant ratio between the elements.
    Normally: s = a + ar + ar^2 + ...  + ar^n
    Now: dSum = dFirst + dFirst*dRatio + dFirst*dRatio^2 + .. + dFist*dRatio^n
    \param dFirst first term of the series
    \param dRatio ratio with which the the first term is multiplied
    \param dSum the total sum of all the serie
    \return the length(n in above example) of the series */
double Geometry::getLengthGeomSeries( double dFirst, double dRatio, double dSum )
{
  if( dRatio < 0 )
    return 0;
  // s = a + ar + ar^2 + .. + ar^n-1 and thus sr = ar + ar^2 + .. + ar^n
  // subtract: sr - s = - a + ar^n) =>  s(1-r)/a + 1 = r^n = temp
  // log r^n / n = n log r / log r = n = length
  double temp = (dSum * ( dRatio - 1 ) / dFirst) + 1;
  if( temp <= 0 )
    return -1.0;
  return log( temp ) / log( dRatio ) ;
}

/*! A geometric series is one in which there is a constant ratio between each
    element and the one preceding it. This method determines the sum of a
    geometric series given its first element, the ratio and the number of steps
    in the series
    Normally: s = a + ar + ar^2 + ...  + ar^n
    Now: dSum = dFirst + dFirst*dRatio + ... + dFirst*dRatio^dSteps
    \param dFirst first term of the series
    \param dRatio ratio with which the the first term is multiplied
    \param dSum the number of steps to be taken into account
    \return the sum of the series */
double Geometry::getSumGeomSeries( double dFirst, double dRatio, double dLength)
{
  // s = a + ar + ar^2 + .. + ar^n-1 and thus sr = ar + ar^2 + .. + ar^n
  // subtract: s - sr = a - ar^n) =>  s = a(1-r^n)/(1-r)
  return dFirst * ( 1 - pow( dRatio, dLength ) ) / ( 1 - dRatio ) ;
}

/*! A geometric series is one in which there is a constant ratio between each
    element and the one preceding it. This method determines the sum of an
    infinite geometric series given its first element and the constant ratio
    between the elements. Note that such an infinite series will only converge
    when 0<r<1.
    Normally: s = a + ar + ar^2 + ar^3 + ....
    Now: dSum = dFirst + dFirst*dRatio + dFirst*dRatio^2...
    \param dFirst first term of the series
    \param dRatio ratio with which the the first term is multiplied
    \return the sum of the series */
double Geometry::getSumInfGeomSeries( double dFirst, double dRatio )
{
  if( dRatio > 1 )
    return 0;
  // s = a(1-r^n)/(1-r) with n->inf and 0<r<1 => r^n = 0
  return dFirst / ( 1 - dRatio );
}

/*! A geometric series is one in which there is a constant ratio between each
    element and the one preceding it. This method determines the first element
    of a geometric series given its element, the ratio and the number of steps
    in the series
    Normally: s = a + ar + ar^2 + ...  + ar^n
    Now: dSum = dFirst + dFirst*dRatio + ... + dFirst*dRatio^dSteps
    \param dSum sum of the series
    \param dRatio ratio with which the the first term is multiplied
    \param dSum the number of steps to be taken into account
    \return the first element (a) of a serie */
double Geometry::getFirstGeomSeries( double dSum, double dRatio, double dLength)
{
  // s = a + ar + ar^2 + .. + ar^n-1 and thus sr = ar + ar^2 + .. + ar^n
  // subtract: s - sr = a - ar^n) =>  s = a(1-r^n)/(1-r) => a = s*(1-r)/(1-r^n)
  return dSum *  ( 1 - dRatio )/( 1 - pow( dRatio, dLength ) ) ;
}

/*! A geometric series is one in which there is a constant ratio between each
    element and the one preceding it. This method determines the first element
    of an infinite geometric series given its first element and the constant
    ratio between the elements. Note that such an infinite series will only
    converge when 0<r<1.
    Normally: s = a + ar + ar^2 + ar^3 + ....
    Now: dSum = dFirst + dFirst*dRatio + dFirst*dRatio^2...
    \param dSum sum of the series
    \param dRatio ratio with which the the first term is multiplied
    \return the first term of the series */
double Geometry::getFirstInfGeomSeries( double dSum, double dRatio )
{
  if( dRatio > 1 )
    return 0;
  // s = a(1-r^n)/(1-r) with r->inf and 0<r<1 => r^n = 0 => a = s ( 1 - r)
  return dSum * ( 1 - dRatio );
}

/*! This method performs the abc formula (Pythagoras' Theorem) on the given
    parameters and puts the result in *s1 en *s2. It returns the number of
    found coordinates.
    \param a a parameter in abc formula
    \param b b parameter in abc formula
    \param c c parameter in abc formula
    \param *s1 first result of abc formula
    \param *s2 second result of abc formula
    \return number of found x-coordinates */
int Geometry::abcFormula(double a, double b, double c, double *s1, double *s2)
{
  double dDiscr = b*b - 4*a*c;       // discriminant is b^2 - 4*a*c
  if (fabs(dDiscr) < EPSILON )       // if discriminant = 0
  {
    *s1 = -b / (2 * a);              //  only one solution
    return 1;
  }
  else if (dDiscr < 0)               // if discriminant < 0
    return 0;                        //  no solutions
  else                               // if discriminant > 0
  {
    dDiscr = sqrt(dDiscr);           //  two solutions
    *s1 = (-b + dDiscr ) / (2 * a);
    *s2 = (-b - dDiscr ) / (2 * a);
    return 2;
  }
}

/*****************************************************************************/
/********************** CLASS FRAME2D ***************************************/
/*****************************************************************************/
Vector operator*(const Frame2d &f1, const Vector &v) 
{
    Vector res;
    res.X= f1.n_x*v.X - f1.n_y*v.Y + f1.p_x;
    res.Y= f1.n_y*v.X + f1.n_x*v.Y + f1.p_y;
    return res;
}

Frame2d::Frame2d() 
{
    scale = 1.0;
    n_x= 1.0;
    n_y= 0.0;
    p_x= 0.0;
    p_y= 0.0;
}

Frame2d::Frame2d (Vector p, Angle a) 
{
	scale=1.0;
	set_angle (a);
	set_position (p);
}

Angle Frame2d::get_angle() const 
{
	Vector vec(n_x,n_y);
	return vec.getDirection();
}

Vector Frame2d::get_heading() const 
{
  return (Vector(n_x,n_y)).normalize();
}

void Frame2d::set_angle(const Angle& a) 
{
	n_x= cosDeg(a ) * scale;
	n_y= sinDeg(a ) * scale;

//f1.n_y= sqrt(1-f1.n_x*f1.n_x);
}
void Frame2d::set_angle(Vector v) 
{
	double a=v.getDirection();
	n_x= cosDeg(a) * scale;
	n_y= sinDeg(a) * scale;

//f1.n_y= sqrt(1-f1.n_x*f1.n_x);
}

// set new original point
void Frame2d::set_position(double x,double y) 
{
  p_x= x;
  p_y= y;
}

void Frame2d::set_position(Vector v) 
{
  p_x= v.X;
  p_y= v.Y;
}

void Frame2d::set_scale(double s) 
{
  n_x /= scale;
  n_y /= scale;
  scale= s;


  n_x *= scale;
  n_y *= scale;
}

void Frame2d::invert() 
{
  double old_p_x= p_x;

  p_x= -n_x*p_x     - n_y*p_y;
  p_y=  n_y*old_p_x - n_x*p_y;
   
  n_y= -n_y;
  
  scale = 1/scale;
}

Frame2d Frame2d::Translation(double x, double y) 
{
  Frame2d f1;
  f1.set_position(x,y);
  return f1;
}

Frame2d Frame2d::Rotation(const Angle& a) 
{
  Frame2d f1;
  f1.set_angle(a);
  return f1;
}



