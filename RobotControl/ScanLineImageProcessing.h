#ifndef _ScanLineImageProcessing_h_
#define _ScanLineImageProcessing_h_

#include "ImageProcessingType.h"
#include "MultiImageProducer.h"
#include "ColorClasses.h"
#include "RobotsException.h"

class ScanResultList;
class LineScanner;
class LineDetector;
class GoalDetector;
class FieldMapper;
class BallDetector;
class CoordinateMapping;
class ObstacleDetector;
class ChainCoder;

class ScanLineImageProcessing : public ImageProcessingType
{
public:
	ScanLineImageProcessing(  ConfigReader& config) 
      throw(InvalidConfigurationException, HardwareException,
	    BadHardwareException, RobotsException);
    virtual ~ScanLineImageProcessing() throw();

    virtual void process_image() throw ( BadHardwareException);   

    virtual void request_image_raw() throw(RobotsException);
    virtual void request_image_processed() throw(RobotsException);
    virtual bool is_image_available() throw();
    virtual const Image* get_image() throw(RobotsException);
    virtual void free_image() throw(RobotsException); 
	
protected:
	 long producerTime;
    long scannerTime;
    long lineDetectorTime;
    long goalDetectorTime;
    long ballDetectorTime;
    long obstacleDetectorTime;
    long fieldMapperTime;

    long cycles;
    
	MultiImageProducer* imgProd;
	
    ColorClassInfoList* colClass;
    ScanResultList*		scanResults;
    LineScanner*		scanner;
    CoordinateMapping*	image2real;
    CoordinateMapping*	real2image;
    LineDetector*		lineDetector;
    GoalDetector*		goalDetector;
    BallDetector*		ballDetector;
    ChainCoder*			cc;
    ObstacleDetector*	obstacleDetector;
    FieldMapper*		fieldmapper;

    ColorClassInfoList* usbcolClass;
	ScanResultList*		usbScanResults;
	LineScanner*		usbScanner;
	CoordinateMapping*	usbImage2real;
	BallDetector*		usbBallDetector;
    bool				bHaveUsb;


    bool report_computation_time;
    bool report_computation_time_gnuplot;

    bool rawImageRequested;
    bool processedImageRequested;
    Image* requestedImage;
    bool imageCollected;
  
    int centerX, centerY;

    void visualizeCommonInfo();
 
};

#endif
