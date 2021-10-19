#ifndef _robot_data_h
#define _robot_data_h

#define MAX_IDSTR_LEN	200
#define READLEN			8

#include "DriveVector.h"

struct RobotData 
{
	RobotData() throw ();
	RobotData (const RobotData&) throw ();
	const RobotData& operator= (const RobotData&) throw ();

	char		robotIdString[MAX_IDSTR_LEN];
	int			BoardID; 
	bool		motors_on;
	float		wheel_vel[4];
	float		robot_vel[4];
	float		motor_current[4];
	float		motor_output[4];
	DriveVector dv_set;
	bool		motor_temp_switch[4];
	float		motor_temp[4];
	float		motor_vcc;
};
 
#endif