#ifndef _FILESOURCE_H_
#define _FILESOURCE_H_

#include "ImageSource.h"
#include "ImageIO.h"
#include <string>
#include <vector>
#include "Time.h"

class FileSource : public ImageSource
{
public:
	FileSource(ImageIO* imageIO, const std::string& filename);
    virtual ~FileSource();
    
    virtual ImageBuffer getImage() throw (HardwareException);
    
    virtual Image* getNextImage(Image* image=0); 
    virtual Image* getPrevImage(Image* image=0);

    virtual Image* getImageNumber(int n, Image* image=0); 
    virtual std::string getFilename() const;
    virtual Time getTimestamp() const;
    
    virtual int getWidth() const { return width; }
    virtual int getHeight() const { return height; }

protected:
	struct FileInfo 
	{
      int n;
      std::string filename;
      Time timestamp;
      FileInfo(int n, const std::string& filename, const Time& timestamp)
	: n(n), filename(filename), timestamp(timestamp) {}
    };

    ImageIO*				imageIO;
    int						pos;
    std::vector<FileInfo>	images;
    
    int width;
    int height;
    
    Image* image;   
};
#endif