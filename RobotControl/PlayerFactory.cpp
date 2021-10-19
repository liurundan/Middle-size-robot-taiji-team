#include "StdAfx.h"
#include "PlayerFactory.h"
#include "PlayerDummy.h"
#include "RCPlayer.h"
#include "Goalie.h"
#include "JoystickPlayer.h"
#include "AddGotoPosPlayer.h"
#include "AddJoystickPlayer.h"

using namespace std;

PlayerFactory* PlayerFactory::the_only_factory = NULL;

PlayerFactory::PlayerFactory () throw () {;}

PlayerFactory* PlayerFactory::get_player_factory () throw (std::bad_alloc) 
{
  if (!the_only_factory)
  {
    the_only_factory = new PlayerFactory;
	::atexit(destroy);
  }

  return the_only_factory;
}

PlayerFactory::~PlayerFactory() throw () {;}


PlayerType* PlayerFactory::get_player (const std::string descriptor, 
									   ConfigReader& reader) 
									   throw (RobotsException,bad_alloc,invalid_argument) 
{
  PlayerType* new_wm = NULL;
  string stmp = descriptor;
  if( descriptor == "Dummy" )
  {
    new_wm =  new PlayerDummy(reader);
  }
  else if (descriptor == "RCPlayer")
  {
	 new_wm = new RCPlayer(reader);
  }
  else if (descriptor == "Goalie") 
  {
  	 new_wm = new Goalie(reader);
  } 
  else if (descriptor == "JoystickPlayer") 
  {
     new_wm = new JoystickPlayer(reader);
  }
  else
	 throw invalid_argument (string("unknown player type ")+descriptor);
 
  bool b = false;
  if (reader.get("add_joystick_player", b)>0 && b) 
  {
    new_wm = new AddJoystickPlayer( reader, new_wm  );
  } 
   
  new_wm = new AddGotoPosPlayer (reader, new_wm);

  return new_wm;
}

void PlayerFactory::player_list (std::vector<std::string>& list) const throw (std::bad_alloc) 
{
  unsigned int sz = 4;
  list.resize (sz);
  
  list[0] = "Dummy";
  list[1] = "Goalie";
  list[2] = "RCPlayer";
  list[3] = "JoystickPlayer";

}

void PlayerFactory::destroy()
{
   if( the_only_factory != NULL)
   {
	   delete the_only_factory;
	   the_only_factory = NULL;
   }
}