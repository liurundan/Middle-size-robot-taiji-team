#ifndef _RemoteTeamState_h
#define _RemoteTeamState_h

#include "RemoteRobotState.h"
#include <vector>
  
struct RemoteTeamState 
{
    std::vector<const RemoteRobotState*> robot_states;          
    std::vector<MessageBoard*> message_boards;   
    RefereeState refstate;                                
	
    RemoteTeamState (unsigned int n) : robot_states(n), message_boards(n) {;}
};
  
#endif
