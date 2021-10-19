#ifndef _image_h_
#define _image_h_

#include "ImageBuffer.h"
#include "Time.h"

class ColorClassifier;

typedef struct 
{
	unsigned char r;  
	unsigned char g; 
	unsigned char b;  
} RGBTuple;

typedef struct 
{
	unsigned char y;  
	unsigned char u;  
	unsigned char v;  
} YUVTuple;

typedef struct
{
    unsigned char u;  
    unsigned char y1; 
    unsigned char v;  
    unsigned char y2; 
} UYVYTuple;

typedef struct 
{
    unsigned char u;  
    unsigned char y;  
    unsigned char v; 
} UYVTuple; 

class Image 
{
public:

	Image(const ColorClassifier* classifier= 0); 
    virtual ~Image() {;}
    virtual Image* clone() const = 0;
 
    int getWidth() const;
   
    int getHeight() const;
     
    inline const Time& getTimestamp() const;
    
    inline  void setTimestamp(const Time& timestamp);
     
    virtual void getPixelYUV (int x, int y, YUVTuple* yuv) const =0;
      
    virtual void getPixelRGB (int x, int y, RGBTuple* rgb) const =0;
    
	virtual void getPixelUYVY(int x, int y, UYVYTuple* uyvy) const =0;

    virtual void setPixelYUV (int x, int y, const YUVTuple& yuv) =0;
      
    virtual void setPixelRGB (int x, int y, const RGBTuple& rgb) =0;
    
	virtual void setPixelUYVY(int x, int y, const UYVYTuple& uyvy) =0;

    virtual unsigned char getPixelClass(int x, int y) const =0;
 
    int getNativeFormat() const;

   
    virtual const ColorClassifier* getClassifier() const;   
   
    virtual void setClassifier(const ColorClassifier* classifier);
 
    inline ImageBuffer& getImageBuffer();
 
    virtual void setBlackBorder();
    
    virtual void setWhiteBorder();
 
    virtual void setBorder(const RGBTuple& rgb);

protected:
	ImageBuffer buffer;                 
    const ColorClassifier* classifier;   
    Time        timestamp;     
private:
 
    Image(const Image&) { ; }           
    Image& operator=(const Image&) {return *this; }

};

class PixelConversion 
{
  public:
    static void convert(const RGBTuple&  rgb,  YUVTuple* yuv);
    static void convert(const YUVTuple&  yuv,  RGBTuple* rgb);
    static void convert(const UYVYTuple& uyvy, YUVTuple* yuv, int pos=0);
    static void convert(const UYVYTuple& uyvy, RGBTuple* rgb, int pos=0);
};

#define PIXEL_YUV2RGB(y,u,v, r,g,b)                        \
      ((r) = (y)+(((v)*1436)>>10),                         \
       (g) = (y)-(((u)*352+(v)*731)>>10),                  \
       (b) = (y)+(((u)*1814)>>10)) 


#define PIXEL_RGB2YUV(r, g, b, y, u, v)                    \
      ((y) = (306*(r) + 601*(g) + 117*(b))  >> 10,         \
       (u) = ((-172*(r) - 340*(g) + 512*(b)) >> 10) + 128, \
       (v) = ((512*(r) - 429*(g) - 83*(b)) >> 10) + 128)

ImageBuffer&  Image::getImageBuffer()  {  return buffer;  }

const Time&  Image::getTimestamp() const { return timestamp; }

void Image::setTimestamp(const Time& timestamp)  { this->timestamp=timestamp; }

inline void PixelConversion::convert(const RGBTuple&  rgb,  YUVTuple* yuv)
{
    int y, u, v;
    PIXEL_RGB2YUV(rgb.r, rgb.g, rgb.b, y, u, v);
    yuv->y = static_cast<unsigned char>(y <0 ? 0 : (y >255 ? 255 : y ));
    yuv->u = static_cast<unsigned char>(u <0 ? 0 : (u >255 ? 255 : u ));
    yuv->v = static_cast<unsigned char>(v <0 ? 0 : (v >255 ? 255 : v ));
}

inline void PixelConversion::convert(const YUVTuple&  yuv,  RGBTuple* rgb)
{
    int y, u, v, r, g, b;
    y = yuv.y;
    u = yuv.u-128;
    v = yuv.v-128;

    PIXEL_YUV2RGB(y,u,v, r,g,b);
    
    rgb->r = static_cast<unsigned char>(r<0 ? 0 : (r>255 ? 255 : r));
    rgb->g = static_cast<unsigned char>(g<0 ? 0 : (g>255 ? 255 : g));
    rgb->b = static_cast<unsigned char>(b<0 ? 0 : (b>255 ? 255 : b));
}

inline void PixelConversion::convert(const UYVYTuple& uyvy, YUVTuple* yuv, int pos) 
{
    if (pos % 2 == 0) 
	{
      yuv->y = uyvy.y1;
    }
    else 
	{
      yuv->y = uyvy.y2;
    }
    yuv->u = uyvy.u;
    yuv->v = uyvy.v;
}

inline void PixelConversion::convert(const UYVYTuple& uyvy, RGBTuple* rgb, int pos)
{
    int y, u, v, r, g, b;
    y = (pos%2 == 0) ? uyvy.y1 : uyvy.y2;
    u = uyvy.u-128;
    v = uyvy.v-128;

    PIXEL_YUV2RGB(y,u,v, r,g,b);

    rgb->r = static_cast<unsigned char>(r<0 ? 0 : (r>255 ? 255 : r));
    rgb->g = static_cast<unsigned char>(g<0 ? 0 : (g>255 ? 255 : g));
    rgb->b = static_cast<unsigned char>(b<0 ? 0 : (b>255 ? 255 : b));
}

#endif