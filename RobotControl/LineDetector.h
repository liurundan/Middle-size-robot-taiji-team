#ifndef _linedetector_h_
#define _linedetector_h_

#include "RobotsException.h"
#include "Image2WorldMapping.h"
#include "Time.h"
#include "VisibleObject.h"

struct ScanResult;
struct Transition;
class FieldMapper;
class Image;

class LineDetector
{
public:
	 LineDetector(const CoordinateMapping* mapping, 
    		 FieldMapper * fieldmapper,
		 double minLineWidth,
		 double maxLineWidth,
		 bool use2Steps = false,
		 bool checkForGreen = false,
		 bool cutOutsideField = false,
		 bool useMaximalDistance = false,
		 double maximalDistance = 3000.,
		 bool useHalfField = false,
		 bool useFieldMapper =false);
 
    void searchLines(const ScanResult* scanResult, Time time = Time(),
		     VisibleObjectList* vol=0, bool writeWM = true)
      throw (RobotsException);

  
    bool checkLinePlausibility(const Transition& transStart,
			       const Transition& transEnd,
			       const Time& time) const;

    void setVisualize(Image* vis) { this->vis=vis; }
protected:
	const CoordinateMapping* mapping;  // cordinate transformation
    FieldMapper * fieldmapper;
    double minLineWidth;       
    double maxLineWidth;      
    bool use2Steps;
    bool checkForGreen;
    bool cutOutsideField;
    bool useHalfField;
    bool useMaximalDistance;
    double maximalDistance;
    bool useFieldMapper;
    Image* vis;

    void visualize(const Vec& line) const;
 
};

#endif