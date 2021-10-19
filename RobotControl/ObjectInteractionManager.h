#ifndef _ObjectInteractionManager_h
#define _ObjectInteractionManager_h

#include "RobotLocation.h"
#include "BallLocation.h"
#include "ObstacleLocation.h"

class ObjectInteractionManager
{
public:   
	void get (RobotLocation&, BallLocation&, ObstacleLocation&, 
		const RobotLocation&, const BallLocation&, unsigned int) 
		const throw (std::bad_alloc);

};

#endif