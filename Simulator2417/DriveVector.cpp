#include "StdAfx.h"
#include "DriveVector.h"

DriveVector::DriveVector () throw () 
{
  vtrans.X=0;
  vtrans.Y=0;
  vrot=0;
  mode = ROBOTVELOCITY;
  for (int i=0; i<3; i++) vaux[i]=0; 
  kick=false;
}

DriveVector::DriveVector (Vector vtrans1, double vrot1, bool kick1) throw() 
  : vtrans(vtrans1), vrot(vrot1), kick(kick1), mode(ROBOTVELOCITY) {;}

DriveVector::DriveVector (double v1, double v2, double v3, bool _kick, DriveVectorMode _mode)
{
  kick = _kick;
  mode = _mode;
  switch (mode) 
  {
    case ROBOTVELOCITY:
	{
		vtrans.X = v1;
		vtrans.Y = v2;
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
		vtrans.X = 0;
		vtrans.Y = 0;
		vrot     = 0;
		break;
  }
}

DriveVector::DriveVector (const DriveVector& src) throw () : vtrans(src.vtrans), vrot(src.vrot), kick(src.kick), mode(src.mode) {for (int i=0; i<3; i++) vaux[i]=src.vaux[i];}

const DriveVector& DriveVector::operator= (const DriveVector& src) throw () 
{
  vtrans=src.vtrans;
  vrot = src.vrot;
  kick = src.kick;
  mode = src.mode;
  for (int i=0; i<3; i++) vaux[i]=src.vaux[i];
  return (*this);
}