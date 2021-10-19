#ifndef _scanlines_h_
#define _scanlines_h_

#include <vector>

#include "Vec.h"
#include "geometry.h"

class ScanLines
{
public:
	ScanLines(const Vec& middle, int innerRadius, int outerRadius, 
	      int width, int height, int n = 12, bool bUsbSacnLine = false);
 
    ScanLines(const Vec& start, const Vec& end, int width=0, int height=0, bool bUsbSacnLine = false);
    
    std::vector<LineSegment> scanLines;  
protected:
    void insertScanLine(const LineSegment& line, int width, int height, bool bUsbSacnLine = false );
};

#endif