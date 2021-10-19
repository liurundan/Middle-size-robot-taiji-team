#ifndef _goaldetector_h_
#define _goaldetector_h_

#include "RobotsException.h"
#include "Image2WorldMapping.h"
#include "Time.h"
#include "Image.h"
#include "Vec.h"

struct VisibleObjectList;
class Image;

class GoalDetector
{
public:
	GoalDetector(int centerX, int centerY, 
		 const CoordinateMapping* mapping = 0,
		 int width = 65, int height = 65, int n = 80);
    virtual ~GoalDetector() { ; }

    /**
     * Benutzt das Weltmodell, um die Position der Tore abzuschätzen und
     * sucht dort in einem Suchfenster nach gelben und blauen Pixeln.
     * Werden von der einen Farbe genügend viele und von der anderen
     * sehr wenige gesehen, gilt ein Tor als gefunden.
     */
    virtual void searchGoals(const Image&, Time time = Time(),
			     VisibleObjectList* vol=0, 
			     bool writeWM = true)
      throw (RobotsException);

    void setVisualize(Image* vis) { this->vis=vis; }
protected:
	int getDominantColor(const Image& image, const Vec& position, int n);
    /** Used to draw a bold rectangle of the color of the detected goal in the 
     *  image (vis) */
    void visualizeGoal(const Vec& pos, int col);

    Vec center;
    int width; 
    int height;
    int n;

    const CoordinateMapping* mapping;

    Image* vis;      
 
};


#endif
