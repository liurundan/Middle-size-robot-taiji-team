#ifndef  _SLStuckSensor_h
#define  _SLStuckSensor_h

#include "Time.h"
#include "RingBuffer.h"
#include "ConfigReader.h"
#include "RobotLocation.h"

class SLStuckSensor
{
public:
	 SLStuckSensor (  ConfigReader&) throw (std::bad_alloc);
    ~SLStuckSensor () throw ();
    
    void update () throw ();
  
    RobotStuckLocation get_stuck_location (Vec, Vec) const throw ();
protected:
private:
	struct TPOS 
	{
      Time timestamp;
      RobotLocation rloc;
      double deriv;                    
    };

    const unsigned int n;             
    RingBuffer<TPOS> oldpos;           

    unsigned int count;                
    Time timestamp_latest_stuck;        
    Vec pos_latest_stuck;             
    Vec dir_latest_stuck;               

    unsigned int stuck_min;             
    bool was_stuck;                  
};


#endif
