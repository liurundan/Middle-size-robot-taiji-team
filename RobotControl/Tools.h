#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "parameters.h"
#include "WSpset.h"
#include "PlayerRole.h"

#define NONE 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3

#define EQUAL 0
#define FIRST 1
#define SECOND 2

class Tools 
{
public:
	static Angle get_angle_between_mPI_pPI(Angle angle);
	static Angle get_angle_between_null_2PI(Angle angle);
	static Angle get_abs_angle(Angle angle);
	static Angle my_angle_to(Vec target);
	static Angle my_abs_angle_to(Vec target);
	static Vec   get_Lotfuss(Vec x1, Vec x2, Vec p);
	static double get_dist2_line(Vec x1, Vec x2, Vec p);
	
	static Vec   point_on_line(Vec steigung, Vec line_point, double x);
	static Vec   intersection_point(Vec p1,Vec steigung1,Vec p2, Vec steigung2);
	static bool  intersection(const Vec& r_center, double size_x, double size_y,
		const Vec& l_start, const Vec& l_end );
	
	static bool point_in_triangle(const Vec& p, const Vec& t1, const Vec& t2, const Vec& t3 );
    static bool point_in_rectangle(const Vec & p, const Vec & r1, const Vec & r2, 
		const Vec & r3, const Vec & r4);
	
    static bool point_in_field(const Vec & p, double extra_margin= 0.0);
	static bool is_position_in_pitch(Vec position,  const float safety_margin = 500.0 );
	static bool is_position_in_penalty_area(Vec position, const float safety_margin  = 500.0  );
	static bool is_position_in_goal_area(Vec position, const float safety_margin = 500.0  );
	static bool in_opp_penalty_area(Vec position, const float safety_margin  = 500.0  );
	static bool in_opp_goal_area(Vec position, const float safety_margin = 500.0  );
	static bool in_own_penalty_area(Vec position, const float safety_margin  = 500.0  );
	static bool in_own_goal_area(Vec position, const float safety_margin = 500.0  );
	
    inline static Vec opponent_goalpos() {return Vec(18000/2.,0);}
	static double min_distance_to_border(const Vec position);
 
	static bool    is_a_scoring_position(Vec pos);

    static Vec     check_potential_pos(  Vec pos, const double max_advance= 10);
    static double  evaluate_pos(const Vec query_pos);
    static double  evaluate_potential_of_pos(const Vec pos);
	static int     compare_two_positions(Vec pos1, Vec pos2);
	static bool    close2_goalline(const Vec pos);
	static int     potential_to_score(Vec pos);
	static double  get_closest_op_dist( Vec pos);
  	static bool    is_pos1_better(const Vec pos1, const Vec pos2);
	static int     compare_positions(const Vec pos1, const Vec pos2, double & difference);
	static bool    is_pos_free( const Vec & pos);
    static double  evaluate_wrt_position(const Vec & pos,const Vec & targetpos);
	static Vec     getPosAroundBallForGoal(int dir = 1, double dis = 1000.0);
    static Angle   getAngleBallToGoal();
    static Vec     getStrategicPosition( PlayerRole pr, Vec posBall, Vec posOwnGoal );
    static Vec     getStrategicPosition2(const Time& t);
    static Vec     getStrategicPosition3(const Time& t);
	
	static int     sign(double d1);
 	static bool    is_ball_kickable();
 	static bool    shoot_possible();
	
    static bool    is_lost_ball_info(const Time& t);
	
 	static bool is_ball_kickable_next_cycle(const DriveVector* dv, Vector & mypos,Vector & myvel, double &newmyang,
		Vec & ballpos,Vec & ballvel);
 	static bool is_ballpos_safe(  const double &opbodydir,
		const Vec &ballpos, int bodydir_age);
	static bool is_ballpos_safe( const Vec &ballpos  );
	static bool is_ball_safe_and_kickable(const Vec &mypos, const Vec &oppos, const double &opbodydir,
		const Vec &ballpos, int bodydir_age);
 	
	static double get_closest_teammate_dist(const Vec pos);
	static double get_optimal_position(Vec & result, Vec * testpos, 
				    const int num_testpos );
	
	static string get_our_fastest_player2ball(Vec &intercept_pos, int & steps2go);
	static bool   is_pos_occupied_by_ballholder(const Vec &pos);
	static int    num_teammates_in_circle(const Vec pos, const double radius);
    
};
#endif