#ifndef _Goalie_h
#define _Goalie_h

#include "PlayerType.h"
#include "ConfigReader.h"
#include "PhysicalMotionModel.h"
#include "InterceptBall.h"
#include "SPhysGotoPos.h"
#include "GoalieTaiji.h"

class Goalie : public PlayerType
{
public:
	Goalie( ConfigReader&) throw();
	~Goalie() throw();
	DriveVector process_drive_vector (Time tt) throw ();
	
protected:
	PhysicalMotionModel motion;
    InterceptBall getBall;
    SPhysGotoPos goto_pos_skill;  
	
private:
	Vec targetParkPosition; 
    bool parkpositionfound;
	GoalieTaiji*  goalie_Taiji08;
};

#endif