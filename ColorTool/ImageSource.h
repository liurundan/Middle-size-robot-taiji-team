#ifndef _imagesource_h_
#define _imagesource_h_

#include "RobotsException.h"
#include "ImageBuffer.h"

class ImageSource
{
public:
	virtual ImageBuffer getImage() throw (HardwareException, RobotsException)=0;
    virtual int getWidth() const=0;
    virtual int getHeight() const=0;
    virtual bool setFramerate(int fps) { return false; }
    virtual int getDelay () const throw () { return 0; }

};
#endif

