#ifndef _PaintPreferences_h
#define _PaintPreferences_h

#include "Vec.h"
#include "geometry.h"
#include <deque>
 
struct ZoomInfo 
{
    Vec image_center;   
    double scaling;    
    int own_half;       
};


struct PaintPreferences 
{
    bool show_wm_robot;          
    bool show_wm_ball;            
    bool show_wm_obstacles;       
    bool show_vis_lines;          
    bool show_vis_ball;           
    bool show_vis_obstacles;      
    bool show_vis_goals;          
    bool show_sl_pos;             
    bool show_aux_lines;          
    
    bool show_robot_ids;          
    bool show_robot_ball_links;  
	
    bool show_robot_trace;        
    bool show_ball_trace;        
	
    bool use_exec_time;           
	
    ZoomInfo zoom;               
    int field_heading;            
	
    unsigned int reference_robot; 
	
    std::deque<ZoomInfo> old_zooms;            
    std::deque<ZoomInfo> redo_zooms;          
	
    std::deque<RobotLocation> rtrace;  
    std::deque<BallLocation> btrace;   
	
    std::deque<LineSegment> mem;       
};

struct PaintActionSelect 
{
    bool show_wm_robot;           
    bool show_wm_ball;            
    bool show_wm_obstacles;       
    bool show_vis_lines;         
    bool show_vis_ball;           
    bool show_vis_obstacles;      
    bool show_vis_goals;          
    bool show_sl_pos;             
    bool show_aux_lines;          
    bool show_robot_ids;          
    bool show_robot_ball_links;   
    bool show_robot_trace;       
    bool show_ball_trace;         
    bool clear_lines;            
    bool zoom_in;                
    bool zoom_out;                
    bool zoom_all;                
    bool zoom_undo;               
    bool zoom_redo;               
    bool flip_sides;              
    bool flip_goals;              
    bool use_exec_time;          
    bool next_refrobot;           
};
 
#endif
