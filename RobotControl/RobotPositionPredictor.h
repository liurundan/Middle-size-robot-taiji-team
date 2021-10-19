#ifndef _RobotPositionPredictor_h_
#define _RobotPositionPredictor_h_

#include "RobotLocation.h"
#include "Time.h"

class RobotPositionPredictor
{
public:
	 
    RobotPositionPredictor () throw ();
 
    void set (const RobotLocation&, Time, Time) throw ();
    
    RobotLocation get (Time) const throw ();
protected:
private:
	RobotLocation current_rloc;       
    Time timestamp_current_rloc;    
    Time timestamp_calculation;     
};
  

 

#endif