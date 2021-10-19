#ifndef _EMAObstacleFilter_h
#define _EMAObstacleFilter_h

#include <deque>
#include "VisibleObject.h"
#include "ObstacleLocation.h"
#include "RobotLocation.h"
#include "BallLocation.h"
#include "FieldGeometry.h"
#include "ConfigReader.h"

class EMAObstacleFilter
{
public:
	EMAObstacleFilter (  ConfigReader&, const FieldGeometry&) throw ();
	~EMAObstacleFilter () throw ();

	ObstacleLocation get () const throw ();
	ObstacleLocation get_with_poles () const throw ();
	ObstacleLocation get_with_poles_and_stuck () const throw ();
	void update (const VisibleObjectList&, const RobotLocation&, const BallLocation&) throw ();
	protected:
private:
	  
	struct ObstacleProperties 
	{
		Vec pos;
		double width;
		double probability;
		bool active;
	};
	struct AssignmentProperties 
	{
		Vec pos;
		double width;
		double nearest_dist;
		int nearest_index;
	};
	std::deque<ObstacleProperties> obstacles;        
	std::vector<AssignmentProperties> assignments;   
	double ema;                                      
	double hysterese_lower;                          
	double hysterese_higher;                        
	ObstacleLocation poles;                         
	unsigned int stuck_obstacle_delay;               
	bool remove_ball_obstacles;                      
};

 

#endif
 