#ifndef _MorphologicOperators_h_
#define _MorphologicOperators_h_

#include "Image.h"

class ImageOperator
{
public: 
    
	virtual ~ImageOperator() {};
    virtual Image* operator() (const Image&) const =0;
};

class Dilatation : public ImageOperator
{
public: 
	Dilatation(int size = 3);
    
    virtual ~Dilatation();
    virtual Image* operator() (const Image&) const;

protected:
    int size;
};

class Erosion : public ImageOperator
{
public: 
	Erosion(int size = 3);
    
    virtual ~Erosion();
    virtual Image* operator() (const Image&) const;

  protected:
    int size;
};

#endif