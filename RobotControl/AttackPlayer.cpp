#include "StdAfx.h"
#include "AttackPlayer.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "parameters.h"
#include "BasicFunctions.h"
#include "OffenceParameter.h"
#include "InPlayParameters.h"
#include "position.h"
#include "Tools.h"
#include <cmath>

AttackPlayer::AttackPlayer( ConfigReader& cfg)
{
	move_with_ball = new MoveWithBall();
	dribble   = new Dribble();
	basic_cmd = new BasicCmd();
	face_ball = new FaceBall();
	getBall   = new BallGet();
	intercept = new InterceptBall();
	go_pos    = new GotoPos();
	go2pos    = new SPhysGotoPos();
	offence   = new Offence();
	defense   = new DefenseBehavior();
	avoid     = new Avoid();
	approach  = new ApproachBall();
    stuck     = new StuckBehavior();
	searchball = new SearchBall();
    motion    = new PhysicalMotionModel(MWM.get_robot_properties());

	go2pos->set_dynamics(1.5, 2, 2, 2);

}

AttackPlayer::~AttackPlayer()
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
	delete  stuck;
	delete  searchball;
	delete  motion;
}

void AttackPlayer::getCmd(DriveVector *dv, const Time& t)
{
   decide_action( dv,t );
  // test_skills(dv,t);
 
}

void AttackPlayer::test_skills(DriveVector* dv,const Time& t)
{
 	BallLocation  bloc = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
    go2pos->set_target(true,Vec(2000,2000),angBall,2.0,1);
    go2pos->getCmd(dv,t);
}

void AttackPlayer::decide_action( DriveVector* dv,const Time& t )
{
 	BallLocation  bloc = MWM.get_ball_location(t);
	RobotLocation rloc = MWM.get_robot_location(t);
	Vec posMe   = rloc.pos;
	Vec posBall = bloc.pos;
	Angle angBall = ( posBall - posMe ).angle();
 	Angle dirDest;
	PlayerRole role     = WBOARD->getPlayerRole();
	bool team_owns_ball = WBOARD->advancedTeamPossesBall();
    Vec  posDest = Tools::getStrategicPosition3(t);

	static DWORD time_count = myGetTickCount();
	static DWORD ball_lost_time=0;
	static DWORD goal_area_time=0;
	int in_play_flag = 1;
	int illegalFlag  = 0;
	static int goalAreaCount=0;
	static int invisibleCount = 0;
	static double angle = 0.0, velocity = 0.0, ROme = 0.0;
    vector<pair<string,Vec> > team_mate = WBOARD->getTeammate();
 
	if( Tools::in_opp_penalty_area(posMe) ) 
	{
		goal_area_time += myGetTickCount() - time_count;
	 
		if(	MWM.goal2Robot.distance < ILLEGAL_OPP_MIN_DISTANCE )
		{ 
			illegalFlag = 1;
		}
		for( unsigned int i= 0; i < team_mate.size(); i++ )
		{
			double dis_team_goal = Vec(9000.0,0.0).getDistanceTo(team_mate[i].second);
			if( dis_team_goal < ILLEGAL_DISTANCE && team_owns_ball )
 				illegalFlag = 1;
		}
		if( goal_area_time > ILLEGAL_TIME*1000 || illegalFlag == 1 )
		{
			in_play_flag = 0;
            if( !team_owns_ball )
			{
                offence->OffenceFW(t,INPLAY_VELOCITY);
				offence->getCmd(dv,t);
            }
			else
			{
				angle = MWM.fieldCenterAngle;
				ROme  = MWM.goal2Robot.angle;
				velocity = INPLAY_VELOCITY * 0.7;
				avoid->Avoid2007(&velocity, &angle, &ROme);
				angle = Limit2PI(angle);
				basic_cmd->set_move(velocity, angle,  ROme);
				basic_cmd->getCmd(dv,t);
			}
		}
	}
	else if( Tools::in_own_penalty_area(posMe) )
	{
		goal_area_time += myGetTickCount() - time_count;
		
		for( unsigned int i= 0; i < team_mate.size(); i++ )
		{
			double dis_team_goal = Vec(-9000.0,0.0).getDistanceTo(team_mate[i].second);
			if( dis_team_goal < ILLEGAL_DISTANCE )
 				illegalFlag = 1;
		}

		if(	MWM.myGoal2Robot.distance < ILLEGAL_MY_MIN_DISTANCE )
		{
			illegalFlag = 1;
		}
		if( goal_area_time>ILLEGAL_TIME*1000 || illegalFlag == 1 )
		{
			in_play_flag = 0;
  			if( Tools::in_own_penalty_area(posBall) )  
			{
 				WSpset p = MWM.valid_opponents;
				p.keep_players_in_rectangle(Vec(-9000.0,-3250.0),Vec(-6750.0,3250.0));
				int num = p.num;
				
				if( !team_owns_ball && num < 2 )
				{
					if( fabs(angBall.get_deg_180()) < 110 && 
						fabs(posBall.y) < 1600 )
					{
						if( fabs(MWM.ball2Robot.angle) < 0.2 && fabs(MWM.goal2Robot.angle) < 0.3 )
						{
							basic_cmd->set_move(1.5,0.0,0.0);
							basic_cmd->getCmd(dv,t);
						}
						else
						{
							approach->fromBehindPointingToGoal();
							approach->getCmd(dv,t);
						}
					}
					else
					{
						offence->OffenceDF(t,INPLAY_VELOCITY);
						offence->getCmd(dv,t);
					}
				}
				motion->set_drive_vector(*dv,rloc);
				motion->avoid_own_goal(MWM.get_field_geometry(),1500.0);
				*dv = motion->get_drive_vector();
			}
 			else
			{
				angle = MWM.fieldCenterAngle;
				ROme  = MWM.goal2Robot.angle;
				velocity = INPLAY_VELOCITY * 0.7;
				avoid->Avoid2007(&velocity, &angle, &ROme);
				angle = Limit2PI(angle);
				basic_cmd->set_move(velocity, angle,  ROme);
				basic_cmd->getCmd(dv,t);
				motion->set_drive_vector(*dv,rloc);
				motion->avoid_own_goal(MWM.get_field_geometry(),3000.0);
				*dv = motion->get_drive_vector();
			}
		}
	}
	else
	{ 
		ball_lost_time = 0, 
		goal_area_time = 0;
	}

	if( in_play_flag == 1 )
	{
        if( Tools::is_lost_ball_info(t) )
		{
			searchball->getCmd(dv,t);
 			return;
        }
		if( !WBOARD->advancedTeamPossesBall() )  // fast to get ball
		{
			if( Tools::in_own_penalty_area(posBall)  )
			{
 				WSpset p = MWM.valid_opponents;
				p.keep_players_in_rectangle(Vec(-9000.0,-3250.0),Vec(-6750.0,3250.0));
				int num = p.num;
				
				if( num < 2 )
				{
					if( fabs(angBall.get_deg_180()) < 110 && 
						fabs(posBall.y) < 1600 )
					{
						if( fabs(MWM.ball2Robot.angle) < 0.2 && fabs(MWM.goal2Robot.angle) < 0.3 )
						{
							basic_cmd->set_move(1.5,0.0,0.0);
							basic_cmd->getCmd(dv,t);
						}
						else
						{
							approach->fromBehindPointingToGoal();
							approach->getCmd(dv,t);
						}
					}
					else
					{
						offence->OffenceDF(t,INPLAY_VELOCITY);
						offence->getCmd(dv,t);
					}
				}
			}
			else
			{
				if( posBall.x < -4500.0 ) 
				{
					if( TwoAngleDifference(MWM.ball2Robot.angle, MWM.myGoal2Robot.angle) < M_PI * 0.4 )
					{
						move_with_ball->Mawaricom(t);
						move_with_ball->getCmd(dv,t);
					}
					else
					{
						if( fabs(MWM.myPosition.y) < FIELD_HALF_WIDTH * 0.7 )
						{
							offence->OffenceDF(t,INPLAY_VELOCITY);
							offence->getCmd(dv,t);
						}
						else
						{
							offence->OffenceFW(t,INPLAY_VELOCITY);
							offence->getCmd(dv,t);
						}
					} 
				}
				else
				{
					if( fabs(posBall.y) < 3000 )
					{
						if( fabs(MWM.ball2Robot.angle) < 0.2 && fabs(MWM.goal2Robot.angle) < 0.3 )
						{
							basic_cmd->set_move(1.5,0.0,0.0);
							basic_cmd->getCmd(dv,t);
						}
						else
						{
							approach->fromBehindPointingToGoal();
							approach->getCmd(dv,t);
						}
						
					}
					else
					{
						if( TwoAngleDifference(MWM.goal2Robot.angle, MWM.ball2Robot.angle) < M_PI * 0.6 )
						{
							offence->OffenceFW(t,INPLAY_VELOCITY);
							offence->getCmd(dv,t);
						}
						else
						{
							move_with_ball->Mawaricom(t);
							move_with_ball->getCmd(dv,t);
						}
					}
				}
				/*if( TwoAngleDifference(MWM.ball2Robot.angle, MWM.myGoal2Robot.angle) < M_PI * 0.4 )
				{
					move_with_ball->Mawaricom(t);
					move_with_ball->getCmd(dv,t);
				}
				else
				{
					if( fabs(MWM.myPosition.y) < FIELD_HALF_WIDTH * 0.7 )
					{
						offence->OffenceDF(t,INPLAY_VELOCITY);
						offence->getCmd(dv,t);
					}
					else
					{
						offence->OffenceFW(t,INPLAY_VELOCITY);
						offence->getCmd(dv,t);
					}
				} */
			}
		}
		else  // go to best pos or defense
		{
			if( posBall.x < 0.0 && !WBOARD->teamPossesBall() )
			{
               if( posMe.getDistanceTo(posBall) < 4000 ) 
			   {
				   if( fabs(angBall.get_deg_180()) < 110 && 
					   fabs(posBall.y) < 1600 )
				   {
					   if( fabs(MWM.ball2Robot.angle) < 0.2 && fabs(MWM.goal2Robot.angle) < 0.3 )
					   {
						   basic_cmd->set_move(1.5,0.0,0.0);
						   basic_cmd->getCmd(dv,t);
					   }
					   else
					   {
						   approach->fromBehindPointingToGoal();
						   approach->getCmd(dv,t);
					   }
				   }
				   else
				   {
					   offence->OffenceFW(t,INPLAY_VELOCITY);
					   offence->getCmd(dv,t);
				   }
			   }
			}
			else
			{
 				go_pos->set_target(posDest,1.5);
				go_pos->getCmd(dv,t);
				motion->set_drive_vector(*dv,rloc);
				motion->avoid_own_goal(MWM.get_field_geometry(),3000.0);
				*dv = motion->get_drive_vector();
			}
		}
	}
	
    
	if( MWM.goal2Robot.distance < 6000 && fabs(MWM.ball2Robot.angle) < 0.2 && MWM.ball2Robot.distance < 480  )
	{
		if( fabs(MWM.goal2Robot.angle) < ANGLE_THAT_I_CAN_SHOOT )
		{
			BYTE kick_power = MAX_KICK_POWER;
			if( MWM.goal2Robot.distance < 5000 ) 
				kick_power = 15;
			basic_cmd->set_kick(kick_power);
			basic_cmd->getCmd(dv,t);
		}
		else
		{
			approach->fromBehindPointingToGoal();
			approach->getCmd(dv,t);
		}
	}  
  	
	//motion->set_drive_vector(*dv,rloc);
	//motion->avoid_own_goal(MWM.get_field_geometry(),1600.0);
	//*dv = motion->get_drive_vector();
  
	time_count = myGetTickCount();
}