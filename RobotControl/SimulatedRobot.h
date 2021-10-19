#ifndef _SimulatedRobot_h_
#define _SimulatedRobot_h_

#include "RobotType.h"
#include "ConfigReader.h"
#include "RingBuffer.h"
#include "SimClient.h"

class SimulatedRobot : public RobotType 
{
private:
    struct TDriveVector 
	{
		Time timestamp;
		DriveVector dv;
    };
    SimClient* the_sim_client;           
    RobotProperties robot_properties;    
    RingBuffer<TDriveVector> drv;       
public:
	SimulatedRobot ( ConfigReader&) throw (std::bad_alloc,InvalidConfigurationException);
	~SimulatedRobot () throw ();
	
	RobotProperties get_robot_properties () const throw ();
	
	void set_drive_vector (DriveVector) throw ();
};


#endif


