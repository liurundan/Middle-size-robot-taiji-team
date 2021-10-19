#include "StdAfx.h"
#include "parameters.h"
#include "behavior.h"
#include "basicfunctions.h"
#include "OffenceParameter.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "Tools.h"
#include "random.h"
#include <cmath>
 
Behavior::Behavior( ConfigReader& cfg) throw(): CmdGenerator("Behavior")
{
	current = freePlay;
	basic_cmd = new BasicCmd();
	getBall   = new BallGet();
	go_pos    = new GotoPos();
	avoid     = new Avoid();
	touch     = new TouchBehavior();
    go2pos    = new SPhysGotoPos();
    face      = new FaceBall();
	ivpAttackPlayerBehavior = new AttackPlayer(cfg);
	ivpDefendPlayerBehavior = new DefendPlayer(cfg);
	ivpSupportPlayerBehavior = new SupportPlayer(cfg);
	stuck     = new StuckBehavior();
	searchball = new SearchBall();
    move_with_ball = new MoveWithBall();

	go2pos->set_dynamics(1.5, 2, 2, 2);
}

Behavior::~Behavior() throw()
{
	delete basic_cmd;
	delete getBall;
	delete go_pos;
	delete go2pos;
	delete face;
	delete avoid;
	delete touch;
	delete stuck;
	delete searchball;
	delete move_with_ball;
	delete ivpAttackPlayerBehavior;
	delete ivpDefendPlayerBehavior;
	delete ivpSupportPlayerBehavior;
}

bool Behavior::DoYouHaveBall()
{
 	if( fabs(MWM.ball2Robot.angle)<	20*DegreeToRad	&& 
 		MWM.ball2Robot.distance	<	400 )
		return true;
 	else return false;
}

void Behavior::getCmd(DriveVector* dv, const Time& t) throw()
{
    RefereeState rs = MWM.get_game_state().refstate;
    
	switch(rs) 
	{
		case preOwnKickOff:			PositionMyKickOff(dv,t);    current = preOwnKickOff;	 break;
		case preOwnThrowIn:			PositionMyThrowIn(dv,t);    current = preOwnThrowIn;	 break;
		case preOwnFreeKick:		PositionMyFreeKick(dv,t);   current = preOwnFreeKick; break;
		case preOwnCornerKick:		PositionMyCornerKick(dv,t); current = preOwnCornerKick; break;
		case preOwnGoalKick:		PositionMyGoalKick(dv,t);   current = preOwnGoalKick;   break;
		case preDroppedBall:		DropBall(dv,t);   		break;
		case preOwnPenalty:			PositionMyPK(dv,t);		break;
		case ownPenalty:			RunMyPK(dv,t);        	break;
		case preOpponentKickOff:
		case postOpponentKickOff:	RunOppKickOff(dv,t);	break;
		case preOpponentThrowIn:
		case postOpponentThrowIn:	RunOppThrowIn(dv,t);	break;
		case preOpponentFreeKick:
		case postOpponentFreeKick:	RunOppFreeKick(dv,t);	break;
		case preOpponentCornerKick:
		case postOpponentCornerKick:RunOppCornerKick(dv,t);	break;
		case preOpponentGoalKick:
		case postOpponentGoalKick:	RunOppGoalKick(dv,t);	break;
		case freePlay:              InPlay(dv,t);           break;
		case stopRobot:
 		default:
			basic_cmd->set_stop();
			basic_cmd->getCmd(dv,t);
 			break;
	}

}

void Behavior::InPlay(DriveVector* dv, const Time& t)
{
	static int flag = 2;
	if( ( flag == 2 ) && ( current == preOwnKickOff || current == preOwnThrowIn || 
		current == preOwnFreeKick || current == preOwnCornerKick || current == preOwnGoalKick)  )
	{
		flag = 0;
	}
	if( flag == 0 )
	{
		latest_update.update();
		flag = 1;
	}
	else if( flag == 1) 
	{
		if( latest_update.elapsed_msec() <= 5000 )
		{
			switch( current )
			{
				case preOwnKickOff:    RunMyKickOff(dv,t);    break;
				case preOwnThrowIn:    RunMyThrowIn(dv,t);    break;
				case preOwnFreeKick:   RunMyFreeKick(dv,t);   break;
				case preOwnCornerKick: RunMyCornerKick(dv,t); break;
				case preOwnGoalKick:   RunMyGoalKick(dv,t);   break;
				default: break;
			}
		}
		else
		{
			flag = 2;
			current = freePlay;
		}
	}
	else if( flag == 2 ) 
	{
		PlayerRole pr = WBOARD->getPlayerRole();
		switch(pr)
		{
			case Attack2:	ivpAttackPlayerBehavior->getCmd(dv,t);	break;
			case Attack1:
			case Support:	ivpSupportPlayerBehavior->getCmd(dv,t); break;
			case Defend1:
			case Defend3:   
			case Defend2:   ivpDefendPlayerBehavior->getCmd(dv,t);  break;
			default: break;
		}	 
	}
}

int Behavior::PositionMyKickOff(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
	Angle dirDest;
	PlayerRole role = WBOARD->getPlayerRole();
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}
  	switch(role)
	{
		case Attack1:
		case Support:	 
			posDest = Vec(-2000,-1000);
			dirDest = angBall;
			break;
		case Attack2:	
			posDest = Vec(-800,0);
			dirDest = angBall;
			break;
		case Defend2:
			posDest = Vec(-3500,1000);
			dirDest = angBall;
			break;
		case Defend1:
		case Defend3:    	
			posDest = Vec(-5500,0);
			dirDest = angBall;
			break;
		default: break;
	}	 	
	
 	go2pos->set_target(true,posDest,0.0,1.0,3.0,1);
	go2pos->getCmd(dv,t);

	return 1;
}
 
int Behavior::RunMyKickOff(DriveVector* dv, const Time& t)
{
	static int flag = 0;
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
 	Angle angBall = ( posBall - posMe ).angle();
    double dist_ball = posMe.getDistanceTo(posBall);

	PlayerRole role = WBOARD->getPlayerRole();
	switch(role)
	{
		case Attack1:
		case Support:
			{
			/*	if( latest_update.elapsed_msec() <= 3000 && dist_ball > 360 )
				{
					go2pos->set_target(true,posBall,0.0,1.2,3.0,0);
					go2pos->getCmd(dv,t);
				}
				else if( latest_update.elapsed_msec() <= 5000 )
				{
 					go2pos->set_target(true,Vec(2000.0,100.0),angBall,1.5,8.0,0);
					go2pos->getCmd(dv,t);
				}
				else*/
					ivpSupportPlayerBehavior->getCmd(dv,t);
			}break;
		case Attack2:	
			{ 
			//	if( latest_update.elapsed_msec() <= 3000 )
			//	{
 			//		go2pos->set_target(true,Vec(0.0,1000.0),0.0,0.6,8.0,0);
			//		go2pos->getCmd(dv,t);
			//	}
			//	else
			//	{
				    ivpAttackPlayerBehavior->getCmd(dv,t);
			//	}
			}break;
		case Defend1:
		case Defend2:
		case Defend3:   ivpDefendPlayerBehavior->getCmd(dv,t);  break;
		default: break;
	}	 		
	return 1;
}

int Behavior::PositionMyThrowIn(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Vec posOppGoal(9000.0,0.0);
    Vec posOwnGoal(-9000.0,0.0);

	LineSegment lnBG(posBall,posOwnGoal);
	Angle angBall = ( posBall - posMe ).angle();
    Angle angBallGoal = (posOppGoal - posBall).angle();
	Angle angBallOwnGoal = (posOwnGoal - posBall).angle();
	double distToBall = posMe.getDistanceTo(posBall);
	double distBallToOwnGoal = posBall.getDistanceTo(posOwnGoal);
	double distToOwnGoal = distBallToOwnGoal / 2.0;
	double distBall = 1000.0;
	Vec   posBehindBall = posBall - Vec(distBall,angBallGoal.get_rad(),POLAR);
	Vec   posFrontBall  = posBall + Vec(distBall+200,angBallGoal.get_rad(),POLAR);

	Vec   posDest;
	Angle dirDest;
	PlayerRole role = WBOARD->getPlayerRole();
	
	switch(role)
	{
		case Attack1:
 		case Support:	 
			{
				posDest = posFrontBall;
				dirDest = angBallGoal;
			}break;
		case Attack2:	
			{
				if( MWM.ball2Robot.distance > 0 )
				{
					Object obstacle_ball;
					obstacle_ball.angle = MWM.ball2Robot.angle;
					obstacle_ball.distance = MWM.ball2Robot.distance;
					obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
					obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
					MWM.obstacle.push_back(obstacle_ball);
					MWM.obstacles_num = MWM.obstacles_num + 1;
				}
				posDest = posBehindBall;
				dirDest = angBall;
			}break;
		case Defend1:
		case Defend2:
 		case Defend3:    
			{
				posDest = Tools::getStrategicPosition3(t);
				if( fabs( angBall.get_deg_180() ) < 80 )
				  dirDest = angBall;
				else
				  dirDest = 0.0;
			}break;
		default: break;
	}	 	
	go2pos->set_target(true,posDest,dirDest,1.5,8.0,1);
	go2pos->getCmd(dv,t);
//	go_pos->set_target(posDest,1.0,500,1,true);
//	go_pos->getCmd(dv,t);
//	face->turn_in_dir(dirDest,8.0,true,true);
//	face->getCmd(dv,t);
 	return 1;

}

int Behavior::RunMyThrowIn(DriveVector* dv, const Time& t)
{
	static int flag = 0;
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	double dist_ball = posMe.getDistanceTo(posBall);
	Vec posOppGoal(9000.0,0.0);
	Angle angBall = ( posBall - posMe ).angle();
    Angle angBallGoal = (posOppGoal - posBall).angle();
	double distBall = 1000.0;
	Vec   posBehindBall = posBall - Vec(distBall,angBallGoal.get_rad(),POLAR);
	Vec   posFrontBall  = posBall + Vec(distBall,angBallGoal.get_rad(),POLAR);
    Vec   posBehindBall_left  = posFrontBall + (posOppGoal-posBall).rotate(-1.0).normalize() * 1500;
    Vec   posBehindBall_right = posFrontBall + (posOppGoal-posBall).rotate(1.0).normalize() * 1500;
    int   side = Tools::sign(posBall.y);
	Vec   posDest = side < 0 ? posBehindBall_right : posBehindBall_left;;
 	PlayerRole role = WBOARD->getPlayerRole();
	switch(role)
	{
		case Attack1:
		case Support:
			{
				if( ( latest_update.elapsed_msec() > 3000  && latest_update.elapsed_msec() < 5000 ) || 
					  dist_ball < 368 )
				{
 					go_pos->set_target(posDest,2.0);
					go_pos->getCmd(dv,t);
				}
				else if( latest_update.elapsed_msec() > 5000 ) 
				{
					ivpSupportPlayerBehavior->getCmd(dv,t);
				}
			}break;
		case Attack2:	
			{ 
				if( latest_update.elapsed_msec() < 4000 )
				{
 					go2pos->set_target(true,posBall,angBall,2.0,8.0,false);
					go2pos->getCmd(dv,t);
				}
 				else if (latest_update.elapsed_msec() >= 4000) 
				{
					if( fabs(MWM.ball2Robot.angle) < 0.2 &&	MWM.ball2Robot.distance < DISTANCE_HAVE_BALL  
						&& fabs(MWM.goal2Robot.angle) < ANGLE_THAT_I_CAN_SHOOT )
					{
 							basic_cmd->set_kick(MAX_KICK_POWER);
							basic_cmd->getCmd(dv,t);
					}
					else
 					   ivpAttackPlayerBehavior->getCmd(dv,t);
				}
			}break;
		case Defend1:
		case Defend2:
		case Defend3:   ivpDefendPlayerBehavior->getCmd(dv,t);  break;
		default: break;
	}	 		
	return 1;
}

int Behavior::PositionMyFreeKick(DriveVector* dv, const Time& t)
{
    PositionMyThrowIn(dv,t);
	return 1;
}

int Behavior::RunMyFreeKick(DriveVector* dv, const Time& t)
{
	RunMyThrowIn(dv,t);
	return 1;
}

int Behavior::PositionMyCornerKick(DriveVector* dv, const Time& t)
{
    PositionMyThrowIn(dv,t);
	return 1;
}

int Behavior::RunMyCornerKick(DriveVector* dv, const Time& t)
{
   	static int flag = 0;
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	double dist_ball = posMe.getDistanceTo(posBall);
	Vec posOppGoal(9000.0,0.0);
	Angle angBall = ( posBall - posMe ).angle();
    Angle angBallGoal = (posOppGoal - posBall).angle();
	double distBall = 1000.0;
	Vec   posBehindBall = posBall - Vec(distBall,angBallGoal.get_rad(),POLAR);
	Vec   posFrontBall  = posBall + Vec(distBall,angBallGoal.get_rad(),POLAR);
    Vec   posBehindBall_left  = posFrontBall + (posOppGoal-posBall).rotate(-1.0).normalize() * 1500;
    Vec   posBehindBall_right = posFrontBall + (posOppGoal-posBall).rotate(1.0).normalize() * 1500;
    int   side = Tools::sign(posBall.y);
	Vec   posDest = side < 0 ? posBehindBall_right : posBehindBall_left;;
 	PlayerRole role = WBOARD->getPlayerRole();
	switch(role)
	{
		case Attack1:
		case Support:
			{
				if( ( latest_update.elapsed_msec() > 3000  && latest_update.elapsed_msec() < 5000 ) || 
					  dist_ball < 368 )
				{
 					go_pos->set_target(posDest,2.0);
					go_pos->getCmd(dv,t);
				}
				else if( latest_update.elapsed_msec() > 5000 ) 
				{
					ivpSupportPlayerBehavior->getCmd(dv,t);
				}
			}break;
		case Attack2:	
			{ 
				if( latest_update.elapsed_msec() < 4000 )
				{
					go2pos->set_target(true,posBall,angBall,0.8,8.0,false);
					go2pos->getCmd(dv,t);
				}
				else if (latest_update.elapsed_msec() >= 4000) 
				{
					
					ivpAttackPlayerBehavior->getCmd(dv,t);
				}
			}break;
		case Defend1:
		case Defend2:
		case Defend3:   ivpDefendPlayerBehavior->getCmd(dv,t);  break;
		default: break;
	}	 		
	return 1;
   return 1;
}

int Behavior::PositionMyGoalKick(DriveVector* dv, const Time& t)
{
    PositionMyThrowIn(dv,t);
	return 1;
}

int Behavior::RunMyGoalKick(DriveVector* dv, const Time& t)
{
	RunMyThrowIn(dv,t);
 	return 1;
}

int Behavior::PositionMyPK(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	double disttoBall = posMe.getDistanceTo(posBall);
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
 	PlayerRole role = WBOARD->getPlayerRole();
	Vec   fieldCenter(0,0);
	Vec   goalCenter(9000.0,0);
	Vec   goalleft(9000.0,800.);
	Vec   goalright(9000.0,-800.);
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}

    Angle  ang_goal_center = (goalCenter - posBall).angle();
	Angle  ang_goal_left   = (goalleft - posBall).angle();
	Angle  ang_goal_right  = (goalright - posBall).angle();
	double distBall = 500.0;
	static Angle dirDest = urandom() > 0.5 ? ang_goal_left : ang_goal_right;
	posDest = posBall - Vec(distBall,dirDest.get_rad(),POLAR);
 
	go2pos->set_target(true,posDest,dirDest,1.0,8.0,1);
	go2pos->getCmd(dv,t);
	return 1;
}

int Behavior::RunMyPK(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	double disttoBall = posMe.getDistanceTo(posBall);
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
     
	if( Tools::shoot_possible() )
	{
		basic_cmd->set_kick(12);
		basic_cmd->getCmd(dv,t);
	}
	else
	{
		go2pos->set_target(true,posBall,angBall,1.0,8.0,1);
		go2pos->getCmd(dv,t);
	}
   	return 1;

}

int Behavior::DropBall(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
  	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
 	Vec posOppGoal(9000.0,0.0);
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
	Angle dirDest;
	PlayerRole role = WBOARD->getPlayerRole();
    Angle angBallGoal = (posOppGoal - posMe).angle();
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}

  	if( posBall.x < -6800.0 )
	{
		RunOppCornerKick(dv,t);
		return 1;
	}
	else
	{
		posDest = Tools::getStrategicPosition3(t);
	 	switch(role)
		{
			case Attack1:
			case Support:
			case Attack2:
				{
 					dirDest = angBall;
				}break;
			case Defend2:
 			case Defend1:
			case Defend3:  
				{
 					dirDest = angBallGoal;
				}break;
			default: break;
		}	  
	}
    go2pos->set_target(true,posDest,dirDest,1.2,8.0,1);
	go2pos->getCmd(dv,t);
	return 1;
}

int Behavior::RunOppKickOff(DriveVector* dv, const Time& t)
{
	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posOwnGoal(-9000.0,0.0);
	Vec posOppGoal(9000.0,0.0);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Vec   posDest;
	Angle dirDest;
    double x,y;
	Angle angBall = ( posBall - posMe ).angle();
	PlayerRole role = WBOARD->getPlayerRole();
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}

	switch(role)
	{
		case Attack1:
		case Support:
			{
 				x = -2300.0;
				y = -1500.0;
			}break;
		case Attack2:
			{
 				x = -2300.0;
				y = 0.0;
			}break;
		case Defend1:
		case Defend3:    	
			{
				x = -5800.0;
   				y = 0.0;
			}break;
		case Defend2:
			{ 
			    x = -2300.0;
				y = 1500.0;
			}break;		
		default: break;
	}
	posDest = Vec(x,y);
 
 	go2pos->set_target(true,posDest,angBall,1.2,8.0,1);
	go2pos->getCmd(dv,t);
	return 1;
}

int Behavior::RunOppThrowIn(DriveVector* dv, const Time& t)
{
 	BallLocation bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	 
 	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Vec posOppGoal(9000.0,0.0);
	Angle angBall = ( posBall - posMe ).angle();
    Angle angBallGoal = (posOppGoal - posMe).angle();
	Vec   posDest;
	Angle dirDest;
	PlayerRole role = WBOARD->getPlayerRole();
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}

	if( posBall.x < -6800.0) 
	{
		RunOppCornerKick(dv,t);
		return 1;
	}
	else  
	{
		posDest = Tools::getStrategicPosition3(t);
  	    switch( role )
		{
			case Attack1:
			case Support:
 			case Attack2:
				{
				    dirDest = angBall;
				}break;
  			case Defend2:
 			case Defend1:
			case Defend3: 
				{
					dirDest = angBallGoal;
				}break;
 			default: break;
		}    	
	}
	go2pos->set_target(true,posDest,dirDest,1.2,8.0,1);
	go2pos->getCmd(dv,t);
	return 1;
}

int Behavior::RunOppFreeKick(DriveVector* dv, const Time& t)
{
	RunOppThrowIn(dv,t);
	return 1;
}

int Behavior::RunOppCornerKick(DriveVector* dv, const Time& t)
{
	BallLocation  bloc  = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posOwnGoal(-9000.0,0.0);
	Vec posOppGoal(9000.0,0.0);
    double x,y;
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
	Vec   posDest;
	Angle dirDest;
	PlayerRole role = WBOARD->getPlayerRole();
    Angle angBallGoal = (posOppGoal - posMe).angle();
	Angle angBallOwnGoal = (posOwnGoal - posBall).angle();
	double distToBall = posMe.getDistanceTo(posBall);
    int   side = Tools::sign(posBall.y);
	if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}

	switch( role )
	{
		case Attack2:
			{
				x = -7200.0;
				y = side * 3800.0;
 				posDest = Vec(x,y);
				dirDest = angBall;
			}break;
		case Attack1:
		case Support:
			{
				x = -6000.0;
				y = side * 2100.0;
 				posDest = Vec(x,y);
				dirDest = angBall;
			}break;
		case Defend1:
 		case Defend3:  
			{
				x = -7500.0;
				y = 0.0;
 				posDest = Vec(x,y);
				dirDest = angBallGoal;
			}break;
 		case Defend2:
			{
				x = -8300.0;
				y = side * 3800.0;
 				posDest = Vec(x,y);
				dirDest = angBall;
			}break;
		default: break;
	}	 
	go2pos->set_target(true,posDest,dirDest,1.2,8.0,1);
	go2pos->getCmd(dv,t);

	return 1;
}

int Behavior::RunOppGoalKick(DriveVector* dv, const Time& t)
{	
	RunOppThrowIn(dv,t);
	return 1;
}



 
 