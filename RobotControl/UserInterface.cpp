#include "StdAfx.h"
#include "Journal.h"
#include "UserInterface.h"

#include "TerminalUserInterface.h"
#include "AddComUserInterface.h"

UserInterface::UserInterface( ConfigReader& conf, Player& pl, ImageProcessing& ip, CDC& dc) throw 
  (RobotsException, std::bad_alloc)
{
  
  bool useComUserInterface = false;
  if (conf.get("add_com_user_interface", useComUserInterface) <=0)
  {
    JERROR("no config line \"add_com_user_interface\" found");
    throw InvalidConfigurationException ("add_com_user_interface");
  }
  if (useComUserInterface)
    the_user_interface = new AddComUserInterface( conf, pl, ip, dc );
  else
  {
      string confline;
      if (conf.get("user_interface_type", confline)<=0) 
	  {
		JERROR("no config line \"user_interface_type\" found");
		throw InvalidConfigurationException ("user_interface_type");
      }
      if (confline=="TerminalUserInterface")
	  {
		  the_user_interface = new TerminalUserInterface( conf, pl, ip,dc );
	  } 
      else 
	  { 
		  JERROR("No UserInterfaceType of this type found");
		  throw InvalidConfigurationException ("user_interface_type");
	  } 
  }
}

UserInterface::~UserInterface() throw ()
{
  delete the_user_interface;
}

