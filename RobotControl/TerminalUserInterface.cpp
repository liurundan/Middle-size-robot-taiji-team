#include "StdAfx.h"
#include <sstream>
#include <cstring>
#include <cmath>
#include "TerminalUserInterface.h"
#include "WorldModel.h"
#include "GameState.h"
#include "WhiteBoard.h"
#include "Journal.h"
#include "ImageIO.h"
#include "Tools.h"

using namespace std;

namespace 
{
  string format_double (double d, unsigned int flen, unsigned int prec)
  {
    std::stringstream inout;
    inout.precision(prec);
    inout.setf(ios_base::fixed,ios_base::floatfield);
    inout << d;
    string res;
    std::getline (inout, res);
    int n=flen-res.length();
    while (n>0) 
	{
      res=string(" ")+res;
      n--;
    }
    return res;
  }
} 

TerminalUserInterface::TerminalUserInterface ( ConfigReader& vr, Player& pl, ImageProcessing& ip, CDC& dc ) throw () : 
			the_player(pl), the_image_processing(ip), single_step_mode(0), wait_for_manual_start(false), 
			manual_start_sec (10), requestedImage(false),pDC(dc)
{
	init_window ();

	int i;
	debug_image_filename = "debug_image.ppm";
	if ((vr.get ("manual_start_waiting_time", i)>0) && (i>0))
	manual_start_sec=i;
	if (vr.get ("debug_image_filename", debug_image_filename) > 0) 
	{
	 // throw InvalidConfigurationException("debug_image_filename");
	}
	messagefromkey = '#';
    
}

TerminalUserInterface::~TerminalUserInterface () throw () 
{
    delete m_pMemDcBitmap;
}

void TerminalUserInterface::init_window () throw () 
{
    width  = 210;
	height = 290;
	m_memDc.CreateCompatibleDC(&pDC);
	m_pMemDcBitmap = new CBitmap();
	m_pMemDcBitmap->CreateCompatibleBitmap(&pDC, width, height);
	m_clientRect = CRect(0,0,width,height);
	
	m_memDc.SelectStockObject(NULL_BRUSH);
	CBitmap*			pOldObject = NULL;
	pOldObject = m_memDc.SelectObject(m_pMemDcBitmap);
   	m_memDc.SetViewportOrg(0, 0);

}  

bool TerminalUserInterface::process_messages () throw () 
{
  if (single_step_mode>0) 
  {
    single_step_mode--;
    if (single_step_mode==0)
      MWM.startstop (false);
  }
  if (wait_for_manual_start && (manual_start_timer.elapsed_msec()>1000*manual_start_sec)) 
  {
    wait_for_manual_start=false;
    MWM.startstop (true);
  }
  if (requestedImage) 
  {
    try 
	{
      if (the_image_processing.is_image_available()) 
	  {
		const Image* img = the_image_processing.get_image();
		save_image(img);
		the_image_processing.free_image();
		requestedImage = false;
      }
    } 
	catch (RobotsException& e) 
	{
      JERROR(e.what());
    }
  }
  int ihalf = 0;
  char c = messagefromkey;
  switch(c) 
  {
	  case ' ' :  
		MWM.startstop (false);
		single_step_mode=0;
		break;
	  case 'A' : 
		MWM.startstop (true);
		single_step_mode=0;
		break;
	  case 'G' :  
		MWM.startstop (true);
		single_step_mode=0;
		MWM.update_refbox (SIGstop);
		MWM.update_refbox (SIGstart);
		break;
	  case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':  
		MWM.startstop (true);
		single_step_mode=static_cast<unsigned int>(c-'0');
		break;    
	  case 'I':
		if (!requestedImage) 
		{
		  try 
		  {
			the_image_processing.request_image_raw();
			requestedImage = true;
		  } 
		  catch (RobotsException& e) 
		  {
			JERROR(e.what());
			requestedImage = false;
		  }
		}
		break;
	  case 'K':
		if (!requestedImage) 
		{
		  try 
		  {
			requestedImage = true;
			the_image_processing.request_image_processed();
		  } 
		  catch (RobotsException& e) 
		  {
			JERROR(e.what());
			requestedImage = false;
		  }
		}
		break;
	  case 'T' :
		wait_for_manual_start=true;
		manual_start_timer.update();
		break;
	  case 'S' :  
		MWM.set_own_half(-1*MWM.get_own_half());
		break;
	  case 'B' : // refresh
		// init_window();
		break;
	  case 'P' :  
		WorldModel::get_main_world_model().reset ();
		break;
	  case 'C' :  
	   	  the_player.change_player_type ("Goalie");
		break;
	  case 'F' : 
	//	  the_player.change_player_type ("FunPlayer");
		break;
	  case 'J' :  
		the_player.change_player_type ("JoystickPlayer");
		break;
	  case 'R' :  
		if (string(the_player.get_player_type())!=string("RCPlayer"))
		  the_player.change_player_type ("RCPlayer");
		else 
		{
		  unsigned int next_role = WhiteBoard::getTheWhiteBoard()->getPlayerRole();
		  next_role = (next_role+1)%num_roles;
		  WhiteBoard::getTheWhiteBoard()->changePlayerRole(PlayerRole(next_role));
		}
		break;
	  default:
		  break;
  }
  update_window();

  if(messagefromkey != '#') // clear the key value from keyboard,reset the signal
	  messagefromkey = '#';

  return true;

}
 
void TerminalUserInterface::update_window( ) throw () 
{
 	show_message(&m_memDc);
    int bresult;
	bresult = pDC.BitBlt(130, 30, width, height, &m_memDc, 0, 0, SRCCOPY);
	// ASSERT(bresult);
 	// m_memDc.SelectObject(pOldObject); 
}

void TerminalUserInterface::show_message(CDC* ppdc)
{
  ppdc->FillSolidRect(m_clientRect,RGB(135,228,118));
  Time current_time;
  CString str;
  str.Format("%4d",current_time.get_msec());
  ppdc->TextOut(1,0,str);
  
  str = referee_state_names[MWM.get_game_state().refstate];
  if (wait_for_manual_start)
    str += " manual Start";
  else if (MWM.get_game_state().in_game)
    str += " activate";
  else
    str += " deactivate";
  ppdc->TextOut(1,38,str);
 
  str = the_player.get_player_type();
  if(strcmp (the_player.get_player_type(), "RCPlayer")==0) 
  {
	str += " / ";
	str += playerRoleNames[WhiteBoard::getTheWhiteBoard()->getPlayerRole()];
	str += "      ";
  } 
  else 
  {
    str += "          "; 
  }
  ppdc->TextOut(1,76,str);

  WorldModel& the_world_model = WorldModel::get_main_world_model ();
  int ihalf2 = the_world_model.get_own_half();

  if( the_world_model.get_own_half() > 0 )
    str = "yellow";
  else
    str = "blue";
  ppdc->TextOut(1,114,str);

  RobotLocation rl = MWM.get_robot_location(current_time);
  str.Format("(%s,%s,%s,%s)",format_double(rl.pos.x,6,0).c_str(),
							  format_double(rl.pos.y,6,0).c_str(),
							  format_double(rl.heading.get_deg(),3,0).c_str(),
						     ( rl.kick ? "kick" : " -- " ) );
  ppdc->TextOut(1,152,str);
  

  str.Format("(%s,%s,%s)",format_double(rl.vtrans.x,5,2).c_str(),
	                      format_double(rl.vtrans.y,5,2).c_str(),
						  format_double(rl.vrot,5,2).c_str() );
  ppdc->TextOut(1,190,str);
  
  BallLocation bl = MWM.get_ball_location (current_time);
  Vec relpos = (bl.pos-rl.pos)/rl.heading; 
  str.Format("(%s,%s)%.f,%d", format_double(((bl.pos-rl.pos)/rl.heading).x,6,0).c_str(),
						format_double(((bl.pos-rl.pos)/rl.heading).y,6,0).c_str(),(bl.pos-rl.pos).length(),rl.stuck.robot_stuck );
  //WBOARD->get_small_camera_ball()
  if( WhiteBoard::getTheWhiteBoard()->doPossesBall(current_time) )
  {
    str += " (own)";
  }
  else
	str += "      ";
 
  if (MWM.get_usb_work())
  {
	  str += "usb";
	  MWM.set_usb_work(false);
  }

  double pe = MWM.ball2Robot.angle;
  BYTE ball_dir = WBOARD->get_small_camera_ball();
  
  ppdc->TextOut(1,228,str);

  str.Format("(%s,%s)%.1f",format_double(bl.velocity.x,6,2).c_str(),
	                   format_double(bl.velocity.y,6,2).c_str(),pe );
  ppdc->TextOut(1,268,str);
   
}

void TerminalUserInterface::save_image(const Image* img) const throw(RobotsException)
{
  try 
  {
    ImageIO* io = new PPMIO();
    io->write(*img, debug_image_filename);
    delete io;
  } 
  catch (RobotsException& e) 
  {
    JERROR (e.what());
  }
}

void TerminalUserInterface::Setmessage( char c )
{
	messagefromkey = c;
}