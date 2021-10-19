#ifndef _CompassContainer_
#define _CompassContainer_
#include "RingBuffer.h"
#include "Time.h"
#include "Angle.h"

class CompassContainer
{
public:
	CompassContainer( unsigned int =20 );
	~CompassContainer() throw();
	void    add_compass( Angle, Time) throw();
	Angle get_compass( Time ) const throw();
protected:
private:
	struct TCM 
	{
		Time  timestamp;
		Angle compassdir;
	};
	RingBuffer<TCM> com;
	unsigned int n;
};
 
#endif

