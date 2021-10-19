#ifndef _image_processing_type_h
#define _image_processing_type_h

#include "RobotsException.h"
#include "VisibleObject.h"
#include "Image.h"

class ImageProcessingType
{
public:  
	virtual ~ImageProcessingType () throw () {;}
    
    virtual void process_image () throw (BadHardwareException) =0;    
    virtual const VisibleObjectList& get_last_seen_objects() const throw()
    { return lastSeenObjects; }

    virtual void request_image_raw() throw(RobotsException) {;}
    virtual void request_image_processed() throw(RobotsException) {;}
    virtual bool is_image_available() throw() { return false; }
    virtual const Image* get_image() throw(RobotsException) 
	{ throw RobotsException("No image ready. This feature is not implemented in this ImageProcessingType."); }
    
	virtual void free_image() throw(RobotsException) {;}

  protected:
    VisibleObjectList lastSeenObjects;
};

 

#endif
