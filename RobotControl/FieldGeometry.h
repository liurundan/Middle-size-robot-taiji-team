#ifndef _field_geometry_h
#define _field_geometry_h

#include "ConfigReader.h"
#include "RobotsException.h"
   
struct FieldGeometry 
{
    FieldGeometry () throw ();
    
    FieldGeometry ( ConfigReader&) throw (InvalidConfigurationException);

    double field_length;           
    double field_width;           

    double side_band_width;         
    double goal_band_width;         

    double goal_area_length;       
    double goal_area_width;        
    double penalty_area_length;     
    double penalty_area_width;     
    double center_circle_radius;    
    double corner_arc_radius;      
    double penalty_marker_distance; 

    double line_thickness;         
    double border_line_thickness;  

    double goal_width;             
    double goal_length;             
    double goal_height;             

    double pole_height;             
    double pole_diameter;          
    double pole_position_offset_x;  
    double pole_position_offset_y; 

    double ball_diameter;          

    unsigned int lineset_type;      
    bool operator== (const FieldGeometry&) const throw ();
    bool operator!= (const FieldGeometry&) const throw ();
};

 

#endif