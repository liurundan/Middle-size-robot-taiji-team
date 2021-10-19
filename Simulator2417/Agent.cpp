#include "stdafx.h"
#include "Agent.h"

#include <fstream>
#include <iomanip>

#define DEBUG_Agent_SCANRESULT 0
#define DEBUG_Agent_LINE 0
/*****************************************************************************/
/********************* CLASS Agent *************************************/
/*****************************************************************************/
 
Agent::Agent( MessageBoard *msg, WorldModel *wm,
			 Command *act,CDisplay *show ):MSG2(msg),WM(wm),ACT(act),SHOW(show)
{
  iCycle = 0;
  odometrypos     = 0;
  odometryheading = 0;
  WM->setSide(SIDE_BLUE);
  scanResults   = new ScanResultList(9);
}

Agent::~Agent()
{
  delete scanResults;
}
 
void Agent::GetInfo()
{
  	// 机器人接收视觉信息，解释信息
	Time time  = WM->getCurrentTime();
    int  cycle = WM->getCurrentCycle();
    
	/*here receive speed infomation*/
 	Vector newdata[10];
	scanResults->clear();
    lines.objectlist.clear();
	goals.objectlist.clear();
    Mylines.objectlist.clear();

    MSG2->ReadData( newdata,0, scanResults );
 	
	int i1; 
    for( i1=0; i1 < scanResults->results[COLOR_LINE]->points.size(); i1++ )
	{ 
         Vector poi  = scanResults->results[COLOR_LINE]->points[i1];
		 lines.objectlist.push_back(VisibleObject(poi,VisibleObject::white_line));
	}

    for( i1=0; i1 < scanResults->results[COLOR_BLUE]->points.size(); i1++ )
	{ 
         Vector poi  = scanResults->results[COLOR_BLUE]->points[i1];
		 goals.objectlist.push_back(VisibleObject(poi,VisibleObject::blue_goal));
	}
	for( i1=0; i1 < scanResults->results[COLOR_YELLOW]->points.size(); i1++ )
	{ 
         Vector poi  = scanResults->results[COLOR_YELLOW]->points[i1];
		 goals.objectlist.push_back(VisibleObject(poi,VisibleObject::yellow_goal));
	}

	MyTime now;
	lines.timestamp = now;
	goals.timestamp = now;
    EWM.set_visual_information(lines);
    EWM.set_visual_information(goals);
	
	// acquire odometry information
 	Vector  delta_pos  = newdata[6] * CONTROL_INTERVAL;
    MyAngle delta_ang  = MyAngle::deg_angle( newdata[7].getX() * CONTROL_INTERVAL );
    RobotLocation delta_xv;
	delta_xv.heading = delta_ang;
	delta_xv.pos     = delta_pos*10.0; // in mm
    EWM.set_odometry(delta_xv);
    
	DriveVector vel;
	vel.vtrans = newdata[6];
	vel.vrot   = newdata[7].X;
	EWM.set_odometry(vel,now);
	
	for( int i = 0; i < 6; i++ )
	{
	  ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
	  WM->Flags[i].setType            ( o                   );
	  WM->Flags[i].setRelativePosition( *(newdata+i),time );
	  WM->Flags[i].setTimeLastSeen    ( time                );
	  WM->Flags[i].setType            ( o                   );
	  WM->Flags[i].setRelativeposLastSeen( *(newdata+i), cycle );      
	}
    
	// odometry info 
	WM->MyInfo.setGlobalVelocity( newdata[6],time );
    WM->MyInfo.setGlobalVelRotate( newdata[7],time );
 /*	
	AngDeg  ang       = WM->MyInfo.BodyFacing;
	Vector  detal_pos = WM->MyInfo.getGlobalVelocity() * CONTROL_INTERVAL;
	AngDeg  detal_ang = WM->MyInfo.getGlobalVelRotate().getX() * CONTROL_INTERVAL;
    MyAngle detal_ang2 = MyAngle::deg_angle( Vector::normalizeAngle(detal_ang) );*/
 	
	WM->ball.setType( OBJECT_BALL );
	WM->ball.setRelativePosition( newdata[8],time );
 
}

void Agent::UpdateWorldModel()
{
    // 根据所获取的环境信息进行机器人自定位
	Time time  = WM->getCurrentTime();
    int  cycle = WM->getCurrentCycle();
	Vector      posGlobal,velGlobal,BallPos,Ballvel;
	AngDeg      angGlobal =0;
    
	//Vector deltaPos = WM->getAgentGlobalVelocity() * CONTROL_INTERVAL;
	//odometrypos     += deltaPos.rotate(odometryheading.get_deg_180());
	//odometryheading += MyAngle::deg_angle(WM->getAgentGlobalVelRotate().getX() * CONTROL_INTERVAL);
  	//WM->calculateStateAgent2( &posGlobal,&velGlobal,&angGlobal );
 

	EWM.update_localisation();
	MyTime time3;
	RobotLocation rloc = EWM.get_robot(time3);
	
	Vector  p = rloc.pos; 
	MyAngle h = rloc.heading; 

 //  WM->MyInfo.setGlobalPosition( odometrypos,time );
 //  WM->MyInfo.setGlobalBodyAngle( odometryheading.get_deg_180(),time );
 //  WM->MyInfo.setGlobalPosition( posGlobal,time );
 //  WM->MyInfo.setGlobalBodyAngle( angGlobal,time );
    
  	WM->MyInfo.setGlobalPosition( p*0.1,time );
  	WM->MyInfo.setGlobalBodyAngle( h.get_deg_180(),time );
	
	WM->calculateStateBall( &BallPos, &Ballvel );
    WM->ball.setGlobalPosition( BallPos,time );
    
	FR.set_position( p );
 	FR.set_angle(  h.get_deg_180() );
 //	FR.set_position( odometrypos*10 );
//	FR.set_angle(  odometryheading.get_deg_180() );
 //	FR.set_position( posGlobal*10 );
// 	FR.set_angle(  angGlobal );
  	Vector poi;
	MyTime time2;
  
 	for (int k = 0; k < lines.objectlist.size(); k++ )
	{
		Vector pos = lines.objectlist[k].pos;
		pos = FR * pos;
		Mylines.objectlist.push_back(pos);
	}
 	
}

void Agent::DecideAction()
{
  	// 根据场上形势和球队策略决定轮子速度
	Time time  = WM->getCurrentTime();
    int  cycle = WM->getCurrentCycle();
	Vector posAgent = WM->getAgentGlobalPosition();
	Vector posBall  = WM->getBallPos();
 	if( posAgent.getDistanceTo( posBall ) > 30 )//WM->isInField( WM->getAgentGlobalPosition(),-50 ) == TRUE
	  ACT->getBall( WM->getBallPos() );
	else
	  ACT->stop();
}

void Agent::SendCommand()
{
   ACT->SetSpeed();
   DriveVector dv = ACT->getDriveVector();
   MyTime now;
   EWM.set_drive_vector(dv,now);
}

void Agent::ShowAgent()
{
  SHOW->PlayerPosition[5]   = WM->getAgentGlobalPosition();
  SHOW->m_player_angle[5]   = WM->getAgentGlobalBodyAngle();
  SHOW->ballsimulatedpos    = WM->getBallPos();
  //SHOW->scanResults       = myscanResults;
  SHOW->lines               = Mylines;

  SHOW->Invalidate(FALSE);
}

void Agent::Run()
{
	iCycle++;
	WM->setCurrentCycle( iCycle );
	GetInfo();
	UpdateWorldModel();
	DecideAction();
	SendCommand();
	ShowAgent();
}

void Agent::reset()
{
    EWM.reset();
}

void Agent::reset(Vector p, MyAngle h)
{
  EWM.reset(-p*10,-h);
}

/*	
	#if DEBUG_Agent_SCANRESULT
	ofstream foo;
 	foo.open("scans.txt",ios::out|ios::app);
 	for( int i2=0;i2 < scanResults.results.size(); i2++ )
	{ 
     for (int j=0; j < scanResults.results[i2]->points.size();j++ ) 
	 {
        Vector poi = scanResults.results[i2]->points[j];
		foo<<setiosflags(ios::fixed);
	    foo<<"scan "<<i2<<": "<<setprecision(0)<<poi.X<<' '<<poi.Y<<'\n'; 
	 }
	}
   	foo.close();
    #endif
 	   
    #if DEBUG_Agent_LINE
	ofstream foo;
 	foo.open("lines.txt",ios::out|ios::app);
 	
//	for( int i2=0;i2 < lines.objectlist.size(); i2++ )
//	{ 
        Vector poi = lines.objectlist[0].pos;
		foo<<setiosflags(ios::fixed);
	    foo<<"line "<<0<<": "<<setprecision(0)<<poi.X<<' '<<poi.Y<<'\n'; 
//	}
   	foo.close();
    #endif*/