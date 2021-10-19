#ifndef _Coach_h
#define _Coach_h

#include "RemoteTeamState.h"
#include "Policy.h"
#include "ConfigReader.h"
#include "Table.h"
#include "Time.h"
#include "RobotsException.h"
#include <vector>
#include <deque>

class Coach
{
public:
/** constructor; 
Arg1: Config file for tactics; 
Arg2: List of the robot IDS */
    Coach ( ConfigReader&, const std::vector<std::string>&) throw (RobotsException);
    ~Coach () throw ();
	
    void update (RemoteTeamState&) throw ();
	
    void set_comm_ball_mode (bool) throw ();
    bool get_comm_ball_mode () const throw ();
	
    void set_owns_ball_mode (bool) throw ();
    bool get_owns_ball_mode () const throw ();
	
    bool dynamic_role_change_mode (const char*) throw ();
    const std::string& dynamic_role_change_mode () const throw ();
    void change_player_role (const char*, const char*) throw ();
    std::vector<std::string> get_list_of_roles () const throw ();
    std::vector<std::string> get_list_of_policies () const throw ();
    std::string get_players_role (const std::string&) const throw ();
    const std::string& get_role_description () const throw ();
	
    bool kick_off_own_procedure (const std::string&) throw ();
    bool kick_off_opponent_procedure (const std::string&) throw ();
    const std::string& kick_off_own_procedure () const throw ();
    const std::string& kick_off_opponent_procedure () const throw ();
    std::vector<std::string> get_list_of_kick_off_procedures () const throw ();
    void set_broadcast_mode (bool) throw();
    bool get_broadcast_mode () const throw ();
	
private:
	struct HPos
	{
		std::string role;
		Vec pos;
		Angle heading;
	};
	struct KickOffPolicy 
	{
		std::string name;
		std::vector<std::deque<HPos> > pos;
		KickOffPolicy () throw (std::bad_alloc);
		KickOffPolicy(const KickOffPolicy&) throw (std::bad_alloc);
	};
	
	std::vector<Policy*> policies;  
	std::vector<std::string> robot_ids;   
	std::vector<std::string> desired_roles;  
	std::vector<std::string> actual_roles; 
	std::vector<bool> role_known;  
	std::vector<bool> is_active; 
	std::vector<Time> activation_time; 
	std::vector<KickOffPolicy> ko_policies;  
	
	int policies_index (const std::string&) throw();  
	int ko_policies_index (const std::string&) throw (); 
	
	bool comm_ball_mode;
	bool broadcast_mode;
	bool owns_ball_mode;
	std::string rc_player_dynamic_role_change_mode;
	std::string kick_off_own_policy;
	std::string kick_off_opponent_policy;
	
	std::vector<std::string> ignore_broadcast_prefix;  
	std::string role_description;
	   
};

#endif