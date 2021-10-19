#ifndef _control_loop_h
#define _control_loop_h

 
#include "Robot.h"
#include "Player.h"
#include "WorldModel.h"
#include "ImageProcessing.h"
#include "UserInterface.h"

class CDistanceCalDlg; 
class ControlLoop
{
public:
    ControlLoop (  ConfigReader&, unsigned int ltime , CDC&  ) throw (RobotsException, std::bad_alloc);
    ~ControlLoop () throw ();
    void loop () throw ();

public:
    UserInterface*      the_user_interface;

protected:
	friend class CDistanceCalDlg;
   
private:
	Robot*				the_robot;
    Player*				the_player;
    WorldModel*			the_world_model;
    ImageProcessing*	the_image_processing;
	
    ConfigReader&       configuration_list;
   

    Time				timestamp;           
    long int			loop_time;       
    Time				first_cycle_time;     
	Time				time_reporter;
    
    bool report_computation_time;               
    bool report_computation_time_detailed;      
    bool report_computation_time_gnuplot;    
	
    unsigned long int usec_image_processing;    
    unsigned long int usec_world_model;         
    unsigned long int usec_player;              
    unsigned long int usec_robot;               
    unsigned long int usec_user_interface;      
    unsigned long int usec_idle;               
    unsigned long int num_cycles;  

	int drive_vector_delay;
	VisibleObjectList list_of_visible_objects;
};

#endif