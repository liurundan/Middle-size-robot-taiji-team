#ifndef _GOALIE_TAIJI_H
#define _GOALIE_TAIJI_H

#include "MoveWithBall.h"
#include "BallGet.h"
#include "InterceptBall.h"
#include "BasicCmd.h"
#include "Dribble.h"
#include "CmdGenerator.h"
#include "FaceBall.h"
#include "GotoPos.h"
#include "Offence.h"
#include "Defense.h"
#include "SPhysGotoPos.h"
#include "ApproachBall.h"
#include "PhysicalMotionModel.h"
#include "StuckBehavior.h"
#include "SearchBall.h"

class GoalieTaiji : public CmdGenerator
{
public:
	GoalieTaiji( ConfigReader& cfg);
	~GoalieTaiji();
  	void getCmd(DriveVector *dv, const Time& t);
	void decide_action( DriveVector* dv,const Time& t );
	void PositionNormal(DriveVector* dv,const Time& t);
	void PositionHisPK(DriveVector* dv,const Time& t);
    void PositionMyPK(DriveVector* dv,const Time& t);
	void RunMyPK(DriveVector* dv,const Time& t);
	void RunHisPK(DriveVector* dv,const Time& t);
	Vec  getBestPosition(const Time& t);
    bool isDeadBallOwn();
	bool isDeadBallOpp();
	int  is_ball_heading_for_goal(const Time& t);
    int  ball_steps_to(Vec pos,const Time& t);
	int  goalie_steps_to(Vec pos,const Time& t);
    Vec  expected_ball_pos(int steps,const Time& t);
	bool is_destination_reachable(const Vec& destination, Vec my_pos,Vec my_vel,
									int steps, const Time& t  );
	bool intercept_goalie(DriveVector* dv, const Time& t);
	void noise_cancel(const Time& t);
	
	void gotoBallforGoalie(double Velocity,DriveVector* dv, const Time& t);
	void decide_action2(DriveVector* dv, const Time& t);

	void test_skills(DriveVector* dv,const Time& t);
private:
	MoveWithBall*  move_with_ball;
	BasicCmd*      basic_cmd;
	Dribble*       dribble;
	BallGet*       getBall;
	InterceptBall* intercept;
	FaceBall*      face_ball;
	GotoPos*       go_pos;
	Offence*       offence;
	DefenseBehavior* defense;
	Avoid*           avoid;
	SPhysGotoPos*   go2pos;
	PhysicalMotionModel* motion;
	ApproachBall*   approach;
    StuckBehavior*  stuck;
	SearchBall*     searchball;

	Vec             id_Punkt_auf_Tor;
	
	double			home_x_pos;
	double			circle_half_width;
	double			attack_area_radius;
	double			attack_area_angle;
	double		    reference_points_x;
	double          reference_points_y;
	double			distance_x;
	double			max_turning;
	bool            kick_permission;
    
 
};
#endif