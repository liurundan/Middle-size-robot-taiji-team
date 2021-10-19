#include "StdAfx.h"
#include "SimulatedRobot.h"
#include "RobotFactory.h"
#include "Journal.h"
#include "WorldModel.h"
#include <cmath>

using namespace std;


SimulatedRobot::SimulatedRobot ( ConfigReader& vr) throw (std::bad_alloc, InvalidConfigurationException) : the_sim_client (NULL), drv(15) 
{
	string confline;
	/*if (vr.get("Simulator::robot_config_file", confline)<=0) 
	{
    JERROR("no config line \"robot_config_file\" found");
    throw InvalidConfigurationException("robot_config_file");
}*/
	if (vr.get("Simulator::odeserver_address", confline)<=0) 
	{
		JERROR("no config line \"odeserver_address\" found");
		throw InvalidConfigurationException("Simulator::odeserver_address");
	}
	try
	{
		the_sim_client = SimClient::get_sim_client(confline.c_str());
	}
	catch(std::invalid_argument&)
	{
		JERROR("cannot connect to simulator");
		throw InvalidConfigurationException("robot_config_file");
	}
	
	robot_properties.max_velocity                  = 1.5;
	robot_properties.max_acceleration              = 0.5;
	robot_properties.max_deceleration              = 0.8;
	robot_properties.max_rotational_velocity       = 3.0;
	robot_properties.max_rotational_acceleration   = 0.5;
	robot_properties.max_rotational_deceleration   = 1.0;
	robot_properties.max_robot_radius              = 300;
	robot_properties.min_robot_radius              = 200;
	robot_properties.kicker_width                  = 290;
	robot_properties.kicker_distance               = 190;
	robot_properties.robot_width                   = 450;
	robot_properties.robot_length                  = 450;
	robot_properties.omnidirectional_drive         = true;
	robot_properties.drive_vector_delay              = 0;
	
	robot_properties.read (vr);
}

SimulatedRobot::~SimulatedRobot () throw () {;}

RobotProperties SimulatedRobot::get_robot_properties () const throw () 
{
	return robot_properties;
}

void SimulatedRobot::set_drive_vector (DriveVector dv) throw () 
{
	TDriveVector nd;
	nd.timestamp.add_msec (robot_properties.drive_vector_delay);
	nd.dv = dv;
	drv.get() = nd;
	drv.step();
	MWM.set_drive_vector( dv, nd.timestamp );
	
	Time now;
	int index=-1;
	unsigned int c=0;
	while (drv[index].timestamp>now && c++<drv.size())
		index--;
	if (drv[index].timestamp<=now)
		the_sim_client->set_drive_vector( drv[index].dv );
	
	RobotData rd;
	rd.BoardID = -1;
	sprintf(rd.robotIdString,"SIMULATION");
	
	Time tt;
	RobotLocation robot_location = (WorldModel::get_main_world_model().get_slfilter_robot_location (tt));
	
	robot_location.vtrans/=robot_location.heading;
	
	float sindelta=sin((M_PI/180.0) * 30);
	float cosdelta=cos((M_PI/180.0) * 30);
	float L1_m = 0.185f;
	float L2_m = 0.185f;
	float R_m  = 0.04f;
//	rd.wheel_vel[0] = 1.0/R_m * (-sindelta * robot_location.vtrans.x + cosdelta * robot_location.vtrans.y  + L1_m * robot_location.vrot);
//	rd.wheel_vel[1] = 1.0/R_m * (-sindelta * robot_location.vtrans.x - cosdelta * robot_location.vtrans.y  + L1_m * robot_location.vrot);
//	rd.wheel_vel[2] = 1.0/R_m * (            robot_location.vtrans.x                  + L2_m * robot_location.vrot);
	
   	float wl_tri = 0.2f;
	float RADIUS_tri = 0.035f;
	float root3_tri = 1.73205f;
 	rd.wheel_vel[0]= (root3_tri*robot_location.vtrans.x/2+robot_location.vtrans.y/2+wl_tri*robot_location.vrot)/RADIUS_tri;
	rd.wheel_vel[1]=(-root3_tri*robot_location.vtrans.x/2+robot_location.vtrans.y/2+wl_tri*robot_location.vrot)/RADIUS_tri;
	rd.wheel_vel[2]=(-robot_location.vtrans.y+wl_tri*robot_location.vrot)/RADIUS_tri;
	
	rd.robot_vel[0] = robot_location.vtrans.x;
	rd.robot_vel[1] = robot_location.vtrans.y;
	rd.robot_vel[2] = robot_location.vrot;
	
	Time      rd_time;
	WorldModel::get_main_world_model().set_robot_data(rd, rd_time);
}
