#ifndef _BASIC_CMD_H
#define _BASIC_CMD_H

#include "CmdGenerator.h"

class BasicCmd : public CmdGenerator
{
public:
	BasicCmd();
    void set_kick(BYTE power);
	void set_dash(double velocity,double direction,double omega);
	void set_turn(double moment);
	void set_rotate(int way);
	void set_move(double velocity,double angle,double omega);
	void set_turnMove(double velocity,double angle,double omega); // use wheel vel
	void set_uturn(); // use wheel
    void set_stop();
	void getCmd(DriveVector *dv, const Time&);

private:
	enum bm_types 
	{
		KICK,DASH,TURN,MOVE,TURN_MOVE,U_TURN,STOP,ROTATE
	};
	int    type;
	long   type_set;
	double power,moment;
	double direction;
	double vx,vy,w;
	Vec    vtrans;
	double vrot;
	double wheelSpeed[4];
	bool   bkick;
	BYTE   powerkick;
};

#endif