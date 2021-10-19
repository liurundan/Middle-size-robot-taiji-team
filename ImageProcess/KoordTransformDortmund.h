#ifndef _KoordTransformDortmund_h_
#define _KoordTransformDortmund_h_

#include <string>
#include <vector>
#include "Image2WorldMapping.h"

class Image2WorldDortmund : public CoordinateMapping
{
public:
	Image2WorldDortmund( double w = 640, double h = 480, 
		double offsetX = 830, double offsetY = -25, double real_width = 780,
		double al = 950, double ch = 380, double dis_ct = 50 );
	Image2WorldDortmund(std::string filename);
    virtual ~Image2WorldDortmund();

    virtual Vec map(const Vec& vec) const;

    Image2WorldMapping* convert() const;

    virtual Vec mapUsb(const Vec& vec) const;
    Image2WorldMapping* convertUsb() const;

    void setMiddle(const Vec& middle);
    Vec  getMiddle() const;

protected:
	int width, height;
    Vec middle;
    int shift;

    std::vector<double> realDistances;
    std::vector<std::vector< double> > imageDistances;
    std::vector<double> angleCorrections;
    
	double axis_length_offsetX;   // basic param
	double axis_length_offsetY;   // basic param

 	double focal_dis;
	double axis_length;
	double camera_height;
	double dis_CameraToRobot;
	double ball_radius;   

	double beta;
	double theta;
    
};

#endif