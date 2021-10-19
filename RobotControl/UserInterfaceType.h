#ifndef _user_interface_type_h
#define _user_interface_type_h

class UserInterfaceType
{
public: 

	virtual ~UserInterfaceType() throw () {;}
    virtual bool process_messages () throw () =0;
	virtual void Setmessage( char c) = 0;
};

#endif