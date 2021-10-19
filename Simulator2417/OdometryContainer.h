#ifndef _OdometryContainer_
#define _OdometryContainer_

#include "DriveVector.h"
#include "RobotLocation.h"
#include "RingBuffer.h"
#include "MyTime.h"

class OdometryContainer
{
public:
 
    OdometryContainer (unsigned int =20, double =1e6, double =1e6) throw (std::bad_alloc);
    ~OdometryContainer () throw ();

    void add_odometry (DriveVector, MyTime) throw ();
    void add_drive_vector (DriveVector, MyTime) throw ();
   
	void          Setodometry( RobotLocation rp );
	RobotLocation Getodometry() const throw(); 
 
    RobotLocation movement (MyTime, MyTime) const throw ();
  
    RobotLocation add_movement (RobotLocation, MyTime, MyTime) const throw ();
    DriveVector   get_odometry (MyTime) const throw ();
    DriveVector   get_drive_vector (MyTime) const throw ();
protected:
private:
    struct TDV 
	{
      MyTime timestamp;
      Vector vtrans;
      double vrot;
      bool   kick;
    };
    RingBuffer<TDV> odo;    
    RingBuffer<TDV> drv;    
    unsigned int n;         
    const double max_acc;    
    const double max_racc; 
	
	RobotLocation deltavalue;
};
#endif