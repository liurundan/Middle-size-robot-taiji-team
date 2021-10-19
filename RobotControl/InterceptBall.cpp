#include "StdAfx.h"
#include "InterceptBall.h"
#include "WorldModel.h"
#include "parameters.h"
#include "Tools.h"
#include "BasicFunctions.h"
#include <cmath>

InterceptBall::InterceptBall() throw() : CmdGenerator ("InterceptBall"),motion(WorldModel::get_main_world_model().get_robot_properties())
{
	ivRequestedTurnAngle = 0.0; 
	intercept_max_cycles = 1;
	tolerance_pos     = true;
	tolerance_heading = true;
	move_with_ball    = new MoveWithBall();
	avoid             = new Avoid();
}

InterceptBall::~InterceptBall()
{
	delete move_with_ball;
	delete avoid;
}

void InterceptBall::getCmd(DriveVector* dv, const Time& t) throw() 
{
	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t);  
 
  	Vec ball_vel = bloc.velocity;
    Vec destination;
	double ball_decay = 0.94;
	Vec ball_new_pos = bloc.pos;
	Vec ball_new_vel = bloc.velocity;
	
	if( rloc.pos.sqr_distance(bloc.pos) > SQUARE(420) )
	{
		int i = 0;
	    Vec my_vel;
	    Vec my_pos;
		do 
		{
			ball_new_pos += ball_new_vel * 33;
		    ball_new_vel *= 0.94;
			my_vel = 2.0 * ( ball_new_pos - my_pos ).normalize();
			my_pos += my_vel * 33;
		} 
		while( !is_destination_reachable2(ball_new_pos, my_pos, my_vel, ++i) );
    	intercept_max_cycles = i;
	}
	if (intercept_max_cycles > 1)
	   destination = ball_new_pos;
	else 
	   destination = bloc.pos;
	
	current_position = rloc.pos;
	current_heading  = rloc.heading;
	
	target_pos  = destination;
	target_heading = ( destination - current_position ).angle();
	
	Vec target_vector = ( destination - current_position );
	static double pos_e1 = 0;
	double pos_e = target_vector.length();
	double speed = 0.6*(1.5*pos_e+1.5*(pos_e-pos_e1)) * 1e-3;
	double Maxvel = 2.0;
	if(speed > Maxvel) 
		speed = Maxvel;
	//speed = 0;
	target_vtrans = speed * target_vector.normalize();    // max  vtans
	pos_e1 = pos_e;
	
	Angle d_angle = target_heading - current_heading;
	static double d_angle_d1 = 0;
	int turn_direction = 
		( d_angle.in_between (Angle::zero, Angle::three_eighth) ? 
		+1 : ( d_angle.in_between (Angle::five_eighth, Angle::zero) ? -1 : (rloc.vrot>=0 ? +1 : -1 )));   
	
	double d_angle_d = ( turn_direction == 1 ? d_angle.get_rad() : 2*M_PI-d_angle.get_rad());
	double w = 1.5 * d_angle_d + 2.0 * (d_angle_d - d_angle_d1);
	
	double max_rome = (fabs( (target_pos/current_heading).angle().get_rad_pi())) * pos_e / Maxvel;
	if (max_rome < 8.0)	max_rome = 8.0;

	target_vrot = turn_direction * min( fabs(w), max_rome );   // max vrot
  		
	d_angle_d1 = d_angle_d;
	
	dv->vtrans = target_vtrans.rotate(-current_heading);
	dv->vrot   = target_vrot;
    
	if( pos_e > 1500 )
	{
		double goAngle  = dv->vtrans.angle().get_rad();
		double velocity = dv->vtrans.length();
		double rome     = dv->vrot;
		avoid->Avoid2007(&velocity,&goAngle,&rome);
		goAngle = Limit2PI(goAngle);
		dv->vtrans.x = velocity * cos(goAngle);
		dv->vtrans.y = velocity * sin(goAngle);
		dv->vrot     = rome; 
	}

}
 
bool InterceptBall::is_destination_reachable2(const Vec& destination,Vec my_pos, Vec my_vel , int maxcycles )
{
   	Vec my_new_pos = my_pos;
	Vec my_new_vel = my_vel;
	
	while( maxcycles-- > 0 )
	{
 		my_new_pos += my_new_vel * 33;
 		if( (destination - my_new_pos).length() <= 580 )
			return true;
	}
    return false;
}

bool InterceptBall::intercept_close(DriveVector* dv, const RobotLocation&rloc, const BallLocation& bloc)
{
 	current_position = rloc.pos;
	current_heading  = rloc.heading;

	target_pos     = bloc.pos;
	target_heading = ( bloc.pos - rloc.pos ).angle();
	Vec target_vector = ( target_pos - current_position );
	static double pos_e1 = 0;
	double pos_e = target_vector.length();
	double speed = 0.6*(1.5*pos_e+1.5*(pos_e-pos_e1)) * 1e-3;
	double Maxvel = 2.2;
	if(speed > Maxvel) 
		speed = Maxvel;
	 
	target_vtrans = speed * target_vector.normalize();    // max  vtans
	pos_e1 = pos_e;
	
	Angle d_angle = target_heading - current_heading;
	static double d_angle_d1 = 0;
	int turn_direction = 
		( d_angle.in_between (Angle::zero, Angle::three_eighth) ? 
		+1 : ( d_angle.in_between (Angle::five_eighth, Angle::zero) ? -1 : (rloc.vrot>=0 ? +1 : -1 )));   
	
	double d_angle_d = ( turn_direction == 1 ? d_angle.get_rad() : 2*M_PI-d_angle.get_rad());
	double w = 1.5 * d_angle_d + 2.0 * (d_angle_d - d_angle_d1);
	
	target_vrot = turn_direction * min( fabs(w), 8.0 );   // max vrot
	
	if( target_vector.length() < 365 && d_angle.in_between (Angle::rad_angle (-0.8), Angle::rad_angle (0.8)))
 		target_vtrans = Vec(0,0);
     	 
	d_angle_d1 = d_angle_d;
	
	dv->vtrans = target_vtrans.rotate(-current_heading);
	dv->vrot   = target_vrot;
	
    return true;
}
 
