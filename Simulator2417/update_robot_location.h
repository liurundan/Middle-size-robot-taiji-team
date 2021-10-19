#ifndef _update_robot_location_h
#define _update_robot_location_h

#include "RobotLocation.h"

  RobotLocation update_robot_location (  RobotLocation&, double) throw ();

     
  RobotLocation flip_sides (const RobotLocation&, int =-1) throw ();

#endif