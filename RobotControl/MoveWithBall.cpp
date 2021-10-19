#include "StdAfx.h"
#include "MoveWithBall.h"
#include "WorldModel.h"
#include "InPlayParameters.h"
#include "OffenceParameter.h"
#include "BasicFunctions.h"
#include "WhiteBoard.h"
#include "position.h"
#include "Tools.h"
#include <cmath>

MoveWithBall::MoveWithBall():CmdGenerator("movewithball")
{
	basic_cmd = new BasicCmd();
	go_pos    = new GotoPos();
	avoid     = new Avoid();
}

MoveWithBall::~MoveWithBall()
{
   delete basic_cmd;
   delete go_pos;
   delete avoid;
}

double MoveWithBall::GetGoodVelocity( )
{
	double Velocity;
	double slope;

	slope = (MOVE2PASS_POSITION_VELOCITY-INPLAY_VELOCITY_NEAR_BALL)*1e3/
					(BALLGET_DISTANCE_FAR-BALLGET_DISTANCE_LITTLE_FAR);
	Velocity = slope * (MWM.ball2Robot.distance - BALLGET_DISTANCE_LITTLE_FAR)*1e-3 
					+ INPLAY_VELOCITY_NEAR_BALL;

	if(MWM.ball2Robot.distance*1e-3 > MOVE2PASS_POSITION_VELOCITY)
		Velocity = MOVE2PASS_POSITION_VELOCITY;
	if(Velocity < INPLAY_VELOCITY_NEAR_BALL)
		Velocity = INPLAY_VELOCITY_NEAR_BALL;

	return Velocity;
}

double MoveWithBall::Angle180To360(double kakudo)
{
	if(kakudo < 0.0)
	{
		kakudo = 180.0*DegreeToRad - fabs(kakudo) + 180.0*DegreeToRad;
	}
	return kakudo;
}

void MoveWithBall::Move2HomePosition( const Time& t)
{
	DriveVector ret;
  	switch( WBOARD->getPlayerRole() )
	{
		case Attack2:
			go_pos->set_target(Vec(ALPHA_HOME_POSITION_X, ALPHA_HOME_POSITION_Y),VELOCITY_MOVE_TO_HOME_POSITION);
			go_pos->getCmd(&ret,t); // -2700,1200
 			break;
		case Support:
			go_pos->set_target(Vec(BETA_HOME_POSITION_X, BETA_HOME_POSITION_Y),VELOCITY_MOVE_TO_HOME_POSITION);
			go_pos->getCmd(&ret,t); // -2700, -1200
 			break;
		case Defend1:
			go_pos->set_target(Vec(GAMMA_HOME_POSITION_X, GAMMA_HOME_POSITION_Y),VELOCITY_MOVE_TO_HOME_POSITION);
			go_pos->getCmd(&ret,t);// -3600,0
 			break;
		case Defend2:
			go_pos->set_target(Vec(DELTA_HOME_POSITION_X, DELTA_HOME_POSITION_Y),VELOCITY_MOVE_TO_HOME_POSITION);
			go_pos->getCmd(&ret,t);// -4500,1800
 			break;
		case Defend3:
			go_pos->set_target(Vec(EPSHILON_HOME_POSITION_X, EPSHILON_HOME_POSITION_Y),VELOCITY_MOVE_TO_HOME_POSITION);
			go_pos->getCmd(&ret,t);// -4500,-1800
 			break;
		default:
			break;
	}
	dest = ret;
}

void MoveWithBall::Move2InPlayHomePosition( const Time& t)
{
	DriveVector ret;
 	switch(WBOARD->getPlayerRole())
	{
		case Attack2:
			go_pos->set_target(Vec(ALPHA_INPLAY_HOME_POSITION_X, ALPHA_INPLAY_HOME_POSITION_Y),VELOCITY_MOVE_TO_INPLAY_HOME_POSITION);
			go_pos->getCmd(&ret,t);
 			break;
		case Support:
			go_pos->set_target(Vec(BETA_INPLAY_HOME_POSITION_X, BETA_INPLAY_HOME_POSITION_Y),VELOCITY_MOVE_TO_INPLAY_HOME_POSITION);
			go_pos->getCmd(&ret,t);
 			break;
		case Defend1:
			go_pos->set_target(Vec(GAMMA_INPLAY_HOME_POSITION_X, GAMMA_INPLAY_HOME_POSITION_Y),VELOCITY_MOVE_TO_INPLAY_HOME_POSITION);
			go_pos->getCmd(&ret,t);
 			break;
		case Defend2:
			go_pos->set_target(Vec(DELTA_INPLAY_HOME_POSITION_X, DELTA_INPLAY_HOME_POSITION_Y),VELOCITY_MOVE_TO_INPLAY_HOME_POSITION);
			go_pos->getCmd(&ret,t);
			break;
		case Defend3:
			go_pos->set_target(Vec(EPSHILON_INPLAY_HOME_POSITION_X, EPSHILON_INPLAY_HOME_POSITION_Y),VELOCITY_MOVE_TO_INPLAY_HOME_POSITION);
			go_pos->getCmd(&ret,t);
			break;
		default:
			break;
	}
	dest = ret;
}

void MoveWithBall::getCmd(DriveVector *dv, const Time& t)
{
    *dv = dest;
}
 
int MoveWithBall::Move2PassPosition(const Time& t)
{
	DriveVector ret;
 	double Velocity, angle = 0.0, ROme;
	static int flag=0,returnValue=0;

	if(MWM.ball2Robot.distance < 0.0)
	{
		Move2HomePosition(t);
		dest = ret;
 		return 0;
	}

	if(MWM.ball2Robot.distance > 0)
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;

	}

	Velocity = GetGoodVelocity();
	ROme = MWM.goal2Robot.angle;

	if(MWM.ball2Robot.distance >1000.0 )
	{
		angle = MWM.ball2Robot.angle;
	}
	else if(MWM.ball2Robot.distance < 600 )
	{
		angle = MWM.ball2Robot.angle+3.14;
		Velocity = 0.5;
	}
	else if(MWM.ballPosition.y > -1000.0 && MWM.ball2Robot.angle < -20*DegreeToRad && 
					MWM.ball2Robot.angle > -100*DegreeToRad &&
					fabs(MWM.goal2Robot.angle)<30*DegreeToRad )
	{
		angle = 0.0;
		Velocity = 0.0;
		flag = 0;
		returnValue=1;
	}
	else if(MWM.ballPosition.y < 1000.0 && MWM.ball2Robot.angle > 20*DegreeToRad && 
					MWM.ball2Robot.angle < 100*DegreeToRad &&
					fabs(MWM.goal2Robot.angle)<30*DegreeToRad )
	{
		angle = 0.0;
		Velocity = 0.0;
		flag=0;
		returnValue=1;
	}
	else
	{
		Velocity = 0.8;
		if(MWM.ballPosition.y > -1000.0) angle = MWM.ball2Robot.angle - M_PI/2.0;
		else							angle = MWM.ball2Robot.angle + M_PI/2.0;

	}

	avoid->Avoid2007(&Velocity, &angle, &ROme);

	angle = Limit2PI(angle);

	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;

    return returnValue;	
}

int MoveWithBall::Move2BallRahen1st(const Time& t)
{
	DriveVector ret;
	double	Velocity = GetGoodVelocity();
	double  angle,ROme;
	static int flag = 0,returnValue=0;

	ROme = MWM.ball2Robot.angle*ROTATE_GAIN;

	if(MWM.ball2Robot.distance < 0.0)
	{
		if(MWM.myPosition.available == TRUE)
		{
			go_pos->set_target(Vec(MWM.ballPosition.x, MWM.ballPosition.y ), INPLAY_VELOCITY * 0.8 );
			go_pos->getCmd(&ret,t);
		}
		else
		{
			basic_cmd->set_move(0.0, 0.0, MWM.goal2Robot.angle);
			basic_cmd->getCmd(&ret,t);
		}
        dest = ret;
		return 0;
	}
	else if(MWM.ball2Robot.distance > 0)
	{
	    Object obstacle_ball;

		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}
	
	if(MWM.ball2Robot.distance >1200 )
	{
		angle = MWM.ball2Robot.angle;
	}
	else if(MWM.ball2Robot.distance <800 )
	{
		angle = MWM.ball2Robot.angle+3.14;
		Velocity = INPLAY_VELOCITY * 0.3;
	}
	else if(fabs( MWM.fieldCenterAngle) < 20*DegreeToRad )
	{
		angle = 0.0;
		Velocity = 0.0;
		flag=0;
		returnValue=1;
	}
	else
	{
		Velocity = INPLAY_VELOCITY * 0.5;
		if(flag==0)
		{
			if(MWM.ball2Robot.angle > 0.0)
			{
				flag=1;
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				flag=2;
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
		else
		{
			if(flag==1)
			{
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
	}

	avoid->Avoid2007(&Velocity, &angle, &ROme);
	
	angle = Limit2PI(angle);

	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;

	return returnValue;
}

int MoveWithBall::Move2BallRahen(const Time& t)
{
	DriveVector ret;
	double	Velocity=GetGoodVelocity( ),angle,ROme;
	static int flag = 0,returnValue=0;

	ROme=MWM.ball2Robot.angle*ROTATE_GAIN;

	if(MWM.ball2Robot.distance < 0.0)
	{
		if(MWM.myPosition.available == TRUE)
		{
			go_pos->set_target(Vec(MWM.ballPosition.x, MWM.ballPosition.y ), INPLAY_VELOCITY * 0.8 );
			go_pos->getCmd(&ret,t);

		}
		else
		{
			basic_cmd->set_move(0.0, 0.0, MWM.goal2Robot.angle);
			basic_cmd->getCmd(&ret,t);
		}
		dest = ret;
 		return 0;
	}
	else if(MWM.ball2Robot.distance > 0)
	{
	    Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;	
	}
	
	if(MWM.ball2Robot.distance >1200 )
	{
		angle = MWM.ball2Robot.angle;
	}
	else if(MWM.ball2Robot.distance <800 )
	{
		angle = MWM.ball2Robot.angle+3.14;
		Velocity = 0.5;
	}
	else if(fabs(MWM.fieldCenterAngle) < 90*DegreeToRad)
	{
		angle = 0.0;
		Velocity = 0.0;
		flag=0;
		returnValue=1;
	}
	else
	{
		Velocity = INPLAY_VELOCITY * 0.5;
		if(flag==0)
		{
			if(MWM.ball2Robot.angle > 0.0)
			{
				flag=1;
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				flag=2;
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
		else
		{
			if(flag==1)
			{
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
	}

	avoid->Avoid2007(&Velocity, &angle, &ROme);
	
	angle = Limit2PI(angle);
	basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(&ret,t);
    dest = ret;

 	return returnValue;
}

int MoveWithBall::Move2BallRahen2OppCombi(const Time& t,double combiAngle)
{
	DriveVector ret;

	double	Velocity = GetGoodVelocity(), angle, ROme;
	static int flag = 0, returnValue = 0;
    Velocity = 1.0;
	
	ROme = MWM.ball2Robot.angle * 2.5; // ROTATE_GAIN
 	
	if(MWM.ball2Robot.distance < 0.0)
	{
		if(MWM.myPosition.available == TRUE)
		{
			go_pos->set_target(Vec(MWM.ballPosition.x, MWM.ballPosition.y ), INPLAY_VELOCITY * 0.8 );
			go_pos->getCmd(&ret,t);

		}
		else
		{
			basic_cmd->set_move(0.0, 0.0, MWM.goal2Robot.angle);
			basic_cmd->getCmd(&ret,t);
		}
		dest = ret;
 		return 0;
	}
	else if(MWM.ball2Robot.distance > 0)
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	} 
	if(MWM.ball2Robot.distance > 1200 )
	{
		angle = MWM.ball2Robot.angle;
	}
	else if(MWM.ball2Robot.distance < 800)
	{
		angle = MWM.ball2Robot.angle + 3.14;
		Velocity = INPLAY_VELOCITY * 0.3;
	}
	else if(Angle180To360(MWM.goal2Robot.angle) > Angle180To360(combiAngle-M_PI-10*DegreeToRad) &&
		Angle180To360(MWM.goal2Robot.angle) < Angle180To360(combiAngle-M_PI+10*DegreeToRad))
	{
		angle = 0.0;
		Velocity = 0.0;
		flag=0;
		returnValue=1;
	}
	else
	{
		Velocity = INPLAY_VELOCITY * 0.5;
		if(flag==0)
		{
			if(MWM.ball2Robot.angle > 0.0)
			{
				flag=1;
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				flag=2;
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
		else
		{
			if(flag==1)
			{
				angle = MWM.ball2Robot.angle + M_PI/2.0;
			}
			else
			{
				angle = MWM.ball2Robot.angle - M_PI/2.0;
			}
		}
	}
 
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	
	angle = Limit2PI(angle); 
	basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(&ret,t);
    dest = ret;

	return returnValue;
}

int MoveWithBall::Move2BehingdBall(const Time& t,double stopDistance)
{
	DriveVector ret;
	
	double	Velocity = GetGoodVelocity(), angle, ROme;
	double	T = 0.5 , K=0.53+0.05*1.4;						//90

	ROme = MWM.goal2Robot.angle;

	if( Tools::is_lost_ball_info(t) )
	{
		Move2HomePosition(t);
		dest = ret;
  		return 0;
	}
	else if( MWM.ball2Robot.distance > 0 )
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}
	
	if( (fabs(MWM.ball2Robot.angle-MWM.goal2Robot.angle) > T*M_PI) || 
		MWM.ball2Robot.distance >1700 || fabs(MWM.ball2Robot.angle)>3.14/2.0)
	{
		if(MWM.ball2Robot.angle > 0.0)	
			angle = MWM.ball2Robot.angle + K * 1e3 / MWM.ball2Robot.distance;
		else						
			angle = MWM.ball2Robot.angle - K * 1e3 / MWM.ball2Robot.distance;
	}
	else if(MWM.ball2Robot.distance > stopDistance + 1000.0 )
	{
		angle = Limit2PI(MWM.ball2Robot.angle + 1*(MWM.ball2Robot.angle-MWM.goal2Robot.angle));
	}
	else if(MWM.ball2Robot.distance < stopDistance )
	{
		angle = 3.14;
		Velocity = 0.5;
	}
	else if( fabs(MWM.ball2Robot.angle - MWM.goal2Robot.angle) > 0.1)
	{
		if(MWM.ball2Robot.angle > 0.0)	
			angle = MWM.ball2Robot.angle + M_PI/2.0;
		else	
			angle = MWM.ball2Robot.angle - M_PI/2.0;
		Velocity = 0.5;
	}
	else
	{
		angle =MWM.ball2Robot.angle;
		Velocity = 0.0;
		basic_cmd->set_move(Velocity,angle,ROme);
		basic_cmd->getCmd(&ret,t);
		dest = ret;
 		return 1;
	}
	
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	angle = Limit2PI(angle);
	
	basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;

 	return 0;
}

int MoveWithBall::Move2BetweenBall(const Time& t,double stopDistanceFromBall)
{
	DriveVector ret;
	
	double angle = 0.0, Velocity = INPLAY_VELOCITY + 0.15;
	double ROme  = MWM.goal2Robot.angle * 2.0;
	int returnValue = 0;

	double a, b, c, A;
	double subAngle;

	b = MWM.ball2Robot.distance;
	c = MWM.myGoal2Robot.distance;
	A = TwoAngleDifference(MWM.ball2Robot.angle, MWM.myGoal2Robot.angle);

	if(MWM.ball2Robot.distance > 0)
	{
		Object obstacle_ball;
		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}
	
	if(MWM.ball2Robot.distance < 0.0)
	{
		//Move2Position(data, INPLAY_VELOCITY * 0.8, MWM.ballPosition.x, MWM.ballPosition.y);
		Move2HomePosition(t);
		dest = ret;
 		return 0;
	}
	else if( A > 150 * DegreeToRad)
	{
		Velocity *= 0.6;
		if(MWM.ball2Robot.distance < stopDistanceFromBall)
		{
			angle = MWM.myGoal2Robot.angle;
		}
		else if(MWM.ball2Robot.distance > stopDistanceFromBall + 1200)
		{
			angle = MWM.ball2Robot.angle;
		}
		else
		{
			angle = 0.0;
			Velocity = 0.0;
			returnValue = 1;
		}
	}
	else
	{
		a = sqrt( b*b + c*c - 2*b*c*cos(A) );

		subAngle = ( (stopDistanceFromBall)/( a ) ) * A;

		if(MWM.ball2Robot.angle > 0)
		{
			if(MWM.myGoal2Robot.angle > 0)
			{
				if(MWM.myGoal2Robot.angle > MWM.ball2Robot.angle)
					subAngle = -subAngle;
			}
			else
			{
				if(fabs(MWM.myGoal2Robot.angle) + fabs(MWM.ball2Robot.angle) > M_PI)
					subAngle = -subAngle;
			}
		}
		else
		{
			if(MWM.myGoal2Robot.angle > 0)
			{
				if(fabs(MWM.myGoal2Robot.angle) + fabs(MWM.ball2Robot.angle) < M_PI)
					subAngle = -subAngle;
			}
			else
			{
				if(MWM.myGoal2Robot.angle > MWM.ball2Robot.angle)
					subAngle = -subAngle;
			}
		}
		angle = MWM.ball2Robot.angle - subAngle;
	}
	double K = (M_PI - A)/M_PI;
	Velocity *= sqrt(sqrt(sqrt(sqrt(sqrt(K)))));

	angle = Limit2PI(angle);
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;

	return 0;
}

void MoveWithBall::Mawaricom( const Time& t)
{
	DriveVector ret;

	double angle;    
	double Velocity;
	double ROme = MWM.goal2Robot.angle;
	double K = 0.45; //1.2; 0.45
	
	Velocity = 1.5;// INPLAY_VELOCITY; 

	if(MWM.ball2Robot.angle > 0.0)	
		angle = MWM.ball2Robot.angle + K/(MWM.ball2Robot.distance *1e-3 * 0.5);
	else						
		angle = MWM.ball2Robot.angle - K/(MWM.ball2Robot.distance *1e-3 * 0.5);

	angle += -0.1;
//	angle = angle + MWM.ball2Robot.sub_angle;

	avoid->Avoid2007(&Velocity, &angle, &ROme);
	angle = Limit2PI(angle);

	basic_cmd->set_move(Velocity,angle,ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;
}
