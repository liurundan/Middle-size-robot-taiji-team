#ifndef robots_exception_h
#define robots_exception_h

#include <exception>
#include <string>

class RobotsException: public std::exception
{
  public:
	 RobotsException (const char*);
    ~RobotsException () throw () {;}

     virtual const char* what () throw();
  protected: 
	std::string what_string;
  private:
};

class HardwareException : public RobotsException
{
  protected:
	std::string config_argument;   
  public:    
	 HardwareException (const char*);
	~HardwareException () throw () {;}
};

class BadHardwareException : public RobotsException 
{
  public:
 	BadHardwareException (const char*);
	~BadHardwareException () throw () {;}
};

class InvalidConfigurationException : public RobotsException 
{
  protected:
	std::string config_argument;   ///< die fehlerhafte Konfigurationsangabe
  public:    
	 InvalidConfigurationException (const char*);
	~InvalidConfigurationException () throw () {;}
};
 
#endif
