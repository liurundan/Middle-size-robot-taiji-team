#ifndef _InterceptBall_h
#define _InterceptBall_h

#include "CmdGenerator.h"
#include "PhysicalMotionModel.h"
#include "SPhysGotoPos.h"
#include "MoveWithBall.h"
#include "RobotLocation.h"
#include "BallLocation.h"
#include "Avoid.h"

class InterceptBall : public CmdGenerator
{
public:
	InterceptBall();
	~InterceptBall();
	void   getCmd(DriveVector* dv, const Time& t) throw();
  	
	Angle  my_angle_to(const Vec& my_pos, const Angle& my_angle, Vec target );
    
	bool   is_destination_reachable2(const Vec& destination, Vec my_pos, Vec my_vel, int maxcycles );
    bool   intercept_close(DriveVector* dv, const RobotLocation&rloc, const BallLocation& bloc);
	
protected:
	
	PhysicalMotionModel motion;
    SPhysGotoPos		goto_pos_skill;  

	Vec					current_position;              
    Angle				current_heading;              
	
    Vec					target_pos;
    Angle				target_heading;
	
	Vec					posIntercept;
    Angle				goAngle;
    
	Vec					target_vtrans;
	double				target_vrot;

    double				max_target_velocity;
    bool				tolerance_pos;
    bool				tolerance_heading;
    MoveWithBall*       move_with_ball; 
    Avoid*              avoid;

private:
	Angle ivRequestedTurnAngle;
    unsigned int intercept_max_cycles;

};

#endif
