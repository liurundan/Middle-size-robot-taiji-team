#include "stdafx.h"
#include "WorldModel.h"


Vector WorldModel::calculatePosAgentWith2Flags(ObjectT objFlag1, ObjectT objFlag2)
{
    double      xA, yA, xB, yB, rA, rB;
    AngDeg      aA, aB;
    xA = getGlobalPosition  ( objFlag1 ).X;
    yA = getGlobalPosition  ( objFlag1 ).Y;
    rA = getRelativeDistance( objFlag1 );
    aA = getRelativeAngle   ( objFlag1 );
    xB = getGlobalPosition  ( objFlag2 ).X;
    yB = getGlobalPosition  ( objFlag2 ).Y;
    rB = getRelativeDistance( objFlag2 );
    aB = getRelativeAngle   ( objFlag2 );
    
	double      L, dx, dy, d_par, d_orth;
    double      x, y;
    
	// Sign is like this 'because' y-axis increases from top to bottom
    double sign = ((aB - aA) > 0.0) ? 1.0 : -1.0;
 	
	dx = xB - xA;
    dy = yB - yA;
    L = sqrt(dx*dx + dy*dy);                   // distance between two flags
    dx /= L; dy /= L;                          // normalize
    d_par = (L*L + rA*rA - rB*rB) / (2.0 * L); // dist from flag1 to orth proj
    double arg = rA*rA - d_par*d_par;
    d_orth = (arg > 0.0) ? sqrt(arg) : 0.0;
    x = xA + d_par * dx - sign * d_orth * dy;
    y = yA + d_par * dy + sign * d_orth * dx;
    
	double dMargin = 60.0;
	if( ( fabs(x) > PITCH_LENGTH/2.0 + dMargin ) ||  
		( fabs(y) > PITCH_WIDTH/2.0 + dMargin ) )
       return Vector( UnknownDoubleValue, UnknownDoubleValue );
	else
	   return Vector( x, y );
}
 
AngDeg WorldModel::calculateAngleAgentWithPos(Vector pos)
{
   
  int    iNrFlags = 0 ;
  double dCosX=0, dSinX=0 ,dAngleNow, xA, yA, aA;
  for( int i = 0; i < MAX_FLAGS; i++ )
  {
    ObjectT obj = Flags[i].getType( ); 
	if( obj == OBJECT_ILLEGAL )
		continue;
    xA = getGlobalPosition( obj ).getX();
    yA = getGlobalPosition( obj ).getY();
    aA = getRelativeAngle( obj );

    // calculate global direction between flag and agent
    // calculate global neck angle by subtracting relative angle to flag
    dAngleNow = atan2Deg( yA - pos.getY(), xA - pos.getX() );
    dAngleNow = Vector::normalizeAngle( dAngleNow - aA );

    // add cosine part of angle and sine part separately; this to avoid
    // boundary problem when computing average angle (average of -176 and
    // 178 equals -179 and not 1).
    dCosX += cosDeg( dAngleNow );
    dSinX += sinDeg( dAngleNow );
    iNrFlags++;
	
  }
 
  // calculate average cosine and sine part and determine corresponding angle
  dCosX /= (double)iNrFlags;
  dSinX /= (double)iNrFlags;
  if( iNrFlags == 0 )
    return UnknownAngleValue;

  return Vector::normalizeAngle( atan2Deg( dSinX, dCosX  ) )  ;

}

bool WorldModel::calculateStateAgent(Vector *posGlobal, Vector *velGlobal, AngDeg *angGlobal)
{
  int    iNrLeft;
//  double dis;
  // first determine global neck angle
 /* ObjectT objLine = getFurthestRelativeInSet( OBJECT_SET_LINES, &dis );
  if( objLine != OBJECT_ILLEGAL )
  {
    double angGlobalLine = getGlobalAngle  ( objLine );
    AngDeg angRelLine    = getRelativeAngle( objLine );
    *angGlobal           = angGlobalLine - angRelLine;
    *angGlobal           = Vector::normalizeAngle( *angGlobal );
  }
  else
  {
     *angGlobal           = getAgentGlobalBodyAngle();
  }*/
  *angGlobal = Vector::normalizeAngle ( getAgentGlobalBodyAngle() + 
	                            MyInfo.getGlobalVelRotate().getX() * 100 );
  // use global neck angle to determine estimate of current global velocity
  // neck angle is better estimate than after sense -> better estimate velocity
  // update all position particles of the agent with this velocity
  //  'false' denotes update after see message. Since global neck angle can
  //  be determined more precise after 'see' it is better to predict position
  //  again (although it was already done after sense)
  // and then check which particles are possible given current perceptions
  Vector v = MyInfo.getGlobalVelocity();

  *velGlobal = MyInfo.getGlobalVelocity();
    

  updateParticlesAgent          ( *velGlobal, false );
  iNrLeft = checkParticlesAgent ( *angGlobal        );

  if( iNrLeft == 0 ) // if no particles left, initialize all particles
  {
    // initialize particles (from random samples using closest flag)
    // check particles are then checked with other flags
    initParticlesAgent ( *angGlobal );
    iNrLeft = checkParticlesAgent( *angGlobal );
    if( iNrLeft == 0 )
    {
      // not succeeded, use second method (weighted average flags)
      // and initialize all particles to this position
      calculateStateAgent2( posGlobal, velGlobal, angGlobal );
      initParticlesAgent( *posGlobal );
      return false;
    }
  }

  // determine global position (= average of all particles)
  // and resample all particles
  *posGlobal = averageParticles( particlesPosAgent, iNrLeft );
  resampleParticlesAgent( iNrLeft, *posGlobal );

  // use the position to calculate better global neck angle of the agent
  // and recalculate global velocity with improved neck angle
  AngDeg ang = calculateAngleAgentWithPos( *posGlobal );
  if( ang != UnknownAngleValue )
    *angGlobal = ang;

  //*velGlobal = MyInfo.getSpeedRelToBody().rotate(*angGlobal);
  return true;
}

bool WorldModel::calculateStateAgent2(Vector *posGlobal, Vector *velGlobal, AngDeg *angGlobal)
{
  double      x=0.0, y=0.0, dMinRadius1, dMaxRadius1, dMinRadius2, dMaxRadius2;
  double      dTotalVar = UnknownDoubleValue, dVar, K;
  ObjectT     obj1,obj2;
  Vector pos;
   
  for( int i = 0; i < 6; i++ )
  {
    obj1 = Flags[i].getType( ); 
    for( int j = i+1; j < 6; j++ )
    {
      obj2 = Flags[j].getType( );
      
	  // calculate the position using the two flags
      pos = calculatePosAgentWith2Flags( obj1, obj2 );

      // get distance range from which perceived value can originate from
      // calculate variance (=weighted factor) based on the distance to the
      // two flags, use variance corresponding to uniform distribution
      // this is not completely correct, better would be to use the
      // intersection area size of the two circle, but is too computational
      // intensive
      getMinMaxDistQuantizeValue(getRelativeDistance(obj1),
                 &dMinRadius1, &dMaxRadius1, QUANTIZESTEP, 0.1 )  ; 
      getMinMaxDistQuantizeValue(getRelativeDistance(obj2),
                 &dMinRadius2, &dMaxRadius2, QUANTIZESTEP, 0.1 )  ; 
      dVar =  (dMaxRadius1-dMinRadius1)*(dMaxRadius1-dMinRadius1)/12; 
      dVar += (dMaxRadius2-dMinRadius2)*(dMaxRadius2-dMinRadius2)/12;
      
      if( pos.X != UnknownDoubleValue &&
          dTotalVar  == UnknownDoubleValue )
      {
        dTotalVar = dVar;                     // initialize the position
        x         = pos.X;
        y         = pos.Y;
      }
      else if( pos.X != UnknownDoubleValue )
      {
        K = dTotalVar / ( dVar + dTotalVar ); // otherwise use new position
        x += K*( pos.X - x );            // based on weighted variance
        y += K*( pos.Y - y );
        dTotalVar -= K*dTotalVar;
      }
    }
    
  }
   
  posGlobal->setVecPosition( x, y );
  
  // now calculate global position (experiments show best results are obtained
  // when average with all perceived flags is taken).
  *angGlobal = calculateAngleAgentWithPos( *posGlobal );

  // update velocity since after 'see' we have a better estimate of neck angle
  //*velGlobal = Vector(0,0);//MyInfo.getSpeedRelToBody().rotate(*angGlobal);
  //MyInfo.setGlobalVelocity( *velGlobal, getCurrentTime() );
  return true;
}

bool WorldModel::calculateStateAgent3(Vector *posGlobal, Vector *velGlobal, AngDeg *angGlobal)
{
  // first determine the two closest flags
    
  static  Time    t1;
 // Vector  lastpos = MyInfo.getGlobalPositionLastSee();
 // Vector  posdiff;
  long int timediff = getCurrentTime().getTime() - t1.getTime();
  double  dist = 0;
  ObjectT objFlag1 = getClosestRelativeInSet( OBJECT_SET_FLAGS );
  ObjectT objFlag2 = getSecondClosestRelativeInSet( OBJECT_SET_FLAGS );
//  ObjectT objLine  = getFurthestRelativeInSet( OBJECT_SET_LINES, &dist );
  // first determine global neck angle with furthest line (max. error is 0.5)
/*  if( objLine != OBJECT_ILLEGAL )
  {
    double angGlobalLine = getGlobalAngle  ( objLine );
    AngDeg angRelLine    = getRelativeAngle( objLine );
    *angGlobal           = angGlobalLine - angRelLine;
    *angGlobal           = Vector::normalizeAngle( *angGlobal );
  }*/
  // if two flags were seen in last see message
  //   calculate global position using two closest flags (Cosinus rule)
  //     except when rel angle flags is smaller than 8 (gives bad results)
  //       except when no line is seen
  if( objFlag1 != OBJECT_ILLEGAL && objFlag2 != OBJECT_ILLEGAL &&
      (
         fabs(getRelativeAngle(objFlag1) - getRelativeAngle(objFlag2)) > 8.0
        // || objLine == OBJECT_ILLEGAL
      ) )
  {
    *posGlobal = calculatePosAgentWith2Flags( objFlag1, objFlag2 );
  }
/*  else if( objFlag1 != OBJECT_ILLEGAL && objLine != OBJECT_ILLEGAL )
  {
    // calculate global position as follows:
    //   - get the global position of the closest flag
    //   - get the rel vector to this flag using global neck angle*
    //   - global position = global pos flag - relative vector

    Vector posGlobalFlag = getGlobalPosition( objFlag1 );
    Vector relPosFlag    = Vector::getVecPositionFromPolar(
		           getRelativeDistance( objFlag1 ),
             *angGlobal + getRelativeAngle( objFlag1 ) );
    *posGlobal                = posGlobalFlag - relPosFlag;
  }
  else
  {
    return false;
  }*/
  // calculate global velocity using the absolute neck angle
 // posdiff = lastpos - *posGlobal;
 // *velGlobal = Vector( posdiff.X/timediff, posdiff.Y/timediff);
   *velGlobal = 0;//MyInfo.getSpeedRelToBody().rotate(*angGlobal);
  MyInfo.setGlobalVelocity( *velGlobal, getCurrentTime() );
  *angGlobal = calculateAngleAgentWithPos( *posGlobal );
  return true;
}

bool WorldModel::calculateStateBall(Vector *posGlobal, Vector *velGlobal)
{
    
    Vector posRelWorld =
      Vector( getRelativeDistance( OBJECT_BALL ),
                   getRelativeAngle( OBJECT_BALL ) +  getAgentGlobalBodyAngle(),
                   POLAR );
  *posGlobal =  getAgentGlobalPosition() + posRelWorld;
  if( getCurrentCycle() > 2 )
  {
	  Vector posGlobaldiff = *posGlobal - ball.getGlobalPositionLastSee()
	                       - MyInfo.getPositionDifference();   
      *velGlobal = 0;//posGlobaldiff / CONTROL_INTERVAL*1000; 
  } 
 
/*  if( ball.getType() == OBJECT_ILLEGAL )
  { 
	  *posGlobal = Vector( UnknownDoubleValue,UnknownDoubleValue );
	  return true;
  }*/
  
  return true;
}

bool WorldModel::calculateStatePlayer(ObjectT o, Vector *posGlobal, Vector *velGlobal)
{
 Player  *pob = (Player*) getObjectPtrFromType( o );
  
 //double  timediff = getCurrentTime().getTime() - 
//	                pob->getTimeGlobalPosDerivedFromSee().getTime();//begintime.getTime() ;
 //Vector  lastpos = pob->getGlobalPositionLastSee();

  if( pob == NULL )
    return false;
  Vector posRelWorld =
      Vector( getRelativeDistance( o ),
                   getRelativeAngle( o ) + getAgentGlobalBodyAngle(),
                   POLAR );
  *posGlobal = getAgentGlobalPosition() + posRelWorld;

  //*velGlobal = getGlobalVelocity( o ) ; 
//  Vector  posdiff = lastpos - *posGlobal;
 // *velGlobal = Vector( posdiff.X/timediff*1000, posdiff.Y/timediff*1000);
  
  if( velGlobal->getMagnitude() >= 300    )
    velGlobal->setMagnitude( 300 );
 // pob->setGlobalVelocity( *velGlobal,getCurrentTime() );
  return true;
	
}
 
void WorldModel::initParticlesAgent(Vector posInitial)
{
  for( int i = 0; i < iNrParticlesAgent; i++ )
    particlesPosAgent[i] = posInitial;
}

void WorldModel::initParticlesAgent(AngDeg angGlobal)
{
  double  dDist, dMaxRadius, dMinRadius, dInput;
  AngDeg  ang;

  // get closest flag from which samples will be generated
  ObjectT objFlag = getClosestRelativeInSet( OBJECT_SET_FLAGS );

 if( objFlag == OBJECT_ILLEGAL )
 {
     return;
 }

  // get the distance to this flag and the possible range it was derived from.
  dInput = getRelativeDistance( objFlag );
  getMinMaxDistQuantizeValue( dInput, &dMinRadius, &dMaxRadius,
                                               QUANTIZESTEP, 0.1 ) ;//SS->getQuantizeStepL()

  // get the perceived angle to this flag (add 180 to get angle relative from
  // flag to agent ) and make it global by adding global neck angle agent.
  AngDeg angFlag   = getRelativeAngle( objFlag ) + 180 + angGlobal ;
  // for all particles
  for( int i = 0 ; i < iNrParticlesAgent ; i++ )
  {
    // determine random point from distance range and
    // determine random point from the range it could be generated from
    // angles are rounded and since noise is in global neck angle and relative
    // angle flag, maximum error is in range [-0.5,0.5] + [-0.5,0.5] = [-1,1].
    dDist = dMinRadius + drand48()*(dMaxRadius-dMinRadius);
    ang   = Vector::normalizeAngle( angFlag - 1.0 + 2*drand48() );

    // create random point from possible interval
    particlesPosAgent[i] = getGlobalPosition( objFlag ) +
                           Vector( dDist, ang, POLAR );
  }
}

int WorldModel::checkParticlesAgent( AngDeg angGlobalNeck )
{
  double dMaxRadius, dMinRadius, dInput, dDist;
  AngDeg ang;
  int     iNrLeft = iNrParticlesAgent, iLength = iNrParticlesAgent;

  // for all current perceived flags
  for( int i = 0; i < 6; i++ )
  {
    iNrLeft = 0;                        // reset number of correct particles
	ObjectT o = Flags[i].getType( ); 

    dInput = getRelativeDistance( o );  // get possible distance range
    getMinMaxDistQuantizeValue( dInput, &dMinRadius, &dMaxRadius,
                                    QUANTIZESTEP, 0.1 )  ; 

    // find all "correct points"
    for( int i = 0; i < iLength; i ++ )
    {
      // determine distance particle to flag
      // determine difference in direction between direction between global
      // flag and agent position and global perceived direction
      dDist = particlesPosAgent[i].getDistanceTo( getGlobalPosition( o ) );
      ang = (getGlobalPosition(o) - particlesPosAgent[i]).getDirection();
      ang = ang - ( getRelativeAngle( o ) + angGlobalNeck );

      // if in possible range, save it (maximum angle range is 0.5 for
      // neck angle and 0.5 for relative flag angle gives 1.0)
      if( dDist > dMinRadius && dDist < dMaxRadius )// &&
          //fabs(Vector::normalizeAngle( ang )) <= 1.0 )
        particlesPosAgent[iNrLeft++] = particlesPosAgent[i];
    }
    // change maximum of correct particles
    iLength = iNrLeft;
  }
  return iNrLeft;
}

void WorldModel::updateParticlesAgent(Vector vel, bool bAfterSense)
{
  AngDeg ang = MyInfo.BodyFacing;
  vel *= CONTROL_INTERVAL;
  vel.rotate(ang);
  for( int i = 0; i < iNrParticlesAgent  ; i ++ )
  {
     particlesPosAgent[i].setX( particlesPosAgent[i].getX( ) + vel.getX() );
     particlesPosAgent[i].setY( particlesPosAgent[i].getY( ) + vel.getY() );
  }
}

Vector WorldModel::averageParticles(Vector posArray[], int iLength)
{
 if( iLength == 0 )
    return Vector( UnknownDoubleValue, UnknownDoubleValue );

  // take average of particles
  double x = 0, y = 0;
  for( int i = 0; i < iLength  ; i ++ )
  {
     x += posArray[ i ].getX( );
     y += posArray[ i ].getY( );
  }

  return Vector( x/iLength, y/iLength );
}

void WorldModel::resampleParticlesAgent(int iLeft,Vector pos)
{
/*  double SIGMA,possum=0,sum2 = 0;
  Vector daverage = pos;
  for ( int j = 0; j < iLeft; j++)
  {
      possum += pow( particlesPosAgent[ j ].getDistanceTo( daverage ),2);
  }
  double a = sqrt( (1/(double)(iLeft) ) * possum );
  double ba1 = pow(  1/(double)(iLeft), 0.167 );
  SIGMA = (0.0833) * a * pow( 1/(double)(iLeft), 0.167 );
  Vector posSample = particlesPosAgent[ (int)(drand48()*iLeft) ];
  for (int i = iLeft; i < iNrParticlesAgent; i++)
  {
     for ( int k = 1; k <=12; k++)
 		 sum2 += drand48();
  	 particlesPosAgent[ i ] = posSample + SIGMA * ( sum2 - 6 );
  }*/
   for ( int i = iLeft; i < iNrParticlesAgent; i ++ )
     particlesPosAgent[ i ] = particlesPosAgent[ (int)(drand48()*iLeft) ];
}

double WorldModel::invQuantizeMin(double put, double step)
{
 return max( 1.0e-10, ( rint(  put / step )-0.5 ) * step );
}
 
double WorldModel::invQuantizeMax(double put, double step)
{
 return ( rint(  put / step ) + 0.5 ) * step;
}

bool WorldModel::getMinMaxDistQuantizeValue(double dOutput, double *dMin, double *dMax, double x1, double x2)
{
  dOutput -= 1.0e-10;
  if( dOutput < 0 )
    *dMin = 0.0;
  else
    *dMin = exp( invQuantizeMin( log( invQuantizeMin(dOutput,x2) ), x1 ) );
  dOutput += 2.0e-10;
  *dMax = exp( invQuantizeMax( log( invQuantizeMax(dOutput,x2) ), x1 ) );
  return true;
}

bool WorldModel::getMinMaxDirChange(double dOutput, double *dMin, double *dMax, double x1)
{
 *dMin = invQuantizeMin( dOutput, x1 ) ;
 *dMax = invQuantizeMax( dOutput, x1 ) ;
 return true;
}

bool WorldModel::getMinMaxDistChange(double dOutput, double dDist, double *dMin, double *dMax, double x1, double xDist1, double xDist2)
{
  double dMinDist, dMaxDist;
  getMinMaxDistQuantizeValue( dDist, &dMinDist, &dMaxDist, xDist1, xDist2 );
  dOutput = dOutput/dDist;
  double dMinCh = invQuantizeMin( dOutput, x1 ) ;
  double dMaxCh = invQuantizeMax( dOutput, x1 ) ;
  *dMin = min( dMinDist*dMinCh, dMaxDist*dMinCh );
  *dMax = max( dMinDist*dMaxCh, dMaxDist*dMaxCh );
  return true;
}