#ifndef _WORLD_MODEL_
#define _WORLD_MODEL_
#include "Objects.h"
#include "SoccerTypes.h"	// Added by ClassView
#include "Geometry.h"	// Added by ClassView

static const int iNrParticlesAgent = 100;

inline double rint(double x)  
{ 
   return floor(x+0.5);
} 

inline double drand48()//产生0－1之间的随机数，返回双精度或浮点型
{ 
  return ((double)(rand() % 100)) / 100;
} 
// inline sign( double x ) { return( (x) >= 0 ? 1 : -1 ); }

/*****************************************************************************/
/********************** CLASS WORLDMODEL *************************************/
/*****************************************************************************/
// 世界模型类包含了球场上所有有用的信息：球、球员、标记、边界线
// 而且也包含了能根据以上基本信息抽取更有用信息的方法
/*! The Class WorlModel contains all the RoboCup information that is available
    on the field. It contains information about the players, ball, flags and
    lines. Furthermore it contains methods to extract useful information.
    The (large amount of) attributes can be separated into different groups:
    - Environmental information: specific information about the soccer server//场上信息
    - Match information: general information about the current state of a match//比赛状态信息
    - Object information: all the objects on the soccer field//物体信息
    - Action information: actions that the agent has performed//动作信息

    The methods can also be divided into different groups:
    - Retrieval methods: directly retrieving information of objects//获取物体信息
    - Update methods: update world based on new sensory information//更新存储信息
    - Prediction methods: predict future states based on past perceptions//预测比赛局势
    - High-Level methods: deriving high-level conclusions from basic worldstate//做出决策判断
*/
class WorldModel  
{
public:
  ////////////////////////// OBJECTS //////////////////////////////////////////
	Player       MyInfo;
	Ball         ball;
	FixedObject  ourgoal;
	FixedObject  oppgoal;
    FixedObject  Lines[8];
	FixedObject  Flags[MAX_FLAGS];
	FixedObject  bluegoal;//ourgoal;
	FixedObject  yellowgoal;//oppgoal;
	Player       UnknownPlayers[12];
	Player       Teammates[6];
	Player       Opponents[6];
	Player       Obstacle;
    int          iNrUnknownPlayers;
// match information
 	BYTE         MatchMode;
	BYTE         MatchType;
	BYTE         MatchState;
	BYTE         OurColor;
    
    Vector       BallFromGlobal;
	Vector       Robot1FromGlobal;
	Vector       Robot2FromGlobal;
	Vector       Robot3FromGlobal;
	Vector       Robot4FromGlobal;
	Vector       Robot5FromGlobal;
	Vector       ImpedimentFromGlobal[61]; // biao
	BYTE         AttackNumber;
	BYTE         A_attackNumber1;
	BYTE         A_attackNumber2;
	BYTE         DefendNumber;
		
	int          MyNum;
    int          m_iMultY;
	int          m_iMultX; 
	double       elapsedTime;
	double       w;
	
 	double       BallConfTeam;
	double       BodyAngTeam;
	Vector       posTeam;
	Vector       posBallTeam;
public:
	bool isInRField( Vector pos = UnknownDoubleValue );
	bool isInLField( Vector pos = UnknownDoubleValue );
	bool isInRightBackField( Vector pos = UnknownDoubleValue );
	bool isInLeftBackField( Vector pos = UnknownDoubleValue );
	bool isBallCatchable();
	bool changePlayerTypes();
	SideT getSidePenalty( );
	bool setSidePenalty( SideT side );
	PlayerT getPlayerTypes                (             ) const;
	bool setPlayerTypes( int iPlayer=-1 );
	bool isOpponentAtAngle( AngDeg ang , double dDist );
	Vector predictAgentPos( int iCycles, Vector velocity );
	void predictStateAfterDash( Vector velocity, Vector *pos,Vector *vel);
	Vector predictFinalAgentPos(Vector *pos=NULL,Vector *vel=NULL);
	Vector predictPosAfterNrCycles( ObjectT o,double dCycles, Vector velocity = 80, Vector *posIn=NULL, 
		                            Vector *velIn=NULL, bool bUpdate=true );
	bool isBallHeadingToGoal();
	ObjectT getFastestInSetTo( ObjectT obj );
 // 获取位于特定区域的球员的数量
  // methods that return the number of players in a certain area of the field	
	bool         isEmptySpace( ObjectT obj, AngDeg ang, double dDist = 200.0 );
	int          getNrInSetInCone( ObjectSetT set, double dWidth,Vector start , Vector end );
	int          getNrInSetInRectangle( ObjectSetT set, Rect  *rect = NULL  );
	int          getNrInSetInCircle( ObjectSetT set, Circle c );
	Vector       getObstaclePositionOfFront();
	bool         isBlockedByFrontObject();
	ObjectT      way_blocked(Vector p);
  /***************************************************************************/
  /*************************** OPERATIONS ************************************/
  /***************************************************************************/ 
	
// get and set methods of attributes in WorldModel itself
	Time      getCurrentTime();
    long int  getCurrentCycle();
    bool      setCurrentCycle(int t);
    PlayModeT   getPlayMode( ) const;
	bool        setPlayMode( PlayModeT pm );
	SideT       getSide( ) const;
	bool        setSide( SideT s );
//    Time      getTimeLastSeen( ObjectT o );
//	Time      getTimeLastSeeMessage( ) const;
	bool      setTimeLastSeen( ObjectT o, Time time );
//	Time      getTimeLastRecvSeeMessage( ) const;
//	Time      getTimeGlobalVelocity( ObjectT o );
//	Time      getTimeGlobalPosition( ObjectT o );
//	Time      getTimeGlobalAngles( ObjectT o );
//	Time      getTimeGlobalPositionLastSee( ObjectT o );
//	Time      getTimeChangeInformation( ObjectT o );
	//bool    setTimeLastSeeGlobalMessage( Time time );
//	bool      setTimeLastSeeMessage( Time time );

 // 获取我自己的信息get and set methods for agent information
 	bool    setPlayerNumber( int i );
	int     getPlayerNumber( ) const;
	ObjectT getAgentObjectType( ) const;
	int     getAgentIndex( ) const;
	bool    setAgentObjectType( ObjectT o );
	AngDeg  getAgentGlobalBodyAngle(  );
	Vector  getAgentGlobalVelocity( ) const;
	Vector  getAgentGlobalVelRotate() const;
	double  getAgentSpeed( ) const;
	Vector  getAgentGlobalPosition( ) const;
 
 // get method for information about goals
	double  getRelAngleOpponentGoal();
	double  getRelDistanceOpponentGoal();
	Vector  getPosOwnGoal( );
	Vector  getPosOpponentGoal( );
 
 // 获取特定对象不同信息的函数get method for different information about a specific object
    double    getConfidence( ObjectT o);
 	bool      setIsKnownPlayer( ObjectT o, bool isKnownPlayer );
	AngDeg    getGlobalBodyAngleLastSee( ObjectT o );
	Vector    getGlobalVelocityLastSee( ObjectT o );
	Vector    getGlobalPositionLastSee( ObjectT o );
	bool      isKnownPlayer( ObjectT o );
	AngDeg    getGlobalBodyAngle( ObjectT o );
	AngDeg    getRelativeAngle( ObjectT o );
	Vector    getRelativePosition( ObjectT o );
	double    getRelativeDistance( ObjectT o );
	Vector    getGlobalVelocity( ObjectT o );

 	Vector    getGlobalPosition( ObjectT o );
 // 获取球的信息get methods for ball information
	AngDeg      getBallDirection();
	double      getBallSpeed();
	Vector      getBallPos();
 
////////////////////////// HIGH-LEVEL METHODS (WorldModelHighLevel.C) ///////
	ObjectT getClosestInSetTo( ObjectSetT objectSet, MyLine l,Vector pos1, Vector pos2, double *dDistToLine = NULL,double *dDistPos1To=NULL);
	ObjectT getFurthestInSetTo( ObjectSetT set, ObjectT objTarget,double *dDist= NULL, double dConfThrdConfThr = -1.0 );
	ObjectT getClosestInSetTo( ObjectSetT set, Vector pos,double *dDist= NULL,  double dConfThrdConfThr = -1.0 ,bool include_me = true );
	ObjectT getFurthestRelativeInSet( ObjectSetT set, double *dDist  );
 	ObjectT getSecondClosestRelativeInSet( ObjectSetT set, double *dDist = NULL);
	ObjectT getClosestRelativeInSet( ObjectSetT set, double *dDist = NULL );	
	
	bool isInOwnPenaltyArea( Vector pos );
	bool isInField( Vector pos, double dMargin = 10 );
	bool isBallKickable();
	bool isVisible( ObjectT o );	 
	Vector getDeadBallPosition(  );
	Vector getStrategicPosition( int iPlayer = -1 );

	
	double reldir_2me(double x, double y);
	double speedControl( int priority );

	double getConfidenceWithTime( ObjectT o);
	double getBallConfidence( Time time );

 ////////////////////////// UPDATE METHODS (WorldModelUpdate.C)///////////////
    bool calculateStateAgent2( Vector *posGlobal,Vector *velGlobal, AngDeg *angGlobal);
	AngDeg calculateAngleAgentWithPos( Vector pos );
	Vector calculatePosAgentWith2Flags( ObjectT objFlag1,ObjectT objFlag2 );
	bool calculateStateAgent( Vector *posGlobal,Vector *velGlobal, AngDeg *angGlobal );
	bool calculateStateBall( Vector *posGlobal,Vector *velGlobal );
	bool calculateStatePlayer( ObjectT o, Vector *posGlobal,Vector *velGlobal );
	bool calculateStateAgent3( Vector *posGlobal,Vector *velGlobal, AngDeg *angGlobal );

	void resampleParticlesAgent( int iLeft, Vector pos );
	Vector averageParticles( Vector posArray[], int iLength );
	void updateParticlesAgent( Vector vel, bool bAfterSense );
	int checkParticlesAgent( AngDeg angGlobalNeck  );
	void initParticlesAgent( AngDeg angGlobal );
	void initParticlesAgent( Vector posInitial );
	bool getMinMaxDistChange( double dOutput, double dDist,double *dMin, double *dMax, double x1, double xDist1, double xDist2);
	bool getMinMaxDirChange( double dOutput, double *dMin,double *dMax,   double x1);
	bool getMinMaxDistQuantizeValue( double dOutput, double *dMin,double *dMax,   double x1, double x2 );
	double invQuantizeMax( double  put, double step );
	double invQuantizeMin( double  put, double step );

	bool isDeadBallThem( PlayModeT pm = PM_ILLEGAL);
	bool isDeadBallUs( PlayModeT pm= PM_ILLEGAL );
	bool isKickInThem( PlayModeT pm = PM_ILLEGAL );
	bool isKickInUs( PlayModeT pm = PM_ILLEGAL );
	bool isCornerKickThem( PlayModeT pm = PM_ILLEGAL );
	bool isCornerKickUs( PlayModeT pm = PM_ILLEGAL );
	bool isFreeKickThem( PlayModeT pm = PM_ILLEGAL );
	bool isFreeKickUs( PlayModeT pm = PM_ILLEGAL );
	bool isDropBall( PlayModeT pm = PM_ILLEGAL );
 	bool isBallInOurPossesion( );
	bool isBeforeKickOff( PlayModeT pm = PM_ILLEGAL );

	void iterateObjectDone( int &iIndex );
	ObjectT iterateObjectNext(int& iIndex,ObjectSetT g, double dConf=-1.0,bool bForwardbForward = false);
	ObjectT iterateObjectStart(int& iIndex,ObjectSetT g,double dConf=-1.0,bool bForwardbForward = false);
	AngDeg getGlobalAngle( ObjectT o );
	bool coordinateWith( ObjectT obj );
	bool isPenaltyThem( PlayModeT pm= PM_ILLEGAL );
	bool isPenaltyUs( PlayModeT pm= PM_ILLEGAL );
	bool isGoalKickThem( PlayModeT pm= PM_ILLEGAL );
	bool isGoalKickUs( PlayModeT pm = PM_ILLEGAL);
	bool isKickOffThem( PlayModeT pm = PM_ILLEGAL);
	bool isKickOffUs( PlayModeT pm = PM_ILLEGAL);
 
	
 	WorldModel();
	virtual ~WorldModel();
private:
	PlayModeT    playMode;
	Object*      getObjectPtrFromType( ObjectT o );
	Time         timeLastRecvSeeMessage;
	Time         timeLastSeeMessage;
	bool         bNewInfo;
	long int      m_cycle;
    SideT        sideSide;
	
    Vector        particlesPosAgent[iNrParticlesAgent];  /*!< particles to store agent position      */
    Vector        particlesPosBall[iNrParticlesAgent];   /*! particles to store ball position       */
    Vector        particlesVelBall[iNrParticlesAgent];   /*! particles to store ball velocity       */
    PlayerT       MyType;
	SideT         m_sidePenalty;
};
#endif