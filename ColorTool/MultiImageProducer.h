#ifndef _multiimageproducer_h_
#define _multiimageproducer_h_

#include <stdio.h>
#include <vector>
#include "Image.h"
#include "ImageProducer.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include "ImageMonitor.h"

class ImageSource;

class MultiImageProducer
{
public:
    MultiImageProducer( ConfigReader& config);
    ~MultiImageProducer();

    Image* nextImage() throw (BadHardwareException);

    Image* nextImage(int producer_id) throw (BadHardwareException);

    int numSources() { return producers.size(); }

public:
    std::vector <ImageProducer*> producers;

protected:

};

#endif

