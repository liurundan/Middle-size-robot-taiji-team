#ifndef _AddWriteWorldModel_h_
#define _AddWriteWorldModel_h_
 
#include "WorldModelType.h"
#include "VisibleObjectReadWriter.h"
#include "DriveVectorReadWriter.h"
#include "GameStateReadWriter.h"
#include "RobotLocationReadWriter.h"
#include "BallLocationReadWriter.h"
#include "ObstacleLocationReadWriter.h"
#include <iostream>

class AddWriteWorldModel: public WorldModelType
{
public:
    AddWriteWorldModel ( ConfigReader&, WorldModelType*) throw ();
    ~AddWriteWorldModel () throw ();

    void set_drive_vector (DriveVector, Time) throw ();
    void set_odometry (DriveVector, Time) throw ();
    void set_compassdir (Angle, Time) throw ();
    void set_visual_information (const VisibleObjectList&) throw ();
    void set_visual_information (const VisibleObject&, Time) throw ();
    void update () throw ();
    std::ostream& log_stream () throw ();   
    void init_cycle (Time, Time) throw ();


    const RobotLocation& get_robot_location (Time, bool) throw ();
    const BallLocation& get_ball_location (Time, bool) throw ();
    const ObstacleLocation& get_obstacle_location (Time, bool) throw ();
    void reset () throw ();
    void reset (const Vec) throw ();
    void reset (const Vec, const Angle) throw ();
    const FieldGeometry& get_field_geometry () const throw () { return the_world_model->get_field_geometry (); }
    int get_own_half () const throw () { return the_world_model->get_own_half (); }
    const GameState& get_game_state () const throw () { return the_world_model->get_game_state (); }
    const RobotProperties& get_robot_properties () const throw () { return the_world_model->get_robot_properties (); }
    const RobotData& get_robot_data (Time& t) const throw () { return the_world_model->get_robot_data (t); }
    void set_own_half (int d) throw () { the_world_model->set_own_half (d); }
    void update_refbox (RefboxSignal sig) throw () { the_world_model->update_refbox (sig); }
    void startstop (bool b) throw () { the_world_model->startstop (b); }
    void set_robot_properties (const RobotProperties& rp) throw () { the_world_model->set_robot_properties (rp); }
    void set_robot_data (const RobotData& rd, Time t) throw();
    const RobotLocation& get_slfilter_robot_location (Time&) const throw ();
    MessageBoard& get_message_board () throw ();
    const VisibleObjectList& get_visible_objects () throw () { return the_world_model->get_visible_objects(); }

	void set_usb_work(bool h) throw() { the_world_model->set_usb_work(h); }
	bool get_usb_work() const throw() { return the_world_model->get_usb_work();	}
private:
	WorldModelType* the_world_model;   
    std::ostream* odometry_out;         
    std::ostream* drive_vector_out;
    std::ostream* visual_info_out;
    std::ostream* robot_pos_out;
    std::ostream* ball_pos_out;
    std::ostream* obs_pos_out;
    std::ostream* log_out;
    std::ostream* gs_out;
    std::ostream* rawrobotdata_out;

    bool first_visual;                 
    Time first_visual_timestamp;        
    Time image_timestamp;               

    VisibleObjectWriter* visual_writer; 
    DriveVectorWriter* drv_writer;
    DriveVectorWriter* odo_writer;
    GameStateWriter* gs_writer;
    RobotLocationWriter* robot_writer;
    BallLocationWriter* ball_writer;
    ObstacleLocationWriter* obstacle_writer;
};

#endif