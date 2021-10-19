#ifndef _ImageProcessingDistanceCalibration_h
#define _ImageProcessingDistanceCalibration_h

#include "ImageProcessingType.h"
#include "ImageProducer.h"
#include "ColorClasses.h"
#include "LineScanning.h"
#include "BallDetector.h"
#include "DistanceCalibration.h"
#include <string>

class ImageProcessingDistanceCalibration : public ImageProcessingType
{
public:
	ImageProcessingDistanceCalibration( ConfigReader& config);
    virtual ~ImageProcessingDistanceCalibration() throw();

    void process_image() throw (BadHardwareException);    

 private:
	Vec image_center;
    std::string markerFile;
    ImageProducer* imgProd;
    ColorClassInfoList* colClass;
    ScanResultList* scanResults;
    LineScanner* scanner;
    CoordinateMapping* image2real;
    CoordinateMapping* real2image;
    BallDetector* ballDetector;
    DistanceCalibration* distcal;

};

#endif


