#ifndef _robot_location_h
#define _robot_location_h

#include "Angle.h"
#include "Vec.h"
 
struct RobotStuckLocation
{
	bool robot_stuck;                
	unsigned int msec_since_stuck;    
	Vec pos_of_stuck;                 
	Vec dir_of_stuck;                
	inline bool operator() () const throw () { return robot_stuck; }   
};
 
struct RobotLocation 
{
	Vec    pos;                  
	Angle  heading;            
	Vec    vtrans;                
	double vrot;             

	double quality;           

	bool   kick;                

	RobotStuckLocation stuck;  
};
 
#endif
