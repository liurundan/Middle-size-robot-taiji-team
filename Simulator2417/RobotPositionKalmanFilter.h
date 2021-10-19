#ifndef _RobotPositionKalmanFilter_
#define _RobotPositionKalmanFilter_
 
#include "Geometry.h"
#include "RobotLocation.h"

class RobotPositionKalmanFilter
{
public:
 
    void set (Vector, MyAngle, Vector, double) throw ();
 
    void update (Vector, MyAngle, Vector, MyAngle,Vector,double) throw ();
 
    void update (Vector, MyAngle, bool =false) throw ();
 
    double get (Vector&, MyAngle&) const throw ();
    Vector get_position_variance () const throw ();
    double get_heading_variance () const throw ();
    void mirror () throw();
protected:
private:
	Vector pos;                //  in mm
    double heading;            // (-pi, pi]
    Vector var_pos;            //  in mm^2
    double var_heading;        //  in rad^2
};

#endif