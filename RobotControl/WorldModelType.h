#ifndef _world_model_type_h
#define _world_model_type_h

#include "FieldGeometry.h"
#include "VisibleObject.h"
#include "DriveVector.h"
#include "RobotLocation.h"
#include "BallLocation.h"
#include "ObstacleLocation.h"
#include "RobotProperties.h"
#include "RobotData.h"
#include "GameState.h"
#include "MessageBoard.h"
    
class WorldModelType 
{

public:
	virtual ~WorldModelType () throw () {;}

	virtual const FieldGeometry& get_field_geometry () const throw () =0;
	virtual int get_own_half () const throw () =0;
	virtual const GameState&  get_game_state () const throw () =0;
	virtual const RobotLocation& get_robot_location (Time, bool) throw () =0;
	virtual const BallLocation& get_ball_location (Time, bool) throw () =0;
	virtual const ObstacleLocation& get_obstacle_location (Time, bool) throw () =0;
	virtual const RobotProperties& get_robot_properties () const throw () =0;
	virtual const RobotData& get_robot_data (Time&) const throw () =0;
	virtual const RobotLocation& get_slfilter_robot_location (Time&) const throw () =0;

	virtual void set_own_half (int) throw () =0;
	virtual void update_refbox (RefboxSignal) throw () =0;
	virtual void startstop (bool) throw () =0;

	virtual void set_drive_vector (DriveVector, Time) throw () =0;

	virtual void set_odometry (DriveVector, Time) throw () =0;
	virtual void set_compassdir (Angle, Time) throw () =0;

	virtual void set_visual_information (const VisibleObjectList&) throw () =0;

	virtual void set_visual_information (const VisibleObject&, Time) throw () =0;
	virtual void set_robot_properties (const RobotProperties&) throw () =0;

	virtual void set_robot_data (const RobotData&, Time) throw() =0;

	virtual void update () throw () =0;

	virtual void reset () throw () =0;
	virtual void reset (const Vec) throw () =0;
	virtual void reset (const Vec, const Angle) throw () =0;

	virtual std::ostream& log_stream () throw () =0;

	virtual void init_cycle (Time, Time) throw () =0;
	virtual MessageBoard& get_message_board () throw () =0;


	virtual const VisibleObjectList& get_visible_objects () throw () =0;


	// edit by lrd use to detect usb image processing
	virtual void set_usb_work (bool h) throw () =0;
    virtual bool get_usb_work () const throw () =0;

};
 
#endif

