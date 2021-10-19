#ifndef  robot_location_h
#define  robot_location_h

#include "MyAngle.h"
#include "Geometry.h"

struct RobotStuckLocation 
{
    bool         robot_stuck;                
    unsigned int msec_since_stuck;    
    Vector       pos_of_stuck;                 
    Vector       dir_of_stuck;                
    inline bool operator() () const throw () { return robot_stuck; }   
};

struct RobotLocation 
{
	Vector       pos;                 
    MyAngle        heading;            
    Vector       vtrans;              
    double       vrot;              
    double       quality;           
    bool         kick;                

    RobotStuckLocation stuck; 
};

#endif