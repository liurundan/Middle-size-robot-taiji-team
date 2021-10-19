#ifndef _AddJoystickPlayer_h_
#define _AddJoystickPlayer_h_

#include "JoystickPlayer.h"

class AddJoystickPlayer : public JoystickPlayer
{
public:
    AddJoystickPlayer (  ConfigReader&, PlayerType*) throw (InvalidConfigurationException, std::bad_alloc);
    ~AddJoystickPlayer () throw ();
    DriveVector process_drive_vector (Time) throw ();
protected:

private:
	PlayerType* the_elementary_player;  
    DriveVector remoteCtrDrv;
    unsigned int start_button, stop_button;  
	
};

#endif
