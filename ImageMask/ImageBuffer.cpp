#include "StdAfx.h"
#include "ImageBuffer.h"
#include "ImageConversion.h"

ImageBuffer::ImageBuffer(int width, int height, int format, 
			   unsigned char* buffer, int size)
    : width(width), height(height), format(format), buffer(buffer), size(size)
{
 
}

void ImageBuffer::convert(const ImageBuffer& src, ImageBuffer& dst) throw (RobotsException)
{
	if (src.width != dst.width ||	src.height != dst.height) 
	{
	   throw RobotsException(__FILE__
			     ": Source and target image buffer don't match");
	}

	if (src.format == dst.format) 
	{
	  if (src.size != dst.size) 
	  {
		throw RobotsException(__FILE__
			       ": Source and target image buffer have same "
			       "dimensions and format, but the size of their "
			       "buffers differ");
	  }
	  memcpy(dst.buffer, src.buffer, src.size * sizeof(src.buffer[0]));
	}
	else 
	{
 	   const ImageConverter* convert =    
	    ImageConversionRegistry::getInstance()->getConverter(src.format,
 							 dst.format);
 	    (*convert)(src, dst);    
	}
}
