#ifndef _yuvlut_h_
#define _yuvlut_h_

#include "ColorClassifier.h"
#include "Image.h"

class YUVLookupTable : public ColorClassifier
{
public:
	YUVLookupTable(int shiftY = 1,
		   int shiftU = 2,
		   int shiftV = 2);		  
   
    virtual ~YUVLookupTable();
 
    virtual const unsigned char& lookup(const RGBTuple&  rgb)  const;
    
    virtual const unsigned char& lookup(const YUVTuple&  yuv)  const;
    
    virtual const unsigned char& lookup(const UYVYTuple& uyvy, int pos=0)const;
 
    virtual void set(const RGBTuple&  rgb,  unsigned char c);
   
    virtual void set(const YUVTuple&  yuv,  unsigned char c);
   
    virtual void set(const UYVYTuple& uyvy, unsigned char c, int pos=0);
 
    virtual void load(string filename);
    
    virtual void save(string filename) const;
 
    virtual ColorClassifier* create() const;
 
    virtual void fillFromClassifier(const ColorClassifier* cc);
	
protected:
	int shiftY;    
    int shiftU;   
    int shiftV;    

    int sizeY;    
    int sizeU;    
    int sizeV;     
    int size;      

    unsigned char* lut; 
private:
	YUVTuple yuvTmp;   

};

#endif