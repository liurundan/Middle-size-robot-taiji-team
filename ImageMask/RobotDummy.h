#ifndef _robot_dummy_h
#define _robot_dummy_h

#include "RobotType.h"
//#include "WorldModel.h"
#include "ConfigReader.h"
#include <iostream>

class RobotDummy:public RobotType
{
public:
    RobotDummy ( ConfigReader& vr) throw ();
    ~RobotDummy () throw ();
    RobotProperties get_robot_properties () const throw ();
    void set_drive_vector (DriveVector) throw ();

private:
    RobotProperties robot_properties;

};

#endif