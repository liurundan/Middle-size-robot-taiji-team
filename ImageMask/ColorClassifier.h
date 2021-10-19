#ifndef _ColorClassifier_h_
#define _ColorClassifier_h_

#include <string>

#include "Image.h"

using std::string;

class ColorClassifier
{
public: 
    virtual const unsigned char& lookup(const RGBTuple&  rgb)  const =0;
   
    virtual const unsigned char& lookup(const YUVTuple&  yuv)  const =0;
   
    virtual const unsigned char& lookup(const UYVYTuple& uyvy,int pos=0) const =0;
 
    virtual void set(const RGBTuple&  rgb,  unsigned char c) =0;
    
    virtual void set(const YUVTuple&  yuv,  unsigned char c) =0;
    
    virtual void set(const UYVYTuple& uyvy, unsigned char c, int pos=0) =0;
 
    virtual void load(string filename) =0;
  
    virtual void save(string filename) const =0;
 
    virtual ColorClassifier* create() const =0;
   
    virtual ColorClassifier* createFromFile(string filename) const;
};

class DefaultClassifier : public ColorClassifier
{
public:
	virtual const unsigned char& lookup(const RGBTuple&  rgb)  const;
    virtual const unsigned char& lookup(const YUVTuple&  yuv)  const;
    virtual const unsigned char& lookup(const UYVYTuple& uyvy, int pos=0) const;

    virtual void set(const RGBTuple&  rgb,  unsigned char c);
    virtual void set(const YUVTuple&  yuv,  unsigned char c);
    virtual void set(const UYVYTuple& uyvy, unsigned char c, int pos=0);
 
    virtual void load(string filename);
    
    virtual void save(string filename) const;
 
    virtual ColorClassifier* create() const;
 
    static const DefaultClassifier* getInstance();
protected:
 
    DefaultClassifier();               
    static DefaultClassifier* singleton;  
    unsigned char defValue;       
};

class WhiteClassifier : public DefaultClassifier
{
public:
	 WhiteClassifier();

    virtual const unsigned char& lookup(const RGBTuple&  rgb)  const;
    virtual const unsigned char& lookup(const YUVTuple&  yuv)  const;
    virtual const unsigned char& lookup(const UYVYTuple& uyvy, 
					int pos=0) const;
protected: 
	unsigned char white;
    unsigned char black;
};

class BlackClassifier : public WhiteClassifier
{
public:   
	BlackClassifier();

};

#endif