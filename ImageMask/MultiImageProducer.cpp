#include "StdAfx.h"
#include "MultiImageProducer.h"

#include <string>
#include <iostream>
#include "Camera.h"
#include "ImageSource.h"
#include "FileSource.h"
#include "ImageBuffer.h"
#include "RGBImage.h"
#include "YUVImage.h"
#include "UYVYImage.h"
#include "ColorClassifier.h"
#include "YUVLookupTable.h"
#include "Time.h"
#include "ImageIO.h"
#include "FileMonitor.h"

using std::string;

MultiImageProducer::MultiImageProducer( ConfigReader& config)
{
    int ret;
    int num_sources;
 	
    ret = config.get("MultiImageProducer::image_sources", num_sources);
    if (ret <= 0) 
	{ 
		throw InvalidConfigurationException("MultiImageProducer::image_sources");
    }
	
	// initialize all image producers
    for( int i=0; i<num_sources; i++ ) 
	{
		char buffer[3];
		sprintf(buffer,"%d",i);
		string suffix(buffer);
		
		string image_source;
		string image_handler;
		
		ret = config.get(("MultiImageProducer::image_source_" + suffix).c_str(), image_source);
		if (ret <= 0) 
		{ // specified, but parse failed
			throw InvalidConfigurationException(("MultiImageProducer::image_source_" + suffix).c_str());
		}
		ret = config.get(("MultiImageProducer::image_handler_" + suffix).c_str(), image_handler);
		if (ret <= 0) 
		{ // specified, but parse failed
			throw InvalidConfigurationException(("MultiImageProducer::image_handler_" + suffix).c_str());
		}
		
		producers.push_back(new ImageProducer(config, image_source, image_handler));
    }
 	
}

MultiImageProducer::~MultiImageProducer()
{
	for(unsigned int i=0; i<producers.size(); i++)
        delete producers[i];
}
 
Image* MultiImageProducer::nextImage() throw (BadHardwareException)
{
	return nextImage(0);
}
 
Image* MultiImageProducer::nextImage(int cam_id) throw (BadHardwareException)
{
	if( (int)producers.size() > cam_id )
		return (producers[cam_id])->nextImage();
	else
		throw new BadHardwareException("MultiImageProducer::nextImage(int) : Tried to aquire an image from an unknown camera id.");
	
}