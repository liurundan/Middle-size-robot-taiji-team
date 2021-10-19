#ifndef _world_model_type_base_h
#define _world_model_type_base_h

#include "WorldModelType.h"
#include "ConfigReader.h"
#include "GameStateManager.h"
#include <fstream>

class LocationShortTimeMemory;
class WorldModelTypeBase : public WorldModelType
{
public:
	 WorldModelTypeBase (  ConfigReader&) throw (InvalidConfigurationException, std::bad_alloc);
    ~WorldModelTypeBase () throw ();
  
    const FieldGeometry& get_field_geometry () const throw ();
    int get_own_half () const throw ();
    const GameState& get_game_state () const throw ();
    const RobotProperties& get_robot_properties () const throw ();
    const RobotData& get_robot_data (Time&) const throw ();
    const RobotLocation& get_slfilter_robot_location (Time&) const throw ();

    virtual RobotLocation get_robot (Time) const throw () =0;
    virtual BallLocation get_ball (Time) const throw () =0;
    virtual ObstacleLocation get_obstacles (Time) const throw () =0;

    const RobotLocation& get_robot_location (Time, bool) throw ();
    const BallLocation& get_ball_location (Time, bool) throw ();
    const ObstacleLocation& get_obstacle_location (Time, bool) throw ();
    virtual Time get_timestamp_latest_update () const throw () =0;   

    void set_own_half (int) throw ();
    void update_refbox (RefboxSignal) throw ();
    void startstop (bool) throw ();
    void set_robot_properties (const RobotProperties&) throw ();
    void set_robot_data (const RobotData&, Time) throw();

    void reset () throw () =0;
    void reset (const Vec) throw () =0;
    void reset (const Vec, const Angle) throw ();

    void update () throw ();

    std::ostream& log_stream () throw ();
 
    void init_cycle (Time, Time) throw ();
    MessageBoard& get_message_board () throw ();

    const VisibleObjectList& get_visible_objects () throw () { return empty_vol; }

	// edit by lrd use to detect usb image processing
	void set_usb_work (bool h) throw ();
    bool get_usb_work () const throw ();

protected:
	FieldGeometry    field_geometry;   
    GameStateManager gsman;            
    MessageBoard     mboard;           
    int              own_half;          
    RobotProperties  robot_properties;  
    RobotData        robot_data;        
    Time             robot_data_time;  
    VisibleObjectList   empty_vol;

    LocationShortTimeMemory* locations; 
    std::ofstream    null_stream;      

    virtual void update_game_state () throw ();        
    virtual void update_localisation () throw () = 0; 
	
	bool  bUsbWork;
private:
	RobotLocation no_good_rloc;   

};


#endif