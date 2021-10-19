#include "StdAfx.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "Tools.h"
#include <cmath>

Angle Tools::get_angle_between_mPI_pPI(Angle angle) 
{
	while (angle.get_rad() >= M_PI) angle -= 2*M_PI;
	while (angle.get_rad() < -M_PI) angle += 2*M_PI;
	return angle;
}

Angle Tools::get_angle_between_null_2PI(Angle angle) 
{
	while (angle.get_rad() >= 2*M_PI) angle -= 2*M_PI;
	while (angle.get_rad() < 0) angle += 2*M_PI;
	return angle;
}

Angle Tools::get_abs_angle(Angle angle)
{
	Angle h;
	h = get_angle_between_null_2PI(angle);
	if(h.get_rad()>M_PI)
		h -= 2*M_PI;
	return(fabs(h.get_rad()));
}

Angle Tools::my_angle_to(Vec target) // relative angle
{
	Angle h;
	Vec pos_me = MWM.posVec_me;
	target -= pos_me;
	h = target.angle() - MWM.heading_me;
	return h;
}

Angle Tools::my_abs_angle_to(Vec target) // pos abs angle
{
	Angle h;
	h = my_angle_to(target);
	return h + MWM.heading_me;
}

Vec Tools::get_Lotfuss(Vec x1, Vec x2, Vec p)
{
	Vec r = x1 - x2;
	Vec a = x1 - p;
	double l = -(a.x*r.x+a.y*r.y)/(r.x*r.x+r.y*r.y);
	Vec ergebnis = x1 + l*r;
	return ergebnis;
}

double Tools::get_dist2_line(Vec x1, Vec x2, Vec p)
{
	return ( get_Lotfuss(x1,x2,p) - p ).normalize().length();
}

Vec Tools::point_on_line(Vec steigung, Vec line_point, double x) 
{
	steigung = (1.0/steigung.x) * steigung;
	if (steigung.x > 0) 
	{
		return (x - line_point.x) * steigung + line_point;  
	}
	if (steigung.x < 0) 
	{
		return (line_point.x - x) * steigung + line_point;  
	}  
	return line_point;
} 

Vec Tools::intersection_point(Vec p1, Vec steigung1, 
							  Vec p2, Vec steigung2) 
{
	double x, y, m1, m2;
	if ((steigung1.x == 0) || (steigung2.x == 0)) 
	{
		if (fabs(steigung1.x) < 0.00001) 
		{
			return point_on_line(steigung2, p2, p1.x);
		}
		else if (fabs(steigung1.x) < 0.00001) 
		{
			return point_on_line(steigung1, p1, p2.x);
		} 
	}
	m1 = steigung1.y/steigung1.x;
	m2 = steigung2.y/steigung2.x;
	if (m1 == m2) 
		return Vec(-18000.0, 0);
	x = (p2.y - p1.y + p1.x*m1 - p2.x*m2) / (m1-m2);
	y = (x-p1.x)*m1 + p1.y;
	return Vec (x, y);
	
}

bool Tools::intersection(const Vec & r_center, double size_x, double size_y,
						 const Vec & l_start, const Vec & l_end)
{
	
	double p1_x= l_start.x - r_center.x;
	double p1_y= l_start.y - r_center.y;
	double p2_x= l_end.x   - r_center.x;
	double p2_y= l_end.y   - r_center.y;
	
	size_x *= 0.5;
	size_y *= 0.5;
	
	double diff_x= (p2_x - p1_x);
	double diff_y= (p2_y - p1_y);
	
	if ( fabs(diff_x) >= 0.0001) 
	{
		double N= (size_x - p1_x) / diff_x;
		double Y= p1_y + N * diff_y;
		if (0.0 <=  N  && N <= 1 && Y <= size_y && Y >= -size_y) return true;
		
		N= (-size_x - p1_x) / diff_x;
		Y= p1_y + N * diff_y;
		if (0.0 <=  N  && N <= 1 && Y <= size_y && Y >= -size_y) return true;
	}
	
	if ( fabs(diff_y) >= 0.0001) {
		double N= (size_y - p1_y) / diff_y;
		double X= p1_x + N * diff_x;
		if (0.0 <=  N  && N <= 1 && X <= size_x && X >= -size_x) return true;
		
		N= (-size_y - p1_y) / diff_y;
		X= p1_x + N * diff_x;
		if (0.0 <=  N  && N <= 1 && X <= size_x && X >= -size_x) return true;
	}
	return false;
}      

bool Tools::point_in_triangle(const Vec & p, const Vec & t1, const Vec & t2, const Vec & t3) 
{
	double A= t2.x - t1.x;
	double B= t3.x - t1.x;
	double C= t2.y - t1.y;
	double D= t3.y - t1.y;
	
	double det= A * D - C * B;
	if ( fabs(det) < 0.000001 ) 
	{ 
		return false;
	}
	
	double x= p.x - t1.x;
	double y= p.y - t1.y;
	
	
	double a= D * x - B * y;
	double b= -C * x + A * y;
	
	a/= det;
	b/= det;
	
	if (a < 0 || b < 0) 
	{
		return false;
	}
	if ( a + b > 1.0) 
	{
		return false;
	}
	return true;
}

bool Tools::point_in_rectangle(const Vec & p, const Vec & r1, const Vec & r2, const Vec & r3, const Vec & r4) 
{
	if ( point_in_triangle(p,r1,r2,r3) )
		return true;
	if ( point_in_triangle(p,r1,r2,r4) )
		return true;
	if ( point_in_triangle(p,r1,r3,r4) )
		return true;
	if ( point_in_triangle(p,r2,r3,r4) )
		return true;
	return false;
}

bool Tools::point_in_field(const Vec & p, double extra_margin) 
{
	FieldGeometry fg = MWM.get_field_geometry();
	double border_x= fg.field_length/2.0 + extra_margin;
	double border_y= fg.field_width/2.0  + extra_margin;
	return 
		p.x <= border_x   && 
		p.x >= -border_x &&
		p.y <= border_y && 
		p.y >= -border_y;
}

bool Tools::is_position_in_pitch(Vec position,  const float safety_margin)
{
	FieldGeometry fg = MWM.get_field_geometry();
	if ( (position.x-safety_margin < -fg.field_length/2.0) 
		|| (position.x+safety_margin > fg.field_length/2.0) ) return false;
	if ( (position.y-safety_margin < -fg.field_width/2.0) 
		|| (position.y+safety_margin > fg.field_width/2.0) ) return false;
	return true;
}

double Tools::min_distance_to_border(const Vec position)
{
	FieldGeometry fg = MWM.get_field_geometry();
	return min(fg.field_length/2.0 - fabs(position.x), fg.field_width / 2. - fabs(position.y));
}

bool Tools::is_a_scoring_position(Vec pos)
{
	FieldGeometry fg = MWM.get_field_geometry();
	const XYRectangle hot_scoring_area( Vec(fg.field_length/2.0 - 3000, -3000),
		Vec(fg.field_length/2.0, 3000)); 
	
	double dist2goal = (Vec(fg.field_length/2.0,0) - pos).length();
	if(dist2goal > 6000.0)
	{
		return false;
	}
	
	WSpset pset = MWM.valid_opponents;
	Vec opgoalpos =Vec(fg.field_length/2.0,0.);
	double scanrange = 6000.0;
	Quadrangle check_area = Quadrangle(pos,opgoalpos,scanrange/2., scanrange);
	pset.keep_players_in(check_area);
	if(pset.num == 0 || pset.num == 1 || pset.num < 2)
	{
		return true;
	}
	
	if(dist2goal > 4000.0)
	{
		return false;
	}
	pset = MWM.valid_opponents;
	scanrange = 4000.0;
	check_area = Quadrangle(pos,opgoalpos,scanrange, scanrange);
	pset.keep_players_in(check_area);
	if(pset.num < 2)
	{
		return true;
	}
	
	pset = MWM.valid_opponents;
	check_area = Quadrangle(pos,Vec(fg.field_length/2.0,+3000.),scanrange, scanrange);
	pset.keep_players_in(check_area);
	if(pset.num < 2)
	{
		return true;
	}
	
	pset = MWM.valid_opponents;
	check_area = Quadrangle(pos,Vec(fg.field_length/2.0,-3000.),scanrange, scanrange);
	pset.keep_players_in(check_area);
	if(pset.num < 2)
	{
		return true;
	}
	
	if(hot_scoring_area.is_inside(pos))
	{
		return true;
	}
	return false;
}

Vec Tools::check_potential_pos( Vec pos, const double max_advance)
{
	FieldGeometry fg = MWM.get_field_geometry();
	WSpset pset;
	Vec endofregion;
	double check_length = 4500.;
	double endwidth = 1.66 * check_length;
	
	double startwidth = 2000.;
	
	Angle testdir[10];
	int num_dirs = 0;
	testdir[num_dirs ++] = Angle::zero;
	testdir[num_dirs ++] = Angle::eighth;
	testdir[num_dirs ++] = Angle::seven_eighth;
	testdir[num_dirs ++] = Angle::quarter;
	testdir[num_dirs ++] = Angle::three_quarters;
	
	double max_evaluation = evaluate_pos(pos); // default: evaluate current position
	Vec best_potential_pos = pos; 
	
	for(int i=0; i<num_dirs; i++)
	{
		pset = MWM.valid_opponents;
		endofregion = Vec::getVecPositionFromPolar(check_length, testdir[i]);
		endofregion += pos;
		Quadrangle check_area = Quadrangle(pos, endofregion, startwidth, endwidth);
		pset.keep_players_in(check_area);
		double can_advance = check_length;
		if( pset.num > 0 )
		{
			if(pset.num > 1 )
			{
				Object closest_op = pset.closest_player_to_point(pos);
				can_advance = pos.getDistanceTo(closest_op.absPos)*0.5;  // 0.5: potential opponent runs towards me
			}
		}
		// from here on, we know that we can advance in this direction!
		Vec potential_pos;
		can_advance = min(max_advance,can_advance);
		potential_pos= Vec::getVecPositionFromPolar(can_advance,testdir[i]);
		potential_pos += pos;
		if(potential_pos.x > fg.field_length/2.0)
			potential_pos.x = fg.field_length/2.0;
		if(potential_pos.y > fg.field_width/2.0)
			potential_pos.y = fg.field_width/2.0;
		if(potential_pos.y < -fg.field_width/2.0)
			potential_pos.y = -fg.field_width/2.0;
		double evaluation = evaluate_pos(potential_pos);
		if(evaluation >max_evaluation)
		{
			max_evaluation = evaluation;
			best_potential_pos = potential_pos;
		}
	}
	
	return best_potential_pos;
}

double Tools::evaluate_pos(const Vec query_pos)
{
	FieldGeometry fg = MWM.get_field_geometry();
	
	double evaluation;
	if(query_pos.x <6000.)
	{
		evaluation = query_pos.x;
	}
	else
	{
		evaluation = query_pos.x + fg.field_width/2.0- fabs(query_pos.y);
	}
	return evaluation;
}


double Tools::evaluate_potential_of_pos(const Vec pos)
{
	Vec query_pos;
	query_pos = check_potential_pos(pos);
	double evaluation = evaluate_pos(query_pos);
	
	return evaluation;
}

int Tools::compare_two_positions(Vec pos1, Vec pos2)
{
	if(evaluate_potential_of_pos(pos1) >= evaluate_potential_of_pos(pos2))
		return 1;
	return 2;
}

bool Tools::close2_goalline(const Vec pos)
{
	return (pos.x > 9000.0 - 2000.0);
}

int Tools::potential_to_score(Vec pos)
{
	if(pos.x > 9000.0 - 3000. && fabs(pos.y) <2000.)
		return HIGH;
	if(pos.x > 9000.0 - 4000. && fabs(pos.y) <3000.)
		return MEDIUM;
	if(pos.x > 9000.0 - 6000. && fabs(pos.y) <4000.)
		return LOW;
	return NONE;
}

double Tools::get_closest_op_dist( Vec pos)
{
    WSpset pset = MWM.valid_opponents;
    Object closest_op = pset.closest_player_to_point(pos);
    double closest_op_dist;
    if( closest_op.onGreen != false )
		closest_op_dist= pos.getDistanceTo(closest_op.absPos);
    else
		closest_op_dist= 99999.;
    return closest_op_dist;
}

bool Tools::is_pos1_better(const Vec pos1, const Vec pos2)
{
	double evaluation_delta;
	if (compare_positions(pos1, pos2, evaluation_delta) == FIRST)
		return true;
	else if (compare_positions(pos1, pos2, evaluation_delta) == SECOND)
		return false;
	else if(evaluation_delta >0)
		return true;
	else
		return false;
	return false;
}

int Tools::compare_positions(const Vec pos1, const Vec pos2, double & difference)
{
	difference = 0;
	if(potential_to_score(pos1)>potential_to_score(pos2))
		return FIRST;
	if(potential_to_score(pos1)<potential_to_score(pos2))
		return SECOND;
	
	if(close2_goalline(pos1) == true && close2_goalline(pos2) == false)
		return FIRST;
	if(close2_goalline(pos1) == false && close2_goalline(pos2) == true)
		return SECOND;
	if(close2_goalline(pos1) == true && close2_goalline(pos2) == true)
	{
		difference =  (6000. - fabs(pos1.y))  - (6000. - fabs(pos2.y));
		return EQUAL;
	}
	
	difference = evaluate_pos(pos1) - evaluate_pos(pos2);
	return EQUAL;
	
}

bool Tools::is_pos_free(const Vec & pos)
{
	WSpset pset = MWM.valid_opponents;
	pset.keep_players_in_circle( MWM.posBall, 2000.0); // consider only players in reasonable distance to the ball
	pset.keep_players_in_circle(pos, 2000.0); // consider only players in reasonable distance to the ball
	if(pset.num > 0)
		return false;
	return true;
}

double Tools::evaluate_wrt_position(const Vec & pos,const Vec & targetpos)
{
	// simple procedure to evaluate quality of position: 0 bad 1 good 2 better
	double result = 0.0;
	
	float width1 =  1.0 * 2* ((pos-targetpos).length()/2.5);
	float width2 = 4000;
	Quadrangle check_area = Quadrangle(pos, targetpos , width1, width2);
	WSpset pset = MWM.valid_opponents;
	pset.keep_players_in(check_area);
	if(pset.num == 0)
	{
		double mindist2op = min((pos-targetpos).length()/2.5, 2000.0);
		if( get_closest_op_dist(pos) > mindist2op)
		{
			return 2.0;
		}
		return 1.0;
	}
	width1 =  .7 * 2* ((pos-targetpos).length()/2.5);
	width2 = 1000.0; // at ball be  a little smaller
	check_area = Quadrangle(pos, targetpos , width1, width2);
	pset.keep_players_in(check_area);
	if(pset.num == 0)
	{
		result = 1.0 ;
	}
	else
		result = 0.0;
	return result;
}

Vec Tools::getPosAroundBallForGoal(int dir /* = 1 */, double dis /* = 1000.0 */)
{
	Time t;
	RobotLocation rloc = MWM.get_robot_location(t);
	BallLocation  bloc = MWM.get_ball_location(t);
	Vec posOppGoal(9000.0,0);
	Vec posMe     = rloc.pos;
	Angle bodyDir = rloc.heading;
	Vec posBall   = bloc.pos;
	Angle direction = ( posOppGoal - posBall ).angle(); 
	Vec posDest = posBall + dir * Vec(dis,direction.get_rad(),POLAR);
	return posDest;
}

Angle Tools::getAngleBallToGoal()
{
	Time t;
	BallLocation  bloc = MWM.get_ball_location(t);
	Vec posBall   = bloc.pos;
	Vec posOppGoal(9000.0,0);
	Angle direction = ( posOppGoal - posBall ).angle(); 
	return direction;
}

bool Tools::is_ball_kickable()
{
	if( MWM.ball2Robot.distance < 420 && fabs(MWM.ball2Robot.angle) < 20*DegreeToRad )
 		return true;
 	else
		return false;
}

bool Tools::shoot_possible()
{
	if (is_ball_kickable() && fabs( MWM.goal2Robot.angle ) < 20*DegreeToRad )
 		return true;
 	else
		return false;
}

Vec Tools::getStrategicPosition( PlayerRole pr, Vec posBall, Vec posOwnGoal )
{
	double x,y;
	LineSegment lnBG(posBall,posOwnGoal);
	Line lnball(posBall,Vec(-9000,posBall.y));
	Angle direction = ( posOwnGoal - posBall ).angle(); 
	Vec   posDest = posBall + Vec(3000.0,direction.get_rad(),POLAR);
 	Angle angBallOwnGoal = (posOwnGoal - posBall).angle();
	double distBallToOwnGoal = posBall.getDistanceTo(posOwnGoal);
	double distToOwnGoal = distBallToOwnGoal / 2.0;
	switch( pr )
	{
		case Attack1:
		case Support:
			{
				x = posBall.x - 2000.0;
				y = posDest.y - 1000.0;
				if( fabs(y) > 0.85 * 0.5 * 12000 )  
					y = sign(y) * 0.85 * 0.5 * 12000;
				if (x > posBall.x)
					x = posBall.x;
			    
				if (x < -8000.0) 
					x = -8000.0;			
			}break;
		case Defend2:
			{
				x = posBall.x - 2000.0;
				y = posDest.y + 1000.0;
				if( fabs(y) > 0.85 * 0.5 * 12000 )  
					y = sign(y) * 0.85 * 0.5 * 12000;
				if (x > posBall.x)
					x = posBall.x;
				if (x > 2000.) 
					x = 2000.0;
				else if (x < -8000.0) 
					x = -8000.0;
			}break;
		case Defend1:
		case Defend3:
			{
				x = posDest.x - 1000.0;
				y = posDest.y;
				if( fabs(y) > 0.85 * 0.5 * 12000 )  
					y = sign(y) * 0.85 * 0.5 * 12000;
				if (x > posBall.x)
					x = posBall.x;
				if (x > 0.) 
					x = 0.0;
				else if (x < -8000.0) 
					x = -8000.0;
			}break;
		default: break;
	}
	if( posBall.x < -3000.0 )
		x = -6200.0;
	
 	return Vec(x,y);
}

int Tools::sign(double d1)
{
	return (d1>0)? 1:-1;
}

Vec Tools::getStrategicPosition2(const Time& t)
{
	RefereeState  rs = MWM.get_game_state().refstate;
    PlayerRole    pr = WBOARD->getPlayerRole();
	RobotLocation rloc = MWM.get_robot_location(t);
	BallLocation  bloc = MWM.get_ball_location(t);
	Vec posBall = bloc.pos;

 	switch(rs)
	{
		case preOwnKickOff:          posBall = Vec(0.0,0.0);	break;
		case preOpponentKickOff:
		case postOpponentKickOff:	 
		case preOpponentThrowIn:
		case postOpponentThrowIn:   
		case preOpponentFreeKick:
		case postOpponentFreeKick:	 
		case preOpponentCornerKick:
		case postOpponentCornerKick: 
		case preOpponentGoalKick:
		case postOpponentGoalKick:	 
			{
			  if( posBall.x > -4000.0 ) 
 				 posBall.x -= 2000.0;	
			  else
				 posBall.x = -6200.0;
			}break;
		case preDroppedBall:         posBall.x -= 1000.0;       break; 
		default: break;
	}

    if( bloc.pos_known == 0 )
		posBall = Vec(0.0,0.0);
  
    Vec  posHome;
	bool bBehindBall;
	double dAttrX,dAttrY,dMinX,dMaxX;
	switch(pr)
	{
		case Attack2:
			{
				posHome = Vec(0,0);
				dAttrX  = 0.6;
				dAttrY  = 1.0;
				bBehindBall = true;
				if( fabs(rloc.pos.y) > 3250 )
					dMinX = -8500.0;
				else
					dMinX = -6200.0;
				dMaxX = 8300.0;
			}break;
		case Attack1:
		case Support:
			{
				posHome = Vec(-3000.,2000.);
				dAttrX  = 0.6;
				dAttrY  = 1;
				bBehindBall = true;
				if( fabs(rloc.pos.y) > 3250 )
					dMinX = -8500.0;
				else
					dMinX = -6200.0;
				dMaxX = 8300.0;
			}break;
		case Defend2:
			{
				posHome = Vec(-3000.,-2000);
				dAttrX  = 0.6;
				dAttrY  = 0.25;
				bBehindBall = true;
				if( fabs(rloc.pos.y) > 3250 )
					dMinX = -8500.0;
				else
					dMinX = -6200.0;
				dMaxX = 3000.0;
			}break;
		case Defend1:
		case Defend3:
			{
				posHome = Vec(-6000,0);
				dAttrX  = 0.6;
				dAttrY  = 0.8;
				bBehindBall = true;
				dMinX = -8300.0;
				dMaxX = -3000.0;
			}break;
 		default:break;
	}
	double x, y, dMaxYPercentage = 0.8;
	x = posHome.x + posBall.x * dAttrX;
	y = posHome.y + posBall.y * dAttrY;
	if (fabs(y) > dMaxYPercentage * 6000 )
	  y = sign(y) * dMaxYPercentage * 6000;
	if (bBehindBall == true && x > posBall.x)
 		x = posBall.x;
    if (x > dMaxX)
	   x = dMaxX;
	else if (x < dMinX)
 	   x = dMinX;

	return Vec(x,y);
        
}

Vec Tools::getStrategicPosition3(const Time& t)
{
	RefereeState  rs = MWM.get_game_state().refstate;
    PlayerRole    pr = WBOARD->getPlayerRole();
	RobotLocation rloc = MWM.get_robot_location(t);
	BallLocation  bloc = MWM.get_ball_location(t);
	Vec posBall = bloc.pos;
 	Vec posMe   = rloc.pos;
    Vec posOwnGoal;
	Vec posDest;
    bool isDeadBallOpp = false;

    if( Tools::is_lost_ball_info(t) )  
		posBall = Vec(0.0,0.0);

 	if (posBall.x < -7500.0 ) 
	   posOwnGoal = Vec(-8300.0,0.0);
	else if (posBall.x < -3000.0) 
	   posOwnGoal = Vec(-7500.0,0.0);
 	else if (posBall.x < 3000.0)  
	   posOwnGoal = Vec(-6000.0,0.0);
	else 
	   posOwnGoal = Vec(-2000.0,0.0);
    Angle angBallOwnGoal = (posOwnGoal - posBall).angle();
   
	double distBall = 2000.0;
    
	static const Vec posLeftTop(-7800,2000);
	static const Vec posRightTop(-7800,-2000);
	static Line lineFront(posLeftTop,posRightTop);
	static Line lineLeft(Vec(-9000.0,posLeftTop.y),posLeftTop);
	static Line lineRight(Vec(-9000.0 ,posRightTop.y),posRightTop);
	Line lineGoal(posBall,Vec(-9000.0,0.0));
	Vec posIntersect = intersect(lineFront,lineGoal);
	if (posIntersect.y > posLeftTop.y )
 		posIntersect = intersect(lineLeft,lineGoal);
    else if (posIntersect.y < posRightTop.y)  
		posIntersect = intersect(lineRight,lineGoal);
    
	if( posIntersect.x < -8300.0 )   
		posIntersect.x = -8300.0;

  	switch( rs )
	{
		case preOwnKickOff:          distBall = 1000.0;	break;
		case preOpponentKickOff:
		case postOpponentKickOff:	 
		case preOpponentThrowIn:
		case postOpponentThrowIn:   
		case preOpponentFreeKick:
		case postOpponentFreeKick:	 
		case preOpponentCornerKick:
		case postOpponentCornerKick: 
		case preOpponentGoalKick:
		case postOpponentGoalKick: 	
		case freePlay:          	
			{
				distBall = 2200.0;  
				isDeadBallOpp = true; 
			}break;
		case preDroppedBall:        
			{
				distBall = 1200.0;  
				isDeadBallOpp = true; 
			}break; 
		default: break;
	}
   
 	Vec    posBehindBall_front = posBall + Vec(distBall,angBallOwnGoal.get_rad(),POLAR);
 	Vec    posBehindBall_back  = posBall + Vec(2000.0+distBall,angBallOwnGoal.get_rad(),POLAR);
    Vec    posBehindBall_left  = posBehindBall_front - (posBehindBall_front-posBall).rotate_quarter().normalize() * 1000.0;
    Vec    posBehindBall_right = posBehindBall_front + (posBehindBall_front-posBall).rotate_quarter().normalize() * 1000.0;
  
	double  dMaxYPercentage = 0.8;
 	if( fabs(posBehindBall_front.y) > 3650 )
	{
		if( posBehindBall_front.x < -8500.0)   posBehindBall_front.x = -8500.0;
		if( posBehindBall_front.x > 8300.0)    posBehindBall_front.x = 8300.0;
		if( posBehindBall_left.x < -8500.0)    posBehindBall_left.x = -8500.0;
		if( posBehindBall_left.x > 6000.0) 	   posBehindBall_left.x = 6000.0;
		if( posBehindBall_right.x < -8500.0)   posBehindBall_right.x = -8500.0;
		if( posBehindBall_right.x > 4000.0)    posBehindBall_right.x = 4000.0;
		if( posBehindBall_back.x < -8300.0)    posBehindBall_back.x = -8300.0;
		if( posBehindBall_back.x > -2000.0)    posBehindBall_back.x = -2000.0;
	} 
	else
	{
		if( posBehindBall_front.x < -6000.0)   posBehindBall_front.x = -5800.0;
		if( posBehindBall_front.x > 8300.0)    posBehindBall_front.x = 8300.0;
		if( posBehindBall_left.x < -6000.0)    posBehindBall_left.x = -5800.0;
		if( posBehindBall_left.x > 6000.0) 	   posBehindBall_left.x = 6000.0;
		if( posBehindBall_right.x < -6000.0)   posBehindBall_right.x = -5800.0;
		if( posBehindBall_right.x > 4000.0)    posBehindBall_right.x = 4000.0;
		if( posBehindBall_back.x < -8300.0)    posBehindBall_back.x = -8300.0;
		if( posBehindBall_back.x > -2000.0)    posBehindBall_back.x = -2000.0;
	}
	
  	if( fabs(posBehindBall_front.y) > 5500.0)  posBehindBall_front.y *= dMaxYPercentage;
	if( fabs(posBehindBall_left.y) > 6000.0)   posBehindBall_left.y  *= dMaxYPercentage;
	if( fabs(posBehindBall_right.y) > 5000.0)  posBehindBall_right.y *= dMaxYPercentage;
	if( fabs(posBehindBall_back.y) > 5500.0)   posBehindBall_back.y  *= dMaxYPercentage;
 
    double dist_ball = posMe.getDistanceTo(posBall);
	Vec ball_left(posBall.x+500,posBall.y + distBall);
	Vec ball_right(posBall.x+500,posBall.y - distBall);
	Vec ball_dir = sign(posBall.y) > 0 ? ball_right : ball_left;
 	Angle hor_dir = (ball_dir - posBall).angle();

	switch( pr )
	{
		case Attack2:
			{
   			    posDest = posBehindBall_front;
				dist_ball = posDest.getDistanceTo(posBall);
 				if( isDeadBallOpp && dist_ball < distBall )
				{
				  if (posBall.x < -4500.0) 
				  {
					if (posDest.getDistanceTo(ball_left) < posDest.getDistanceTo(ball_right ))
                        posDest = ball_left;
 					else
						posDest = ball_right;
				  }
				  else
				  {
					  posDest.x -= 1000;
				  }
				}
 			}break;
		case Attack1:
		case Support:
			{
				posDest = posBehindBall_left;
 				dist_ball = posDest.getDistanceTo(posBall);
				ball_left = Vec(posBall.x-300,posBall.y + distBall);
				ball_right= Vec(posBall.x-300,posBall.y - distBall);

 				if( isDeadBallOpp && dist_ball < distBall )
				{
 					if (posBall.x < -4500.0) 
					{
						if (posDest.getDistanceTo(ball_left) < posDest.getDistanceTo(ball_right ))
							posDest = ball_left;
						else
							posDest = ball_right;
					}
					else
					{
						posDest.x -= 1000;
					}
				}
				if (posDest.getDistanceTo(posBehindBall_front) < 800 ) 
				{
					posDest.y += sign(posDest.y)*800;
				}
			}break;
		case Defend2:
			{
				posDest = posBehindBall_right;
 				dist_ball = posDest.getDistanceTo(posBall);
				ball_left = Vec(posBall.x-200,posBall.y + distBall);
				ball_right= Vec(posBall.x-200,posBall.y - distBall);
				
				if( isDeadBallOpp && posBall.x < -5500.0 && dist_ball < distBall )
				{
					if (posDest.getDistanceTo(ball_left) < posDest.getDistanceTo(ball_right ))
                        posDest = ball_left;
					else
						posDest = ball_right;
				}
				if ( posDest.getDistanceTo(posBehindBall_front) < 800 ) 
				{
					posDest.y += sign(posDest.y)*800;
				}
				if (posBall.x < -4500.0 && Vec(-6200.0,0.).getDistanceTo(posBall) > 2000 )
				{
					posDest = Vec(-6000,0);
				}

			}break;
		case Defend1:
		case Defend3:
			{
			   if( posBall.x < -4500.0 )   posDest = posIntersect;
			   else	                       posDest = posBehindBall_back;
			   ball_left = Vec(posDest.x,posDest.y + distBall);
			   ball_right= Vec(posDest.x,posDest.y - distBall);
			   if( isDeadBallOpp && posBall.x < -4500.0 && dist_ball < distBall )
			   {
				   if (posDest.getDistanceTo(ball_left) < posDest.getDistanceTo(ball_right ))
					   posDest = ball_left;
				   else
					   posDest = ball_right;
			   }

			}break;
 		default:break;
	}
	Vec start_game_pos1(-6000,-3000);
	Vec start_game_pos2(-6000,0);
	Vec start_game_pos3(-6000,3000);

    if( posBall.getDistanceTo(start_game_pos1) < 500 ) 
	{
	  if (rs == preDroppedBall )
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,-1800); 	break;
			case Attack1:
			case Support: posDest = Vec(-6800,-3550);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7100,-2500);   break;
			case Defend2: posDest = Vec(-6000,0);       break;
			default:break;
        }
	  }
	  else if( rs == preOpponentFreeKick ) 
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,-800); 	break;
			case Attack1:
			case Support: posDest = Vec(-6800,-4550);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7000,-1000);   break;
			case Defend2: posDest = Vec(-6000,200);     break;
			default:break;
        }
	  }
    }
	else  if( posBall.getDistanceTo(start_game_pos3) < 500 ) 
	{
	  if (rs == preDroppedBall )
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,1800); 	break;
			case Attack1:
			case Support: posDest = Vec(-6800,3550);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7000,2500);   break;
			case Defend2: posDest = Vec(-6000,0);       break;
			default:break;
        }
	  }
	  else if( rs == preOpponentFreeKick ) 
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,800); 	break;
			case Attack1:
			case Support: posDest = Vec(-6800,4550);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7000,1000);   break;
			case Defend2: posDest = Vec(-6000,-200);     break;
			default:break;
        }
	  }
    }
	else if( posBall.getDistanceTo(start_game_pos2) < 500 ) 
	{
	  if( rs == preDroppedBall )
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,-1200); 	break;
			case Attack1:
			case Support: posDest = Vec(-6000,1200);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7200,0);   break;
			case Defend2: posDest = Vec(-6000,2200);       break;
			default:break;
        }
	  }
	  else if( rs == preOpponentFreeKick ) 
	  {
        switch(pr) 
		{
			case Attack2: posDest = Vec(-6000,-2200); 	break;
			case Attack1:
			case Support: posDest = Vec(-7000,-4550);   break;
			case Defend1: 
			case Defend3: posDest = Vec(-7000,-1000);   break;
			case Defend2: posDest = Vec(-6000,2200);     break;
			default:break;
        }
	  }
    }

  	return posDest;
        
}

bool Tools::is_lost_ball_info(const Time& t)
{
	BallLocation  bloc = MWM.get_ball_location(t);
    if( bloc.pos_known != 0 )
 		return false;
 	else
	{
		if( bloc.quality < 0.1 )
 			return true;
 		else
			return false;
	}
}

bool Tools::is_position_in_penalty_area(Vec position, const float safety_margin )
{
   XYRectangle own_pa( Vec(-9000.0,-3250.0),Vec(-6750.0,3250.0) );
   XYRectangle opp_pa( Vec(6750.0,-3250.0),Vec(9000.0,3250.0) );
   if (own_pa.is_inside(position) || opp_pa.is_inside(position))
 	   return true;
   return false;
}

bool Tools::is_position_in_goal_area(Vec position, const float safety_margin )
{
   XYRectangle own_ga( Vec(-9000.0,-1750.0),Vec(-8250.0,1750.0) );
   XYRectangle opp_ga( Vec(8250.0,-1750.0),Vec(9000.0,1750.0) );

   if (own_ga.is_inside(position) || opp_ga.is_inside(position))
 	   return true;
   else if( fabs(position.x > 9000) &&  fabs(position.y) < 2000 ) 
       return true; 
   return false;
	
}

bool Tools::in_opp_penalty_area(Vec position, const float safety_margin )
{
   XYRectangle opp_pa( Vec(6750.0,-3250.0),Vec(9000.0,3250.0) );
   if ( opp_pa.is_inside(position) )
 	   return true;
   return false;
}

bool Tools::in_opp_goal_area(Vec position, const float safety_margin )
{
   XYRectangle opp_ga( Vec(8250.0,-1750.0),Vec(9000.0,1750.0) );

   if ( opp_ga.is_inside(position) )
 	   return true;
   else if( fabs(position.x > 9000) &&  fabs(position.y) < 2000 ) 
       return true; 
   return false;
	
}

bool Tools::in_own_penalty_area(Vec position, const float safety_margin )
{
   XYRectangle own_pa( Vec(-9000.0,-3250.0),Vec(-6750.0,3250.0) );
   if ( own_pa.is_inside(position) )
 	   return true;
   return false;
}

bool Tools::in_own_goal_area(Vec position, const float safety_margin )
{
   XYRectangle own_ga( Vec(-9000.0,-1750.0),Vec(-8250.0,1750.0) );

   if ( own_ga.is_inside(position) )
 	   return true;
   else if( fabs(position.x > 9000) &&  fabs(position.y) < 2000 ) 
       return true; 
   return false;
	
}