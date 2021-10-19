#include "StdAfx.h"
#include "WhiteBoard.h"
#include "WorldModel.h"
#include "stringconvert.h"
#include <cmath>
#include <fstream>

using namespace std;

#define LEFT 0
#define MIDDLE 1
#define RIGHT 2

#define DISPLAY_AREAS 1
#define DISPLAY_DRIBBLE_CORRIDOR 0
#define DISPLAY_FREEGOALPOS_CORRIDORS 0
#define DISPLAY_SHOOTCORRIDOR 1

WhiteBoard* WhiteBoard::instance = 0;
  
WhiteBoard* WhiteBoard::getTheWhiteBoard()
{
    if (instance == 0) 
	{
      instance = new WhiteBoard();
      ::atexit(destroy);
    }
    return instance;
}
 
  ////////////////////////// init and deinit code /////////////////////////////
  
WhiteBoard::WhiteBoard()
    :m_bDoNotGetBallPossession(false),ballInOppGoalData(0),possesBallData(0),
	abs2relData(0),rel2absData(0),playerRole(PlayerRole(4)),
	cycles_without_team_posses_ball(10),cycles_without_advanced_team_posses_ball(10),cycles_ball_free(10)
{
    frames_ball_owned=0;
	small_camera_ball_dir = 0;
	m_bShall_I_get_ball = true;
	teammate.resize(4);
}

WhiteBoard::~WhiteBoard()
{
    if (instance == this) 
	{       
		instance = 0;
    }
    
    // <<<<<<<<< put your clean up code here

    if (ballInOppGoalData) delete ballInOppGoalData;
    if (possesBallData) delete possesBallData;
    if (abs2relData) delete abs2relData;
    if (rel2absData) delete rel2absData;
}

void WhiteBoard::destroy()
{
   if( instance != NULL)
   {
	   delete instance;
	   instance = NULL;
   }
}

  //////////////////////////// isBallInOppGoal ////////////////////////////////

bool WhiteBoard::isBallInOppGoal(const Time& t)
{
    BoolData* data;
    if (ballInOppGoalData) 
	{
      data = dynamic_cast<BoolData*>(ballInOppGoalData);
    
      if (data->t == t &&
	  data->cycle == 
	  WorldModel::get_main_world_model().get_game_state().cycle_num)
	  {
		return data->b;           
      }
    }
    else 
	{                        
      data = new BoolData();      
      ballInOppGoalData = data;
    }

    data->t = t;
    data->cycle = 
      WorldModel::get_main_world_model().get_game_state().cycle_num;

    BallLocation ballLocation =
      WorldModel::get_main_world_model().get_ball_location(t);
    FieldGeometry field = 
      WorldModel::get_main_world_model().get_field_geometry(); 

    if (ballLocation.pos.x < field.field_length / 2. ||
		ballLocation.pos.y < -field.goal_width ||
		ballLocation.pos.y > field.goal_width ||
		ballLocation.pos.x > field.field_length / 2. + field.goal_length * 2)
	{
      return data->b =  false;
    }
    else 
	{
      return data->b = true;
    }
}

  ////////////////////////////// doPossesBall /////////////////////////////////

bool WhiteBoard::doPossesBall(const Time& t)
{
    BoolData* data;
    if (possesBallData) 
	{
      data = dynamic_cast<BoolData*>(possesBallData);
    
      if (data->t == t && data->cycle == 
	  WorldModel::get_main_world_model().get_game_state().cycle_num )
	  {
		return data->b;           
      }
    }
    else 
	{                       
      data = new BoolData();      
      possesBallData = data;
      
    }

    data->t = t;
    data->cycle = 
      WorldModel::get_main_world_model().get_game_state().cycle_num;

    BallLocation ballLocation =
      WorldModel::get_main_world_model().get_ball_location(t);

    Vec relBall = getAbs2RelFrame(t) * ballLocation.pos;

    bool ballclose=true;
    if (
    (relBall.x> 380|| relBall.x<0)  // edit by lrd 350
        ||
     (fabs(relBall.y) > 60)     ) // 120
    {  
      ballclose=false;
	} 
      
    if (ballclose)
	{
		frames_ball_owned++;

	}
    else 
       frames_ball_owned--;

	if (frames_ball_owned<0)
	{
		frames_ball_owned = 0;
	}
	if (frames_ball_owned>10)
	{
		frames_ball_owned = 10;
	}


	if (frames_ball_owned <5)
	{
		data->b = false; 
		return false;
	}

	if(!m_bDoNotGetBallPossession)
	{
		data->b = true; 
		return true;
	}
	return true;
}

  ///////////////////////////// getRel2AbsFrame ///////////////////////////////

const Frame2d&  WhiteBoard::getRel2AbsFrame(const Time& t)
{
    Frame2dData* data;
    if (rel2absData) 
	{
      data = dynamic_cast<Frame2dData*>(rel2absData);
    
      if (data->t == t && data->cycle == 
	  WorldModel::get_main_world_model().get_game_state().cycle_num) 
	  {
	       return data->frame;      
      }
    }
    else 
	{                       
      data = new Frame2dData();  
      rel2absData = data;
    }
    
    RobotLocation robotLocation = 
      WorldModel::get_main_world_model().get_robot_location(t);
    
    data->frame.set_position(robotLocation.pos);
    data->frame.set_angle(robotLocation.heading);

    return data->frame;
}

  ///////////////////////////// getAbs2RelFrame ///////////////////////////////

const Frame2d& WhiteBoard::getAbs2RelFrame(const Time& t)
{
    Frame2dData* data;
    if (abs2relData) 
	{
      data = dynamic_cast<Frame2dData*>(abs2relData);
    
      if (data->t == t && data->cycle == 
	  WorldModel::get_main_world_model().get_game_state().cycle_num) 
	  {
		return data->frame;       
      }
    }
    else 
	{                       
      data = new Frame2dData();   
      abs2relData = data;
    }

    data->frame = getRel2AbsFrame(t);    
    data->frame.invert();                 

    return data->frame;
}
  
const PlayerRole WhiteBoard::getPlayerRole()
{
	return playerRole;
}

void WhiteBoard::changePlayerRole(PlayerRole newrole)
{
	playerRole = newrole;
	MOUT << "NewRole: " << playerRoleNames[newrole] << '\n';
}

void WhiteBoard::kickOffPosition(Vec& p, Angle& h) 
{
	p = kick_off_pos;
	h = kick_off_heading;
}

void WhiteBoard::checkMessageBoard()
{
    string prline = MWM.get_message_board().scan_for_prefix ("ChangeRole:");
    deque<string> parts;
    split_string (parts, prline);
    if (parts.size()>1) 
	{
      for (unsigned int i=0; i<num_roles; i++)
	  {
		if (parts[1] == playerRoleNames[i]) 
		{
		  changePlayerRole (PlayerRole(i));
		}
      }
    }

    teammate.clear();
  	for (unsigned int i=0; i<num_roles; i++)
	{
		string tp_role_name = playerRoleNames[i];
		string tp_pre = tp_role_name + ": ";
		const string tp = MWM.get_message_board().scan_for_prefix(tp_pre);
		Vec tp_pos;
		if (tp.length()>0) 
		{
			deque<string> parts;
			split_string(parts, tp);
			if(parts.size()>=3) 
			{
				string2double (tp_pos.x, parts[1]);
				string2double (tp_pos.y, parts[2]);
				pair<string,Vec> ta;
				ta.first  = tp_role_name;
				ta.second = tp_pos;
				teammate.push_back(ta);
			}
		}
	}
 
    if (MWM.get_message_board().scan_for_prefix ("WhichRole?").length()>0)
      MOUT << "NewRole: " << playerRoleNames[playerRole] << '\n';

     
    if( MWM.get_message_board().scan_for_prefix ("OwnsBall!").length()>0 )
      cycles_without_team_posses_ball = 0;
    else
      cycles_without_team_posses_ball++;

    if( MWM.get_message_board().scan_for_prefix ("GetBall!").length()>0 )
      cycles_ball_free++;
    else
      cycles_ball_free = 0;
      
     
    if( MWM.get_message_board().scan_for_prefix ("NearBall!").length()>0 )
      cycles_without_advanced_team_posses_ball = 0;
    else
      cycles_without_advanced_team_posses_ball++;
  
     
    prline = MWM.get_message_board().scan_for_prefix ("KickOffPos:");
    if (prline.length()>0)
	{
      deque<string> parts;
      split_string (parts, prline);
      if (parts.size()>=4) 
	  {
		string2double (kick_off_pos.x, parts[1]);
		string2double (kick_off_pos.y, parts[2]);
		double d;
		string2double (d, parts[3]);
		kick_off_heading.set_deg(d);
      }
    }
}

bool WhiteBoard::teamPossesBall() 
{
	return (cycles_without_team_posses_ball<6);
}

bool WhiteBoard::advancedTeamPossesBall() 
{
	return (cycles_without_advanced_team_posses_ball<6);
}

bool WhiteBoard::shall_I_get_ball()
{
	return (cycles_ball_free>6);
}

vector<pair<string,Vec> > WhiteBoard::getTeammate()
{
	return teammate;
}

void WhiteBoard::set_small_camera_ball(BYTE p)
{
	small_camera_ball_dir = p;
}

BYTE WhiteBoard::get_small_camera_ball()
{
	return small_camera_ball_dir;
}