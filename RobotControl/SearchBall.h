#ifndef _SEARCH_BALL_H_
#define _SEARCH_BALL_H_

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

class SearchBall : public CmdGenerator
{
public:
	SearchBall();
	~SearchBall();
	void getCmd(DriveVector *dv, const Time& t);
	bool is_searching(const Time& t);
	void start_search(const Time& t);
	
protected:
private:
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
	Time           update_time;
};
#endif