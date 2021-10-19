#include "StdAfx.h"
#include "Offence.h"
#include "OffenceParameter.h"
#include "InPlayParameters.h"
#include "parameters.h"
#include "position.h"
#include "WorldModel.h"
#include <cmath>

Offence::Offence()
{
	basic_cmd = new BasicCmd();
	dribble   = new Dribble();
	getBall   = new BallGet();
}

Offence::~Offence()
{
   delete basic_cmd;
   delete dribble;
   delete getBall;
}
 
void Offence::getCmd(DriveVector *dv, const Time& t)
{
    *dv = dest;
}

void Offence::SideAttack( const Time& t)
{
	double angle = 0.0;
	
	if(MWM.myPosition.available == TRUE)
	{
		if( MWM.myGoal2Robot.distance < FIELD_HALF_LENGTH * 0.5)
		{
			angle = atan( (FIELD_WIDTH) / (FIELD_HALF_LENGTH - MWM.myPosition.x) );
		}
		else if(MWM.myGoal2Robot.distance < FIELD_HALF_LENGTH)
		{
			angle = atan( (FIELD_HALF_WIDTH) / (FIELD_HALF_LENGTH - MWM.myPosition.x) );
		}
		if(MWM.myPosition.y > -500)
			MWM.goal2Robot.angle += angle;
		else if(MWM.myPosition.y < 500 )
			MWM.goal2Robot.angle -= angle;
	}
	OffenceFW(t, INPLAY_VELOCITY);
}

void Offence::OffenceFW( const Time& t, double offenceVelocity )
{
	DriveVector ret;
	
	static int shootFlag = 0;
	double shootDistance = SHOOT_DISTANCE;
	
	int goal_count;

	if( MWM.ball2Robot.distance > DISTANCE_HAVE_BALL || fabs(MWM.ball2Robot.angle) > 10*DegreeToRad)
	{
		getBall->BallGetPura(t,offenceVelocity);
		getBall->getCmd(&ret,t);
		 
		shootFlag = 0;
	}
	else if( MWM.goal2Robot.distance > shootDistance )
	{
 		goal_count = dribble->dribble_turn(t,offenceVelocity+0.1); 
		dribble->getCmd(&ret,t);
		shootFlag = 0;
	}
	else
	{
 		goal_count = dribble->dribble_turn(t,offenceVelocity+0.1); 
		dribble->getCmd(&ret,t);

		if ( ( ( MWM.oppGoalObject.maxAngle > 0. && MWM.oppGoalObject.minAngle < 0. ) ||
			( MWM.oppGoalObject.maxAngle < 0. && MWM.oppGoalObject.minAngle > 0. ) ) &&
			( fabs( MWM.ball2Robot.angle ) < 30*DegreeToRad ) )
		{
			shootFlag++;
		}
		else
		{
			shootFlag = 0;	
		}
	
		if( ( shootFlag >= goal_count * 2 )  && MWM.ball2Robot.distance > 0.0 )
		{
			basic_cmd->set_kick(15); 
			basic_cmd->getCmd(&ret,t);
			shootFlag = 0;
		}
	}
	dest = ret;
}

void Offence::OffenceDF(const Time& t,double offenceVelocity)
{
	DriveVector ret;

	static int shootFlag = 0;
	double shootDistance = SHOOT_DISTANCE;
	
	int goal_count;
 
	if(MWM.ball2Robot.distance > DISTANCE_HAVE_BALL || fabs(MWM.ball2Robot.angle) > 10*DegreeToRad)
	{
		getBall->BallGetDF(t,offenceVelocity);
		getBall->getCmd(&ret,t);
		 
		shootFlag = 0;
	}
	else if( MWM.goal2Robot.distance > shootDistance )
	{
 		goal_count = dribble->dribble_turn(t,offenceVelocity+0.1); 
		dribble->getCmd(&ret,t);
		shootFlag = 0;
	}
	else
	{
 		goal_count = dribble->dribble_turn(t,offenceVelocity+0.1); 
		dribble->getCmd(&ret,t);

		if ( ( ( MWM.oppGoalObject.maxAngle > 0. && MWM.oppGoalObject.minAngle < 0. ) ||
			( MWM.oppGoalObject.maxAngle < 0. && MWM.oppGoalObject.minAngle > 0. ) ) &&
			( fabs( MWM.ball2Robot.angle ) < 30*DegreeToRad ) )
		{
		    shootFlag++;
		}
		else
		{
			shootFlag = 0;	
		}
	
		if( ( shootFlag >= goal_count * 2 )  && MWM.ball2Robot.distance > 0.)
		{
			basic_cmd->set_kick(15); 
			basic_cmd->getCmd(&ret,t);
			shootFlag = 0;
		}
	}
	
	dest = ret;
}