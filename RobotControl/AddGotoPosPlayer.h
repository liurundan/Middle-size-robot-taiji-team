#ifndef _AddGotoPosPlayer_h_
#define _AddGotoPosPlayer_h_

#include "PlayerType.h"
#include "ConfigReader.h"
#include "SPhysGotoPos.h"
#include "GameState.h"

class AddGotoPosPlayer : public PlayerType
{
public:
    AddGotoPosPlayer ( ConfigReader&, PlayerType*) throw (InvalidConfigurationException, std::bad_alloc);
    ~AddGotoPosPlayer () throw ();
    DriveVector process_drive_vector (Time) throw ();

private:
	PlayerType* the_elementary_player;            
    bool is_active;                               
    RefereeState latest_refstate;                  
    SPhysGotoPos goto_pos_skill;                  

};

#endif
