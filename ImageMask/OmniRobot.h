#ifndef _OMNIROBOT_H
#define _OMNIROBOT_H

#include "RobotType.h"
#include "ConfigReader.h"
//#include "WorldModel.h"
#include "SerialPort.h"
#include "Time.h"
#include <iostream>

#ifndef READLEN
#define     READLEN      8    // if READLEN is changed, in robotdata.h 
#endif 
 
#define     SENDLEN      6    // there is another constant has to be changed

class OmniRobot : public RobotType
{
public:
    OmniRobot(  ConfigReader& vr) throw(RobotsException, std::bad_alloc);
    ~OmniRobot() throw();

    RobotProperties get_robot_properties () const throw ();
    
    void set_drive_vector (DriveVector dv) throw ();
//    void fill_robot_data( const BYTE* src, const DriveVector &dv_set, RobotData &target, Time &target_time );
	
protected:
	CSerialPort*        ctrRobot;
    BYTE*				m_command;
	BYTE*				readbuffer;
    
    double              v[4];
    double              wheel_vel[4];
    double				rece_dsp_data[READLEN]; 

	bool				blocking_communication;
  
    bool				amIKicking;
    Time				lastKickStart;
    Time				lastKickEnd;

    DriveVector			lastDriveState;
    Time				lastDriveStateTime;

    RobotProperties		robot_properties;
    BYTE				kickBallTime;
private:
    
};
 
#endif
