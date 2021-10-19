#ifndef _WorldModelFactory_h_
#define _WorldModelFactory_h_

#include "WorldModelType.h"
#include "ConfigReader.h"
#include <string>

class WorldModelFactory
{
public:
	 
    static WorldModelFactory* get_world_model_factory () throw (std::bad_alloc);
   
    WorldModelType* get_world_model (const std::string,  ConfigReader&) 
		throw (RobotsException,std::bad_alloc,std::invalid_argument);
   
protected:
private:
	static void destroy();
    static WorldModelFactory* the_only_factory;                 
    WorldModelFactory () throw ();                             
    ~WorldModelFactory() throw ();            
};

#endif