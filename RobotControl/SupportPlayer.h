#ifndef _SUPPORT_PLAYER_H_
#define _SUPPORT_PLAYER_H_

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

class SupportPlayer : public CmdGenerator
{
public:
	SupportPlayer( ConfigReader& cfg);
	~SupportPlayer();
	void getCmd(DriveVector *dv, const Time& t);
	void decide_action( DriveVector* dv,const Time& t );
	void test_skills(DriveVector* dv,const Time& t);
private:
	MoveWithBall*  move_with_ball;
	BasicCmd*      basic_cmd;
	Dribble*       dribble;
	BallGet*       getBall;
	InterceptBall* intercept;
	FaceBall*      face_ball;
	GotoPos*       go_pos;
	SPhysGotoPos*  go2pos;
	Offence*       offence;
	DefenseBehavior* defense;
	Avoid*           avoid;
	StuckBehavior*  stuck;
	SearchBall*     searchball;
    ApproachBall*   approach;
	PhysicalMotionModel* motion;
};

#endif