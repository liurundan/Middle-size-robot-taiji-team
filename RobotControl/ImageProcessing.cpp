#include "StdAfx.h"
#include "ImageProcessing.h"

#include "ImageProcessing.h"
#include "ImageProcessingFactory.h"
#include "Journal.h"
  
using namespace std;

const char*  ImageProcessing::get_image_processing_type () const throw () { return image_processing_descriptor; }

ImageProcessing::~ImageProcessing () throw () 
{
  delete the_image_processing;
  delete [] image_processing_descriptor;
}

ImageProcessing::ImageProcessing (  ConfigReader& vread) throw (RobotsException, bad_alloc)
 : the_image_processing(NULL), configuration_list(vread) 
{
  string confline;
  if (vread.get("image_processing_type", confline)<=0) 
  {
    JERROR("no config line \"image_processing_type\" found");
    throw  InvalidConfigurationException ("image_processing_type");
  }
  really_change_image_processing_type (confline.c_str(), vread);
}

bool ImageProcessing::change_image_processing_type (const char* ipt) throw () 
{
  return change_image_processing_type (ipt, configuration_list);
}

bool ImageProcessing::change_image_processing_type (const char* ipt,   ConfigReader& vread) throw () 
{
  try
  {
    really_change_image_processing_type (ipt, vread);
  }
  catch(bad_alloc&)
  {
    JWARNING("Change of image processing type failed due to lack of memory");
    return false;
  }
  catch(RobotsException&)
  {
    JWARNING("Change of image processing type failed");
    return false;
  }
  return true;
}

void  ImageProcessing::really_change_image_processing_type (const char* ipt,   ConfigReader& vread) 
throw (RobotsException, bad_alloc) 
{
  ImageProcessingType*	new_image_processing;
  char*					new_descriptor;
  try
  {
    string ipts (ipt);
    new_image_processing = ImageProcessingFactory::get_image_processing_factory()->get_image_processing(ipts, vread);
  }
  catch(invalid_argument&)
  {
    throw  InvalidConfigurationException ("image_processing_type");
  }
  
  new_descriptor = new char [strlen(ipt)+1];
  strcpy(new_descriptor,ipt);
  if (the_image_processing!=NULL) 
  {
    delete the_image_processing;
    delete [] image_processing_descriptor;
  }
  the_image_processing			=	new_image_processing;
  image_processing_descriptor	=	new_descriptor;
}