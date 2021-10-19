#include "StdAfx.h"
#include "Policy.h"

using namespace std;

namespace
{
	const unsigned int max_player = 4;  
}

StaticPolicy::StaticPolicy ( ConfigReader& cfg, const char* nm) 
throw (std::bad_alloc, RobotsException) : Policy(nm), roles(max_player, max_player)
{
	for (unsigned int i1=0; i1<max_player; i1++)
		for (unsigned int j=0; j<max_player; j++)
			roles(i1,j) = "";

	vector<string> change_lines;
	int index = 0;
	string s(nm);
	string sTmp = "Policy::"+s;
	
	for (unsigned int k=0; k<cfg.m_nRows; k++ )
	{
		string stable = &(cfg.m_Table[k].s[0][0]);
        if ( sTmp == stable )
		{
			index = k;
			break;
        }
	}
	for (unsigned int i=0; i<max_player; i++) 
	{
		index++;
		for (unsigned int k1 = 0; k1 < cfg.m_Table[index].nID; k1++ )
		{
			string srole =  &(cfg.m_Table[index].s[k1][0]);
			change_lines.push_back(srole);
		}
		for (unsigned int j=0; j<=i; j++)
			if( change_lines.size()>=j+1 ) 
				roles(i,j) = change_lines[j];
			else 
				cerr << "ACHTUNG: Rollenwechselstrategie " << name 
				<< " undefiniert fuer den " << j+1 << "-ten unter " << i+1 << " Spielern\n";
		change_lines.clear();
	}
}

StaticPolicy::StaticPolicy (const StaticPolicy& src) throw (std::bad_alloc) : Policy(src), roles (src.roles) {;}

Policy* StaticPolicy::clone () const throw (std::bad_alloc) 
{
	return new StaticPolicy (*this);
}

const Table<std::string> StaticPolicy::get_roles () const throw () 
{
	return roles;
}

DynamicDefendOffendPolicy::DynamicDefendOffendPolicy (  ConfigReader& cfg, const char* nm) 
throw (std::bad_alloc, RobotsException) : Policy (nm), defend_policy (NULL), offend_policy (NULL) 
{
	string policypolicy;
	cfg.get ((nm+string("::defensive_policy")).c_str(), policypolicy);
	defend_policy = new StaticPolicy (cfg, policypolicy.c_str());
	cfg.get ((nm+string("::offensive_policy")).c_str(), policypolicy);
	offend_policy = new StaticPolicy (cfg, policypolicy.c_str());
	hyst_x1 = -200;
	hyst_x2 = 200;
	was_defend=true;
}

DynamicDefendOffendPolicy::DynamicDefendOffendPolicy (const DynamicDefendOffendPolicy& src) 
throw (std::bad_alloc) : Policy(src), hyst_x1(src.hyst_x1), hyst_x2(src.hyst_x2), 
defend_policy(src.defend_policy->clone()), offend_policy(src.offend_policy->clone()), was_defend(src.was_defend) 
{;}

DynamicDefendOffendPolicy::~DynamicDefendOffendPolicy () throw () 
{
	if (offend_policy) delete offend_policy;
	if (defend_policy) delete defend_policy;
}

Policy* DynamicDefendOffendPolicy::clone () const throw (std::bad_alloc) 
{
	return new DynamicDefendOffendPolicy (*this);
}

void DynamicDefendOffendPolicy::update (RemoteTeamState& ts) throw () 
{
	bool ball_found=false;
	unsigned int nearest_robot=99999;
	double smallest_dist=1e99;
	unsigned int num_robots=ts.robot_states.size();
	for (unsigned int sender=0; sender<num_robots; sender++)
	{
		if (ts.robot_states[sender]->in_game)
		{
			if (ts.robot_states[sender]->ball_pos.pos_known == BallLocation::known) 
			{
				double len = (ts.robot_states[sender]->ball_pos.pos-ts.robot_states[sender]->robot_pos.pos).length();
				if( len < smallest_dist ) 
				{
					smallest_dist = len;
					nearest_robot = sender;
					ball_found = true;
				}
			}
		}
	}
	if (ball_found) 
	{
		Vec ball = ts.robot_states[nearest_robot]->team*ts.robot_states[nearest_robot]->tribots_team.own_half*ts.robot_states[nearest_robot]->ball_pos.pos;
		if (ball.x<hyst_x1 && !was_defend) 
		{
			was_defend=true;
			cerr << "schalte auf defensive Strategie um\n";
		} 
		else if (ball.x>hyst_x2 && was_defend) 
		{
			was_defend=false;
			cerr << "schalte auf offensive Strategie um\n";
		}
	}
}

const Table<std::string> DynamicDefendOffendPolicy::get_roles () const throw () 
{
	if (was_defend)
		return defend_policy->get_roles();
	else
		return offend_policy->get_roles();
}