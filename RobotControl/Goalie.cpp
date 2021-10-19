#include "StdAfx.h"
#include "Goalie.h"
#include "WorldModel.h"
#include <cmath>

Goalie::Goalie( ConfigReader& vread ) throw() : motion( WorldModel::get_main_world_model().get_robot_properties() )
{
	goto_pos_skill.set_dynamics (0.8, 2, 2, 2);
	goalie_Taiji08 = new GoalieTaiji(vread);
}

Goalie::~Goalie() throw()
{
	delete goalie_Taiji08;
}

DriveVector Goalie::process_drive_vector(Time t) throw()
{
    DriveVector ret;
    goalie_Taiji08->getCmd(&ret,t);
	return ret; 

}
