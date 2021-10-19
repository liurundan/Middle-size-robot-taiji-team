#include "StdAfx.h"
#include "WorldModel.h"
#include "Journal.h"
#include "WorldModelFactory.h"
#include "BasicFunctions.h"
#include <cstring>
#include <cmath>

using namespace std;

WorldModel*  WorldModel::main_world_model = NULL;

WorldModel&  WorldModel::get_main_world_model () throw () { return * WorldModel::main_world_model; }
 
bool  WorldModel::is_main_world_model_available () throw()
{
  return main_world_model != NULL;
}

const char*  WorldModel::get_world_model_type () const throw () { return world_model_descriptor; }

WorldModel::~WorldModel () throw () 
{
  if( WorldModel::main_world_model == this )
     WorldModel::main_world_model = NULL;    
  delete the_world_model;
  delete [] world_model_descriptor;
}

WorldModel::WorldModel (  ConfigReader& vread) throw (RobotsException,bad_alloc) : 
the_world_model(NULL), configuration_list(vread),usb_camera_valid(true)
{
  if( WorldModel::main_world_model == NULL ) 
     WorldModel::main_world_model = this;  
  
  string confline;
  if( vread.get("world_model_type", confline) <=0 ) 
  {
    JERROR("no config line \"world_model_type\" found");
    throw  InvalidConfigurationException ("world_model_type");
  }
  really_change_world_model_type (confline.c_str(), vread);
}

bool  WorldModel::change_world_model_type (const char* wmt) throw () 
{
  return change_world_model_type (wmt, configuration_list);
}

bool  WorldModel::change_world_model_type (const char* wmt,   ConfigReader& vread) throw () 
{
  try
  {
    really_change_world_model_type (wmt, vread);
  }
  catch(bad_alloc&)
  {
    JWARNING("Change of world model type failed due to lack of memory");
    return false;
  }
  catch(RobotsException&)
  {
    JWARNING("Change of world model type failed");
    return false;
  }
  return true;
}

void  WorldModel::really_change_world_model_type (const char* wmt,   ConfigReader& reader) 
throw (RobotsException,bad_alloc) 
{
  WorldModelType* new_world_model;
  char* new_world_model_descriptor;
  try 
  {
    string wmts(wmt);

    new_world_model = WorldModelFactory::get_world_model_factory ()->get_world_model (wmts, reader);
  }
  catch (invalid_argument)
  {
    throw  InvalidConfigurationException ("world_model_type");
  }
  
  new_world_model_descriptor = new char [strlen(wmt)+1];
  strcpy (new_world_model_descriptor, wmt);

  if (the_world_model!=NULL) 
  {
    delete the_world_model;
    delete [] world_model_descriptor;
  }
  the_world_model = new_world_model;
  world_model_descriptor = new_world_model_descriptor;
}

namespace
{
	inline double distance(double x1,double x2,double y1,double y2)
	{
		return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	}
	
	inline double min3(double a, double b, double c)
	{
		if(a > b) a = b;
		if(a > c) a = c;
		return a;
	}
	
	inline double max3(double a, double b, double c)
	{
		if(a < b) a = b;
		if(a < c) a = c;
		return a;
	}
	
	inline void swap(double *a, double *b)
	{
		double temp;
		
		temp = *a;
		*a = *b;
		*b = temp;
	}
}

void WorldModel::update_data()
{
	Time t;
	obstacle.clear();
	avoidData.clear();
	int    i = 0, j = 0;
	double X = 0, Y = 0, endangle[3] = {0,0,0}, Rotation = 0;
	double BallX = 0, BallY = 0, BallZ = 0;
	
	double pos_x= 0;
	double pos_y = 0;
	double pos_angle = 0;

	double oppGoal2Robot_distance = 0;
	double oppGoal2Robot_angle    = 0;
	double oppGoal2Robot_maxAngle = 0;
	double oppGoal2Robot_minAngle = 0;
	double oppGoalEndangle[3]     = {0,0,0};

	double myGoal2Robot_distance = 0;
	double myGoal2Robot_angle    = 0;
	double myGoal2Robot_maxAngle = 0;
	double myGoal2Robot_minAngle = 0;
	double myGoalEndangle[3]     = {0,0,0};

	double fieldCenter_distance = 0;
	double fieldCenter_angle    = 0;
	double ball2robot_distance  = 0;
	double ball2robot_angle     = 0;

	double ObstacleX = 0, ObstacleY = 0;
	double obstacle_distance = 0;
	double obstacle_angle    = 0;
	double obstacle_maxAngle = 0;
	double obstacle_minAngle = 0;
    
	RobotLocation    rloc = get_robot_location(t);
	BallLocation	 bloc = get_ball_location(t);
	ObstacleLocation oloc = get_obstacle_location(t);

	pos_x = rloc.pos.x;
	pos_y = rloc.pos.y;
	pos_angle = rloc.heading.get_rad();
	heading_me = rloc.heading;
	posBall    = bloc.pos;
	
	oppGoal2Robot_distance = distance( pos_x, 9000.0, pos_y, 0.0 );
	myGoal2Robot_distance  = distance( pos_x, -9000.0, pos_y, 0.0 );
	fieldCenter_distance   = distance( pos_x, 0.0, pos_y, 0.0);
    
	X = pos_x - 9000.0;
	Y = pos_y;
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else 	Rotation = atan(-Y/-X);
	oppGoal2Robot_angle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
    
	X = pos_x + 9000.0;
	Y = pos_y;
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else Rotation = atan(-Y/-X);
	myGoal2Robot_angle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();

	X = pos_x;
	Y = pos_y;
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else Rotation = atan(-Y/-X);
	fieldCenter_angle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
    
	Vec goal_parts[3] = { Vec(-9317,1080), Vec(-9317,-1080),Vec(-9590,0.0) };
	Vec goal_parts2[3] = { Vec(9317,1080), Vec(9317,-1080),Vec(9590,0.0) };
    
	for( j = 0; j < 3; j++ )
	{
		double myGendX = goal_parts[j].x;
		double myGendY = goal_parts[j].y;
		X = pos_x - myGendX;
		Y = pos_y - myGendY;
		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		myGoalEndangle[j] = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
	}
	myGoal2Robot_maxAngle = max3( myGoalEndangle[0], myGoalEndangle[1], myGoalEndangle[2] );
	myGoal2Robot_minAngle = min3( myGoalEndangle[0], myGoalEndangle[1], myGoalEndangle[2] );

	for( j = 0; j < 3; j++ )
	{
		double oppGendX = goal_parts2[j].x;
		double oppGendY = goal_parts2[j].y;
		X = pos_x - oppGendX;
		Y = pos_y - oppGendY;
		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		oppGoalEndangle[j] = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
	}
	oppGoal2Robot_maxAngle = max3( oppGoalEndangle[0], oppGoalEndangle[1], oppGoalEndangle[2] );
	oppGoal2Robot_minAngle = min3( oppGoalEndangle[0], oppGoalEndangle[1], oppGoalEndangle[2] );
	
	BallX = bloc.pos.x;
	BallY = bloc.pos.y;
	
	ball2robot_distance = distance( pos_x, BallX, pos_y, BallY );// - 110;
	X = pos_x - BallX;
	Y = pos_y - BallY;
	if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
	else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
	else Rotation = atan(-Y/-X);
	ball2robot_angle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
    
	vector<Vec> opos1 (oloc.pos.size());  
	vector<Vec> opos2 (oloc.pos.size());
	
	vector<Vec>::const_iterator itp = oloc.pos.begin();
	vector<Vec>::const_iterator itpend = oloc.pos.end();
	vector<double>::const_iterator itw = oloc.width.begin();
	vector<Vec>::iterator ito1 = opos1.begin();
	vector<Vec>::iterator ito2 = opos2.begin();
	vector<Vec>::iterator ito1end = opos1.end();
    
	//valid_opponents.num = oloc.pos.size() > 16 ? 16 : oloc.pos.size();
	valid_opponents.remove_all();
	unsigned int in = 0;
	while( itp < itpend ) 
	{
		Vec cc = (*itp)-rloc.pos;
		Vec ccnn = cc.rotate_quarter().normalize();
		(*ito1) = (*itp)+0.5*(*itw)*ccnn;
		(*ito2) = (*itp)-0.5*(*itw)*ccnn;

        ObstacleX = (*itp).x;
        ObstacleY = (*itp).y;
		
		Object obstacle_tmp;
		obstacle_tmp.distance = distance( pos_x, ObstacleX, pos_y, ObstacleY ) - 450; //450
		double t = obstacle_tmp.distance;
		X = pos_x - ObstacleX;
		Y = pos_y - ObstacleY;

		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		obstacle_tmp.angle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
        obstacle_tmp.gravityAngle = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();
		
		double endX = (*itp).x;
		double endY = (*itp).y;
		X = pos_x - endX;
		Y = pos_y - endY;
		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		endangle[0] = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();

		endX = (*ito1).x;
		endY = (*ito1).y;
		X = pos_x - endX;
		Y = pos_y - endY;
		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		endangle[1] = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();

		endX = (*ito2).x;
		endY = (*ito2).y;
		X = pos_x - endX;
		Y = pos_y - endY;
		if(X >= 0 && Y >= 0) Rotation = -M_PI+atan(Y/X);
		else if(X >= 0 && Y < 0) Rotation = M_PI+atan(Y/X);
		else Rotation = atan(-Y/-X);
		endangle[2] = Angle::rad_angle(Rotation - pos_angle).get_rad_pi();

		obstacle_tmp.maxAngle = max3(*endangle, *(endangle+1), *(endangle+2));
		obstacle_tmp.minAngle = min3(*endangle, *(endangle+1), *(endangle+2));
		
		obstacle_tmp.absPos = (*itp); 
		obstacle_tmp.onGreen = true;
		
		if (in < valid_opponents.num)
		{
			valid_opponents.p[in] = obstacle_tmp;
			in++;
		}
		valid_opponents.p_obstacle.push_back(obstacle_tmp);
		obstacle.push_back(obstacle_tmp);
		itp++;
		itw++;
		ito1++;
		ito2++;
	}
 
	myPosition.x = rloc.pos.x;
	myPosition.y = rloc.pos.y;
	posVec_me    = rloc.pos;
	goal2Robot.distance   = oppGoal2Robot_distance;
	myGoal2Robot.distance = myGoal2Robot_distance;
	myGoalObject.distance = myGoal2Robot_distance;

	fieldCenterDistance = fieldCenter_distance;
	myPosition.angle    = pos_angle;

	goal2Robot.angle = oppGoal2Robot_angle;
	shootAngle       = oppGoal2Robot_angle;
	myGoal2Robot.angle = myGoal2Robot_angle;
	myGoalObject.angle = myGoal2Robot_angle;

	fieldCenterAngle = fieldCenter_angle;

	ballPosition.x = bloc.pos.x;
	ballPosition.y = bloc.pos.y;
	ballPosition.height    = BallZ;
	ballPosition.available = TRUE;

	ball2Robot.distance = ball2robot_distance;
	ball2Robot.angle    = ball2robot_angle;

	double a,b,c,A;
	b = ball2robot_distance;
	c = myGoal2Robot.distance;
	A = TwoAngleDifference(ball2robot_angle,myGoal2Robot_angle);
	a = sqrt(b*b+c*c-2*b*c*cos(A));
	ball2Robot.sub_angle    = ( (1000.0) / (a) ) * A;
	
	obstacles_num = oloc.pos.size();

	oppGoalObject.maxAngle = oppGoal2Robot_maxAngle;
	oppGoalObject.minAngle = oppGoal2Robot_minAngle;

	myGoalObject.maxAngle = myGoal2Robot_maxAngle;
	myGoalObject.minAngle = myGoal2Robot_minAngle;
}