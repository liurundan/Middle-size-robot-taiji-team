#ifndef _imageproducer_h_
#define _imageproducer_h_

#include "Image.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include "ImageMonitor.h"
#include "Camera.h"
#include "UsbCamera.h"

#include <string>

using std::string;

class ImageSource;
   
class ImageProducer
{
public:
	ImageProducer(ConfigReader& config) throw(RobotsException);
 
    ImageProducer( ConfigReader& config,const std::string& image_source, const std::string& image_handler)      
      throw(RobotsException);
  
    ~ImageProducer();
 
    Image*	nextImage() throw (BadHardwareException, RobotsException);

    enum { TYPE_RGBImage=0, TYPE_UYVYImage, TYPE_YUVImage };
    
    bool	setFramerate(int fps);

	int     getWidth();  
	int     getHeight();
	
    string	GetImageSourceType() { return imgSourceType2; }
protected:

	void init( ConfigReader& config, const string& image_source, const string& image_handler);

    ImageSource*		imgSource; 
    ColorClassifier*	classifier; 
    ImageMonitor*		imgMonitor;  
    int					failCount;         
    int					retriesMax;         
    int					imageType;         

    int					camera_delay;      

    ImageBuffer			buffer;	   
    bool				fromFile;
 	bool				fromUsb;

	string				imgSourceType2;  
};
#endif