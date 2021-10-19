#ifndef _PhysicalMotionModel_
#define _PhysicalMotionModel_

#include "DriveVector.h"
#include "RobotProperties.h"
#include "RobotLocation.h"
#include "FieldGeometry.h"
#include "Time.h"

class PhysicalMotionModel
{
public:
    PhysicalMotionModel (const RobotProperties&) throw ();
    ~PhysicalMotionModel () throw ();
    void direct_approach_stop (const Vec&, const Angle&, const RobotLocation&, bool =true, bool =true) throw ();
    void direct_approach_stop (const Vec&, const Angle&, const RobotLocation&, double, double, bool =true, bool =true) throw ();
    void direct_approach_nostop (const Vec&, const Angle&, const RobotLocation&, bool =true, bool =true) throw ();
    void direct_approach_nostop (const Vec&, const Angle&, const RobotLocation&, double, double, bool =true, bool =true) throw ();
    void dogs_approach_stop (const Vec&, const Angle&, double, int, const RobotLocation&) throw ();
    void set_no_motion () throw ();
    void set_drive_vector (const DriveVector&, const RobotLocation&) throw ();
    void set_kick (bool =true) throw ();
    bool avoid_point_collision (const Vec&) throw ();
    bool avoid_into_goal (const FieldGeometry&, double) throw ();  
    bool avoid_abrupt_changes () throw ();
    bool avoid_own_goal (const FieldGeometry&, double) throw ();
    const DriveVector& get_drive_vector (bool =false) throw ();

private:

	bool first_cycle;                  
    Time cycle_timer;                   
    double average_cycle_time;          
    DriveVector latest_drive_vector;    
    const RobotProperties properties;   

    Vec target_vtrans;                  
    double target_vrot;                 
    bool target_kick;                   
    Vec current_position;              
    Angle current_heading;              

    double max_dec_vel (double) throw ();     
    double max_rot_dec_vel (double) throw ();  

};

#endif
