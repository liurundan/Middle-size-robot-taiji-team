#include "StdAfx.h"
#include "SearchBall.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "Tools.h"
#include <cmath>

SearchBall::SearchBall() : CmdGenerator("searchBall")
{
	get_ball	= new BallGet();
	intercept	= new InterceptBall();
	go2pos		= new SPhysGotoPos();
	go_pos      = new GotoPos();
	avoid		= new Avoid();
	basic_cmd	= new BasicCmd();
	move_with_ball = new MoveWithBall();
	face        = new FaceBall();
}

SearchBall::~SearchBall()
{
	delete go_pos;
	delete get_ball;
	delete basic_cmd;
	delete avoid;
	delete intercept;
	delete move_with_ball;
	delete go2pos;
	delete face;
}

void SearchBall::getCmd(DriveVector *dv, const Time& t)
{
	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t);  
	Vec posBall = bloc.pos;
	Vec posMe   = rloc.pos;
	Angle angBall = ( posBall - posMe ).angle();

	static int flag = 0;
    if( bloc.pos_known != 0 || bloc.quality > 0.1 )
		return;
    Vec   Ball_left  = posBall + (posBall - posMe).rotate(-1.0).normalize() * 2500;
    Vec   Ball_right = posBall + (posBall - posMe).rotate(1.0).normalize() * 2500;
    Vec   posDest = Tools::sign(posBall.y) > 0 ?Ball_right : Ball_left;

    PlayerRole role = WBOARD->getPlayerRole();
	bool team_owns_ball = WBOARD->advancedTeamPossesBall();
	if( flag == 0 )
	{
	    flag = 1;
		update_time.update();
	}
	else if( flag == 1 )
	{
        if( update_time.elapsed_msec() > 10000 && update_time.elapsed_msec() < 15000 ) 
		{
			if( !team_owns_ball )
			{
               go2pos->set_target(true,posDest,angBall,1.0,8.0,1);
			   go2pos->getCmd(dv,t);
			}
        }
		else if ( update_time.elapsed_msec() > 15000 )
		{
		    move_with_ball->Move2HomePosition(t);
			move_with_ball->getCmd(dv,t);
			flag = 0;
		}
	}

}

bool SearchBall::is_searching(const Time& t)
{
  	RobotLocation rloc = MWM.get_robot_location(t);  
	BallLocation bloc  = MWM.get_ball_location(t);  
    if( bloc.pos_known != 0 && bloc.quality > 0.1 )
	{
		return false;
    }
	return true;
}