#include "StdAfx.h"
#include "SPhysGotoPos.h"
#include "BasicFunctions.h"
#include "WorldModel.h"
#include <cmath>

using namespace std;

int SPhysGotoPos::consider_obstacles = 1;
 
SPhysGotoPos::SPhysGotoPos () throw () : CmdGenerator ("SPhysGotoPos"),use_pd_control(false),needToTurn(false)
{
	acc_delay = 100;
	const RobotProperties& rp = (MWM.get_robot_properties());
	set_dynamics( rp.max_velocity, rp.max_rotational_velocity, 
		rp.max_acceleration, rp.max_rotational_acceleration );
	init(Vec::zero_vector, 0, true);
	avoid = new Avoid();
}

SPhysGotoPos::~SPhysGotoPos() throw()
{
	delete avoid;
}

void SPhysGotoPos::init (Vec tp, Angle ta, bool st, bool tolp, bool tolh) throw () 
{
	max_target_velocity = (st ? 0.0 : max_tv);
	target_pos = tp;
	target_heading = ta;
	tolerance_pos = tolp;
	tolerance_heading = tolh;
}

void SPhysGotoPos::init (Vec tp, Angle ta, double mtv, bool tolp, bool tolh) throw () 
{
	max_target_velocity = (mtv>max_tv ? max_tv : mtv);
	target_pos = tp;
	target_heading = ta;
	tolerance_pos = tolp;
	tolerance_heading = tolh;
}

void SPhysGotoPos::set_target(bool pd, Vec pos, double vel,int cons_obstacles   ) throw ()
{
	target_pos = pos;
	max_target_velocity = vel;
	use_pd_control = pd;
	needToTurn = false;
	consider_obstacles = cons_obstacles;
}

void SPhysGotoPos::set_target(bool pd, Vec pos, Angle ang, double vel, double w,int cons_obstacles  ) throw ()
{
	target_pos = pos;
	target_heading = ang;
	max_target_velocity = vel;
	max_target_vrot = w;
	use_pd_control = pd;
	needToTurn = true;
	consider_obstacles = cons_obstacles;
}

void SPhysGotoPos::set_dynamics (double vt, double vr, double at, double ar) throw () 
{
	max_tv = vt;
	max_rv = vr;
	max_ta = at;
	max_ra = ar;
	max_td = 0.75*max_ta;  // 负的平移加速度
	max_rd = 0.75*max_ra;
}

void SPhysGotoPos::get_dynamics (double& vt, double& vr, double& at, double& ar, double& bt, double& br) throw () 
{
	vt = max_tv;
	vr = max_rv;
	at = max_ta;
	ar = max_ra;
	bt = max_td;
	br = max_rd;
}

void SPhysGotoPos::getCmd(DriveVector* dest, const Time& texec) throw() 
{
	if (use_pd_control)
	{
		getCmd(dest,texec,use_pd_control);
		return;
	}
	const RobotLocation& rloc_exec = MWM.get_robot_location(texec);
	Time tsmooth = texec;
	tsmooth.add_msec(-acc_delay);
	const RobotLocation& rloc_smooth = MWM.get_robot_location(tsmooth, false);
	
	LOUT << "\\% red cross " << target_pos << " line " << target_pos << ' ' 
		<< target_pos+1000*Vec::unit_vector(target_heading+Angle::quarter) << '\n';
	
	double diff_heading = (target_heading-rloc_exec.heading).get_rad_pi();
	if (tolerance_heading) 
	{
		if (fabs(diff_heading)<0.06) 
			diff_heading=0;  
		if (fabs(diff_heading)<0.12 && fabs(rloc_exec.vrot)<1e-2) 
			diff_heading=0; 
	}
	if (diff_heading==0) 
		dest->vrot=0;
	else if (diff_heading>0)
		dest->vrot = max_rv;
	else
		dest->vrot = -max_rv;
	
	// 旋转制动的距离公式
	{
		double m = sqrt( 2*max_rd*fabs(diff_heading) );  
		if (dest->vrot<-m) dest->vrot = -m;
		if (dest->vrot>m)  dest->vrot = m;
	}
    
	if (1e3*(fabs(dest->vrot)-rloc_smooth.vrot)>max_ra*acc_delay) 
	{
		double delta = 1e-3*acc_delay*max_ra;
		if (dest->vrot<rloc_smooth.vrot-delta) dest->vrot = rloc_smooth.vrot-delta;
		if (dest->vrot>rloc_smooth.vrot+delta) dest->vrot = rloc_smooth.vrot+delta;
	}
 	
	Vec diff_pos = target_pos-rloc_exec.pos;
	if (tolerance_pos) 
	{
		if (diff_pos.length()<50) diff_pos=Vec::zero_vector;  
		if (diff_pos.length()<100 && rloc_exec.vtrans.length()<1e-2) diff_pos=Vec::zero_vector; 
	}
	double desired_velocity = 0;
	if (diff_pos.length()>1)
		desired_velocity = max_tv; 
	
	
	if (desired_velocity>max_target_velocity) 
	{
		double m = sqrt (max_target_velocity*max_target_velocity+2e-3*max_td*fabs(diff_pos.length()));  
		if (desired_velocity<-m) desired_velocity=-m;
		if (desired_velocity>m) desired_velocity=m;
	}
	dest->vtrans = desired_velocity*diff_pos.normalize();
	
	double desired_acc = 1e3*(dest->vtrans-rloc_smooth.vtrans).length()/acc_delay;
	if (desired_acc>max_ta)
		dest->vtrans = rloc_smooth.vtrans+(max_ta/desired_acc)*(dest->vtrans-rloc_smooth.vtrans);
	
	if( fabs(dest->vrot) >= 1e-2 ) 
	{
		Angle half_expected_rotation = Angle::rad_angle( 0.5e-3*dest->vrot*MWM.get_game_state().actual_cycle_time);
		dest->vtrans /= (half_expected_rotation);
	}
	
	dest->vtrans /= rloc_exec.heading;
 	
	if( consider_obstacles )
	{
 		double goAngle  = dest->vtrans.angle().get_rad();
		double velocity = dest->vtrans.length();
		double rome     = dest->vrot;
		avoid->Avoid2007(&velocity,&goAngle,&rome);
		goAngle = Limit2PI(goAngle);
		dest->vtrans.x = velocity * cos(goAngle);
		dest->vtrans.y = velocity * sin(goAngle);
		dest->vrot     = rome; 
	}
	
}

void SPhysGotoPos::getCmd(DriveVector* dv, const Time& t,bool pd ) throw() 
{
	const RobotLocation& rloc_exec = MWM.get_robot_location(t);
	Time tsmooth = t;
	tsmooth.add_msec(-acc_delay);
	const RobotLocation& rloc_smooth = MWM.get_robot_location(tsmooth, false);
	
	Vec current_position = rloc_smooth.pos;
	Angle current_heading  = rloc_smooth.heading;
  	
	Vec target_vector = ( target_pos - current_position );
	static double pos_e1 = 0;
	double pos_e = target_vector.length();
	double speed = 0.6*(1.5*pos_e+1.5*(pos_e-pos_e1)) * 1e-3;
	double Maxvel = max_target_velocity;
	if(speed > Maxvel) 		
		speed = Maxvel;

 	Vec target_vtrans = speed * target_vector.normalize();    // max  vtans
	pos_e1 = pos_e;

	dv->vtrans = target_vtrans.rotate(-current_heading);
	
	if( needToTurn )
	{
		Angle d_angle = target_heading - current_heading;
		static double d_angle_d1 = 0;
		int turn_direction = 
			( d_angle.in_between (Angle::zero, Angle::three_eighth) ? 
			+1 : ( d_angle.in_between (Angle::five_eighth, Angle::zero) ? -1 : (rloc_smooth.vrot>=0 ? +1 : -1 )));   
		
		double d_angle_d = ( turn_direction == 1 ? d_angle.get_rad() : 2*M_PI-d_angle.get_rad());
		double w = 1.5 * d_angle_d + 2.0 * (d_angle_d - d_angle_d1);
		
		double target_vrot = turn_direction * min( fabs(w), max_target_vrot );   // max vrot

		double temp = pos_e + fabs(d_angle.get_rad_pi()) * 220.5;
		if( temp < 3 )
		{
			dv->vtrans = Vec::zero_vector;
			dv->vrot   = 0.0;
			return;
		}
		
/*	 	if( target_vector.length() < 365 )
			target_vtrans = Vec(0,0);
		if( d_angle.in_between (Angle::rad_angle (-0.1), Angle::rad_angle (0.1))) // 0.1
			target_vrot = 0; */
	    
	    d_angle_d1 = d_angle_d;
		dv->vrot   = target_vrot;

	}

	if( consider_obstacles ) 
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
