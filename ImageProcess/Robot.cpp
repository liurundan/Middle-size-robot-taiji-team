#include "StdAfx.h"
#include "Robot.h"

#include "RobotFactory.h"
#include <cstring>

using namespace std;

const char* Robot::get_robot_type () const throw () { return robot_descriptor; }

void Robot::emergency_stop () throw () 
{
  DriveVector dv (Vec(0,0),0,false);
  set_drive_vector (dv);
}

Robot::~Robot () throw () 
{
  delete the_robot;
  delete [] robot_descriptor;
}

Robot::Robot (  ConfigReader& vread, CMSComm& m_com) throw (RobotsException, bad_alloc) 
{
  string confline;
  
  if (vread.get("robot_type", confline)<=0) 
  {
     throw  InvalidConfigurationException ("robot_type");
  }

  try
  {
    the_robot = RobotFactory::get_robot_factory()->get_robot (confline,vread,m_com);
  }
  catch(invalid_argument) 
  {
     throw  InvalidConfigurationException ("robot_type");
  }
  robot_descriptor = new char [confline.length()+1];
  strcpy (robot_descriptor, confline.c_str());
  
}