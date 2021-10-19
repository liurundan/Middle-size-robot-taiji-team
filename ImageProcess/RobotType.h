#ifndef _robot_type_h
#define _robot_type_h

#include "DriveVector.h"
#include "RobotProperties.h"
#include "RobotsException.h"
#include "mscomm.h"

class RobotType
{
public:
	virtual ~RobotType () throw () {;}

    virtual RobotProperties get_robot_properties () const throw () =0;
 
    virtual void set_drive_vector (DriveVector) throw (BadHardwareException) =0;

};

#endif