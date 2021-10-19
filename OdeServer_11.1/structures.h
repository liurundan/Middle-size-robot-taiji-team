 
#ifndef _structuresH_
#define _structuresH_

#include "v2d.h"
#include "iostream"


struct RobotTri
{
	double steer[3];
	double odo[3];
	bool   kick;
	
	RobotTri()
	{
		kick=false;
		for(int i=0;i<3;i++)
		{
			steer[i]=0;
			odo[i]=0;
		}
	}
	
};


#endif
