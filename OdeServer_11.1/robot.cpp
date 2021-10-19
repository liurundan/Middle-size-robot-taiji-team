#include "OdeServer.h"

void makeBase( int robot_num )
{
	int i;
	dMass MASS, mass;
	dReal M = 10.0;
	dReal m = 5.0;
  
	for( i = 0; i < robot_num; i++ )
	{
		dMassSetZero( &MASS );
		dMassSetBoxTotal( &MASS, M, ROBOT_BASE1[0], ROBOT_BASE1[1], ROBOT_BASE1[2] );
		dMassSetZero( &mass );
		dMassSetBoxTotal( &mass, m, ROBOT_BASE2[0], ROBOT_BASE2[1], ROBOT_BASE2[2] );

		Robot[i].body_base[0] = dBodyCreate( world );
		dBodySetMass( Robot[i].body_base[0], &MASS );
 //		Robot[i].geom_base[0] = dCreateBox( space, ROBOT_BASE1[0], ROBOT_BASE1[1] - 0.1, ROBOT_BASE1[2] );
 //		dGeomSetBody( Robot[i].geom_base[0], Robot[i].body_base[0] ); 
		dBodySetPosition( Robot[i].body_base[0], Robot_Formation_X[i] + (WIDTH_HM/2.0) + LENGTH_HM/ (2.0*root3), Robot_Formation_Y[i], Z );

		Robot[i].body_base[1]  = dBodyCreate( world );
		dBodySetMass( Robot[i].body_base[1], &MASS );
		Robot[i].geom_base[1] = dCreateBox( space, ROBOT_BASE1[0], ROBOT_BASE1[1] - 0.1, ROBOT_BASE1[2] );  
		dGeomSetBody( Robot[i].geom_base[1], Robot[i].body_base[1] );
		dMatrix3 q1;
		dRFromAxisAndAngle( q1, 0, 0, 1, -M_PI/3 );
		dBodySetRotation( Robot[i].body_base[1], q1 );
		dBodySetPosition( Robot[i].body_base[1], Robot_Formation_X[i] - ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * sin(M_PI/6),
		                  Robot_Formation_Y[i] + ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * cos(M_PI/6), Z );

	    Robot[i].body_base[2] = dBodyCreate( world );  
		dBodySetMass( Robot[i].body_base[2], &MASS );
		Robot[i].geom_base[2] = dCreateBox( space, ROBOT_BASE1[0], ROBOT_BASE1[1] - 0.1, ROBOT_BASE1[2] );	
		dGeomSetBody( Robot[i].geom_base[2], Robot[i].body_base[2] );
		dMatrix3 q2;
		dRFromAxisAndAngle( q2, 0, 0, 1, M_PI/3 );
		dBodySetRotation( Robot[i].body_base[2], q2 );
		dBodySetPosition( Robot[i].body_base[2], Robot_Formation_X[i] - ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * sin(M_PI/6),
		                  Robot_Formation_Y[i] - ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * cos(M_PI/6), Z );

		Robot[i].body_base[3] = dBodyCreate( world );
		dBodySetMass( Robot[i].body_base[3], &mass );
		dMatrix3 q3;
		Robot[i].geom_base[3] = dCreateBox( space, ROBOT_BASE2[0], ROBOT_BASE2[1], ROBOT_BASE2[2] );  
		dGeomSetBody( Robot[i].geom_base[3], Robot[i].body_base[3] );
		dRFromAxisAndAngle( q3, 0, 0, 1, -M_PI/3 );
		dBodySetRotation( Robot[i].body_base[3], q3 );
		dBodySetPosition( Robot[i].body_base[3], Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6) + HEIGHT_HM * root3 / 4.0,
						  Robot_Formation_Y[i] - LENGTH_HM / 2.0 + HEIGHT_HM / 4.0, Z );
	
		Robot[i].body_base[4] = dBodyCreate( world );
		dBodySetMass( Robot[i].body_base[4], &mass );
		dMatrix3 q4;
		Robot[i].geom_base[4] = dCreateBox( space, ROBOT_BASE2[0], ROBOT_BASE2[1], ROBOT_BASE2[2] );  
		dGeomSetBody( Robot[i].geom_base[4], Robot[i].body_base[4] );
		dRFromAxisAndAngle ( q4, 0, 0, 1, M_PI/3 );
		dBodySetRotation( Robot[i].body_base[4], q4 );
		dBodySetPosition( Robot[i].body_base[4], Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6) + HEIGHT_HM * root3 /4.0,
						  Robot_Formation_Y[i] + LENGTH_HM / 2.0 - HEIGHT_HM / 4.0, Z );


	    Robot[i].body_base[5]  = dBodyCreate( world );
		dMassSetBoxTotal( &mass, m, ROBOT_BASE3[0], ROBOT_BASE3[1], ROBOT_BASE3[2] );
		dBodySetMass( Robot[i].body_base[5], &mass );
		Robot[i].geom_base[5] = dCreateBox( space, ROBOT_BASE3[0], ROBOT_BASE3[1], ROBOT_BASE3[2] );  
	    dGeomSetBody( Robot[i].geom_base[5], Robot[i].body_base[5] );
		dBodySetPosition( Robot[i].body_base[5], Robot_Formation_X[i] - (LENGTH_HM*root3) / 3, Robot_Formation_Y[i], Z );

		Robot[i].body_base[6] = dBodyCreate( world );
		dMassSetBoxTotal( &mass, m, ROBOT_BASE4[0], ROBOT_BASE4[1], ROBOT_BASE4[2] );
		dBodySetMass( Robot[i].body_base[6], &mass );
	    Robot[i].geom_base[6] = dCreateBox( space, ROBOT_BASE4[0], ROBOT_BASE4[1], ROBOT_BASE4[2] );
		dGeomSetBody( Robot[i].geom_base[6], Robot[i].body_base[6]);
		dBodySetPosition( Robot[i].body_base[6], Robot_Formation_X[i] + WIDTH_HM + LENGTH_HM / (2.0*root3) + ROBOT_BASE4[0] / 2, 
						  Robot_Formation_Y[i] - LENGTH_HM / 2 + ROBOT_BASE4[1] + ROBOT_BASE4[1] / 2.0, Z);

		Robot[i].body_base[7] = dBodyCreate( world );
		dBodySetMass( Robot[i].body_base[7], &mass );
	    Robot[i].geom_base[7] = dCreateBox( space, ROBOT_BASE4[0], ROBOT_BASE4[1], ROBOT_BASE4[2] );
		dGeomSetBody( Robot[i].geom_base[7], Robot[i].body_base[7]);
		dBodySetPosition( Robot[i].body_base[7], Robot_Formation_X[i] + WIDTH_HM + LENGTH_HM / (2.0*root3) + ROBOT_BASE4[0] / 2, 
						  Robot_Formation_Y[i]+LENGTH_HM / 2 - ROBOT_BASE4[1] - ROBOT_BASE4[1] / 2.0, Z );

		Robot[i].joint_base[0] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[1] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[2] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[3] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[4] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[5] = dJointCreateFixed( world, 0 );
		Robot[i].joint_base[6] = dJointCreateFixed( world, 0 );
	    Robot[i].joint_base[7] = dJointCreateFixed( world, 0 );
		
		dJointAttach( Robot[i].joint_base[0], Robot[i].body_base[0], Robot[i].body_base[5] );
		dJointAttach( Robot[i].joint_base[1], Robot[i].body_base[1], Robot[i].body_base[3] );
		dJointAttach( Robot[i].joint_base[2], Robot[i].body_base[2], Robot[i].body_base[4] );
		dJointAttach( Robot[i].joint_base[3], Robot[i].body_base[3], Robot[i].body_base[0] );
		dJointAttach( Robot[i].joint_base[4], Robot[i].body_base[4], Robot[i].body_base[1] );
		dJointAttach( Robot[i].joint_base[5], Robot[i].body_base[5], Robot[i].body_base[2] );
	    dJointAttach( Robot[i].joint_base[6], Robot[i].body_base[6], Robot[i].body_base[0] );
		dJointAttach( Robot[i].joint_base[7], Robot[i].body_base[7], Robot[i].body_base[0] );

		dJointSetFixed( Robot[i].joint_base[0] );
		dJointSetFixed( Robot[i].joint_base[1] );
		dJointSetFixed( Robot[i].joint_base[2] );
		dJointSetFixed( Robot[i].joint_base[3] );
		dJointSetFixed( Robot[i].joint_base[4] );
		dJointSetFixed( Robot[i].joint_base[5] );
		dJointSetFixed( Robot[i].joint_base[6] );
  		dJointSetFixed( Robot[i].joint_base[7] );
	}
}


void drawBase( int robot_num )
{
	int i;

	for( i = 1; i < robot_num; i++ )
	{
		if ( (i/4)%2 )
			dsSetColor (3,3,3);
		else 
		    dsSetColor( 0.1, 0.1, 0.1 ); 

		dsDrawBox( dBodyGetPosition( Robot[i].body_base[0] ), dBodyGetRotation( Robot[i].body_base[0] ), ROBOT_BASE1 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[1] ), dBodyGetRotation( Robot[i].body_base[1] ), ROBOT_BASE1 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[2] ), dBodyGetRotation( Robot[i].body_base[2] ), ROBOT_BASE1 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[3] ), dBodyGetRotation( Robot[i].body_base[3] ), ROBOT_BASE2 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[4] ), dBodyGetRotation( Robot[i].body_base[4] ), ROBOT_BASE2 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[5] ), dBodyGetRotation( Robot[i].body_base[5] ), ROBOT_BASE3 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[6] ), dBodyGetRotation( Robot[i].body_base[6] ), ROBOT_BASE4 );	
		dsDrawBox( dBodyGetPosition( Robot[i].body_base[7] ), dBodyGetRotation( Robot[i].body_base[7] ), ROBOT_BASE4 );	
   	}
}


void drawPlayerBase()
{
	if ( viewchange %2 == 1 && viewmode %2 == 0 )
		dsSetColorAlpha(0.1,0.1,0.1,0.5);
	else
		dsSetColor(0.1,0.1,0.1); 
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[6] ), dBodyGetRotation( Robot[0].body_base[6] ), ROBOT_BASE4 );	
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[7] ), dBodyGetRotation( Robot[0].body_base[7] ), ROBOT_BASE4 );	
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[0] ), dBodyGetRotation( Robot[0].body_base[0] ), ROBOT_BASE1 );	
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[1] ), dBodyGetRotation( Robot[0].body_base[1] ), ROBOT_BASE1 );	
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[2] ), dBodyGetRotation( Robot[0].body_base[2] ), ROBOT_BASE1 );
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[3] ), dBodyGetRotation( Robot[0].body_base[3] ), ROBOT_BASE2 );	
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[4] ), dBodyGetRotation( Robot[0].body_base[4] ), ROBOT_BASE2 );
	dsDrawBox( dBodyGetPosition( Robot[0].body_base[5] ), dBodyGetRotation( Robot[0].body_base[5] ), ROBOT_BASE3 );	
}


void makeWheel( int robot_num ) 
{ 
	int i;
	dMass mass;
	dReal r = RADIUS; 
	dReal m = 4.0;
	dReal tmp_z = Z - HEIGHT_HM/2.0 - r;

	for( i = 0; i < robot_num; i++ )
	{
		dMassSetZero( &mass );
		dMassSetSphereTotal( &mass, m, r );

		Robot[i].body_wheel[0] = dBodyCreate( world );
		dMatrix3 R1;
		dRFromAxisAndAngle(R1, root3, 1, 0, M_PI/2.0 );
		dBodySetRotation( Robot[i].body_wheel[0], R1 );
		dBodySetMass( Robot[i].body_wheel[0], &mass );
		Robot[i].geom_wheel[0] = dCreateSphere( space, r );
		dGeomSetBody( Robot[i].geom_wheel[0], Robot[i].body_wheel[0] );
    
		Robot[i].body_wheel[1] = dBodyCreate( world );
		dMatrix3 R2;
		dRFromAxisAndAngle( R2, -root3, 1, 0, M_PI/2.0 );
		dBodySetRotation( Robot[i].body_wheel[1], R2 );
		dBodySetMass( Robot[i].body_wheel[1], &mass );
		Robot[i].geom_wheel[1] = dCreateSphere( space, r );
		dGeomSetBody( Robot[i].geom_wheel[1], Robot[i].body_wheel[1] );
    
		Robot[i].body_wheel[2] = dBodyCreate( world );
		dMatrix3 R3;
		dRFromAxisAndAngle( R3, 0, -1, 0, M_PI/2.0 );
		dBodySetRotation( Robot[i].body_wheel[2], R3 );
		dBodySetMass( Robot[i].body_wheel[2], &mass );
		Robot[i].geom_wheel[2] = dCreateSphere( space, r );
		dGeomSetBody( Robot[i].geom_wheel[2], Robot[i].body_wheel[2] );

		dBodySetPosition( Robot[i].body_wheel[0], 
					      Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6),
						  Robot_Formation_Y[i] - LENGTH_HM / 2.0,
						  tmp_z );
	
		dBodySetPosition( Robot[i].body_wheel[1], 
						  Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6),
						  Robot_Formation_Y[i] + LENGTH_HM / 2.0,
						  tmp_z );

		dBodySetPosition( Robot[i].body_wheel[2], 
						  Robot_Formation_X[i] - (LENGTH_HM*root3) / 3.0,
						  Robot_Formation_Y[i],
						  tmp_z );

		Robot[i].joint_wheel[0] = dJointCreateHinge( world, 0 );
		Robot[i].joint_wheel[1] = dJointCreateHinge( world, 0 );
		Robot[i].joint_wheel[2] = dJointCreateHinge( world, 0 );

 		dJointAttach( Robot[i].joint_wheel[0], Robot[i].body_base[3], Robot[i].body_wheel[0]);
		dJointAttach( Robot[i].joint_wheel[1], Robot[i].body_base[4], Robot[i].body_wheel[1]);
		dJointAttach( Robot[i].joint_wheel[2], Robot[i].body_base[5], Robot[i].body_wheel[2]); 

		dJointSetHingeAxis( Robot[i].joint_wheel[0],  1, -root3, 0 );
		dJointSetHingeAxis( Robot[i].joint_wheel[1],  1,  root3, 0 );
		dJointSetHingeAxis( Robot[i].joint_wheel[2], -1,      0, 0 );

		dJointSetHingeAnchor( Robot[i].joint_wheel[0], 
							  Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6),
							  Robot_Formation_Y[i] - LENGTH_HM / 2.0,
							  tmp_z );
		dJointSetHingeAnchor( Robot[i].joint_wheel[1],
							  Robot_Formation_X[i] + (LENGTH_HM/root3) * sin(M_PI/6),
							  Robot_Formation_Y[i] + LENGTH_HM / 2.0,
							  tmp_z );
		dJointSetHingeAnchor( Robot[i].joint_wheel[2], 
							  Robot_Formation_X[i] - (LENGTH_HM*root3) / 3.0,
							  Robot_Formation_Y[i],
							  tmp_z );

	}
}


void drawWheel( int robot_num ) 
{
	int i;
	dReal radius = RADIUS, length = 0.024;

	for ( i = 1; i < robot_num; i++ )
	{
		if ( (i/4)%2 )
			dsSetColor (1.5,1.5,1.5);
		else 
			dsSetColor( 0.9, 0.9, 0.9 ); 

		//dsSetColor( 0.9, 0.9, 0.9 );  
		dsDrawCylinder( dGeomGetPosition( Robot[i].geom_wheel[0] ), dGeomGetRotation( Robot[i].geom_wheel[0] ), length, radius ); // 右ホイール
		dsDrawCylinder( dGeomGetPosition( Robot[i].geom_wheel[1] ), dGeomGetRotation( Robot[i].geom_wheel[1] ), length, radius ); // 左
		dsDrawCylinder( dGeomGetPosition( Robot[i].geom_wheel[2] ), dGeomGetRotation( Robot[i].geom_wheel[2] ), length, radius ); // 後
	}
}


void drawPlayerWheel() 
{
	dReal radius = RADIUS, length = 0.024;

	if( viewchange %2 == 1 && viewmode %2 == 0 )
		dsSetColorAlpha( 0.9, 0.9, 0.9, 0.5);
	else
		dsSetColor( 0.9, 0.9, 0.9 );  

	dsDrawCylinder( dGeomGetPosition( Robot[0].geom_wheel[0] ), dGeomGetRotation( Robot[0].geom_wheel[0] ), length, radius ); // 右ホイール
	dsDrawCylinder( dGeomGetPosition( Robot[0].geom_wheel[1] ), dGeomGetRotation( Robot[0].geom_wheel[1] ), length, radius ); // 左
	dsDrawCylinder( dGeomGetPosition( Robot[0].geom_wheel[2] ), dGeomGetRotation( Robot[0].geom_wheel[2] ), length, radius ); // 後
}


void makeFrame( int robot_num )
{
	int i;
	dMass MASS;
	dReal M = 0.1;
  
	for( i = 0; i < robot_num; i++ )
	{
		dMassSetZero( &MASS );
		dMassSetBoxTotal( &MASS, M, ROBOT_FRAME[0], ROBOT_FRAME[1], ROBOT_FRAME[2] );

		Robot[i].body_frame[0]  = dBodyCreate( world );
		dBodySetMass( Robot[i].body_frame[0], &MASS );
	    //Robot[i].geom_frame[0] = dCreateBox( space, ROBOT_FRAME[0], ROBOT_FRAME[1], ROBOT_FRAME[2] );
		//dGeomSetBody( Robot[i].geom_frame[0], Robot[i].body_frame[0] ); 
		dMatrix3 R1,R2,R3;
	    dRFromAxisAndAngle( R1, 0, 0, 1, -M_PI/3.0 );
		dRFromAxisAndAngle( R2, 0, 1, 0, -M_PI/6.0 );
		dMultiply0( R3, R1, R2, 3, 3, 3 );
		dBodySetRotation( Robot[i].body_frame[0], R3 );
		dBodySetPosition( Robot[i].body_frame[0], Robot_Formation_X[i] + ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * sin(M_PI/6.0),
						  Robot_Formation_Y[i] - ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * cos(M_PI/6.0), 0.035 + Z + (LENGTH_HM/4.0) * root3 );

	    Robot[i].body_frame[1]  = dBodyCreate( world );
		dBodySetMass( Robot[i].body_frame[1], &MASS );
		//Robot[i].geom_frame[1] = dCreateBox( space, ROBOT_FRAME[0], ROBOT_FRAME[1], ROBOT_FRAME[2] );
		//dGeomSetBody( Robot[i].geom_frame[1], Robot[i].body_frame[1] ); 
		dMatrix3 R4,R5,R6;
		dRFromAxisAndAngle( R4, 0, 0, 1,  M_PI/3.0 );
		dRFromAxisAndAngle( R5, 0, 1, 0, -M_PI/6.0);
	    dMultiply0(R6, R4, R5, 3, 3, 3 );
		dBodySetRotation( Robot[i].body_frame[1], R6 );
		dBodySetPosition( Robot[i].body_frame[1], Robot_Formation_X[i] + ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * sin(M_PI/6.0),
						  Robot_Formation_Y[i] + ( (WIDTH_HM/2.0) + LENGTH_HM / (2.0*root3) ) * cos(M_PI/6.0), 0.035 + Z + (LENGTH_HM/4.0) * root3 );

	    Robot[i].body_frame[2]  = dBodyCreate( world );  
		dBodySetMass( Robot[i].body_frame[2], &MASS );
		//Robot[i].geom_frame[2] = dCreateBox( space, ROBOT_FRAME[0], ROBOT_FRAME[1], ROBOT_FRAME[2] );	
	    //dGeomSetBody( Robot[i].geom_frame[2], Robot[i].body_frame[2] );
		dMatrix3 R7;
		dRFromAxisAndAngle( R7, 0, 1, 0, M_PI/6.0 );
		dBodySetRotation( Robot[i].body_frame[2], R7 );
	    dBodySetPosition( Robot[i].body_frame[2], Robot_Formation_X[i] - LENGTH_HM/3.0, Robot_Formation_Y[i], 0.035 + Z + (LENGTH_HM/4.0) * root3 );

		Robot[i].joint_frame[0] = dJointCreateFixed( world, 0 );
		Robot[i].joint_frame[1] = dJointCreateFixed( world, 0 );
		Robot[i].joint_frame[2] = dJointCreateFixed( world, 0 );

		dJointAttach( Robot[i].joint_frame[0], Robot[i].body_frame[0], Robot[i].body_base[3] );
		dJointAttach( Robot[i].joint_frame[1], Robot[i].body_frame[1], Robot[i].body_base[4] );
		dJointAttach( Robot[i].joint_frame[2], Robot[i].body_frame[2], Robot[i].body_base[5] );

		dJointSetFixed( Robot[i].joint_frame[0] );
		dJointSetFixed( Robot[i].joint_frame[1] );
		dJointSetFixed (Robot[i].joint_frame[2] );
	}
}


void drawFrame( int robot_num )
{
	int i;

	for( i = 1; i < robot_num; i++ )
	{
		if ( (i/4)%2 )
			dsSetColor (3,3,3);
		else 
		    dsSetColor( 0.1, 0.1, 0.1 ); 

		//dsSetColor( 0.1, 0.1, 0.1 ); 
		dsDrawBox( dBodyGetPosition( Robot[i].body_frame[0] ), dBodyGetRotation( Robot[i].body_frame[0] ), ROBOT_FRAME );
		dsDrawBox( dBodyGetPosition( Robot[i].body_frame[1] ), dBodyGetRotation( Robot[i].body_frame[1] ), ROBOT_FRAME );
		dsDrawBox( dBodyGetPosition( Robot[i].body_frame[2] ), dBodyGetRotation( Robot[i].body_frame[2] ), ROBOT_FRAME );
	}
}

void drawPlayerFrame() 
{
	if (viewchange %2 ==1 && viewmode %2 == 0)
	{
		dsSetColorAlpha( 0.1, 0.1, 0.1, 0.5 );
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[0] ), dBodyGetRotation( Robot[0].body_frame[0] ), ROBOT_FRAME );
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[1] ), dBodyGetRotation( Robot[0].body_frame[1] ), ROBOT_FRAME );
		dsSetColorAlpha( 0.1, 0.1, 0.1, 0.1 );
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[2] ), dBodyGetRotation( Robot[0].body_frame[2] ), ROBOT_FRAME );

	}
	else
	{
		dsSetColor( 0.1, 0.1, 0.1 ); 
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[0] ), dBodyGetRotation( Robot[0].body_frame[0] ), ROBOT_FRAME );
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[1] ), dBodyGetRotation( Robot[0].body_frame[1] ), ROBOT_FRAME );
		dsDrawBox( dBodyGetPosition( Robot[0].body_frame[2] ), dBodyGetRotation( Robot[0].body_frame[2] ), ROBOT_FRAME );
	}
}

void makeCore( int robot_num )
{
	int i;
	dMass MASS, mass;
	dReal M = 0.1;
  
	for( i = 0; i < robot_num; i++ )
	{
		dMassSetZero( &MASS );
		dMassSetBoxTotal( &MASS, M, ROBOT_CORE[0], ROBOT_CORE[1]/2.0, ROBOT_CORE[2] );
		dMassSetSphereTotal( &MASS, M, CORE_RAD );

		Robot[i].body_core[0]  = dBodyCreate( world );
		dBodySetMass( Robot[i].body_core[0], &MASS );
		//if( i != 1 )
		{
			Robot[i].geom_core[0] = dCreateBox( space,  ROBOT_CORE[0], ROBOT_CORE[1], ROBOT_CORE[2] );
			dGeomSetBody( Robot[i].geom_core[0], Robot[i].body_core[0] ); 
		}	
		dBodySetPosition( Robot[i].body_core[0], Robot_Formation_X[i], Robot_Formation_Y[i], Z + ROBOT_CORE[2] / 2.0 );
		Robot[i].joint_core[0] = dJointCreateFixed( world, 0 );
		dJointAttach( Robot[i].joint_core[0],Robot[i].body_core[0], Robot[i].body_base[0] );
		dJointSetFixed( Robot[i].joint_core[0] );
/*
		Robot[i].body_core[1]  = dBodyCreate( world );
		dBodySetMass( Robot[i].body_core[1], &MASS );
		Robot[i].geom_core[1] = dCreateSphere( space,  CORE_RAD );
		dGeomSetBody( Robot[i].geom_core[1], Robot[i].body_core[1] );
		dBodySetPosition( Robot[i].body_core[1], Robot_Formation_X[i] + 0.02, Robot_Formation_Y[i], Z );
		Robot[i].joint_core[1] = dJointCreateFixed( world, 0 );
		dJointAttach( Robot[i].joint_core[1],Robot[i].body_core[1], Robot[i].body_core[0] );
		dJointSetFixed( Robot[i].joint_core[1] );
*/
	}
}


void drawCore( int robot_num )
{
	int i;

	for( i = 1; i < robot_num; i++ )
	{
		if ( (i/4)%2 )
			dsSetColor (3,3,3);
		else 
		    dsSetColor( 0.1, 0.1, 0.1 ); 
		dsDrawBox( dBodyGetPosition( Robot[i].body_core[0] ), dBodyGetRotation( Robot[i].body_core[0] ), ROBOT_CORE );
		if( i == 1 || i == 7 ) dsDrawBox( dBodyGetPosition( Robot[i].body_core[0] ), dBodyGetRotation( Robot[i].body_core[0] ), ROBOT_GOALIE );
	}
}


void drawPlayerCore()
{
	if( viewchange %2 == 1 && viewmode %2 == 0 )
		dsSetColorAlpha( 0.1, 0.1, 0.1, 0.5 );
	else
		dsSetColor( 0.1, 0.1, 0.1 ); 

	dsDrawBox( dBodyGetPosition( Robot[0].body_core[0] ), dBodyGetRotation( Robot[0].body_core[0] ), ROBOT_CORE );

	//dsSetColorAlpha( 0.1, 0.1, 0.1, 0.5 );
	//dsDrawSphere( dBodyGetPosition( Robot[0].body_core[1] ), dBodyGetRotation( Robot[0].body_core[1] ), CORE_RAD );

}


void makeHead( int robot_num )
{
	int i;
	dMass mass;
	dReal r = 0.10; 
	dReal w = 0.15; 
	dReal m = 0.05; 

	for( i = 0; i < robot_num; i++ )
	{
		dMassSetZero( &mass );
		dMassSetSphereTotal( &mass, m, r);

		Robot[i].body_head[0] = dBodyCreate( world );
		dBodySetMass( Robot[i].body_head[0], &mass );
		//if( i != 1 )
		{
			Robot[i].geom_head[0] = dCreateSphere( space ,r );
			dGeomSetBody( Robot[i].geom_head[0], Robot[i].body_head[0] );
		}
		dBodySetPosition( Robot[i].body_head[0], Robot_Formation_X[i], Robot_Formation_Y[i], 0.60 );
  
		Robot[i].joint_head[0] = dJointCreateFixed(world,0);
		dJointAttach( Robot[i].joint_head[0], Robot[i].body_head[0], Robot[i].body_core[0] );
		dJointSetFixed( Robot[i].joint_head[0] );
  }
}


void drawHead( int robot_num ) 
{
	int i;
    dReal radius = 0.08, length = 0.15;

	for( i = 1; i < robot_num; i++ )
	{
		if ( (i/4)%2 )
			dsSetColor (3,3,3);
		else 
		    dsSetColor( 0.1, 0.1, 0.1 ); 
		dsDrawCylinder( dBodyGetPosition( Robot[i].body_head[0] ), dBodyGetRotation( Robot[i].body_head[0] ), length, radius );
	}
}


void drawPlayerHead() 
{
    dReal radius = 0.08, length = 0.15;

	if( viewchange %2 == 1 && viewmode %2 == 0  )//&& now_boosting == 1
		dsSetColorAlpha( 1.0, 0.0, 0.0, 0.65 ); 
	else if( viewchange %2 == 1 && viewmode %2 == 0 && simulation!=1  )//&& now_boosting == 0
		dsSetColorAlpha( 0.1, 0.1, 0.1, 0.5 ); 
	else if( viewchange %2 ==1 && viewmode %2 == 0 && simulation == 1 )
		dsSetColorAlpha( 0.0, 0.0, 1.0, 0.65 ); 
	else
		dsSetColor( 0.1, 0.1, 0.1 ); 

	dsDrawCylinder( dGeomGetPosition( Robot[0].geom_head[0] ), dGeomGetRotation( Robot[0].geom_head[0] ), length, radius );
}

void makeRobot( int robot_num )
{
 	makeBase( robot_num );
	makeWheel( robot_num );
 	makeFrame( robot_num );
	makeCore( robot_num );
 	makeHead( robot_num );
	dMass m;
	for (int i=0; i<robot_num; i++) 
	{
	/*	body[i] = dBodyCreate (world);
		dMassSetBox (&m,1,LENGTH,LENGTH,HEIGHT);
		dMassAdjust (&m,MASS_R);
		dBodySetMass (body[i],&m);
		dBodySetPosition (body[i],i*2.2*LENGTH+1,i/4*2.2*LENGTH+1,HEIGHT*0.5);
		
		box[i] = dCreateBox (space,LENGTH,LENGTH,HEIGHT);
		dGeomSetBody (box[i],body[i]);*/
		 body[i] = Robot[i].body_core[0];
		 box[i]  = Robot[i].geom_core[0];
	}
}


void drawRobot( int robot_num )
{
	drawBase( robot_num );
	drawWheel( robot_num );
	drawFrame( robot_num );
	drawCore( robot_num );
	drawHead( robot_num );
/*
	dReal sides[3] = {LENGTH,LENGTH,HEIGHT};
	dReal headsides[3] = {LENGTH*0.9,LENGTH*0.3f,HEIGHT*1.1f};
	for (int i=0; i<robot_num; i++)
	{
		dsSetColor (1,0,1);
		if ( (i/4)%2 )
			dsSetColor (1,0,1);
		else 
			dsSetColor (0,1,0);
 		
		dsDrawBox (dGeomGetPosition(box[i]),dGeomGetRotation(box[i]),
			sides);
		
		dsSetColor (1,1,0);
		// Draw heading of the active robot 
		if(i==activerobot) 
			dsSetColor(1,0,0);
		dsDrawBox (dGeomGetPosition(box[i]),dGeomGetRotation(box[i]),
			headsides);
		
    } */
}


void drawPlayer()
{
	drawPlayerBase();
	drawPlayerHead();
	drawPlayerWheel();
	drawPlayerCore();
	drawPlayerFrame();
}


void destroyRobot( int robot_num )
{
	int i, j;

	for( i = 0; i < robot_num; i++ )
	{

		for( j = 0; j < 8; j++ )
		{
			dJointDestroy( Robot[i].joint_base[j] );
		}
		for( j = 0; j < 3; j++)
		{
			dJointDestroy( Robot[i].joint_wheel[j] );
		}
		for( j = 0; j < 3; j++ )
		{
			dJointDestroy( Robot[i].joint_frame[j] );
		}
		for( j = 0; j < 1; j++ )
		{
			dJointDestroy( Robot[i].joint_core[j] );
		}
		dJointDestroy( Robot[i].joint_head[0] );

		for( j = 0; j < 8; j++ )
		{
			dBodyDestroy( Robot[i].body_base[j] );
		}
		for( j = 0; j < 3; j++ )
		{
			dBodyDestroy( Robot[i].body_wheel[j] );
		}
		for( j = 0; j < 3; j++ )
		{
			dBodyDestroy( Robot[i].body_frame[j] );
		}
		for( j = 0; j < 1; j++ )
		{
			dBodyDestroy( Robot[i].body_core[j] );
		}
			dBodyDestroy( Robot[i].body_head[0] );

		for( j = 1; j < 8; j++ )
		{
			dGeomDestroy( Robot[i].geom_base[j] );
		}
		for( j = 0; j < 3; j++ )
		{
			dGeomDestroy( Robot[i].geom_wheel[j] );
		}
		//if ( i != 1 ) 
		{
			dGeomDestroy( Robot[i].geom_core[0] );
			dGeomDestroy( Robot[i].geom_head[0] );
		}
	}
}

hyper myGetTickCount(void)
{
	static BOOL firstTime=TRUE;
	static double count_to_msec;			 
	LARGE_INTEGER Counter;			 
	
	if(firstTime) 
	{
		LARGE_INTEGER Freq;				 
		if(!QueryPerformanceFrequency(&Freq)) 
		{
			printf("\nNo high frequency timer awailable! Abort.\n");
			exit(0);
		}
		count_to_msec = ( 1000.0 )/Freq.u.LowPart;  
		firstTime = FALSE;
		
	}
	
	QueryPerformanceCounter(&Counter);
	return (hyper)(Counter.QuadPart * count_to_msec);
}