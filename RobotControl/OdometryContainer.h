#ifndef _OdometryContainer_h_
#define _OdometryContainer_h_

#include "DriveVector.h"
#include "RobotLocation.h"
#include "RingBuffer.h"
#include "Time.h"

class OdometryContainer
{
public:
	 
    OdometryContainer (unsigned int =20, double =1e6, double =1e6) throw (std::bad_alloc);
    
    ~OdometryContainer () throw ();

   
    void add_odometry (DriveVector, Time) throw ();
    
    void add_drive_vector (DriveVector, Time) throw ();

    
    RobotLocation movement (Time, Time) const throw ();
   
    RobotLocation add_movement (RobotLocation, Time, Time) const throw ();
    
    DriveVector get_odometry (Time) const throw ();
    DriveVector get_drive_vector (Time) const throw ();

protected:
private:
	struct TDV 
	{
      Time timestamp;
      Vec  vtrans;
      double vrot;
      bool   kick;
    };
    RingBuffer<TDV> odo;    
    RingBuffer<TDV> drv;   
    unsigned int n;         
    const double max_acc;     
    const double max_racc;     
};

#endif