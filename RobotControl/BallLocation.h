#ifndef _ball_location_h
#define _ball_location_h

#include "Vec.h"
#include "Time.h"
  
struct BallLocation 
{
	Vec pos;              
	Vec velocity;          

	double quality;        
	Time lastly_seen;      
	int pos_known;         
	static const int unknown;      
	static const int known;        
	static const int raised;        
	static const int communicated;  
	static const int usb_camera;
    static const int small_camera;
};
  

#endif

