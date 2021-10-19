#include "stdafx.h"
#include "SoccerTypes.h"
 
/*****************************************************************************/
/********************* CLASS SOCCERTYPES *************************************/
/*****************************************************************************/
int SoccerTypes::getIndex( ObjectT o )
{
  if( o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_6 )
    return o - OBJECT_OPPONENT_1;
  else if( o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_6 )
    return o - OBJECT_TEAMMATE_1;
  else if( o >= OBJECT_MARKPOLE_BLUE_0 && o <= OBJECT_FLAG_C  )
  	  return o - OBJECT_MARKPOLE_BLUE_0;  
  else if( o >= OBJECT_LINE_0 && o <= OBJECT_LINE_8 )
    return o - OBJECT_LINE_0;
  else if( o >= OBJECT_PLAYER_UNKNOWN_0 && o <= OBJECT_PLAYER_UNKNOWN_11 )
	return o - OBJECT_PLAYER_UNKNOWN_1;
  else
    return -1;
}

ObjectT SoccerTypes::getObstacleObjectFromIndex( int  iIndex )
{
   return (ObjectT) ( OBJECT_PLAYER_UNKNOWN_0 + iIndex );
}

ObjectT SoccerTypes::getFlagObjectFromIndex(int i)
{
   return (ObjectT) ( OBJECT_MARKPOLE_BLUE_0 + i  );
}
 
bool SoccerTypes::isBall( ObjectT o )
{
  return o == OBJECT_BALL;
}
  
bool SoccerTypes::isLine( ObjectT o )
{
  return ( o >= OBJECT_LINE_0  && o <= OBJECT_LINE_8 );
}
 
bool SoccerTypes::isFlag( ObjectT o )
{
  return ( o >= OBJECT_MARKPOLE_BLUE_0 && o <= OBJECT_FLAG_C );  // OBJECT_FLAG_C
}

bool SoccerTypes::isPole( ObjectT o )
{
  return ( o >= OBJECT_MARKPOLE_BLUE_0 && o <= OBJECT_MARKPOLE_YELLOW_3 );  
                
}
  
AngDeg SoccerTypes::getGlobalAngleLine( ObjectT o , SideT s )
{
  AngDeg angle;
  switch( o )
  {
    case OBJECT_LINE_0: angle = 180.0; break;
    case OBJECT_LINE_1: angle =   0.0; break;
    case OBJECT_LINE_2: angle = -90.0; break;
    case OBJECT_LINE_3: angle =  90.0; break;
  default:
     return UnknownAngleValue;
  }
   if( s == SIDE_BLUE )
    angle += 180;
   return Vector::normalizeAngle( angle );
}
  
bool SoccerTypes::isGoal( ObjectT o )
{
  return o == OBJECT_GOAL_BLUE || o == OBJECT_GOAL_YELLOW;
}
  
bool SoccerTypes::isTeammate( ObjectT o )
{
  return o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_5;
}
  
bool SoccerTypes::isOpponent( ObjectT o )
{
  return o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_UNKNOWN;
}

bool SoccerTypes::isKnownPlayer( ObjectT o )
{
  return (o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_6) ||
         (o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_6);
}
  
bool SoccerTypes::isPlayer( ObjectT o )
{
  return isKnownPlayer( o ) || o == OBJECT_TEAMMATE_UNKNOWN ||
         o == OBJECT_OPPONENT_UNKNOWN || 
		 ( o >= OBJECT_PLAYER_UNKNOWN_0 && o <= OBJECT_PLAYER_UNKNOWN_11);
}

bool SoccerTypes::isGoalie( ObjectT o )
{
  return o == OBJECT_TEAMMATE_1 || o == OBJECT_OPPONENT_1;
}
  
ObjectT SoccerTypes::getOwnGoal( SideT s )
{
  if( SIDE_YELLOW == s )
    return OBJECT_GOAL_YELLOW;
  else if( SIDE_BLUE == s )
    return OBJECT_GOAL_BLUE;
   return OBJECT_ILLEGAL;
}

ObjectT SoccerTypes::getGoalOpponent( SideT s )
{
  if( SIDE_YELLOW == s )
    return OBJECT_GOAL_BLUE;
  else if( SIDE_BLUE == s )
    return OBJECT_GOAL_YELLOW;
   return OBJECT_ILLEGAL;
}
 
 
 //1200*800 really match option set
Vector SoccerTypes::getGlobalPositionFlag( ObjectT o, SideT s,
                                                double dGoalWidth )
{
  Vector pos;
  
  if( !(isFlag(o) || isGoal(o)) )
      return Vector(UnknownDoubleValue, UnknownDoubleValue);
  switch( o )  
  {
    case OBJECT_GOAL_BLUE:
      pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN, 0 );                  break;
    case OBJECT_GOAL_YELLOW:
      pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, 0 );                  break;
    case OBJECT_MARKPOLE_BLUE_0:
      pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);     break;
    case OBJECT_MARKPOLE_BLUE_1:
      pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  PITCH_WIDTH/2.0 +PITCH_MARGIN);  break;
    case OBJECT_MARKPOLE_YELLOW_2:
      pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, PITCH_WIDTH/2.0+PITCH_MARGIN  );  break;
    case OBJECT_MARKPOLE_YELLOW_3:
      pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, -PITCH_WIDTH/2.0 -PITCH_MARGIN );  break;
	
	case OBJECT_FLAG_YE_T:
      pos.setVecPosition( -PITCH_LENGTH/2.0, PITCH_WIDTH/2.0 );     break;
     
    case OBJECT_FLAG_C_T:
      pos.setVecPosition(   0.0,  PITCH_WIDTH/2.0);                  break;
    
    case OBJECT_FLAG_BL_T:
      pos.setVecPosition( PITCH_LENGTH/2.0,   PITCH_WIDTH/2.0 );     break;
     
    case OBJECT_FLAG_G_BL_T:
      pos.setVecPosition( PITCH_LENGTH/2.0,  dGoalWidth/2.0 );      break;
    
	case OBJECT_FLAG_BL_0:
      pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  0.0 );   break;
    case OBJECT_FLAG_G_BL_B:
      pos.setVecPosition( PITCH_LENGTH/2.0,  -dGoalWidth/2.0 );       break;
    
 
    case OBJECT_FLAG_BL_B:
      pos.setVecPosition( PITCH_LENGTH/2.0,  -PITCH_WIDTH/2.0 );      break;
    
    case OBJECT_FLAG_C_B:
      pos.setVecPosition(   0.0, -PITCH_WIDTH/2.0 );                  break;
    
	 
    case OBJECT_FLAG_YE_B:
      pos.setVecPosition( -PITCH_LENGTH/2.0,  -PITCH_WIDTH/2.0 );     break;
    
    case OBJECT_FLAG_G_YE_B:
      pos.setVecPosition( -PITCH_LENGTH/2.0,  -dGoalWidth/2.0 );      break;
    case OBJECT_FLAG_YE_0:
      pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN,  0.0 );  break;
    case OBJECT_FLAG_G_YE_T:
      pos.setVecPosition( -PITCH_LENGTH/2.0,  dGoalWidth/2.0 );     break;
     
    case OBJECT_FLAG_P_YE_T:
		pos.setVecPosition(  -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, PENALTY_AREA_WIDTH/2.0    );		break;
    case OBJECT_FLAG_P_YE_C:
      pos.setVecPosition( -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 0.0 );       break;
    case OBJECT_FLAG_P_YE_B:
      pos.setVecPosition( -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, -PENALTY_AREA_WIDTH/2.0 ); break;
    
	case OBJECT_FLAG_P_BL_T:
      pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, PENALTY_AREA_WIDTH/2.0 ); break;
    case OBJECT_FLAG_P_BL_C:
      pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 0.0 );
	  break;
    case OBJECT_FLAG_P_BL_B:
      pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH,  -PENALTY_AREA_WIDTH/2.0 );	  break; 
    case OBJECT_FLAG_C:
      pos.setVecPosition(  0.0 , 0.0 );                              break;
	default: 
		break;
  }
  if( s == SIDE_BLUE ) // change side for team on the right side.
 	  pos.setVecPosition( -pos.X, -pos.Y );
   return pos;
} 

bool SoccerTypes::isInSet( ObjectT o, ObjectSetT o_g, ObjectT objGoalie )
{
  switch( o_g )
  {
    case OBJECT_SET_TEAMMATES: return isTeammate( o ) && isKnownPlayer( o );
    case OBJECT_SET_OPPONENTS: return isOpponent( o ) && isKnownPlayer( o );
    case OBJECT_SET_PLAYERS:   return isPlayer  ( o ) ;//&& isKnownPlayer( o );
    case OBJECT_SET_FLAGS:     return isFlag    ( o );
    case OBJECT_SET_LINES:     return isLine    ( o );
    case OBJECT_SET_TEAMMATES_NO_GOALIE: 
                               return isTeammate( o ) && isKnownPlayer( o ) &&
                                      o != objGoalie;    
    case OBJECT_SET_ILLEGAL:
    default:                   break;
  }
  return false;
}
 
ObjectT SoccerTypes::getTeammateObjectFromIndex( int iIndex )
{
  return (ObjectT) ( OBJECT_TEAMMATE_1 + iIndex );
}
 
ObjectT SoccerTypes::getOpponentObjectFromIndex( int iIndex )
{
  return (ObjectT) ( OBJECT_OPPONENT_1 + iIndex );
}
 
ObjectT SoccerTypes::getLineObjectFromIndex( int iIndex)
{
  return (ObjectT) ( OBJECT_LINE_0 + iIndex );
}

/*****************************************************************************/
/********************* CLASS TIME ********************************************/
/*****************************************************************************/
Time::Time( long int iTime  )
{
  m_iTime    = iTime;
  m_iCycle   = iTime;
}
 
bool Time::updateTime( long int iTime )
{
   m_iTime    = iTime;
   m_iCycle   = iTime;
   return true;
}
 
long int Time::getTime( )
{
  return m_iTime; // cycle
}
 
long int Time::getTimeDifference( Time t )
{
    if( getTime() == t.getTime() )
      return 0;
    else
      return (long int)(fabs( getTime() - t.getTime() ));
}
 
bool Time::updateCycle( long int iCycle )
{
   m_iCycle    = iCycle;
   return true;
}

long int Time::getCycle( )
{
  return m_iCycle;
}
 
long int Time::getCycleDifference( Time t )
{
    if( getCycle() == t.getCycle() )
      return 0;
    else
      return (long int)(fabs( getCycle() - t.getCycle() ));
}
bool Time::operator != ( const long int &i )
{
  return getTimeDifference( Time(i) ) != 0;
}
  
bool Time::operator != ( Time t )
{
  return getTimeDifference( t ) != 0;
}

 
bool Time::operator == ( const long int &i )
{
  return !( *this != i );
}

 
bool Time::operator == ( Time t )
{
  return !( *this != t );
}

 
bool Time::operator <  ( Time t )
{
  return getTimeDifference( t ) < 0;
}
  
bool Time::operator <  ( const long int  &i )
{
  return Time( i  ) >= *this ;
}
  
bool Time::operator <= ( Time t )
{
  return ( *this < t ) || (*this == t );
}
  
bool Time::operator <= ( const long int  &i )
{
  return ( *this < i ) || (*this == i );
}
  
bool Time::operator >  ( Time t )
{
  return !( *this <= t );
}
  
bool Time::operator >  ( const long int  &i )
{
  return !( *this <= i );
}
  
bool Time::operator >= ( Time t )
{
  return !( *this < t );

}
  
bool Time::operator >= ( const long int  &i )
{
  return !( *this < i );
}
  
void Time::operator += ( Time t )
{
  updateTime    ( getTime() + t.getTime() );
}

void Time::operator -= ( Time t )
{
  updateTime    ( getTime() - t.getTime() );
}
  
long int Time::operator - ( Time t )
{
  return getTimeDifference( t );
}
  
Time Time::operator + ( Time t )
{
  return Time( getTime() + t.getTime() );
}
 

