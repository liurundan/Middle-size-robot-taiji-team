#ifndef _RobotFactory_h_
#define _RobotFactory_h_

#include "RobotType.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include <string>

class RobotFactory
{
public:
     static RobotFactory* get_robot_factory () throw (std::bad_alloc);

      RobotType* get_robot ( const std::string, ConfigReader& ) throw (RobotsException,std::bad_alloc,
		 std::invalid_argument);
private:
 
    static RobotFactory* the_only_factory; 
	static void destroy();
	
    RobotFactory () throw ();                             
    ~RobotFactory() throw ();       
};

#endif