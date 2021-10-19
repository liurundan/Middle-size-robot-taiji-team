#include "StdAfx.h"
#include "EMAObstacleFilter.h"

#include "WorldModel.h"
#include "geometry.h"
#include <cmath>

using namespace std;

EMAObstacleFilter::EMAObstacleFilter (  ConfigReader& reader, const FieldGeometry& fg) throw () :
assignments (0), ema (0.6), hysterese_lower (0.2), hysterese_higher(0.7), 
stuck_obstacle_delay(2000), remove_ball_obstacles (true) 
{
	double params[3];
	if (reader.get ("ObstacleFilter::parameters_0", params[0]) > 0 && 
		reader.get ("ObstacleFilter::parameters_1", params[1]) > 0 &&
		reader.get ("ObstacleFilter::parameters_2", params[2]) > 0 ) 
	{
		ema = params[0];
		hysterese_lower = params[1];
		hysterese_higher = params[2];
	} 
	reader.get ("ObstacleFilter::stuck_obstacle_delay", stuck_obstacle_delay);
	reader.get ("ObstacleFilter::remove_ball_obstacles", remove_ball_obstacles);
	
	poles.pos.resize(4);
	poles.width.resize(4);
	double dx = 0.5*fg.field_length+fg.pole_position_offset_x;
	double dy = 0.5*fg.field_width+fg.pole_position_offset_y;
	double w = fg.pole_diameter;
	poles.pos[0] = Vec(dx,dy);
	poles.pos[1] = Vec(-dx,dy);
	poles.pos[2] = Vec(-dx,-dy);
	poles.pos[3] = Vec(dx,-dy);
	poles.width[0]=w;
	poles.width[1]=w;
	poles.width[2]=w;
	poles.width[3]=w;  
}

EMAObstacleFilter::~EMAObstacleFilter () throw () {;}

ObstacleLocation EMAObstacleFilter::get () const throw ()
{
	ObstacleLocation oloc;
	oloc.pos.reserve(obstacles.size()+10);
	oloc.width.reserve(obstacles.size()+10);
	try
	{
		deque<ObstacleProperties>::const_iterator it = obstacles.begin();
		deque<ObstacleProperties>::const_iterator itend = obstacles.end();
		while (it<itend) 
		{
			if (it->active) 
			{
				oloc.pos.push_back(it->pos);
				oloc.width.push_back(it->width);
			}
			it++;
		}
	}
	catch (std::bad_alloc&) {;} 
	
	return oloc;
}

ObstacleLocation EMAObstacleFilter::get_with_poles () const throw () 
{
	ObstacleLocation dest = get();
	dest.pos.insert(dest.pos.end(), poles.pos.begin(), poles.pos.end());  
	dest.width.insert(dest.width.end(), poles.width.begin(), poles.width.end());
	return dest;
}

ObstacleLocation EMAObstacleFilter::get_with_poles_and_stuck () const throw () 
{
	ObstacleLocation dest = get();
	dest.pos.insert(dest.pos.end(), poles.pos.begin(), poles.pos.end());  
	dest.width.insert(dest.width.end(), poles.width.begin(), poles.width.end());
	Time now;
	RobotLocation rloc = MWM.get_robot_location (now,false);
	if( rloc.stuck.msec_since_stuck <= stuck_obstacle_delay ) 
	{
		BallLocation bloc = MWM.get_ball_location (now,false);
		const RobotProperties& rp = MWM.get_robot_properties();
		const FieldGeometry& fg = MWM.get_field_geometry ();
		double dist = rp.max_robot_radius;
		if ((bloc.pos-rloc.pos).length()< rp.max_robot_radius + fg.ball_diameter + 500)
			if (rloc.stuck.dir_of_stuck.angle (bloc.pos-rloc.pos).in_between (Angle::seven_eighth, Angle::eighth) && 
				bloc.pos_known==BallLocation::known)
				dist+=fg.ball_diameter;
			dest.pos.push_back (rloc.stuck.pos_of_stuck+dist*rloc.stuck.dir_of_stuck.normalize());
			dest.width.push_back (rp.robot_width);
	} 
	return dest;
}
 
namespace 
{
	
	inline double tolerance_radius (const double& v) 
	{
		return (v<2000 ? 500 : (v>4000 ? 1000 : v/4));
	}
	
}

void EMAObstacleFilter::update (const VisibleObjectList& vis, const RobotLocation& rloc, 
								const BallLocation& bloc) throw () 
{
	try
	{
		assignments.clear();
		vector<VisibleObject>::const_iterator visit (vis.objectlist.begin());
		deque<ObstacleProperties>::iterator obsit;
		unsigned int size_vis = vis.objectlist.size();
		unsigned int size_obs = obstacles.size();
		AssignmentProperties ap;
		for (unsigned int i=0; i<size_vis; i++) 
		{
			ap.pos = rloc.pos+(visit->pos).rotate(rloc.heading);
			ap.width = visit->width;
			ap.nearest_index = -1;
			ap.nearest_dist = 1e100;
			double tolerance = tolerance_radius ((visit->pos).length());
			obsit = obstacles.begin();
			
			for (unsigned int j=0; j<size_obs; j++) 
			{
				double d=(obsit->pos-ap.pos).length();
				if (d<=tolerance && d<ap.nearest_dist) 
				{
					ap.nearest_dist=d;
					ap.nearest_index=j;
				}
				obsit++;
			}
			assignments.push_back (ap);
			visit++;
		}
		
		obsit = obstacles.begin();
		vector<AssignmentProperties>::iterator asit;
		unsigned int size_as = assignments.size();
		i=0;
		while (obsit<obstacles.end())
		{
			asit = assignments.begin();
			int index = -1;
			double mindist = 1e100;
			for (unsigned int j=0; j<size_as; j++) 
			{
				if (asit->nearest_index==i) 
				{
					if (index>=0) 
					{
						if (asit->nearest_dist < assignments[index].nearest_dist) 
						{
							assignments[index].nearest_index=-1;
							index=j;
							mindist=asit->nearest_dist;
						} 
						else
							asit->nearest_index=-1;
					} 
					else 
					{
						index=j;
						mindist=asit->nearest_dist;
					}
				}
				asit++;
			}
			
			if (index>=0) 
			{
				obsit->pos=ema*obsit->pos+(1-ema)*assignments[index].pos;
				obsit->width=ema*obsit->width+(1-ema)*assignments[index].width;
				obsit->probability=ema*obsit->probability+1-ema;
				if (!obsit->active && obsit->probability>=hysterese_higher)
					obsit->active=true;
				obsit++;        
			} 
			else 
			{
				obsit->probability*=ema;
				if (obsit->probability>=hysterese_lower)
					obsit++;      
				else
					obsit = obstacles.erase (obsit);  
			}
			i++;
		}
		
		asit = assignments.begin();
		ObstacleProperties op;
		for ( i=0; i<size_as; i++) 
		{
			if (asit->nearest_index==-1) 
			{
				op.pos=asit->pos;
				op.width=asit->width;
				op.probability=1-ema;
				op.active=false;
				obstacles.push_back (op);
			}
			asit++;
		}
		
		if (remove_ball_obstacles) 
		{
			try
			{
				Line robot_ball (bloc.pos, rloc.pos);
				double rbd2 = (bloc.pos-rloc.pos).squared_length();
				deque<ObstacleProperties>::iterator oit = obstacles.begin();
				deque<ObstacleProperties>::iterator oitend = obstacles.end();
				while (oit<oitend) 
				{
					Vec p = robot_ball.perpendicular_point (oit->pos);
					if ((oit->width<300) && ((oit->pos-bloc.pos).squared_length()<90000) && 
						((p-(oit->pos)).squared_length()<40000) && ((p-rloc.pos).squared_length()<rbd2))
					{
						oit = obstacles.erase (oit);
						oitend = obstacles.end();
					} 
					else
						oit++;
				}
			}
			catch(std::invalid_argument&){;} 
		}
		
  }
  catch(std::bad_alloc){;}    
}


