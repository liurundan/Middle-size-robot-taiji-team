#ifndef _OMNIROBOT_H
#define _OMNIROBOT_H

#include "RobotType.h"
#include "ConfigReader.h"
#include <iostream>
#include "Time.h"
#include "ConfigReader.h"
  
class OmniRobot : public RobotType
{
public:
    OmniRobot(  ConfigReader& vr, CMSComm& m_com ) throw(RobotsException, std::bad_alloc);
    ~OmniRobot() throw();

    RobotProperties get_robot_properties () const throw ();
    
	void InitializeComm(ConfigReader &vr,CMSComm& m_com) throw(RobotsException);
    void set_drive_vector (DriveVector dv) throw ();
    
protected:
 	CByteArray			m_command;
	
	bool				blocking_communication;
  
    bool				amIKicking;
    Time				lastKickStart;
    Time				lastKickEnd;

    DriveVector			lastDriveState;
    Time				lastDriveStateTime;

    RobotProperties		robot_properties;
    BYTE				kickBallTime;


    
};
 
#endif
