#include "StdAfx.h"
#include "DriveVector.h"

DriveVector::DriveVector () throw () 
{
  vtrans.x=0;
  vtrans.y=0;
  vrot=0;
  kickballtime=0;
  mode = ROBOTVELOCITY;
  for (int i=0; i<3; i++) vaux[i]=0; 
  kick=false;
}

DriveVector::DriveVector (Vec vtrans1, double vrot1, bool kick1, BYTE ktime) throw() 
  : vtrans(vtrans1), vrot(vrot1),kick(kick1),kickballtime(ktime),mode(ROBOTVELOCITY) {;}

DriveVector::DriveVector (double v1, double v2, double v3, bool _kick, BYTE ktime, DriveVectorMode _mode)
{
  kick = _kick;
  mode = _mode;
  kickballtime = ktime;
  switch (mode) 
  {
    case ROBOTVELOCITY:
	{
		vtrans.x = v1;
		vtrans.y = v2;
		vrot     = v3;
		for (int i=0; i<3; i++) 
			vaux[i] = 0;
		break;
	}
    case WHEELVELOCITY:
    case MOTORVOLTAGE:
		vaux[0]=v1;
		vaux[1]=v2;
		vaux[2]=v3;
		vtrans.x = 0;
		vtrans.y = 0;
		vrot     = 0;
		break;
  }
}

DriveVector::DriveVector (const DriveVector& src) throw () : 
vtrans(src.vtrans), vrot(src.vrot), kick(src.kick),kickballtime(src.kickballtime), mode(src.mode) 
{for (int i=0; i<3; i++) vaux[i]=src.vaux[i];}

const DriveVector& DriveVector::operator= (const DriveVector& src) throw () 
{
  vtrans=src.vtrans;
  vrot = src.vrot;
  kick = src.kick;
  kickballtime=src.kickballtime;
  mode = src.mode;
  for (int i=0; i<3; i++) vaux[i]=src.vaux[i];
  return (*this);
}