#include "StdAfx.h"
#include "RobotData.h"

#include <stdio.h>
RobotData::RobotData() throw ()
{
  sprintf(robotIdString,"NOT DEFINED");
  BoardID    = -1;
  motors_on  = true;
  
  for (int i=0; i<4; i++)
  {
      wheel_vel[i]         = 0;
      motor_current[i]     = 0;
	  robot_vel[i]         = 0;
      motor_output[i]      = 0;
      motor_temp_switch[i] = 0;
      motor_temp[i]        = 0;
  }
  
  motor_vcc = 0;
  dv_set    = DriveVector();
}

RobotData::RobotData (const RobotData& src) throw ()
{
  sprintf(robotIdString,"%s", src.robotIdString);
  BoardID    = src.BoardID;
  motors_on  = src.motors_on;
  
  for (int i=0; i<4; i++)
  {
      wheel_vel[i]         = src.wheel_vel[i];
	  robot_vel[i]         = src.robot_vel[i];
      motor_current[i]     = src.motor_current[i];
      motor_output[i]      = src.motor_output[i];
      motor_temp_switch[i] = src.motor_temp_switch[i];
      motor_temp[i]        = src.motor_temp[i];
  }
  motor_vcc = src.motor_vcc;
  dv_set    = src.dv_set;
}

const RobotData& RobotData::operator= (const RobotData& src) throw ()
{
  sprintf(robotIdString,"%s", src.robotIdString);
  BoardID    = src.BoardID;
  motors_on  = src.motors_on;
  
  for (int i=0; i<4; i++)
  {
      wheel_vel[i]         = src.wheel_vel[i];
	  robot_vel[i]         = src.robot_vel[i];
      motor_current[i]     = src.motor_current[i];
      motor_output[i]      = src.motor_output[i];
      motor_temp_switch[i] = src.motor_temp_switch[i];
      motor_temp[i]        = src.motor_temp[i];
  }

  motor_vcc = src.motor_vcc;
  dv_set    = src.dv_set;

  return *this;
}

