#include "StdAfx.h"
#include "ApproachBall.h"
#include "WorldModel.h"
#include "Tools.h"
#include  <cmath>

ApproachBall::ApproachBall() : CmdGenerator("approach_ball"),desired_dir(0.0),velocity(0.0),
								point_type(1),approcach_type(0)
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

ApproachBall::~ApproachBall()
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

void ApproachBall::getCmd(DriveVector* dv, const Time& t)
{
	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t);  
 	Vec    goal_center(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goal_left(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goal_right(0.5*MWM.get_field_geometry().field_length,0);
	Vec    goalOwn(-0.5*MWM.get_field_geometry().field_length,0);
	Vec    middle_center(0.0,0.0);

	if( approcach_type == 1) 
	{
		intercept->getCmd(dv,t);
		return;
	}
	else if ( approcach_type == 2 )
	{
		Angle  goalball = ( goal_center - bloc.pos ).angle();
		Angle  angball  = ( bloc.pos - rloc.pos ).angle();
		Vec    posIntercept = bloc.pos - Vec(500,goalball.get_rad(),POLAR);
		
		if( bloc.pos.x - rloc.pos.x > 340.0 )
		{
			//intercept->getCmd(dv,t);
			go2pos->init(bloc.pos,angball,1.5);
			go2pos->getCmd(dv,t);
		}
		else
		{
 			if (rloc.pos.getDistanceTo(posIntercept) < 180 )
			{
				intercept->getCmd(dv,t);
			}
			else
			{
				move_with_ball->Move2BehingdBall(t,500);
				move_with_ball->getCmd(dv,t);
			}
		}
 		return;
	}

    static int flag = 0;
     
	double relAngDest;
	double distBall = rloc.pos.getDistanceTo(bloc.pos);

	Angle angOwnGoal  = ( rloc.pos - goalOwn ).angle();
	Angle angToMiddle = ( middle_center - rloc.pos ).angle();
	
	Angle  goal_left_rob  =   (goal_left - rloc.pos).angle();
	Angle  goal_right_rob  =  (goal_right - rloc.pos).angle();
    Angle  goalrob = rloc.pos.y < 0 ? goal_left_rob : goal_right_rob;
    
	double relAngBall = fabs( Tools::my_angle_to(bloc.pos).get_deg_180() );
   
	int  way;  
    switch( point_type )
	{
		case -1:  
			relAngDest = fabs(( angOwnGoal - rloc.heading ).get_deg_180());
			way  = ( angOwnGoal - rloc.heading ).get_deg_180() > 0 ? -1 :1; 	
			break;
		case 0:   
			relAngDest = fabs(( angToMiddle - rloc.heading ).get_deg_180());
			way  = ( angToMiddle - rloc.heading ).get_deg_180() > 0 ? -1 :1;		
			break;
		case 1:   
			relAngDest = fabs( ( goalrob - rloc.heading ).get_deg_180() );	
			way  = ( goalrob - rloc.heading ).get_deg_180() > 0 ? -1 :1;        
			break;
		default: break;
    }

	double dis_get_ball		= 410;
	double ang_get_ball		= 10;
	double dis_rot_point	= 480;
	double ang_rot_point	= 40;
    double dis_ball_front	= 480;
	double ang_ball_front	= 30;
/*	double dis_get_ball		= 360;
	double ang_get_ball		= 10;
	double dis_rot_point	= 450;
	double ang_rot_point	= 40;
    double dis_ball_front	= 480;
	double ang_ball_front	= 30;*/
 
	if( flag == 0 ) 
	{
		if( (distBall < dis_get_ball && relAngBall < ang_get_ball ))  
			flag = 1;
		else
			intercept->getCmd(dv,t);
	}
	else if( flag == 1 )
	{
		if( distBall < dis_rot_point && relAngBall < ang_rot_point )
		{
			if( relAngDest <= 20.0 )
			{
				flag = 2;
			}
			else
			{
 				basic_cmd->set_rotate(way);
				basic_cmd->getCmd(dv,t);
			}
		}
		else
			flag = 0;
	} 
	else if( flag == 2 )
	{
		if( relAngDest < 20.0 )
		{
			if( relAngBall < 10.0 ) 
			{
				basic_cmd->set_move(1.5,0.0,0.0);
				basic_cmd->getCmd(dv,t);
			} 
			else
			{
				flag = 0;
			}
		}
		else
		{
			if (distBall < 250 && relAngBall < 10 )
			{
				face->turn_in_dir(Angle::deg_angle(relAngDest),1.0,true,true);
				face->getCmd(dv,t);
			}
			else
				flag = 0;
		}
	}
 }

bool ApproachBall::fromBehindPointingToGoal()
{
	approcach_type = 0;
	point_type = 1;
	return true;
}

bool ApproachBall::fromBehindPointingToMiddle()
{
	approcach_type = 0;
	point_type = 0;
	return true;
}

bool ApproachBall::fromBehindPointingAwayOwnGoal()
{
	approcach_type = 0;
	point_type = -1;
	return true;
}

bool ApproachBall::indrectly()
{
	approcach_type = 2;
	return true;
}

bool ApproachBall::directly()
{
	approcach_type = 1;
	return true;
}

void ApproachBall::calculate_best_intercept_pos(const RobotLocation& rloc, const BallLocation& bloc, Vec& p, Angle& h )
{
	static bool flag1 = false;
	static bool flag2 = false;

    Vec current_position   = rloc.pos;
	Angle current_heading    = rloc.heading;

	Vec    opp_goal_center( MWM.get_field_geometry().field_length / 2.0, 0 );
	Vec    ballpos = bloc.pos;
	Angle  goalball = ( opp_goal_center - bloc.pos ).angle();

 	Vec    posIntercept_tmp = bloc.pos - Vec(800,goalball.get_rad(),POLAR);
	Angle  go_angle = ( posIntercept_tmp - rloc.pos ).angle();

    bool   angl_thr = (rloc.heading - goalball).in_between(Angle::rad_angle (-0.18), Angle::rad_angle (0.18));
	double dist_thr = rloc.pos.getDistanceTo(posIntercept_tmp);
	
	Vec    gb = opp_goal_center - bloc.pos;
	Vec    gbnn = gb.rotate(Angle::quarter+Angle::rad_angle(0.1)).normalize();
	Vec    posIntercept_tmp2;
	double axis_x_diff = bloc.pos.x - rloc.pos.x;
	double axis_y_diff = bloc.pos.y - rloc.pos.y;
	if( axis_y_diff > 0) 
	{
		posIntercept_tmp2 = bloc.pos - gbnn * 900;
	}
	else
	{
		posIntercept_tmp2 = bloc.pos + gbnn * 900;
	}
	double dist_thr2 = rloc.pos.getDistanceTo(posIntercept_tmp2);
	double vel = 1.0;
	double ome = 2.0;
   
   
	if( ( MWM.ball2Robot.distance < 350 && fabs( MWM.ball2Robot.angle ) < 0.2 &&
		fabs( MWM.goal2Robot.angle ) < 0.2 ) )
	{
		p = bloc.pos;
		h = goalball;
		flag1 = false;
		flag2 = false;
  		return;
	} 
	else
	{
		if( ( fabs( MWM.ball2Robot.angle ) > 1.2 ||
			fabs( MWM.goal2Robot.angle ) > 1.2 ) && MWM.ball2Robot.distance < 250 )	
		{
			flag1 = false;
			flag2 = false;
		}
		
		if( axis_x_diff < 240 )
		{
 			if ( dist_thr2 > 185 && !flag1)
			{
				p = posIntercept_tmp2;
				h = goalball;
			}
			else
			{
				flag1 = true;
				if( dist_thr > 185 && !flag2)
				{
 					p = posIntercept_tmp;
					h = goalball;
				}
				else
				{
 					p = bloc.pos;
					h = goalball;
					flag2 = true;
				}
			}
 			return;
		}
		else
		{
 			if ( flag2 )
			{
				p = bloc.pos;
				h = goalball;
				return;
			}
			if( dist_thr > 185 && !flag2) 
			{
				p = posIntercept_tmp;
				h = goalball;
			}
			else 
			{
				p = bloc.pos;
				h = goalball;
				flag2 = true;
			}
			return;
		}
	}
     
}

