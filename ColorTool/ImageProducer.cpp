#include "StdAfx.h"
#include "ImageProducer.h"

#include <string>

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

using namespace std;

struct KeyValueEntry
{
    string key;
    int value;
    KeyValueEntry(string,int);
};

KeyValueEntry::KeyValueEntry(string s, int k)
{
	key   = s;
	value = k;
}  

static const KeyValueEntry image_buffer_types1("YUV",  ImageProducer::TYPE_YUVImage);
static const KeyValueEntry image_buffer_types2("RGB",  ImageProducer::TYPE_RGBImage); 
static const KeyValueEntry image_buffer_types3("UYVY", ImageProducer::TYPE_UYVYImage);

ImageProducer::ImageProducer(ConfigReader& config) 
throw(RobotsException)
: imgSource(0), classifier(0), imgMonitor(0), failCount(0), 
imageType(TYPE_YUVImage) 
{
    ImageProducer::init(config,"Camera_default", "ImageProducer_default"); 
} 


ImageProducer::ImageProducer(ConfigReader& config, const std::string& image_source, 
							 const std::string& image_handler)
							 throw(RobotsException)
							 : imgSource(0), classifier(0), imgMonitor(0), failCount(0),
							 imageType(TYPE_YUVImage)
{
	ImageProducer::init(config,image_source, image_handler);
} 

void ImageProducer::init( ConfigReader& config,
						 const string& image_source, 
						 const string& image_handler)
{
    fromFile	= false;
	fromUsb     = false;
	retriesMax  = 1000;
    int			iTmp = -1;
    int			ret	 = -1;  
	
	string		strMfc;
    string		sTmp = ""; 
    string		imgSourceType = "" ;
    string		classifierType;
    
	ret = config.get((image_handler+"::max_retries").c_str() , iTmp);
    if (ret < 0) 
	{ 
		throw InvalidConfigurationException((image_handler+"::max_retries").c_str());
    }
	
    if (ret > 0) 
	{  
		retriesMax = iTmp;
    }
    ret = config.get((image_handler+"::image_buffer_type").c_str(), sTmp);
    if (ret <= 0) 
	{ 
		throw InvalidConfigurationException((image_handler+"::image_buffer_type").c_str());
    }
    if (ret > 0) 
	{  
		if ( sTmp == "YUV" )
		{
			imageType = TYPE_YUVImage;
		}
		else if (sTmp == "RGB")
		{
			imageType = TYPE_RGBImage;
		}
    }
	
	if (config.get((image_handler+"::image_source_type").c_str(), 
		imgSourceType) <= 0) 
	{
		throw InvalidConfigurationException((image_handler+"::image_source_type").c_str());
    }
	
	if (imgSourceType == "CameraSource") 
	{ 
		int frammerate = -1;
		PdtImageFormat fmt = fmtYuv422;
		if (config.get((image_source+"::framerate").c_str(), frammerate) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::framerate").c_str());
		}
		if (config.get((image_source+"::mode").c_str(), sTmp) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::framerate").c_str());
		}
		if (sTmp == "RGB24")
		{
			fmt = fmtRgb24;
		}
		imgSource  = Camera::getCamera(frammerate,fmt);
		imageType  = TYPE_YUVImage;
		
	}
    else if (imgSourceType == "FileSource")
	{
		if (config.get((imgSourceType+"::filename").c_str(), sTmp) <= 0) 
		{ 
			throw InvalidConfigurationException((imgSourceType+"::filename").c_str());
		} 
		fromFile  = true;	 
		imageType = TYPE_RGBImage;
		imgSource = new FileSource(new PPMIO(), sTmp );
    }
	else if (imgSourceType == "UsbCameraSource") 
	{
		int frammerate = -1;
		int port = 0;
		int iwidth  = 0;
		int iheight = 0;
		if (config.get((image_source+"::device_port").c_str(), port) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::port").c_str());
		}
		if (config.get((image_source+"::framerate").c_str(), frammerate) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::framerate").c_str());
		}
		if (config.get((image_source+"::mode").c_str(), sTmp) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::mode").c_str());
		}
		if (config.get((image_source+"::width").c_str(), iwidth) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::width").c_str());
		}
		if (config.get((image_source+"::height").c_str(), iheight) < 0 ) 
		{
			throw InvalidConfigurationException((image_source+"::heigth").c_str());
		}
		fromUsb = true;
		imgSource  = UsbCamera::getCamera(port,frammerate,iwidth,iheight,sTmp.c_str());
		
	}
    else 
	{
		throw InvalidConfigurationException((image_handler+"::image_source_type").c_str());
    }
	
    imgSourceType2  = imgSourceType;
	
	ret = config.get(	(	image_handler+"::color_classifier_type").c_str(), 
						classifierType);
    if (ret < 0) 
	{
      throw InvalidConfigurationException( (image_handler+"::color_classifier_type").c_str() );
    }
    else if ( ret > 0 )
	{
		if (classifierType == "YUVLut") 
		{                
			classifier = new YUVLookupTable();
		}
		ret = config.get((image_handler+"::color_classifier_file").c_str(), sTmp );
		if (ret < 0) 
		{
			throw InvalidConfigurationException((image_handler+"::color_classifier_file").c_str());
		}
		else if (ret > 0) 
		{
			classifier->load(sTmp);
			classifier->save(("test"+image_handler+".lut").c_str());
		}
    }
    
    string monitorSection;
	
    ret=config.get((image_handler+"::monitor").c_str(),monitorSection); 

	if (ret > 0) 
	{
		
		string filenameBase = "Logs\\image";
		bool   singleFile	= false;
		int    step			=	1;
		
		// output image data as a logfile
		if (config.get((monitorSection+"::filename_base").c_str(), 
			filenameBase) <= 0) 
		{
			throw InvalidConfigurationException(
				(monitorSection+"::monitor_filename_base").c_str());
		}
		if (config.get((monitorSection+"::single_file").c_str(), 
			singleFile) < 0) 
		{
			throw InvalidConfigurationException((monitorSection+"::single_file").c_str());
		}
		
		if (config.get((monitorSection+"::step").c_str(), step) <= 0) 
		{
			throw InvalidConfigurationException((monitorSection+"::step").c_str());
		}
		sTmp = "PPM";
		
		if (config.get((monitorSection+"::file_type").c_str(), sTmp) < 0)
		{
			throw InvalidConfigurationException((monitorSection+"::file_type").c_str());
		}
		if ( sTmp == "PPM" ) 
		{  
			imgMonitor = new FileMonitor(new PPMIO(), filenameBase, step,
				singleFile);
		}
		else 
		{
			throw InvalidConfigurationException( (monitorSection+"::file_type").c_str());
		}
	}
	
}

ImageProducer::~ImageProducer()
{
	if( imgSource != 0 ) 
	{
		if( fromFile ) 
		{
			delete imgSource;
		}
		else if( fromUsb )
		{
			UsbCamera::destroyUsbCamera();
		}
		else
		{
			Camera::destroyCamera();
		}
	}
	
	if( classifier != 0 ) 
	{
		delete classifier;
	}
	
	if( imgMonitor != 0 ) 
	{
		delete imgMonitor;
	} 
}

Image* ImageProducer::nextImage() throw (BadHardwareException, RobotsException)
{
    ImageBuffer buffer;
    do 
	{
        try
		{
			buffer = imgSource->getImage();
		} 
		catch (HardwareException& e)
		{
			
			if (failCount++ >= retriesMax) 
			{
				string msg = __FILE__": There is a serious  hardware error. "
					"A number of retries have not helped to solve the problem. ";
				msg += string("\nOriginal Exception:\n:") + e.what(); 
				throw BadHardwareException(msg.c_str());
			}
			else
			{
				continue;
			}
		}
		failCount = 0;
	}
	while (failCount > 0);
	
	Time   imageTimestamp;    // remember Timestamp for the image
	Image* image;
	
	try
	{
		switch (imageType) 
		{
		case TYPE_RGBImage:  image = new RGBImage (buffer); break;
		case TYPE_UYVYImage: image = new UYVYImage(buffer); break;
		default:             image = new YUVImage (buffer); break;
		}
	}
	catch (RobotsException& e) 
	{
		string msg =
			__FILE__
			": There has occured an error while creating an Image arround "
			"the ImageBuffer that has been received from the ImageSource. "
			"It is most likely, that there is no implementation of the "
			"needed conversion method to convert the format of the ImageBuffer "
			" to the format used by the selected Image class.\n\n"
			"Solution: Edit the configuration files and select another Image "
			"class that is compatible to the selected ImageSource.";
		msg += string("\n\nOriginal Exception: ") + e.what();
		
		throw new BadHardwareException(msg.c_str());
	}	
	
    if (classifier!=0)
	{
		Time iTp = imageTimestamp;
		iTp.add_msec(-30);
		image->setTimestamp(iTp);
		image->setClassifier(classifier);
    }
    
	if( imgMonitor )
	{
		imgMonitor->monitor(*image, image->getTimestamp(),image->getTimestamp());
	}
	
    return image;
}

bool ImageProducer::setFramerate(int fps)
{
    return  imgSource->setFramerate(fps);
}

int ImageProducer::getWidth()
{
	return (imgSource->getWidth());
}

int ImageProducer::getHeight()
{
	return (imgSource->getHeight());
}