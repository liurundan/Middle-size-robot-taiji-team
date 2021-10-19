#include "StdAfx.h"
#include "WorldModelFactory.h"
#include "ErrorMinimiserWorldModel.h"
#include "WorldModelDummy.h"
#include "AddWriteWorldModel.h"
#include "SimulatorWorldModel.h"

using namespace std;

WorldModelFactory*  WorldModelFactory::the_only_factory = NULL;

WorldModelFactory::WorldModelFactory () throw () {;}

WorldModelFactory* WorldModelFactory::get_world_model_factory () throw (std::bad_alloc) 
{
  if (!the_only_factory)
  {
    the_only_factory = new WorldModelFactory;
	::atexit(destroy);
  }
  return the_only_factory;
}

WorldModelFactory::~WorldModelFactory() throw () {;}

 
WorldModelType* WorldModelFactory::get_world_model (const std::string descriptor,  ConfigReader& reader) 
throw (RobotsException,bad_alloc,invalid_argument) 
{
  
	WorldModelType* new_wm = NULL;
	string sTmp = descriptor;
	if( sTmp == "ErrorMinimiser" )
	{
	  new_wm = new ErrorMinimiserWorldModel(reader);
	}
    else if (sTmp == "Dummy")
	{
	  new_wm = new WorldModelDummy (reader);
    }
	else if (sTmp == "Simulator" )
	{
		new_wm = new SimulatorWorldModel(reader);
		
	}
	bool b;
    if (reader.get("add_write_world_model", b)>0 && b) 
	{
      new_wm = new AddWriteWorldModel(reader, new_wm);
	} 
	
	return new_wm;
}

void WorldModelFactory::destroy()
{
   if( the_only_factory != NULL)
   {
	   delete the_only_factory;
	   the_only_factory = NULL;
   }
}