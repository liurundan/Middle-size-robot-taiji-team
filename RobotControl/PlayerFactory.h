#ifndef _PlayerFactory_h_
#define _PlayerFactory_h_

#include "PlayerType.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include <string>
#include <vector>

class PlayerFactory
{
public:
    static PlayerFactory* get_player_factory () throw (std::bad_alloc);
    PlayerType* get_player (const std::string, ConfigReader&) throw (RobotsException,std::bad_alloc,std::invalid_argument);
    void player_list (std::vector<std::string>&) const throw (std::bad_alloc);

private:
    static PlayerFactory* the_only_factory;
	static void destroy();

    PlayerFactory () throw ();                             
    ~PlayerFactory() throw ();                            

};

#endif
