#ifndef _fieldmapper_h_
#define _fieldmapper_h_

#include "RobotsException.h"
#include "Time.h"
#include "Frame2D.h"

#include <vector>

struct ScanResult;
struct Transition;
class Image;

class FieldMapper
{
public:
	FieldMapper(int x1=0,int x2=640,int y1=0,int y2=480,int w=64,int h=48);
    ~FieldMapper() {;};
 
    bool insideField(int x, int y) const;
    void buildFieldMap(const Image& image);
  void growRegions();
protected: 
	int colClass;
    int x1,x2;
    int y1,y2;
    int w,h;
    bool imagemap[64][48];
    int xstep;
    int ystep;
      
};

 
#endif