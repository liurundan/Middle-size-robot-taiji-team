#include "StdAfx.h"
#include "JoystickPlayer.h"
#include "Journal.h"
#include "WorldModel.h"
#include <cmath>

JoystickPlayer::JoystickPlayer( ConfigReader&) throw (InvalidConfigurationException, std::bad_alloc) :
motion(WorldModel::get_main_world_model().get_robot_properties()) 
{
	 ;
}

JoystickPlayer::~JoystickPlayer()
{
	;
}

DriveVector JoystickPlayer::process_drive_vector (Time t) throw () 
{
	DriveVector dv;
	
	if( MWM.get_game_state().refstate == freePlay ) 
	{  // check whether it's free play
		RobotLocation rloc = MWM.get_robot_location (t);  // in global coordinates
		BallLocation bloc  = MWM.get_ball_location (t);  // in global coordinates
		Vec direction = (bloc.pos-rloc.pos).normalize();
		double ang = (direction.angle()-rloc.heading).get_rad_pi(); //+Angle::quarter
		dv.vtrans = Vec::zero_vector;//0.3*direction.rotate(-rloc.heading); // drive with 1.0m/s in direction of the ball
		dv.vrot = 0.;//-1.3*ang; // rotate towards the ball (simple P-controller)
		if( ( bloc.pos-rloc.pos ).length() < 400 ) // kick if the ball is near
			dv.kick=true;
		else
			dv.kick=false;
		 
	}
	else 
	{
		 dv =  DriveVector (Vec(0,0),0,false);
	} 
	
	
	RobotLocation no_good;
	motion.set_drive_vector (dv, no_good);
	motion.avoid_abrupt_changes ();
	dv = motion.get_drive_vector ();
	
    return dv;
}