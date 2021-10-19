#ifndef _RefereeStateMachine_h
#define _RefereeStateMachine_h

#include "GameState.h"
#include "FieldGeometry.h"
#include "Time.h"
#include "Vec.h"

class RefereeStateMachine
{
public:
	 
    RefereeStateMachine (const FieldGeometry&, int =1, RefereeState =stopRobot) throw ();
    
    RefereeStateMachine (const RefereeStateMachine&) throw ();
    
    const RefereeStateMachine& operator= (const RefereeStateMachine&) throw ();

    
    RefereeState get_state () const throw ();

    void set_state (RefereeState) throw ();
    
    void set_team_color (int) throw ();

   
    RefboxSignal update (RefboxSignal, Vec, bool) throw ();
    
    void update (Vec, Vec, bool) throw ();
 
private:
	int team;                 
    RefereeState current;  
	//RefboxSignal latest_signal;

    Time latest_update;       
    Vec ballpos;              
    bool ballknown;           
    double penalty_marker_y; 
    double center_circle_radius;   
};

#endif