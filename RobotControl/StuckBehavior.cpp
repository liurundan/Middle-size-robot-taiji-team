#include "StdAfx.h"
#include "StuckBehavior.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "BasicFunctions.h"
#include "random.h"
#include <cmath>

StuckBehavior::StuckBehavior()
{
	get_ball	= new BallGet();
	intercept	= new InterceptBall();
	go2pos		= new SPhysGotoPos();
	go_pos      = new GotoPos();
	avoid		= new Avoid();
	basic_cmd	= new BasicCmd();
	move_with_ball = new MoveWithBall();
	face        = new FaceBall();
}

StuckBehavior::~StuckBehavior()
{
	delete go_pos;
	delete get_ball;
	delete basic_cmd;
	delete avoid;
	delete intercept;
	delete move_with_ball;
	delete go2pos;
	delete face;
}

void StuckBehavior::getCmd(DriveVector *dv, const Time& t)
{
	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t); 
	RobotStuckLocation rsloc = rloc.stuck;
	bool bstuck = rsloc.robot_stuck;

	Vec  dir    = rsloc.dir_of_stuck.rotate_half().normalize();
	Vec  dir_left = dir.rotate(-0.9).normalize();
	Vec  dir_right = dir.rotate(0.9).normalize();
	int  side = urandom() > 0.5 ? -1 : 1;
	//dir  = urandom() > 0.5 ? dir_left : dir_right;
	
    double vel_acc = 1.0;
    double rot_acc = 1.0;
	unsigned int count = rsloc.msec_since_stuck;
	static int flag = 0;
	Vec     my_vel = rloc.vtrans;
    double  my_rot = rloc.vrot;
    int     turn_direction = rloc.vrot > 0 ? 1 : -1;

  	Vec    goal_center(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goal_left(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goal_right(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goalOwn(-0.5*MWM.get_field_geometry().field_length,0);
	Vec    middle_center(0.0,0.0);
    Time   now;
	if( flag == 0 )
	{
		if( !bstuck ) 
			return;
		else
		{
			if( count > 300 ) //  now.get_msec() - 
			{
  				flag = 1;
				update_time.update();
			}
		}
	}
	else if( flag == 1 )
	{
		if( update_time.elapsed_msec() < 2000 )
		{
			double vel_back = 0.8;
			
			dv->vtrans = vel_back * dir_left / rloc.heading;
			dv->vrot   = turn_direction * 1.0;
		}
		else
			flag = 0;
		
	}
}		
		 