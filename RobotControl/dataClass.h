#ifndef __HEADER_DATACLASS
#define __HEADER_DATACLASS

#include "Vec.h"
#include <vector>
using namespace std;

class MaxMinDis 
{
public:
 	std::vector<double> MaxAngle;
	std::vector<double> MinAngle;
	std::vector<double> Distance;

	std::vector<double> MaxAngleVec;
	std::vector<double> MinAngleVec;
	std::vector<double> DistanceVec;

	int    Num;
	void   clear();
};

class Vector 
{
public:
	double x;
	double y;
	double distance;
	double angle;
	double height;
	double sub_distance;
	double sub_angle;
	BOOL   available;
};

class Ball 
{
public:
	double	x;
	double	y;
	double	distance;
	double	angle;
	double	height;
	BOOL	available;
	double	distance_speed;
	double	angle_speed;
	double	sub_speed;
	double	sub_distance;
	double	sub_angle;
	bool	onGreen;
};

class Object
{
public:
	double	distance;
	double  angle;
	double	gravityAngle;
	double	minAngle;
	double	maxAngle;
	Vec     absPos;
	bool	onGreen;
	bool    operator==(const Object) const throw();
	bool    operator!=(const Object) const throw();
	const   Object& operator=(const Object& o) const throw();
	
};

class Teammate 
{
public:
	Ball	ball2Robot;	
	int		my_role;	
	Vector	myPosition;	
	char	robot_name[3];	
	int		player_num; 
	hyper	comm_time_DA;	
	int		free_role;	
	int		state;
	int		action;
	Vector	myGoal;
	Vector	oppGoal;
	int		flag1;
	int		flag2;
	int		flag3;
};

struct ReStart
{
	int restart;
	int count ;
};


#endif