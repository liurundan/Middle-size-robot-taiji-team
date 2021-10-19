#ifndef _balldetector_h_
#define _balldetector_h_

#include "RobotsException.h"
#include "Image2WorldMapping.h"
#include "Time.h"
#include "VisibleObject.h"
#include "Vec.h"
#include "Frame2D.h"

#include <vector>

struct ScanResult;
class Image;
class Region;

class ImageParticleTracker
{
public:
	ImageParticleTracker(int colClass, int centerX, int centerY,
			 int windowWidth  = 40,
			 int windowHeight = 40,
			 int n = 400, 
			 unsigned int minParticles = 4);
    ~ImageParticleTracker() {;};

    void addParticle(const Vec& p);
    void propagate(const Image& image, const CoordinateMapping& img2robot, 
		   const Frame2d& robot2world);

    Vec getPositionMean() const;

    bool found() const;

    void setVisualize(Image* vis) { this->vis=vis; }

protected:
	int colClass;
    Vec center;
    int windowWidth;
    int windowHeight;
    int n;
    unsigned int minParticles;

    std::vector<Vec> particles;
    std::vector<Vec> survivingparticles;

    Vec middle;      ///< mean of all positive particles
    double max_x, max_y;
    Image* vis;

    void visualizeSample(int x, int y, int c);
    void visualizeCenter();
 
};

class NearestPixelFinder
{
public:
	NearestPixelFinder(const Image& image, const Vec& imageCenter);
    Vec find(const Vec& pos, int color);
protected:
	const Image& image;
    const Vec& imageCenter;
 
};

class BallDetector
{
public:
	 BallDetector(const CoordinateMapping* mapping, 
		 int centerX, int centerY, 
		 bool useRegion=true, bool useTrig=true);
	  void searchBall(const Image& image,
		    const ScanResult* scanResult, Time time = Time(),
		    VisibleObjectList* vol=0, bool writeWM = true)
      throw (RobotsException);

    void setVisualize(Image* vis) { this->vis=vis; }
protected:
	const CoordinateMapping* mapping;   
    ImageParticleTracker tracker;
    Image* vis;
    int centerX, centerY;
    bool useRegion;  ///< use chaincoder to analyze ball-cluster
    bool useTrig;    ///< use projection theorems to correct ball position

    void visualizeBall(const Vec& pos);
    void visualizeRegion(const Region& region);
 
};

class InterceptTheoremCorrection
{
public:
	InterceptTheoremCorrection(double cameraHeight);
 
    Vec correct(const Vec& point, double height) const;
protected:
	double cameraHeight;
 
};

class LookForColorInSurrounding
{
public:
	 LookForColorInSurrounding(const Image& image);
    Vec search(const Vec& start, int color, int maxDist) const;    
protected:
	const Image& image;
 
};
#endif