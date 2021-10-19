#include "StdAfx.h"
#include <cmath>
#include "parameters.h"
#include "basicfunctions.h"
#include "Angle.h"

hyper myGetTickCount(void)
{
	static BOOL firstTime=TRUE;
	static double count_to_msec;			 
	LARGE_INTEGER Counter;			 
	
	if(firstTime) 
	{
		LARGE_INTEGER Freq;				 
		if(!QueryPerformanceFrequency(&Freq)) 
		{
			printf("\nNo high frequency timer awailable! Abort.\n");
			exit(0);
		}
		count_to_msec = ( 1000.0 )/Freq.u.LowPart;  
		firstTime = FALSE;
		
	}
	
	QueryPerformanceCounter(&Counter);
	return (hyper)(Counter.QuadPart * count_to_msec);
}

int KitchenTimer(double time)
{
	static double startCount = 0.0;
	double	nowCount;
	static int	flag=0;

	if(flag == 0)
	{
		startCount = (double)(myGetTickCount());
		flag = 1;
	}
	else
	{
		nowCount = myGetTickCount() - startCount;
		if(nowCount > time * 1000.0 )
		{
			flag = 0;
			return 1;
		}
	}
	return 0;
}

double Limit2PI(double ang)
{
  if(ang > M_PI) ang = ang -2.0*M_PI;
  else if(ang < -M_PI) ang = ang + 2.0*M_PI;
  return ang;
}

double Angle2Positive(double ang)
{
	if(ang < 0.0)
		ang += 2.0*M_PI;
	return ang;
}

double TwoAngleDifference(double ang1 , double ang2)
{
	double angle;
	if(ang1 > 0.0)
	{
		if(ang2 > ang1 - M_PI)
			angle = fabs(ang1-ang2);
		else
			angle = fabs(ang2 + 2.0*M_PI - ang1);
	}
	else
	{
		if(ang2 > ang1 + M_PI)
			angle = fabs(ang2 - 2.0*M_PI-ang1);
		else
			angle = fabs(ang2-ang1);
	}
	return angle;
}

double TwoAngleBetweenDirection(double ang1 , double ang2)
{
	double angle;
	if(ang1 > 0.0)
	{
		if(ang2 > ang1 - M_PI)
			angle = ang1+ang2;
		else
			angle = ang1 + ang2 + 2.0*M_PI ;
	}
	else
	{
		if(ang2 > ang1 + M_PI)
			angle = ang1+ang2 - 2.0*M_PI;
		else
			angle = ang1+ang2;
	}
	return Limit2PI(angle / 2.0);
}

