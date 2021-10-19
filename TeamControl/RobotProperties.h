#ifndef _robot_properties
#define _robot_properties

#include "ConfigReader.h"

struct RobotProperties 
{
    void read (   ConfigReader& ) throw ();

    double max_velocity;                 //  in m/s (>0)
    double max_acceleration;             //  in m/s2 (>0)
    double max_deceleration;             //  in m/s2 (>0)

    double max_rotational_velocity;      //  in rad/s (>0)
    double max_rotational_acceleration;  //  in rad/s2 (>0)
    double max_rotational_deceleration;  //  in rad/s2 (>0) 

    double max_robot_radius;              
    double min_robot_radius;             
    double robot_width;                  //in mm
    double robot_length;                 
    double kicker_width;                  
    double kicker_distance;               

    bool   omnidirectional_drive;          

    int drive_vector_delay;              
    
};

 
#endif

