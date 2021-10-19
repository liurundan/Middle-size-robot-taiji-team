#ifndef _ErrorMinimiserSL_h
#define _ErrorMinimiserSL_h

#include "FieldLUT.h"
#include "OdometryContainer.h"
#include "CompassContainer.h"
#include "VisualPositionOptimiser.h"
#include "RobotPositionKalmanFilter.h"
#include "VisibleObject.h"
#include "RobotLocation.h"
#include "ConfigReader.h"
#include <fstream>

class ErrorMinimiserSL
{
public:

    ErrorMinimiserSL (  ConfigReader&, const OdometryContainer&, const CompassContainer&, const FieldGeometry&) throw (std::bad_alloc);
    ~ErrorMinimiserSL () throw ();
	
	bool update (const VisibleObjectList&, const VisibleObjectList&) throw ();
    RobotLocation get (Time) const throw ();
	
    void reset () throw ();
    void reset (Vec) throw ();
    void reset (Vec, Angle) throw ();

private:
    struct AltPos 
	{
		AltPos ();                               
		AltPos (Vec);                           
		AltPos (Vec, Angle);                    
		RobotPositionKalmanFilter rpos_filter;  
		Time   init_time;                         
		double winning_coefficient;             
		double latest_error;                    
		bool   heading_checked;                   
		double ema_right_goal;                   
		double ema_wrong_goal;                  
    };
	
    FieldLUT* field_lut;                      
    VisualPositionOptimiser* vis_optimiser;    
    const OdometryContainer& odobox;   
	const CompassContainer& combox;
    std::ofstream* plog;                       
    Time latest_update;                        
    unsigned int cycles_since_reset;          
	
    unsigned int max_lines;                  
	
    bool consider_yellow_goal;                
    bool consider_blue_goal;                   
    bool use_internal_alternatives;          
	
    AltPos main_position;                      
    unsigned int num_external_alternatives;    
    std::vector<AltPos> external_alternatives; 
	
    void update_alternative (AltPos&, const VisibleObjectList&, const VisibleObjectList&, bool, bool);   
 
	void choose_main_pos(const VisibleObjectList& lines, const VisibleObjectList& goals, AltPos& main_pos );
	int    number_particles;
	double min_match_error;
	bool   use_global_localization;
	bool   consider_compass;
};

#endif
