#include "StdAfx.h"
#include "RobotsUdpServer.h"
#include "Time.h"
#include <sstream>
#include "Journal.h"
#include "update_robot_location.h"

RobotsUdpServer* RobotsUdpServer::tribotsUdpCom = NULL;

RobotsUdpServer *RobotsUdpServer::get_instance_pointer(int port)
{
	if (tribotsUdpCom) 
	{
		return tribotsUdpCom;
	} 
	else 
	{
		return tribotsUdpCom = new RobotsUdpServer(port);
	}
}

RobotsUdpServer::RobotsUdpServer(int port) : RobotsUdpCommunication(true, port)
{
	;
}

RobotsUdpServer::~RobotsUdpServer()
{
	;
}


void RobotsUdpServer::delete_instance()
{
	if (! tribotsUdpCom ) 
	{
		return ;
	} 
    else 
	{
		delete tribotsUdpCom;
		tribotsUdpCom = NULL;
	}
}


bool RobotsUdpServer::hasInstance()
{
	return (tribotsUdpCom!= NULL);
}


bool RobotsUdpServer::processRequests(WorldModel& _wm, Player& _pl)
{
	clear_to_send();
	Time current_time;
	
	bool something_to_send=false;
	if (requestSettings->request[GameStateIdx]==send_once)
    {
		GameState gs = _wm.get_game_state();
		putGameState(gs);
		something_to_send=true;
    }
	if (requestSettings->request[OwnHalfIdx]==send_once)
    {
		int oh = _wm.get_own_half();
		putOwnHalf(oh);
		something_to_send=true;
    }
	if (requestSettings->request[FieldGeometryIdx]==send_once)
    {
		FieldGeometry fg = _wm.get_field_geometry();
		putFieldGeometry(fg);
		something_to_send=true;
    }
	if (requestSettings->request[BallLocationIdx]==send_once)
    {
		BallLocation bl = flip_sides (_wm.get_ball_location(current_time), _wm.get_own_half());
		putBallLocation(bl);
		something_to_send=true;
    }
	if (requestSettings->request[RobotPropertiesIdx]==send_once)
    {
		RobotProperties rp = _wm.get_robot_properties();
		putRobotProperties(rp);
		something_to_send=true;
    }
	if (requestSettings->request[RobotLocationIdx]==send_once)
    {
		RobotLocation rl = flip_sides (_wm.get_robot_location(current_time), _wm.get_own_half());
		putRobotLocation(rl);
		something_to_send=true;
    }
	if (requestSettings->request[PlayerTypeIdx]==send_once)
    {
		putPlayerType(_pl.get_player_type());
		something_to_send=true;
    }
	if (requestSettings->request[RobotDataIdx]==send_once)
    {
		Time d;
		RobotData rd = _wm.get_robot_data(d);
		putRobotData(rd);
		something_to_send=true;
    }
	
	something_to_send = something_to_send || processXMLRequests(_wm,_pl);
	
	if (something_to_send) 
	{ 
		MessageBoard& mb = _wm.get_message_board();
		std::vector<std::string> msgList = mb.get_outgoing();
		if (msgList.size()>0) 
		{
			putMessageList(msgList);
		}
	}
	
	if (something_to_send)
		send();
	
	requestSettings->clear();
	
	return true;
}


bool RobotsUdpServer::processXMLRequests(WorldModel &_wm, Player & _pl)
{
	Time current_time;
	bool res = false;
	
	//JERROR("processXMLReq");
	for (unsigned i=0; i<ReceivedXMLRequests.size(); i++)
    {
		if (ReceivedXMLRequests[i]==string("PlayerTypeList")) 
		{
			vector<string> ptl;
			_pl.getPlayerTypeList(ptl);
			putPlayerTypeList(ptl);
			res = true;
		}
		else if (ReceivedXMLRequests[i]==string("ObstacleLocation")) 
		{
			const ObstacleLocation& ol = flip_sides (_wm.get_obstacle_location(current_time, false), _wm.get_own_half());
			putObstacleLocation(ol);
			res = true;
		}
		else if (ReceivedXMLRequests[i]==string("VisibleObjectList")) 
		{
			const VisibleObjectList& ol = _wm.get_visible_objects();
			putVisibleObjectList(ol);
			res = true;
		}
		else {
			ostringstream e;
			e << "Received a not known XML request! [" << ReceivedXMLRequests[i] << "]!";
			JERROR(e.str().c_str());
		}
    }
	ReceivedXMLRequests.clear();
	return res;
}
