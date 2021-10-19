#ifndef _STUCK_BEHAVIOR_H_
#define _STUCK_BEHAVIOR_H_

#include "CmdGenerator.h"
#include "InterceptBall.h"
#include "BasicCmd.h"
#include "Avoid.h"
#include "BallGet.h"
#include "SPhysGotoPos.h"
#include "MoveWithBall.h"
#include "RobotLocation.h"
#include "BallLocation.h"
#include "GotoPos.h"
#include "FaceBall.h"

class StuckBehavior : public CmdGenerator
{
public:
	StuckBehavior();
	~StuckBehavior();
	void getCmd(DriveVector *dv, const Time& t);
	
protected:
private:
	Time          update_time;
	double        velocity;
	Vec           desired_point;
	Angle         desired_dir;
	DriveVector   dest;
    unsigned int  point_type;
    unsigned int  approcach_type;

	BallGet*      get_ball;
	BasicCmd*     basic_cmd;
	InterceptBall* intercept;
	SPhysGotoPos*  go2pos;
	GotoPos*       go_pos;
	Avoid*         avoid;
	MoveWithBall*  move_with_ball;
	FaceBall*      face;
};
 
#endif