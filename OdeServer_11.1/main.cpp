#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <GL/glut.h>
 
#include "OdeServer.h"

struct BodyState
{
	dReal pos[3];
	dReal vel[3];
	dQuaternion q;
	
};

struct OdeSimState
{
	BodyState robbies[Nrobot];
	BodyState ball;
};
 
 dWorldID world;
 dSpaceID space;
 dBodyID  body[Nrobot];
 dBodyID sphere;
//static dBodyID bodygoal;
//static dBodyID bodywall[4];
//static dGeomID immovablebox[6];

 dJointGroupID contactgroup;
 dGeomID ground;
 dGeomID box[Nrobot];
 dGeomID ball;

OdeSimState   savesimstate[3];
static double savedBallPos[3];
static dReal  savedBallVel[3];
static dReal  savedBallAngVel[3];
int			  goaldelay;

int simloopCount = 0;
int viewmode = 0;
int viewchange = 0;
int now_boosting = 0, simulation = 1;

float* camsmooth;
float* camlooksmooth;

bool in_field;
int  activerobot;
bool move_ball=false;
int POWER = 100;

double shootspeed = 1.0;
static RobotTri robbies[Nrobot];

static OdeSimComm serverobject;

LPCSTR sndShoot, sndBoost, sndCharge, sndResetCam, sndBall;

dGeomID  goal_parts[7];
dGeomID  goal_parts2[7];
dGeomID	 wall[8];
dGeomID	 pole[12];
double   cam_angle = 90, cam_zoom = 0.0;
int      map = 0, boost = 0; 
ROBOT    Robot[Nrobot];

static void followRules()    
{
	dReal *dpos; 
	dpos =(dReal*)dBodyGetPosition(sphere);
	double up,down,left,right;
	up    = FIELD_WIDTH/2.;
	down  = -FIELD_WIDTH/2.;
	left  = -FIELD_LENGTH/2.;
	right = FIELD_LENGTH/2.;
	
	if ((dpos[0]>left)&&(dpos[0]<right) &&
		dpos[1]>up+0.10)
	{
		
		dBodySetPosition(sphere,dpos[0]+((rand()%10)-5)*1.0f/10,up,1.0);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel(sphere, 0,0,0);
		dBodySetAngularVel(sphere, 0,0,0	);
		cout<<"OUT !! THROW IN!!! 0"<<endl;
	}
 	
	if ((dpos[0]>left)&&(dpos[0]<right) &&
		dpos[1]<down-0.10)
	{
		
		dBodySetPosition (sphere,dpos[0]+((rand()%10)-5)*1.0f/10,down,1.0);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel  (sphere, 0,0,0);
		
		dBodySetAngularVel  (sphere, 0,0,0	);
		cout<<"OUT !! THROW IN!!! 1"<<endl;
	}
	
	if ((dpos[0]<left) && (dpos[0]>left-50) &&  fabs(dpos[1])<1)
	{
		cout<<"GOAL_BLUE!!!! 2"<<endl;
		goaldelay++;
	}
	
	if ((dpos[0]>right) && (dpos[0]<right+50) &&  fabs(dpos[1])<1)
	{
		cout<<"GOAL_YELLOW 3"<<endl;
		goaldelay++;
	}
	if ((dpos[0]>right)&&  dpos[1]>1)
	{
		dBodySetPosition (sphere,right-0.5,up,0.4);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel  (sphere, 0,0,0);
		dBodySetAngularVel  (sphere, 0,0,0	);
		cout<<"OUT !! CORNER!!! 4"<<endl;
	}
	if ((dpos[0]>right)&&  dpos[1]<-1)
	{
 		dBodySetPosition (sphere,right-0.5,down,0.4);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel  (sphere, 0,0,0);
		dBodySetAngularVel  (sphere, 0,0,0	);
		cout<<"OUT !! CORNER!!! 5"<<endl;
	}
	if ((dpos[0]<left)&&  dpos[1]>1)
	{
 		dBodySetPosition (sphere,left+0.5,up,0.4);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel  (sphere, 0,0,0);
		dBodySetAngularVel  (sphere, 0,0,0	);
		cout<<"OUT !! CORNER!!! 6"<<endl;
	}
	if ((dpos[0]<left)&&  dpos[1]<-1)
	{
		dBodySetPosition (sphere,left+0.5,down,0.4);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel  (sphere, 0,0,0);
		dBodySetAngularVel  (sphere, 0,0,0	);
		cout<<"OUT !! CORNER!!! 7"<<endl;
	}
	
}
 
static void setFormation()   
{
	simulation = 0;
	stop = 0;
	cam_angle = 90;
	cam_zoom = 0.0;
	dBodySetPosition(sphere,0.0,0.0,0.14);
	dBodySetLinearVel( sphere, 0.0, 0.0, 0.0 );
	dBodySetAngularVel( sphere, 0.0, 0.0, 0.0 );
	
	destroyRobot( Nrobot );
	
	dJointGroupDestroy( contactgroup );
	contactgroup = dJointGroupCreate( 0 );
	
	makeRobot( Nrobot );
}

static void saveState(int state)
{
 	//Bodies
	const dReal* dreal;
 	for (int i=0; i<Nrobot; i++) 
	{
 		dreal=dBodyGetPosition(body[i]);
		savesimstate[state].robbies[i].pos[0]=dreal[0];
		savesimstate[state].robbies[i].pos[1]=dreal[1];
		savesimstate[state].robbies[i].pos[2]=dreal[2];
		dreal=dBodyGetLinearVel(body[i]);
		savesimstate[state].robbies[i].vel[0]=dreal[0];
		savesimstate[state].robbies[i].vel[1]=dreal[1];
		savesimstate[state].robbies[i].vel[2]=dreal[2];
		dreal=dBodyGetQuaternion(body[i]);
		savesimstate[state].robbies[i].q[0]=dreal[0];
		savesimstate[state].robbies[i].q[1]=dreal[1];
		savesimstate[state].robbies[i].q[2]=dreal[2];
		savesimstate[state].robbies[i].q[3]=dreal[3];
	}
	
	//Ball
	
	dreal=dBodyGetPosition(sphere);
	savesimstate[state].ball.pos[0]=dreal[0];
	savesimstate[state].ball.pos[1]=dreal[1];
	savesimstate[state].ball.pos[2]=dreal[2];
	dreal=dBodyGetLinearVel(sphere);
	savesimstate[state].ball.vel[0]=dreal[0];
	savesimstate[state].ball.vel[1]=dreal[1];
	savesimstate[state].ball.vel[2]=dreal[2];
	dreal=dBodyGetQuaternion(sphere);
	savesimstate[state].ball.q[0]=dreal[0];
	savesimstate[state].ball.q[1]=dreal[1];
	savesimstate[state].ball.q[2]=dreal[2];
	savesimstate[state].ball.q[3]=dreal[3];
}

static void recallSavedState(int state)
{
	for (int i=0; i<Nrobot; i++) 
	{
		dBodySetPosition(body[i],savesimstate[state].robbies[i].pos[0],savesimstate[state].robbies[i].pos[1],savesimstate[state].robbies[i].pos[2]);
		dBodySetLinearVel(body[i],savesimstate[state].robbies[i].vel[0],savesimstate[state].robbies[i].vel[1],savesimstate[state].robbies[i].vel[2]);
		dBodySetQuaternion(body[i],savesimstate[state].robbies[i].q);      
	}
	
	dBodySetPosition(sphere,savesimstate[state].ball.pos[0],savesimstate[state].ball.pos[1],savesimstate[state].ball.pos[2]);
	dBodySetLinearVel(sphere,savesimstate[state].ball.vel[0],savesimstate[state].ball.vel[1],savesimstate[state].ball.vel[2]);
	dBodySetQuaternion(sphere,savesimstate[state].ball.q);      
}

static void saveStatesToDisk()
{
	FILE * outfile;
	outfile= fopen("savestate.bin","w");
	fwrite(&savesimstate,sizeof(savesimstate),3,outfile);
	fclose(outfile);
	cout<<"saving"<<endl;
}
 
static void loadStatesFromDisk()
{
	FILE * infile;
	infile= fopen("savestate.bin","r");
	fread(&savesimstate,sizeof(savesimstate),3,infile);
	fclose(infile);
	cout<<"If no segmentation fault = >Loading ready"<<endl;
}
 
/*
static void nearCallback (void *data, dGeomID o1, dGeomID o2) 
{
	int i;
	// if (o1->body && o2->body) return;
	
	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;
	
	dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
	for (i=0; i<MAX_CONTACTS; i++) 
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = 0.1;
		//contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.7;
		contact[i].surface.bounce_vel = 0.2;
		contact[i].surface.soft_cfm = 0.03;

	}
	if (int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,
		sizeof(dContact))) 
	{
		dMatrix3 RI;
		dRSetIdentity(RI);
		const dReal ss[3] = {0.02,0.02,0.02};
		for (i=0; i<numc; i++) 
		{
			dJointID c = dJointCreateContact (world,contactgroup,contact+i);
			dJointAttach (c,b1,b2);
		}
	}
} */
 
static void nearCallback(void *data, dGeomID o1, dGeomID o2) 
{
	dBodyID b1 = dGeomGetBody( o1 );
	dBodyID b2 = dGeomGetBody( o2 );
	
	if( b1 && b2 && dAreConnectedExcluding( b1, b2, dJointTypeContact ) ) return;
	
	static const int Nall = 20;	
	dContact contact[Nall];		
	
	int n = dCollide( o1, o2, Nall, &contact[0].geom, sizeof(dContact) );
 	if( n > 0  ) 
	{
		for ( int i = 0; i < n; i++) 
		{
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactBounce |
				dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contact[i].surface.mu         = 2.0; // dInfinity;
			contact[i].surface.slip1      = 0.05;
			contact[i].surface.slip2      = 0.05;
			contact[i].surface.bounce     = 0.5; 
			contact[i].surface.bounce_vel = 0.5;
			contact[i].surface.soft_erp   = 0.8;  // 0.8
			contact[i].surface.soft_cfm   = 1e-3;  // 1e-4 
			
			dJointID c = dJointCreateContact( world, contactgroup, &contact[i] );
			dJointAttach( c, b1, b2 );
		}
	}
} 
  
static void view()
{
	static float hpr[3] = {0, 0, 0};		 
	static float xyz[3] = {0, 0, 0};		 
	static float cam_hpr[3] = {0, 0, 0};	 
	static float cam_xyz[3] = {0, 0, 0};	 
	const dReal *robot_hpr = dBodyGetRotation( body[activerobot] );	 // lrd ch
	const dReal *ball_hpr  = dBodyGetRotation( sphere );				 
	const dReal *robot_xyz = dBodyGetPosition( body[activerobot] );	 
	const dReal *ball_xyz  = dBodyGetPosition( sphere );				 
	double robot_angle = 0.0, SIN = 0.0, COS = 0.0, X = 0.0, Y = 0.0, Rotation = 0.0;
	double cam2Robot_angle = 0.0, cam2Ball_angle = 0.0;
	
	COS = *robot_hpr;
	SIN = *(robot_hpr + 1);
	if( SIN >= 0) robot_angle = -acos(COS);
	else robot_angle = acos(COS);          

	dsGetViewpoint( cam_xyz, cam_hpr );
	X = cam_xyz[0] - robot_xyz[0];
	Y = cam_xyz[1] - robot_xyz[1];
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else Rotation = atan(-Y/-X);
    cam2Robot_angle = Limit2PI(Rotation - cam_hpr[0] * DegreeToRad);

	X = cam_xyz[0] - ball_xyz[0];
	Y = cam_xyz[1] - ball_xyz[1];
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else Rotation = atan(-Y/-X);
    cam2Ball_angle = Limit2PI(Rotation - cam_hpr[0] * DegreeToRad);

	float camspeedpos=0.2;
	float camspeedlook=0.2;
	static float pos[3];
	static float look[4];
    int i = 0;
	
	if( viewchange %2 == 1 )
	{
        if( viewmode == 3 )
		{
			xyz[0] = robot_xyz[0] - 2 * cos( cam_angle * DegreeToRad );
			xyz[1] = robot_xyz[1] - 2 * sin( cam_angle * DegreeToRad );
			xyz[2] = 1.5;
			hpr[0] = cam_angle;
			hpr[1] = -26;
			hpr[2] =   0;
		}

        if( viewmode == 2 )
		{
			xyz[0] = ( robot_xyz[0] + ball_xyz[0]) / 2;
			xyz[1] = ( robot_xyz[1] + ball_xyz[1]) / 2 - (fabs(robot_xyz[1] - ball_xyz[1]) * fabs(robot_xyz[1] - ball_xyz[1] ) ) / ( 7.0 + ( fabs( robot_xyz[1] - ball_xyz[1] ) * fabs(robot_xyz[1] - ball_xyz[1] ) ) / 10.0 ) - ( fabs( xyz[0] - ball_xyz[0] ) * fabs( xyz[0] - ball_xyz[0] ) ) / 6.0 -2.5;
			if( xyz[1] <= -20.0 ) xyz[1] = -20.0; 
			xyz[2] = ( fabs( xyz[0] - ball_xyz[0] ) * fabs( xyz[0] - ball_xyz[0] ) ) / 50 + ball_xyz[2] + (float)1.3;
			hpr[0] =  90;
			hpr[1] = -26;
			hpr[2] =   0;
		}

        if( viewmode == 1 )
		{
			float diff = sqrt( pow(robot_xyz[0] - ball_xyz[0], 2) + pow(robot_xyz[1] - ball_xyz[1], 2) );
			float robotball = fabs(cam2Robot_angle - cam2Ball_angle);
			xyz[0] = (robot_xyz[0] + ball_xyz[0]) / 2 - ( (2.0 + cam_zoom + 1.3 * diff / (2 - 0.01 * robotball) ) ) * cos( cam_angle * DegreeToRad );
 			xyz[1] = (robot_xyz[1] + ball_xyz[1]) / 2 - ( (2.0 + cam_zoom + 1.3 * diff / (2 - 0.01 * robotball) ) ) * sin( cam_angle * DegreeToRad );
			xyz[2] = sqrt( pow( robot_xyz[0] - ball_xyz[0], 2 ) + pow( robot_xyz[1] - ball_xyz[1], 2 ) ) / 50  + ball_xyz[2] / 2.0 + (float)1.3 + cam_zoom;
			if( xyz[0] <= -25.0 ) xyz[0] = -25.0; 
			if( xyz[0] >=  25.0 ) xyz[0] =  25.0; 
			if( xyz[1] <= -20.0 ) xyz[1] = -20.0; 
			if( xyz[1] >=  20.0 ) xyz[1] =  20.0; 
			hpr[0] = cam_angle;
			hpr[1] = -26 - cam_zoom * 10;
			hpr[2] =  0;
		}

		if( viewmode == 0 )
		{
			xyz[0] = robot_xyz[0] - 0.85 * robot_hpr[0];
			xyz[1] = robot_xyz[1] + 0.85 * robot_hpr[1];
			xyz[2] = robot_xyz[2] + 0.35;	  
			hpr[0] = ( robot_angle ) * 180/M_PI;	
			hpr[1] = 0;
			hpr[2] = 0;
			cam_angle = ( robot_angle ) * 180/M_PI;
		}
		if( viewmode == 4 )
		{
 			pos[0]=0;
			pos[1]=0;
			pos[2]=15;    // view height
			look[0]=90.0000;
			look[1]=-90.0000;
			look[2]=0.0000;
			look[3]=0.0000;
			for( i=0;i<3;i++)
			{
				xyz[i]=xyz[i]*(1-camspeedpos)+pos[i]*camspeedpos;
				hpr[i]=hpr[i]*(1-camspeedlook)+look[i]*camspeedlook;
			}		 
			
		}
		if (viewmode == 5)
		{
			dReal* dpos;
			dReal* dlook;
			dpos =(dReal*)robot_xyz;//(dReal*)dGeomGetPosition(box[activerobot]); // robot_xyz
			dlook=(dReal*)robot_hpr;//(dReal*)dGeomGetRotation(box[activerobot]); // robot_hpr
			pos[0]=(float)dpos[0];
			pos[1]=(float)dpos[1];
			pos[2]=(float)dpos[2]+0.4;
			look[0]=(float)dlook[0];
			look[1]=(float)dlook[1];
			look[2]=(float)dlook[2];
			
			look[3]=(float)dlook[3];
			
			look[0]=(atan2(look[0],look[1])-M_PI/2)*180/M_PI;
			
			look[1]=-25;
			camspeedpos=0.4;
			camspeedlook=0.9;
			for ( i=0;i<3;i++)
			{
				xyz[i]=xyz[i]*(1-camspeedpos)+pos[i]*camspeedpos;
				hpr[i]=hpr[i]*(1-camspeedlook)+look[i]*camspeedlook;
			}		 
		}
		if( viewmode == 6 )
		{
			dReal* dpos;
			dReal* dlook;
			dpos =(dReal*)ball_xyz;//(dReal*)dGeomGetPosition(ball);
			dlook=(dReal*)ball_hpr;//(dReal*)dGeomGetRotation(ball);
			
			pos[0]=(float)dpos[0];
			pos[1]=(float)dpos[1];
			pos[2]=(float)+4.4;
			look[0]=(float)90;
			look[1]=(float)0;
			look[2]=(float)0;
			
			look[3]=(float)0;
			look[1]=-90;
			camspeedpos=0.05;
			camspeedlook=0.05;
			for ( i=0;i<3;i++)
			{
				xyz[i]=xyz[i]*(1-camspeedpos)+pos[i]*camspeedpos;
				hpr[i]=hpr[i]*(1-camspeedlook)+look[i]*camspeedlook;
			}		 
		}
 
  	    dsSetViewpoint(xyz,hpr);
  }
}
static void start()  
{
	float xyz[3] = {-0.012, -0.9817, 0.92};
	float hpr[3] = { 126.5, -30.5, 0.0};
	//float xyz[3] = {0.012, 0.9817, 0.92};
	//float hpr[3] = { -50.0, -30.5, 0.0};
	dsSetViewpoint( xyz,hpr );
	dsSetSphereQuality( 3 );

	printf("***************  How To Play  *****************\n");
	printf(" Control View Point\n");
	printf(" 'V' ------ Change view mode\n");
	printf(" 'Space' -- View Lock ON/OFF\n\n");

	printf(" Move The Robot\n");
	printf(" 'a' 'd' 'w' 's' -- Move robot\n");
	printf(" 'k' 'l' ---------- Rotate robot\n");
	printf(" 'z' ----- Place Ball at ActiveRobot and shoot.\n\n");

	printf(" Others\n");
 	printf ("  'r' ----- Reset ball.\n");
	printf ("  'b' ----- Move Ball toggle.\n");
	printf ("  'y' ----- Ball from some distance to roll leave.\n");
	//printf ("   't' viewport toggel n1.person/ from above.\n");
	printf ("  'f' ----- Both teams formation take.\n");
	printf ("  'n' ----- Active robot forwards or back.\n");
	printf ("  'c' ----- Capture Ball Position/Velocity.\n");
	printf ("  'p' ----- Play Ball Position/Velocity.\n");
	printf ("  'u' ----- Start automatic decision.\n");

	PlaySound( NULL, NULL, NULL );
}

void Move( double velocity, double angle, double omega )
{
 	double RVx=0.0, RVy=0.0, ROme=0.0;
	
	RVx  = velocity*cos(angle);
	RVy  = velocity*sin(angle);
	ROme = omega;
	
    *wheelSpeed = (root3*RVx/2+RVy/2+wl*ROme)/RADIUS;
    *(wheelSpeed+1)=(-root3*RVx/2+RVy/2+wl*ROme)/RADIUS;
    *(wheelSpeed+2)=(-RVy+wl*ROme)/RADIUS;
}

void Kick( int i )
{
	dReal *dpos;
	dReal *dlook;
	//dpos=new double[3];
	dpos =(dReal*)dGeomGetPosition(box[i]);
	V2d robotPos;
	//cout<<"activerobot"<<activerobot<<endl;
	robotPos.setPoint(-dpos[0],-dpos[1]);
	//cout <<robotPos;
	dlook=(dReal*)dGeomGetRotation(box[i]);
	V2d heading;
	heading.setPoint(-dlook[0],dlook[1]); // dlook * -1
	Frame2d framerobot;
	framerobot.set_position(robotPos);
	framerobot.set_angle(heading);
	framerobot.invert();
	dpos =(dReal*)dGeomGetPosition(ball);
	V2d absBall(-dpos[0],-dpos[1]);
	V2d relBall=framerobot*absBall;
	
	if( (fabs(relBall.angle_DEG()) < 20)
		&& (relBall.length()<0.43)
		)
	{
		V2d glob=((robotPos+heading.normalize().scale(0.30))-absBall).normalize().scale(0.2);
		
		if ( (robbies[i].kick) ) // activerobot==i && joy.button[3]==1
		{   // apply kick
			//glob=(robotPos-absBall).normalize().scale(3.1);
			//dBodySetLinearVel(sphere,glob.x,glob.y,0.7);
			//dBodyAddForce(sphere,glob.x,glob.y,0.7);
			const dReal *bp  = dBodyGetPosition(sphere);
			const dReal *p   = dBodyGetPosition(body[i]); 
			const dReal *R   = dBodyGetRotation(body[i]);
			dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1],2));
			if ( bp[2] < 0.4 && dist < 0.3 ) 
			{ 
 				dReal vmax45 = POWER * 0.014 * 8.0 / sqrt(2.0);
				dBodySetLinearVel(sphere,vmax45 * R[0],vmax45 * R[4], vmax45/2.0);
				// printf("z:vx=%f vy=%f \n",R[0],R[4]); 
			}
			cout<< "KICKING!!! with"<<R[0]<<" "<<R[4]<<endl;
		}
		
	} 
}

static void command (int cmd)
{
	//  cout <<"Taste:"<<cmd<<endl;
	size_t i;
	int j,k;
	const dReal* bpos;
	bpos = dBodyGetPosition(sphere);
    
	switch(cmd)
	{
	case ' ':
		viewchange += 1;
		break;
	case 'v':case'V':
		viewchange = 1;
		viewmode=(viewmode+1)%7;
		cout <<"viewmode:"<<viewmode<<endl;
		break;
	case 'b':
		move_ball=!move_ball;
		cout<< "BAll Move is"<<move_ball<<endl;  
		break;
	case 'w':
		{
			if (move_ball) 
			{
				dBodySetPosition(sphere,bpos[0],bpos[1]+0.5,bpos[2]);
				dBodySetLinearVel(sphere,  0,0,0);
			}
			else 
			{
				dBodyAddRelForce (body[activerobot],80,0,0);
			}
		}break;
	case 's':
		{
			if (move_ball)
			{
				dBodySetPosition (sphere,bpos[0],bpos[1]-0.5,bpos[2]);
				dBodySetLinearVel  (sphere,  0,0,0);
			}
			else 
				dBodyAddRelForce (body[activerobot],-80,0,0);
		}break;
	case 'a':
		{
			if (move_ball) 
			{
				dBodySetPosition (sphere,bpos[0]+.5,bpos[1],bpos[2]);
				dBodySetLinearVel  (sphere,  0,0,0);
			}
			else 
				dBodyAddRelForce (body[activerobot],0,-80,0);
		}break;
	case 'd':
		{
			if (move_ball) 
			{
				dBodySetPosition (sphere,bpos[0]-.5,bpos[1],bpos[2]);
				dBodySetLinearVel  (sphere,  0,0,0);
			}
			else 
				dBodyAddRelForce (body[activerobot],0,80,0);
		}break;
	case 'k':
		dBodyAddRelTorque (body[activerobot], 0, 0,60.0f/10);
        break;
	case 'l':
		dBodyAddRelTorque (body[activerobot], 0, 0,-60.0f/10);
		break;
    case 'r':
		{
		   // dBodySetPosition (sphere,0,-1,1);
			int random_position_x = 0;
			int random_position_y = 0;
			dBodySetLinearVel( sphere, 0.0, 0.0, 0.0 );
			dBodySetAngularVel( sphere, 0.0, 0.0, 0.0 );
			srand( GetTickCount() );
			random_position_x = rand() % 18;
			srand( (unsigned)myGetTickCount() );
			random_position_y = rand() % 12 ;
			PlaySound( sndBall, NULL, SND_MEMORY | SND_ASYNC ); 
			dBodySetPosition( sphere, -9 + random_position_x, -6 + random_position_y, Z );
		}break;
    case 'y':
		{
			dBodySetPosition(sphere,0,0,0.5);
			dReal* ballvel;
			//(dReal*)dBodyGetLinearVel (sphere);
			dBodySetLinearVel(sphere, (rand()%20+10)*0.1,(rand()%10-5)*0.1f,0);
			dBodySetAngularVel(sphere, 0,0,0	);
		}break;
	case 'f':
		setFormation();
		cout<<"Setting Formation!!"<<endl;
		break;
	case 'n':
		activerobot = (activerobot+1)%Nrobot;
		cout<<"activating Robot no "<<activerobot<<endl;
		break;
	case 'N':
		activerobot=(activerobot-1);
		if (activerobot<0) activerobot = Nrobot-1;
		cout<<"activating Robot no "<<activerobot<<endl;
		break;
	case 'c':
		{
			cout<<"saving ball data"<<endl;;
			dReal *d;
			d =(dReal*)dBodyGetPosition (sphere);
			savedBallPos[0]=d[0];  savedBallPos[1]=d[1];  savedBallPos[2]=d[2];
			d =(dReal*)dBodyGetLinearVel (sphere);
			savedBallVel[0]=d[0];  savedBallVel[1]=d[1];  savedBallVel[2]=d[2];
			d =(dReal*)dBodyGetAngularVel (sphere);
			savedBallAngVel[0]=d[0];  savedBallAngVel[1]=d[1];  savedBallAngVel[2]=d[2];
		}break;
	case 'p': 
		{
			cout<<"recalling ball data"<<endl;;
			dBodySetPosition (sphere,savedBallPos[0],savedBallPos[1],savedBallPos[2]);
			dBodySetLinearVel  (sphere,  savedBallVel[0],savedBallVel[1],savedBallVel[2]);
			dBodySetAngularVel  (sphere, savedBallAngVel[0],savedBallAngVel[1],savedBallAngVel[2]);
		}break;
	case 'z': 
		{
			cout<<"Replace Ball and shoot"<<endl;;
				dReal* dpos;
			dReal* dlook;
			dpos =(dReal*)dGeomGetPosition(box[activerobot]);
			dlook=(dReal*)dGeomGetRotation(box[activerobot]);  // dlook * -1
			dBodySetPosition (sphere,dpos[0]+dlook[0]/2,dpos[1]-dlook[1]/2,0.2);  
			dBodySetLinearVel (sphere,  dlook[0]*shootspeed,  -dlook[1]*shootspeed,  0);
			dBodySetAngularVel (sphere, 0,0,0);
		/*	
			const dReal *bp  = dBodyGetPosition(sphere);
			const dReal *p   = dBodyGetPosition(body[activerobot]); 
			const dReal *R   = dBodyGetRotation(body[activerobot]);
			dReal dist  = sqrt(pow(bp[0]-p[0],2)+pow(bp[1]-p[1],2));
			if ( bp[2] < 0.4 && dist < 0.3 ) 
			{ 
 				dReal vmax45 = POWER * 0.014 * 8.0 / sqrt(2.0);
				dBodySetLinearVel(sphere,vmax45 * R[0],vmax45 * R[4], vmax45/2.0);
				//printf("z:vx=%f vy=%f \n",R[0],R[4]); 
			}*/
 		 
		}break;
	case 'g': 
		{
			shootspeed+=0.1;
			cout<<"Increase Ball Replacement  Shoot Speed to "<<shootspeed<< endl;;
		}break;
	case 'h': 
		{
			shootspeed-=0.1;
			cout<<"Decreasing Ball Replacement  Shoot Speed to "<<shootspeed<<endl;;
		}break;
	case '1': 
		{
			cout<<"Recalling Saved State of Simulation !!!!"<<shootspeed<< endl;;
			recallSavedState(0);
		}break;
	case '!': 
		{
			saveState(0);
			cout<<"Saving State of Simulation"<<shootspeed<<endl;;
		}break;
		
	case '2': 
		{
			cout<<"Recalling Saved State of Simulation !!!!"<<shootspeed<< endl;;
			recallSavedState(1);
		}break;
	case '"': 
		{
			saveState(1);
			cout<<"Saving State of Simulation"<<shootspeed<<endl;;
		}break;
		
	case 'S': 
		{
			saveStatesToDisk();
			cout<<"Saving States to Disk"<<shootspeed<<endl;;
		}break;
		
	case 'L': 
		{
			loadStatesFromDisk();
			cout<<"Load States from Disk"<<shootspeed<<endl;;
		}
		
	case 'm':case 'M':
		{
			map = (map + 1)%2;
		}break;
	case 'u':
		{
			simulation = 1;
			stop = 0;
		}break;
	case 'i': 
		{
			simulation = 0;
			stop = 1;
		}break;
	case 'q':case 'Q':
		{
			exit(0);
		}break;
		
	default:break;
	}
 }

static void communicate()  
{
    // receive and try to map the client to a robbi and send back appropriate Worldmodel data
    int howmanyreceived;
    howmanyreceived=0;
    serverobject.serverdata.check_for_inactivity();
	double RVx=0.0, RVy=0.0, ROme=0.0;
	
    while (serverobject.receive()) // Receive all client data
	{   
		// this is robbi steering information for the server
		
		RVx  = serverobject.simstate->steer[0]*2.0;
		RVy  = serverobject.simstate->steer[1]*2.0;
		ROme = serverobject.simstate->steer[2]*3.8;
		
		robbies[serverobject.last_client].steer[0]=(root3*RVx/2+RVy/2+wl*ROme)/RADIUS; 
		robbies[serverobject.last_client].steer[1]=(-root3*RVx/2+RVy/2+wl*ROme)/RADIUS; 
		robbies[serverobject.last_client].steer[2]=(-RVy+wl*ROme)/RADIUS; 

		//robbies[serverobject.last_client].steer[0]=serverobject.simstate->steer[0]; 
		//robbies[serverobject.last_client].steer[1]=serverobject.simstate->steer[1]; 
		//robbies[serverobject.last_client].steer[2]=serverobject.simstate->steer[2]; 
		robbies[serverobject.last_client].kick=serverobject.simstate->kick;
		
		howmanyreceived++;
		
	}
	//cout <<"Number of Received Infos: "<<howmanyreceived<<endl;
 	
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (serverobject.serverdata.client_active[i])
		{
			// this is worldmodel information for the client
			//cout <<"last_client"<<serverobject.last_client<<endl;
			dReal *dpos;
			dReal *dlook;
			// dpos=new double[3];
			dpos =(dReal*)dGeomGetPosition(box[i]);
			V2d robotPos,robotVel;
			robotPos.setPoint(dpos[0],dpos[1]);
			dlook=(dReal*)dGeomGetRotation(box[i]);
			V2d heading;
			heading.setPoint(dlook[0],-dlook[1]);
			//Set Pos in Communication object
			serverobject.simstate->pos[0]=-dpos[0];
			serverobject.simstate->pos[1]=-dpos[1];
			serverobject.simstate->pos[2]= heading.angle();
			
			dpos =(dReal*)dBodyGetLinearVel(body[i]);
			robotVel.setPoint(dpos[0],dpos[1]);
			robotVel=robotVel.rotate(-heading.angle());
			serverobject.simstate->robvel[0]=robotVel.x;
			serverobject.simstate->robvel[1]=robotVel.y;

			dlook =(dReal*)dBodyGetAngularVel(body[i]);
			//Robvel
			serverobject.simstate->robvel[2]=dlook[2];
			
			dpos =(dReal*)dGeomGetPosition(ball);
			V2d absBall(dpos[0],dpos[1]);
			//cout <<"absball "<<absBall<<endl;
			serverobject.simstate->ball[0]=-dpos[0];
			serverobject.simstate->ball[1]=-dpos[1];
			for(int j=0;j<16;j++)
			{
				dpos =(dReal*)dGeomGetPosition(box[j]);
				serverobject.simstate->obstacles[0][j]=-dpos[0]; // lrd * -1
				serverobject.simstate->obstacles[1][j]=-dpos[1];
			}
			
			serverobject.simstate->num_obstacles = 16; // 16
			serverobject.sock.serv_addr=serverobject.serverdata.serv_addr[i];
			serverobject.send();
		}
		else
		{// when client inactive, reset the steering
			robbies[i].steer[0]=0; 
			robbies[i].steer[1]=0; 
			robbies[i].steer[2]=0; 
			robbies[i].kick=false;
			
		}
	}
}

void BallControl(const dReal *x, const dReal *y, const dReal *z)
{
	double xx = 0, yy = 0, zz = 0; 
	double rate =  50;
	double rate2 = 0.01;
	double BallX = 0, BallY = 0;          
	
	BallX = *(dBodyGetPosition( sphere ));
	BallY = *(dBodyGetPosition( sphere ) + 1);
	
	xx = *x - *x / rate;
	yy = *y - *y / rate;
	zz = *z;
	
	if ( *z < 0.11 ) 
	{
		if ( xx < rate2 && xx > -rate2 ) 
		{
			if ( yy < rate2 && yy > -rate2 ) 
			{
				xx = 0;
				yy = 0;
			}
		}  
	}
	dBodySetLinearVel( sphere, xx, yy, zz );	
}

void Control()
{
	int i;
	const dReal * angularv;
	const dReal * linearv;
	
	double drag_linear;
	double drag_angular;
	double balldrag_linear;
	double balldrag_angular;
	drag_linear  = 40;
	drag_angular = 1.2;
	balldrag_linear  = -0.005101f;
	balldrag_angular = -0.005101f;
	
	dReal * speed;
	dReal * stpos;

	double fMax = 100;
	const dReal *robot_hpr = dBodyGetRotation(Robot[activerobot].body_base[0]);
	double robot_rotation = 0;
	float  xyz[3], hpr[3];
	double angle = 0, enemy_angle = 0, team_angle = 0, s_angle = 0;

	dsGetViewpoint( xyz, hpr );
	if(robot_hpr[1]<0.0)	
		robot_rotation =  180/M_PI*acos(robot_hpr[0]);
	else		
		robot_rotation = -180/M_PI*acos(robot_hpr[0]);

	angle =  M_PI / 180 * (hpr[0] - robot_rotation);
 
    if(GetAsyncKeyState(VK_ESCAPE)) exit(0); 
    if(GetAsyncKeyState('7')) cam_angle++;
    if(GetAsyncKeyState('8')) cam_angle--;
	if(GetAsyncKeyState('9')) {cam_zoom += 0.02; if(cam_zoom >= 1.0) cam_zoom = 1.0;}
	if(GetAsyncKeyState('0')) {cam_zoom -= 0.02; if(cam_zoom <=-1.0) cam_zoom =-1.0;}
	if(GetAsyncKeyState('6')) {cam_angle = robot_rotation; cam_zoom = 0.0; PlaySound( sndResetCam, NULL, SND_MEMORY | SND_ASYNC );}
 

	if(viewmode %2 == 0)
	{
		LTURN = 12.7; 
	    RTURN = -12.7;
	}
	else
	{  
		LTURN = 15.0; 
		RTURN = -15.0;
	}
	for( i=0; i< Nrobot; i++ )
	{
		// apply forces to all bodies
		dJointSetHingeParam( Robot[i].joint_wheel[0], dParamFMax, fMax );
		dJointSetHingeParam( Robot[i].joint_wheel[1], dParamFMax, fMax );
		dJointSetHingeParam( Robot[i].joint_wheel[2], dParamFMax, fMax );        
		if( activerobot == i )
		{

			if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))	{Move(velocity, angle +   M_PI/4,	LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))		{Move(velocity, angle +   M_PI/4,	RTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))  {Move(velocity, angle -   M_PI/4,	LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))	{Move(velocity, angle -   M_PI/4,	RTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT)) {Move(velocity, angle - 5*M_PI/4,	LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_SHIFT)&& GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))	{Move(velocity, angle - 5*M_PI/4,	RTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT)){Move(velocity, angle + 5*M_PI/4,	LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_SHIFT) && GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))	{Move(velocity, angle + 5*M_PI/4,	RTURN); simulation = 2;} 
			
			else if(GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))   {Move(velocity, angle + M_PI/4, 0); simulation = 2;}		 
			else if(GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))  {Move(velocity, angle - M_PI/4, 0); simulation = 2;}		
			else if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT)) {Move(velocity, angle - 5*M_PI/4, 0); simulation = 2;}		 
			else if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT)){Move(velocity, angle + 5*M_PI/4, 0); simulation = 2;}		 
			
			else if(GetAsyncKeyState(VK_UP)   && GetAsyncKeyState(VK_CONTROL))	{Move(velocity,	angle,			LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_UP)   && GetAsyncKeyState(VK_SHIFT))	{Move(velocity, angle,			RTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_CONTROL))	{Move(velocity, angle +   M_PI, LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_SHIFT))	{Move(velocity, angle +   M_PI, RTURN); simulation = 2;} 
			
			else if(GetAsyncKeyState(VK_LEFT)  && GetAsyncKeyState(VK_CONTROL)) {Move(velocity, angle + M_PI/2, LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_LEFT)  && GetAsyncKeyState(VK_SHIFT))	{Move(velocity, angle + M_PI/2, RTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_CONTROL)) {Move(velocity, angle - M_PI/2, LTURN); simulation = 2;} 
			else if(GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_SHIFT))	{Move(velocity, angle - M_PI/2, RTURN); simulation = 2;} 
			
			else if(GetAsyncKeyState(VK_UP))	  {Move(velocity, angle,		  0); simulation = 2;}	
			else if(GetAsyncKeyState(VK_DOWN))	  {Move(velocity, angle +   M_PI, 0); simulation = 2;}
			else if(GetAsyncKeyState(VK_LEFT))	  {Move(velocity, angle + M_PI/2, 0); simulation = 2;}	
			else if(GetAsyncKeyState(VK_RIGHT))	  {Move(velocity, angle - M_PI/2, 0); simulation = 2;}	
			else if(GetAsyncKeyState(VK_CONTROL)) {Move(0, 0, LTURN); simulation = 2;}					
			else if(GetAsyncKeyState(VK_SHIFT))	 {Move(0, 0, RTURN);simulation = 2;	}	
			else if(simulation == 2){*wheelSpeed =   0; *(wheelSpeed+1) =   0; *(wheelSpeed+2) =   0;}	

			dJointSetHingeParam( Robot[i].joint_wheel[0], dParamVel , *wheelSpeed     );
			dJointSetHingeParam( Robot[i].joint_wheel[1], dParamVel , *(wheelSpeed+1) );
			dJointSetHingeParam( Robot[i].joint_wheel[2], dParamVel , *(wheelSpeed+2) );

		}

		if( simulation == 0 || stop == 1 )
		{
			*wheelSpeed = 0; 
			*(wheelSpeed+1) = 0; 
			*(wheelSpeed+2) = 0; 
		}
		else if( simulation == 1 )
		{
			dJointSetHingeParam( Robot[i].joint_wheel[0], dParamVel , robbies[i].steer[0] );
			dJointSetHingeParam( Robot[i].joint_wheel[1], dParamVel , robbies[i].steer[1] );
			dJointSetHingeParam( Robot[i].joint_wheel[2], dParamVel , robbies[i].steer[2] );
		}
		

		//dJointSetHingeParam( Robot[i].joint_wheel[0], dParamVel , 30 );
		//dJointSetHingeParam( Robot[i].joint_wheel[1], dParamVel , 30 );
		//dJointSetHingeParam( Robot[i].joint_wheel[2], dParamVel , 30 );
        Kick(i);
	
		if (i == activerobot)
		{
			linearv = dBodyGetLinearVel(body[i]);
			angularv = dBodyGetAngularVel(body[i]);
		//  cout<<"Vx = "<<setprecision(2)<<linearv[0]<<endl;
		//  cout<<"Vy = "<<setprecision(2)<<linearv[1]<<endl;
		//	cout<<"Vz = "<<setprecision(2)<<linearv[2]<<endl;
	  	//cout<<"Vw0 = "<<setprecision(2)<<angularv[0]<<endl;
		// 	cout<<"Vw1 = "<<setprecision(2)<<angularv[1]<<endl;
		//  	cout<<"Vw2 = "<<setprecision(2)<<angularv[2]<<endl;
		}
	}
  
}

static void simLoop (int pause)
{
 	static double startCount = 0.0;
	static double nowCount = 0.0;
	static int    flag = 0;
	static double startCount2 = 0.0;
	static double stopCount = 0.0;
	static int    flag2 = 0;

	followRules();  
	
	if( !pause ) 
	{
 		dSpaceCollide( space, 0, &nearCallback );
		dWorldStep( world, sim_step ); // 0.033  0.016 sim_step
		// remove all contact joints
		dJointGroupEmpty( contactgroup );
		BallControl( dBodyGetLinearVel( sphere ), dBodyGetLinearVel( sphere ) + 1, 
			dBodyGetLinearVel( sphere ) + 2 );
	}
	// printf("KICK POWER:%3d\r",POWER);
	// dsSetColor (1,0,1);
    Control();
  	view();
 	
    drawRobot(Nrobot);
	drawField(); 
	drawBall();
 	drawPlayer();

	if( goaldelay > 0 ) goaldelay++;
	if( goaldelay > 60 )
	{
		dBodySetPosition(sphere,0,0,0);
		//(dReal*)dBodyGetLinearVel (sphere);
		dBodySetLinearVel(sphere, 0,0,0);
		dBodySetAngularVel(sphere, 0,0,0	);
		goaldelay=0;
	}
 
	//dsSetColor (1,1,0);
	
	if( map == 0 )
	{
		drawMap();
	}
 	simloopCount++; 
	//cout<<"Yeah!!  "<<robots[0].steer[0];
	
	communicate(); 
	
	if( flag == 0 )
	{
		startCount = myGetTickCount();
		first_step = step;
		flag = 1;
	}
	else if ( flag == 1 )
	{
		nowCount = myGetTickCount() - startCount;
		step++;
		if( nowCount <= 1000 )
		{
			tmp_step = step - first_step;
		}
		if( nowCount > 1000)
		{
			sim_step = 0.68/tmp_step;  
			if( sim_step >= 0.05 ) 
				sim_step = 0.05;
			flag = 2;
		}
	}

	if( flag == 2 && simulation == 2)
	{
		startCount = myGetTickCount();
		flag = 3;
	}
	else if ( flag == 3 )
	{
		if( pause || Pause%2 )
		{
			if ( flag2 == 0 )
			{
				startCount2 = myGetTickCount();
				flag2 = 1;
			}
			else if(flag2)
			{  
			    stopCount = myGetTickCount() - startCount2;
			}
		}
		else
		{
			nowCount = myGetTickCount() - startCount - stopCount;
			flag2 = 0;
		}
	}

 	Sleep(50);
	
}

static void LoadSound()
{
  HANDLE fh;
  DWORD dwFileSize,dwReadSize;

	fh = CreateFile(SOUND_SHOOT,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"can not open file",SOUND_SHOOT,MB_OK);
	}
	dwFileSize = GetFileSize(fh,NULL);
	sndShoot = (LPCSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwFileSize);
	ReadFile(fh,(LPVOID)sndShoot,dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);


	fh = CreateFile(SOUND_BALL,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"can not open file",SOUND_BALL,MB_OK);
	}
	dwFileSize = GetFileSize(fh,NULL);
	sndBall = (LPCSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwFileSize);
	ReadFile(fh,(LPVOID)sndBall,dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);

	fh = CreateFile(SOUND_CHARGE,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"can not open file",SOUND_CHARGE,MB_OK);
	}
	dwFileSize = GetFileSize(fh,NULL);
	sndCharge = (LPCSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwFileSize);
	ReadFile(fh,(LPVOID)sndCharge,dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);

	fh = CreateFile(SOUND_BOOST,GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fh == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"can not open file",SOUND_BOOST,MB_OK);
	}
	dwFileSize = GetFileSize(fh,NULL);
	sndBoost = (LPCSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwFileSize);
	ReadFile(fh,(LPVOID)sndBoost,dwFileSize,&dwReadSize,NULL);
	CloseHandle(fh);

}

static void CloseSound()
{
	PlaySound(NULL,NULL,0);
    HeapFree(GetProcessHeap(),0,(LPVOID)sndShoot);
    HeapFree(GetProcessHeap(),0,(LPVOID)sndBall);
	HeapFree(GetProcessHeap(),0,(LPVOID)sndCharge);
	HeapFree(GetProcessHeap(),0,(LPVOID)sndBoost);
}

int main(int argc, char **argv)
{
	LoadSound();
	serverobject.use_as_server(30001);
	serverobject.use_nonblock();
	
	camsmooth = new float[3];
	camlooksmooth = new float[3];
	camsmooth[0]=0;camlooksmooth[0]=0;
	camsmooth[1]=0;camlooksmooth[1]=0;
	camsmooth[2]=0;camlooksmooth[1]=0;
	
	goaldelay=0;
	activerobot=0;

	int i,j;
	dMass m;
	// setup pointers to drawstuff callback functions
	dsFunctions fn;
	fn.version = DS_VERSION;
	fn.start = &start;
	fn.step = &simLoop;
	fn.command = &command;
	//fn.stop = 0;
	fn.path_to_textures = "textures";
	
	world = dWorldCreate();
	dWorldSetContactMaxCorrectingVel( world, 5.0 );
	dWorldSetGravity (world,0,0,-9.8);  // 0.9

	space = dHashSpaceCreate( 0 );
	contactgroup = dJointGroupCreate( 0 );
	ground = dCreatePlane (space,0,0,1,0);
	

 	makeRobot(Nrobot);
    makeField();
	makeBall();
 //	setFormation();
	
	saveState(0);
	saveState(1);
	
	// run simulation
	dsSimulationLoop (argc,argv,800,472,&fn);
	
	dJointGroupDestroy (contactgroup);
	CloseSound();

	dSpaceDestroy (space);
	dWorldDestroy (world);
	
	return 0;
}
  
