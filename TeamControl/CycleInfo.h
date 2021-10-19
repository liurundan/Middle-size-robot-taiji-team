#ifndef _CycleInfo_h
#define _CycleInfo_h

#include <string>
#include <vector>
#include "RobotLocation.h"
#include "BallLocation.h"
#include "ObstacleLocation.h"
#include "VisibleObject.h"
#include "GameState.h"

struct PossiblePosition 
{
    Vec pos;         
    Angle heading;   
    double value;             
};


struct CycleInfo 
{
    unsigned long int cycle_num;                
    unsigned long int time_msec;               
	
    std::vector<RobotLocation> rloc_vis;   
    std::vector<BallLocation> bloc_vis;    
    std::vector<RobotLocation> rloc_exec;  
    std::vector<BallLocation> bloc_exec;  
    ObstacleLocation oloc;            
    VisibleObjectList vloc;           
    GameState gs;                      
    std::string logmsg;                         
    std::string paintmsg;                       
    std::vector<PossiblePosition> ppos;         
    
    CycleInfo ();
    CycleInfo (const CycleInfo&);
    const CycleInfo& operator= (const CycleInfo&);
};

#endif
