#ifndef _AGENT_H_
#define _AGENT_H_

#include "WorldModel.h"
#include "Command.h"
#include "DisPlay.h"
#include "ErrorMinimiserWorldModel.h"
#include "MessageBoard.h"

class Agent
{
public:
	void reset(Vector p,MyAngle h);
	void reset();
	Agent( MessageBoard *msg, WorldModel *wm,Command *act,CDisplay *show );
	 ~Agent();
 	void GetInfo();
	void UpdateWorldModel();
	void DecideAction();
	void SendCommand();
	void ShowAgent();
	void Run();
protected:

private:
	WorldModel *WM;
	
	MessageBoard *MSG2;

	Command    *ACT;
	CDisplay   *SHOW;
	long     int iCycle;
	
	ScanResultList*  scanResults;
 	Frame2d FR;
    
	ErrorMinimiserWorldModel EWM;
	VisibleObjectList lines; 
    VisibleObjectList goals;
	
    VisibleObjectList Mylines; 
    
	// for test odometry error 
	Vector  odometrypos;
	MyAngle odometryheading;
};
#endif