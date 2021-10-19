#ifndef _scanlines_h_
#define _scanlines_h_

#include "Geometry.h"

class ScanLines 
{
  public:
     
	ScanLines( int innerRadius, int outerRadius, int n = 60 );//const Vector& middle, int innerRadius, int outerRadius, int n = 60
 
    ScanLines(const Vector& start, const Vector& end, int width=0, int height=0);
    
    vector<LineSegment> scanLines;  
	void init( int innerRadius, int outerRadius, int n = 60 );

  protected:
    void insertScanLine(  LineSegment& line, int width, int height);
};

 
#endif

