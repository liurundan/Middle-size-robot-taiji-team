#ifndef _DRIBBLE_H_
#define _DRIBBLE_H_

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "Avoid.h"
#include "ApproachBall.h"

class Dribble : public CmdGenerator
{
public:
	Dribble();
	~Dribble();
	void getCmd(DriveVector *dv, const Time& t);
	void dribble_straight(const Time& t,double Velocity);
	int  dribble_turn(const Time& t,double Velocity);
	int  dribble_turn(const Time& t,double Velocity,double angle, double ROme);

 
private:
	BasicCmd* basic_cmd;
	Avoid*    avoid;
	ApproachBall* approach;

	int        osbstacle_found;
    static int consider_obstacles;
    DriveVector dest;
	
};
#endif