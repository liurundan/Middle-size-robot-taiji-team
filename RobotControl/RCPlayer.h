#ifndef _RC_player_h
#define _RC_player_h

#include "PlayerType.h"
#include "ConfigReader.h"
#include "PhysicalMotionModel.h"
#include "SPhysGotoPos.h"
#include "InterceptBall.h"
#include "Behavior.h"
 
class RCPlayer : public PlayerType
{
public:
	RCPlayer( ConfigReader& cfg) throw();
	~RCPlayer() throw();
	DriveVector process_drive_vector (Time tt) throw ();
    Vec getTargetpos(const Vec& v) throw();
    int  DoYouHaveBall();
	
protected:
    PhysicalMotionModel motion;
    SPhysGotoPos        goto_pos_skill;   
	InterceptBall       getBall;
	
    Behavior*           gdtj_08;
  	
private:
    Vec  targetParkPosition; 
    bool parkpositionfound;
   
};

#endif