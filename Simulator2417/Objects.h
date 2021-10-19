#ifndef _OBJECTS_
#define _OBJECTS_
#include "SoccerTypes.h"
#include "geometry.h"	// Added by ClassView

/*****************************************************************************/
/********************** CLASS OBJECT *****************************************/
/*****************************************************************************/

/*! Class Object contains RoboCup information that is available for all objects
    in the simulation. All (relative) information is relative to an agent as
    declared in AgentObject. Update of an object (or one of the subclasses)
    happens by calling the standard get and set methods available in these
    classes. Calculations on these attributes do not occur in these classes,
    but in the update methods of the WorldModel. */
class Object
{
public:
	Vector  GlobalPos;
	Vector  RelPos;
public:
	bool setRelativeposLastSeen( Vector p,int c );
	double getConfidence2( int cycle );
	int    getCycleLastSeen() const;
    Vector  getRelativeposLastSeen() const;
    bool   setGlobalPositionLastSee(Vector p, int cycle );

	bool    setGlobalPosition( Vector p, Time time );
	Vector  getRelativePosition() const;
	bool    setRelativePosition( Vector v, Time time );
	Time    getTimeRelativePosition() const;
	bool    setTimeRelativePosition( Time time );
	bool    setRelativePosition( double dDist, AngDeg ang, Time time );
	ObjectT getType() const;
	bool    setType( ObjectT o );
	AngDeg  getRelativeAngle( );
	Time    getTimeLastSeen() const;
	bool    setTimeLastSeen( Time time);
 	Vector  getGlobalPositionLastSee() const;
	bool    setTimeGlobalPosDerivedFromSee( Time time );
	bool    setGlobalPositionLastSee( Vector p, Time time );
	Time    getTimeGlobalPosition() const;
	bool    setTimeGlobalPosition( Time time );
	Vector  getGlobalPosition() const;
	double  getRelativeDistance( );
	double  getConfidence( Time time );
	
	Object();

protected:
	Vector posGlobalLastSee;
	Vector posRelativeLastSee;
	Time timeGlobalPosDerivedFromSee;
	Time timeGlobalPosition;
	Time timeLastSeen;
	Time timeRelativePosition;
	int  cycleLastSeen;
	ObjectT objectType;
};
/*****************************************************************************/
/********************** CLASS FIXEDOBJECT ************************************/
/*****************************************************************************/

/*! Class FixedObject contains RoboCup information that is available for
    objects that cannot move (flags, goals, lines). No additional information
    is added to the superclass Object. */
class FixedObject : public Object
{
public:
	Vector getGlobalPosition( SideT s, double dGoalWidth = 200.0 )const;
	//FixedObject();
};
/*****************************************************************************/
/********************** CLASS DYNAMICOBJECT **********************************/
/*****************************************************************************/

/*! Class DynamicObject contains RoboCup information that is available for
    objects that can move (players, ball). Different variables are added to
    the superclass Object */
class DynamicObject : public Object
{
	Vector posPositionDifference;
public:
	Vector RelVel;
	
public:
	Vector getPositionDifference() const;
	bool   setPositionDifference(Vector p);
	
	//Vector getGlobalPosLastSee ( );
	//bool setGlobalPosLastSee( Vector pos );
	bool   setGlobalVelocity( Vector v, Time time);
	Vector getGlobalVelocityLastSee ( )   const;
	bool   setGlobalVelocityLastSee ( Vector vec );
	double getRelativeAngleChange() const;
	bool   setRelativeAngleChange( double d, Time time );
	double getRelativeDistanceChange() const;
	Time   getTimeChangeInformation()  const;
	bool   setTimeChangeInformation( Time time );
	bool   setRelativeDistanceChange( double d, Time time );
	Time   getTimeGlobalVelocity() const;
	bool   setTimeGlobalVelocity( Time time );
	double getSpeed( ) const;
	Vector getGlobalVelocity( ) const;
	Vector getGlobalVelRotate() const;
	bool   setGlobalVelRotate( Vector vec,Time time);
	bool   setTimeGlobalVelRotate( Time time );
	DynamicObject();
protected:
	Vector vecGlobalVelocityLastSee;
	Vector GlobalPosLastSee;
	Time   timeChangeInformation;
	Time   timeGlobalVelocity;
	Time   timeGlobalVelRotate;
	double dRelativeDistanceChange;
	double dRelativeAngleChange;
	Vector GlobalVel;
    Vector GlobalVelRotate;
};

/*****************************************************************************/
/********************* CLASS BALL*********************************************/
/*****************************************************************************/
 
class Ball : public DynamicObject
{
public:
public:
	Ball();
};
/*****************************************************************************/
/********************* CLASS PLAYER ******************************************/
/*****************************************************************************/

/*! Class PlayerObject contains RoboCup information that is available for
    players. Different variables are added to the superclass DynamicObject   */
class Player : public DynamicObject
{
public:
	Angle BodyFacing;
	double w;
public:
	bool setSpeedRelToBody( Vector v );
	Vector getSpeedRelToBody( ) const;
	bool setPositionDifference( Vector p );
	Vector getPositionDifference() const;
	Time getTimeRelativeAngles( ) const;
	bool setIsGoalie( bool b );
	bool setIsKnownPlayer( bool b );
	Time getTimeGlobalAngles( ) const;
	bool setGlobalBodyAngleLastSee( AngDeg ang );
	bool setTimeGlobalAngles( Time time );
	bool setGlobalBodyAngle( AngDeg ang, Time time);
	bool setTimeRelativeAngles( Time time );
	bool   setRelativeBodyAngle( AngDeg ang, Time time );
	AngDeg getRelativeBodyAngle( )  const;
	bool   getIsKnownPlayer() const;
	bool   getIsGoalie() const;
	AngDeg getGlobalBodyAngleLastSee( )  const;
	AngDeg getGlobalBodyAngle( )  const;
 	Player();
protected:
	Time   timeGlobalAngles;
	AngDeg angGlobalBodyAngleLastSee;
 	bool   isGoalie;
	bool   isKnownPlayer;
 private:
	 Vector posPositionDifference;
	 Vector velSpeedRelToBody;
	 Time    timeRelativeAngles;
	 AngDeg angRelativeBodyAngle;
};
#endif