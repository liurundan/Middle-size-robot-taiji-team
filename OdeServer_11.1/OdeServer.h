#ifndef _ODE_SERVER_H_
#define _ODE_SERVER_H_

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#include <GL/glut.h>
 
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
 
#include "structures.h"
#include "v2d.h"
#include "frame2d.h"
#include "odesimcomm.h"

#if _MSC_VER
#pragma warning(disable:4244 4305)        
#pragma warning(disable:4101 4305 4700)  // for VC++, no precision loss complaints
#endif

#ifdef  dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif
 
// some constants

#define BALL_RADIUS		0.11	// ball radius
#define LENGTH			0.36	// box length & width
#define HEIGHT			0.4	    // box height
#define MASS_R			10		// mass of box[i][j] = (i+1) * MASS
#define FORCE			0.1	    // force applied to box[i][j] = (j+1) * FORCE
#define MU				2.0		// the global mu to use 0.1
#define GRAVITY			9.8	    // the global gravity to use 0.9
#define Nrobot			16		// number of robots
#define MAX_CONTACTS	3		// maximum number of contact points per body
#define FIELD_LENGTH	18		// maximum number of contact points per body
#define FIELD_WIDTH		12		// maximum number of contact points per body

#define POLERAD			0.1
#define DegreeToRad		M_PI/180		  
#define RadToDegree		180/M_PI		  

#define LENGTH_HM		0.45
#define WIDTH_HM		0.05
#define HEIGHT_HM		0.1
#define RADIUS			0.035
#define BALLRAD			0.11
#define CORE_RAD		0.32
#define Z				0.18

#define RADIUS          0.035
#define wl		        0.2		// length to wheel

#define SOUND_SHOOT		"sound/shoot.wav"
#define SOUND_BALL		"sound/ball.wav"
#define SOUND_CHARGE	"sound/charge.wav"
#define SOUND_BOOST		"sound/boost.wav"

extern LPCSTR sndShoot;

// dynamics and collision objects
    
extern  dWorldID world;
extern  dSpaceID space;
extern  dBodyID  body[Nrobot];
extern  dBodyID  sphere;
extern  dBodyID  bodygoal;
extern  dBodyID  bodywall[4];

extern  dJointGroupID contactgroup;
extern  dGeomID ground;
extern  dGeomID box[Nrobot];
extern  dGeomID immovablebox[6];
extern  dGeomID ball;
extern  dGeomID geomgoal;
extern  dGeomID geomwall;

extern  double savedBallPos[3];
extern  dReal savedBallVel[3];
extern  dReal savedBallAngVel[3];
extern  int			goaldelay;

extern int simloopCount;
extern int viewmode;
extern int viewchange;

extern float* camsmooth;
extern float* camlooksmooth;

extern bool in_field;
extern int  activerobot;
extern bool move_ball;

extern double shootspeed;
extern RobotTri robbies[Nrobot];

//lrd
static const dReal GOAL_SIDES[7][3] = {{0.635, 0.125, 1.000}, 
	     {0.635, 0.125, 1.000}, {0.125, 2.010, 1.00}, {0.125, 2.270, 0.125},
	     {0.500, 0.010, 1.000}, {0.500, 0.010, 1.00}, {0.010, 2.010, 1.000}};
const double root3 = sqrt(3.0);

extern  dGeomID  goal_parts[7];
extern  dGeomID  goal_parts2[7];
extern  dGeomID	 wall[8];
extern  dGeomID	 pole[12];
extern  double   cam_angle, cam_zoom;
extern  int      map, boost;
static  int      step = 0;
extern  int		 now_boosting, simulation;
static  int      stop = 0;
static  float    sim_step = 0.02;
static  int      first_step = 0;
static  int      tmp_step = 0, Pause = 0;

const int TURN = 1;	     
static double LTURN = TURN;   
static double RTURN =-TURN;  
extern int POWER; 

static int Robot_Formation_X[16] = {-1, -FIELD_LENGTH/2+0.5, -2,-2, 1, 2, 2,FIELD_LENGTH/2-0.5 , -1, -2, -3,-4, 1,2,3,4 };
static int Robot_Formation_Y[16] = { 0, 0, -2,  2,  0, -2,  2, 0,  FIELD_WIDTH/2+1, FIELD_WIDTH/2+1,  FIELD_WIDTH/2+1,FIELD_WIDTH/2+1, FIELD_WIDTH/2+1,FIELD_WIDTH/2+1,FIELD_WIDTH/2+1,FIELD_WIDTH/2+1};
//static int Enemy_Formation_X[11] = { 3,  8,  4,  4,  5,  5,  5,  6,  6,  6,  6};
//static int Enemy_Formation_Y[11] = { 0,  0,  1, -1,  0,  2, -2,  1, -1,  3, -3};

static dReal ROBOT_BASE1[3] = {WIDTH_HM,		   LENGTH_HM, HEIGHT_HM};  
static dReal ROBOT_BASE2[3] = {WIDTH_HM, WIDTH_HM*root3+0.1, HEIGHT_HM};   
static dReal ROBOT_BASE3[3] = {WIDTH_HM,     WIDTH_HM*root3, HEIGHT_HM};   
static dReal ROBOT_BASE4[3] = {0.090,			WIDTH_HM, HEIGHT_HM};   
static dReal ROBOT_FRAME[3] = {WIDTH_HM,	  WIDTH_HM*root3, LENGTH_HM};  
static dReal ROBOT_CORE[3]  = {0.260, 0.260, 0.30};				  
static dReal ROBOT_GOALIE[3]= {0.010, 0.700, 0.60};

static dReal  wheelSpeed[3];
static double velocity = 4.5, low_velocity = 2.5;

struct ROBOT
{
	dBodyID		body_base[8];
	dBodyID		body_wheel[5];
	dBodyID		body_finger[4];
	dBodyID		body_frame[5];
	dBodyID		body_core[2];
	dBodyID		body_head[6];

    dGeomID		geom_base[8];
	dGeomID		geom_wheel[5];
	dGeomID		geom_finger[4];
	dGeomID		geom_frame[5];
	dGeomID		geom_core[2];
	dGeomID		geom_head[6];

    dJointID	joint_base[8];
	dJointID	joint_wheel[5];
	dJointID	joint_finger[4];
	dJointID	joint_frame[5];
	dJointID	joint_core[2];
	dJointID	joint_head[6];
};
extern ROBOT Robot[Nrobot];

void drawDot(double x, double y, float dotsize, float color[3]);
void drawBar(double length);
void drawMap(void);

void drawCircle(dReal r,dReal center[2], int angle1, int angle2);
void drawPole(void);
void drawGoal(void);
void drawLine(void);
void drawWall(void);
void drawField(void);
void drawBall(void);

void makeWall(void);
void makePole(void);
void makeGoal(void); 
void makeBall(void);
void makeField(void);

double Limit2PI(double ang);

void makeRobot(int);
void drawRobot(int);
//void makeEnemy(int);
//void drawEnemy(int);
void drawPlayer(void);

 
void destroyRobot(int);
//void destroyEnemy(int);
hyper myGetTickCount(void);

#endif
