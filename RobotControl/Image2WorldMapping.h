#ifndef _image2worldmapping_h_
#define _image2worldmapping_h_

#include <string>

#include "Vec.h"
#include "geometry.h"
#include "RobotsException.h"

class CoordinateMapping
{
public:
	virtual ~CoordinateMapping() {};

    virtual Vec map(const Vec& vec) const =0;
    virtual Line map(const Line& line) const;
    virtual LineSegment map(const LineSegment& line) const;
 
};

class Image2WorldMapping : public CoordinateMapping
{
public:
	 Image2WorldMapping(std::string filename);
    Image2WorldMapping(int width, int height);
    virtual ~Image2WorldMapping();
    
   
    virtual Vec map(const Vec& vec) const;
   
    virtual Vec map(int x, int y) const;

 
    virtual void load(std::string filename);
  
    virtual void save(std::string filename) const;
 
    virtual void set(int x, int y, const Vec& vec);
    
    virtual int getWidth() const { return width; };
    virtual int getHeight() const { return height; }

protected:
	Vec* lut;    
    int width;
    int height;
 
};

class World2Image : public CoordinateMapping
{
public:
	World2Image(std::string filename);
    virtual ~World2Image();

    virtual Vec map(const Vec& vec) const;

    void setMiddle(const Vec& middle);
    Vec getMiddle() const;

protected:
	int width, height;
    Vec middle;
    int shift;

    std::vector<double> realDistances;
    std::vector<std::vector< double> > imageDistances;
    std::vector<double> angleCorrections;
 
};

#endif 