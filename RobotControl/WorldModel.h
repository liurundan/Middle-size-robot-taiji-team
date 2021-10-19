#ifndef _world_model_h
#define _world_model_h

#include "WorldModelType.h"
#include "RobotsException.h"
#include "MessageBoard.h"
#include <iostream>
#include "dataClass.h"
#include "WSpset.h"

class AddComUserInterface;

class WorldModel
{
public:
    static WorldModel& get_main_world_model () throw ();
    static bool is_main_world_model_available () throw ();

    WorldModel (  ConfigReader&) throw (RobotsException,std::bad_alloc);
    ~WorldModel () throw ();

    bool change_world_model_type (const char*) throw ();
 
    bool change_world_model_type (const char*,   ConfigReader&) throw ();

    const char* get_world_model_type () const throw ();

    inline const FieldGeometry& get_field_geometry () const throw () { return the_world_model->get_field_geometry(); }
    inline int get_own_half () const throw () { return the_world_model->get_own_half(); }
    inline const GameState& get_game_state () const throw () { return the_world_model->get_game_state(); }
    inline const RobotLocation& get_robot_location (Time t, bool ia =true) throw () { return the_world_model->get_robot_location(t, ia); }
    inline const BallLocation& get_ball_location (Time t, bool ia =true) throw () { return the_world_model->get_ball_location(t, ia); }
    inline const ObstacleLocation& get_obstacle_location (Time t, bool ia = true) throw () { return the_world_model->get_obstacle_location(t, ia); }
    inline const RobotProperties& get_robot_properties () const throw () { return the_world_model->get_robot_properties(); }
    inline const RobotData& get_robot_data (Time& t) const throw () { return the_world_model->get_robot_data (t);};
    inline const RobotLocation& get_slfilter_robot_location (Time& t) const throw () { return the_world_model->get_slfilter_robot_location (t); }
 
	inline void set_own_half (int h) throw () { the_world_model->set_own_half(h); }
    inline void update_refbox (RefboxSignal sig) throw () { the_world_model->update_refbox(sig); }
    inline void startstop (bool b) throw () { the_world_model->startstop (b); }
    inline void set_drive_vector (DriveVector dv, Time t) throw () { the_world_model->set_drive_vector(dv,t); }
    inline void set_odometry (DriveVector dv, Time t) throw () { the_world_model->set_odometry(dv,t); }
    inline void set_compassdir (Angle h, Time t) throw () { the_world_model->set_compassdir(h,t); }
    inline void set_visual_information (const VisibleObjectList& vol) throw () { the_world_model->set_visual_information(vol); }
    inline void set_visual_information (const VisibleObject& vo, Time t) throw () { the_world_model->set_visual_information(vo,t); }
    inline void set_robot_properties (const RobotProperties& rpr) throw () { the_world_model->set_robot_properties (rpr); }
    inline void set_robot_data (const RobotData& rd, Time t) throw() {the_world_model->set_robot_data ( rd, t);};

    inline void update () throw () { the_world_model->update(); update_data(); }
    inline void reset () throw () { the_world_model->reset(); }
    inline void reset (const Vec p) throw () { the_world_model->reset(p); }
    inline void reset (const Vec p , const Angle a) throw () { the_world_model->reset(p, a); }
    inline std::ostream& log_stream () throw () { return the_world_model->log_stream (); }
    inline void init_cycle (Time t1, Time t2) throw () { the_world_model->init_cycle(t1, t2); }
    inline MessageBoard& get_message_board () throw () { return the_world_model->get_message_board(); }
 
	// edit by lrd use to detect usb image processing
	inline void set_usb_work (bool h) throw () { the_world_model->set_usb_work(h); }
    inline bool get_usb_work () const throw () { return the_world_model->get_usb_work(); }
    
	void        update_data();
	double      pos_err;
    bool        usb_camera_valid;
	
	Ball		ball2Robot;
	Vector		goal2Robot;
	Vector		myGoal2Robot;
	Vector		myPosition;
	Vector		ballPosition;
	Vec         posBall;
    Vec         posVec_me;
	Angle       heading_me;

 	Object		oppGoalObject;
	Object		myGoalObject;
	Object		nearPoul;
	std::vector<Object>      obstacle;
	
	Teammate	teammate[4];
	MaxMinDis	avoidData;

	double	    shootAngle;

	int		    obstacles_num;
	double	    oldGoAngle;
	double	    fieldCenterAngle;
	double	    fieldCenterDistance;

	int			state;
	int			role;
 	int			reaction;
	int			robotMode;
	int			action;		

	int			flag1;	 
	int			flag2;
	int			flag3;	 
 	int			receive_data;
	WSpset      valid_opponents;
	
protected:
	inline const VisibleObjectList& get_visible_objects () throw () 
	{ return the_world_model->get_visible_objects(); }
	
	friend class AddComUserInterface;
    friend class RobotsUdpServer;
	
private:
	WorldModelType* the_world_model;
    char*			world_model_descriptor;
    ConfigReader&	configuration_list;

    void really_change_world_model_type (const char*,   ConfigReader&) throw (RobotsException,std::bad_alloc);

    static WorldModel* main_world_model;
};

#define MWM  WorldModel::get_main_world_model()
#define LOUT WorldModel::get_main_world_model().log_stream()

#endif

