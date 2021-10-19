#ifndef _imageconversion_h_
#define _imageconversion_h_

#include <map>
#include "RobotsException.h"
using namespace std;
 
#pragma warning(disable: 4786)

class ImageBuffer;

class ImageConverter
{
public:
 
    ImageConverter(int srcFormat, int dstFormat);
  
    virtual void operator() (const ImageBuffer& src, ImageBuffer& dst) const
      throw (RobotsException)=0;
    
    int getSourceFormat() { return srcFormat; }
    
    int getDestinationFormat() { return dstFormat; }
protected:
	int srcFormat;    
    int dstFormat; 
     
};
 
class ImageConversionRegistry
{
public:
 
    static ImageConversionRegistry* getInstance();
    
    ~ImageConversionRegistry();
     
    const ImageConverter* getConverter(int srcFormat, int dstFormat) 
      throw (RobotsException);
  
    void registerConverter(ImageConverter* converter);

protected:
 	map<int, map<int, ImageConverter*> > converterMap;
    static ImageConversionRegistry* singleton;
 
    ImageConversionRegistry();    
};
  
class YUV2RGB : public ImageConverter
{
public:
	YUV2RGB();
    virtual void operator() (const ImageBuffer& src, ImageBuffer& dst) const
      throw (RobotsException);
};

class UYV2RGB : public ImageConverter
{
public:
 public:
    UYV2RGB();
    virtual void operator() (const ImageBuffer& src, ImageBuffer& dst) const
      throw (RobotsException);
};

class UYVY2YUV : public ImageConverter
{
public:
  UYVY2YUV();
    virtual void operator() (const ImageBuffer& src, ImageBuffer& dst) const
      throw (RobotsException);
};

class UYVY2RGB : public ImageConverter
{
public:
 UYVY2RGB();
    virtual void operator() (const ImageBuffer& src, ImageBuffer& dst) const
      throw (RobotsException);
};

#endif