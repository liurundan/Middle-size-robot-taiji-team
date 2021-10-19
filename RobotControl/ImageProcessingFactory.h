#ifndef _ImageProcessingFactory_h_
#define _ImageProcessingFactory_h_

#include "ImageProcessingType.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include <string>


class ImageProcessingFactory
{
public:
	 
    static ImageProcessingFactory* get_image_processing_factory () throw (std::bad_alloc);

    ImageProcessingType* get_image_processing (const std::string,   ConfigReader&) 
		throw (RobotsException,std::bad_alloc,std::invalid_argument);
   
protected:
private:
	static void destroy();
	static ImageProcessingFactory* the_only_factory;                
    ImageProcessingFactory () throw ();                             
    ~ImageProcessingFactory() throw ();           
};

#endif