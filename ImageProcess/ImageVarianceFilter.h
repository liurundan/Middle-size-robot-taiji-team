#ifndef _ImageVarianceFilter_h_
#define _ImageVarianceFilter_h_
 
class Image;

class ImageVarianceFilter
{
public:
	virtual ~ImageVarianceFilter() {;}

    virtual void initialize(int w, int h) = 0;
    virtual void initialize(const Image& image);

    virtual void add(const Image& image) =0;
    
    virtual Image* createMask(double threshold) =0;

    virtual int getNumImagesAdded() const =0;    
};

class ImageDistanceFilter : public ImageVarianceFilter
{
public:
	ImageDistanceFilter();
    virtual ~ImageDistanceFilter();

    virtual void initialize(int w, int h) ;

    virtual void add(const Image& image);
    
    virtual Image* createMask(double threshold);

    virtual int getNumImagesAdded() const;
protected:
	bool	initialized;       
    int		numImagesAdded;     
    double* diff;           
    double* tmp;            
    Image*	lastImage;       
    int		w;
    int		h;
 
};
 
class ImageMaxDistanceFilter : public ImageVarianceFilter
{
public:
	ImageMaxDistanceFilter();
    virtual ~ImageMaxDistanceFilter();

    virtual void initialize(int w, int h) ;

    virtual void add(const Image& image);
    
    virtual Image* createMask(double threshold);

    virtual int getNumImagesAdded() const; 
protected:
	bool	initialized;       
    int		numImagesAdded;     
    double* maxDiff;        
    Image*	lastImage;       
    int		w;
    int		h;

}; 
 
#endif