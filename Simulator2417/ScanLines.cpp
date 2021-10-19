#include "stdafx.h"
#include "ScanLines.h"

void ScanLines::insertScanLine( LineSegment& line, int width, int height)
{
    Vector direction = line.p2 - line.p1;
    int    maxSteps  = (int) direction.getMagnitude();
    int    steps     = 0;

    direction = direction.normalize();
 
    Vector pos=line.p1;          
    for(; steps <= maxSteps; steps++ ) 
	{ 
      if( fabs(pos.X) < width && fabs(pos.Y) < height  ) 
	  {
	     break;                 
      }
      pos = pos + direction;
    }

    if(steps >= maxSteps) 
	{    
      return;
    }

    Vector startPos = pos;        
    for (; steps <= maxSteps; steps++) 
	{ 
      if( fabs(pos.X) < width && fabs(pos.Y) < height  )
	  {
        scanLines.push_back( LineSegment(startPos, pos + direction * -1.  ) );
 	    return ;                
      }
      pos = pos + direction;
    }
    scanLines.push_back(LineSegment(startPos, line.p2));  
}

ScanLines::ScanLines( int innerRadius, int outerRadius, int n /* = 60  */ )
{   
  	MyAngle stepAngle     = MyAngle::deg_angle( 360.0/n ) ;  
    MyAngle halfStepAngle = stepAngle;             
    halfStepAngle*=0.5; 
	
 	LineSegment baseLine1( Vector(innerRadius,0), Vector(outerRadius,0 ) );
 
    int num_lines_set=0;
	LineSegment baseLine2 = baseLine1;
 	for(int k = 0; k < n; k++ )
	{
	   LineSegment ls = baseLine2.rotate (k*stepAngle);
	   
       num_lines_set++;
	   scanLines.push_back(ls);
       baseLine2 = baseLine1;
	}
}

void ScanLines::init(int innerRadius, int outerRadius, int n /* = 60  */)
{
 	MyAngle stepAngle     = MyAngle::deg_angle( 360.0/n ) ;  
    MyAngle halfStepAngle = stepAngle;             
    halfStepAngle*=0.5; 
	
 	LineSegment baseLine1( Vector(innerRadius,0), Vector(outerRadius,0 ) );
 
    int num_lines_set=0;
	LineSegment baseLine2 = baseLine1;
 	for(int k = 0; k < n; k++ )
	{
	   LineSegment ls = baseLine2.rotate (k*stepAngle);
	   
       num_lines_set++;
	   scanLines.push_back(ls);
       baseLine2 = baseLine1;
	}
}
/*
ScanLines::ScanLines( const Vector& middle, int innerRadius, int outerRadius, 
		              int width, int height, int n) 
{    
 	int distance = (int)(innerRadius * 1.4);
	
	for (int i=0; i < 5; i++) 
	{
		LineSegment horizontal(Vector(-distance,0), Vector(distance, 0));
		LineSegment vertical(Vector(0,-distance), Vector(0, distance));
		insertScanLine(horizontal.translate(Vector(0,-distance)).translate(middle), 
			 width, height);
		insertScanLine(horizontal.translate(Vector(0, distance)).translate(middle), 
			 width, height);
		insertScanLine(vertical.translate(Vector(-distance, 0)).translate(middle), 
			 width, height);
		insertScanLine(vertical.translate(Vector( distance, 0)).translate(middle), 
			 width, height);
		distance+= 15;
	}
 
	Angle stepAngle = Angle::deg_angle(360. / n);  
	Angle halfStepAngle = stepAngle;              
	halfStepAngle*=0.5;                          

	LineSegment baseLine1(Vector(0, innerRadius), Vector(0, outerRadius));
	LineSegment baseLine2(Vector(0, (2*innerRadius+outerRadius)/3),
		  Vector(0, outerRadius));
	baseLine2.s_rotate(halfStepAngle);


	vector<bool> line_set (n);
	for (int i=0; i<n; i++)
	line_set[i]=false;
	int num_lines_set=0;

	insertScanLine (baseLine1.translate (middle),width,height);
	line_set[0]=true;
	num_lines_set++;

	int base=2;
	while( num_lines_set<n ) 
	{
		for (int i=1; i<base; i+=2) 
		{
			int j = (n*i)/base;
			if (!line_set[j]) 
			{
				insertScanLine (baseLine1.rotate (j*stepAngle).translate (middle),width,height);
				line_set[j]=true;
				num_lines_set++;
			}
		}
		base*=2;
	}

	insertScanLine (baseLine2.translate (middle),width,height);
	num_lines_set--;
	line_set[0]=false;

	base=2;
	while( num_lines_set  >0 ) 
	{
		for (int i=1; i<base; i+=2) 
		{
			int j = (n*i)/base;
			if (line_set[j]) 
			{
				insertScanLine (baseLine2.rotate (j*stepAngle).translate (middle),width,height);
				line_set[j]=false;
				num_lines_set--;
			}
		}
		base*=2;
	}

}*/
  
ScanLines::ScanLines(const Vector& start, const Vector& end, int width, int height)
{
	insertScanLine(LineSegment(start, end), 
	width <= 0 ? 999999 : width, 
	height <= 0 ? 999999 : height);
}
 
