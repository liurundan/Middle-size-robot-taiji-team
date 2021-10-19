#ifndef _ObstacleContainer_h
#define _ObstacleContainer_h

#include "VisibleObject.h"
#include "ObstacleLocation.h"
#include "RobotLocation.h"
#include "FieldGeometry.h"
#include "ConfigReader.h"

class ObstacleContainer
{
public:
 
    ObstacleContainer (  ConfigReader&, const FieldGeometry&) throw ();
    ObstacleLocation get () const throw ();
    ObstacleLocation get_with_poles () const throw ();
   
    ObstacleLocation get_with_poles_and_stuck () const throw ();
    void update (const VisibleObjectList&, const RobotLocation&) throw ();
   
    void update (const std::vector<Vec>&, const std::vector<double>&, Time) throw ();
 
private:
	ObstacleLocation obstacles;
    ObstacleLocation poles;
    unsigned int stuck_obstacle_delay;
};

#endif