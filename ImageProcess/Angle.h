#ifndef Angle_h
#define Angle_h
  	
class Angle 
{
private:
	double the_angle;    
	public:

	Angle () throw () { the_angle = 0.;}

	Angle (double a) throw () {set_rad (a);}

	Angle (const Angle& a) throw () : the_angle(a.the_angle) {;}

	const Angle& operator= (const Angle& a) throw () { the_angle=a.the_angle; return *this; }


	bool operator== (const Angle&) const throw ();
	bool operator!= (const Angle&) const throw ();
	bool in_between (const Angle, const Angle) const throw ();

	void set_rad (double) throw ();
	double get_rad () const throw ();

	static Angle rad_angle (double) throw ();
	static Angle deg_angle (double) throw ();

	void set_deg (double) throw ();
	double get_deg () const throw ();

	double get_rad_pi () const throw ();
	double get_deg_180 () const throw ();

	Angle operator+ (const Angle) const throw ();
	const Angle& operator+= (const Angle) throw ();
	Angle operator- (const Angle) const throw ();
	const Angle& operator-= (const Angle) throw ();
	Angle operator- () const throw ();
	const Angle& operator*= (double) throw();

	static const Angle zero;           ///< 0 Grad
	static const Angle twelvth;        ///< 30 Grad
	static const Angle eighth;         ///< 45 Grad
	static const Angle sixth;          ///< 60 Grad
	static const Angle quarter;        ///< 90 Grad
	static const Angle three_eighth;   ///< 135 Grad
	static const Angle half;           ///< 180 Grad
	static const Angle five_eighth;    ///< 225 Grad
	static const Angle three_quarters; ///< 270 Grad
	static const Angle five_sixth;     ///< 300 Grad
	static const Angle seven_eighth;   ///< 315 Grad
	static const Angle eleven_twelvth; ///< 330 Grad
};

Angle operator* (const Angle, double) throw ();
Angle operator* (double, const Angle) throw ();
 
#endif