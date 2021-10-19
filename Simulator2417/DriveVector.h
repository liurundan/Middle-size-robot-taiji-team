#ifndef _drive_vector_
#define _drive_vector_

#include "Geometry.h"

enum DriveVectorMode { ROBOTVELOCITY, WHEELVELOCITY, MOTORVOLTAGE };
struct DriveVector 
{
    DriveVector () throw ();
  
    DriveVector (Vector, double =0, bool =false) throw();
 
    DriveVector (double, double, double, bool =false, DriveVectorMode =ROBOTVELOCITY);

    DriveVector (const DriveVector&) throw ();
    const DriveVector& operator= (const DriveVector&) throw ();
    
    Vector vtrans;    
    double vrot;    
    bool kick;     
    DriveVectorMode mode;   
    double vaux[3];  
};

#endif