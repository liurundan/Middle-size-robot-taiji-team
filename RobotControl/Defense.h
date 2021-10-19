#ifndef _DEFENSE_H
#define _DEFENSE_H

#include "CmdGenerator.h"
#include "GotoPos.h"
#include "BasicCmd.h"
#include "Avoid.h"
#include "MoveWithBall.h"

class DefenseBehavior : public CmdGenerator
{
public:
	DefenseBehavior();
	~DefenseBehavior();
    void getCmd(DriveVector *dv, const Time& t);
	void Defense(const Time& t,double distance2MyGoal);
	int  SetPlayDefense(const Time& t,double distance2MyGoal);
	void SupportCenter(const Time& t,double supportDistance);
  
private:
	GotoPos*  go_pos;
	BasicCmd* basic_cmd;
	Avoid*    avoid;
	MoveWithBall*  move_with_ball;
	DriveVector dest;
	
};
#endif