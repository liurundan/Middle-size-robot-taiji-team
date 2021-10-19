#include "stdafx.h"
#include "WorldModel.h"

 
#include "Mmsystem.h"
 
////////////////////////////////////////////////////

////////////////////////////////////////////////////
/////////////////____WorldModel_____////////////////
////////////////////////////////////////////////////
WorldModel::WorldModel():MyInfo()
{
  bNewInfo       = false;
  ball.setType ( OBJECT_BALL    );
  MyInfo.setType( OBJECT_ILLEGAL );
  setPlayMode( PM_BEFORE_KICK_OFF );
  setSide( SIDE_ILLEGAL );
  int i;
  for( i = 0; i < OURTEAMNUM ; i ++ )
    Teammates[i].setType( SoccerTypes::getTeammateObjectFromIndex( i ) );
  for( i = 0; i < OPPTEAMNUM ; i ++ )
    Opponents[i].setType( SoccerTypes::getOpponentObjectFromIndex( i ) );
  for( i = 0; i < OPPTEAMNUM + OURTEAMNUM ; i ++ )
    UnknownPlayers[i].setType( OBJECT_ILLEGAL );
  for( i = 0; i < MAX_FLAGS; i ++ )
    Flags[i].setType( OBJECT_ILLEGAL );
  for( i = 0; i < 8; i ++ )
    Lines[i].setType( OBJECT_ILLEGAL );
  Obstacle.setType( OBJECT_ILLEGAL );
  iNrUnknownPlayers = 0;
  BallFromGlobal    = UnknownDoubleValue;
  Robot1FromGlobal  = UnknownDoubleValue;
  Robot2FromGlobal  = UnknownDoubleValue;
  Robot3FromGlobal  = UnknownDoubleValue;
  Robot4FromGlobal  = UnknownDoubleValue;
  Robot5FromGlobal  = UnknownDoubleValue;
  A_attackNumber1   = -1;
  A_attackNumber2   = -1;
  AttackNumber      = -1;
}

WorldModel::~WorldModel()
{

}
Object* WorldModel::getObjectPtrFromType(ObjectT o)
{
  Object *object = NULL;
  if( o == OBJECT_ILLEGAL )
    return NULL;
  if( SoccerTypes::isKnownPlayer( o ) )
  {
    if( o == MyInfo.getType() )
      object = &MyInfo;
    else if( SoccerTypes::isTeammate( o ) )
      object = &Teammates[ SoccerTypes::getIndex(o) ];
    else
      object = &Opponents[ SoccerTypes::getIndex(o) ];
  }
  else if( SoccerTypes::isFlag( o ) )
  {
 	  object = &Flags[SoccerTypes::getIndex(o)];
  }
  else if( SoccerTypes::isLine( o ) )
    object = &Lines[SoccerTypes::getIndex(o)];
  else if( SoccerTypes::isBall( o ) )
	  object = &ball;
  else 
      object = &UnknownPlayers[ SoccerTypes::getIndex(o) ];
 return object;

}

bool WorldModel::setSide(SideT s)
{
  sideSide = s;
  m_iMultX = (getSide() == SIDE_YELLOW ) ?  1 : -1 ;    // set the draw info
  m_iMultY = (getSide() == SIDE_YELLOW ) ?  1 : -1 ;    // from Logger.C
  return true;
}

SideT WorldModel::getSide() const
{
 return sideSide;
}

bool WorldModel::setPlayMode(PlayModeT pm)
{
  playMode = pm;
  return true;
}

PlayModeT WorldModel::getPlayMode() const
{
 return playMode;
}

Vector WorldModel::getBallPos()
{
 return getGlobalPosition( OBJECT_BALL );
}

double WorldModel::getBallSpeed()
{
 return ball.getGlobalVelocity().getMagnitude();
}

Vector WorldModel::getGlobalPosition(ObjectT o)
{
 Object *object = getObjectPtrFromType( o );
  if( object != NULL )
  {
    if( SoccerTypes::isFlag( o ) || SoccerTypes::isGoal( o ) )
      return SoccerTypes::getGlobalPositionFlag( o, getSide() );
    else
      return object->getGlobalPosition();
  }
  return Vector( UnknownDoubleValue, UnknownDoubleValue);
}
 
AngDeg WorldModel::getBallDirection()
{
  return ball.getGlobalVelocity().getDirection();
}
 /*
Time WorldModel::getTimeGlobalPosition(ObjectT o)
{
 Player *object = (Player*) getObjectPtrFromType( o );
  if( object != NULL )
    return object->getTimeGlobalPosition();
  return UnknownTime;
}

Time WorldModel::getTimeGlobalVelocity(ObjectT o)
{
 Player  *object = (Player*) getObjectPtrFromType( o );
  if( object != NULL )
    return object->getTimeGlobalVelocity();
  return UnknownTime;
}

Time WorldModel::getTimeLastRecvSeeMessage() const
{
  return timeLastRecvSeeMessage;
}
*/
Vector WorldModel::getGlobalVelocity(ObjectT o) 
{
 DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
  if( object != NULL )
    return object->getGlobalVelocity(  );
  return Vector( UnknownDoubleValue, UnknownDoubleValue );
}

double WorldModel::getRelativeDistance(ObjectT o)
{
  Object *object = getObjectPtrFromType( o );
  if( object != NULL )
    return object->getRelativeDistance();
  return UnknownDoubleValue;
}

Vector WorldModel::getRelativePosition(ObjectT o)
{
  Object *object = getObjectPtrFromType( o );
  if( object != NULL )
    return object->getRelativePosition();
  return Vector(UnknownDoubleValue, UnknownDoubleValue);
}
 
AngDeg WorldModel::getRelativeAngle(ObjectT o)
{
 Object *object = getObjectPtrFromType( o );
   
  if( object != NULL )
  {
     return Vector::normalizeAngle( object->getRelativeAngle() );
  }
  return UnknownDoubleValue;
}
/*
Time WorldModel::getTimeGlobalAngles(ObjectT o)
{
 Player  *object = (Player*) getObjectPtrFromType( o );
  if( object != NULL )
    return object->getTimeGlobalAngles();
  return Time( -1);
}*/

AngDeg WorldModel::getGlobalBodyAngle(ObjectT o)
{
 Player  *object = (Player*) getObjectPtrFromType( o );
  if( object != NULL )
    return object->getGlobalBodyAngle();
  return UnknownAngleValue;
}

bool WorldModel::isKnownPlayer(ObjectT o)
{
 Player  *object = (Player  *)getObjectPtrFromType( o );
  if( object != NULL  )
    return object->getIsKnownPlayer();
  return false;
}
/*
Time WorldModel::getTimeChangeInformation(ObjectT o) // useless
{
 DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
  if( object != NULL )
    return object->getTimeChangeInformation(  );
  return Time( -1);
}*/
 
Vector WorldModel::getGlobalPositionLastSee(ObjectT o)
{
  DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
  if( object != NULL )
    return object->getGlobalPositionLastSee(  );
  return Vector( UnknownDoubleValue, UnknownDoubleValue);
}
/*
Time WorldModel::getTimeGlobalPositionLastSee(ObjectT o) // get cycle
{
 DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
  
  return UnknownTime;
}*/
 
Vector WorldModel::getGlobalVelocityLastSee(ObjectT o)
{
  DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
  if( object != NULL )
    return object->getGlobalVelocityLastSee(  );
  return Vector( UnknownDoubleValue, UnknownDoubleValue);
}

AngDeg WorldModel::getGlobalBodyAngleLastSee(ObjectT o)
{
 Player  *object = (Player*)getObjectPtrFromType( o );
  if( object != NULL )
    return object->getGlobalBodyAngleLastSee(  );
  return UnknownAngleValue;
}

bool WorldModel::setIsKnownPlayer(ObjectT o, bool isKnownPlayer)
{
  Player  *object = (Player*) getObjectPtrFromType( o );
  return object->setIsKnownPlayer( isKnownPlayer );
}
/*
bool WorldModel::setTimeLastSeen(ObjectT o, Time time)
{
  Player  *object = (Player*) getObjectPtrFromType( o );    
  return object->setTimeLastSeen( time );
}*/
 
Vector WorldModel::getPosOpponentGoal()
{
 ObjectT objGoal = SoccerTypes::getGoalOpponent( getSide() );
 return SoccerTypes::getGlobalPositionFlag( objGoal,getSide( ), 200.0 );
}

Vector WorldModel::getPosOwnGoal()
{
  SideT sideGoal = getSide();
  ObjectT objGoal = SoccerTypes::getOwnGoal( sideGoal );
  return SoccerTypes::getGlobalPositionFlag( objGoal, getSide( ),200.0 );
}

double WorldModel::getRelDistanceOpponentGoal()
{
  Vector posGoal = getPosOpponentGoal();
  return getAgentGlobalPosition().getDistanceTo( posGoal );
}

double WorldModel::getRelAngleOpponentGoal()
{
	/*
  Vector posGoal;
  if( sideSide == OBJECT_GOAL_YELLOW )
    posGoal = SoccerTypes::getGlobalPositionFlag( OBJECT_GOAL_BLUE, sideSide );
  else
    posGoal = SoccerTypes::getGlobalPositionFlag( OBJECT_GOAL_YELLOW, sideSide );

  return ( posGoal - getAgentGlobalPosition()).getDirection();*/
  ObjectT objgoal = ( getSide() == SIDE_BLUE )? OBJECT_GOAL_YELLOW : OBJECT_GOAL_BLUE;
  AngDeg angoppGoal = getRelativeAngle( objgoal );
  return angoppGoal;
}

Vector WorldModel::getAgentGlobalPosition() const
{
 return MyInfo.getGlobalPosition();
}

double WorldModel::getAgentSpeed() const
{
 return MyInfo.getSpeed();
}

Vector WorldModel::getAgentGlobalVelocity() const
{
 return MyInfo.getGlobalVelocity();
}

Vector WorldModel::getAgentGlobalVelRotate() const
{
 return MyInfo.getGlobalVelRotate();
}

AngDeg WorldModel::getAgentGlobalBodyAngle()
{
  return MyInfo.getGlobalBodyAngle( );
}

bool WorldModel::setAgentObjectType(ObjectT o)
{
  MyInfo.setType( o );
  return true;
}

int WorldModel::getAgentIndex() const
{
  return SoccerTypes::getIndex( getAgentObjectType() );
}
 
ObjectT WorldModel::getAgentObjectType() const
{
  return MyInfo.getType();
}

Time WorldModel::getCurrentTime()
{
  long int iTime2 = getCurrentCycle();//timeGetTime();
  Time   time2;
  time2.updateTime( iTime2 );
  return time2;
}

long int WorldModel::getCurrentCycle()
{
   return  m_cycle;
}
bool WorldModel::setCurrentCycle( int t )
{
      m_cycle = t;
	  return true;
}
int WorldModel::getPlayerNumber() const
{
  return MyNum;
}

bool WorldModel::setPlayerNumber(int i)
{
  MyNum = i;
  return true;
}
/*
Time WorldModel::getTimeLastSeeMessage() const
{
  return timeLastSeeMessage;
}

Time WorldModel::getTimeLastSeen(ObjectT o)
{
 Object *object = getObjectPtrFromType( o );
  if( object != NULL )
    return object->getTimeLastSeen(  );
  return Time( -1 );
}*/

double WorldModel::getConfidence(ObjectT o)
{
  Object *object = getObjectPtrFromType( o );
  if( object != NULL )
    return object->getConfidence( getCurrentTime() );
  return 0.0;
}

bool WorldModel::isKickOffUs(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_KICKOFF  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_KICKOFF && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isKickOffThem(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OPP_KICKOFF  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_KICKOFF   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isGoalKickUs(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_GOALKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_GOALKICK && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isGoalKickThem(PlayModeT pm)
{ 
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OPP_GOALKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_GOALKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isPenaltyUs(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_PENALTY  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_PENALTY   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isPenaltyThem(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
  return ( pm == PM_OPP_PENALTY  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_PENALTY   && getSide() == SIDE_BLUE ) ;
}


AngDeg WorldModel::getGlobalAngle(ObjectT o)
{
 if( SoccerTypes::isLine( o ) )
    return SoccerTypes::getGlobalAngleLine( o, getSide() );
  return UnknownAngleValue;
}
  /*
bool WorldModel::setTimeLastSeeMessage(Time time)
{
    timeLastRecvSeeMessage  = time;
	timeLastSeeMessage  = time;
    return true;
}*/

/*
bool WorldModel::setTimeLastSeeGlobalMessage(Time time)
{
  return setTimeLastSeeMessage( time );

}*/

bool WorldModel::isBeforeKickOff(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
  return  ( ( isKickOffUs( pm ) || isKickOffThem( pm ) ) && MatchMode != STARTROBOT );
  
}
 
bool WorldModel::isFreeKickUs(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_FREEKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_FREEKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isFreeKickThem(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OPP_FREEKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_FREEKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isCornerKickUs(PlayModeT pm)
{
   if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_CORNERKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_CORNERKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isCornerKickThem(PlayModeT pm)
{
   if( pm == PM_ILLEGAL )
     pm = getPlayMode();
   return ( pm == PM_OPP_CORNERKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_CORNERKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isKickInUs(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OUR_THROWIN  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OPP_CORNERKICK   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isKickInThem(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();
   return ( pm == PM_OPP_CORNERKICK  && getSide() == SIDE_YELLOW  ) ||
         ( pm == PM_OUR_THROWIN   && getSide() == SIDE_BLUE ) ;
}

bool WorldModel::isDeadBallUs(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
  return isKickInUs  ( pm ) || isFreeKickUs  ( pm ) || isCornerKickUs     ( pm)
      || isKickOffUs ( pm ) || isGoalKickUs( pm )  ;
}

bool WorldModel::isDeadBallThem(PlayModeT pm)
{
 if( pm == PM_ILLEGAL )
    pm = getPlayMode();

  return isFreeKickThem( pm ) || isKickInThem  ( pm ) || isCornerKickThem ( pm)
     ||  isKickOffThem ( pm ) || isGoalKickThem( pm )    ;
}


bool WorldModel::isDropBall(PlayModeT pm)
{
  if( pm == PM_ILLEGAL )
    pm = getPlayMode();
  return ( pm == PM_DROPBALL ); 
}  

double WorldModel::reldir_2me(double x, double y)
{
  Vector    posAgent  = getAgentGlobalPosition();
  double t = Rad2Deg( atan2( y - posAgent.getY(),x - posAgent.getX() ) );//getAgentGlobalBodyAngle
  return( Vector::normalizeAngle( t - getAgentGlobalBodyAngle() ));//world.body.loc.body_dir
}
 
double WorldModel::getBallConfidence( Time time )
{
   
  double diff2 = time.getTimeDifference( ball.getTimeLastSeen() );
  if( diff2 < 5 )
	  diff2 = 0;
  if( ball.getTimeLastSeen() == -1 )       
    return 0.0;
  double dConf = 
      max( 0.0, 1.0 - (double)( diff2 / 35  ) / 100.0 );
  if( dConf > 1.0 )
    return 0.0;
  return dConf;
}

double WorldModel::getConfidenceWithTime(ObjectT o)
{
  Object *object = getObjectPtrFromType( o );
  if( object != NULL )
    return object->getConfidence2( getCurrentCycle() );
  return 0.0;
}
 
ObjectT WorldModel::way_blocked(Vector p)
{
 /* Vector   posBall   = getBallPos();
  Vector	posAgent = getAgentGlobalPosition();
  int i;
  double d, r,angtoopp;
  double dir = ( p - posAgent ).getDirection();
  if( posAgent.getDistanceTo( posBall ) < BLOCKING_BALL_DIST ||
      posAgent.getDistanceTo( p ) < 80 )
    return OBJECT_ILLEGAL;
  for( i = 0; i < 12; i++ ) 
  { 
	ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
	if( ( getConfidenceWithTime( o ) ) > MIN_USEFUL_CRR )
	{
		angtoopp = ( UnknownPlayers[i].getGlobalPosition() - posAgent ).getDirection();
		d = Vector::normalizeAngle( angtoopp - dir );
		r = posAgent.getDistanceTo( UnknownPlayers[i].getGlobalPosition() );
		if( ( fabs(d) < 90 ) && ( r < BLOCKING_DIST ) ) // 150
           if( fabs( sin( Deg2Rad( d ) ) * r ) < 100 )
			   return o;
	}
  }
  return OBJECT_ILLEGAL;*/

  Vector   posBall  = getBallPos();
  Vector   posAgent = getAgentGlobalPosition();
  double dis1 = 10000; // o with line distance
  double dis2 = 10000; // me to proj distance
  double dis3 = 10000; // o with line distance
  double dis4 = 10000; // me to proj distance
  MyLine l1 = MyLine::makeLineFromTwoPoints( posAgent, p );
  ObjectT opp = getClosestInSetTo( OBJECT_SET_PLAYERS, l1, posAgent, p, &dis1, &dis2 );
  ObjectT ote = getClosestInSetTo( OBJECT_SET_TEAMMATES, l1, posAgent, p, &dis3, &dis4 );
  // Vector posobstacle = getGlobalPosition( o );
 // double d = posAgent.getDistanceTo( posobstacle );
  if( posAgent.getDistanceTo( posBall ) < 100 ||
      posAgent.getDistanceTo( p ) < 100 )
    return OBJECT_ILLEGAL;
  if( dis3 < 60 && dis4 < 60 )
	return ote;
//  else if( dis1 < 100 && dis2 < 100 )
  //  return opp;
  else
	return OBJECT_ILLEGAL;

}

bool WorldModel::isBlockedByFrontObject()
{
  ObjectT o = Obstacle.getType();
  if( o == OBJECT_ILLEGAL )
	  return false;
  else
	  return true;
}
 
Vector WorldModel::getObstaclePositionOfFront()
{
  Vector pos;
  if( isBlockedByFrontObject() == true )
     pos = getGlobalPosition( OBJECT_UNKNOWN );
  else
	 pos = UnknownDoubleValue;
  return pos;
}
 
bool WorldModel::setPlayerTypes(int iPlayer)
{
  if( iPlayer == -1 )
    iPlayer = getPlayerNumber();
  switch( iPlayer )
  {
	 case 1: MyType = PT_GOALKEEPER; break;
	 case 2: MyType = PT_DEFENDER1;  break;
 	 case 3: MyType = PT_DEFENDER2;  break;
	 case 4: MyType = PT_ATTACKER1;  break;
	 case 5: MyType = PT_ATTACKER2;  break;
 	 default:
	  break;
  }
  return true;
}

PlayerT WorldModel::getPlayerTypes() const
{
  return ( MyType );
}

bool WorldModel::setSidePenalty(SideT side)
{
  m_sidePenalty = side;
  return true;
}

SideT WorldModel::getSidePenalty()
{
  return m_sidePenalty; 
}

bool WorldModel::changePlayerTypes()
{
//  double dist,dist2;
  Vector posAgent   = getAgentGlobalPosition();
  Vector posBall    = getBallPos();
  Vector posOwnGoal = getPosOwnGoal();
  ObjectT o         = getAgentObjectType();
  AngDeg  angball   = getRelativeAngle( OBJECT_BALL );
  AngDeg angBody    = getAgentGlobalBodyAngle();
/*
  static  PlayerT   myLastType = MyType;
  if( A_attackNumber1 == getPlayerNumber()  )
  {
     MyType = PT_ATTACKER1;
  }
  else if( A_attackNumber2 == getPlayerNumber() )
  {
     MyType = PT_ATTACKER1; 
  }
  else if( getClosestInSetTo( OBJECT_SET_TEAMMATES, posBall, &dist )  == o  )
  {
	if( dist < 100 && fabs( angball ) < 90 ) 
	   MyType = PT_ATTACKER1;
	else if( getPlayerNumber() == 1 )
        MyType = PT_GOALKEEPER;
  }
  else if( getClosestInSetTo( OBJECT_SET_TEAMMATES, posOwnGoal, &dist2 )  == o )
  {
     if( dist < 100 && fabs( angBody ) < 90 )
       MyType = PT_GOALKEEPER;
	 else if( getPlayerNumber() == 5 )
		 MyType = PT_ATTACKER1;
	   
  }
  myLastType = MyType;*/
  return true;
}
 
bool WorldModel::isInLeftBackField( Vector pos )
{
  if( pos == UnknownDoubleValue )
    Vector pos = getAgentGlobalPosition();
  return Rect( Vector( -450.0, 400.0 ), Vector( 0.0, 20.0 ) ).isInside( pos );
}

bool WorldModel::isInRightBackField(Vector pos)
{
  if( pos == UnknownDoubleValue )
    Vector pos = getAgentGlobalPosition();
  return Rect( Vector( -450.0, -400.0 ), Vector( 0.0, -20.0 ) ).isInside( pos );
}

bool WorldModel::isInLField(Vector pos)
{
  if( pos == UnknownDoubleValue )
    Vector pos = getAgentGlobalPosition();
  return Rect( Vector( 0.0, 400.0 ), Vector( 580.0, 0.0 ) ).isInside( pos );
}

bool WorldModel::isInRField(Vector pos)
{
  if( pos == UnknownDoubleValue )
    Vector pos = getAgentGlobalPosition();
  return Rect( Vector( 0.0, -400.0 ), Vector( 580.0, 0.0 ) ).isInside( pos );
}
