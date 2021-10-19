#include "StdAfx.h"
#include "BallGet.h"
#include "WorldModel.h"
#include "OffenceParameter.h"
#include "BasicFunctions.h"
#include "WhiteBoard.h"
#include <cmath>

int BallGet::consider_obstacles = 1;

BallGet::BallGet() : CmdGenerator("ballGet"),
						motion(WorldModel::get_main_world_model().get_robot_properties())
{
	basic_cmd = new BasicCmd();
	avoid     = new Avoid();
	osbstacle_found = false;
	//goto_pos_skill.set_dynamics(0.5, 2, 2, 2);
}

BallGet::~BallGet()
{
	delete basic_cmd;
	delete avoid;
}

void BallGet::getCmd(DriveVector *dv, const Time& t)
{
    *dv = dest;
}
 
void BallGet::BallGet2007(const Time& t,double vel)
{
	DriveVector ret;

	double angle, Velocity, ROme;
	const double K = 0.75;	

	Velocity = vel;
	ROme = 2.0 * (MWM.goal2Robot.angle);  //4.5

	if( fabs(MWM.ball2Robot.angle) > M_PI/2.75 ) 
	{  
		if( MWM.ball2Robot.angle > 0.0 )
			 angle = MWM.ball2Robot.angle + K * 1e3 / MWM.ball2Robot.distance;
		else 
			angle = MWM.ball2Robot.angle - K * 1e3 / MWM.ball2Robot.distance;
	} 
	else 
	{
		if(MWM.ball2Robot.distance > 1000.0) 
		{ 
			 angle = MWM.ball2Robot.angle;
		}
		else
		{  
		  if(MWM.ball2Robot.angle > 0.0)
		  {
			if ( MWM.ball2Robot.distance > 600 ) 
				angle = MWM.ball2Robot.angle + K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*3. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.0;  //1.4
		  }
		  else
		  {
			if(MWM.ball2Robot.distance > 600 ) 
				angle = MWM.ball2Robot.angle - K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*3. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.0; //1.4
		  }
		  if(fabs(MWM.ball2Robot.angle) < M_PI/2.5) 
		  {
			  ROme = 2. * (MWM.ball2Robot.angle);
			  Velocity *= 0.75;
		  }
		}
	}

	if( MWM.ball2Robot.distance > DISTANCE_HAVE_BALL && 
		MWM.ball2Robot.distance < DISTANCE_HAVE_BALL + 1000.0  &&
		fabs(MWM.ball2Robot.angle) < M_PI * 0.25 
	)	
	{;
	}
	else
	{
		avoid->Avoid2007(&Velocity, &angle, &ROme);
	}

	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);

	dest = ret;
}

void BallGet::BallGetPura(const Time& t,double vel)
{
	DriveVector ret;
	double angle, Velocity, ROme;
	const double K = 0.75;	

	Velocity = vel;
	ROme = 3.0 * (MWM.goal2Robot.angle);  // 4.5 notice: the end point is robot pos

	if( fabs(MWM.ball2Robot.angle) > M_PI/2.75 ) 
	{  
		if(MWM.ball2Robot.angle > 0.0)
			angle = MWM.ball2Robot.angle  + K*1e3 / MWM.ball2Robot.distance;
		else 
			angle = MWM.ball2Robot.angle  - K*1e3 / MWM.ball2Robot.distance;
	}
	else
	{  
		if(MWM.ball2Robot.distance > 1000.0) 
		{ 
			 angle = MWM.ball2Robot.angle;   // relative ang to ball
		}
		else 
		{  
		  if(MWM.ball2Robot.angle > 0.0)
		  {
			if(MWM.ball2Robot.distance > 600 ) 
				angle = MWM.ball2Robot.angle  + K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*2. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.0;   
		  }
		  else
		  {
			if(MWM.ball2Robot.distance > 600 ) 
				angle = MWM.ball2Robot.angle  - K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*2. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.0;  
		  }

		  if(fabs(MWM.ball2Robot.angle) < M_PI/2.5 )
		  {
			  ROme = 2.0 * (MWM.ball2Robot.angle);
			  Velocity *= 0.75;
		  }
		  
		}
	}

	if (fabs(MWM.ball2Robot.angle)  < 0.3 && fabs(MWM.goal2Robot.angle) < 0.3 &&
		MWM.ball2Robot.distance < 600 )
	{
		angle = MWM.ball2Robot.angle * 1.0; 
		ROme  = 0.0;
	}

 	if( MWM.ball2Robot.distance > DISTANCE_HAVE_BALL && 
		MWM.ball2Robot.distance < DISTANCE_HAVE_BALL + 1000.0  &&
		fabs(MWM.ball2Robot.angle) < M_PI * 0.25 
		)  // && BallGetRank() == 1
	{;
	}
	else
	{
		avoid->Avoid2007(&Velocity, &angle, &ROme);
	} 
   
	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;

}

void BallGet::BallGetNew(const Time& t,double vel)
{
	DriveVector ret;
	double angle, Velocity, ROme;
	double T = 0.5;					// 90V
	double K = 0.53 + 0.05 * 1.4;	 
	
	Velocity = vel;		//MAXSPEED_BALLGET;

	if(fabs(MWM.ball2Robot.angle) < ANGLE_BEHIND_BALL_ZONE_BALL &&
		fabs(MWM.goal2Robot.angle) < ANGLE_BEHIND_BALL_ZONE_GOAL)
	{
		angle = MWM.ball2Robot.angle;
		ROme  = ROTATE_GAIN * (MWM.ball2Robot.angle /*+ MWM.ball2Robot.sub_angle*/ ); // lrd
	}
	else
	{
		ROme = ROTATE_GAIN * (MWM.goal2Robot.angle/* + MWM.goal2Robot.sub_angle*/);
		if(fabs(MWM.ball2Robot.angle) > HIKKAKARU1 && 
			fabs(MWM.ball2Robot.angle) < HIKKAKARU2 && 
			MWM.ball2Robot.distance < DISTANCE_HIKKAKARU_BALL)
		{
			ROme = ROTATE_GAIN * (MWM.ball2Robot.angle /*+ MWM.ball2Robot.sub_angle*/);
		}
		if( (fabs(MWM.ball2Robot.angle - MWM.goal2Robot.angle) > T*M_PI) || 
			fabs(MWM.ball2Robot.angle) > M_PI * 0.5)
		{
			if(MWM.ball2Robot.angle > 0.0)
			{
				angle = MWM.ball2Robot.angle + K * 1e3 / MWM.ball2Robot.distance;
			}
			else
			{		
				angle = MWM.ball2Robot.angle - K * 1e3 / MWM.ball2Robot.distance;
			}
		}
		else if(fabs(MWM.ball2Robot.angle) > 90 * DegreeToRad	&& 
				MWM.ball2Robot.distance >0.					&&
				MWM.ball2Robot.distance < 500 )
		{
			angle = MWM.ball2Robot.angle + M_PI;
		}
		else
		{
			angle = Limit2PI(MWM.ball2Robot.angle + KEISU_MAWARIKOMI*(MWM.ball2Robot.angle-MWM.goal2Robot.angle));
		}
	}

	if( MWM.ball2Robot.distance > DISTANCE_HAVE_BALL && 
		MWM.ball2Robot.distance < DISTANCE_HAVE_BALL + 1000.0  &&
		fabs(MWM.ball2Robot.angle) < M_PI * 0.25)
	{;
	}
	else
	{
	  avoid->Avoid2007(&Velocity, &angle, &ROme);
	}
	angle = Limit2PI(angle);

	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;
}

void BallGet::BallGetDF(const Time& t,double vel)
{
	DriveVector ret;

	double angle, Velocity, ROme;
	const	double	K = 0.75;
	Vector tmpGoal;
	if(MWM.myGoal2Robot.distance > 0.0)
	{
		tmpGoal = MWM.myGoal2Robot;
		tmpGoal.angle = Limit2PI(tmpGoal.angle + M_PI);
	}
	else
	{
		tmpGoal = MWM.goal2Robot;
	}

	Velocity = vel;
	ROme = 3.0 * (tmpGoal.angle);  //4.5

	if(fabs(MWM.ball2Robot.angle) > M_PI/2.75) 
	{  
		if(MWM.ball2Robot.angle > 0.0)
			 angle = MWM.ball2Robot.angle + K *1e3/ MWM.ball2Robot.distance;
		else angle = MWM.ball2Robot.angle - K *1e3/ MWM.ball2Robot.distance;
	}
	else
	{  
		if(MWM.ball2Robot.distance > 1000.0) 
		{ 
			 angle = MWM.ball2Robot.angle;
		}
		else
		{  
		  if(MWM.ball2Robot.angle > 0.0)
		  {
			if(MWM.ball2Robot.distance > 600.0 ) 
				angle = MWM.ball2Robot.angle + K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*2. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.4;  //1.4
		  }
		  else
		  {
			if(MWM.ball2Robot.distance > 600.0 ) 
				angle = MWM.ball2Robot.angle - K*1e3*fabs(MWM.ball2Robot.angle)/M_PI*2. / MWM.ball2Robot.distance;
			else 
				angle = MWM.ball2Robot.angle * 1.4; //1.4
		  }
		  if(fabs(MWM.ball2Robot.angle) < M_PI/2.5) 
		  {
			  ROme = 2. * (MWM.ball2Robot.angle);
			  Velocity *= 0.75;
		  }
		}
	}
	if( MWM.ball2Robot.distance > DISTANCE_HAVE_BALL && 
		MWM.ball2Robot.distance < DISTANCE_HAVE_BALL + 1000.0  &&
		fabs(MWM.ball2Robot.angle) < M_PI * 0.25)
	{;
	}
	else
	{
		avoid->Avoid2007(&Velocity, &angle, &ROme);
	}

	basic_cmd->set_move(Velocity, angle, ROme);
	basic_cmd->getCmd(&ret,t);
	dest = ret;

}
