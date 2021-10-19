#include "StdAfx.h"
#include "YUVLookupTable.h"

#include <string>
#include <fstream>

#include "Image.h"
#include "RobotsException.h"

using namespace std;

YUVLookupTable::YUVLookupTable(int shiftY, int shiftU, int shiftV) 
    : ColorClassifier(), shiftY(shiftY), shiftU(shiftU), shiftV(shiftV),
      sizeY(256 >> shiftY), sizeU(256 >> shiftU), sizeV(256 >> shiftV),
      size(sizeY*sizeU*sizeV), lut(new unsigned char[size])
{}

YUVLookupTable::~YUVLookupTable()
{
    delete [] lut;
}
  
const unsigned char& YUVLookupTable::lookup(const RGBTuple& rgb) const
{
    PixelConversion::convert(rgb, const_cast<YUVTuple*>(&yuvTmp));
    return lookup(yuvTmp);
}

const unsigned char& YUVLookupTable::lookup(const UYVYTuple& uyvy, int pos) const
{
    PixelConversion::convert(uyvy, const_cast<YUVTuple*>(&yuvTmp), pos);
    return lookup(yuvTmp);
}

const unsigned char& YUVLookupTable::lookup(const YUVTuple& yuv) const
{
    return lut[((yuv.y >> shiftY) * sizeU + 
		(yuv.u >> shiftU)) * sizeV +
	       (yuv.v >> shiftV)];
}

void YUVLookupTable::set(const RGBTuple&  rgb,  unsigned char c)
{
    PixelConversion::convert(rgb, &yuvTmp);
    set(yuvTmp, c);
}

void YUVLookupTable::set(const UYVYTuple& uyvy, unsigned char c, int pos)
{
    PixelConversion::convert(uyvy, &yuvTmp, pos);
    set(yuvTmp, c);
}
  
void YUVLookupTable::set(const YUVTuple&  yuv,  unsigned char c)
{
    lut[((yuv.y >> shiftY) * sizeU + 
	 (yuv.u >> shiftU)) * sizeV +
	(yuv.v >> shiftV)] = c;    
}  

static void skip2(istream& in);
   
void YUVLookupTable::load(string filename)
{
    int oldSize = size;// remember the size, to detect wheter or not it changes

    ifstream lutFile (filename.c_str(), ios::in | ios::binary );    
    if (! lutFile.is_open() ) 
	{
      throw RobotsException((string(__FILE__)+string(": Could not open file ")+filename).c_str());
	} 
	
    skip2(lutFile);
    lutFile >> shiftY;
	skip2(lutFile);
    lutFile >> shiftU;
	skip2(lutFile);
    lutFile >> shiftV; // this one reads the '\n' that follows the shiftV value
    lutFile.ignore(1);
	
    sizeY = 256 >> shiftY;  // 128 64 64
    sizeU = 256 >> shiftU;
    sizeV = 256 >> shiftV;

    size = sizeY * sizeU * sizeV;

    if (oldSize != size) 
	{              // size has been changed
      delete [] lut;                    // delete old and
      lut = new unsigned char[size];    // create new lookup table 
    }
     
    if (! lutFile.read(reinterpret_cast<char*>(lut), size))
	{
      throw RobotsException((string(__FILE__)+string(": Could not read lookup table from file ")+filename).c_str());
    }
    lutFile.close();
}
   
void YUVLookupTable::save(string filename) const
{
 	ofstream lutFile( filename.c_str(), ios::out | ios::binary );    
    if (! lutFile.is_open() ) 
	{
      throw RobotsException((string(__FILE__)+string(": Could not open file ")+filename).c_str());
    }

    lutFile << shiftY << ' ' << shiftU << ' ' << shiftV <<  '\n';
 
    if (! lutFile.write(reinterpret_cast<char*>(lut), size)) 
	{
      throw RobotsException((string(__FILE__)+string(": Could not write lookup table to file ")+filename).c_str());
    }
	 
    lutFile.close();
}

ColorClassifier* YUVLookupTable::create() const
{
    return new YUVLookupTable(shiftY, shiftU, shiftV);
}

void  YUVLookupTable::fillFromClassifier(const ColorClassifier* cc)
{
    int classCounter[256];    // 256 classes (unsigned char)
    YUVTuple yuv;
		
    for (register int yQ = 0; yQ < sizeY; yQ++) 
	{
      for (register int uQ = 0; uQ < sizeU; uQ++) 
	  {
		for (register int vQ = 0; vQ < sizeV; vQ++) 
		{ // for every entry in the lut
  			  // check every yuv-tuple, that falls into the present quantizized
			  // entry of the lut

			  memset(classCounter, 0, sizeof(int) * 256);

			  for (register int y = yQ << shiftY; y < (yQ+1) << shiftY; y++) 
			  {
				 
				for (register int u = uQ << shiftU; u < (uQ+1) << shiftU; u++) 
				{
				  for (register int v = vQ << shiftV; v < (vQ+1) << shiftV; v++) 
				  {
					yuv.y = y; yuv.u = u; yuv.v = v;
					classCounter[cc->lookup(yuv)]++;
				  }
				}
			  }
			  int maxID = 0;   // look for the most frequent class 
			  for (int i=1; i < 256; i++) 
			  {
				if (classCounter[maxID] < classCounter[i])
				{ // more entries?
				  maxID = i; 
				}
			  }
			  set(yuv, maxID); // since yuv is still _in_ the lut entry, this
								   // should do the necessary update (I hope...)
		}
      }
    }
} 

static void skip2(istream& in)
{
    char c;
    do 
	{
      if (!in.get(c)) return;
    } 
	while(isspace(c) || c=='\n' || c=='\r');
    if (c=='#') 
	{                  // start of a comment
      do 
	  {                         // look for the end of the line
		if (!in.get(c))
			return;
	  } 
	  while(c!='\n');
      skip2(in);                    // skip again at start of next line.
    }
    else                           // c was neither comment(#) nor space
      in.putback(c);               // so better put it back,
}