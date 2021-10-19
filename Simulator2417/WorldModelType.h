#ifndef _WorldModelType_
#define _WorldModelType_

#include "FieldGeometry.h"
#include "VisibleObject.h"
#include "DriveVector.h"
#include "RobotLocation.h"
#include "GameState.h"

class WorldModelType
{
public:
	virtual ~WorldModelType() throw() {; }

	virtual const FieldGeometry& get_field_geometry () const throw () =0;
	virtual int get_own_half () const throw () =0;
	virtual const GameState&  get_game_state () const throw () =0;
	virtual const RobotLocation& get_robot_location (MyTime, bool) throw () =0;

	virtual const RobotLocation& get_slfilter_robot_location (MyTime&) const throw () =0;
 
	virtual void set_own_half (int) throw () =0;
	virtual void update_refbox (RefboxSignal) throw () =0;
	virtual void startstop (bool) throw () =0;

	virtual void set_drive_vector (DriveVector, MyTime) throw () =0;

	virtual void set_odometry (DriveVector, MyTime) throw () =0;

	virtual void set_visual_information (const VisibleObjectList&) throw () =0;

	virtual void set_visual_information (const VisibleObject&, MyTime) throw () =0;
 
	virtual void update () throw () =0;

	virtual void reset () throw () =0;
	virtual void reset (const Vec) throw () =0;
	virtual void reset (const Vec, const Angle) throw () =0;
 
	virtual void init_cycle (MyTime, MyTime) throw () =0;
 
	virtual const VisibleObjectList& get_visible_objects () throw () =0;
protected:
private:
};
#endif