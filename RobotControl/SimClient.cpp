#include "StdAfx.h"
#include "SimClient.h"
#include "WorldModel.h"
#include <iostream>
#include <cmath>

SimClient* SimClient::the_only_sim_client = NULL;

SimClient* SimClient::get_sim_client (const char* conf) throw (std::bad_alloc, std::invalid_argument) 
{
	if (!the_only_sim_client)
		the_only_sim_client = new SimClient(conf);
	return the_only_sim_client;
}

void SimClient::delete_sim_client () throw () 
{
	if (the_only_sim_client)
		delete the_only_sim_client;
}

SimClient::SimClient (const char* conf) throw (std::bad_alloc, std::invalid_argument) 
: obstacle_positions (20) 
{
	odesimcomm=new OdeSimComm();
	odesimcomm->use_nonblock();
	 
	//if(vr.get("odeserver_address",odeserver_address,MAX_STRING_LEN)>-1)
	// simclient.use_as_client(odeserver_address, odeserver_port);
	// else 
	odesimcomm->use_as_client(conf,30001);
	//  odesimcomm->use_as_client("10.0.0.106",30001);
	odesimcomm->send(); // Send something so the server knows we are here and the port and inet adress
	
	obstacle_positions.resize (0);
 	
}

SimClient::~SimClient () throw () 
{;}

void SimClient::set_drive_vector (DriveVector dv) throw () 
{
	odesimcomm->simstate->steer[0]=dv.vtrans.x;
	odesimcomm->simstate->steer[1]=dv.vtrans.y;
	odesimcomm->simstate->steer[2]=dv.vrot;
	odesimcomm->simstate->kick=dv.kick;
}


void SimClient::update () throw () 
{
	odesimcomm->send();
	odesimcomm->receive();
 	
	//int x1, y1, phi1;
	Time now;
	SYSTEMTIME ts1;
	now.get(ts1);
 	
	// Roboter position
	
	robot_position.x = odesimcomm->simstate->pos[0]*1000;
	//cout <<"ROBOT POS X"<< robot_position.x<<endl;
	
	robot_position.y = odesimcomm->simstate->pos[1]*1000;
	// cout << robot_position.y<<endl;
	
	robot_heading.set_rad(odesimcomm->simstate->pos[2]+M_PI);
	//timestamp.set(ts1);
 	timestamp.update();
	// Ball position
 	ball_position = Vec(odesimcomm->simstate->ball[0]*1000,odesimcomm->simstate->ball[1]*1000);
 	
	unsigned int n = odesimcomm->simstate->num_obstacles;
	obstacle_positions.resize(n);
	unsigned int i=0;
	unsigned int p=0;
	while (i<n) 
	{
 		
		if (
			(fabs(odesimcomm->simstate->pos[0]-odesimcomm->simstate->obstacles[0][i])>0.2)
			||
			(fabs(odesimcomm->simstate->pos[1]-odesimcomm->simstate->obstacles[1][i])>0.2)
			)
			
		{
			Vec obstacle_position = Vec(odesimcomm->simstate->obstacles[0][i]*1000,odesimcomm->simstate->obstacles[1][i]*1000);
			obstacle_positions[p++] = obstacle_position;
		}
 		i++;
		
	}
	obstacle_positions.resize (p);
	
}

