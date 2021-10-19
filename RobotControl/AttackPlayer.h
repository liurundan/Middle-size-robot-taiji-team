#ifndef _ATTACK_PLAYER_H_
#define _ATTACK_PLAYER_H_

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
#include "ApproachBall.h"
#include "StuckBehavior.h"
#include "SearchBall.h"
#include "PhysicalMotionModel.h"

class AttackPlayer : public CmdGenerator
{
public:
	AttackPlayer( ConfigReader& cfg);
	~AttackPlayer();
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
	Offence*       offence;
	DefenseBehavior* defense;
	Avoid*           avoid;
	SPhysGotoPos*   go2pos;
	ApproachBall*   approach;
	StuckBehavior*  stuck;
	SearchBall*     searchball;
	PhysicalMotionModel* motion;
};
#endif
