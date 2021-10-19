#ifndef _TOUCH_H_
#define _TOUCH_H_

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "MoveWithBall.h"

class TouchBehavior : public CmdGenerator 
{
public:
	TouchBehavior();
	~TouchBehavior();
    void getCmd(DriveVector *dv, const Time& t);
	int  normal_touch(const Time& t);
	int  MkTouch(const Time& t);
    int	 TouchForCornerKick(const Time& t);
	int	 YokoTouch(const Time& t);
    void setTouchFlag( int tf = 4 );
    int  getTouchFlag() { return touchFlag;   }
	
private:
	BasicCmd*     basic_cmd;
	MoveWithBall* move_with_ball;
	Avoid*        avoid;
    int		      touchFlag;
	DriveVector   dest;
};
#endif