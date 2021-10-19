#ifndef _update_robot_location_h
#define _update_robot_location_h

#include "RobotLocation.h"
#include "BallLocation.h"
#include "ObstacleLocation.h"

RobotLocation update_robot_location (const RobotLocation&, double) throw ();

RobotLocation flip_sides (const RobotLocation&, int =-1) throw ();

BallLocation flip_sides (const BallLocation&, int =-1) throw ();

ObstacleLocation flip_sides (const ObstacleLocation&, int =-1) throw ();
  
#endif