#include "stdafx.h"
#include "WorldModel.h"


ObjectT WorldModel::getClosestRelativeInSet(ObjectSetT set, double *dDist)
{
  ObjectT     closestObject = OBJECT_ILLEGAL;
  double      dMinMag       = 1000.0;
  double      dConfThr         = -1.0;
  int         i;
  if( set == OBJECT_SET_FLAGS )
  {
	for( i = 0; i < MAX_FLAGS; i++ )
	{
		ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
		if( Flags[i].getType() == OBJECT_ILLEGAL )
			continue;
		if( getConfidenceWithTime( o ) > dConfThr &&
			getRelativeDistance( o ) < dMinMag )  
			 
		{
		  dMinMag       = getRelativeDistance( o );
		  closestObject = o;
		}
	}
 	if( dDist != NULL )
	  *dDist = dMinMag;
	 
  }
  else if( set == OBJECT_SET_PLAYERS )
  {
    
	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) > dConfThr &&
			getRelativeDistance(o) < dMinMag )  
			 
		{
		  dMinMag       = getRelativeDistance(o);
		  closestObject = o;
		}
 		if( dDist != NULL )
		  *dDist = dMinMag;
	}
  }
  return closestObject;
}

ObjectT WorldModel::getSecondClosestRelativeInSet(ObjectSetT set, double *dDist)
{
  ObjectT     closestObject       = OBJECT_ILLEGAL;
  ObjectT     secondClosestObject = OBJECT_ILLEGAL;
  double      dMinMag             = 1000.0;
  double      dSecondMinMag       = 1000.0;
  double      d;
  int         i;
  double      dConfThr         = -1.0;
  Vector      posAgent  = getAgentGlobalPosition();
  switch( set )
  {
	 case OBJECT_SET_FLAGS:
 			for( i = 0; i < MAX_FLAGS; i++ )
			{
				if( Flags[i].getType() == OBJECT_ILLEGAL )
					continue;
				ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
				if( getConfidenceWithTime( o ) < dConfThr )
					continue;
				d = getRelativeDistance( o );
				if( d < dMinMag )                                 // closer then first
				{
				  dSecondMinMag         = dMinMag;                // put first to second
				  secondClosestObject   = closestObject;
				  dMinMag               = d;                      // and this to first
				  closestObject         = o;
				}
				else if( d < dSecondMinMag )                      // between first and 2nd
				{
				  dSecondMinMag         = d;                      // put this to second
				  secondClosestObject   = o;
				}
			}
			if( dDist != NULL )
			  *dDist = dSecondMinMag;
 		     break;
     case OBJECT_SET_PLAYERS:
			for( i = 0; i < MAX_OBSTACLES; i++ )
			{
				ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
				if( getConfidenceWithTime( o ) < dConfThr )
					continue;
				d = getRelativeDistance( o );
				if( d < dMinMag )                                 // closer then first
				{
				  dSecondMinMag         = dMinMag;                // put first to second
				  secondClosestObject   = closestObject;
				  dMinMag               = d;                      // and this to first
				  closestObject         = o;
				}
				else if( d < dSecondMinMag  )                      // between first and 2nd
				{
				  dSecondMinMag         = d;                      // put this to second
				  secondClosestObject   = o;
				}
			}
			if( dDist != NULL )
			  *dDist = dSecondMinMag;
			break;
	  case OBJECT_SET_TEAMMATES:
		  for( i = 0; i < OURTEAMNUM; i++ )
			{
				ObjectT o = SoccerTypes::getTeammateObjectFromIndex(i);
				if( getConfidenceWithTime( o ) < dConfThr )
					continue;
				d = posAgent.getDistanceTo( getGlobalPosition( o ) );//getRelativeDistance( o );
				if( d < dMinMag )                                 // closer then first
				{
				  dSecondMinMag         = dMinMag;                // put first to second
				  secondClosestObject   = closestObject;
				  dMinMag               = d;                      // and this to first
				  closestObject         = o;
				}
				else if( d < dSecondMinMag  )                      // between first and 2nd
				{
				  dSecondMinMag         = d;                      // put this to second
				  secondClosestObject   = o;
				}
			}
			if( dDist != NULL )
			  *dDist = dSecondMinMag;
			break;
	  default:
		    break;
  }
  return secondClosestObject;
/*
  for( i = 0; i < 6; i++ )
  {
    d = getRelativeDistance( Flags[i].getType() );
    if( d < dMinMag )                                 // closer then first
    {
      dSecondMinMag         = dMinMag;                // put first to second
      secondClosestObject   = closestObject;
      dMinMag               = d;                      // and this to first
      closestObject         = Flags[i].getType();
    }
    else if( d < dSecondMinMag  )                      // between first and 2nd
    {
      dSecondMinMag         = d;                      // put this to second
      secondClosestObject   = Flags[i].getType();
    }
  }
  if( dDist != NULL )
    *dDist = dSecondMinMag;
  return secondClosestObject;*/
 
}

ObjectT WorldModel::getClosestInSetTo(ObjectSetT set, MyLine l, 
									  Vector pos1, Vector pos2, 
									  double *dDistObjToLine, double *dDistPos1ToPoint)
{
  Vector posObj;
  Vector posOnLine;
  Vector posAgent = getAgentGlobalPosition();
  Vector myobj    = getAgentObjectType();
  double      dConfThr  = 0.89; // adjust by speed 
  ObjectT     obj       = OBJECT_ILLEGAL;
  double      dDist     ;
  double      dMinDist  = 1000.0;
  double      dDistPos1 = 1000.0;
  int         i;
  
  switch( set )
  {
     case OBJECT_SET_FLAGS:
		for( i = 0; i < MAX_FLAGS; i++ )
		{
			ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
			if( getConfidenceWithTime( o ) < dConfThr )
				continue;
			posObj    = getGlobalPosition( o );
			posOnLine = l.getPointOnLineClosestTo( posObj );
			dDist     = posObj.getDistanceTo( posOnLine );
			if( l.isInBetween( posOnLine, pos1, pos2 ) && dDist < dMinDist )
			{
			  dMinDist  = dDist;
			  obj       = o;
			  dDistPos1 = pos1.getDistanceTo( posOnLine );
			}
		}
		if( dDistObjToLine != NULL )
		   *dDistObjToLine = dMinDist;
		if( dDistPos1ToPoint != NULL )
		   *dDistPos1ToPoint = dDistPos1;
	    break;
     case OBJECT_SET_PLAYERS:
		for( i = 0; i < MAX_OBSTACLES; i++ )
		{
			ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
			if( getConfidenceWithTime( o ) < dConfThr )
				continue;
			posObj    = getGlobalPosition( o );
			if( posAgent == posObj )
				continue;
			posOnLine = l.getPointOnLineClosestTo( posObj );
			dDist     = posObj.getDistanceTo( posOnLine );
			if( l.isInBetween( posOnLine, pos1, pos2 ) && dDist < dMinDist )
			{
			  dMinDist  = dDist;
			  obj       = o;
			  dDistPos1 = pos1.getDistanceTo( posOnLine );
			}
		}
		if( dDistObjToLine != NULL )
		   *dDistObjToLine = dMinDist;
		if( dDistPos1ToPoint != NULL )
		   *dDistPos1ToPoint = dDistPos1;
		break;
     case OBJECT_SET_TEAMMATES:
		  for( i = 0; i < OURTEAMNUM; i++ )
			{
				ObjectT o = SoccerTypes::getTeammateObjectFromIndex(i);
				if( getConfidenceWithTime( o ) < dConfThr ||
					myobj == o )
					continue;
				posOnLine = l.getPointOnLineClosestTo( posObj );
				dDist     = posObj.getDistanceTo( posOnLine );
				if( l.isInBetween( posOnLine, pos1, pos2 ) && dDist < dMinDist )
				{
				  dMinDist  = dDist;
				  obj       = o;
				  dDistPos1 = pos1.getDistanceTo( posOnLine );
				}
			}
			if( dDistObjToLine != NULL )
		      *dDistObjToLine = dMinDist;
		    if( dDistPos1ToPoint != NULL )
		      *dDistPos1ToPoint = dDistPos1;
		    break;
  }
  return obj;
}

ObjectT WorldModel::getFurthestRelativeInSet(ObjectSetT set, double *dDist)
{
  ObjectT     furthestObject = OBJECT_ILLEGAL;
  double      dMaxMag       = -1000.0;
  int         i;
  double      dConfThr         = -1.0;
 
  if( set == OBJECT_SET_FLAGS )
  {
	for( i = 0; i < MAX_FLAGS; i++ )
	{
		ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
		if( getRelativeDistance( o ) > dMaxMag )  
			 
		{
		  dMaxMag        = getRelativeDistance( o );
          furthestObject = o;
		}
	}
 	if( dDist != NULL )
	  *dDist = dMaxMag;
	 
  }
  else if( set == OBJECT_SET_PLAYERS )
  {
 	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
		if( getRelativeDistance(o) > dMaxMag )  
			 
		{
		  dMaxMag        = getRelativeDistance( o );
          furthestObject = o;
		}
 		if( dDist != NULL )
		  *dDist = dMaxMag;
	}
  }
  else if( set == OBJECT_SET_LINES )
  {
    for( i = 0; i < MAX_LINES; i++ )
	{
		ObjectT o = SoccerTypes::getLineObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
		if( getRelativeDistance(o) > dMaxMag )  
			 
		{
		  dMaxMag        = getRelativeDistance( o );
          furthestObject = o;
		}
 		if( dDist != NULL )
		  *dDist = dMaxMag;
	}
  }
  return furthestObject;
}

ObjectT WorldModel::getClosestInSetTo( ObjectSetT set, Vector pos, double *dDist, double dConfThr, bool include_me )
{
  ObjectT     closestObject   = OBJECT_ILLEGAL;
  double      dMinMag         = 1000.0;
  Vector v;
  int         i;
  if( dConfThr == -1.0 ) dConfThr = 0.25; // PS->getPlayerConfThr();
  Vector      posAgent  = getAgentGlobalPosition();

  /*
  for( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
       o != OBJECT_ILLEGAL;
       o = iterateObjectNext( iIndex, set, dConfThr ) )
  {
    v = pos - getGlobalPosition( o );
    if( v.getMagnitude() < dMinMag )
    {
      dMinMag         = v.getMagnitude();
      closestObject   = o;
    }
  }
  iterateObjectDone( iIndex );
  if( dDist != NULL )
    *dDist = dMinMag;
  return closestObject; */
  switch( set )
  {
    case OBJECT_SET_FLAGS:
		for( i = 0; i < MAX_FLAGS; i++ )
		{
			ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
			if( getConfidenceWithTime( o ) < dConfThr )
				continue;
			v = pos - getGlobalPosition( o );
			if( v.getMagnitude() < dMinMag )
			{
			  dMinMag         = v.getMagnitude();
			  closestObject   = o;
			}
		}
		if( dDist != NULL )
		  *dDist = dMinMag;
	    break;
	case OBJECT_SET_PLAYERS:
		for( i = 0; i < MAX_OBSTACLES; i++ )
		{
			ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
			if( getConfidenceWithTime( o ) < dConfThr )
				continue;
			v = pos - getGlobalPosition( o );
			if( v.getMagnitude() < dMinMag )
			{
			  dMinMag         = v.getMagnitude();
			  closestObject   = o;
			}
 			if( dDist != NULL )
			  *dDist = dMinMag;
		}
		break;
	 case OBJECT_SET_TEAMMATES:
         for( i = 0; i < OURTEAMNUM; i++ )
		 {
			ObjectT o = SoccerTypes::getTeammateObjectFromIndex(i);
			double dconf = getConfidenceWithTime( o );
			if( getConfidenceWithTime( o ) < dConfThr )
				continue;
			if( include_me == false && getPlayerNumber() == ( i+1 ) )
				continue;
			v = pos -  getGlobalPosition( o ) ;//getRelativeDistance( o );
			if( v.getMagnitude() < dMinMag )
			{
			  dMinMag         = v.getMagnitude();
			  closestObject   = o;
			}
 			if( dDist != NULL )
			  *dDist = dMinMag;
		 }
  		 break;
	 default:
		break;
  }
   return closestObject;
}

ObjectT WorldModel::getFurthestInSetTo(ObjectSetT set, ObjectT objTarget, double *dDist, double dConfThr)
{
  if( dConfThr == -1.0 ) dConfThr      = 0.99;//PS->getPlayerConfThr();

  ObjectT     furthestObject = OBJECT_ILLEGAL;
  double      dMaxMag       = -1000.0;
  Vector      v;
  int         i;
/*
  for( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
       o != OBJECT_ILLEGAL;
       o = iterateObjectNext ( iIndex, set, dConfThr ) )
  {
    if( o != objTarget )
    {
      v = getGlobalPosition( objTarget ) - getGlobalPosition( o );
      if( v.getMagnitude() > dMaxMag )
      {
        dMaxMag        = v.getMagnitude();
        furthestObject = o;
      }
    }
  }
  iterateObjectDone( iIndex );
  if( dDist != NULL )
    *dDist = dMaxMag;
  return furthestObject;*/

  if( set == OBJECT_SET_FLAGS )
  {
	for( i = 0; i < MAX_FLAGS; i++ )
	{
		ObjectT o = SoccerTypes::getFlagObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
		if( o != objTarget )
		{ 
			v = getGlobalPosition( objTarget ) - getGlobalPosition( o );
			if( v.getMagnitude() > dMaxMag )
			{
			  dMaxMag        = v.getMagnitude();
			  furthestObject = o;
			}
		}
	}
 	if( dDist != NULL )
	  *dDist = dMaxMag;
  }
  else if( set == OBJECT_SET_PLAYERS )
  {
 	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
 	    if( o != objTarget )
		{ 
			v = getGlobalPosition( objTarget ) - getGlobalPosition( o );
			if( v.getMagnitude() > dMaxMag )
			{
				dMaxMag        = v.getMagnitude();
				furthestObject = o;
			}
		}
	}
	if( dDist != NULL )
	  *dDist = dMaxMag;
  }
  return furthestObject;

}
 
ObjectT WorldModel::iterateObjectStart( int &iIndex, ObjectSetT g, double dConf, bool bForward )
{
  iIndex = -1;
  return iterateObjectNext( iIndex, g, dConf, bForward );
}

ObjectT WorldModel::iterateObjectNext(int &iIndex, ObjectSetT g, double dConf, bool bForward)
{
  ObjectT o, objGoalie = OBJECT_TEAMMATE_1;
  bool bContinue = true;
  
  if( iIndex < 0 )
    iIndex = (bForward==false) ? OBJECT_MAX_OBJECTS : -1 ;
  int i = 0;
  if( bForward == true )
    i = iIndex + 1;
  else
    i = iIndex - 1;
  bContinue = ( bForward == false ) ? ( i >= 0 ) : ( i < OBJECT_MAX_OBJECTS ); 

  while( bContinue )
  {
    o = (ObjectT) i;
    if( SoccerTypes::isInSet( o, g, objGoalie ) )
    {
      if( getConfidenceWithTime( o ) >= dConf )
      {
        iIndex = i;
        return o;
      }
      else if( dConf == 1.0  )
      {
        iIndex = i; // confidence of 1.0 can only be in same cycle as see
        return o;   // message. Therefore first test should succeed normally;
      }             // in cases where this method is called after see message,
                    // but new sense has already arrived, confidence is lowered
    }               // but we want to return object that was seen in last see
                    // message; this compensates for those cases.
    if( bForward == true )
      i++;
    else
      i--;
    bContinue = ( bForward == false ) ? ( i >= 0 ) :( i < OBJECT_MAX_OBJECTS); 
  }

  return OBJECT_ILLEGAL;
}

void WorldModel::iterateObjectDone(int &iIndex)
{
  iIndex = -1;
}

double WorldModel::speedControl( int priority )
{
  double st =  30;
  if( priority == 6 ) 
   	st = 300;
  else if( priority == 5 )
    st = 200;
  else if( priority == 4 )
    st = 100;
  else if( priority == 3 )
    st = 50;
  else if( priority == 2 )
    st = 20;
   
  return st;
}

bool WorldModel::isBallKickable()
{
 Vector posBall  = getBallPos();
 Vector posAgent = getAgentGlobalPosition();
 double ang      = getRelativeAngle( OBJECT_BALL );
 if( getRelativeDistance( OBJECT_BALL ) < 60 &&
	 fabs( ang ) < 30 && posAgent.isBehindOf( posBall ) )
	return true;
 else
	return false;
}

bool WorldModel::isBallCatchable()
{
 Vector posBall  = getBallPos();
 Vector posAgent = getAgentGlobalPosition();
 double ang      = getRelativeAngle( OBJECT_BALL );
 if( getRelativeDistance( OBJECT_BALL ) < 35 &&  fabs( ang ) < 30   )
	return true;
 else
	return false;
}

bool WorldModel::isVisible(ObjectT o)
{
  Object *object = getObjectPtrFromType( o );

  if( object != NULL   )
    return true;

  return false;
}
 
bool WorldModel::isInField(Vector pos, double dMargin)
{
  return Rect( 
             Vector( + PITCH_LENGTH/2.0 - dMargin, 
                          - PITCH_WIDTH/2.0  + dMargin ),
             Vector( - PITCH_LENGTH/2.0 + dMargin, 
                          + PITCH_WIDTH/2.0  - dMargin ) 
             ).isInside( pos );
}

bool WorldModel::isInOwnPenaltyArea(Vector pos)
{
  /*ObjectT     objFlag = ( getSide() == SIDE_LEFT  )
                              ?  OBJECT_FLAG_P_L_C
                              :  OBJECT_FLAG_P_R_C ;

  if( isPenaltyUs() || isPenaltyThem() )
    objFlag = ( getSidePenalty() == SIDE_LEFT ) ? OBJECT_FLAG_P_L_C 
                                                : OBJECT_FLAG_P_R_C ;
  Vector posFlag =SoccerTypes::getGlobalPositionFlag( objFlag, getSide());
  if( fabs(pos.getX())   > fabs(posFlag.getX()) &&
      fabs( pos.getY() ) < PENALTY_AREA_WIDTH/2.0 )
    return true;*/
  if( fabs( pos.getX() )   > 100 && fabs( pos.getX() ) < 360 &&
      fabs( pos.getY() ) < 220 &&
	  sign( pos.getX() ) < 0 )
	  return true;
  return false;
}

Vector WorldModel::getStrategicPosition( int iPlayer )
{
 Vector pos, posBall   = getBallPos();
 bool   bOwnBall       = isBallInOurPossesion();
 if( iPlayer == -1 )
    iPlayer = getPlayerNumber();
 double dMaxX = 0;
 if( bOwnBall && 
      getGlobalPosition(
        SoccerTypes::getTeammateObjectFromIndex( iPlayer - 1 )).getX() 
      < posBall.getX() )
    dMaxX = max( dMaxX, posBall.getX() ); // 设定球员站位的最大x坐标，为球的x
 
  if( isGoalKickThem() )
    dMaxX = min( dMaxX, PENALTY_X - 200.0 ); // 对方开球门球，所有队员退后2m远，不能超过第二个参数设定的数值
  else if( isBeforeKickOff() ) // 中场开球，所有队员退后1m远
    dMaxX = min( dMaxX, -100.0 );

  if( isBeforeKickOff() ) // 球的位置初始化
    posBall.setVecPosition( 0, 0 );
  else if( isDropBall() )
    posBall.setX( posBall.getX() - 100.0 );
  else if( isGoalKickUs() || ( isFreeKickUs() && posBall.getX() < - 300 ) ) // 是我方门球，或者后场任意球
    posBall.setX( -FIELDLENGTH/4 + 5.0 ); // 设定球的位置
  else if( getConfidenceWithTime( OBJECT_BALL ) < 0.3 )
    posBall.setVecPosition( 0.0, 0.0 ); // 如果球的信息太久未更新，则认为球在中点
  else if( isGoalKickThem() || // 是对方门球，或对方后场任意球
           ( isFreeKickThem() && posBall.getX() > 300 ) ) // 设定球的相应位置
    posBall.setX( PENALTY_X - 200.0 );
  else if( isFreeKickThem() ) // 对方任意球，我方队员站位不能太近球，距球应有2m远
    posBall.setX( posBall.getX() - 200.0 );
  else if( isBallInOurPossesion() && // 我方控球，求不是死球状态，则球的位置
           !( isDeadBallUs() || isDeadBallThem() ) )
    posBall.setX( posBall.getX() + 50.0 );
  
  Vector     posHome; // 各球员的基本位置
  bool       bBehindBall; // 是否可站在球的前面，即球的x坐标是否可小于我的x坐标
  double     dAttrX,dAttrY,dMinX,dManX; // 位置调整系数
  switch( iPlayer )
  {
	 case 1:  
	  posHome = Vector( -250,0 ); // -500,0 
	  dAttrX  = 0.1;
	  dAttrY  = 0.1;
	  bBehindBall = true;
	  dMinX = -330;
	  dManX = -180;
	  break;
	case 2:  
	  posHome = Vector( -120,60 );
	  dAttrX  = 0.6;
	  dAttrY  = 0.25;
	  bBehindBall = true;
	  dMinX = -200;
	  dManX =  100;
	  break;
	case 3:  
	  posHome = Vector( -90,-50);
	  dAttrX  = 0.6;
	  dAttrY  = 0.25;
	  bBehindBall = true;
	  dMinX = -250;
	  dManX =  100;
	  break;
	case 4:  
	  posHome = Vector( -100,0);
	  dAttrX  = 0.6;
	  dAttrY  = 0.35;
	  bBehindBall = false;
	  dMinX = -300;
	  dManX = 350;
	  break;
	case 5: 
	  posHome = Vector( -90,0);
	  dAttrX  = 0.6;
	  dAttrY  = 0.2;
	  bBehindBall = false;
	  dMinX = -100;
	  dManX = 350;
	  break;
	case 6:  
	  posHome = Vector( 200,-100);
	  dAttrX  = 0.5;
	  dAttrY  = 0.25;
	  bBehindBall = false;
	  dMinX = -150;
	  dManX = 300;
	  break; 
	default:
	  break;
  }
  double x, y, dMaxYPercentage        = 0.8;
  y = posHome.getY() + posBall.getY() * dAttrY;
  x = posHome.getX() + posBall.getX() * dAttrX;
   if( fabs( y ) > 0.5 * dMaxYPercentage * FIELDWIDTH )
    y = sign(y) * 0.5 * dMaxYPercentage * FIELDWIDTH;
   if( bBehindBall == true && x > posBall.getX() )
    x = posBall.getX();
  if( x > dManX )
    x = dManX;
  else if( x < dMinX )
    x = dMinX;
  // when x coordinate is in front of allowed x value, change it
  if( x > dMaxX )
    x = dMaxX;
  return Vector( x, y ); // x y
}

Vector WorldModel::getDeadBallPosition()
{
  Vector pos, posBall = getBallPos();
  Vector posAgent     = getAgentGlobalPosition();
  Vector posStrate    = getStrategicPosition();
  Vector posOppGoal   =  getPosOpponentGoal();
  Angle  d1           = ( posBall - posOppGoal  ).getDirection();

  Vector posintercept = posBall + Vector( 60,d1,POLAR );

  Vector posOwnGoal   =  getPosOwnGoal();
  Angle  d2           = ( posOwnGoal - posBall ).getDirection();
  Vector posintercept2 = posBall + Vector( 100, d2, POLAR );
//  double dDist;
  if(  isKickInUs()  )//若是我方得分
    pos = posintercept;//posBall + Vector( -80.5, sign( posBall.getY() )*30 );
  else if( isCornerKickUs( ) )//如不是得分，是我方角球
    pos = posBall + Vector( 20.5, sign( posBall.getY() ) * 60 );//符号函数返回1，－1
  else if(  isFreeKickUs() || isGoalKickUs() )
    pos = posintercept;//posBall + Vector( -80.5, 0.0 );
  else if( isDeadBallThem() )
	pos = posintercept2;
  else if( isDropBall() )
	pos = posintercept2;
  else
	pos = posStrate;
/*  AngDeg      angBall = (posBall-posAgent).getDirection() ;
  ObjectT     obj = getClosestInSetTo( OBJECT_SET_PLAYERS,
                                            posAgent, &dDist);
  Vector posPlayer = getGlobalPosition( obj );

  // change point when heading towards other player or towards the ball
  if( fabs( angBall - (posPlayer-posAgent).getDirection() ) < 20 &&
      dDist < 6 )
    pos = pos - Vector( 80, 0 );
  if( fabs( angBall -  (pos-posAgent).getDirection()) < 20 )
  {
    angBall = Vector::normalizeAngle( angBall - 90 );
    pos = posBall + Vector( 100, angBall , POLAR );
  }*/
  return pos;
}

bool WorldModel::isBallInOurPossesion()
{
  Vector     posBall = getBallPos();
  ObjectT o = getFastestInSetTo( OBJECT_BALL );
  if( o == OBJECT_ILLEGAL )
    return false;
  if( SoccerTypes::isTeammate( o ) )
    return true;
  else
    return false;
}
 
bool WorldModel::coordinateWith( ObjectT obj ) // 可配合的队友
{
 Vector pos = getGlobalPosition( obj );
  if( pos.getDistanceTo( getBallPos() ) < 200.0 &&
      pos.X > getBallPos().X - 50.0 )
       return true;
  return false;
}

int WorldModel::getNrInSetInCircle(ObjectSetT set, Circle c)
{
  double dConfThr = 0.25; //PS->getPlayerConfThr();
  int    iNr      = 0;
  int    i;
  if( set == OBJECT_SET_PLAYERS )
  {
 	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
 	    if( c.isInside( getGlobalPosition( o ) ) )
          iNr++;
	}
  }
  return iNr;
}

int WorldModel::getNrInSetInRectangle(ObjectSetT set, Rect *rect)
{
  double dConfThr = 0.25;//PS->getPlayerConfThr();
  int    iNr      = 0;
  int    i;
  
  if( set == OBJECT_SET_PLAYERS )
  {
 	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
 	    if( rect == NULL || rect->isInside( getGlobalPosition( o ) ) )
          iNr++;
	}
  }
   
  return iNr;
}

int WorldModel::getNrInSetInCone(ObjectSetT set, double dWidth, Vector start, Vector end)
{
  double      dConfThr   = 0.25;//PS->getPlayerConfThr();
  int         iNr        = 0;
  int         i;
  MyLine        line       = MyLine::makeLineFromTwoPoints( start, end );
  Vector posOnLine;
  Vector posObj;
  
  if( set == OBJECT_SET_PLAYERS )
  {
 	for( i = 0; i < MAX_OBSTACLES; i++ )
	{
		ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
		if( getConfidenceWithTime( o ) < dConfThr )
			continue;
 	    posObj    = getGlobalPosition( o );
        posOnLine = line.getPointOnLineClosestTo( posObj );
    // whether posOnLine lies in cone is checked by three constraints
    // - does it lie in triangle (to infinity)
    // - lies between start and end (and thus not behind me)
    // - does it lie in circle
       if( posOnLine.getDistanceTo( posObj ) < dWidth * posOnLine.getDistanceTo( start )
       && line.isInBetween( posOnLine, start, end )
       && start.getDistanceTo( posObj ) < start.getDistanceTo( end ) )
        iNr++;
	}
  }
   
  return iNr;
}

bool WorldModel::isEmptySpace(ObjectT obj, AngDeg ang, double dDist)
{
  if( obj == OBJECT_ILLEGAL )
    return false;

  Vector pos = getGlobalPosition( obj );
   pos += Vector( dDist, ang, POLAR );

  if( getNrInSetInCircle( OBJECT_SET_PLAYERS, Circle( pos, dDist ) ) == 0 )
    return true;

  return false;
}
 
ObjectT WorldModel::getFastestInSetTo( ObjectT obj )
{
  ObjectT  objFastestOpp  = OBJECT_ILLEGAL, objFastestTeam  = OBJECT_ILLEGAL;
  double   distteam,distopp;
  Vector   pos = getGlobalPosition( obj );
  objFastestTeam = getClosestInSetTo( OBJECT_SET_TEAMMATES, pos, &distteam );
  objFastestOpp  = getClosestInSetTo( OBJECT_SET_PLAYERS, pos, &distopp );
  if( distteam > distopp )
	  return objFastestOpp;
  else
	  return objFastestTeam;
}
 
bool WorldModel::isBallHeadingToGoal()
{
  int iSide = 1;

 // if( isPenaltyUs() || isPenaltyThem() )
  //  iSide = ( getSide() == getSidePenalty() ) ? 1 : -1;
  
  if( getConfidenceWithTime( OBJECT_BALL ) < 0.65 ||
      fabs( getBallPos().getX() ) <  PENALTY_X - 5.0 )
  {
     return false;
  }
  MyLine l  = MyLine::makeLineFromPositionAndAngle( getBallPos(),getBallDirection() );
  MyLine l2 = MyLine::makeLineFromTwoPoints( getPosOwnGoal(), getPosOwnGoal() +
                                                         Vector( 0, 10 ));
  Vector posIntersect = l.getIntersection( l2 );
  if( fabs(posIntersect.getY()) > GoalWidth/2.0 + 30.0)
  {
     return false;
  }
  Vector pos    = getBallPos();
  int         iCycle = 1;
  while( fabs( pos.getX() ) < PITCH_LENGTH/2.0 && iCycle < 100)
  {
    pos = predictPosAfterNrCycles( OBJECT_BALL, iCycle );
    iCycle ++;
  }
  return ( iCycle == 20 ) ? false : true;
}
 
bool WorldModel::isOpponentAtAngle(AngDeg ang, double dDist)
{
  Vector posAgent   = getAgentGlobalPosition();
  double dConfThr  = 0.25;
  Vector posOpp;
  AngDeg      angOpp;
  int         i;
  for( i = 0; i < MAX_OBSTACLES; i++ )
  {
	ObjectT o = SoccerTypes::getObstacleObjectFromIndex(i);
	if( getConfidenceWithTime( o ) < dConfThr )
		continue;
     posOpp    = getGlobalPosition( o );
    angOpp    = ( posOpp - posAgent ).getDirection() ;
    if( fabs( angOpp - ang ) < 60 &&
        posAgent.getDistanceTo( posOpp ) < dDist )
      return true;
    else if( fabs( angOpp - ang ) < 120 &&
             posAgent.getDistanceTo( posOpp ) < dDist/2.0 )
      return true;
  }
  return false;
}