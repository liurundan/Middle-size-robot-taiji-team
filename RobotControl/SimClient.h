#ifndef _SimClient_h_
#define _SimClient_h_

#include "Time.h"
#include "Vec.h"
#include "DriveVector.h"
#include "odesimcomm.h"
#include <stdexcept>
#include <vector>

class SimClient 
{
public:
    static SimClient* get_sim_client (const char*) throw (std::bad_alloc, std::invalid_argument);
    static void delete_sim_client () throw ();
	
private:
    static SimClient* the_only_sim_client;
    
    OdeSimComm * odesimcomm;
    SimClient (const char*) throw (std::bad_alloc, std::invalid_argument);
    ~SimClient () throw ();
public:
	
    void set_drive_vector (DriveVector) throw ();
    void update () throw ();
	
    Time timestamp;        
    Vec robot_position;    
    Angle robot_heading;   
    Vec ball_position;     
    std::vector<Vec> obstacle_positions;  
	
};


#endif
