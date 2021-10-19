#include "StdAfx.h"
#include "AddGotoPosPlayer.h"
#include "WorldModel.h"
#include "stringconvert.h"

using namespace std;

AddGotoPosPlayer::AddGotoPosPlayer (ConfigReader& cfg, PlayerType* pt) 
throw (InvalidConfigurationException, std::bad_alloc) : the_elementary_player (pt), is_active(false) 
{
	if (pt==NULL)
		throw InvalidConfigurationException("AddGotoPosPlayer ohne richtigen Player");
	goto_pos_skill.set_dynamics (0.5, 2, 2, 2);
}

AddGotoPosPlayer::~AddGotoPosPlayer () throw () 
{
	delete the_elementary_player;
}

DriveVector AddGotoPosPlayer::process_drive_vector (Time t) throw () 
{
	DriveVector dest = the_elementary_player->process_drive_vector (t);
	
	string prline = MWM.get_message_board().scan_for_prefix ("GotoPos:");
	deque<string> parts;
	split_string (parts, prline);
	if (parts.size()>=4) 
	{
		Vec tp;
		double thd;
		string2double (tp.x, parts[1]);
		string2double (tp.y, parts[2]);
		string2double (thd, parts[3]);
		int oh = MWM.get_own_half();
		goto_pos_skill.init (oh*tp, Angle::deg_angle (thd) + (oh>0 ? Angle::zero : Angle::half), true);
		is_active = true;
		latest_refstate = MWM.get_game_state().refstate;
	}
	
	if (is_active) 
	{
		RefereeState crefstate = MWM.get_game_state().refstate;
		if (latest_refstate!=crefstate) 
		{
			if (crefstate==postOpponentKickOff || crefstate==postOpponentGoalKick ||
				crefstate==postOpponentCornerKick || crefstate==postOpponentThrowIn || 
				crefstate==postOpponentFreeKick || crefstate==postOpponentPenalty )
				latest_refstate = crefstate;
			else
				is_active=false;
		}
	}
	
	if (is_active) 
	{
		goto_pos_skill.getCmd (&dest, t);
		//dest.vtrans *= 1e2;
		dest.kick=false;

	}
	return dest;
}

