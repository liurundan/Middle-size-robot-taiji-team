#include "StdAfx.h"
#include "Player.h"
#include "PlayerFactory.h"
#include "Journal.h"
#include <cstring>

using namespace std;

const char* Player::get_player_type () const throw () { return player_descriptor; }

Player::~Player () throw () 
{
  delete the_player;
  delete [] player_descriptor;
}

Player::Player ( ConfigReader& vread) throw (RobotsException, bad_alloc) : the_player(NULL), 
configuration_list(vread) 
{
  string confline;
  if (vread.get("player_type", confline)<=0) 
  {
    JERROR("no config line \"player_type\" found");
    throw InvalidConfigurationException ("player_type");
  }
  really_change_player_type (confline.c_str(), vread);
}

bool Player::change_player_type (const char* pt) throw () 
{
  return change_player_type (pt, configuration_list);
}

bool Player::change_player_type (const char* pt,  ConfigReader& vread) throw () 
{
  try
  {
    really_change_player_type (pt, vread);
  }
  catch(bad_alloc&)
  {
    JWARNING("Change of player type failed due to lack of memory");
    return false;
  }
  catch(RobotsException&)
  {
    JWARNING("Change of player type failed");
    return false;
  }
  return true;
}

void Player::really_change_player_type (const char* pt,  ConfigReader& vread) throw (RobotsException, bad_alloc) 
{
  PlayerType* new_player;
  char* new_descriptor;
  try
  {
    string plt (pt);
    new_player = PlayerFactory::get_player_factory()->get_player (plt, vread);
  }
  catch(invalid_argument)
  {
    throw InvalidConfigurationException ("player_type");
  }

  new_descriptor = new char [strlen(pt)+1];
  strcpy(new_descriptor,pt);
  if (the_player!=NULL) 
  {
    delete the_player;
    delete [] player_descriptor;
  }

  the_player=new_player;
  player_descriptor=new_descriptor;
}

void Player::getPlayerTypeList(std::vector<std::string> &ptl)
{
  PlayerFactory::get_player_factory()->player_list(ptl);
}
