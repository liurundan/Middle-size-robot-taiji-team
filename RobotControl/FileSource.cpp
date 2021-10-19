#include "StdAfx.h"
#include "FileSource.h"
#include <fstream>
//#include "RobotControl.h"
//#include "RobotControlDlg.h"

using namespace std;

FileSource::FileSource(ImageIO* imageIO, const string& filename)
    : ImageSource(), imageIO(imageIO), pos(0), width(0), height(0)
{
    ifstream in(filename.c_str());
    if (!in) 
	{
      throw BadHardwareException(__FILE__": Could not open file.");
    }
    string type;
    in >> type;
    if (! (type == "single")) 
	{
      throw BadHardwareException(__FILE__": This type is not supported.");
    }

    string ioType;
    in >> ioType;
    if (! (ioType == imageIO->getTypeId())) 
	{
      throw BadHardwareException(__FILE__": The file type does not match "
				 "the imageIO you have provided");
    }

    int n;
    string imgFile;
    long timestamp;
    Time time;
    while(in) 
	{
      in >> n >> imgFile >> timestamp;
      time.set_msec(timestamp);
      images.push_back(FileInfo(n, imgFile, time));
    }
    in.close();

    if (images.size() == 0) 
	{
      throw BadHardwareException(__FILE__": File does not conatain any images.");
    }

    image  = imageIO->read(0, images[0].filename);  
    width  = image->getWidth();
    height = image->getHeight();
}

FileSource::~FileSource()
{
	if (imageIO) 
	{
	  delete imageIO;
	}
	delete image;
}

ImageBuffer FileSource::getImage() throw (HardwareException)
{
	image				= getNextImage(image);
	ImageBuffer buffer	= image->getImageBuffer();
	return buffer;
}

Image* FileSource::getNextImage(Image* image)
{
 /*	static int lastpos = 0;
 	CRobotControlDlg * pMainDlg = (CRobotControlDlg*)(AfxGetApp()->m_pMainWnd);
    bool bstop = pMainDlg->bStopReadImage;  
    if( pos < images.size() - 2 )
	else
	  pos = 0;
	if(bstop)
 		pos = lastpos;
 	lastpos = pos;*/
	
    if( pos < images.size() - 2 )
	  pos = (pos+1) % images.size();
	else
	  pos = 0;  // edit by lrd

	//pos = (pos+1) % images.size();
	image = imageIO->read(image, images[pos].filename);
	image->setTimestamp(images[pos].timestamp);
	return image;
}

Image* FileSource::getPrevImage(Image* image)
{
	pos = (pos-1+images.size()) % images.size();
	image = imageIO->read(image, images[pos].filename);
	image->setTimestamp(images[pos].timestamp);
	return image;
}

string FileSource::getFilename() const
{
	return images[pos].filename;
}

Time FileSource::getTimestamp() const
{
	return images[pos].timestamp;
}


Image* FileSource::getImageNumber(int n, Image* image)
{
	 while (pos < (int)images.size()-1 && n > images[pos].n)
	 {
	    pos++;
	 } 

	 while (pos > 0 && n < images[pos].n) 
	 {
	    pos--;
	 } 
	 
	 image = imageIO->read(image, images[pos].filename);
	 image->setTimestamp(images[pos].timestamp);
	 return image;
}
