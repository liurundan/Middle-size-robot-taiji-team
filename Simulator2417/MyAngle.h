#ifndef _MyAngle_
#define _MyAngle_


#define M_PI     3.14159265358979323846
 
class MyAngle 
{
  private:
    double the_angle;   
  public:
     MyAngle () throw () { the_angle = 0.;}
     MyAngle (double a) throw () {set_rad (a);}
     MyAngle (const MyAngle& a) throw () : the_angle(a.the_angle) {;}
     const MyAngle& operator= (const MyAngle& a) throw () { the_angle=a.the_angle; return *this; }
    
     bool operator== (const MyAngle&) const throw ();
     bool operator!= (const MyAngle&) const throw ();
     bool in_between (const MyAngle, const MyAngle) const throw ();

     void set_rad (double) throw ();
     double get_rad () const throw ();

     static MyAngle rad_angle (double) throw ();
     static MyAngle deg_angle (double) throw ();
    
     void set_deg (double) throw ();
     double get_deg () const throw ();

     double get_rad_pi () const throw ();
     double get_deg_180 () const throw ();

     MyAngle operator+ (const MyAngle) const throw ();
     const MyAngle& operator+= (const MyAngle) throw ();
     MyAngle operator- (const MyAngle) const throw ();
     const MyAngle& operator-= (const MyAngle) throw ();
     MyAngle operator- () const throw ();
     const MyAngle& operator*= (double) throw();
    
    static const MyAngle zero;           ///< 0 Grad
    static const MyAngle twelvth;        ///< 30 Grad
    static const MyAngle eighth;         ///< 45 Grad
    static const MyAngle sixth;          ///< 60 Grad
    static const MyAngle quarter;        ///< 90 Grad
    static const MyAngle three_eighth;   ///< 135 Grad
    static const MyAngle half;           ///< 180 Grad
    static const MyAngle five_eighth;    ///< 225 Grad
    static const MyAngle three_quarters; ///< 270 Grad
    static const MyAngle five_sixth;     ///< 300 Grad
    static const MyAngle seven_eighth;   ///< 315 Grad
    static const MyAngle eleven_twelvth; ///< 330 Grad
  };

   MyAngle operator* (const MyAngle, double) throw ();
   MyAngle operator* (double, const MyAngle) throw ();
  
#endif 
 
