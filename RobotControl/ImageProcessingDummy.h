#ifndef _image_processing_dummy_h
#define _image_processing_dummy_h

#include "ImageProcessingType.h"
#include "WorldModel.h"

class ImageProcessingDummy:public ImageProcessingType
{
public:
	void process_image () throw ();
};

#endif
