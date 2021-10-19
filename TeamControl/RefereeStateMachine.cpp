#include "StdAfx.h"
#include "RefereeStateMachine.h"

RefereeStateMachine::RefereeStateMachine (const FieldGeometry& fg, int tc, RefereeState rs) throw () 
{
  team = tc;
  current = rs;
  ballknown = false;
  penalty_marker_y = -0.5*fg.field_length+fg.penalty_marker_distance;
  center_circle_radius = fg.center_circle_radius;
}

RefereeStateMachine::RefereeStateMachine (const RefereeStateMachine& src) throw () 
{
  team = src.team;
  current = src.current;
  latest_update = src.latest_update;
  ballpos = src.ballpos;
  ballknown = src.ballknown;
  penalty_marker_y = src.penalty_marker_y;
}

const RefereeStateMachine& RefereeStateMachine::operator= (const RefereeStateMachine& src) throw () 
{
  team = src.team;
  current = src.current;
  latest_update = src.latest_update;
  ballpos = src.ballpos;
  ballknown = src.ballknown;
  penalty_marker_y = src.penalty_marker_y;
  return (*this);
}

RefereeState RefereeStateMachine::get_state () const throw () 
{
  return current;
}

void RefereeStateMachine::set_state (RefereeState rs) throw () 
{
  current = rs;
}

void RefereeStateMachine::set_team_color (int tc) throw () 
{
  team = tc;
}

RefboxSignal RefereeStateMachine::update (RefboxSignal sig, Vec ball, bool bk) throw () 
{
  
  if (team>0) 
  {  // cyan=own magenta=opponent
    switch (sig) 
	{
    case SIGcyanKickOff: sig=SIGownKickOff; break;
    case SIGmagentaKickOff: sig=SIGopponentKickOff; break;
    case SIGcyanFreeKick: sig=SIGownFreeKick; break;
    case SIGmagentaFreeKick: sig=SIGopponentFreeKick; break;
    case SIGcyanGoalKick: sig=SIGownGoalKick; break;
    case SIGmagentaGoalKick: sig=SIGopponentGoalKick; break;
    case SIGcyanCornerKick: sig=SIGownCornerKick; break;
    case SIGmagentaCornerKick: sig=SIGopponentCornerKick; break;
    case SIGcyanThrowIn: sig=SIGownThrowIn; break;
    case SIGmagentaThrowIn: sig=SIGopponentThrowIn; break;
    case SIGcyanPenalty: sig=SIGownPenalty; break;
    case SIGmagentaPenalty: sig=SIGopponentPenalty; break;
    case SIGcyanGoalScored: sig=SIGownGoalScored; break;
    case SIGmagentaGoalScored: sig=SIGopponentGoalScored; break;
    default: break;
    }
  } 
  else 
  {  // cyan=opponent, magenta=own
    switch (sig) 
	{
    case SIGcyanKickOff: sig=SIGopponentKickOff; break;
    case SIGmagentaKickOff: sig=SIGownKickOff; break;
    case SIGcyanFreeKick: sig=SIGopponentFreeKick; break;
    case SIGmagentaFreeKick: sig=SIGownFreeKick; break;
    case SIGcyanGoalKick: sig=SIGopponentGoalKick; break;
    case SIGmagentaGoalKick: sig=SIGownGoalKick; break;
    case SIGcyanCornerKick: sig=SIGopponentCornerKick; break;
    case SIGmagentaCornerKick: sig=SIGownCornerKick; break;
    case SIGcyanThrowIn: sig=SIGopponentThrowIn; break;
    case SIGmagentaThrowIn: sig=SIGownThrowIn; break;
    case SIGcyanPenalty: sig=SIGopponentPenalty; break;
    case SIGmagentaPenalty: sig=SIGownPenalty; break;
    case SIGcyanGoalScored: sig=SIGopponentGoalScored; break;
    case SIGmagentaGoalScored: sig=SIGownGoalScored; break;
    default: break;
    }
  }

  switch (sig) 
  {
  case SIGstop: 
  case SIGhalt:
    current = stopRobot;
    break;
  case SIGstart:
    current = freePlay;
    break;
  case SIGready:
    switch (current) 
	{
    case preOwnKickOff:
    case preOwnFreeKick:
    case preOwnGoalKick:
    case preOwnCornerKick:
    case preOwnThrowIn:
	case preDroppedBall:
      current = freePlay;
      break;
    case preOwnPenalty:
      current = ownPenalty;
      break;
    case preOpponentKickOff:
      current = postOpponentKickOff;
      ballpos = ball;
      ballknown = bk;
      break;
    case preOpponentFreeKick:
      current = postOpponentFreeKick;
      ballpos = ball;
      ballknown = bk;
      break;
    case preOpponentGoalKick:
      current = postOpponentGoalKick;
      ballpos = ball;
      ballknown = bk;
      break;
    case preOpponentCornerKick:
      current = postOpponentCornerKick;
      ballpos = ball;
      ballknown = bk;
      break;
    case preOpponentThrowIn:
      current = postOpponentThrowIn;
      ballpos = ball;
      ballknown = bk;
      break;
    case preOpponentPenalty:
      current = postOpponentPenalty;
      ballpos = ball;
      ballknown = bk;
      break;
    default:
      return sig;  
    }
    break;
  case SIGownKickOff:
    current = preOwnKickOff;
    break;
  case SIGopponentKickOff:
    current = preOpponentKickOff;
    break;
  case SIGownFreeKick:
    current = preOwnFreeKick;
    break;
  case SIGopponentFreeKick:
    current = preOpponentFreeKick;
    break;
  case SIGownGoalKick:
    current = preOwnGoalKick;
    break;
  case SIGopponentGoalKick:
    current = preOpponentGoalKick;
    break;
  case SIGownCornerKick:
    current = preOwnCornerKick;
    break;
  case SIGopponentCornerKick:
    current = preOpponentCornerKick;
    break;
  case SIGownThrowIn:
    current = preOwnThrowIn;
    break;
  case SIGopponentThrowIn:
    current = preOpponentThrowIn;
    break;
  case SIGownPenalty:
    current = preOwnPenalty;
    break;
  case SIGopponentPenalty:
    current = preOpponentPenalty;
    break;
  case SIGdroppedBall:
	current = preDroppedBall;
  default:
    return sig;   
  }
  latest_update.update();
  return sig;
}

void RefereeStateMachine::update (Vec ball, Vec robot, bool bk) throw () 
{
  if (current==postOpponentKickOff || current==postOpponentFreeKick || 
	  current==postOpponentGoalKick || current==postOpponentCornerKick  
	  || current==postOpponentThrowIn) 
  {
    if (latest_update.elapsed_msec ()>=10000) 
	{
      current = freePlay;
      latest_update.update();
    }
  }
  if (current==postOpponentKickOff || current==postOpponentFreeKick || 
	  current==postOpponentGoalKick || current==postOpponentCornerKick  || 
	  current==postOpponentThrowIn || current==postOpponentPenalty) 
  {
    if (ballknown && bk) 
	{
      double len = (ballpos-ball).length();
      if ((len>500 && (robot-ball).length()<2500) || (len>1000)) 
	  {
		if (current==postOpponentPenalty)
		  current = opponentPenalty;
		else
		  current = freePlay;
		latest_update.update();
      }
    }
  }
  if (current==postOpponentPenalty) 
  {
    if (bk && ball.y<penalty_marker_y-500)
      current = opponentPenalty;
  }
  if (current == postOpponentKickOff) 
  {
    if ((bk && ball.length()>0.7*center_circle_radius && robot.length()<2500) || 
		(bk && ball.length()>1.2*center_circle_radius))
      current = freePlay;
  }
}