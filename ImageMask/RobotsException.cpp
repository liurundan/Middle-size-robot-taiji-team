#include "StdAfx.h"
#include "RobotsException.h"
 
RobotsException::RobotsException (const char* err_type)
{
  what_string = std::string ("RobotsException: ")+std::string(err_type);
}

const char* RobotsException::what () throw () 
{
  return what_string.c_str();
}

HardwareException::HardwareException (const char* err_type) : RobotsException (err_type) {;}

BadHardwareException::BadHardwareException (const char* err_type) : RobotsException (err_type) {;}

InvalidConfigurationException::InvalidConfigurationException (const char* err_line) : RobotsException (
		(std::string ("invalid configuration line \"")+std::string(err_line)+std::string("\"")).c_str() ) {;}

