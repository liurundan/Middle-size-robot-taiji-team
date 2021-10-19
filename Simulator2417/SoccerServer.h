#ifndef _SOCCERSERVER_H_
#define _SOCCERSERVER_H_

#include "Objects.h"
#include "Display.h"
#include "FieldGeometry.h"
#include "LineScanning.h"
#include "MessageBoard.h"
  
class SoccerServer
{
protected:

private:
	Vector   AgentRealGlobalPos;
	AngDeg   AgentRealHeading;
    Vector   VelRealme;
    Vector   VelRotateRealme;
 	Vector   BallRealPos;
    
	Vector   motionData[10];
    Vector   visualData[10];
    
	FieldGeometry FG;
    RobotConf     RC;
 

//	ScanResultList  scanResults;
//  LineScanner     scanner;
    Frame2d  FR;

    ScanResultList*  scanResults;
//	ScanResultList*  globalResults;
   
    LineScanner*     myscanner;
    ColorClassInfoList*  colClass;
	 
	MessageBoard *MSG2;
	
	 CDisplay     *SHOW;

	int nScanLines   ;
    int innerRadius  ;
    int outerRadius  ;
public:
	void reset(Vector p,MyAngle h);
	void reset();
	void ShowIdealResult();
    SoccerServer( MessageBoard *msg, CDisplay *show );
	~SoccerServer();
 
    int      ReceiveMessage( );
	void     UpdateAgentState();
	void     VisualSensor();
	bool     SendVisualInfo( );
	void     Run();
	bool     SetAgentRealGlobalPos( Vector pos);
	bool     SetBallRealGlobalPos( Vector pos);
	bool     SetAgentRealHeading( AngDeg ang );
	Vector   GetAgentRealGlobalPos();
	AngDeg   GetAgentRealHeading();
	Vector   GetBallRealPos();
};
#endif