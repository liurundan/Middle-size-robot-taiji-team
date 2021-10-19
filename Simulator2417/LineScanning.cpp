#include "stdafx.h"
#include "LineScanning.h"
#include "Geometry.h"
 
LineScanner::LineScanner( ScanLines* lines,
		         const ColorClassInfoList* colClasses,
				 FieldGeometry* Myfieldlines  
				 ):lines(lines),colClasses(colClasses),fieldlines(Myfieldlines)     
{
}

LineScanner::~LineScanner()
{
  delete lines;
  delete fieldlines;
  //delete colClasses;
}
 
void LineScanner::scan( ScanResultList* results, Frame2d FR )  
{
  Vector pos;
  double coeff      = 0.02;
  double coeff_goal = 0.02;
  double sigma = 2.618;
  int Line   = COLOR_LINE;
  int Blue   = COLOR_BLUE;
  int Yellow = COLOR_YELLOW;
  Vector posAgent = FR.get_pos();
  for( unsigned int s=0; s < lines->scanLines.size(); s++ )  // do for every scan line
  {
    //if( s < 60 && s > 10 ) {continue;} // simulate an occluded view range,limit robot view range 
 
	LineSegment  line = FR*lines->scanLines[s];  // current scanline
	FR.invert(); 
	for( unsigned int i=0; i < fieldlines->lines.size(); i++ )
	{
 		LineSegment    ln1 = fieldlines->lines[i];
		vector<Vector> res = intersect( line,ln1 );
	    for(unsigned int j = 0; j < res.size();j++ )
		{
         	pos = FR * res[j];
  			sigma = pos.getMagnitude()*coeff;
			pos.X = nrandom( pos.X,sigma );
			pos.Y = nrandom( pos.Y,sigma );
 			//pos =  Vector( CalcQuanDist( pos.getMagnitude(), QUANTIZESTEP_LINE ),
	         //              Quantize( pos.getDirection(),1.0), POLAR );
 			results->results[Line]->points.push_back( pos );
			
		}
	}
	for( unsigned int k=0; k < fieldlines->arcs.size(); k++) 
	{
 		MyArc ac = fieldlines->arcs[k];
		vector<Vector> res2 = intersect( line,ac );
        for(unsigned int j1 = 0; j1 < res2.size();j1++ )
		{
         	pos = FR * res2[j1];
		    sigma = pos.getMagnitude() * coeff;
			pos.X = nrandom( pos.X,sigma );
			pos.Y = nrandom( pos.Y,sigma ); 
			//pos =  Vector( CalcQuanDist( pos.getMagnitude(), QUANTIZESTEP_LINE ),
	             //          Quantize( pos.getDirection(),1.0), POLAR );
 			results->results[Line]->points.push_back(pos);
		}
	}
 	 
	// detect goal lines
	LineSegment lnBluefrontline   = fieldlines->goallines[0];
	LineSegment lnYellowfrontline = fieldlines->goallines[1];
	vector<Vector> res3 = intersect(line,lnBluefrontline);
	vector<Vector> res4 = intersect(line,lnYellowfrontline);
    Rect r1(Vector(-0.5*fieldlines->field_length,0.5*fieldlines->goal_width),
		    Vector(-0.5*fieldlines->field_length-fieldlines->goal_length,-0.5*fieldlines->goal_width));
	Rect r2(Vector(0.5*fieldlines->field_length,0.5*fieldlines->goal_width),
		    Vector(0.5*fieldlines->field_length+fieldlines->goal_length,-0.5*fieldlines->goal_width));
	bool binsidebluegoal   = false;
    bool binsideyellowgoal = false;
	binsidebluegoal   = r1.isInside(posAgent);
    binsideyellowgoal = r2.isInside(posAgent);
    if( res3.size() > 0 || res4.size() > 0 || binsidebluegoal || binsideyellowgoal 
		|| ( fabs(posAgent.getY()) < 0.5 * fieldlines->goal_width ) )
	{
	  for( unsigned int k1=2; k1 < fieldlines->goallines.size(); k1++) 
	  {
	     if( res3.size() > 0 || binsidebluegoal || ( fabs(posAgent.getY()) < 0.5 * fieldlines->goal_width ) )
		 {
			 LineSegment bluegoalines = fieldlines->goallines[k1];
			 vector<Vector> res5  = intersect(line,bluegoalines);
             
			 for( unsigned int k2=0; k2 < res5.size(); k2++ )
			 {	 
				if( (posAgent.getX() > res5[k2].getX() ) || binsidebluegoal )
				{
                    pos = FR * res5[k2];
					sigma = pos.getMagnitude()*coeff_goal;
			        //pos.X = nrandom( pos.X,sigma );
			        //pos.Y = nrandom( pos.Y,sigma );
			        results->results[Blue]->points.push_back(pos); 
				} 
			 }	 
		 }
		 if( res4.size() > 0 || binsideyellowgoal || ( fabs(posAgent.getY()) < 0.5 * fieldlines->goal_width ) ) 
		 {
			LineSegment yellowgoalines = fieldlines->goallines[k1];
			vector<Vector> res6  = intersect(line,yellowgoalines);
             
			 for( unsigned int k3=0; k3 < res6.size(); k3++ )
			 {	 
				if( (posAgent.getX() < res6[k3].getX()) || binsideyellowgoal )
				{
                    pos = FR * res6[k3];
					sigma = pos.getMagnitude()*coeff_goal;
			        //pos.X = nrandom( pos.X,sigma );
			       // pos.Y = nrandom( pos.Y,sigma );
			        results->results[Yellow]->points.push_back(pos); 
				} 
			 }	  
		 }
	  }
	}
	 
	FR.invert();
  }
  
} 

void ScanResult::clear() 
{
     points.clear();
}

ScanResultList::ScanResultList( int n )
{
    for (int i=0; i < n; i++) 
	{
 	  results.push_back(new ScanResult(i)); 
    } 
}

ScanResultList::~ScanResultList()
{
    for(unsigned int i=0; i < results.size(); i++) 
	{
 	  delete results[i];
    }
    results.clear();
}
  
void  ScanResultList::clear()
{
    for (unsigned int i=0; i < results.size(); i++) 
	{
      results[i]->clear();
    }
}