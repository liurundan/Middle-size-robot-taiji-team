#ifndef _world_model_dummy_h
#define _world_model_dummy_h

#include "WorldModelTypeBase.h"

class WorldModelDummy : public WorldModelType
{
public:
	WorldModelDummy ( ConfigReader& vr);
    ~WorldModelDummy () throw ();
    const RobotLocation& get_robot_location (Time, bool) throw () { return robot_loc; }
    const BallLocation& get_ball_location (Time, bool) throw () { return ball_loc; }
    const ObstacleLocation& get_obstacle_location (Time, bool) throw () { return obstacle_loc; }
    const FieldGeometry& get_field_geometry () const throw () { return field_geometry; }
    int get_own_half () const throw () { return 1; }
    const GameState& get_game_state () const throw () { return game_state; }
    const RobotLocation& get_slfilter_robot_location (Time&) const throw () { return robot_loc; }
    const RobotProperties& get_robot_properties () const throw () { return robot_properties; }
    const RobotData& get_robot_data (Time&) const throw () { return robot_data; }
    void set_own_half (int) throw () {;}
    void update_refbox (RefboxSignal) throw ();
    void startstop (bool) throw ();
    void set_robot_properties (const RobotProperties&) throw () {;}
    void set_robot_data (const RobotData&, Time) throw() {;}
    void set_drive_vector (DriveVector, Time) throw () {;}
    void set_odometry (DriveVector, Time) throw () {;}
    void set_compassdir (Angle, Time) throw () {;}
    void set_visual_information (const VisibleObject&, Time) throw () {;}
    void set_visual_information (const VisibleObjectList&) throw () {;}
    void update_localisation () throw () {;}
    void reset () throw () {;}
    void reset (const Vec) throw () {;}
    void reset (const Vec, const Angle) throw () {;}
    void update () throw () {;}
    std::ostream& log_stream () throw () { return null_stream; }
    void init_cycle (Time, Time) throw () {;}
    MessageBoard& get_message_board () throw () { return mboard; }
    const VisibleObjectList& get_visible_objects () throw () { return vol; }

	// edit by lrd for usb image processing
	void set_usb_work (bool h) throw () {;}
    bool get_usb_work () const throw () {return false;}

private:
	RobotLocation robot_loc;
    BallLocation ball_loc;
    ObstacleLocation obstacle_loc;
    FieldGeometry field_geometry;
    GameState game_state;
    RobotProperties robot_properties;
    RobotData robot_data;
    std::ofstream null_stream;
    MessageBoard mboard;
    VisibleObjectList vol;

};

#endif
