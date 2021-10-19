#ifndef _AVOID_H_
#define _AVOID_H_

#include "dataClass.h"
#include <vector>

class Avoid
{
	double best_angle;
public:
	Avoid();

  	double	Avoid2006(double GoAngle );	 
	double	AvoidTurn(double GoAngle ); 
	double	Avoid2006Ball(double GoAngle );	 
	double	Min2(double AA,double BB);			
	double	Avoid2005(double GoAngle ); 
	void	Avoid2007(double *, double *, double * );
	void	ConnectObstacles();
	void	EditObstacles();
	void	DecideVAR(double *, double *, double * );

 
};
#endif