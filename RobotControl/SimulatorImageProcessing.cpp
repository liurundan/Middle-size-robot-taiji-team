#include "StdAfx.h"
#include "SimulatorImageProcessing.h"
#include "ImageProcessingFactory.h"
#include "random.h"
#include "Journal.h"
#include "WorldModel.h"
#include <cmath>

using namespace std;
  
SimulatorImageProcessing::SimulatorImageProcessing (ConfigReader& reader) throw (std::bad_alloc, InvalidConfigurationException) : the_sim_client (NULL) 
{
	string confline;
/*	if (reader.get("Simulator::robot_config_file", confline)<=0) 
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
		the_sim_client=SimClient::get_sim_client (confline.c_str());
	}
	catch(std::invalid_argument&)
	{
		JERROR("cannot connect to simulator");
		throw InvalidConfigurationException("Simulator::robot_config_file");
	}
	
	noise_level = 0.001;
	mis_probability = 0;
	goal_mis_probability=0;
	ball_mis_probability=0;
	line_vision_radius = 6000;
 	double d;
	if (reader.get ("SimulatorImageProcessing::noise_level",d)>=1 && d>0)
		noise_level=d;
	if (reader.get ("SimulatorImageProcessing::mis_probability",d)>=1 && d>=0 && d<=1)
		mis_probability=d;
	if (reader.get ("SimulatorImageProcessing::goal_mis_probability",d)>=1 && d>=0 && d<=1)
		goal_mis_probability=d;
	if (reader.get ("SimulatorImageProcessing::ball_mis_probability",d)>=1 && d>=0 && d<=1)
		ball_mis_probability=d;
	if (reader.get ("SimulatorImageProcessing::line_vision_radius",d)>=1 && d>=0)
		line_vision_radius=d;
 
	const FieldGeometry& fg = WorldModel::get_main_world_model().get_field_geometry();
	lines.push_back (LineSegment (Vec(-0.5*fg.field_length,-0.5*fg.field_width ), 
		Vec(0.5*fg.field_length,-0.5*fg.field_width)));
	lines.push_back (LineSegment (Vec(-0.5*fg.field_length,0.5*fg.field_width ), 
		Vec(0.5*fg.field_length,0.5*fg.field_width)));
	lines.push_back (LineSegment (Vec(-0.5*fg.field_length,-0.5*fg.field_width), 
		Vec(-0.5*fg.field_length,0.5*fg.field_width)));
	lines.push_back (LineSegment (Vec(0.5*fg.field_length,-0.5*fg.field_width),
		Vec(0.5*fg.field_length,0.5*fg.field_width )));
	lines.push_back (LineSegment (Vec(0,-0.5*fg.field_width), Vec(0,0.5*fg.field_width)));

	if (fg.goal_area_length>0 && fg.goal_area_width>0) 
	{
		//8-9
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width), 
			Vec(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width)));
		lines.push_back (LineSegment (Vec (0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width), 
			Vec(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width)));
		//12-13
		lines.push_back (LineSegment (Vec (0.5*fg.field_length,0.5*fg.goal_area_width),
			Vec(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width)));
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length,0.5*fg.goal_area_width),
			Vec(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width)));
		//14-15
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length,-0.5*fg.goal_area_width), 
			Vec(-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width)));
		lines.push_back (LineSegment (Vec (0.5*fg.field_length,-0.5*fg.goal_area_width),
			Vec(0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width)));
	}
	if (fg.penalty_area_length>0 && fg.penalty_area_width>0) 
	{
		//6-7
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width), 
			Vec(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width )));
		lines.push_back (LineSegment (Vec (0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width), 
			Vec(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width)));
		//10-11
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length,0.5*fg.penalty_area_width), 
			Vec(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width)));
		lines.push_back (LineSegment (Vec (0.5*fg.field_length,0.5*fg.penalty_area_width), 
			Vec(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width)));
		//16-17
		lines.push_back (LineSegment (Vec (-0.5*fg.field_length,-0.5*fg.penalty_area_width), 
			Vec(-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width)));
		lines.push_back (LineSegment (Vec (0.5*fg.field_length,-0.5*fg.penalty_area_width), 
			Vec(0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width)));
	}
	if (fg.center_circle_radius>0)
		arcs.push_back (MyArc(Vec(0,0),fg.center_circle_radius, Angle::zero, Angle::deg_angle (259.99)));
	if (fg.corner_arc_radius>0) 
	{
		//18-21
		arcs.push_back (MyArc(Vec(-0.5*fg.field_length,0.5*fg.field_width),fg.corner_arc_radius,
			Angle::three_quarters,Angle::zero)); 
		arcs.push_back (MyArc(Vec(0.5*fg.field_length,0.5*fg.field_width),fg.corner_arc_radius,
			Angle::half,Angle::three_quarters));
		arcs.push_back (MyArc(Vec(0.5*fg.field_length,-0.5*fg.field_width),fg.corner_arc_radius,
			Angle::quarter,Angle::half)); 
		arcs.push_back (MyArc(Vec(-0.5*fg.field_length,-0.5*fg.field_width),fg.corner_arc_radius,
			Angle::zero,Angle::quarter));
	}
}

SimulatorImageProcessing::~SimulatorImageProcessing () throw () {;}

void SimulatorImageProcessing::process_image () throw () 
{
	the_sim_client->update();
	const FieldGeometry& fg = WorldModel::get_main_world_model().get_field_geometry();
	VisibleObjectList vol;
	vol.timestamp = the_sim_client->timestamp;
	
	if (!brandom (ball_mis_probability))
		vol.objectlist.push_back (VisibleObject ((the_sim_client->ball_position-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::ball));
	
	std::vector<Vec>::iterator oit = the_sim_client->obstacle_positions.begin();
	std::vector<Vec>::iterator oit_end = the_sim_client->obstacle_positions.end();
	while (oit<oit_end) 
	{
		if (!brandom (mis_probability)) 
			vol.objectlist.push_back (VisibleObject (((*oit)-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::obstacle, 500));  // Hindernisbreite pauschal mit 500mm angenommen
		oit++;
	}
	
	Vec blue_goal(-0.5*fg.field_length-fg.goal_length,0 );
	if (!brandom (goal_mis_probability))
		vol.objectlist.push_back (VisibleObject ((blue_goal-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::blue_goal));
	Vec yellow_goal(0.5*fg.field_length+fg.goal_length,0 );
	if (!brandom (goal_mis_probability))
		vol.objectlist.push_back (VisibleObject ((yellow_goal-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::yellow_goal));
	
	try
	{
		deque<Line> beams;
		for (double a=0; a<180; a+=15)
			beams.push_back (Line (the_sim_client->robot_position, the_sim_client->robot_position+Vec::unit_vector (the_sim_client->robot_heading+Angle::deg_angle(a))));
		deque<Line>::iterator bit = beams.begin();
		deque<Line>::iterator bit_end = beams.end();
		deque<LineSegment>::iterator lit;
		deque<LineSegment>::iterator lit_end = lines.end();
		deque<MyArc>::iterator ait;
		deque<MyArc>::iterator ait_end = arcs.end();
		double lvr2 = line_vision_radius*line_vision_radius;
		while (bit<bit_end) 
		{
			lit=lines.begin();
			while (lit<lit_end) 
			{
				vector<Vec> pvec = intersect (*lit, *bit);
				if (pvec.size()>0) 
				{
					if ((lit->distance (pvec[0])<0.01) && (pvec[0]-the_sim_client->robot_position).squared_length()<lvr2)
						if (!brandom (mis_probability))
							vol.objectlist.push_back (VisibleObject ((pvec[0]-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::white_line));
				}  
				lit++;
			}
			ait=arcs.begin();
			while (ait<ait_end) 
			{
				try
				{
					vector<Vec> p = intersect (*ait, *bit);
					for (unsigned int i=0; i<p.size(); i++)
						if ((p[i]-the_sim_client->robot_position).squared_length()<lvr2)
							if (!brandom (mis_probability))
								vol.objectlist.push_back (VisibleObject ((p[i]-the_sim_client->robot_position).rotate(-the_sim_client->robot_heading)+noise_level*n2random(), VisibleObject::white_line));
				}
				catch(exception&){;}  
				ait++;
			}
			bit++;
		}
	}
	catch(std::bad_alloc){;} 
	WorldModel::get_main_world_model().set_visual_information(vol);
}