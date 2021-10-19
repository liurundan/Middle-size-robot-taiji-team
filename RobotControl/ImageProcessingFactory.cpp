#include "StdAfx.h"
#include "ImageProcessingFactory.h"
#include "ScanLineImageProcessing.h"
#include "ImageProcessingDistanceCalibration.h"
#include "ImageProcessingDummy.h"
#include "SimulatorImageProcessing.h"

using namespace std;

ImageProcessingFactory*  ImageProcessingFactory::the_only_factory = NULL;

ImageProcessingFactory::ImageProcessingFactory () throw () {;}

ImageProcessingFactory* ImageProcessingFactory::get_image_processing_factory () throw (std::bad_alloc) 
{
  if (!the_only_factory)
  {
    the_only_factory = new ImageProcessingFactory;
	::atexit(destroy);
  }
  return the_only_factory;
}

ImageProcessingFactory::~ImageProcessingFactory() throw () {;}
 
ImageProcessingType* ImageProcessingFactory::get_image_processing (const std::string descriptor, 
																     ConfigReader& reader) 
																   throw (RobotsException,bad_alloc,invalid_argument) 
{
  ImageProcessingType* new_wm = NULL;
 
  if( descriptor == "ScanLine" )
  {
     new_wm = new ScanLineImageProcessing (reader);
  }
  else if (descriptor == "DistanceCalibration") 
  {
	 new_wm = new ImageProcessingDistanceCalibration (reader);
  }
  else if (descriptor == "Dummy")
  {
	  new_wm = new ImageProcessingDummy;
  }
  else if( descriptor == "Simulator" )
  {
	  new_wm = new SimulatorImageProcessing(reader);
  }
  return new_wm;
  
}

void ImageProcessingFactory::destroy()
{
   if( the_only_factory != NULL)
   {
	   delete the_only_factory;
	   the_only_factory = NULL;
   }
}