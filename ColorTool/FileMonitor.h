#ifndef _filemonitor_h_
#define _filemonitor_h_

#include "ImageMonitor.h"
#include "ImageIO.h"
#include "Image.h"
#include <string>
#include <fstream>

class FileMonitor : public ImageMonitor
{
public:
	FileMonitor(ImageIO* imageIO, const std::string& filename, 
		int step=1, bool singleFile=true);
    virtual ~FileMonitor();
    virtual void monitor(const Image& image, 
			 const Time& available, const Time& created);
protected:
	ImageIO* imageIO;
    std::ofstream* logOut;
    std::ofstream* imgOut;
    std::string filename;
    bool singleFile;

    int counter;
    int step;
};

#endif
