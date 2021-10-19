#include "stdafx.h"
#include "FieldGeometry.h"
#include "math.h"
#include "MyAngle.h"

#define FIELD_12_8_DIMENSION 1
#define FIELD_18_12_DIMENSION 0

FieldGeometry::FieldGeometry () throw () 
{
    // field of size 12*8m data
#if FIELD_12_8_DIMENSION
  	field_length		=	12000;
	field_width		    =	8000;
	side_band_width		=	1000;
	goal_band_width		=	500;
	goal_area_length	=	500;
	goal_area_width		=	3000;
	penalty_area_length	=	1500;
	penalty_area_width	=	5000;
	center_circle_radius=	1000;
	corner_arc_radius	=	500;
	penalty_marker_distance	=	2000;
	line_thickness		    =	50;
	border_line_thickness	=	100;
	goal_width		=	2000;
	goal_length		=	500;
	goal_height		=	1000;
	pole_height		=	1000;
	pole_diameter	=	200;
	pole_position_offset_x	=	500;
	pole_position_offset_y	=	500;
	ball_diameter		=	220;   
#endif
#if FIELD_18_12_DIMENSION
	/* field size of 12*18m */
 /* 	field_length	    	=	18000; // 12000mm 2w
	field_width		        =	12000; // 8000  2h

	side_band_width		    =	1000;  // 
	goal_band_width		    =	500;

	goal_area_length	    =	750;   // 500   g
	goal_area_width		    =	3500;  //  3000 2s

	penalty_area_length	    =	2250;  // 1500  p
	penalty_area_width	    =	6500;  // 5000  2q

	center_circle_radius    =	2000;  // 1000  r
	corner_arc_radius	    =	750;   // 500    a
	penalty_marker_distance	=	0;     // 3000 2000 // odeserver this value must be zero
	line_thickness		    =	50;
	border_line_thickness	=	100;

	goal_width		        =	2000;
	goal_length		        =	500;
	goal_height		        =	1000;

	pole_height		        =	1000;
	pole_diameter		    =	200;
	pole_position_offset_x	=	500;
	pole_position_offset_y	=	500;
	ball_diameter		    =	220;*/
#endif    
	lineset_type            =   0;     // deal with different lines for each field
	// 1-5 
	lines.push_back (LineSegment (Vector(-0.5*field_length,-0.5*field_width ), Vector(0.5*field_length,-0.5*field_width)));
	lines.push_back (LineSegment (Vector(-0.5*field_length,0.5*field_width ), Vector(0.5*field_length,0.5*field_width)));
 	lines.push_back (LineSegment (Vector(-0.5*field_length,-0.5*field_width), Vector(-0.5*field_length,0.5*field_width)));
 	lines.push_back (LineSegment (Vector(0.5*field_length,-0.5*field_width), Vector(0.5*field_length,0.5*field_width )));
	lines.push_back (LineSegment (Vector(0,-0.5*field_width), Vector(0,0.5*field_width)));
    
	//6-7
	lines.push_back (LineSegment (Vector (-0.5*field_length+penalty_area_length,-0.5*penalty_area_width), Vector(-0.5*field_length+penalty_area_length,0.5*penalty_area_width )));
	lines.push_back (LineSegment (Vector (0.5*field_length-penalty_area_length,-0.5*penalty_area_width), Vector(0.5*field_length-penalty_area_length,0.5*penalty_area_width)));
	//8-9
	lines.push_back (LineSegment (Vector (-0.5*field_length+goal_area_length,-0.5*goal_area_width), Vector(-0.5*field_length+goal_area_length,0.5*goal_area_width)));
	lines.push_back (LineSegment (Vector (0.5*field_length-goal_area_length,-0.5*goal_area_width), Vector(0.5*field_length-goal_area_length,0.5*goal_area_width)));
    //10-11
	lines.push_back (LineSegment (Vector (-0.5*field_length,0.5*penalty_area_width), Vector(-0.5*field_length+penalty_area_length,0.5*penalty_area_width)));
	lines.push_back (LineSegment (Vector (0.5*field_length,0.5*penalty_area_width), Vector(0.5*field_length-penalty_area_length,0.5*penalty_area_width)));
	//12-13
	lines.push_back (LineSegment (Vector (0.5*field_length,0.5*goal_area_width), Vector(0.5*field_length-goal_area_length,0.5*goal_area_width)));
 	lines.push_back (LineSegment (Vector (-0.5*field_length,0.5*goal_area_width), Vector(-0.5*field_length+goal_area_length,0.5*goal_area_width)));
	//14-15
	lines.push_back (LineSegment (Vector (-0.5*field_length,-0.5*goal_area_width), Vector(-0.5*field_length+goal_area_length,-0.5*goal_area_width)));
	lines.push_back (LineSegment (Vector (0.5*field_length,-0.5*goal_area_width), Vector(0.5*field_length-goal_area_length,-0.5*goal_area_width)));

	//16-17
	lines.push_back (LineSegment (Vector (-0.5*field_length,-0.5*penalty_area_width), Vector(-0.5*field_length+penalty_area_length,-0.5*penalty_area_width)));
	lines.push_back (LineSegment (Vector (0.5*field_length,-0.5*penalty_area_width), Vector(0.5*field_length-penalty_area_length,-0.5*penalty_area_width)));
    //18-21
	arcs.push_back (MyArc(Vector(-0.5*field_length,0.5*field_width),corner_arc_radius,MyAngle::three_quarters,MyAngle::zero)); 
	arcs.push_back (MyArc(Vector(0.5*field_length,0.5*field_width),corner_arc_radius,MyAngle::half,MyAngle::three_quarters));
	arcs.push_back (MyArc(Vector(0.5*field_length,-0.5*field_width),corner_arc_radius,MyAngle::quarter,MyAngle::half)); 
	arcs.push_back (MyArc(Vector(-0.5*field_length,-0.5*field_width),corner_arc_radius,MyAngle::zero,MyAngle::quarter));
	
	// 22
//	arcs.push_back (MyArc(Vector(0,0),center_circle_radius, MyAngle::zero, MyAngle::deg_angle(259.99)));
    arcs.push_back (MyArc(Vector(0,0),center_circle_radius, MyAngle::zero, MyAngle::half));   
    arcs.push_back(MyArc(Vector(0,0),center_circle_radius, MyAngle::half, MyAngle::zero));   

	// 8 goallines
	goallines.push_back( LineSegment( Vector(-0.5*field_length,-0.5*goal_width), 
		                              Vector(-0.5*field_length,0.5*goal_width) ));
	goallines.push_back( LineSegment( Vector(0.5*field_length,-0.5*goal_width), 
		                              Vector(0.5*field_length,0.5*goal_width) ));
    // blue goal 3 lines
	goallines.push_back( LineSegment( Vector(-0.5*field_length,-0.5*goal_width), 
		                              Vector(-0.5*field_length-goal_length,-0.5*goal_width) ));
	goallines.push_back( LineSegment( Vector(-0.5*field_length,0.5*goal_width), 
		                              Vector(-0.5*field_length-goal_length,0.5*goal_width) ));
	goallines.push_back( LineSegment( Vector(-0.5*field_length-goal_length,-0.5*goal_width), 
		                              Vector(-0.5*field_length-goal_length,0.5*goal_width) ));
	// yellow goal 3 lines
	goallines.push_back( LineSegment( Vector(0.5*field_length,-0.5*goal_width), 
		                              Vector(0.5*field_length+goal_length,-0.5*goal_width) ));
	goallines.push_back( LineSegment( Vector(0.5*field_length,0.5*goal_width), 
		                              Vector(0.5*field_length+goal_length,0.5*goal_width) ));
	goallines.push_back( LineSegment( Vector(0.5*field_length+goal_length,-0.5*goal_width), 
		                              Vector(0.5*field_length+goal_length,0.5*goal_width) ));
}

RobotConf::RobotConf()
{
	noise_level			    =	10;
	mis_probability			=	0.2;
	goal_mis_probability    =	0.5;
	ball_mis_probability	=	0.1;
	line_vision_radius		=	4000;
}