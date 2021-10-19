#ifndef _drive_vector_
#define _drive_vector_

#include "Vec.h"

enum DriveVectorMode { ROBOTVELOCITY, WHEELVELOCITY, MOTORVOLTAGE };
struct DriveVector 
{
    DriveVector () throw ();
  
    DriveVector (Vec, double =0,  bool = false, BYTE =0) throw();
 
    DriveVector (double, double, double, double, bool =false, BYTE =0, DriveVectorMode =ROBOTVELOCITY );

    DriveVector (const DriveVector&) throw ();
    const DriveVector& operator= (const DriveVector&) throw ();
    
    Vec vtrans;    
    double vrot;    
    bool kick;
	BYTE kickballtime;
    DriveVectorMode mode;   
    double vaux[4];  
};

#endif