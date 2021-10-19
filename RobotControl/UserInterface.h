#ifndef _user_interface_h
#define _user_interface_h


#include "RobotsException.h"
#include "Player.h"
#include "ConfigReader.h"
#include "ImageProcessing.h"

#include "UserInterfaceType.h"

class UserInterface
{
public:
	UserInterface ( ConfigReader&, Player&, ImageProcessing&, CDC& dc ) throw (RobotsException, std::bad_alloc);
    ~UserInterface() throw ();

    inline bool process_messages () throw () {return the_user_interface->process_messages();}
	inline void Setmessage( char c ) { the_user_interface->Setmessage( c );	}
protected:
	UserInterfaceType  * the_user_interface;

};

#endif