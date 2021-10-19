#include "StdAfx.h"
#include "WorldModelTypeBase.h"
#include "GameState.h"
#include "LocationShortTimeMemory.h"
  
using namespace std;

WorldModelTypeBase::WorldModelTypeBase (  ConfigReader& vread) throw (InvalidConfigurationException, std::bad_alloc)
	 : field_geometry (vread), gsman (vread, field_geometry), own_half (1), null_stream ("null.out")  
	 
{
  locations = ( new LocationShortTimeMemory (*this) );
  string confline;
  if (vread.get ("own_half", confline)>0) 
  {
    if( confline == "yellow" )
      own_half = 1;
    else if ( confline=="blue" )
      own_half = -1;
  }
  null_stream.close();   

  no_good_rloc.pos  = no_good_rloc.vtrans=Vec::zero_vector;
  no_good_rloc.vrot = 0;
  no_good_rloc.heading = Angle::zero;
  no_good_rloc.kick = false;

  bUsbWork = false;
}

WorldModelTypeBase::~WorldModelTypeBase () throw () 
{
  delete locations;
}

const  FieldGeometry&  WorldModelTypeBase::get_field_geometry () const throw () { return field_geometry; }

int  WorldModelTypeBase::get_own_half () const throw () { return own_half; }

const GameState&  WorldModelTypeBase::get_game_state () const throw () { return gsman.get(); }

const  RobotProperties&  WorldModelTypeBase::get_robot_properties () const throw () { return robot_properties; }

void  WorldModelTypeBase::set_own_half (int h) throw () { own_half = ( h>=0 ? 1 : -1);}

void  WorldModelTypeBase::set_robot_properties (const  RobotProperties& rpr) throw () { robot_properties=rpr; }

const  RobotData&  WorldModelTypeBase::get_robot_data (Time & _time) const throw () { _time = robot_data_time; return robot_data; };

void  WorldModelTypeBase::set_robot_data (const  RobotData& rd, Time _time) throw() { robot_data = rd; robot_data_time = _time;};

std::ostream&  WorldModelTypeBase::log_stream () throw () 
{
  return null_stream;
}

void WorldModelTypeBase::reset (const Vec p, const Angle) throw () 
{
  reset (p);
}

void WorldModelTypeBase::update () throw () 
{
  update_game_state ();
  update_localisation ();
}

void WorldModelTypeBase::update_game_state () throw () 
{
  gsman.update();
}

void  WorldModelTypeBase::update_refbox (RefboxSignal sig) throw () 
{
  gsman.update (sig);
}

void  WorldModelTypeBase::startstop (bool b) throw () 
{
  gsman.set_in_game (b);
}

void WorldModelTypeBase::init_cycle (Time t1, Time t2) throw () 
{
  gsman.init_cycle (t1, t2);
}

const RobotLocation& WorldModelTypeBase::get_slfilter_robot_location (Time&) const throw () 
{
  return no_good_rloc;
}

MessageBoard& WorldModelTypeBase::get_message_board () throw () 
{
  return mboard;
}

const RobotLocation& WorldModelTypeBase::get_robot_location (Time t, bool b) throw () 
{
  return locations->get_robot_location (t, b);
}

const BallLocation& WorldModelTypeBase::get_ball_location (Time t, bool b) throw () 
{
  return locations->get_ball_location (t, b);
}

const ObstacleLocation& WorldModelTypeBase::get_obstacle_location (Time t, bool b) throw () 
{
  return locations->get_obstacle_location (t, b);
}

void WorldModelTypeBase::set_usb_work(bool h)throw ()
{
   bUsbWork = h;
}

bool WorldModelTypeBase::get_usb_work() const throw ()  { return bUsbWork; }