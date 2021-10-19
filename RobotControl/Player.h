#ifndef _player_h
#define _player_h

#include <stdexcept>
#include "PlayerType.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include <vector>

class Player
{
public:
    Player (ConfigReader&) throw (RobotsException, std::bad_alloc);
    ~Player () throw ();
    bool change_player_type (const char*) throw ();
    bool change_player_type (const char*,  ConfigReader&) throw ();

    const char* get_player_type () const throw ();

    void getPlayerTypeList(std::vector<std::string> &ptl);
    inline DriveVector process_drive_vector (Time t) throw () { return (the_player->process_drive_vector (t)); }

protected:
private:
	PlayerType* the_player;
    char* player_descriptor;
    ConfigReader& configuration_list;

    void really_change_player_type (const char*,  ConfigReader&) throw (RobotsException, std::bad_alloc);
};

#endif
