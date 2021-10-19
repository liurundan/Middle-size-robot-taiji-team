#include "StdAfx.h"
#include "ErrorMinimiserWorldModel.h"
#include "update_robot_location.h"

using namespace std;

ErrorMinimiserWorldModel::ErrorMinimiserWorldModel ( ) throw ( bad_alloc):
sl( odobox, FG ),odobox(20, 5, 10)   
{
  ; 
}

ErrorMinimiserWorldModel::~ErrorMinimiserWorldModel () throw () {;}

void ErrorMinimiserWorldModel::set_drive_vector (DriveVector dv, MyTime t) throw () 
{
  odobox.add_drive_vector (dv, t);
}

void ErrorMinimiserWorldModel::set_odometry (DriveVector dv, MyTime t) throw ()
{
  odobox.add_odometry (dv,t);
}

void ErrorMinimiserWorldModel::set_odometry(RobotLocation res) throw()
{
	RobotLocation rp;
	rp.heading = res.heading;
	rp.pos     = res.pos;
  	odobox.Setodometry(rp);
}

void ErrorMinimiserWorldModel::set_visual_information (const VisibleObjectList& vol) throw () 
{
  visbox.add (vol);
}

void ErrorMinimiserWorldModel::set_visual_information (const VisibleObject& vo, MyTime t) throw ()
{
  visbox.add (vo, t);
}

void ErrorMinimiserWorldModel::reset () throw () 
{
  sl.reset ();
}

void ErrorMinimiserWorldModel::reset (  Vector p) throw () 
{
  sl.reset (p*-1);
}

void ErrorMinimiserWorldModel::reset (  Vector p, const MyAngle h) throw ()
 {
  sl.reset (p*-1,h+( -1==-1? MyAngle::half : MyAngle::zero));
}

void ErrorMinimiserWorldModel::update_localisation () throw () 
{
  sl.update (visbox.get_lines(), visbox.get_goals());
  latest_vis_timestamp = visbox.get_lines().timestamp;
  RobotLocation pos_tvis = flip_sides (sl.get (latest_vis_timestamp), 1);
}

RobotLocation ErrorMinimiserWorldModel::get_robot (MyTime t) const throw () 
{
  RobotLocation dest =  flip_sides (sl.get (t), 1 ); // get_own_half() default value is yellow_side
  return dest;
}
 
MyTime ErrorMinimiserWorldModel::get_timestamp_latest_update () const throw () 
{ return latest_vis_timestamp; }

const VisibleObjectList& ErrorMinimiserWorldModel::get_visible_objects () throw () 
{ return visbox.get_all(); }
