#ifndef _JPEGIO_h_
#define _JPEGIO_h_

#include "ImageIO.h"

 
  class LibJPEGData; 

  class JPEGIO : public ImageIO 
  {
  public:
    JPEGIO(int quality=50);
    virtual ~JPEGIO();

    virtual void write(const Image& image, ostream& out) const
      throw(RobotsException);
    
    virtual Image* read(Image* image, istream& in) const
      throw(RobotsException);

    virtual std::string getDefEnding() const { return ".jpg"; }
    virtual std::string getTypeId() const { return "JPEG"; }
  protected:
    LibJPEGData *lj;
  };

 

#endif