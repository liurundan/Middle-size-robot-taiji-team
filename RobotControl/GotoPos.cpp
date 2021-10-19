#include "StdAfx.h"
#include "GotoPos.h"
#include "WorldModel.h"
#include "BasicFunctions.h"
#include "InPlayParameters.h"
#include <cmath>

int GotoPos::consider_obstacles = 1;

GotoPos::GotoPos(): usePD(false)
{
	target_tolerance = 1000.0;
	basic_cmd = new BasicCmd();
	avoid     = new Avoid();
	osbstacle_found = false;
}

GotoPos::~GotoPos()
{
	delete basic_cmd;
	delete avoid;
}

void GotoPos::set_target( Vec target, double velocity, 
						  double target_tolerance /* = 1000.0 */, int cons_obstacles /* = 1  */,bool usePD )
{
	target_pos = target;
	this->velocity = velocity;
	this->target_tolerance = target_tolerance;
	consider_obstacles = cons_obstacles;
	this->usePD = usePD;
}

void GotoPos::getCmd(DriveVector *dv, const Time& t)
{
	if (usePD)
	{
		getCmd(dv,t,usePD);
		return;
	}
	double x = target_pos.x;
	double y = target_pos.y;
	
	double sideX, sideY;
	static double moveAngle = 0.0;
	double	myAngle, tmpAngle, ROme;
	double	rotateAngle = 0;
	int		returnValue = 0;
	int		checkPosition=0, checkAngle=0;
 
	if( MWM.goal2Robot.distance < 0.0 || MWM.myGoal2Robot.distance < 0.0)
	{
		double velocity, angle, ROme;
		angle = MWM.fieldCenterAngle;
		ROme = 0.0;
		velocity = INPLAY_VELOCITY * 0.5;
		avoid->Avoid2007(&velocity, &angle, &ROme);
		angle = Limit2PI(angle);
		basic_cmd->set_move(velocity,angle,ROme);
		basic_cmd->getCmd(dv,t);
	}

	if( sqrt( (MWM.ballPosition.x - x) * (MWM.ballPosition.x - x) +
		(MWM.ballPosition.y - y) * (MWM.ballPosition.y - y) ) < 2000.0 )
	{
	//	x -= 2000.0;
	} 

	myAngle = Angle2Positive(MWM.myPosition.angle);

	sideX = fabs( x - MWM.myPosition.x );
	sideY = fabs( y - MWM.myPosition.y );

	if( MWM.ball2Robot.distance > 0)
	{
		Object obstacle_ball;
 		obstacle_ball.angle = MWM.ball2Robot.angle;
		obstacle_ball.distance = MWM.ball2Robot.distance;
		obstacle_ball.maxAngle= Limit2PI(MWM.ball2Robot.angle + 0.12);
		obstacle_ball.minAngle= Limit2PI(MWM.ball2Robot.angle - 0.12);
		MWM.obstacle.push_back(obstacle_ball);
		MWM.obstacles_num = MWM.obstacles_num + 1;
	}
    double dist_tol = 500.0;
	if( sideX > dist_tol || sideY > dist_tol )
	{
		if(sideX != 0)
		{
			tmpAngle = atan(sideY/sideX);
		}
		else
		{
			tmpAngle = 90*DegreeToRad;
		}

		if(y > MWM.myPosition.y)
		{
			if(x > MWM.myPosition.x)
			{
				moveAngle = fabs(myAngle - tmpAngle);
				if(myAngle > tmpAngle)
					moveAngle = -moveAngle;
				moveAngle = Limit2PI(moveAngle);
			}
			else
			{
				myAngle += M_PI;
				moveAngle = myAngle + tmpAngle;
				moveAngle = -moveAngle;
				moveAngle = Limit2PI(moveAngle);
			}
		}
		else
		{
			if(x > MWM.myPosition.x)
			{
				moveAngle = myAngle + tmpAngle;
				moveAngle = -moveAngle;
				moveAngle = Limit2PI(moveAngle);
			}
			else
			{
				myAngle += M_PI;
				moveAngle = myAngle - tmpAngle;
				moveAngle = -moveAngle;
				moveAngle = Limit2PI(moveAngle);			
			}
		}
	}
	else
	{
		velocity = 0;
		moveAngle = 0;
		checkPosition = 1;
		returnValue = 1;
	}

	ROme = MWM.goal2Robot.angle;
	avoid->Avoid2007(&velocity, &moveAngle, &ROme);
 
 	basic_cmd->set_move(velocity, moveAngle, ROme);
	basic_cmd->getCmd(dv,t);
}

void GotoPos::getCmd(DriveVector *dv, const Time& t, bool usePD )
{
	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t);  

	Vec current_position = rloc.pos;
	Angle current_heading  = rloc.heading;
	
	target_pos  = bloc.pos;
	target_heading = ( bloc.pos - current_position ).angle();
	
	Vec target_vector = ( target_pos - current_position );
	static double pos_e1 = 0;
	double pos_e = target_vector.length();
	double speed = 0.6*(1.5*pos_e+1.5*(pos_e-pos_e1)) * 1e-3;
	double Maxvel = 2.0;
	if(speed > Maxvel) 
		speed = Maxvel;
	//speed = 0;
	Vec target_vtrans = speed * target_vector.normalize();    // max  vtans
	pos_e1 = pos_e;
	
	Angle d_angle = target_heading - current_heading;
	static double d_angle_d1 = 0;
	int turn_direction = 
		( d_angle.in_between (Angle::zero, Angle::three_eighth) ? 
		+1 : ( d_angle.in_between (Angle::five_eighth, Angle::zero) ? -1 : (rloc.vrot>=0 ? +1 : -1 )));   
	
	double d_angle_d = ( turn_direction == 1 ? d_angle.get_rad() : 2*M_PI-d_angle.get_rad());
	double w = 1.5 * d_angle_d + 2.0 * (d_angle_d - d_angle_d1);
	
	double target_vrot = turn_direction * min( fabs(w), 8.0 );   // max vrot
	
	if( target_vector.length() < 365 )
		target_vtrans = Vec(0,0);
	if( d_angle.in_between (Angle::rad_angle (-0.1), Angle::rad_angle (0.1))) // 0.1
		target_vrot = 0;
	//if( d_angle.in_between(Angle::rad_angle (-0.2), Angle::rad_angle (0.2)) == false ) // 0.1
	//	target_vtrans = Vec::zero_vector;
	
	d_angle_d1 = d_angle_d;
	
	dv->vtrans = target_vtrans.rotate(-current_heading);
	dv->vrot   = target_vrot;
}

