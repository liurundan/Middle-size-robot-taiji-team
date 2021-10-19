#ifndef _LocationShortTimeMemory
#define _LocationShortTimeMemory

#include "RingBuffer.h"
#include "Time.h"
#include "RobotLocation.h"
#include "BallLocation.h"
#include "ObstacleLocation.h"
#include "WorldModelTypeBase.h"
#include "ObjectInteractionManager.h"

class LocationShortTimeMemory
{

public:
    LocationShortTimeMemory (const WorldModelTypeBase&) throw (std::bad_alloc);
    
    const RobotLocation& get_robot_location (Time, bool) throw ();
    const BallLocation&  get_ball_location (Time, bool) throw ();
    const ObstacleLocation& get_obstacle_location (Time, bool) throw ();
protected:
private:
	const WorldModelTypeBase& wm;
    ObjectInteractionManager interaction_manager;

    template<class Loc>
    struct CycleTimeLocationTriple 
	{
      unsigned long int cycle;
      Time timestamp;
      Loc value;
    };

    RingBuffer<CycleTimeLocationTriple<RobotLocation> > robots_pure;  
    RingBuffer<CycleTimeLocationTriple<RobotLocation> > robots_interacted;  
    RingBuffer<CycleTimeLocationTriple<BallLocation> > balls_pure;  
    RingBuffer<CycleTimeLocationTriple<BallLocation> > balls_interacted; 
    CycleTimeLocationTriple<ObstacleLocation> obstacles;  

    void get_interacted(Time); 
	
};

#endif