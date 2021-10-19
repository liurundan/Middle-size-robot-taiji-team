#include "StdAfx.h"
#include "LineScanning.h"
#include "Painter.h"
#include <cmath>

using namespace std;

LineScanner::LineScanner(ScanLines* lines, 
		   const ColorClassInfoList* colClasses,bool bScanUsb )
 : lines(lines), colClasses(colClasses), vis(0),bScanUsbimage(bScanUsb)
{}

LineScanner::~LineScanner()
{
	delete lines;
}

void LineScanner::scan(const Image& image, ScanResultList* results ) 
  throw (RobotsException)
{
	if (results->results.size() != colClasses->classList.size()) 
	{
	  throw RobotsException(__FILE__ 
				 ": Size of given ScanResultList does not match "
				 "the number of classes defined in the "
				 "ColorClassInfoList.");
	}

	for (unsigned int s=0; s < lines->scanLines.size();  s++) 
	{
  
	  const LineSegment& line = lines->scanLines[s];   
	  Vec step   = (line.getEnd() - line.getStart());  
	  double totalLength = step.length();
	  step = step.normalize();        
	  if (fabs(step.x) > fabs(step.y))
	  {
		step = step / fabs(step.x);
	  }
	  else
	  {
		step = step / fabs(step.y);           
	  }
	  int numSteps= static_cast<int>(totalLength / step.length());   
	  std::vector<bool> seenFirstTrans(colClasses->classList.size());
  
	  for (unsigned int i=0; i<colClasses->classList.size(); i++) 
		seenFirstTrans[i] = false;

	  Vec currPos = line.getStart();
	  int currClass = COLOR_IGNORE;       

	  Vec oldPos   = currPos - step;   
	  int oldClass = COLOR_IGNORE;    

	  bool first   = true;
 
	  for ( i=0; i <= numSteps; i++) 
	  {
		currClass = image.getPixelClass(static_cast<int>(currPos.x),
						static_cast<int>(currPos.y));	
  
		if( colClasses->classList[currClass]->findPoints ) 
		{
		  results->results[currClass]->points.push_back(currPos);
		}
  
		if( currClass != oldClass ) 
		{     

		  if (colClasses->classList[oldClass]->findTransitions &&
			  !(seenFirstTrans[oldClass] && 
			colClasses->classList[oldClass]->findOnlyFirst)
			  ) 
		  {  
			Vec twoPos = currPos + step;
			twoPos.x = min(max(twoPos.x, 0.), image.getWidth()-1.);  // clip at boundaries
			twoPos.y = min(max(twoPos.y, 0.), image.getHeight()-1.); // clip at boundaries

			results->results[oldClass]->transitions.push_back(
			  Transition(Transition::END,
				 oldClass, currClass, 
				 image.getPixelClass(static_cast<int>(twoPos.x),
							 static_cast<int>(twoPos.y)),
				 oldPos, currPos, first));  
			seenFirstTrans[oldClass]=true;
			
		  }
		  if (colClasses->classList[currClass]->findTransitions &&
			  !(seenFirstTrans[currClass] && 
			colClasses->classList[currClass]->findOnlyFirst)) 
		  { 
			Vec twoPos = oldPos - step;
			twoPos.x = min(max(twoPos.x, 0.), image.getWidth()-1.);  // clip at boundaries
			twoPos.y = min(max(twoPos.y, 0.), image.getHeight()-1.); // clip at boundaries
			
			results->results[currClass]->transitions.push_back(
			  Transition(Transition::START,
				 oldClass, currClass,
				 image.getPixelClass(static_cast<int>(twoPos.x),
							 static_cast<int>(twoPos.y)),
				 oldPos, currPos, first));  
		  }
		  first = false;
		}	      
		oldClass = currClass;           
		oldPos   = currPos;             
		currPos+=step;                  
	  }
	   
	  if (colClasses->classList[currClass]->findTransitions &&
		  !(seenFirstTrans[currClass] && 
		   colClasses->classList[currClass]->findOnlyFirst)) 
	  { 
		 results->results[currClass]->transitions.push_back(Transition(Transition::END,
			 oldClass, COLOR_IGNORE, COLOR_IGNORE,
			 oldPos, currPos, true));   
	  }      
	}  
	//
	if (vis)
	{
	  visualize(results);
	  vis = 0;
	}
}

void LineScanner::visualize(const ScanResultList* results)
{
	Painter p(*vis);
	for (unsigned col=0; col < colClasses->classList.size(); col++) 
	{
	  p.setColor(colClasses->classList[col]->color);
	  ScanResult* result = results->results[col];
	  for (unsigned t=0; t < result->transitions.size(); t+=2)
	  {
		Vec start = result->transitions[t].toPos;
		Vec end = result->transitions[t+1].fromPos;
		p.drawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y);
	  }
	  for (unsigned i=0; i < result->points.size(); i++)
	  {
		Vec point = result->points[i];
		p.drawPoint((int)point.x, (int)point.y);
	  }
	}
}

void ScanResult::clear() 
{
	transitions.clear();
	points.clear();
}

ScanResultList::ScanResultList(int n)
{
	for (int i=0; i < n; i++) 
	{
	  results.push_back(new ScanResult(i));
	}
}

ScanResultList::~ScanResultList()
{
	for (unsigned int i=0; i < results.size(); i++) 
	{
	  delete results[i];
	}
	results.clear();
}

void ScanResultList::clear()
{
	for (unsigned int i=0; i < results.size(); i++) 
	{
	  results[i]->clear();
	}
}
