#ifndef _BALL_GET_H
#define _BALL_GET_H

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "Avoid.h"
#include "PhysicalMotionModel.h"
#include "SPhysGotoPos.h"

class BallGet : public CmdGenerator 
{
public:
	BallGet();
	~BallGet();
    void getCmd(DriveVector *dv, const Time& t);

	void BallGet2007(const Time& t,double vel);
	void BallGetPura(const Time& t,double vel);
	void BallGetNew(const Time& t,double vel);
	void BallGetDF(const Time& t,double vel);
 
private:
	PhysicalMotionModel motion;
    SPhysGotoPos		goto_pos_skill;  

	double    velocity;
	BasicCmd* basic_cmd;
	Avoid*    avoid;
	int        osbstacle_found;
    static int consider_obstacles;
    
	DriveVector dest;
	
};

#endif