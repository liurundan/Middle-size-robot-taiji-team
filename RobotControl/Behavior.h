#ifndef __HEADER_BEHAVIOR
#define __HEADER_BEHAVIOR

#include "parameters.h"
#include "InPlayParameters.h"
#include "basicfunctions.h"
#include "position.h"
#include "CmdGenerator.h"
#include "Avoid.h"
#include "BasicCmd.h"
#include "BallGet.h"
#include "GotoPos.h"
#include "AttackPlayer.h"
#include "SupportPlayer.h"
#include "DefendPlayer.h"
#include "GameState.h"
#include "SPhysGotoPos.h"
#include "Touch.h"
#include "FaceBall.h"
#include "StuckBehavior.h"
#include "SearchBall.h"
#include "MoveWithBall.h"

#define FIELD_HALF_LENGTH	FIELD_LENGTH/2
#define FIELD_HALF_WIDTH	FIELD_WIDTH/2


const int TimeTenSec = 10000;
const int TimeFiveSec = 5000;
const int TimeOneSec = 1000;
const int WaitKickTime5Sec = 5000;  

namespace LiveRobot
{
	const int Zero = 0;
	const int One = 1;
	const int Two = 2;
	const int Three = 3;
	const int Four = 4;
	const int Five = 5;
};

namespace BallDistance
{
	const double Near = 300;
	const double Middle = 2000;
	const double Faraway = 4000;
};

namespace Position
{
	const int Alpha = 1;
	const int Beta = 2;
	const int Gamma = 3;
	const int Delta = 4;
	const int Epshilon = 5;
	const int GK = 6;
	const int INIT = -1;
}

const int ReceiveDataStart = 1;

class Behavior : public CmdGenerator
{
public:
    Behavior( ConfigReader& cfg) throw();
    ~Behavior() throw();
    void getCmd(DriveVector*, const Time& t) throw();
	
	int		CheckBallMove();
	void	NoiseCancel();
	bool	DoYouHaveBall();
	
	void	GoalieMove(double m_per_sec, double rad, double rad_per_sec);
	void    StopGoalieWheel();
	
	//Stay.cpp
	void	KeepBall(double );
	
	void	InPlay(DriveVector* dv, const Time& t);
	void	FW1P();
	void	MF1P();
	void	MF2P();
	void	DF1P();
	void	DF2P();
	void	DF3P();
	
	void	InPlay1P();
	void	InPlay2P();
	void	InPlay3P();
	void	InPlay4P();
	void	InPlay5P();
	void	InPlay6P();
	void	InPlay0P();
	
	//Kick_Off
	int		RunMyKickOff(DriveVector* dv, const Time& t);
	int		RunOppKickOff(DriveVector* dv, const Time& t);
	int		PositionMyKickOff(DriveVector* dv, const Time& t);
	int		PositionOppKickOff(DriveVector* dv, const Time& t);
	int		Move2KickOffPassPosition(DriveVector* dv, const Time& t);
	
	//Free_Kick
	int		RunMyFreeKick(DriveVector* dv, const Time& t);
	int		RunOppFreeKick(DriveVector* dv, const Time& t);
	int		PositionMyFreeKick(DriveVector* dv, const Time& t);
	int		PositionOppFreeKick(DriveVector* dv, const Time& t);
	
	//Goal_Kick
	int		RunMyGoalKick(DriveVector* dv, const Time& t);
	int		RunOppGoalKick(DriveVector* dv, const Time& t);
	int		PositionMyGoalKick(DriveVector* dv, const Time& t);
	int		PositionOppGoalKick(DriveVector* dv, const Time& t);
	
	//Corner_Kick
	int		RunMyCornerKick(DriveVector* dv, const Time& t);
	int		RunOppCornerKick(DriveVector* dv, const Time& t);
	int		PositionMyCornerKick(DriveVector* dv, const Time& t);
	int		PositionOppCornerKick(DriveVector* dv, const Time& t);
	int		Move2CornerKickPosition(DriveVector* dv, const Time& t);
	int		Move2CornerPassPosition(DriveVector* dv, const Time& t);
	
	//Throw_in
	int		RunMyThrowIn(DriveVector* dv, const Time& t);
	int		RunOppThrowIn(DriveVector* dv, const Time& t);
	int		PositionMyThrowIn(DriveVector* dv, const Time& t);
	int		PositionOppThrowIn(DriveVector* dv, const Time& t);
	int		Move2BehindBallThrow(DriveVector* dv, const Time& t);
	//PK
	int		RunMyPK(DriveVector* dv, const Time& t);
	int		RunOppPK(DriveVector* dv, const Time& t);
	int		PositionMyPK(DriveVector* dv, const Time& t);
	int		PositionOppPK(DriveVector* dv, const Time& t);
	//DropBall
	int		DropBall(DriveVector* dv, const Time& t);
	//Goal!!!!
	int		MyGoal();
	int		OppGoal();
	
	int		Set_Role_Robot();
	int		SetRoleRobot();
	
public:
	void		Main(DriveVector* dv, const Time& t);
    void		GoalieMain();
	
public:
	int		touchFlag;
	int		moveBallFlag;
	int		mukiFlag;
	int		setPlayFlag;
    double  wheelSpeed[4],goalie_v[4];

private:
	BasicCmd*		basic_cmd;
	BallGet*		getBall;
    GotoPos*		go_pos;
	Avoid*			avoid;
 	TouchBehavior*  touch;
    AttackPlayer*	ivpAttackPlayerBehavior;
	SupportPlayer*	ivpSupportPlayerBehavior;
	DefendPlayer*	ivpDefendPlayerBehavior;
    SPhysGotoPos*   go2pos;
    FaceBall*		face;
	StuckBehavior*  stuck;
	SearchBall*     searchball;
	MoveWithBall*   move_with_ball;
	
	DriveVector		dest;
	RefereeState    current;
	Time			latest_update;

};

#endif
