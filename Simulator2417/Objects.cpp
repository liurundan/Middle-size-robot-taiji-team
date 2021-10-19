#include "stdafx.h"
#include "Objects.h"

/*****************************************************************************/
/********************* CLASS OBJECT ******************************************/
/*****************************************************************************/ 
Object::Object()
{
	GlobalPos=Vector(0.0,0.0);
	RelPos=Vector(0.0,0.0);
    objectType  = OBJECT_ILLEGAL;
}

double Object::getConfidence(Time time)
{
  double  value = getRelativeDistance();
  int    up    = 0; 
  double dConf,left,right;
  if( value < 1000 )//&& value > 10 )
  {
      left  = MINDISTANCE;
      right = MAXDISTANCE;
      dConf = GetLinearFuzzyValue( value,left, right, up );
  }
  else 
	  return 0.0;
  if( dConf > 1.0 )
    return 0.0;
  return dConf;
}
double Object::getConfidence2( int cycle )
{ 
  if( cycleLastSeen < 1 )       
    return 0.0;
  double dConf = 
      max( 0.0, 1.0-(double)( cycle - cycleLastSeen )/100.0);
  if( dConf > 1.0 )
    return 0.0;
  return dConf;
}
double Object::getRelativeDistance()
{
 return RelPos.getMagnitude();
}
 
Vector Object::getGlobalPosition() const
{
  return GlobalPos;
}
 
bool Object::setTimeGlobalPosition(Time time)
{
  timeGlobalPosition = time;
  return true;
}
 
Time Object::getTimeGlobalPosition() const
{
 return timeGlobalPosition;
}

bool Object::setRelativeposLastSeen(Vector p, int c)
{
  posRelativeLastSee = p;
  cycleLastSeen = c;
  return true;
}

int Object::getCycleLastSeen() const
{
return cycleLastSeen;
}

Vector Object::getRelativeposLastSeen() const
{
  return posRelativeLastSee;
}

bool Object::setGlobalPositionLastSee(Vector p, Time time)
{
  posGlobalLastSee = p;
  setTimeGlobalPosDerivedFromSee( time );
  return true;
}

bool Object::setGlobalPositionLastSee(Vector p, int cycle )
{
  posGlobalLastSee = p;
  cycleLastSeen = cycle;
  return true;
}

bool Object::setTimeGlobalPosDerivedFromSee(Time time)
{
  timeGlobalPosDerivedFromSee = time;
  return true;
}

 

Vector Object::getGlobalPositionLastSee() const
{
  return posGlobalLastSee;
}
 
bool Object::setTimeLastSeen(Time time)
{
  timeLastSeen  = time;
  cycleLastSeen = time.getTime();
  return true;
}

Time Object::getTimeLastSeen() const
{
return timeLastSeen;
}

AngDeg Object::getRelativeAngle()
{
 return Vector::normalizeAngle( RelPos.getDirection() );
}


bool Object::setType(ObjectT o)
{
  objectType = o;
  return true;
}

ObjectT Object::getType() const
{
return objectType;
}
 
bool Object::setRelativePosition(double dDist, AngDeg ang, Time time)
{
  RelPos.setVecPosition( dDist, ang, POLAR );
  setTimeRelativePosition( time );
  return true;
}

bool Object::setTimeRelativePosition(Time time)
{
  timeRelativePosition = time;
  return true;
}

Time Object::getTimeRelativePosition() const
{
return timeRelativePosition;
}

bool Object::setRelativePosition(Vector v, Time time)
{
  RelPos = v;
  setTimeRelativePosition( time );
  return true;
}
 
Vector Object::getRelativePosition() const
{
 return RelPos;
}

bool Object::setGlobalPosition(Vector p, Time time)
{
  GlobalPos = p;
  setTimeGlobalPosition( time );
  return true;
}

//FixedObject::FixedObject():Object()
//{}

/*****************************************************************************/
/********************** CLASS FIXEDOBJECT ************************************/
/*****************************************************************************/
Vector FixedObject::getGlobalPosition(SideT s, double dGoalWidth) const
{
 return  SoccerTypes::getGlobalPositionFlag( getType(), s, dGoalWidth );
}
/*
bool DynamicObject::setGlobalPosLastSee(Vector pos,Time time)
{
  GlobalPosLastSee = pos;
  return true;
}*/
/*
Vector DynamicObject::getGlobalPosLastSee()
{
  return GlobalPosLastSee;
}*/

/*****************************************************************************/
/********************* CLASS DYNAMICOBJECT ***********************************/
/*****************************************************************************/
DynamicObject::DynamicObject():Object()
{
	GlobalVel=Vector(0.0,0.0);
    GlobalVelRotate=Vector(0.0,0.0);
    RelVel=Vector(0.0,0.0);
	dRelativeDistanceChange   = UnknownDoubleValue;
    dRelativeAngleChange      = UnknownDoubleValue;
}

Vector DynamicObject::getGlobalVelocity() const
{
  return GlobalVel;
}

Vector DynamicObject::getGlobalVelRotate() const
{
  return GlobalVelRotate;
}

bool  DynamicObject::setGlobalVelRotate( Vector vec,Time time)
{
  GlobalVelRotate = vec;
  setTimeGlobalVelRotate( time);
  return true;
}

bool DynamicObject::setTimeGlobalVelRotate(Time time)
{
  timeGlobalVelRotate = time;
  return true;
}

double DynamicObject::getSpeed() const
{
return GlobalVel.getMagnitude();
}

bool DynamicObject::setTimeGlobalVelocity(Time time)
{
 timeGlobalVelocity = time;
  return true;
}

Time DynamicObject::getTimeGlobalVelocity() const
{
return timeGlobalVelocity;
}

bool DynamicObject::setRelativeDistanceChange(double d, Time time)
{
  dRelativeDistanceChange = d;
  setTimeChangeInformation( time );
  return true;
}

bool DynamicObject::setTimeChangeInformation(Time time)
{
  timeChangeInformation = time ; 
  return true;
}

Time DynamicObject::getTimeChangeInformation() const
{
return timeChangeInformation;
}

double DynamicObject::getRelativeDistanceChange() const
{
return dRelativeDistanceChange;
}

bool DynamicObject::setRelativeAngleChange(double d, Time time) // useless
{
  dRelativeAngleChange = d;
  setTimeChangeInformation( time );
  return true;
}

double DynamicObject::getRelativeAngleChange() const
{
 return dRelativeAngleChange;
}

bool DynamicObject::setGlobalVelocityLastSee(Vector vec)
{
  vecGlobalVelocityLastSee = vec;
  return true;
}

Vector DynamicObject::getGlobalVelocityLastSee() const
{
return vecGlobalVelocityLastSee;
}

bool DynamicObject::setGlobalVelocity(Vector v, Time time)
{
 if( v.getMagnitude() < EPSILON )
    GlobalVel.setVecPosition( 0.0, 0.0 );
  else
     GlobalVel = v;
  setTimeGlobalVelocity( time );
  return true;
}

/*****************************************************************************/
/********************* CLASS BALLOBJECT **************************************/
/*****************************************************************************/

Ball::Ball():DynamicObject()
{
	
}  

/*****************************************************************************/
/********************* CLASS PLAYEROBJECT ************************************/
/*****************************************************************************/

Player::Player():DynamicObject()
{
	BodyFacing=0.0;
 	isKnownPlayer        = false;
    isGoalie             = false;
    angRelativeBodyAngle = UnknownAngleValue;
}

AngDeg Player::getGlobalBodyAngle() const
{
return BodyFacing;
}

AngDeg Player::getGlobalBodyAngleLastSee() const
{
 return angGlobalBodyAngleLastSee;
}


bool Player::getIsGoalie() const
{
 return isGoalie;
}

bool Player::getIsKnownPlayer() const
{
return isKnownPlayer;
}

AngDeg Player::getRelativeBodyAngle() const
{
return angRelativeBodyAngle;
}

bool Player::setRelativeBodyAngle(AngDeg ang, Time time)
{
  angRelativeBodyAngle = ang;
  setTimeRelativeAngles( time );
  return true;
}

bool Player::setTimeRelativeAngles(Time time)
{
  timeRelativeAngles = time;
  return true;
}


bool Player::setGlobalBodyAngle(AngDeg ang, Time time)
{
  BodyFacing = ang;
  setTimeGlobalAngles( time );
  return true;
}

bool Player::setTimeGlobalAngles(Time time)
{
 timeGlobalAngles = time;
  return true;
}

bool Player::setGlobalBodyAngleLastSee(AngDeg ang)
{
 angGlobalBodyAngleLastSee = ang;
  return true;
}

Time Player::getTimeGlobalAngles() const
{
 return timeGlobalAngles;
}

bool Player::setIsKnownPlayer(bool b)
{
  isKnownPlayer = b;
  return true;
}

bool Player::setIsGoalie(bool b) // 
{
  isGoalie = b;
  return true;
}

Time Player::getTimeRelativeAngles() const
{
 return timeRelativeAngles ;
}

Vector Player::getPositionDifference() const
{
  return posPositionDifference;
}

bool Player::setPositionDifference(Vector p)
{
  posPositionDifference = p;
  return true;
}

Vector Player::getSpeedRelToBody() const
{
 return velSpeedRelToBody;
}

bool Player::setSpeedRelToBody(Vector v) // useless
{
  velSpeedRelToBody = v;
  return true;
}

bool DynamicObject::setPositionDifference(Vector p)
{
  posPositionDifference = p;
  return true;
}

Vector DynamicObject::getPositionDifference() const
{
 return posPositionDifference;
}
