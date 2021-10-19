#include "StdAfx.h"
#include "RobotPositionKalmanFilter.h"

#include <cmath>
#include "WorldModel.h"


using namespace std;

#define DEBUG_KFILTER 0

#define ZWEIPI 6.283185307179586232

void RobotPositionKalmanFilter::update (Vec delta_pos, Angle delta_heading, 
										Vec vis_pos, Angle vis_heading, Vec var_vis_pos, 
										double var_vis_heading) throw () 
{
	update (delta_pos, delta_heading, true);
	
#if DEBUG_KFILTER
	WorldModel::get_main_world_model().log_stream() << "KFilter Bild: " << vis_pos.x << ' ' 
		<< vis_pos.y << ' ' << vis_heading.get_deg() << ' ' << sqrt(var_vis_pos.x) << ' ' 
		<< sqrt(var_vis_pos.y) << ' ' << sqrt(var_vis_heading) << '\n';
#endif
	
	double v1, v2;
	v1=var_pos.x;
	v2=var_vis_pos.x;
	pos.x = (v2*pos.x+v1*vis_pos.x)/(v1+v2);
	var_pos.x = (v1*v2)/(v1+v2);
	v1=var_pos.y;
	v2=var_vis_pos.y;
	pos.y = (v2*pos.y+v1*vis_pos.y)/(v1+v2);
	var_pos.y = (v1*v2)/(v1+v2);
	v1=var_heading;
	v2=var_vis_heading;
	double vis_head=vis_heading.get_rad();
	if (heading-vis_head<=-M_PI)
		vis_head-=ZWEIPI;
	heading = (v2*heading+v1*vis_head)/(v1+v2);
	var_heading = (v1*v2)/(v1+v2);
	if (heading>M_PI)
		heading-=ZWEIPI;
	if (heading<=-M_PI)
		heading+=ZWEIPI;
	
#if DEBUG_KFILTER
	WorldModel::get_main_world_model().log_stream() << "KFilter Fusion: " << pos.x << ' ' << pos.y << ' ' << heading*180/M_PI << ' ' << sqrt(var_pos.x) << ' ' << sqrt(var_pos.y) << ' ' << sqrt(var_heading) << '\n';
#endif
}

void RobotPositionKalmanFilter::update (Vec delta_pos, Angle delta_heading, bool vis_available) throw () 
{
	
#if DEBUG_KFILTER
	WorldModel::get_main_world_model().log_stream() << "KFilter Ausgangspunkt: " << pos.x 
		<< ' ' << pos.y << ' ' << heading*180/M_PI << ' ' << sqrt(var_pos.x) << ' ' << sqrt(var_pos.y) << ' ' << sqrt(var_heading) << '\n';
#endif
	delta_pos = delta_pos.rotate (Angle::rad_angle(heading));  
	
	Vec std_delta_pos (max (0.375*abs(delta_pos.x),100.0), max (0.375*abs(delta_pos.y),100.0));
	
	if (vis_available) 
		delta_pos*=0.75;
	
	double delta_head_pi = delta_heading.get_rad();
	if (delta_head_pi>M_PI)
		delta_head_pi-=ZWEIPI;
	double std_delta_head = max(0.375*fabs(delta_head_pi),0.05);
	double delta_head = (vis_available ? 0.75 : 1.0)*delta_head_pi;
	
#if DEBUG_KFILTER
	WorldModel::get_main_world_model().log_stream() << "KFilter delta_heading: " << delta_head*180/M_PI << '\n';
#endif
	
	var_pos.x+=std_delta_pos.x*std_delta_pos.x;
	pos.x+=delta_pos.x;
	var_pos.y+=std_delta_pos.y*std_delta_pos.y;
	pos.y+=delta_pos.y;
	var_heading+=std_delta_head;
	heading += delta_head;
	if (heading>M_PI)
		heading-=ZWEIPI;
	if (heading<=-M_PI)
		heading+=ZWEIPI;
	
#if DEBUG_KFILTER
	WorldModel::get_main_world_model().log_stream() << "KFilter Odometrie: " << pos.x << ' ' << pos.y << ' ' << heading*180/M_PI << ' ' << sqrt(var_pos.x) << ' ' << sqrt(var_pos.y) << ' ' << sqrt(var_heading) << '\n';
#endif
}

void RobotPositionKalmanFilter::set(Vec p, Angle h, Vec vp, double vh) throw () 
{
	pos=p;
	heading=h.get_rad();
	if( heading>M_PI )
		heading -= ZWEIPI;
	var_pos = vp;
	var_heading = vh;
}

double RobotPositionKalmanFilter::get( Vec& p, Angle& h ) const throw () 
{
	p=pos;
	h.set_rad(heading);
	double sum_var = var_pos.x+var_pos.y+1e6*var_heading;   
	return 2e5/(2e5+sum_var);  
}

Vec RobotPositionKalmanFilter::get_position_variance () const throw () 
{
	return var_pos;
}

double RobotPositionKalmanFilter::get_heading_variance () const throw () 
{
	return var_heading;
}

void RobotPositionKalmanFilter::mirror () throw () 
{
	pos*=-1;
	heading+=M_PI;
	if (heading>M_PI)
		heading-=ZWEIPI;
}
