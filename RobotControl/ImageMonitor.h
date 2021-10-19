#ifndef _imagemonitor_h_
#define _imagemonitor_h_

#include "Time.h"
#include "Image.h"

class ImageMonitor
{
public:
	virtual ~ImageMonitor() {};
    virtual void monitor(const Image& image, 
			 const Time& available, const Time& created)=0;
};

#endif