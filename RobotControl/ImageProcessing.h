#ifndef _image_processing_h
#define _image_processing_h

#include <stdexcept>
#include "ImageProcessingType.h"
#include "RobotsException.h"
#include "ConfigReader.h"
#include "Image.h"

struct VisibleObjectList;

class ImageProcessing
{
public:
	 ImageProcessing (  ConfigReader&) throw (RobotsException, std::bad_alloc);
    ~ImageProcessing () throw ();
 
    bool change_image_processing_type (const char*) throw ();
    
    bool change_image_processing_type (const char*, ConfigReader&) throw ();
    
    const char* get_image_processing_type () const throw ();

    inline void process_image () throw (BadHardwareException) {  the_image_processing->process_image (); }

    inline const VisibleObjectList& get_last_seen_objects() const throw()
    { return the_image_processing->get_last_seen_objects(); }
    
    void request_image_raw()  throw(RobotsException) {   the_image_processing->request_image_raw (); }
    void request_image_processed()  throw(RobotsException) {   the_image_processing->request_image_processed(); }
    bool is_image_available() throw() { return the_image_processing->is_image_available(); }
    const Image* get_image()  throw(RobotsException) { return the_image_processing->get_image();}  
    void free_image() throw(RobotsException) {   the_image_processing->free_image(); }
 
private:
	ImageProcessingType* the_image_processing;
    char* image_processing_descriptor;
    ConfigReader& configuration_list;

    void really_change_image_processing_type (const char*,   ConfigReader&) throw (RobotsException, std::bad_alloc);
};

#endif