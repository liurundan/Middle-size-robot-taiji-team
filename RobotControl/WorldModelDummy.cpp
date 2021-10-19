#include "StdAfx.h"
#include "WorldModelDummy.h"

#include "Journal.h"

#include "WorldModel.h"

using namespace std;

WorldModelDummy::WorldModelDummy ( ConfigReader& vr) : field_geometry (vr), null_stream ("/dev/null")  
{
	game_state.in_game  = false;
	game_state.refstate = stopRobot;
	bool b = false;
	double temp = 0.0;
	if( vr.get ("WorldModelDummy::robot_pos_x", temp) >0 ) 
	{
		robot_loc.pos.x = temp;
	}
	if( vr.get ("WorldModelDummy::robot_pos_y", temp) >0 ) 
	{
		robot_loc.pos.y = temp;
	}
	if( vr.get ("WorldModelDummy::robot_heading", temp) >0 ) 
	{
		robot_loc.heading.set_deg(temp);
	}
	if( vr.get ("WorldModelDummy::robot_vtrans_x", temp) >0 ) 
	{
		robot_loc.vtrans.x = temp;
	}
	if( vr.get ("WorldModelDummy::robot_vtrans_y", temp) >0 ) 
	{
		robot_loc.vtrans.y = temp;
	}
	if( vr.get ("WorldModelDummy::robot_vrot", temp) > 0 ) 
	{
		robot_loc.vrot = temp;
	}

	if( vr.get ("WorldModelDummy::ball_pos_x", temp) >0 ) 
	{
		ball_loc.pos.x = temp;
	}
	if( vr.get ("WorldModelDummy::ball_pos_y", temp) >0 ) 
	{
		ball_loc.pos.y = temp;
	}
	if( vr.get ("WorldModelDummy::ball_vel_x", temp) >0 ) 
	{
		ball_loc.velocity.x = temp;
	}
	if( vr.get ("WorldModelDummy::ball_vel_y", temp) > 0 ) 
	{
		ball_loc.velocity.y = temp;
	}
	if( vr.get ("WorldModelDummy::kick", b)>=1)
		robot_loc.kick = b;
 
	/*
 	ball_loc.pos = Vec (-900, -3000);
	ball_loc.velocity = Vec (0, -5);
	ball_loc.quality=0.8;
	ball_loc.pos_known=BallLocation::known;

	robot_loc.pos = Vec (-200, -5803.15);
	robot_loc.heading.set_deg (0);
	robot_loc.vtrans = Vec (0, 0);
	robot_loc.vrot =0;


	robot_loc.kick = false;*/
}

WorldModelDummy::~WorldModelDummy () throw () {;}

void WorldModelDummy::startstop (bool b) throw () 
{
  game_state.in_game=b;
}

void WorldModelDummy::update_refbox (RefboxSignal sig) throw () 
{
  if (sig== SIGstop)
    game_state.refstate =  stopRobot;
  else if (sig== SIGstart)
    game_state.refstate =  freePlay;
}
