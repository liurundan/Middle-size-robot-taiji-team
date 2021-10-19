#ifndef _robot_h
#define _robot_h

#include <stdexcept>
#include "RobotType.h"
#include "RobotsException.h"
#include "ConfigReader.h"

class Robot
{
private:
    RobotType* the_robot;
    char* robot_descriptor;

  public:
   
    Robot (  ConfigReader&, CMSComm& m_com) throw (RobotsException, std::bad_alloc);
    ~Robot () throw ();
 
    const char* get_robot_type () const throw ();
   
    inline void set_drive_vector (DriveVector dv) 
		throw (BadHardwareException) {  the_robot->set_drive_vector(dv); }
     
    void emergency_stop () throw ();
};

 

#endif

