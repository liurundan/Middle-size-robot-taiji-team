#include "StdAfx.h"
#include "CompassContainer.h"
#include "math.h"

CompassContainer::CompassContainer( unsigned int s/* =20  */) : com(s),n(s)
{
	for (unsigned int i=0; i<n; i++)
	{
		com[i].compassdir = 0;
		com[i].timestamp.set_msec(0);
	}
}

CompassContainer::~CompassContainer() throw() {;}

void CompassContainer::add_compass( Angle h, Time t) throw()
{
	com.get().timestamp  = t;
	com.get().compassdir = h;
	com.step();
}

Angle CompassContainer::get_compass( Time t) const throw()
{
	Angle dest;
	dest = com[0].compassdir;
	unsigned int i = 1;
	while (i<n && com[i].timestamp < t)
	{
		dest = com[i].compassdir;
		i++;
	}
	return dest;
}
