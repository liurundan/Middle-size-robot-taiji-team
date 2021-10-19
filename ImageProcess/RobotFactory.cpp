#include "StdAfx.h"
#include "RobotFactory.h"

using namespace std;

RobotFactory* RobotFactory::the_only_factory = NULL;

RobotFactory::RobotFactory () throw () {;}

RobotFactory* RobotFactory::get_robot_factory () throw (std::bad_alloc) 
{
  if (!the_only_factory)
    the_only_factory = new RobotFactory;
  return the_only_factory;
}

RobotFactory::~RobotFactory() throw () {;}
  
RobotType* RobotFactory::get_robot (const std::string descriptor, ConfigReader& reader,CMSComm& m_com) 
throw (RobotsException,bad_alloc,invalid_argument) 
{
   RobotType* new_wm = NULL;
   string sTmp = descriptor;
   if( sTmp == "OmniRobot" )
   {
	  new_wm = new OmniRobot(reader,m_com);
   }

   return new_wm;
}
 
