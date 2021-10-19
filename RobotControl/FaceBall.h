#ifndef _FACE_BALL_H_
#define _FACE_BALL_H_

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "SPhysGotoPos.h"
#include "PhysicalMotionModel.h"

class FaceBall : public CmdGenerator
{
public:
	 FaceBall();
	~FaceBall();
     void turn_to_ball(	double omega = 1.0, bool tolerance_heading = true,  
						bool pd_control = false );
	 void turn_in_dir(	Angle dir,double omega = 1.0,bool tolerance_heading = true,
						bool pd_control = false);
	 void turn_to_point(	Vec target, double omega = 1.0,bool tolerance_heading = true, 
							bool pd_control = false );
	 void getCmd(DriveVector *dv, const Time& t);
	 void getCmd(DriveVector *dv, const Time& t,bool pd_control);
	 
private:
	Angle     desired_dir;
	double    omega;
	BasicCmd* basic_cmd;
    bool      turn_body;
	bool      tolerance_heading;
    bool      use_pd_control;
	double    abs_turnangle(Angle h);
};
#endif