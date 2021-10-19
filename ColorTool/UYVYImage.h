#ifndef _UYVYImage_h_
#define _UYVYImage_h_

#include "Image.h"

class UYVYImage : public Image
{
public:
	UYVYImage(const ImageBuffer& buffer);
    UYVYImage(int width, int height);
    virtual ~UYVYImage();

    virtual Image* clone() const;

    virtual void getPixelYUV (int x, int y, YUVTuple* yuv) const;
    virtual void getPixelUYVY(int x, int y, UYVYTuple* uyvy) const;
    virtual void getPixelRGB (int x, int y, RGBTuple* rgb) const;

    virtual void setPixelYUV (int x, int y, const YUVTuple& yuv);
    virtual void setPixelUYVY(int x, int y, const UYVYTuple& uyvy);
    virtual void setPixelRGB (int x, int y, const RGBTuple& rgb);

    virtual unsigned char getPixelClass(int x, int y) const;
protected:
	    bool controlsBuffer; 

};
#endif