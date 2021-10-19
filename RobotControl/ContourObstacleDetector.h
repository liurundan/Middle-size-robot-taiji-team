#ifndef _contourobstacledetector_h_
#define _contourobstacledetector_h_

#include "RobotsException.h"
#include "Image2WorldMapping.h"
#include "Time.h"
#include "Vec.h"
#include "ChainCoding.h"
#include "ObstacleDetector.h"

#include <vector>



struct VisibleObjectList;
struct ScanResult;
struct Transition;
class Image;

class ContourObstacleDetector : public ObstacleDetector
{
public:
	ContourObstacleDetector(const CoordinateMapping* mapping,
			    ChainCoder* cc,
			    double minTransitionSize = 4);

    virtual ~ContourObstacleDetector();

    
    virtual void searchObstacles(const Image& image,
				 const ScanResult* scanResult, 
				 Time time = Time(),
				 VisibleObjectList* vol=0, 
				 bool writeWM = true)
      throw (RobotsException);
protected:
	const CoordinateMapping* mapping;  ///< Koordinatentransformation
    ChainCoder* cc;

    char* borderMap;
    int borderMapSize;

    double minTransitionSize;
 
};

#endif