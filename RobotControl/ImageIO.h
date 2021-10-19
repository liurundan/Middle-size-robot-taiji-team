#ifndef _ImageIO_h_
#define _ImageIO_h_

#include "Image.h"
#include "RobotsException.h"
#include <string>
#include <iostream>

using std::ostream;
using std::istream;

class ImageIO
{
public:
	virtual ~ImageIO() {};
    virtual void write(const Image& image, const std::string& filename) const
      throw(RobotsException);
    virtual void write(const Image& image, ostream& out) const
      throw(RobotsException) =0;
	
	virtual Image* read(Image* image, const std::string& filename) const
      throw(RobotsException);
    virtual Image* read(Image* image, istream& in) const
      throw(RobotsException) =0;

    virtual std::string getDefEnding() const { return ""; }
    virtual std::string getTypeId() const { return ""; }
 
};

class PPMIO : public ImageIO
{
public: 
	virtual void write(const Image& image, ostream& out) const
      throw(RobotsException);
    
    virtual Image* read(Image* image, istream& in) const
      throw(RobotsException);

    virtual std::string getDefEnding() const;
    virtual std::string getTypeId() const;
};

#endif