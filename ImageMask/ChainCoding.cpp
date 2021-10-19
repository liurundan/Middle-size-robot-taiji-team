#include "StdAfx.h"
#include "ChainCoding.h"
#include "Image.h"
#include <stdlib.h>
#include <iostream>

 
using namespace std;

int ChainCoder::xDir[4] = {  1,  0, -1,  0 };
int ChainCoder::yDir[4] = {  0,  1,  0, -1 };  

int ChainCoder::traceBorder(const Image& image, int x, int y, Region* region,
		  char* borderMap)
{
	int dir;                       
	int colClass = image.getPixelClass(x,y);    

	int width  = image.getWidth();

	*region = Region(colClass, x, y);

	for (dir=0; dir < 4; dir++) 
	{  
	  if (image.getPixelClass(x+xDir[dir], y+yDir[dir]) == colClass)
	  {
		break;
	  }
	}
	if (dir == 4) 
	{                
	  if (vis) vis = 0;
		  return 1;
	}

	do 
	{
	  for (dir=0; dir < 4; dir++) 
	  { 
		if (image.getPixelClass(x+xDir[dir], y+yDir[dir]) != colClass) 
		{
		   break;
		}
	  }

	  if (dir == 4) 
	  {             
		while (image.getPixelClass(x-1, y) == colClass) 
		{
		  x--;
		} 
	  }
	} 
	while (dir == 4);

	region->x = x;

	if (borderMap && borderMap[x+y*width] > 0) 
	{
	  if (vis) vis = 0;
	  return 0;                   
	}

	dir++;                       

	do 
	{
	  int sDir;
	  for (sDir = (dir+3) % 4; ; sDir = (++sDir) % 4)
	  {
		if (image.getPixelClass(x+xDir[sDir], y+yDir[sDir]) == colClass)
		{
		  break;                   
		}
	  }
	  x+=xDir[sDir];
	  y+=yDir[sDir];
	  dir = sDir;

	  if (borderMap) 
	  {
		borderMap[x+y*width] = 1; 
	  }

	  region->chainCode.push_back(dir);

	  region->minX = min(region->minX, x);
	  region->maxX = max(region->maxX, x);
	  region->minY = min(region->minY, y);
	  region->maxY = max(region->maxY, y);
	} 
	while (region->chainCode.size() < 2 ||  
		 x != region->x+xDir[static_cast<int>(region->chainCode[0])] ||
		 y != region->y+yDir[static_cast<int>(region->chainCode[0])] ||
		 x-xDir[dir] != region->x ||
		 y-yDir[dir] != region->y);

	region->chainCode.pop_back();  

	if (vis) 
	{
	  visualizeRegion(region);
	  vis = 0;
	}

	return 1;
}

void ChainCoder::visualizeRegion(const Region* region)
{}

int Region::getArea() const
{
	if (area < 0) 
	{               // have to calculate it first
	  int a = 0;

	  int vertPos=0;
	  int horiPos=0;

	  int cX = 0;
	  int cY = 0;      

	  // walk through the chaincode and integrate (see Horn for a desc.)
	  for (unsigned int i=0; i < chainCode.size(); i++) 
	  {
		switch(chainCode[i]) 
		{
			case 0: a-=vertPos; cX-=vertPos*horiPos; break;
			case 1: cY+=vertPos*horiPos; break;
			case 2: a+=vertPos; cX+=vertPos*horiPos; break;
			case 3: cY-=horiPos*vertPos; break;
		}
		vertPos += ChainCoder::yDir[(int)chainCode[i]];
		horiPos += ChainCoder::xDir[(int)chainCode[i]];
	  }      
	  const_cast<Region*>(this)->area=a;
	  const_cast<Region*>(this)->centerOfGravity =
			Vec(cX/(double)a, cY/(double)a) + Vec(x,y);
	}
	return area;   
}

double  Region::getCompactness() const
{
	return ((double)chainCode.size() * chainCode.size()) / getArea();
}

Vec Region::getCenterOfGravity() const
{
	if (area < 0) 
	{
	  getArea();               
	}
	return centerOfGravity;
}


RegionDetector::RegionDetector()
: buf(0), size(0)
{ 
  cc = new ChainCoder();
}

RegionDetector::~RegionDetector()
{
	if (buf) delete [] buf;
	if (cc) delete cc;
}

void  RegionDetector::findRegions (const Image& image, int colClass,
			   vector<Region>* regions) const
{
	int w = image.getWidth();
	int h = image.getHeight();

	if (size != w*h) 
	{         
	  if (buf) 
	  {
		delete [] const_cast<RegionDetector*>(this)->buf;
	  }
	  const_cast<RegionDetector*>(this)->buf = new char[w*h];
	  memset(buf, 0, sizeof(char)*w*h);
	}
	else 
	{
	  memset(buf, 0, sizeof(char)*size);  
	}

	int actClass = 0;          
	Region region;

	for (int x=1; x < w-1; x++) 
	{
	  for (int y=1; y < h-1; y++) 
	  {
		int newClass = image.getPixelClass(x,y);
		if (actClass != colClass &&       
			newClass == colClass &&     
			buf[x+y*w] == 0) 
		{            
		  if (cc->traceBorder(image, x,y, &region, buf)) 
		  {  
			if (region.getArea() >= 0) 
			{   
			  regions->push_back(region);  
			}                             
		  }
		}
		actClass = newClass;
	  }
	}    
}

 