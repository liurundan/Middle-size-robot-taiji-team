#include "StdAfx.h"
#include "RobotDummy.h"
//#include "WorldModel.h"
#include <iostream>

using namespace std;

RobotDummy::RobotDummy ( ConfigReader& vr) throw () 
{
  robot_properties.read (vr);
}

RobotDummy::~RobotDummy () throw () {;}

RobotProperties  RobotDummy::get_robot_properties () const throw () 
{
  return robot_properties;
}

void RobotDummy::set_drive_vector (DriveVector dv) throw () 
{
  //Time tt;
  //WorldModel::get_main_world_model().set_drive_vector (dv, tt);
  //WorldModel::get_main_world_model().set_odometry (dv, tt);
  //std::cout << dv.vtrans.x << ' ' << dv.vtrans.y << ' ' << dv.vrot << ' ' << (dv.kick ? "kick       " : "--           ") << '\n';
}
