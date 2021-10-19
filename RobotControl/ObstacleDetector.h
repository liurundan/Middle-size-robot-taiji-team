#ifndef _obstacledetector_h_
#define _obstacledetector_h_

#include "RobotsException.h"
#include "Time.h"

struct VisibleObjectList;
struct ScanResult;
struct Transition;
class Image;

class ObstacleDetector 
{
public:
    virtual ~ObstacleDetector();
    
    virtual void searchObstacles(   const Image& image,
									const ScanResult* scanResult, 
									Time time = Time(),
									VisibleObjectList* vol=0, 
									bool writeWM = true)
									throw (RobotsException) =0;
	
    virtual void setVisualize(Image* vis) {;}
};

#endif
