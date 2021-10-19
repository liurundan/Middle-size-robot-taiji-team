#ifndef _RobotPositionKalmanFilter
#define _RobotPositionKalmanFilter

#include "Vec.h"
#include "RobotLocation.h"

class RobotPositionKalmanFilter
{
public:
	 
    void set (Vec, Angle, Vec, double) throw ();
  
    void update (Vec, Angle, Vec, Angle,Vec,double) throw ();
  
    void update (Vec, Angle, bool =false) throw ();
 
    double get (Vec&, Angle&) const throw ();
  
    Vec get_position_variance () const throw ();
    
    double get_heading_variance () const throw ();
     
    void mirror () throw();

private:
	Vec pos;                
    double heading;    
    Vec var_pos;            
    double var_heading;      
};

#endif