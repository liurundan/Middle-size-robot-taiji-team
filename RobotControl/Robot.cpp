#include "StdAfx.h"
#include "Robot.h"
#include "Journal.h"
#include "WorldModel.h"
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

Robot::Robot (  ConfigReader& vread ) throw (RobotsException, bad_alloc) 
{
  string confline;
  
  if (vread.get("robot_type", confline)<=0) 
  {
	 JERROR("no config line \"robot_type\" found");
     throw  InvalidConfigurationException ("robot_type");
  }

  try
  {
     the_robot = RobotFactory::get_robot_factory()->get_robot (confline,vread);
  }
  catch(invalid_argument) 
  {
	 JERROR("unknown robot_type");
     throw  InvalidConfigurationException ("robot_type");
  }
  robot_descriptor = new char [confline.length()+1];
  strcpy (robot_descriptor, confline.c_str());
  
  if( WorldModel::is_main_world_model_available() ) 
  {  
     WorldModel::get_main_world_model().set_robot_properties(the_robot->get_robot_properties());
  }
  else 
  {
    JWARNING("no worldmodel available");
  }
}