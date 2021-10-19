#ifndef _chaincoding_h_
#define _chaincoding_h_
  
#include <vector>
#include "Vec.h"

 

  class Image;

  class Region 
  {
  public:
    int colClass;
    int x, y;
    std::vector<char> chainCode;
    int maxX, maxY, minX, minY;

    inline Region(int colClass, int x, int y);
    inline Region();
      
    int		getArea() const;
    Vec		getCenterOfGravity() const;
    double	getCompactness() const;

  protected:
    int area;
    Vec centerOfGravity;
  };

  /**
   
   *
   *    |1           y ^ 
   * 2  |              |
   * --- ---           |
   *    |  0           |
   *   3|              ------> x
   *
   
   */
  class ChainCoder 
  {
  public:
    static int xDir[4];
    static int yDir[4];

    virtual ~ChainCoder() {;}
    virtual int traceBorder(const Image& image, int x, int y, Region* region,
			    char* borderMap = 0);

    void setVisualize(Image* vis) { this->vis = vis; }
  protected:
    Image* vis;

    void visualizeRegion(const Region* region);
  };

  class RegionDetector 
  {
  public: 
    
	RegionDetector(); //

    virtual ~RegionDetector();
   
    virtual void findRegions (const Image& image, int colClass, 
			      std::vector<Region>* regions) const;
  protected:
    char* buf;
    int size;
    ChainCoder* cc;
  };
    
  // inlines

  Region::Region(int colClass, int x, int y) 
    : colClass(colClass), x(x), y(y), maxX(x), maxY(y), minX(x), minY(y),
      area(-1)
  {}

  Region::Region()
    : colClass(0), x(0), y(0), maxX(0), maxY(0), minX(0), minY(0),
      area(-1)
  {}
 
 
#endif
