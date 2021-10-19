#include "StdAfx.h"
#include "FileMonitor.h"

#include <sstream>
#include <fstream>

using namespace std;

FileMonitor::FileMonitor(ImageIO* imageIO, const std::string& filename,
		   int step, bool singleFile)
: ImageMonitor(), imageIO(imageIO), logOut(0), imgOut(0), 
  filename(filename), singleFile(singleFile), counter(0), step(step)
{
	logOut = new ofstream((filename+".log").c_str());

	if( singleFile ) 
	{
	  string ending = imageIO->getDefEnding();
	  string imgFilename = filename+ending+"_stream";
	  imgOut = new ofstream(imgFilename.c_str());

	  (*logOut) << "stream " << imageIO->getTypeId() 
		<< " " << imgFilename << '\n';
	} 
	else 
	{
	  (*logOut) << "single " << imageIO->getTypeId() << " " << '\n';
	}    
}

FileMonitor::~FileMonitor()
{
	if (logOut)
	{
	  logOut->close();
	  delete logOut;
	}
	if (imgOut) 
	{
	  imgOut->close();
	  delete imgOut;
	}
	if (imageIO) 
	{
	  delete imageIO;
	}
}

void FileMonitor::monitor(const Image& image, 
			const Time& available, const Time& created)
{
	if( counter % step != 0) 
	{
	  counter++;   
	  return;     
	}

	if(! *logOut) 
	{
	  throw RobotsException("Could not open image monitor log file.");
	}
	if( singleFile )
	{
	  if( ! *imgOut ) 
	  {
		throw RobotsException("Could not open image monitor output file.");
	  }
	  (*imgOut) << image.getTimestamp() << '\n';
	  imageIO->write(image, *imgOut);
	  (*logOut) << counter++ << " " << image.getTimestamp() << '\n';
	}
	else
	{
	  stringstream base;
	  base << filename << image.getTimestamp() << imageIO->getDefEnding();
	  imageIO->write(image, base.str());
	  (*logOut) << counter++ << " " << base.str() << " " 
		<< image.getTimestamp() << '\n';
	}
}