#include "StdAfx.h"
#include "FaceBall.h"
#include "WorldModel.h"
#include "Tools.h"
#include <cmath>

FaceBall::FaceBall():use_pd_control(false)
{
	basic_cmd = new BasicCmd();
}

FaceBall::~FaceBall()
{
	delete basic_cmd;
}

void FaceBall::turn_to_ball(double omega,bool tolerance_heading, bool pd_control )
{
	Vec ball_pos = MWM.posBall;
	this->omega = omega;
	this->tolerance_heading = tolerance_heading;
	desired_dir = Tools::my_abs_angle_to(ball_pos);
	turn_body = true;
	use_pd_control = pd_control;
}

void FaceBall::turn_in_dir(Angle dir,double omega /* = 1.0 */,bool tolerance_heading,bool pd_control )
{
   desired_dir = dir;
   this->omega = omega;
   this->tolerance_heading = tolerance_heading;
   turn_body = true;
   use_pd_control = pd_control;
}

void FaceBall::turn_to_point(Vec target, double omega /* = 1.0 */,bool tolerance_heading,bool pd_control)
{
	desired_dir = Tools::my_abs_angle_to(target);
	this->omega = omega;
	this->tolerance_heading = tolerance_heading;
	turn_body = true;
	use_pd_control = pd_control;
}

void FaceBall::getCmd( DriveVector *dv, const Time& t  )
{
	if (use_pd_control)
	{
		getCmd(dv,t,use_pd_control);
		return;
	}
	bool res = true;
	RobotLocation rloc = MWM.get_robot_location(t);  
	
	Angle target_dir = desired_dir;
	Angle robot_heading = MWM.heading_me;
	double diff_heading = (target_dir-robot_heading).get_rad_pi();
	if( tolerance_heading ) 
	{
		if (fabs(diff_heading)<0.3) 
			diff_heading=0;  
		if (fabs(diff_heading)<0.12 && fabs(rloc.vrot)<1e-2) 
			diff_heading=0; 
	}
	if (diff_heading==0) 
		dv->vrot=0;
	else if (diff_heading>0)
		dv->vrot = omega;
	else
		dv->vrot = -omega;

	double acc_delay = 100;
	double max_ra = 12.0;
	double max_rd = 7.5;
	// 旋转制动的距离公式
	{
		double m = sqrt( 2*max_rd*fabs(diff_heading) );  
		if (dv->vrot<-m) dv->vrot = -m;
		if (dv->vrot>m)  dv->vrot = m;
	}
    
	if (1e3*(fabs(dv->vrot)-rloc.vrot) > max_ra*acc_delay) 
	{
		double delta = 1e-3*acc_delay*max_ra;
		if (dv->vrot<rloc.vrot-delta) dv->vrot = rloc.vrot-delta;
		if (dv->vrot>rloc.vrot+delta) dv->vrot = rloc.vrot+delta;
	}
}

void FaceBall::getCmd(DriveVector *dv, const Time& t, bool pd_control)
{
	bool res = true;
	RobotLocation rloc = MWM.get_robot_location(t);  
	
	Angle target_dir = desired_dir;
	Angle robot_heading = rloc.heading;
	Angle d_angle = target_dir - robot_heading;
	static double d_angle_d1 = 0.0;
	int turn_direction = 
		( d_angle.in_between (Angle::zero, Angle::three_eighth) ? 
		+1 : ( d_angle.in_between (Angle::five_eighth, Angle::zero) ? -1 : (rloc.vrot>=0 ? +1 : -1 )));  
	double d_angle_d = ( turn_direction == 1 ? d_angle.get_rad() : 2*M_PI-d_angle.get_rad());
	double w = 1.5 * d_angle_d + 2.0 * (d_angle_d - d_angle_d1);
 	double target_vrot = turn_direction * min( fabs(w), omega );   // max vrot
    d_angle_d1 = d_angle_d;
	
    dv->vrot = target_vrot;  
}

double FaceBall::abs_turnangle(Angle h)
{
	if (h.get_rad() > M_PI)
	{
		h -= 2*M_PI;
	}
	return (fabs(h.get_rad()));
}