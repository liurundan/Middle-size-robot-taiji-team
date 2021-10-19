#ifndef _SimulatorWorldModel_h_
#define _SimulatorWorldModel_h_

#include "WorldModelTypeBase.h"
#include "DynamicSlidingWindowBallFilter.h"
#include "ObstacleContainer.h"
#include "OdometryContainer.h"
#include "SLVelocitySensor.h"
#include "SLStuckSensor.h"
#include "SimClient.h"


class SimulatorWorldModel : public WorldModelTypeBase 
{
private:
    SimClient* the_sim_client;         
    DynamicSlidingWindowBallFilter ball_filter;
    ObstacleContainer obstacle_filter;
    OdometryContainer odobox;
    SLVelocitySensor velocity_filter;
    SLStuckSensor stuck_sensor;
    RobotLocation cpos;
    Time cpos_time;
	
public:
    SimulatorWorldModel ( ConfigReader&) throw (std::bad_alloc, InvalidConfigurationException);
    ~SimulatorWorldModel () throw ();
	
    void update_localisation () throw ();
    RobotLocation get_robot (Time) const throw ();
    BallLocation get_ball (Time) const throw ();
    ObstacleLocation get_obstacles (Time) const throw ();
    Time get_timestamp_latest_update () const throw ();
    const RobotLocation& get_slfilter_robot_location (Time&) const throw ();
    void set_drive_vector (DriveVector, Time) throw ();
	
    void set_odometry (DriveVector, Time) throw ();
	void set_compassdir (Angle, Time) throw () {;}

    void set_visual_information (const VisibleObjectList&) throw ();
    void set_visual_information (const VisibleObject&, Time) throw ();
    void reset () throw ();
    void reset (const Vec) throw ();
};
 
#endif

