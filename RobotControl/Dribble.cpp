#include "StdAfx.h"
#include "Dribble.h"
#include "WorldModel.h"
#include "OffenceParameter.h"
#include "parameters.h"
#include "BasicFunctions.h"
#include <cmath>

#define SHOOT_COUNT 2
#define TURN_SHOOT_COUNT 10

int Dribble::consider_obstacles = 1;

Dribble::Dribble()
{
	osbstacle_found = false;
	basic_cmd = new BasicCmd();
	avoid     = new Avoid();
	approach  = new ApproachBall(); 
}

Dribble::~Dribble()
{
	delete basic_cmd;
	delete avoid;
	delete approach;
}

void Dribble::getCmd(DriveVector *dv, const Time& t)
{
    *dv = dest;
}

void Dribble::dribble_straight(const Time& t,double Velocity)
{
	DriveVector ret;
	double angle, ROme;
	angle = MWM.ball2Robot.angle;
	ROme = ROTATE_GAIN * MWM.shootAngle;

	avoid->Avoid2007(&Velocity, &angle, &ROme);
	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;
}

int Dribble::dribble_turn(const Time& t,double Velocity)
{
	DriveVector ret;

	double angle, ROme;
	static double turn = 0.0;
	double goAngle;

	angle = MWM.ball2Robot.angle;
	//ROme = shootAngle;
	ROme = MWM.shootAngle;

	for(int i=0; i < MWM.obstacles_num; i++)
	{
		if( MWM.obstacle[i].distance > 0.0 && MWM.obstacle[i].distance < 500 &&
			fabs(MWM.obstacle[i].angle) < 20 * DegreeToRad)
		{
			avoid->Avoid2007(&Velocity, &angle, &ROme);
			basic_cmd->set_move(Velocity, angle,  ROTATE_GAIN * ROme);
			basic_cmd->getCmd(&ret,t);
            dest = ret;
			return SHOOT_COUNT;
		}
	}

	if( MWM.goal2Robot.distance > 0.0 && MWM.goal2Robot.distance < 4500 )
	{
		avoid->Avoid2007(&Velocity, &angle, &ROme);
		basic_cmd->set_turnMove(Velocity, angle, ROTATE_GAIN * ROme);
		basic_cmd->getCmd(&ret,t);
		//approach->fromBehindPointingToGoal();
		//approach->getCmd(&ret,t);
        dest = ret;
		return SHOOT_COUNT;
	}

	goAngle = avoid->AvoidTurn(ROme);

	if( TwoAngleDifference(ROme, goAngle) > 15 * DegreeToRad)
	{
		// TurnMove(Velocity, angle, 2.0 * goAngle);
		basic_cmd->set_turnMove(Velocity, angle, 2.0 * ROme);
		basic_cmd->getCmd(&ret,t);
		//approach->fromBehindPointingToGoal();
		//approach->getCmd(&ret,t);

	}
	else
	{
		basic_cmd->set_turnMove(Velocity, angle, 3.0 * ROme);
		basic_cmd->getCmd(&ret,t);
		//approach->fromBehindPointingToGoal();
		//approach->getCmd(&ret,t);

	}
    dest = ret;
	if( MWM.obstacle[0].distance > 0.0 && MWM.obstacle[0].distance < 2000.0
		&& MWM.obstacle[0].angle < 45.0 * DegreeToRad 
		&& MWM.obstacle[0].angle > -45.0 * DegreeToRad )
	{
		return TURN_SHOOT_COUNT;
	}
	else
	{
		return SHOOT_COUNT;
	}

}

int Dribble::dribble_turn(const Time& t,double Velocity,double angle, double ROme)
{
	DriveVector ret;

	static double turn = 0.0;
	double goAngle;

	for(int i=0; i < MWM.obstacles_num; i++)
	{
		if( MWM.obstacle[i].distance > 0.0 && MWM.obstacle[i].distance < 500 &&
			fabs(MWM.obstacle[i].angle) < 20 * DegreeToRad)
		{
			avoid->Avoid2007(&Velocity, &angle, &ROme);
			basic_cmd->set_move(Velocity, angle,  8.0 * ROme);
			basic_cmd->getCmd(&ret,t);
            dest = ret;

 			return SHOOT_COUNT;
		}
	}

	if( MWM.goal2Robot.distance > 0.0 && MWM.goal2Robot.distance < SHOOT_DISTANCE )
	{
		avoid->Avoid2007(&Velocity, &angle, &ROme);
		basic_cmd->set_turnMove(Velocity, angle, 8.0 * ROme);
		basic_cmd->getCmd(&ret,t);
        dest = ret;

		return SHOOT_COUNT;
	}

	goAngle = avoid->AvoidTurn(ROme);
//	TurnMove(1.0, angle, 70*DegreeToRad);
//	TurnMove(Velocity, angle, 2.0 * goAngle);

	if( TwoAngleDifference(ROme, goAngle) > 15 * DegreeToRad)
	{
		basic_cmd->set_turnMove(Velocity, angle, 2.0 * ROme);
		basic_cmd->getCmd(&ret,t);
	}
	else
	{
		basic_cmd->set_turnMove(Velocity, angle, 8.0 * ROme);
		basic_cmd->getCmd(&ret,t);
	}
    dest = ret;

	if( MWM.obstacle[0].distance > 0.0 && MWM.obstacle[0].distance < 2000.0
		&& MWM.obstacle[0].angle < 45.0 * DegreeToRad 
		&& MWM.obstacle[0].angle > -45.0 * DegreeToRad )
	{
		return TURN_SHOOT_COUNT;
	}
	else
	{
		return SHOOT_COUNT;
	}
}
 
 