#include "StdAfx.h"
#include "GoalieTaiji.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "parameters.h"
#include "BasicFunctions.h"
#include "OffenceParameter.h"
#include "InPlayParameters.h"
#include "Tools.h"
#include "WSpset.h"
#include <cmath>

GoalieTaiji::GoalieTaiji( ConfigReader& cfg) : CmdGenerator("goalie")
{
	move_with_ball = new MoveWithBall();
	dribble   = new Dribble();
	basic_cmd = new BasicCmd();
	face_ball = new FaceBall();
	getBall   = new BallGet();
	intercept = new InterceptBall();
    stuck     = new StuckBehavior();
	searchball = new SearchBall();

	go_pos    = new GotoPos();
	go2pos    = new SPhysGotoPos();
	offence   = new Offence();
	defense   = new DefenseBehavior();
	avoid     = new Avoid();
	approach  = new ApproachBall();
    motion    = new PhysicalMotionModel(MWM.get_robot_properties());
	go2pos->set_dynamics(1.5, 2, 2, 2);

 	home_x_pos         = -8500.0;
	circle_half_width  = 1750;
	attack_area_radius = 1500;
	attack_area_angle  = 45;
	reference_points_x = 200;
	reference_points_y = 500;
	distance_x         = 500;
	max_turning        = 60;
	kick_permission    = true; 

	cfg.get("Goalie::home_x_pos",home_x_pos);
	cfg.get("Goalie::circle_half_width",circle_half_width);  		 
	cfg.get("Goalie::attack_area_radius",attack_area_radius); 	 
	cfg.get("Goalie::attack_area_angle",attack_area_angle);  		 
	cfg.get("Goalie::reference_points_x",reference_points_x);    
	cfg.get("Goalie::reference_points_y",reference_points_y);   	 
	cfg.get("Goalie::distance_x",distance_x);  	 
	cfg.get("Goalie::max_turning",max_turning);    
	cfg.get("Goalie::kick_permission",kick_permission);   
  
 
}

GoalieTaiji::~GoalieTaiji()
{
	delete	move_with_ball;
	delete	dribble;
	delete	basic_cmd;
	delete	face_ball;
	delete	getBall;
	delete	intercept;
	delete  go_pos;
	delete  offence;
	delete  defense;
	delete  avoid;
	delete  approach;
	delete  motion;
	delete  stuck;
	delete  searchball;
}

void GoalieTaiji::getCmd(DriveVector *dv, const Time& t)
{
	RefereeState rs = MWM.get_game_state().refstate;
    
	switch(rs) 
	{
		case preOwnKickOff:			 	  
		case preOwnThrowIn:			 	  
		case preOwnFreeKick:	   
		case preOwnCornerKick:	   
		case preOwnGoalKick:		   
		case preDroppedBall:		 
		case preOpponentKickOff:
		case postOpponentKickOff: 
		case preOpponentThrowIn:
		case postOpponentThrowIn:	 
		case preOpponentFreeKick:
		case postOpponentFreeKick: 
		case preOpponentCornerKick:
		case postOpponentCornerKick: 
		case preOpponentGoalKick:
		case postOpponentGoalKick:  PositionNormal(dv,t);   break;	 
		case preOpponentPenalty:
		case postOpponentPenalty:   PositionHisPK(dv,t);    break; 
		case preOwnPenalty:			PositionMyPK(dv,t);		break;
		case ownPenalty:			RunMyPK(dv,t);        	break;
		case opponentPenalty:       
		case freePlay:              decide_action(dv,t);    break;
		case stopRobot:
 		default:
			basic_cmd->set_stop();
			basic_cmd->getCmd(dv,t);
 			break;
	}
}
 
bool GoalieTaiji::isDeadBallOwn()
{
	RefereeState rs = MWM.get_game_state().refstate;
 	if( rs == preOwnKickOff || rs == preOwnThrowIn ||
		rs == preOwnFreeKick || rs == preOwnCornerKick || rs == preOwnGoalKick ) 
	{	 
		return true;
	}
	else
	{
		return false;
	}
}

bool GoalieTaiji::isDeadBallOpp()
{
 	RefereeState rs = MWM.get_game_state().refstate;
 	if( rs == preDroppedBall || rs == preOpponentThrowIn ||
		rs == postOpponentThrowIn || rs == preOpponentFreeKick || rs == postOpponentFreeKick  ||
		rs == preOpponentCornerKick || rs == postOpponentCornerKick || rs == preOpponentGoalKick ||
		rs == postOpponentGoalKick || rs == preOpponentKickOff || rs == postOpponentKickOff ) 
	{	 
		return true;
	}
	else
	{
		return false;
	}
}

void GoalieTaiji::PositionNormal(DriveVector* dv,const Time& t)
{
	RefereeState rs = MWM.get_game_state().refstate;
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
 	Vec homePos(home_x_pos,0.0);
    Vec interceptPos = getBestPosition(t);
	
	if( isDeadBallOwn() == true )
 		posDest = homePos;
    else if( isDeadBallOpp() == true)
	{
		if (rs == preOpponentKickOff || rs == postOpponentKickOff )
 			posDest = homePos;
 		else
			posDest = interceptPos;
    }	
    if( bloc.pos_known == 0 && bloc.quality < 0.1 )
	{
		posDest = homePos;
		angBall = 0.0;
	}
	if (posMe.getDistanceTo(posDest) > 180)
	{
		go2pos->set_target(true,posDest,angBall,1.0,3.0);
		go2pos->getCmd(dv,t);
	}
	else
	{
		basic_cmd->set_stop();
		basic_cmd->getCmd(dv,t);
	}
}

void GoalieTaiji::PositionMyPK(DriveVector* dv,const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest = Vec(posBall.x - 500,posBall.y);

    if( bloc.pos_known == 0 )
	{
		posDest = Vec(0.0,0.0);
		angBall = 0.0;
	}
	if (posMe.getDistanceTo(posDest) > 180)
	{
		go2pos->set_target(true,posDest,angBall,1.0,3.0);
		go2pos->getCmd(dv,t);
	}
	else
	{
		basic_cmd->set_stop();
		basic_cmd->getCmd(dv,t);
	}

}

void GoalieTaiji::RunMyPK(DriveVector* dv,const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest = Vec(posBall.x - 500,posBall.y);

    if( bloc.pos_known == 0 )
	{
		basic_cmd->set_stop();
		basic_cmd->getCmd(dv,t);
	}
	if (posMe.getDistanceTo(posBall) < 420 && fabs(Tools::my_angle_to(posBall).get_deg_180()) < 20)
	{
		basic_cmd->set_kick(12);
		basic_cmd->getCmd(dv,t);
	}
	else
	{
		intercept->getCmd(dv,t);
	}
}

void GoalieTaiji::PositionHisPK(DriveVector* dv,const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
 	Vec homePos(home_x_pos,0.0);
    Vec interceptPos = getBestPosition(t);
    Vec posDest = homePos;

    if( bloc.pos_known == 0 )
	{
		posDest = Vec(0.0,0.0);
		angBall = 0.0;
	}
	if (posMe.getDistanceTo(posDest) > 180)
	{
		go2pos->set_target(true,posDest,angBall,1.0,3.0);
		go2pos->getCmd(dv,t);
	}
	else
	{
		basic_cmd->set_stop();
		basic_cmd->getCmd(dv,t);
	}
}

void GoalieTaiji::RunHisPK(DriveVector* dv,const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
 	Vec homePos(home_x_pos,0.0);
    Vec interceptPos = getBestPosition(t);
    Vec posDest = interceptPos;

    if( bloc.pos_known == 0 )
	{
		posDest = homePos;
		angBall = 0.0;
	}
	if (posMe.getDistanceTo(interceptPos) > 180)
	{
		go2pos->set_target(false,posDest,0.0,2.0,3.0);
		go2pos->getCmd(dv,t);
	}
	else
	{
		basic_cmd->set_stop();
		basic_cmd->getCmd(dv,t);
	}	
}

Vec GoalieTaiji::getBestPosition(const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
 	Vec homePos(home_x_pos,0.0);

	static const Vec posLeftTop(-8250,1000);
	static const Vec posRightTop(-8250,-1000);
	static Line lineFront(posLeftTop,posRightTop);
	static Line lineLeft(Vec(-8500.0,posLeftTop.y),posLeftTop);
	static Line lineRight(Vec(-8500.0 ,posRightTop.y),posRightTop);
	 
	Line lineGoal(posBall,Vec(-9000.0,0.0));

	Vec posIntersect = intersect(lineFront,lineGoal);
	if (posIntersect.y > posLeftTop.y )
 		posIntersect = intersect(lineLeft,lineGoal);
    else if (posIntersect.y < posRightTop.y)  
		posIntersect = intersect(lineRight,lineGoal);
    
	if (posIntersect.x < -8500.0)   
		posIntersect.x = -8500.0;
	
 	posDest = posIntersect;
    if (posIntersect == Vec::zero_vector)
	  posDest = homePos;

 	return posDest;
}

int GoalieTaiji::is_ball_heading_for_goal(const Time& t)
{
 	BallLocation bloc  = MWM.get_ball_location(t);
	Vec ball_vel = bloc.velocity;
	Vec ball_pos = bloc.pos;
	double steigung;
	if( ball_vel.x == 0.0 ) return 0;
	steigung = -(ball_vel.y / ball_vel.x);
	id_Punkt_auf_Tor = Vec(-9000.0,0.0) +
		Vec(0, ball_pos.y + steigung * (ball_pos.x - (-9000.0)));
	Vec p(-1,0);
	if ( ( ball_vel*p > 0 ) && 
		 (fabs(id_Punkt_auf_Tor.y) <= 1000.0 + 5000.0) ) 
	{
		return 1;
	}
	return 0;
}

int GoalieTaiji::ball_steps_to(Vec pos,const Time& t)
{
 	BallLocation bloc  = MWM.get_ball_location(t);
	Vec ball_vel = bloc.velocity;
	Vec ball_pos = bloc.pos;
	if(ball_vel.length() == 0.0) return 100;
	double distance = pos.getDistanceTo(bloc.pos);
	double ball_decay = 0.94;
	return (int)( log(1-(1-ball_decay)*distance/ball_vel.length()) / log(ball_decay) );
}

int GoalieTaiji::goalie_steps_to(Vec pos,const Time& t)
{
 	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec my_vel = 2.0 * ( bloc.pos - rloc.pos).normalize();
	Vec my_pos = rloc.pos;
	double v0 = fabs(my_vel.y);
	if( ( pos.y - my_pos.y )* my_vel.y < 0 ) 
	{
		v0 = - v0;
	}
	
	double distance = pos.getDistanceTo(my_pos) - 2000.0; //  
	double schwelle = distance + 1/0.6 - v0/0.6 - 1;
	double temp = 2/3 - v0/0.6;
	for (int k = 1; k<10; k++) 
	{
		if (k + pow(0.4,k)*temp > schwelle) 
		{
			return k;
		}
	}
	return 10;
}

Vec GoalieTaiji::expected_ball_pos(int steps,const Time& t)
{
 	BallLocation bloc  = MWM.get_ball_location(t);
	Vec ball_vel = bloc.velocity;
	Vec ball_pos = bloc.pos;
	Vec  sum = Vec (0.0, 0.0);
	Vec  vel = 0.94 * ball_vel;
	for (int i=0; i < steps;i++) 
	{
		sum += vel;
		vel *= 0.94;
	}
	return ball_pos + sum;
}

bool GoalieTaiji::is_destination_reachable(const Vec& destination, Vec my_pos,Vec my_vel, int steps, const Time& t )
{
	while( steps-- >0 )
	{
		Vec me_to_destination = destination - my_pos;
		my_pos += my_vel * 33;
		if (my_pos.getDistanceTo(destination) < 420 )
		{
			return true;
		}
	}
	return false;
}

bool GoalieTaiji::intercept_goalie(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec my_pos_L   = rloc.pos;
	Vec my_vel_L   = 2.0 * ( bloc.pos - rloc.pos).normalize();
	Angle my_ang_L = rloc.heading;
	Vec ball_pos_L = bloc.pos;
	Vec ball_pos   = bloc.pos;
	Vec ball_vel_L = bloc.velocity;
	Vec ball_vel   = bloc.velocity;
	Vec posBall = bloc.pos;
	Angle angBall = ( ball_pos_L - my_pos_L ).angle();
	Vec   posDest;
    double ball_decay = 0.94;
	if( !Tools::is_ball_kickable() )
	{
		int i = 0;
		do 
		{
			ball_pos_L += ball_vel_L * 33;
			ball_vel_L *= ball_decay;
		} while(!is_destination_reachable(ball_pos_L,my_pos_L,my_vel_L,++i,t));
	}

	Vec destination = ball_pos_L;

	if (!Tools::is_position_in_pitch(ball_pos_L,500.0))
	{
		double steigung;
        if (ball_vel.x == 0.0) 
		{
			return 0;
		}
		steigung = -(ball_vel.y / ball_vel.x);
		Vec id_P_auf_Tor = Vec(-9000.0,0.0) +
			Vec(0, ball_pos.y + steigung * (ball_pos.x - (-9000.0)));
		Vec p(-1.0,0.0);
		if ( ( ball_vel * p > 0 ) 
			&& (fabs(id_P_auf_Tor.y) <= 1000.0 + 5000.0) ) 
		{
			Vec bv = ball_vel;
			bv.normalize(1500);
			destination = id_P_auf_Tor - bv;
		}
	}
  
	if( fabs(rloc.heading.get_deg_180()) < max_turning )
	{
		face_ball->turn_to_ball(3.0,true,true);
		face_ball->getCmd(dv,t);
	}
	
    go2pos->set_target(false,destination,2.2);
	go2pos->getCmd(dv,t);

    return true;
}

void GoalieTaiji::decide_action( DriveVector* dv,const Time& t )
{
	RobotLocation rloc = MWM.get_robot_location(t);
 	BallLocation bloc  = MWM.get_ball_location(t);
	Vec posMe   = bloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec homePos(home_x_pos,0.0);
    Vec interceptPos = getBestPosition(t);
    Vec posDest;
    Vec own_goal(-9000.0,0.0);
 
    Line goalLine(Vec(-8500,1000),Vec(-8500,-1000));
    Vec  posemergence = homePos;
    
	WSpset p = MWM.valid_opponents;
	p.keep_players_in_circle(posBall,2000);
	int num = p.num;

	Cone2d attack_area(Vec(-9000.0,0.0),Angle::deg_angle(-attack_area_angle),Angle::deg_angle(attack_area_angle));
	Circle defend_area(Vec(-9000.0,0.0),circle_half_width);
	Angle angDest = Angle::deg_angle(-Tools::sign(angBall.get_rad_pi())*max_turning) ;
	if( fabs(rloc.heading.get_deg_180()) < max_turning)
  		angDest = angBall;
     if( ( bloc.pos_known == 0 && bloc.quality < 0.1 ) ||  
		!Tools::is_position_in_pitch(posBall,10.0)  )
	{
		posDest = homePos;
		angBall = 0.0;
 		go2pos->set_target(true,homePos,angBall,2.0,8.0,1);
		go2pos->getCmd(dv,t);
	}
	else
	{

		if( posBall.x < -4500.0 )
		{
			if( is_ball_heading_for_goal(t) )
			{
				Line ballLine = Line::makeLineFromPositionAndAngle(posBall,bloc.velocity.angle());
				Vec  tmp = intersect(goalLine,ballLine);
				if( fabs(tmp.y) < 2000 && tmp != Vec::zero_vector ) 
					posDest = tmp;
				
				go2pos->set_target(false,Vec(posMe.x,posBall.y),angDest,2.0,8.0,0);
				go2pos->getCmd(dv,t);
			}
			else if( ( attack_area.is_inside(posBall) && own_goal.getDistanceTo(posBall) < attack_area_radius ) ||
				( own_goal.getDistanceTo(posBall) < 2000.0 ) ) 
			{
				go2pos->set_target(false,posBall,angDest,2.0,8.0,0 );
				go2pos->getCmd(dv,t);
			}
			else if (posBall.getDistanceTo(posMe) < 500 && Tools::in_opp_penalty_area( posBall) &&
				     (fabs(posBall.y) < 800.0 ))
			{
				move_with_ball->Move2BehingdBall(t,500);
				move_with_ball->getCmd(dv,t);
			}
			else
			{
				go2pos->set_target(true,interceptPos,angDest,2.0,8.0,1);
				go2pos->getCmd(dv,t);
			}

			if( fabs(rloc.heading.get_deg_180()) < max_turning)
			{
				face_ball->turn_to_ball(8.0,true,true);
				face_ball->getCmd(dv,t);
			}

 			motion->set_drive_vector(*dv,rloc);
			motion->avoid_point_collision(posBall);
			*dv = motion->get_drive_vector();
		} 
		else
		{
			go2pos->set_target(true,interceptPos,angDest,1.5,8.0,0);
			go2pos->getCmd(dv,t);
		}
	}
	if( fabs(MWM.goal2Robot.angle) < 60 * DegreeToRad   && 
		fabs(MWM.ball2Robot.angle) < 0.2 &&	MWM.ball2Robot.distance < 550  )
	{
		{
			basic_cmd->set_kick(20);
			basic_cmd->getCmd(dv,t);
		}
	}  

	motion->set_drive_vector(*dv,rloc);
	motion->avoid_own_goal(MWM.get_field_geometry(),distance_x);
	*dv = motion->get_drive_vector();

}

void GoalieTaiji::noise_cancel(const Time& t)
{
	RobotLocation rloc = MWM.get_robot_location(t);
 	BallLocation bloc  = MWM.get_ball_location(t);
	Vec posMe   = bloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
    static double beforeBall = 0.0;
	static int countBall = 0;
	int count = 10;
    
	if( bloc.velocity.length() > 0.5)
	  return;

	if( MWM.ball2Robot.distance < 2000.0 && MWM.ball2Robot.distance > 1000.0)
	{
		countBall++;
		if( countBall < count )
		{
			MWM.ball2Robot.distance = beforeBall;
		}
	}
	else
	{
		countBall = 0;
		beforeBall = MWM.ball2Robot.distance;
	}

}

void GoalieTaiji::gotoBallforGoalie(double Velocity,DriveVector* dv, const Time& t)
{
	double RVx=0, RVy=0, ROme=0;
	double angle;    
	double front;
 
	angle = MWM.ball2Robot.angle;
	front = (MWM.ball2Robot.angle + MWM.goal2Robot.angle)/2;

	if( fabs(MWM.ball2Robot.angle) > M_PI/2.75 )  
		front = 2.0 * MWM.goal2Robot.angle;

	if(MWM.ballPosition.x < -8500 )
		front = 3.0 * MWM.goal2Robot.angle;

	if( MWM.ball2Robot.distance < 400  ) 
	{
 		basic_cmd->set_kick(15);
		basic_cmd->getCmd(dv,t);
	}
	
    basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(dv,t);
}

void GoalieTaiji::decide_action2(DriveVector* dv, const Time& t)
{
	RobotLocation rloc = MWM.get_robot_location(t);
	BallLocation bloc  = MWM.get_ball_location(t);
	Vec posMe   = bloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
    Vec interceptPos = getBestPosition(t);
    Vec own_goal(-9000.0,0.0);
	double velocity = 1.5;
	double BallX = MWM.ballPosition.x;
	double BallY = MWM.ballPosition.y;
	
	Cone2d attack_area(own_goal,Angle::deg_angle(-attack_area_angle),Angle::deg_angle(attack_area_angle));
	Cone2d attack_area_close(own_goal,Angle::deg_angle(-2*attack_area_angle),Angle::deg_angle(2*attack_area_angle));
	Circle defend_area(own_goal,circle_half_width);

	if( posBall.x < -7000.0 && posBall.x > -9000.0 )
	{
		if( ( attack_area.is_inside(posBall) && own_goal.getDistanceTo(posBall) < attack_area_radius ) 
			|| ( own_goal.getDistanceTo(posBall) < 2000.0 && attack_area_close.is_inside(posBall) )) 
		{
			gotoBallforGoalie(velocity, dv,t);
		}
		else if( MWM.myGoalObject.distance >= 1000.0 || 
			(fabs(MWM.myGoalObject.maxAngle - MWM.myGoalObject.minAngle) < 0.1 * MWM.myGoalObject.minAngle
			&& MWM.myGoalObject.distance < 1000.0))
		{
			double angle_turn = 0.0; 
			if( fabs(MWM.heading_me.get_deg_180() ) < max_turning )
			    angle_turn = MWM.ball2Robot.angle;
			else
			{
				face_ball->turn_to_point(Vec::zero_vector);
				face_ball->getCmd(dv,t);
			}
			if( posMe.getDistanceTo(own_goal) < 500 )
				velocity = 0.0;
			
			basic_cmd->set_move(velocity,MWM.myGoalObject.angle, angle_turn );
			basic_cmd->getCmd(dv,t);
		}
 		else
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(dv,t);
		}		
	}
	else
	{
		go_pos->set_target(interceptPos,1.0);
		go_pos->getCmd(dv,t);
	}

	
}

