#ifndef _add_com_user_interface_h
#define _add_com_user_interface_h

#include "RobotsException.h"
#include "UserInterface.h"
#include "UserInterfaceType.h"
#include "RobotsUdpServer.h"

class AddComUserInterface : public UserInterfaceType
{
public:
    AddComUserInterface ( ConfigReader&, Player&, ImageProcessing&, CDC& dc) 
		throw (RobotsException, std::bad_alloc);
    virtual ~AddComUserInterface () throw ();
    virtual bool process_messages () throw ();
	virtual void Setmessage(char c); 

protected:
	UserInterfaceType * the_local_user_interface;
    RobotsUdpServer * the_communication;

    Player &pl;
    ImageProcessing &ip;

    int commFailCount;
    int commMaxFailCount;

};

#endif