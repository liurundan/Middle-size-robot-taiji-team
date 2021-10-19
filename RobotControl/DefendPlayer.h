#ifndef _DEFEND_PLAYER_H_
#define _DEFEND_PLAYER_H_

#include "MoveWithBall.h"
#include "BallGet.h"
#include "InterceptBall.h"
#include "BasicCmd.h"
#include "Dribble.h"
#include "CmdGenerator.h"
#include "FaceBall.h"
#include "GotoPos.h"
#include "Offence.h"
#include "Defense.h"
#include "SPhysGotoPos.h"
#include "StuckBehavior.h"
#include "SearchBall.h"
#include "ApproachBall.h"
#include "PhysicalMotionModel.h"

class DefendPlayer : public CmdGenerator
{
public:
	DefendPlayer( ConfigReader& cfg);
	~DefendPlayer();
	void getCmd(DriveVector *dv, const Time& t);
	void decide_action( DriveVector* dv,const Time& t );
	void test_skills(DriveVector* dv,const Time& t);
protected:
private:
	MoveWithBall*  move_with_ball;
	BasicCmd*      basic_cmd;
	Dribble*       dribble;
	BallGet*       getBall;
	InterceptBall* intercept;
	FaceBall*      face_ball;
	GotoPos*       go_pos;
	Offence*       offence;
	DefenseBehavior* defense;
	Avoid*           avoid;
	SPhysGotoPos*   go2pos;
	StuckBehavior*  stuck;
	SearchBall*     searchball;
    ApproachBall*   approach;
	PhysicalMotionModel* motion;

};

#endif