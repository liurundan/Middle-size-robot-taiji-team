#ifndef _simpleobstacledetector_h_
#define _simpleobstacledetector_h_

#include "RobotsException.h"
#include "Image2WorldMapping.h"
#include "Time.h"
#include "Vec.h"
#include "ObstacleDetector.h"
#include "SimpleCluster.h"
#include "ConfigReader.h"

#include <vector>

struct VisibleObjectList;
struct ScanResult;
struct Transition;
class  Image;

class SimpleObstacleDetector: public ObstacleDetector
{
public:
	SimpleObstacleDetector(const ConfigReader& config,
			   const CoordinateMapping* mapping,
			   const CoordinateMapping* rel2img,
			   double minTransitionSize = 4);

    virtual ~SimpleObstacleDetector();

    virtual void searchObstacles(const Image& image,
				 const ScanResult* scanResult, 
				 Time time = Time(),
				 VisibleObjectList* vol=0, 
				 bool writeWM = true)
      throw (RobotsException);

    virtual void setVisualize(Image* vis) { this->vis=vis; }
protected:
	 const CoordinateMapping* mapping;  
    const CoordinateMapping* rel2img;  
    double minTransitionSize;
    double clustering_thresh;
    double min_width;

    Image* vis;


    void paintResults(SimpleVecCluster& cluster);
 
};

#endif