#ifndef __HEADER_BASICFUNCTION
#define __HEADER_BASICFUNCTION

#include "parameters.h"

#define DRIBBLE_DISTANCE		0.35 
#define DRIBBLE_ANGLE			15.0*DegreeToRad 
#define KICK_DISTANCE			0.35 

#define KEEPBALL_ANGLE 2		0.0*DegreeToRad 
#define KEEPBALL_DISTANCE		0.8 

#define SUPPORT_BALL_DISTANCE	1.5 
#define MYGOAL_MOVE_DISTANCE	3.0 

#define FW_HOMEPOSITION			5.5 
#define MF_HOMEPOSITION			7.5 
#define DF_HOMEPOSITION			3.0 

#define FW_SUPPORT_BACK_LINE	5.0 
#define MF_SUPPORT_BACK_LINE	4.0 

#define DEFENSE_BALLGET_TYPE 1  // 1:OppGoal&mygoal 0:mygoal
 
// BasicFunctions.cpp
hyper	myGetTickCount(void);
int		KitchenTimer(double second);
double  Limit2PI(double );
double  TwoAngleDifference(double , double);
double  TwoAngleBetweenDirection(double , double);
double  Angle2Positive(double);
 

#endif
