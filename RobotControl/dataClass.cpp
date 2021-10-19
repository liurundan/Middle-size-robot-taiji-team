#include "StdAfx.h"
#include "dataClass.h"

void MaxMinDis::clear()
{
	 MaxAngle.clear();
	 MinAngle.clear();
	 Distance.clear();
	
	 MaxAngleVec.clear();
	 MinAngleVec.clear();
	 DistanceVec.clear();
}

bool Object::operator ==(const Object o) const throw()
{
	return (absPos==o.absPos);
}

bool Object::operator !=(const Object o) const throw()
{
	return (absPos != o.absPos);
}

const Object& Object::operator =(const Object& o) const throw()
{
	double	distance = o.distance;
	double  angle    = o.angle;
	double	gravityAngle = o.gravityAngle;
	double	minAngle = o.minAngle;
	double	maxAngle = o.maxAngle;
	Vec     absPos = o.absPos;
	bool	onGreen = o.onGreen;
	return *this;
}