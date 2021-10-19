#ifndef _SLVelocitySensor_h
#define _SLVelocitySensor_h

#include "RobotLocation.h"
#include "RingBuffer.h"
#include "Time.h"

class SLVelocitySensor
{
public:
    SLVelocitySensor (unsigned int =10) throw (std::bad_alloc);
    void update (const RobotLocation&, Time) throw ();
   
    const RobotLocation& get (Time&) const throw ();
protected:
private: 
    struct TPH
	{
      Time timestamp;
      Vec pos;
      double heading;  // in rad
    };

    const unsigned int n;     
    unsigned int burn_in;     
    RobotLocation model;    
    Time timestamp;           
    RingBuffer<TPH> buffer;   
};

 

#endif

