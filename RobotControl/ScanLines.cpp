#include "StdAfx.h"
#include "ScanLines.h"
#include "RobotMask.h"
#include <vector>

using namespace std;

void ScanLines::insertScanLine(const LineSegment& line, 
				 int width, int height, bool bUsbSacnLine )
{
    Vec direction = line.p2 - line.p1;
    int maxSteps = (int) direction.length();
    int steps = 0;

    direction = direction.normalize();
	const RobotMask* robotMask = NULL;
	if(!bUsbSacnLine) 
      const RobotMask* robotMask = RobotMask::getMask();  

    Vec pos=line.p1;  // 起点         
    for (; steps <= maxSteps; steps++) 
	{ // first valid point look for 第一个有效点搜索
      if ((robotMask != 0 && robotMask->isValid((int)pos.x, (int)pos.y)) ||
	  (robotMask == 0 && pos.x >= 0 && pos.y >= 0 && 
	   pos.x < width && pos.y < height)) 
	  {
		break;                // point is valid    点是有效的
      }
      pos = pos + direction;  // find valid start point in same direction
    }
    if (steps >= maxSteps)    // if the line go to end, that is exceed image size
	{   
      return;   // 扫描线已经到最大搜索范围，无趣
    }
    Vec startPos = pos;  // 顶端注意       
    for (; steps <= maxSteps; steps++) 
	{  // if the exceed image size, push the line into 先行点无效
      if( ( robotMask != 0 && ! robotMask->isValid((int)pos.x, (int)pos.y)) ||
	      ( robotMask == 0 && 
	      ( pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height ) ) )
	  {
		scanLines.push_back(LineSegment(startPos, pos + -1. * direction));
		if (robotMask != 0) 
		{  
							   
		  insertScanLine(LineSegment(pos, line.p2), width, height,bUsbSacnLine); 
		}
		return ;  // finish make scan lines              
      }
      pos = pos + direction;
    }
    scanLines.push_back(LineSegment(startPos, line.p2));  
}

ScanLines::ScanLines(const Vec& middle, int innerRadius, int outerRadius, 
		       int width, int height, int n, bool bUsbSacnLine ) 
{  
	Angle stepAngle;
	LineSegment baseLine1;
    LineSegment baseLine2;

    if( !bUsbSacnLine )
	{
		int distance = (int)(innerRadius * 1.4);
		for (int i=0; i < 5; i++) 
		{
			LineSegment horizontal(Vec(-distance,0), Vec(distance, 0));
			LineSegment vertical(Vec(0,-distance),  Vec(0, distance) );
			insertScanLine(horizontal.translate(Vec(0,-distance)).translate(middle), 
				width, height);
			insertScanLine(horizontal.translate(Vec(0, distance)).translate(middle), 
				 width, height);
			insertScanLine(vertical.translate(Vec(-distance, 0)).translate(middle), 
				width, height);
			insertScanLine(vertical.translate(Vec( distance, 0)).translate(middle), 
				width, height);
			distance+= 15;
		} 
        stepAngle = Angle::deg_angle(360. / n); 
	    baseLine1 =	LineSegment( Vec(0, innerRadius), Vec(0, outerRadius) );
	    baseLine2 =	LineSegment( Vec(0, (2*innerRadius+outerRadius)/3 ),
								Vec(0, outerRadius));
	}
	else
	{
        stepAngle = Angle::deg_angle(180. / n); 
	    baseLine1 =	LineSegment( Vec(-innerRadius,0 ), Vec(-outerRadius,0 ) );
	    baseLine2 =	LineSegment( Vec(-innerRadius,0 ), Vec(-outerRadius,0 ) );

			// LineSegment baseLine2(Vec( -(2*innerRadius+outerRadius)/3,0),
			//			  Vec(-outerRadius,0 ));
	}
 
     Angle halfStepAngle = stepAngle;          // 2.25   
    halfStepAngle*=0.5;                     // intermediate line     

	
    baseLine2.s_rotate(halfStepAngle);

    vector<bool> line_set (n);
    for ( int j=0; j<n; j++)
      line_set[j]=false;
    int num_lines_set=0;
    
    insertScanLine (baseLine1.translate (middle),width,height,bUsbSacnLine);
    line_set[0]=true;
    num_lines_set++;

    // 进一步插入分层长扫描线
    int base=2;
    while (num_lines_set<n) 
	{
      for (int i=1; i<base; i+=2) 
	  {
		int j = (n*i)/base;
		if (!line_set[j]) 
		{
		  insertScanLine (baseLine1.rotate (j*stepAngle).translate (middle),width,height,bUsbSacnLine);
		  line_set[j]=true;
		  num_lines_set++;
		}
      }
      base*=2;
    }
    
	// 分层插入短扫描线
    insertScanLine (baseLine2.translate (middle),width,height,bUsbSacnLine);
    num_lines_set--;
    line_set[0]=false;

    // 进一步插入分层短扫描线线
    base=2;
    while (num_lines_set>0) 
	{
      for (int i=1; i<base; i+=2) 
	  {
		int j = (n*i)/base;
		if (line_set[j]) 
		{
		  insertScanLine (baseLine2.rotate (j*stepAngle).translate (middle),width,height,bUsbSacnLine);
		  line_set[j]=false;
		  num_lines_set--;
		}
      }
      base*=2;
    }

}

ScanLines::ScanLines(const Vec& start, const Vec& end, int width, int height, bool bUsbSacnLine )
{
  insertScanLine( LineSegment(start, end), 
				  width <= 0 ? 999999 : width, 
				  height <= 0 ? 999999 : height, bUsbSacnLine );
}