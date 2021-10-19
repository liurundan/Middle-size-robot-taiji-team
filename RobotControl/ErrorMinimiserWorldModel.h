#ifndef _ErrorMinimiserWorldModel_h_
#define _ErrorMinimiserWorldModel_h_

#include "WorldModelTypeBase.h"
#include "ErrorMinimiserSL.h"
#include "OdometryContainer.h"
#include "VisualContainer.h"
#include "DynamicSlidingWindowBallFilter.h"
#include "EMAObstacleFilter.h"
#include "SLVelocitySensor.h"
#include "SLStuckSensor.h"

class ErrorMinimiserWorldModel : public WorldModelTypeBase
{
public:
    ErrorMinimiserWorldModel (  ConfigReader&) throw (std::bad_alloc);
    ~ErrorMinimiserWorldModel () throw ();

    RobotLocation get_robot (Time) const throw ();
    BallLocation get_ball (Time) const throw ();
    ObstacleLocation get_obstacles (Time) const throw ();
    Time get_timestamp_latest_update () const throw ();
    const RobotLocation& get_slfilter_robot_location (Time&) const throw ();
    void set_drive_vector (DriveVector, Time) throw ();
    void set_odometry (DriveVector, Time) throw ();
    void set_compassdir (Angle, Time) throw ();
    void set_visual_information (const VisibleObjectList&) throw ();
    void set_visual_information (const VisibleObject&, Time) throw ();
    void update_localisation () throw ();
    void reset () throw ();
    void reset (const Vec) throw ();
    void reset (const Vec, const Angle) throw ();
    const VisibleObjectList& get_visible_objects () throw ();

protected:
private:
	
	OdometryContainer odobox;  
	CompassContainer combox;
    VisualContainer visbox;                     
    ErrorMinimiserSL sl;                        
    DynamicSlidingWindowBallFilter ball_filter; 
    EMAObstacleFilter obstacle_filter;           
    SLVelocitySensor velocity_filter;            
    SLStuckSensor stuck_sensor;                
    Time latest_vis_timestamp;                   
};

#endif