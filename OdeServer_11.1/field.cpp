#include "OdeServer.h"

void drawDot(double x, double y, float dotsize, float color[3])
{
		glPushAttrib(GL_TRANSFORM_BIT |   
				     GL_ENABLE_BIT);      
		glDisable (GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glPointSize(dotsize);
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_POINTS);
        glVertex2f(x*0.025+0.67, y*0.04-0.65);
		glEnd();
		glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glPopAttrib();
}


void drawBar(double length)
{
 	double x =  0.95;		 
	double y = -0.945;		 
	double height = 0.05;	 
	double width = length * 0.00028;

	glPushAttrib(GL_TRANSFORM_BIT |  /* for matrix contents */
			     GL_ENABLE_BIT);     /* for various glDisable calls */
	glDisable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_QUADS);
	glVertex2d(x - width, y - height);
    glVertex2d(		   x, y - height);
	glVertex2d(		   x,  		   y);
    glVertex2d(x - width,		   y);
	glEnd();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}
  
void drawMap()
{
	int i;
	double th1,th2,th1_rad,th2_rad ,hankei=1.0, x1,x2,y1,y2;
	double BallX = 0,  BallY = 0;				 
    double RobotX = 0, RobotY = 0;               
	double PlayerX =0, PlayerY = 0;				 
	double EnemyX = 0, EnemyY = 0;				 
 
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glColor4f(0.35, 0.8, 0.35, 0.8);
    glBegin(GL_QUADS);
    glVertex2d(0.386, -0.94);
    glVertex2d(0.953, -0.94);
    glVertex2d(0.953, -0.36);
    glVertex2d(0.386, -0.36);
	glEnd();

    glColor4f(0.1, 0.1, 0.1, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.386, -0.94);
    glVertex2d(0.953, -0.94);
    glVertex2d(0.953, -0.36);
    glVertex2d(0.386, -0.36);
	glEnd();

    glColor4f(1, 1, 1 , 0.3);
    glBegin(GL_LINES);
    glVertex2d(0.6695, -0.41);
    glVertex2d(0.6695, -0.89);
	glEnd();

	glBegin(GL_LINE_LOOP);
    glVertex2d(0.4427, -0.89);
    glVertex2d(0.8963, -0.89);
    glVertex2d(0.8963, -0.41);
    glVertex2d(0.4427, -0.41);
	glEnd();

	glBegin(GL_LINE_STRIP);
    glVertex2d(0.4427, -0.78);
    glVertex2d( 0.50 , -0.78);
    glVertex2d( 0.50 , -0.52);
    glVertex2d(0.4427, -0.52);
	glEnd();

	glBegin(GL_LINE_STRIP);
    glVertex2d(0.4427, -0.73);
    glVertex2d(0.47  , -0.73);
    glVertex2d(0.47  , -0.57);
    glVertex2d(0.4427, -0.57);
	glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2d(0.8963, -0.78);
    glVertex2d(0.837 , -0.78);
    glVertex2d(0.837 , -0.52);
    glVertex2d(0.8963, -0.52);
	glEnd();

	glBegin(GL_LINE_STRIP);
    glVertex2d( 0.8963, -0.73);
    glVertex2d( 0.869 , -0.73);
    glVertex2d( 0.869 , -0.57);
    glVertex2d( 0.8963, -0.57);
	glEnd();

    for (th1 = 0.0;  th1 < 360.0;  th1 = th1 + 10.0)
	{              
		th2 = th1 + 10.0;
		th1_rad = th1 / 180.0 * M_PI;  
		th2_rad = th2 / 180.0 * M_PI; 

		x1 = hankei * cos(th1_rad);
		y1 = hankei * sin(th1_rad);
		x2 = hankei * cos(th2_rad);
		y2 = hankei * sin(th2_rad);

		glBegin(GL_LINES);			
		glVertex2f( x1*0.05+0.67, y1*0.08-0.65 );      
		glVertex2f( x2*0.05+0.67, y2*0.08-0.65 );
		glEnd();			
	}

		glBegin(GL_LINE_STRIP);
		glVertex2d( 0.898, -0.70);
		glVertex2d( 0.912, -0.70);
		glVertex2d( 0.912, -0.60);
		glVertex2d( 0.898, -0.60);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2d( 0.442, -0.70);
		glVertex2d( 0.430, -0.70);
		glVertex2d( 0.430, -0.60);
		glVertex2d( 0.442, -0.60);
		glEnd();

		glColor4f(0, 0, 1 , 0.3);
		glBegin(GL_LINE_STRIP);
		glVertex2d( 0.898, -0.698);
		glVertex2d( 0.910, -0.698);
		glVertex2d( 0.910, -0.604);
		glVertex2d( 0.898, -0.604);
		glEnd();
	
		glColor4f(1, 1, 0 , 0.3);
		glBegin(GL_LINE_STRIP);
		glVertex2d( 0.44, -0.698);
		glVertex2d( 0.432, -0.698);
		glVertex2d( 0.432, -0.604);
		glVertex2d( 0.44, -0.604);
		glEnd();

 
    for(i=0; i<Nrobot; i++)
    {
	  if (i == activerobot)
	  {
		  continue;
	  }
	  RobotX = *(dBodyGetPosition( body[i] ) );
	  RobotY = *(dBodyGetPosition( body[i] ) + 1);
	  float robotcolor[3] = {0.1, 0.1, 0.1};
	  float enemycolor[3] = {1.3, 1.3, 1.3};
      float robotsize = 8.0;
	  if ((i/4)%2)
	  {
	    drawDot(RobotX, RobotY, robotsize, enemycolor);
	  }
	  else
		drawDot(RobotX, RobotY, robotsize, robotcolor);
   	}
	
	PlayerX = *(dBodyGetPosition( body[activerobot] ));
	PlayerY = *(dBodyGetPosition(  body[activerobot] ) + 1);
    float playercolor[3] = {0, 0, 1};
	float playersize = 8.0;
	drawDot(PlayerX, PlayerY, playersize, playercolor);

	BallX = *(dBodyGetPosition(sphere));
    BallY = *(dBodyGetPosition(sphere)+1);

    float ballcolor[3]={1.0, 0.0, 0.0};
    float ballsize = 5.0;
    drawDot(BallX, BallY, ballsize, ballcolor);

}

void drawCircle(dReal r,dReal center[2], int angle1, int angle2)
{
	dReal pos1[3], pos2[3], z = 0.01;
	pos1[0] = r * cos( angle1 * M_PI/180.0 ) + center[0];  
	pos1[1] = r * sin( angle1 * M_PI/180.0 ) + center[1];  
	pos1[2] = z;
	for( int i = angle1; i <= angle2; i++ )
	{
		pos2[0] = r* cos(i * M_PI/180.0) + center[0];
		pos2[1] = r* sin(i * M_PI/180.0) + center[1];
		pos2[2] = z;
		dsDrawLine(pos1,pos2);
		pos1[0] = pos2[0]; pos1[1] = pos2[1]; pos1[2] = pos2[2];
	}    
}

void drawPole()
{
	int i = 0;
	dsSetColor( 0, 0, 255 );
	for( i = 0; i <= 5; i++ )
	{
		dsDrawCylinder( dGeomGetPosition( pole[i] ), dGeomGetRotation( pole[i] ), (float)0.3, (float)POLERAD );
	}
	dsSetColor(255, 255, 0);	
	for( i = 6; i <= 11; i++ )
	{
		dsDrawCylinder( dGeomGetPosition( pole[i] ), dGeomGetRotation( pole[i] ), (float)0.3, (float)POLERAD );
	} 
}


void drawGoal()
{
	int i = 0;
 	dsSetTexture( DS_NONE );
	for( i = 0; i < 7; i++ )
	{
		if( i < 4 )   dsSetColor( 1.3, 1.3, 1.3 );
		else          dsSetColor( 1.3, 1.3, 0.0 );
		dsDrawBox( dGeomGetPosition( goal_parts[i] ), dGeomGetRotation( goal_parts[i] ), GOAL_SIDES[i] );
 
		if( i < 4 )   dsSetColor( 1.3, 1.3, 1.3 );
		else          dsSetColor( 0.0, 0.0, 1.3 );
		dsDrawBox( dGeomGetPosition( goal_parts2[i] ), dGeomGetRotation( goal_parts2[i] ), GOAL_SIDES[i] );
	}
/*		dReal sides0[3] = {0.1,2.2,1};
	dReal sides1[3] = {0.6,0.1,1};
	dsSetColor (1,1,0);
	dsDrawBox (dGeomGetPosition(immovablebox[0]),dGeomGetRotation(immovablebox[0]),sides0);
	dsDrawBox (dGeomGetPosition(immovablebox[2]),dGeomGetRotation(immovablebox[2]),sides1);
	dsDrawBox (dGeomGetPosition(immovablebox[4]),dGeomGetRotation(immovablebox[4]),sides1);
	dsSetColor (0,0,1);
	
	dsDrawBox (dGeomGetPosition(immovablebox[1]),dGeomGetRotation(immovablebox[1]),sides0);
	dsDrawBox (dGeomGetPosition(immovablebox[3]),dGeomGetRotation(immovablebox[3]),sides1);
	dsDrawBox (dGeomGetPosition(immovablebox[5]),dGeomGetRotation(immovablebox[5]),sides1);*/
}

void drawLine()
{
	dReal z = 0.01;
	dReal center_r = 2.0, corner_r = 0.75;
	dReal pos[][3] = {{9.0, 6.0, z},{9.0, -6.0, z},
					 {-9.0, -6.0, z},{-9.0, 6.0, z},{0.0,  6.0, z},
					 {0.0,  -6.0, z},
					 {9.0,   1.9, z},{ 8.1, 1.9, z},{8.1, -1.9, z}, {9.0, -1.9, z},
					 {-9.0,  1.9, z},{-8.1, 1.9, z},{-8.1,-1.9, z}, {-9.0,-1.9, z},
					 {9.0,   3.25, z},{ 6.75, 3.25, z},{6.75, -3.25, z}, {9.0, -3.25, z},
					 {-9.0,  3.25, z},{-6.75, 3.25, z},{-6.75,-3.25, z}, {-9.0,-3.25, z},};
	dsSetColor( 1.3, 1.3, 1.3 );
	dsDrawLine( pos[0], pos[1] );	dsDrawLine( pos[1], pos[2] );
	dsDrawLine( pos[2], pos[3] );	dsDrawLine( pos[3], pos[0] );
    dsDrawLine( pos[4], pos[5] );
	dsDrawLine( pos[6], pos[7] );   dsDrawLine( pos[7],  pos[8]  ); dsDrawLine( pos[8],  pos[9]  );
	dsDrawLine( pos[10], pos[11] ); dsDrawLine( pos[11], pos[12] ); dsDrawLine( pos[12], pos[13] );
	dsDrawLine( pos[14], pos[15] ); dsDrawLine( pos[15], pos[16] ); dsDrawLine( pos[16], pos[17] );
    dsDrawLine( pos[18], pos[19] ); dsDrawLine( pos[19], pos[20] ); dsDrawLine( pos[20], pos[21] );

	dReal pos_center[2] = {0, 0};
    dReal pos_corner[4][2] = {{-9, -6}, {-9, 6}, {9, 6}, {9, -6}};
    drawCircle( center_r, pos_center,      0,  360 );
	drawCircle( corner_r, pos_corner[0],   0,   90 );
    drawCircle( corner_r, pos_corner[1], -90,    0 );
	drawCircle( corner_r, pos_corner[3],  90,  180 );
	drawCircle( corner_r, pos_corner[2], -180, -90 );
}

void drawWall()
{
	dReal wall1[3] = {0.05, 15, 0.2};
	dReal wall2[3] = {22+0.05, 0.05, 0.2};
	dsSetColor( 0, 0, 0 );
	dsDrawBox( dGeomGetPosition( wall[0] ), dGeomGetRotation( wall[0] ), wall1 );
	dsDrawBox( dGeomGetPosition( wall[1] ), dGeomGetRotation( wall[1] ), wall1 );
	dsDrawBox( dGeomGetPosition( wall[2] ), dGeomGetRotation( wall[2] ), wall2 );
	dsDrawBox( dGeomGetPosition( wall[3] ), dGeomGetRotation( wall[3] ), wall2 );
}

void drawField()
{
	drawLine();
	drawGoal();
	drawPole();
	drawWall();
}

double Limit2PI(double ang)
{
  if(ang > M_PI) ang = ang -2.0*M_PI;
  else if(ang < -M_PI) ang = ang + 2.0*M_PI;
  return ang;
}

void drawBall()
{
	dsSetColor (1,0.4,0);
	dsDrawSphere(dGeomGetPosition(ball),dGeomGetRotation(ball),BALL_RADIUS);
}

 
void makeWall()
{
	int i;
	dMass m;

	for( i = 0; i <= 1; i++ ) 
	{
 		wall[i] = dCreateBox( space, 0.05, 14.0, 10.0 );
	}
	
	dGeomSetPosition( wall[0],  11.0, 0, 0.01 );
	dGeomSetPosition( wall[1], -11.0, 0, 0.01 );
	for( i = 2; i <= 3; i++ ) 
		wall[i] = dCreateBox( space, 21, 0.05, 10.0 );
	dGeomSetPosition( wall[2], 0, -7.5, 0.01 );
	dGeomSetPosition( wall[3], 0,  7.5, 0.01 );

 	dMatrix3 q1, q2;
	dRFromAxisAndAngle( q1, 0, 0, 1, -M_PI/4 );
	dRFromAxisAndAngle( q2, 0, 0, 1,  M_PI/4 );
	 
	for( i = 4; i <= 5; i++ ) 
		wall[i] = dCreateBox( space, 0.65, 0.05, 10.0 );
	dGeomSetRotation( wall[4], q1 );
	dGeomSetPosition( wall[4], 10.75,  6.75, 0.01 );
	dGeomSetRotation( wall[5], q2 );
	dGeomSetPosition( wall[5], 10.75, -6.75, 0.01 );

	for( i = 6; i <= 7; i++ )
		wall[i] = dCreateBox( space, 0.65, 0.05, 10.0 );
	dGeomSetRotation( wall[6], q1 );
	dGeomSetPosition( wall[6], -10.75, -6.75, 0.01 );
	dGeomSetRotation( wall[7], q2 );
	dGeomSetPosition( wall[7], -10.75,  6.75, 0.01 ); 
}
 
void makePole()
{
	for(int i = 0; i <= 11; i++ ) 
	   pole[i] = dCreateSphere( 0, POLERAD );
	dGeomSetPosition( pole[3],   9.5,  6.5,  0.75 );	
	dGeomSetPosition( pole[7],   9.5,  6.5,  0.45 );
	dGeomSetPosition( pole[2],   9.5,  6.5,  0.15 );

	dGeomSetPosition( pole[1],   9.5, -6.5,  0.75 );	
	dGeomSetPosition( pole[6],   9.5, -6.5,  0.45 );	
	dGeomSetPosition( pole[0],   9.5, -6.5,  0.15 );	

	dGeomSetPosition( pole[11], -9.5,  6.5,  0.75 );	
	dGeomSetPosition( pole[5],  -9.5,  6.5,  0.45 );	
	dGeomSetPosition( pole[10], -9.5,  6.5,  0.15 );	
	dGeomSetPosition( pole[8],  -9.5, -6.5,  0.75 );	
	dGeomSetPosition( pole[4],  -9.5, -6.5,  0.45 );	
	dGeomSetPosition( pole[9],  -9.5, -6.5,  0.15 );	
}

void makeGoal() 
{
	int i = 0;
  
    dReal pos[7][3] =											{{-9.3180, 1.0800, 0.5050},
		{-9.3180, -1.0800, 0.5050}, {-9.5900,  0.0000, 0.5050}, {-9.0625, 0.0000, 1.0700},
		{-9.2500,  1.0050, 0.5050}, {-9.2500, -1.0050, 0.5050}, {-9.5055, 0.0000, 0.5050}};

	for( i = 0; i < 7; i++ )
	{
		goal_parts[i] = dCreateBox(space, GOAL_SIDES[i][0],GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
		dGeomSetPosition(goal_parts[i],pos[i][0], pos[i][1], pos[i][2]);
	}
 
	dReal pos2[7][3] =                                     {{9.3180, 1.0800, 0.5050},
		{9.3180, -1.0800, 0.5050}, {9.5900,  0.0000, 0.5050}, {9.0625, 0.0000, 1.0700}, 
 		{9.2500,  1.0050, 0.5050}, {9.2500, -1.0050, 0.5050}, {9.5055, 0.0000, 0.5050}};

	for( i = 0; i < 7; i++ )
	{
		goal_parts2[i] = dCreateBox(space, GOAL_SIDES[i][0], GOAL_SIDES[i][1], GOAL_SIDES[i][2]);
		dGeomSetPosition(goal_parts2[i],pos2[i][0], pos2[i][1], pos2[i][2]);
	}
/*		
	immovablebox[0] = dCreateBox (space,0.1,2.2,1);//goal_backside
	immovablebox[1] = dCreateBox (space,0.1,2.2,1);//goal_backside
	immovablebox[2] = dCreateBox (space,0.6,0.1,1);//goal_backside
	immovablebox[3] = dCreateBox (space,0.6,0.1,1);//goal_backside
	immovablebox[4] = dCreateBox (space,0.6,0.1,1);//goal_backside
	immovablebox[5] = dCreateBox (space,0.6,0.1,1);//goal_backside
	dGeomSetBody (immovablebox[0],0);
	dGeomSetBody (immovablebox[1],0);
	dGeomSetBody (immovablebox[2],0);
	dGeomSetBody (immovablebox[3],0);
	dGeomSetBody (immovablebox[4],0);
	dGeomSetBody (immovablebox[5],0);
	dGeomSetPosition (immovablebox[0], -FIELD_LENGTH/2-0.6, 0, 0.3);
	dGeomSetPosition (immovablebox[1], FIELD_LENGTH/2+0.6, 0, 0.3);
	dGeomSetPosition (immovablebox[2], -FIELD_LENGTH/2-0.3, -1.1, 0.3);
	dGeomSetPosition (immovablebox[3], FIELD_LENGTH/2+0.3, -1.1, 0.3);
	dGeomSetPosition (immovablebox[4], -FIELD_LENGTH/2-0.3, 1.1, 0.3);
	dGeomSetPosition (immovablebox[5], FIELD_LENGTH/2+0.3, 1.1, 0.3);*/
}
  
void makeBall()
{
	dMass mass;
	dReal ball_m = 0.45, ball_r = BALL_RADIUS;
	dReal ball_x = 0, ball_y = 0.00, ball_z = 0.14;
	sphere    = dBodyCreate( world );  
	dMassSetZero( &mass );
	dMassSetSphereTotal( &mass, ball_m, ball_r );
	dBodySetMass( sphere, &mass );
	ball = dCreateSphere( space, ball_r );
	dGeomSetBody( ball, sphere );
	dBodySetPosition( sphere, ball_x, ball_y, ball_z );
	/* 
	dMass m;
	dMassAdjust(&m,0.4);
	sphere = dBodyCreate(world);
	dMassSetSphere(&m, 10.4, BALL_RADIUS);
	dBodySetMass(sphere,&m);
	dBodySetPosition (sphere,0,0,1);
	ball=dCreateSphere (space,BALL_RADIUS);
	dGeomSetBody (ball,sphere);*/

}

void makeField()
{
	makeGoal();
	makePole();
	makeWall();
}