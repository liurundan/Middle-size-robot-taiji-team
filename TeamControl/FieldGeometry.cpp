
#include "StdAfx.h"
#include "FieldGeometry.h"
#include <cmath>

FieldGeometry::FieldGeometry () throw () {;}

FieldGeometry::FieldGeometry ( ConfigReader& vread) throw (InvalidConfigurationException) 
{
	field_length = 0;
    field_width  = 0;
	if (vread.get ("FieldGeometry::field_length", field_length)<=0 || field_length<0)
		throw  InvalidConfigurationException ("field_length");
	if (vread.get ("FieldGeometry::field_width", field_width)<=0 || field_width<0)
		throw  InvalidConfigurationException ("field_width");
	if (vread.get ("FieldGeometry::side_band_width", side_band_width)<=0 || side_band_width<0)
		throw  InvalidConfigurationException ("side_band_width");
	if (vread.get ("FieldGeometry::goal_band_width", goal_band_width)<=0 || goal_band_width<0)
		throw  InvalidConfigurationException ("goal_band_width");
	if (vread.get ("FieldGeometry::goal_area_length", goal_area_length)<=0 || goal_area_length<0)
		throw  InvalidConfigurationException ("goal_area_length");
	if (vread.get ("FieldGeometry::goal_area_width", goal_area_width)<=0 || goal_area_width<0)
		throw  InvalidConfigurationException ("goal_area_width");
	if (vread.get ("FieldGeometry::penalty_area_length", penalty_area_length)<=0 || penalty_area_length<0)
		throw  InvalidConfigurationException ("penalty_area_length");
	if (vread.get ("FieldGeometry::penalty_area_width", penalty_area_width)<=0 || penalty_area_width<0)
		throw  InvalidConfigurationException ("penalty_area_width");
	if (vread.get ("FieldGeometry::center_circle_radius", center_circle_radius)<=0 || center_circle_radius<0)
		throw  InvalidConfigurationException ("center_circle_radius");
	if (vread.get ("FieldGeometry::corner_arc_radius", corner_arc_radius)<=0 || corner_arc_radius<0)
		throw  InvalidConfigurationException ("corner_arc_radius");
	if (vread.get ("FieldGeometry::penalty_marker_distance", penalty_marker_distance)<=0 || penalty_marker_distance<0)
		throw  InvalidConfigurationException ("penalty_marker_distance");
	if (vread.get ("FieldGeometry::line_thickness", line_thickness)<=0 || line_thickness<0)
		throw  InvalidConfigurationException ("line_thickness");
	if (vread.get ("FieldGeometry::border_line_thickness", border_line_thickness)<=0 || border_line_thickness<0)
		throw  InvalidConfigurationException ("border_line_thickness");
	if (vread.get ("FieldGeometry::goal_width", goal_width)<=0 || goal_width<0)
		throw  InvalidConfigurationException ("goal_width");
	if (vread.get ("FieldGeometry::goal_length", goal_length)<=0 || goal_length<0)
		throw  InvalidConfigurationException ("goal_length");
	if (vread.get ("FieldGeometry::goal_height", goal_height)<=0 || goal_height<0)
		throw  InvalidConfigurationException ("goal_height");
	if (vread.get ("FieldGeometry::pole_height", pole_height)<=0 || pole_height<0)
		throw  InvalidConfigurationException ("pole_height");
	if (vread.get ("FieldGeometry::pole_diameter", pole_diameter)<=0 || pole_diameter<0)
		throw  InvalidConfigurationException ("pole_diameter");
	if (vread.get ("FieldGeometry::pole_position_offset_x", pole_position_offset_x)<=0 || pole_position_offset_x<0)
		throw  InvalidConfigurationException ("pole_position_offset_x");
	if (vread.get ("FieldGeometry::pole_position_offset_y", pole_position_offset_y)<=0 || pole_position_offset_y<0)
		throw  InvalidConfigurationException ("pole_position_offset_y");
	if (vread.get ("FieldGeometry::ball_diameter", ball_diameter)<=0 || ball_diameter<0)
		throw  InvalidConfigurationException ("ball_diameter");
	
	std::string lineset;
	if (vread.get ("FieldGeometry::lineset_type", lineset)<=0)
		lineset_type=0;
	else 
	{
		if (lineset == "center_circle")
		  lineset_type=0;
		else if (lineset == "center_triangle")
		  lineset_type=1;
		else if (lineset == "center_triangle_inv")
		  lineset_type=2;
		else
		  throw  InvalidConfigurationException ("lineset_type");
	}
}

bool  FieldGeometry::operator== (const FieldGeometry& fg) const throw () 
{
	if (fabs(field_length-fg.field_length)>10) return false;
	if (fabs(field_width-fg.field_width)>10) return false;
	if (fabs(side_band_width-fg.side_band_width)>10) return false;
	if (fabs(goal_band_width-fg.goal_band_width)>10) return false;
	if (fabs(goal_area_length-fg.goal_area_length)>10) return false;
	if (fabs(goal_area_width-fg.goal_area_width)>10) return false;
	if (fabs(penalty_area_length-fg.penalty_area_length)>10) return false;
	if (fabs(penalty_area_width-fg.penalty_area_width)>10) return false;
	if (fabs(center_circle_radius-fg.center_circle_radius)>10) return false;
	if (fabs(corner_arc_radius-fg.corner_arc_radius)>10) return false;
	if (fabs(penalty_marker_distance-fg.penalty_marker_distance)>10) return false;
	if (fabs(line_thickness-fg.line_thickness)>10) return false;
	if (fabs(border_line_thickness-fg.border_line_thickness)>10) return false;
	if (fabs(goal_width-fg.goal_width)>10) return false;
	if (fabs(goal_length-fg.goal_length)>10) return false;
	if (fabs(goal_height-fg.goal_height)>10) return false;
	if (fabs(pole_diameter-fg.pole_diameter)>10) return false;
	if (fabs(pole_height-fg.pole_height)>10) return false;
	if (fabs(pole_position_offset_x-fg.pole_position_offset_x)>10) return false;
	if (fabs(pole_position_offset_y-fg.pole_position_offset_y)>10) return false;
	if (fabs(ball_diameter-fg.ball_diameter)>10) return false;
	if (lineset_type!=fg.lineset_type) return false;
	return true;
}

bool  FieldGeometry::operator!= (const FieldGeometry& fg) const throw () 
{
	return !operator== (fg);
}
