#ifndef Vec_h
#define Vec_h

#include "Angle.h"
#include <iostream>
  
class Vec
{ 
public:
	double x;    
	double y;     

	Vec () throw () { x=y=0.;}
	Vec (double x1, double y1) throw () : x(x1), y(y1) {;}
	Vec (const Vec& v) throw () : x(v.x), y(v.y) {;}
	const Vec& operator= (const Vec& v) { x=v.x; y=v.y; return *this; }

	bool operator== (const Vec) const throw ();
	bool operator!= (const Vec) const throw ();

	Vec operator+ (const Vec) const throw ();
	const Vec& operator+= (const Vec) throw ();
	Vec operator- (const Vec) const throw ();
	const Vec& operator-= (const Vec) throw ();
	Vec operator- () const throw ();
	const Vec& operator*= (double) throw ();
	const Vec& operator/= (double) throw ();    
	double operator* (const Vec) const throw ();

	Vec operator* (const Angle) const throw ();
	const Vec& operator*= (const Angle) throw ();
	Vec operator/ (const Angle) const throw ();
	const Vec& operator/= (const Angle) throw ();

	Vec rotate (const Angle) const throw ();       
	Vec rotate_twelvth () const throw ();          
	Vec rotate_eleven_twelvth () const throw ();   
	Vec rotate_eighth () const throw ();           
	Vec rotate_seven_eighth () const throw ();      
	Vec rotate_sixth () const throw ();             
	Vec rotate_five_sixth () const throw ();        
	Vec rotate_quarter () const throw ();           
	Vec rotate_three_quarters () const throw ();   
	Vec rotate_half () const throw ();             

	Vec s_rotate (const Angle) throw ();           

	Vec mirror (const Vec) const throw ();          
	Vec mirror_x () const throw ();                 
	Vec mirror_y () const throw ();                
	Vec mirror_eighth () const throw ();           
	Vec mirror_three_eighth () const throw ();      
         
	double squared_length () const throw ();
	double length () const throw ();
	Angle angle () const throw ();
	Vec normalize () const throw ();

	Angle angle (const Vec) const throw ();

	static Vec unit_vector (Angle) throw ();

	static const Vec unit_vector_x;            
	static const Vec unit_vector_y;            
	static const Vec zero_vector;             

};

Vec operator* (Vec, double) throw ();
Vec operator* (double, Vec) throw ();
Vec operator/ (Vec, double) throw ();     

bool linearly_independent (const Vec, const Vec);
   
   

std::ostream& operator<< (std::ostream& os, const Vec& v);

#endif
