#ifndef _imagebuffer_h_
#define _imagebuffer_h_

#include "RobotsException.h"

class ImageBuffer
{
public:
	enum {
      FORMAT_YUV444=0,
      FORMAT_YUV422,
      FORMAT_YUV411,
      FORMAT_RGB,
      FORMAT_MONO,
      FORMAT_MONO16,
      FORMAT_UYV
    };     
	
	int width;              
    int height;              
    int format;             
    int size;                
    unsigned char* buffer;   
	
	ImageBuffer(int width=0, int height=0, int format=0, 
		unsigned char* buffer=0, int size=0);
	
	 static void convert(const ImageBuffer& src, ImageBuffer& dst) 
      throw (RobotsException);	
 
};

#endif