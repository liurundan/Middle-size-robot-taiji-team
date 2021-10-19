#include "StdAfx.h"
#include "Defense.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "InPlayParameters.h"
#include "BasicFunctions.h"
#include <cmath>

DefenseBehavior::DefenseBehavior()
{
	go_pos = new GotoPos();
	basic_cmd = new BasicCmd();
	avoid   = new Avoid();
	move_with_ball = new MoveWithBall();
}

DefenseBehavior::~DefenseBehavior()
{
	delete go_pos;
	delete basic_cmd;
	delete avoid;
	delete move_with_ball;
}

void DefenseBehavior::getCmd(DriveVector *dv, const Time& t)
{
	*dv = dest;
}

void DefenseBehavior::Defense(const Time& t,double distance2MyGoal)
{
	DriveVector ret;
	
   	double angle = 0.0, Velocity = INPLAY_VELOCITY + 0.15;
	double ROme = MWM.goal2Robot.angle * 2.0;

	double a, b, c, A;
	double subAngle;

	b = MWM.ball2Robot.distance;
	c = MWM.myGoal2Robot.distance;
	A = TwoAngleDifference(MWM.ball2Robot.angle, MWM.myGoal2Robot.angle);
	if( A > 160 * DegreeToRad)
	{
		Velocity *= 0.6;
		if(MWM.myGoal2Robot.distance < distance2MyGoal - 750 )
		{
			angle = MWM.ball2Robot.angle;
		}
		else if(MWM.myGoal2Robot.distance > distance2MyGoal + 750 )
		{
			angle = MWM.myGoal2Robot.angle;
		}
		else
		{
			angle = 0.0;
			Velocity = 0.0;
		}
	}
	else
	{
		a = sqrt( b*b + c*c - 2*b*c*cos(A) );

		subAngle = ( (distance2MyGoal)/( a ) ) * A;

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
		angle = MWM.myGoal2Robot.angle + subAngle;
	}

	double K = (M_PI - A)/M_PI;
	Velocity *= sqrt(sqrt(sqrt(sqrt(sqrt(K)))));

	angle = Limit2PI(angle);
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;
		 
}

int DefenseBehavior::SetPlayDefense(const Time& t,double distance2MyGoal)
{
	DriveVector ret;

	double angle = 0.0, Velocity = INPLAY_VELOCITY + 0.15;
	double ROme = MWM.goal2Robot.angle * 2.0;
	int    returnValue = 0;

	double a, b, c, A;
	double subAngle;

	b = MWM.ball2Robot.distance * 1e-3;
	c = MWM.myGoal2Robot.distance * 1e-3;
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
 		move_with_ball->Move2HomePosition(t);
		move_with_ball->getCmd(&ret,t);
		dest = ret;
		return 0;
	}
	else if( A > 150 * DegreeToRad)  // lrd
	{
		Velocity *= 0.6;
		if(MWM.myGoal2Robot.distance  < distance2MyGoal)
		{
			angle = MWM.ball2Robot.angle;
		}
		else if(MWM.myGoal2Robot.distance  > distance2MyGoal + 2000.0 ||
			(MWM.ball2Robot.distance  < 2000.0 && MWM.ball2Robot.distance > 0.0))
		{
			angle = MWM.myGoal2Robot.angle;
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

		subAngle = ( ( distance2MyGoal * 1e-3 )/( a ) ) * A;

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
		angle = MWM.myGoal2Robot.angle + subAngle;
	}

	double K = (M_PI - A)/M_PI;
	Velocity *= sqrt(sqrt(sqrt(sqrt(sqrt(K)))));

	angle = Limit2PI(angle);
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
  
	dest = ret;

	return returnValue;
}
 
void DefenseBehavior::SupportCenter(const Time& t,double supportDistance)
{
	DriveVector ret;
	
	double backLine = ILLEGAL_MY_MIN_DISTANCE;
	static double Velocity = 0.0, angle = 0.0;
	double ROme = MWM.goal2Robot.angle;

	if(MWM.ball2Robot.distance > 0)
	{
		if(fabs(MWM.ball2Robot.angle) > 45 * DegreeToRad) 
		{
			//BallGetNew(INPLAY_VELOCITY,data);
			move_with_ball->Mawaricom(t);
			move_with_ball->getCmd(&ret,t);
	        dest = ret;
 			return ;
		}
		else if(MWM.ball2Robot.distance > supportDistance + 500)
		{
			Velocity = INPLAY_VELOCITY;
			angle = MWM.ball2Robot.angle;
			ROme = MWM.goal2Robot.angle;
		}
		else if(MWM.ball2Robot.distance > supportDistance + 300)
		{
			Velocity = INPLAY_VELOCITY*0.7;
			angle = MWM.ball2Robot.angle;
			ROme = MWM.goal2Robot.angle;
		}
		else if(MWM.ball2Robot.distance > supportDistance)
		{
			Velocity = INPLAY_VELOCITY*0.5;
			angle = MWM.ball2Robot.angle;
			ROme = MWM.goal2Robot.angle;
		}
		else if(MWM.ball2Robot.distance < supportDistance - 300)
		{
			Velocity = INPLAY_VELOCITY*0.7;
			angle = MWM.myGoal2Robot.angle;
			ROme = MWM.goal2Robot.angle;
		}
		else
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
	        dest = ret;
 			return;
		}
	}
	else
	{
		if(MWM.myPosition.available == TRUE)
		{
		    go_pos->set_target(Vec(MWM.ballPosition.x, MWM.ballPosition.y ), INPLAY_VELOCITY * 0.8 );
			go_pos->getCmd(&ret,t);
		}
		else
		{
 			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
		}
        dest = ret;
 		return;
	}
	angle = Limit2PI(angle);
	avoid->Avoid2007(&Velocity, &angle, &ROme);
	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;

}