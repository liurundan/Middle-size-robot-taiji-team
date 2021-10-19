#include "StdAfx.h"
#include "RobotProperties.h"

void RobotProperties::read(  ConfigReader& vr ) throw() 
{
  double u;
  int i;
  if (vr.get("max_velocity", u)>0 && u>0)
    max_velocity=u;
  if (vr.get("max_acceleration", u)>0 && u>0)
    max_acceleration=u;
  if (vr.get("max_deceleration", u)>0 && u>0)
    max_deceleration=u;
  if (vr.get("max_rotational_velocity", u)>0 && u>0)
    max_rotational_velocity=u;
  if (vr.get("max_rotational_acceleration", u)>0 && u>0)
    max_rotational_acceleration=u;
  if (vr.get("max_rotational_deceleration", u)>0 && u>0)
    max_rotational_deceleration=u;
  if (vr.get("max_robot_radius", u)>0 && u>0)
    max_robot_radius=u;
  if (vr.get("min_robot_radius", u)>0 && u>0)
    min_robot_radius=u;
  if (vr.get("robot_width", u)>0 && u>0)
    robot_width=u;
  if (vr.get("robot_length", u)>0 && u>0)
    robot_length=u;
  if (vr.get("kicker_width", u)>0 && u>0)
    kicker_width=u;
  if (vr.get("kicker_distance", u)>0 && u>0)
    kicker_distance=u;
  if (vr.get("drive_vector_delay", i)>0)
    drive_vector_delay=i;
}