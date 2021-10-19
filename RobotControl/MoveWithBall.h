#ifndef _MOVEWITHBALL_H_
#define _MOVEWITHBALL_H_

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "GotoPos.h"

class MoveWithBall : public CmdGenerator
{
public:
	MoveWithBall();
	~MoveWithBall();

	void getCmd(DriveVector *dv, const Time& t);
	int Move2PassPosition(const Time& t);
	int Move2BallRahen1st(const Time& t);
	/* not fine */
	int Move2BallRahen(const Time& t);  
	int Move2BallRahen2OppCombi(const Time& t,double combiAngle);
	int Move2BehingdBall(const Time& t,double stopDistance);
	int Move2BetweenBall(const Time& t,double stopDistanceFromBall);
    void  Mawaricom(const Time& t);
	void Move2HomePosition( const Time& t);

	void Move2InPlayHomePosition( const Time& t);
	
    double Angle180To360(double kakudo);
    double GetGoodVelocity();
	
private:
	BasicCmd* basic_cmd;
	GotoPos*  go_pos;
	Avoid*    avoid;
	DriveVector dest;
};
 

#endif