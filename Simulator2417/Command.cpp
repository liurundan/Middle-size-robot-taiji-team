#include "stdafx.h"
#include "Command.h"


Command::Command( MessageBoard *msg, WorldModel *wm ):MSG2(msg),WM(wm)
{
  
}

Command::~Command()
{
  // delete MSG2;
  // delete WM;
}
  
void Command::moveTo( Vector pos )
{
    double dAngleDiff = ( pos - WM->getAgentGlobalPosition() ).getDirection();
     
	static double pos_e1 = 0;
	double pos_e = pos.getDistanceTo( WM->getAgentGlobalPosition() );
	
	double speed = 0.6 * ( 1.5 * pos_e + 1.5 * ( pos_e - pos_e1 ) ); 
	
	if( speed < 0 )    speed = 0;
	if( speed > 0.18 ) speed = 0.18; // 80cm/s  0.18cm/ms
	
 
	double vx = speed * cosDeg( Vector::normalizeAngle(dAngleDiff - WM->getAgentGlobalBodyAngle())  );
	double vy = speed * sinDeg( Vector::normalizeAngle(dAngleDiff - WM->getAgentGlobalBodyAngle())  );
	
	pos_e1 = pos_e; 
	speedData[6] = Vector(vx,vy);

}

void Command::turnTo( Vector pos )
{
   	double dAngleDiff = ( pos - WM->getAgentGlobalPosition() ).getDirection();

	static double theta_e1 = 0;
	double theta_e = Deg2Rad( Vector::normalizeAngle( dAngleDiff - WM->getAgentGlobalBodyAngle() )  );	
 
	double w = 1.0 * theta_e + 2.0 * ( theta_e - theta_e1 );
	
	if( fabs(w) > 0.15 ) // 150rad/s 0.15rad/ms
	{
		if( w > 0 ) 
			w =  0.15;
		else       
			w = -0.15;
	}
 	theta_e1 = theta_e;

	speedData[7] = Vector(w,0);

}

void Command::getBall( Vector pos )
{
    double pos_e = pos.getDistanceTo( WM->getAgentGlobalPosition() );
    double theta = ( pos - WM->getAgentGlobalPosition() ).getDirection();
    double theta_e =  Deg2Rad( theta - WM->getAgentGlobalBodyAngle() );	
 	double temp    = pos_e + fabs( theta_e ) * RADIUS;
	
	if( temp < 3.5 )
	{
		speedData[6] = 0;
		speedData[7] = 0;
	}
	else
	{
  	    turnTo(  pos );
  		moveTo(  pos );
	}
 
}

void Command::SetSpeed(  )
{
   MSG2->WriteData(speedData);
   Time t1;
   WM->MyInfo.setGlobalVelocity( speedData[6],t1);
   WM->MyInfo.setGlobalVelRotate(speedData[7],t1);
}

void Command::stop()
{
  speedData[6] = 0;
  speedData[7] = 0;
}

DriveVector Command::getDriveVector()
{
  DriveVector dv;
  dv.vtrans = speedData[6];
  dv.vrot   = speedData[7].X;
  return dv;
}
