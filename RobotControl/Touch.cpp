#include "StdAfx.h"
#include "Touch.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "InPlayParameters.h"
#include "BasicFunctions.h"
#include <cmath>

TouchBehavior::TouchBehavior()
{
	basic_cmd = new BasicCmd();
	avoid   = new Avoid();
	move_with_ball = new MoveWithBall();
	touchFlag = 4;
}

TouchBehavior::~TouchBehavior()
{
	delete basic_cmd;
	delete avoid;
	delete move_with_ball;
}

void TouchBehavior::getCmd(DriveVector *dv, const Time& t)
{
   *dv = dest;
}

void TouchBehavior::setTouchFlag( int tf /* = 4  */)
{
	touchFlag = tf;
}

int TouchBehavior::normal_touch(const Time& t)
{
	DriveVector ret;

	double touchVelocity = TOUCH_VELOCITY;
	double backDistance = TOUCH_BACK_DISTANCE;
	static double angle=0.0, ROme=0.0;

	int				time = 0;
	static double	startCount = 0.0;
	double			nowCount = 0.0;
	static int countFlag = 0;

	if(touchFlag == 0)
	{
		startCount = myGetTickCount();
		touchFlag = 1;
	}
	else if(touchFlag == 1)
	{
		if(fabs(MWM.ball2Robot.angle) >= 90*DegreeToRad)
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
		}
		else if(MWM.ball2Robot.distance > 1200 )
		{
			basic_cmd->set_move(0.2,MWM.ball2Robot.angle,MWM.goal2Robot.angle);
			basic_cmd->getCmd(&ret,t);
		}
		else if(fabs(MWM.ball2Robot.angle) < 90*DegreeToRad)
		{
			basic_cmd->set_move(0.2,MWM.goal2Robot.angle,MWM.goal2Robot.angle);
			basic_cmd->getCmd(&ret,t);
		}
		else
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
		}
		nowCount = myGetTickCount() - startCount;
		if(nowCount > TOUCH_AND_GO_ADJAST_TIME * 1000)
		{
			startCount = myGetTickCount();
			touchFlag = 2;
		}
	}
	else if(touchFlag == 2)
	{
		if(MWM.ball2Robot.angle > 0)
		{
			if(MWM.ball2Robot.angle <100*DegreeToRad){ROme = -1.0;	}
			else if(MWM.ball2Robot.angle > 140*DegreeToRad){ROme = 1.0;}
			else{ROme = 0.0;touchFlag = 3;}
		}
		else
		{
			if(MWM.ball2Robot.angle > -100*DegreeToRad){ROme = 1.0;}
			else if(MWM.ball2Robot.angle < -140*DegreeToRad){	ROme = -1.0;}
			else{ROme = 0.0;touchFlag = 3;}
		}
		basic_cmd->set_move(0.0,0.0,ROme);
		basic_cmd->getCmd(&ret,t);

 		nowCount = myGetTickCount() - startCount;
		if(nowCount > TOUCH_AND_GO_ROTATE_TIME * 1000)
		{
			touchFlag = 3;
		}
	}
	else if(touchFlag == 3)
	{
		if( MWM.ball2Robot.distance > MAWARIKOMI_DISTANCE_TOUCH )
		{
			angle = MWM.ball2Robot.angle;
			ROme = MWM.ball2Robot.angle;
	//		Avoid2007(&touchVelocity, &angle, &ROme, data);
			basic_cmd->set_move(touchVelocity, angle, 0.);
			basic_cmd->getCmd(&ret,t);
		}
		else
		{
			if(countFlag < 5)
			{
				countFlag++;
			}
			else
			{
				countFlag = 0;
				touchVelocity = 0.0;
				touchFlag = 4;
			}
		}
	}
	else if(touchFlag == 4)
	{
		if(MWM.ball2Robot.distance < backDistance)
		{
#if TOUCH_AND_GO
				angle = MWM.ball2Robot.angle+M_PI;
#else
				angle = MWM.goal2Robot.angle;
#endif
				touchVelocity = INPLAY_VELOCITY;
			//	Avoid2007(&touchVelocity, &angle, &ROme, data);
				if( MWM.ball2Robot.distance < 500 )
				{
					//Move(touchVelocity,MWM.ball2Robot.angle+M_PI,ROme);
					basic_cmd->set_move(touchVelocity,MWM.ball2Robot.angle+M_PI,ROme);
					basic_cmd->getCmd(&ret,t);
				}
				else
				{
					//Move(touchVelocity,angle,MWM.goal2Robot.angle);
					basic_cmd->set_move(touchVelocity,angle,MWM.goal2Robot.angle);
					basic_cmd->getCmd(&ret,t);

				}
		}
		else
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
			//touchFlag = 0;
			dest = ret;
			return 1;
		}
	}
	MWM.ball2Robot.distance = 100.0;

	dest = ret;

	return 0;
}

int TouchBehavior::MkTouch(const Time& t)
{
	DriveVector ret;
	double touchVelocity = TOUCH_VELOCITY;
	double backDistance = TOUCH_BACK_DISTANCE;

	int				time = 0;
	static double	startCount = 0.0, angle=0.0, ROme=0.0;
	double			nowCount = 0.0;
	static int countFlag = 0;

	MWM.goal2Robot = MWM.myGoal2Robot;

	if(touchFlag == 0)
	{
		startCount = myGetTickCount();
		touchFlag = 1;
	}
	else if(touchFlag == 1)
	{
 		move_with_ball->Move2BehingdBall(t,1000.0);
		move_with_ball->getCmd(&ret,t);
		nowCount = myGetTickCount() - startCount;
		if(nowCount > MAWARIKOMI_TOUCH_TIME * 1000)
		{
			touchFlag = 2;
		}
	}
	else if(touchFlag == 2)
	{
		if(MWM.ball2Robot.distance > MAWARIKOMI_DISTANCE_TOUCH)
		{
			angle = MWM.ball2Robot.angle;
			ROme = MWM.ball2Robot.angle;
			angle = Limit2PI(angle);
			avoid->Avoid2007(&touchVelocity, &angle, &ROme);
			basic_cmd->set_move(touchVelocity, angle, ROme);
			basic_cmd->getCmd(&ret,t);
		}
		else
		{
			if(countFlag < 5)
			{
				countFlag++;
			}
			else
			{
				countFlag = 0;
				touchVelocity = 0.0;
				touchFlag = 3;
			}
		}
	}
	else if(touchFlag == 3)
	{
		if(MWM.ball2Robot.distance < backDistance)
		{
			angle = Limit2PI(MWM.ball2Robot.angle + M_PI);
			ROme = MWM.ball2Robot.angle;
			angle = Limit2PI(angle);
			avoid->Avoid2007(&touchVelocity, &angle, &ROme);
			basic_cmd->set_move(touchVelocity, angle, ROme);
			basic_cmd->getCmd(&ret,t);
			dest = ret;
		}
		else
		{
			basic_cmd->set_stop();
			basic_cmd->getCmd(&ret,t);
			touchFlag = 0;
			dest = ret;
			return 1;
		}
	}

	MWM.ball2Robot.distance = 100.0;

    dest = ret;
	return 0;
}