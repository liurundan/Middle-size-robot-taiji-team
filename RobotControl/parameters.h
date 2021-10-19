#ifndef __HEADER_PARAMETERS
#define __HEADER_PARAMETERS

#include "geometry.h"
#include "Vec.h"

#define ROBOT_NUM_MAX	5		
 
#define SHOOT_OFFSET	0.4		

#define CHANGE_ROLE		1		
#define INTERVAL		10		

#define TOUCH_MODE		0		
#define TOUCH_OR_NOT	1	
#define TOUCH_AND_GO	1		

#define YOKERU			1		
#define DOKU			1	
#define PASS_GOOD_POSITION 0	

#define AVOID_NUKASANAI	0		
#define KACHIKACHI		1		

#define CORNER_KICK		0		

#define OVER_TIME_PASS	0		

#define AVOID_DEBUG		0	

#define OBJ_NUM_MAX		12		
#define MAX_OBSTACLES	22	

#define	FIELD_LENGTH	18000
#define	FIELD_WIDTH		12000
#define RADIUS          0.035
#define wl		        0.2		
 
#define	PI				3.14159265359
#define root3			1.732050807569
#define DegreeToRad		M_PI/180		 
#define RadToDegree		180/M_PI		 

#ifndef SQUARE
#define SQUARE(x)((x)*(x))
#endif

const double PI_MINUS_EPS = M_PI-0.001;

const int NUM_PLAYERS = 5;

  
const int NARROW= 0;
const int NORMAL= 1;
const int WIDE=   2;

const int LOW= 0;
const int HIGH= 1;
  
const double FIELD_BORDER_X = 9000.0;
const double FIELD_BORDER_Y = 6000.0;
const double PENALTY_AREA_LENGTH =  2250.0;
const double PENALTY_AREA_WIDTH= 6500.0;


const XYRectangle FIELD_AREA( Vec(0,0), FIELD_BORDER_X * 2.0, FIELD_BORDER_Y * 2.0);
const XYRectangle LEFT_PENALTY_AREA( Vec(-FIELD_BORDER_X, -PENALTY_AREA_WIDTH * 0.5), 
				       Vec(-FIELD_BORDER_X + PENALTY_AREA_LENGTH, PENALTY_AREA_WIDTH*0.5));
const XYRectangle RIGHT_PENALTY_AREA( Vec(FIELD_BORDER_X - PENALTY_AREA_LENGTH, -PENALTY_AREA_WIDTH*0.5),
					Vec(FIELD_BORDER_X, PENALTY_AREA_WIDTH * 0.5)); 
					
const XYRectangle LEFT_HALF_AREA( Vec(-FIELD_BORDER_X,-FIELD_BORDER_Y),
				    Vec(0,FIELD_BORDER_Y));

const XYRectangle RIGHT_HALF_AREA( Vec(0,-FIELD_BORDER_Y),
				    Vec(FIELD_BORDER_X,FIELD_BORDER_Y));
				    

const Vec MY_GOAL_LEFT_CORNER= Vec(-FIELD_BORDER_X,1000.0);
const Vec MY_GOAL_CENTER= Vec(-FIELD_BORDER_X,0.0);
const Vec MY_GOAL_RIGHT_CORNER= Vec(-FIELD_BORDER_X,-1000.0);
const Vec HIS_GOAL_LEFT_CORNER= Vec(FIELD_BORDER_X,1000.0);
const Vec HIS_GOAL_CENTER= Vec(FIELD_BORDER_X,0.0);
const Vec HIS_GOAL_RIGHT_CORNER= Vec(FIELD_BORDER_X,-1000.0);
#define RAD2DEG(x)((int)(x/PI*180.))
#define DEG2RAD(x)((double)(x/180.*PI))

 
#define DPASS 4000
 
#define DPASS_DIAG DPASS / 1.414

const Vec Pass_Direct = Vec(0,0);
const Vec Pass_Forward = Vec(DPASS,0);
const Vec Pass_Backward = Vec(-DPASS,0);
const Vec Pass_Left = Vec(0,DPASS);
const Vec Pass_Right = Vec(0,-DPASS);
const Vec Pass_LeftForward = Vec(DPASS_DIAG,DPASS_DIAG);
const Vec Pass_RightForward = Vec(DPASS_DIAG,-DPASS_DIAG);
const Vec Pass_LeftBackward = Vec(-DPASS_DIAG,DPASS_DIAG);
const Vec Pass_RightBackward = Vec(-DPASS_DIAG,-DPASS_DIAG);
const Vec Pass_LongForward = Vec(2*DPASS,0);

const Vec Stay = Vec(0,0);
const Vec Forward = Vec(DPASS,0);
const Vec Backward = Vec(-DPASS,0);
const Vec Left = Vec(0,DPASS);
const Vec Right = Vec(0,-DPASS);
const Vec LeftForward = Vec(DPASS_DIAG,DPASS_DIAG);
const Vec RightForward = Vec(DPASS_DIAG,-DPASS_DIAG);
const Vec LeftBackward = Vec(-DPASS_DIAG,DPASS_DIAG);
const Vec RightBackward = Vec(-DPASS_DIAG,-DPASS_DIAG);

#define DDRIBBLE 3000
#define DDRIBBLE_DIAG DDRIBBLE / 1.414

const Vec Dribble_Forward = Vec(DDRIBBLE,0);
const Vec Dribble_Backward = Vec(-DDRIBBLE,0);
const Vec Dribble_Left = Vec(0,DDRIBBLE);
const Vec Dribble_Right = Vec(0,-DDRIBBLE);
const Vec Dribble_LeftForward = Vec(DDRIBBLE_DIAG,DDRIBBLE_DIAG);
const Vec Dribble_RightForward = Vec(DDRIBBLE_DIAG,-DDRIBBLE_DIAG);
const Vec Dribble_LeftFForward = Vec(DDRIBBLE,DDRIBBLE_DIAG);
const Vec Dribble_RightFForward = Vec(DDRIBBLE,-DDRIBBLE_DIAG);
const Vec Dribble_LeftBackward = Vec(-DDRIBBLE_DIAG,DDRIBBLE_DIAG);
const Vec Dribble_RightBackward = Vec(-DDRIBBLE_DIAG,-DDRIBBLE_DIAG);

#endif

