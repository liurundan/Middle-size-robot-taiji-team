#include "stdafx.h"
#include "WorldModel.h"
 
Vector WorldModel::predictPosAfterNrCycles(ObjectT o,double dCycles, Vector velocity,  Vector *posIn, Vector *velIn, bool bUpdate)
{
  Vector vel = ( velIn == NULL ) ? getGlobalVelocity( o ) : *velIn ;
  Vector pos = ( posIn == NULL ) ? getGlobalPosition( o ) : *posIn ;
  double dBallDecay = 0.94;
  
  if( o == OBJECT_BALL )
  {
    // get the speed and the distance it travels in iCycle's.
    // use this distance and direction it travels in, to calculate new pos
    // geom series is serie s=a+ar+..+ar^n...decay=r,iCycles=n,dSpeed=a
    double dDist = Geometry::getSumGeomSeries( vel.getMagnitude(),
                                               dBallDecay,
                                               dCycles);
    pos          += Vector( dDist, vel.getDirection(), POLAR );
    vel          *= pow( dBallDecay, dCycles );
  }
  else if( SoccerTypes::isKnownPlayer( o )  )
  {
    if( getAgentObjectType() == o )
    {
       for( int i = 0; i < (int)dCycles ; i ++ )
        predictStateAfterDash( velocity, &pos, &vel );
	}
  
  }
  
  if( posIn != NULL && bUpdate )
    *posIn = pos;
  if( velIn != NULL && bUpdate )
    *velIn = vel;
  return pos;
}
 
Vector WorldModel::predictFinalAgentPos(Vector *pos, Vector *vel)
{
  Vector velAgent   = (vel==NULL) ? getAgentGlobalVelocity (): *vel;
  Vector posAgent   = (pos==NULL) ? getAgentGlobalPosition (): *pos;
  double dPlayerDecay = 0.40;
  double      dDistExtra =
   Geometry::getSumInfGeomSeries( velAgent.getMagnitude(), dPlayerDecay );
  return posAgent + Vector( dDistExtra,velAgent.getDirection(), POLAR );
}
 
void WorldModel::predictStateAfterDash( Vector velocity, Vector *pos, Vector *vel)
{
  double dPlayerDecay = 0.40;
  *vel += velocity;
  if( vel->getMagnitude() > 300 )
    vel->setMagnitude( 300 );
  *pos += *vel;
  *vel *= dPlayerDecay;
}
 
Vector WorldModel::predictAgentPos(int iCycles, Vector velocity)
{
   return predictPosAfterNrCycles( getAgentObjectType(), iCycles, velocity   );
}
