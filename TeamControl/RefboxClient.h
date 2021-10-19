#ifndef _RefboxClient_h
#define _RefboxClient_h
 
#include "GameState.h"
#include <fstream>

#define	  connectMessage		WM_USER+103

class RefboxClient
{
public:
    RefboxClient (const char* ="/dev/null", CWnd* pParent = NULL ) throw ();
    
    ~RefboxClient () throw ();

    bool connect (const char*, int) throw ();
    void disconnect () throw ();
    RefboxSignal listen () throw ();

    bool is_okay () const throw ();
    
    bool is_connected () const throw ();

private:
	RefboxSignal	latest_signal;    
    int				sockfd;                             
    int				okayfailed;                        
    bool			connected;                        
    std::ofstream	logstream; 
	CWnd*			pOwner;
};

#endif
