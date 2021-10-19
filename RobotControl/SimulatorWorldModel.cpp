#include "StdAfx.h"
#include "SimulatorWorldModel.h"
#include "WorldModelFactory.h"
#include "Journal.h"

using namespace std;

SimulatorWorldModel::SimulatorWorldModel ( ConfigReader& reader) throw (std::bad_alloc, InvalidConfigurationException) 
	: WorldModelTypeBase (reader), the_sim_client (NULL), ball_filter (reader), 
		obstacle_filter (reader, get_field_geometry()), odobox (20, 5, 5), velocity_filter (10), 
		stuck_sensor (reader) 
{
  string confline;
 /* if( reader.get("Simulator::robot_config_file", confline) <=0 )
  {
    JERROR("no config line \"Simulator::robot_config_file\" found");
    throw InvalidConfigurationException("Simulator::robot_config_file");
  }*/
  if (reader.get("Simulator::odeserver_address", confline)<=0) 
  {
	  JERROR("no config line \"odeserver_address\" found");
	  throw InvalidConfigurationException("Simulator::odeserver_address");
  }

  try
  {
     the_sim_client = SimClient::get_sim_client (confline.c_str());
  }
  catch(std::invalid_argument&)
  {
    JERROR("cannot connect to simulator");
    throw InvalidConfigurationException("Simulator::robot_config_file");
  }
}

SimulatorWorldModel::~SimulatorWorldModel () throw () {;}

RobotLocation SimulatorWorldModel::get_robot (Time t) const throw () 
{
  RobotLocation dest = odobox.add_movement (cpos, cpos_time, t);
  dest.stuck = stuck_sensor.get_stuck_location(dest.pos, dest.vtrans);
  return dest;
}

BallLocation SimulatorWorldModel::get_ball (Time t) const throw () 
{
  return ball_filter.get(t);
}

ObstacleLocation SimulatorWorldModel::get_obstacles (Time) const throw () 
{
  return obstacle_filter.get_with_poles_and_stuck();
}

void SimulatorWorldModel::set_drive_vector (DriveVector dv, Time t) throw () 
{
  odobox.add_drive_vector (dv, t);
}

void SimulatorWorldModel::set_odometry (DriveVector, Time) throw () {;}

void SimulatorWorldModel::set_visual_information (const VisibleObjectList&) throw () {;}

void SimulatorWorldModel::set_visual_information (const VisibleObject&, Time) throw () {;}

void SimulatorWorldModel::update_localisation () throw () 
{
  the_sim_client->update();
  double own = -get_own_half();
  Angle add = (own<0.0 ? Angle::half : Angle::zero);
  cpos.pos = own*the_sim_client->robot_position;
  cpos.heading = the_sim_client->robot_heading+add;
  cpos.quality = 1;
  cpos_time = the_sim_client->timestamp;
  ball_filter.update(own*the_sim_client->ball_position, the_sim_client->timestamp, cpos.pos);
  vector<double> olwidth (0);
  vector<Vec> olpos (the_sim_client->obstacle_positions.size());
  olwidth.assign (the_sim_client->obstacle_positions.size(), 600);  
  for (unsigned int i=0; i<the_sim_client->obstacle_positions.size(); i++)
    olpos[i]=own*the_sim_client->obstacle_positions[i];
  obstacle_filter.update(olpos, olwidth, the_sim_client->timestamp);
  velocity_filter.update(cpos, cpos_time);
  stuck_sensor.update ();
}

const RobotLocation& SimulatorWorldModel::get_slfilter_robot_location (Time& t) const throw () 
{
  return velocity_filter.get (t);
}

void SimulatorWorldModel::reset () throw () {;}

void SimulatorWorldModel::reset (const Vec) throw () {;}

Time SimulatorWorldModel::get_timestamp_latest_update () const throw () { return cpos_time; }