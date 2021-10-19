#include "StdAfx.h"
#include "AddJoystickPlayer.h"
#include "WorldModel.h"
#include "GameState.h"
#include "RobotsUdpServer.h"

AddJoystickPlayer::AddJoystickPlayer (  ConfigReader& vr, PlayerType* pl) 
throw (InvalidConfigurationException, std::bad_alloc) : JoystickPlayer (vr), the_elementary_player (pl) 
{
  if(!pl)
    throw InvalidConfigurationException ("player_type");
  start_button = 1;
  stop_button  = 0;
  
}

AddJoystickPlayer::~AddJoystickPlayer () throw () 
{
  delete the_elementary_player;
}

DriveVector AddJoystickPlayer::process_drive_vector (Time t) throw () 
{
  DriveVector dest = the_elementary_player->process_drive_vector (t); 

  // if there is a communication take the remote control Vector for joystick control
  if (RobotsUdpServer::hasInstance() && MWM.get_game_state().refstate==stopRobot) 
  {
    RobotsUdpCommunication * com = RobotsUdpServer::get_instance_pointer();
    com->getRemoteCtr(remoteCtrDrv);
    dest = remoteCtrDrv;
  } 
 
  return dest;
}

