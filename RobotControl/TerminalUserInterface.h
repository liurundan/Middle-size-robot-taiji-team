#ifndef _terminal_user_interface_h
#define _terminal_user_interface_h

#include "UserInterfaceType.h"
#include "Player.h"
#include "Time.h"
#include "ConfigReader.h"
#include "Image.h"
#include "ImageProcessing.h"
#include <string>

using std::string;

class TerminalUserInterface : public UserInterfaceType
{
public:
    TerminalUserInterface ( ConfigReader&, Player&, ImageProcessing&, CDC& pDC) throw ();

    virtual ~TerminalUserInterface () throw ();

    virtual bool process_messages () throw ();
    void         Setmessage( char c );

protected:
    void init_window () throw ();
    void show_message(CDC* ppdc);
    void update_window () throw ();

    void save_image(const Image*) const throw(RobotsException);

    Player&				the_player;
    ImageProcessing&	the_image_processing;
    unsigned int		single_step_mode;  
    Time				manual_start_timer;         
    bool				wait_for_manual_start;      
    int					manual_start_sec;            
    bool				requestedImage;            
    string				debug_image_filename;

private:
    char                messagefromkey;	
	CDC&				pDC;

	CDC					m_memDc;
	CBitmap*			m_pMemDcBitmap;  // New bitmap each time size changes
	CRect				m_clientRect;
	int					width;  
	int					height;
};

#endif
