#include "StdAfx.h"
#include "RCPlayer.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "data.h"
#include <cmath>

RCPlayer::RCPlayer( ConfigReader& cfg) throw() : motion(WorldModel::get_main_world_model().get_robot_properties())
{
	goto_pos_skill.set_dynamics(1.5, 3, 2, 2);
 	gdtj_08       = new Behavior(cfg);
	double a = 0;
	cfg.get("Goalie::attack_radius",a);
}

RCPlayer::~RCPlayer() throw()
{
 	delete gdtj_08;
}

DriveVector RCPlayer::process_drive_vector(Time t) throw()
{
 	WhiteBoard::getTheWhiteBoard()->checkMessageBoard();
    DriveVector ret;
    gdtj_08->getCmd(&ret,t);
	return ret; 
/*	
 	if( MWM.get_game_state().refstate == freePlay ) 
	{  
		DriveVector ret;  
		
 		RobotLocation rloc = MWM.get_robot_location (t);  
		BallLocation bloc  = MWM.get_ball_location (t);  
        ObstacleLocation oloc = MWM.get_obstacle_location(t);
		
        Vec    direction  = (bloc.pos-rloc.pos).normalize();
	    Angle  rbdeg      = direction.angle();
        double ballrelang = fabs( ( direction.angle() - rloc.heading ).get_deg_180() );
		
		Vec    goal_center(0.5*MWM.get_field_geometry().field_length,0);
		Vec    goal_left(0.5*MWM.get_field_geometry().field_length,800);
		Vec    goal_right(0.5*MWM.get_field_geometry().field_length,-800);
        Vec    posDest;
		Angle  goalrob;
    	Angle  goal_left_rob  =   (goal_left - rloc.pos).angle();
		Angle  goal_right_rob  =  (goal_right - rloc.pos).angle();
        if( rloc.pos.y < 0 ) 
		{
			goalrob = goal_left_rob;
			posDest= goal_left;
		}
		else
		{
			goalrob = goal_right_rob;
			posDest = goal_right;
		}

        int    way = ( goalrob - rloc.heading ).get_deg_180() > 0 ? -1 :1;
		double relang  = fabs( ( goalrob - rloc.heading ).get_deg_180() );
  
 		double distBall = rloc.pos.getDistanceTo(bloc.pos);
        
		static double dist_thred = 380;
		static double ang_thred  = 10;

		if( distBall > dist_thred || ballrelang > ang_thred )
		{
			getBall.getCmd(&ret,t);
			//goto_pos_skill.init(bloc.pos,rbdeg,0.8,false);
 			//goto_pos_skill.getCmd (&ret, t);
		}
		else
		{
			if( relang > 12.0 ) // 5.0
			{
				ret.vtrans = Vec( 0.5, 1.0*way );
				ret.vrot   = -2.0 * way;
				dist_thred = 480;
				ang_thred  = 40;
			}
			else  
			{
				if( rloc.pos.getDistanceTo(goal_center) > 2000 )
				{
					//motion.direct_approach_nostop( goal_center, goalrob, rloc, 0.8, 3.0, false, true );
					//ret = motion.get_drive_vector();
					goto_pos_skill.init(posDest, goalrob,1.2);
 			        goto_pos_skill.getCmd (&ret, t);
					dist_thred = 500;
					ang_thred  = 50;
				}
			}
		}

		if( rloc.pos.getDistanceTo(goal_center) < 2000 && 
			distBall < 560 && ballrelang < 15 && relang < 10 )
		{
			ret.kick = true;
			ret.kickballtime = 0x78;
			dist_thred = 380;
			ang_thred  = 20;
		}
 		
		return ret;
		
	}
	else 
	{
		return DriveVector (Vec(0,0),0,false);
	}  */
}

