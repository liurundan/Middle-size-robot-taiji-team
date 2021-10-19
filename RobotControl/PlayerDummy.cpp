#include "StdAfx.h"
#include "PlayerDummy.h"
#include "WorldModel.h"
#include "GameState.h"

using namespace std;

PlayerDummy::PlayerDummy ( ConfigReader& reader) throw ():
motion(WorldModel::get_main_world_model().get_robot_properties()) 
 
{
  vtrans=Vec(0,0);
  vrot=0;
  for (int i=0; i<4; i++) vaux[i] = 0;
  dv_mode = ROBOTVELOCITY;

  int h;
  if (reader.get("PlayerDummy::DriveVectorMode", h)) 
  {
    switch (h) 
	{
    case 0: dv_mode = ROBOTVELOCITY; break;
    case 1: dv_mode = WHEELVELOCITY; break;
    case 2: dv_mode = MOTORVOLTAGE; break;
    default:
      dv_mode = ROBOTVELOCITY;
    }
  }

  double dTmp = 0.0;
  if( reader.get("PlayerDummy::vtrans_x", dTmp)>0) 
    vtrans.x = dTmp;
  if( reader.get("PlayerDummy::vtrans_y", dTmp)>0) 
    vtrans.y = dTmp;

  vrot = 0.6;
  if (reader.get("PlayerDummy::vrot", dTmp)> 0)
    vrot=dTmp;

}

DriveVector PlayerDummy::process_drive_vector (Time tt) throw () 
{
  MWM.get_message_board().publish("Dummy an Erde");
  MWM.get_message_board().publish("<Spielzug><Was>Pass</Was><Wo>x=1000 y=2000</Wo></Spielzug>");

  if (MWM.get_game_state().refstate==stopRobot)
    return DriveVector (Vec(0,0),0.0,false);
  else 
  {
    if (dv_mode == ROBOTVELOCITY)
      return DriveVector (vtrans,vrot,false);
    return DriveVector(vaux[0], vaux[1], vaux[2],vaux[3], false, dv_mode);
  }
}

