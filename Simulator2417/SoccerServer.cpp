#include "stdafx.h"
#include "SoccerServer.h"
#include <fstream>
#include <iomanip>

#define DEBUG_SS 0
/*****************************************************************************/
/********************* CLASS SoccerServer *************************************/
/*****************************************************************************/
SoccerServer::SoccerServer( MessageBoard *msg, CDisplay *show ):MSG2(msg),SHOW(show)
{
 	AgentRealGlobalPos = Vector(0,0);
    AgentRealHeading   = 0;
 
	innerRadius = 500;
    outerRadius = 12000;
    nScanLines  = 80;

	VelRealme          = 0;
	VelRotateRealme    = 0;
 	BallRealPos        = Vector(drand48()*500+200,drand48()*100);
	SHOW->BallPosition      = BallRealPos;
    SHOW->PlayerPosition[8] = AgentRealGlobalPos;
	SHOW->m_player_angle[8] = AgentRealHeading;
	SHOW->innerRadius       = innerRadius;
	
	FieldGeometry* fg  = new FieldGeometry;
	ScanLines* mylines = new ScanLines(innerRadius,outerRadius,nScanLines);
	colClass           = new ColorClassInfoList;
	myscanner          = new LineScanner(mylines,colClass,fg);
	scanResults        = new ScanResultList(colClass->classList.size());
}

SoccerServer::~SoccerServer()
{
   delete colClass;
   delete myscanner;
   delete scanResults;
}
 
int SoccerServer::ReceiveMessage()
{
   MSG2->ReadData( motionData,0 );
   VelRealme = motionData[6];
   VelRotateRealme = motionData[7].getX();
   return 1;
}

void SoccerServer::UpdateAgentState()
{ 
	 // limit robot active region
    if( fabs( AgentRealGlobalPos.getX() ) > 1050 || fabs( AgentRealGlobalPos.getY() ) > 750  )
    {
       motionData[6] = 0;
	}
    // 假定在机器人平移的过程中，角度保持不变
    Vector vel = motionData[6];
 	Vector dv  = motionData[6].rotate(AgentRealHeading);
    AgentRealGlobalPos += dv * CONTROL_INTERVAL;

    AgentRealHeading = Vector::normalizeAngle( AgentRealHeading + motionData[7].getX() * CONTROL_INTERVAL );
 	VelRealme = vel;
	VelRotateRealme = motionData[7];
    
	if( AgentRealGlobalPos.getDistanceTo(BallRealPos) < 40 )  
 	  BallRealPos = Vector(urandom(-600,600),urandom(-400,400));
}

void SoccerServer::VisualSensor()
{
   scanResults->clear();
   
   FR.set_position(AgentRealGlobalPos*10.0);//
   FR.set_angle(AgentRealHeading);
    
   myscanner->scan(scanResults,FR);
   
   #if DEBUG_SS
	ofstream foo;
 	foo.open("lines.txt",ios::out|ios::app);
 	
	for( int i=0;i < scanResults.results.size(); i++ )
	{ 
     for (int j=0; j < scanResults.results[i]->points.size();j++ ) 
	 {
        Vector poi = scanResults.results[i]->points[j];
		foo<<setiosflags(ios::fixed);
	    foo<<"line "<<i<<": "<<setprecision(0)<<poi.X<<' '<<poi.Y<<'\n'; 
	 }
	}
 
  	foo.close();
  #endif
     // 计算在当前位姿下的理想观测信息
   Vector yellowgoalpos = SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_YELLOW,SIDE_BLUE).globalToRelative( 
	                          AgentRealGlobalPos, AgentRealHeading );
   Vector bluegoalpos    = SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_BLUE,SIDE_BLUE).globalToRelative( 
	                          AgentRealGlobalPos, AgentRealHeading );
   Vector Flag0pos = SoccerTypes::getGlobalPositionFlag(OBJECT_MARKPOLE_BLUE_0,SIDE_BLUE).globalToRelative(
	                          AgentRealGlobalPos, AgentRealHeading );
   Vector Flag1pos = SoccerTypes::getGlobalPositionFlag(OBJECT_MARKPOLE_BLUE_1,SIDE_BLUE).globalToRelative(
	                          AgentRealGlobalPos, AgentRealHeading );
   Vector Flag2pos = SoccerTypes::getGlobalPositionFlag(OBJECT_MARKPOLE_YELLOW_2,SIDE_BLUE).globalToRelative(
	                          AgentRealGlobalPos, AgentRealHeading );
   Vector Flag3pos = SoccerTypes::getGlobalPositionFlag(OBJECT_MARKPOLE_YELLOW_3,SIDE_BLUE).globalToRelative(
	                          AgentRealGlobalPos, AgentRealHeading );
   
   // ball relative information
   Vector BallPos =  BallRealPos;
   BallPos =  BallPos.globalToRelative( AgentRealGlobalPos, AgentRealHeading );
 
   double coeff_ball  = 0.05;
   double sigma_ball  = BallPos.getMagnitude() * coeff_ball;
   double noise_ballx = nrandom(BallPos.X,sigma_ball);	
   double noise_bally = nrandom(BallPos.Y,sigma_ball);
   visualData[8]      = Vector(noise_ballx,noise_bally);
 //visualData[8]  = Vector( CalcQuanDist(BallPos.getMagnitude(),QUANTIZESTEP), 
 //	                       Quantize( BallPos.getDirection(),1.0),POLAR );

   // 对理想观测信息进行加噪处理
   visualData[0] = Vector( CalcQuanDist( Flag0pos.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( Flag0pos.getDirection(),1.0), POLAR );
   visualData[1] = Vector( CalcQuanDist( Flag1pos.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( Flag1pos.getDirection(),1.0), POLAR );
   visualData[2] = Vector( CalcQuanDist( Flag2pos.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( Flag2pos.getDirection(),1.0), POLAR );
   visualData[3] = Vector( CalcQuanDist( Flag3pos.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( Flag3pos.getDirection(),1.0), POLAR );
 
   visualData[4] = Vector( CalcQuanDist( bluegoalpos.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( bluegoalpos.getDirection(),1.0), POLAR );
   visualData[5] = Vector( CalcQuanDist(yellowgoalpos.getMagnitude(),QUANTIZESTEP), 
	                       Quantize( yellowgoalpos.getDirection(),1.0),POLAR );
 
   // add odometry noise
    double coeff     = 0.2; 
    double coeff_ang = 0.2;
	double sigma     = VelRealme.getMagnitude() * coeff;
	double sigma_ang = VelRotateRealme.X * coeff_ang;
 	double noise_x   =  nrandom(VelRealme.X,sigma);	
    double noise_y   =  nrandom(VelRealme.Y,sigma);
    double noise_fhi =  nrandom(VelRotateRealme.X,sigma_ang);
    visualData[6]    =  Vector(noise_x,noise_y);
    visualData[7]    =  Vector(noise_fhi,0);
/*
	double noise_x   = Max( 1e-10,Min(nrandom(VelRealme.X,sigma),1e10) );	
    double noise_y   = Max( 1e-10,Min(nrandom(VelRealme.Y,sigma),1e10) );
    double noise_fhi = Max( 1e-10,Min(nrandom(VelRotateRealme.X,sigma_ang),1e10) );*/
  /* visualData[6] = Vector( CalcQuanDist( VelRealme.getMagnitude(), QUANTIZESTEP ),
	                       Quantize( VelRealme.getDirection(),1.0), POLAR );
    visualData[7] = Vector( VelRotateRealme.getX()*urandom(0.96,1.11),0  );// */  
  /* 
    visualData[6] =   VelRealme;
    visualData[7] =   VelRotateRealme;  */
}

bool SoccerServer::SendVisualInfo()
{
    // 将视觉信息存放到线程共享的临界区
   // 发送视觉消息给机器人
   MSG2->WriteData( visualData, scanResults );
   return true;
}

void SoccerServer::Run()
{
 	ReceiveMessage( );
 	UpdateAgentState();
	VisualSensor();
 	SendVisualInfo( );
 	ShowIdealResult();
}

bool SoccerServer::SetAgentRealGlobalPos( Vector pos )
{
    AgentRealGlobalPos = pos;
	return true;
}

bool SoccerServer::SetAgentRealHeading( AngDeg ang )
{
	AgentRealHeading = ang;
	return true;
}
bool SoccerServer::SetBallRealGlobalPos( Vector pos)
{
    BallRealPos = pos;
	return true;
}

Vector SoccerServer::GetBallRealPos()
{
   return BallRealPos;
}

Vector SoccerServer::GetAgentRealGlobalPos()
{
   return AgentRealGlobalPos;
}

AngDeg SoccerServer::GetAgentRealHeading()
{
   return AgentRealHeading;
}

void SoccerServer::ShowIdealResult()
{
   SHOW->PlayerPosition[8] = AgentRealGlobalPos;
   SHOW->m_player_angle[8] = AgentRealHeading;
   SHOW->BallPosition      = BallRealPos;
   SHOW->scanResults      = scanResults;
   SHOW->Invalidate(FALSE);
}

void SoccerServer::reset()
{
   	//AgentRealGlobalPos = Vector(-drand48()*500+200,-drand48()*100);
    //AgentRealHeading   = 0;
    AgentRealGlobalPos = Vector(urandom(-600,600),urandom(-400,400));
    AgentRealHeading   = urandom(-180,180);
	VelRealme          = 0;
	VelRotateRealme    = 0;
 	//BallRealPos        = Vector(drand48()*500+200,drand48()*100);
	BallRealPos        = Vector(urandom(-600,600),urandom(-400,400));

	SHOW->BallPosition      = BallRealPos;
    SHOW->PlayerPosition[8] = AgentRealGlobalPos;
	SHOW->m_player_angle[8] = AgentRealHeading;
}

void SoccerServer::reset(Vector p, MyAngle h)
{
    AgentRealGlobalPos = p;
    AgentRealHeading   = h.get_deg_180();
 
	VelRealme          = 0;
	VelRotateRealme    = 0;
 	BallRealPos        = Vector(drand48()*500+200,drand48()*100);
	SHOW->BallPosition      = BallRealPos;
    SHOW->PlayerPosition[8] = AgentRealGlobalPos;
	SHOW->m_player_angle[8] = AgentRealHeading;
}
