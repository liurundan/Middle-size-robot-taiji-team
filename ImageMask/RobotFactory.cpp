#include "StdAfx.h"
#include "RobotFactory.h"
#include "OmniRobot.h"
#include "RobotDummy.h"

using namespace std;

RobotFactory* RobotFactory::the_only_factory = NULL;

RobotFactory::RobotFactory () throw () {;}

RobotFactory* RobotFactory::get_robot_factory () throw (std::bad_alloc) 
{
  if (!the_only_factory)
  {
    the_only_factory = new RobotFactory;
	::atexit(destroy);
  }
  return the_only_factory;
}

RobotFactory::~RobotFactory() throw () {;}
  
RobotType* RobotFactory::get_robot (const std::string descriptor, ConfigReader& reader) 
throw (RobotsException,bad_alloc,invalid_argument) 
{
   RobotType* new_wm = NULL;
   string sTmp = descriptor;
   if( sTmp == "OmniRobot" )
   {
	  new_wm = new OmniRobot(reader);
   }
   else if (sTmp == "Dummy") 
   {
	  new RobotDummy (reader);
   }

   return new_wm;
}

void RobotFactory::destroy()
{
   if( the_only_factory != NULL)
   {
	   delete the_only_factory;
	   the_only_factory = NULL;
   }
}
 
