#include "StdAfx.h"
#include "ErrorMinimiserWorldModel.h"
#include "update_robot_location.h"
#include "stringconvert.h"
#include "WhiteBoard.h"

using namespace std;

ErrorMinimiserWorldModel::ErrorMinimiserWorldModel ( ConfigReader& reader) 
throw (std::bad_alloc) : WorldModelTypeBase (reader), 
		odobox(20, 5, 10),combox(20),sl(reader, odobox, combox, get_field_geometry()), ball_filter (reader), 
		obstacle_filter (reader, get_field_geometry()), velocity_filter (10), stuck_sensor (reader) 
{;}

ErrorMinimiserWorldModel::~ErrorMinimiserWorldModel () throw () {;}

void ErrorMinimiserWorldModel::set_drive_vector (DriveVector dv, Time t) throw () 
{
	odobox.add_drive_vector (dv, t);
}

void ErrorMinimiserWorldModel::set_odometry (DriveVector dv, Time t) throw () 
{
	odobox.add_odometry (dv,t);
}

void ErrorMinimiserWorldModel::set_compassdir (Angle h, Time t) throw () 
{
	combox.add_compass(h,t);
}

void ErrorMinimiserWorldModel::set_visual_information (const VisibleObjectList& vol) throw () 
{
	visbox.add (vol);
}

void ErrorMinimiserWorldModel::set_visual_information (const VisibleObject& vo, Time t) throw ()
{
	visbox.add (vo, t);
}

void ErrorMinimiserWorldModel::reset () throw () 
{
	sl.reset ();
}

void ErrorMinimiserWorldModel::reset (const Vec p) throw () 
{
	sl.reset (get_own_half()*p);
}

void ErrorMinimiserWorldModel::reset (const Vec p, const Angle h) throw () 
{
	sl.reset (get_own_half()*p,h+(get_own_half()==-1? Angle::half : Angle::zero));
}

void ErrorMinimiserWorldModel::update_localisation () throw () 
{
	sl.update (visbox.get_lines(), visbox.get_goals());
	latest_vis_timestamp = visbox.get_lines().timestamp;
	RobotLocation pos_tvis = flip_sides (sl.get (latest_vis_timestamp), get_own_half());
	ball_filter.update (visbox.get_balls(), pos_tvis);
	BallLocation bloc_tvis = ball_filter.get(latest_vis_timestamp);
	obstacle_filter.update (visbox.get_obstacles(), pos_tvis, bloc_tvis);
	velocity_filter.update (pos_tvis, latest_vis_timestamp);
	stuck_sensor.update ();
	
	BallLocation bloc = ball_filter.get (latest_vis_timestamp);
	const string cb = get_message_board().scan_for_prefix ("Ball: ");
	if (cb.length()>0) 
	{
		deque<string> parts;
		split_string (parts, cb);
		if (parts.size()>=3) 
		{
			Vec bp;
			string2double (bp.x, parts[1]);
			string2double (bp.y, parts[2]);
			ball_filter.comm_ball (bp);
		}
	}

	BYTE sb = WBOARD->get_small_camera_ball();
	if( sb > 0 )
	{
		ball_filter.small_camera_ball(sb,pos_tvis);
	}
	
}

RobotLocation ErrorMinimiserWorldModel::get_robot (Time t) const throw () 
{
	RobotLocation dest = flip_sides (sl.get (t), get_own_half());
	dest.stuck = stuck_sensor.get_stuck_location (dest.pos, dest.vtrans);
	return dest;
}

const RobotLocation& ErrorMinimiserWorldModel::get_slfilter_robot_location (Time& t) const throw () 
{
	return velocity_filter.get (t);
}

BallLocation ErrorMinimiserWorldModel::get_ball (Time t) const throw () 
{
	return ball_filter.get (t);
}

ObstacleLocation ErrorMinimiserWorldModel::get_obstacles (Time) const throw () 
{
	return obstacle_filter.get_with_poles_and_stuck ();
}

Time ErrorMinimiserWorldModel::get_timestamp_latest_update () const throw () { return latest_vis_timestamp; }

const VisibleObjectList& ErrorMinimiserWorldModel::get_visible_objects () throw () { return visbox.get_all(); }
