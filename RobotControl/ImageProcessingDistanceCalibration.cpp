#include "StdAfx.h"
#include "ImageProcessingDistanceCalibration.h"
#include <iostream>

#include "ScanLines.h"
#include "Image.h"
#include "RobotMask.h"
#include "KoordTransformDortmund.h"
#include "Journal.h"
#include "WorldModel.h"

using namespace std;

ImageProcessingDistanceCalibration::ImageProcessingDistanceCalibration( ConfigReader& config) 
{
  int innerRadius, outerRadius;
  int nScanLines;
  string robotMaskFilename="";
  
  if (config.get("ScanLine::robot_mask_file", robotMaskFilename) < 0) 
  {
    throw InvalidConfigurationException("ScanLine::robot_mask_filename");
  }   
  if (config.get("ScanLine::image_center_x", image_center.x) <= 0) 
  {
    throw InvalidConfigurationException("ScanLine::image_center_x");
  }   
  if (config.get("ScanLine::image_center_y", image_center.y) <= 0) 
  {
    throw InvalidConfigurationException("ScanLine::image_center_y");
  }   
  if (config.get("ScanLine::inner_scan_radius", innerRadius) <= 0) 
  {
    throw InvalidConfigurationException("ScanLine::inner_scan_radius");
  }   
  if (config.get("ScanLine::outer_scan_radius", outerRadius) <= 0) 
  {
    throw InvalidConfigurationException("ScanLine::outer_scan_radius");
  }   
  if (config.get("ScanLine::number_of_scanlines", nScanLines) <= 0) 
  {
    throw InvalidConfigurationException("ScanLine::number_of_scanlines");
  }   
  if (config.get("ScanLine::image2world_marker_file", markerFile) <=0) 
  {
    throw InvalidConfigurationException("ScanLine::image2world_marker_file");
  }

  imgProd = new  ImageProducer(config);
  colClass = new ColorClassInfoList(config);
  colClass->classList[COLOR_BLUE]->findTransitions=true;
  colClass->classList[COLOR_BALL]->findTransitions=true;
  Image* image = imgProd->nextImage();
  
  if (robotMaskFilename != "") 
  {
    RobotMask::load(robotMaskFilename);
  }
  else 
  {
    JWARNING("No robot mask specified!");
  }
  
  ScanLines* lines = new ScanLines(image_center,
				   innerRadius,
				   outerRadius,
				   image->getWidth(), image->getHeight(),
				   nScanLines);
  scanner = new LineScanner(lines, colClass);
  scanResults = new ScanResultList(colClass->classList.size());
  
  Image2WorldDortmund dortmund(markerFile);
  if (static_cast<int>(image_center.x) != dortmund.getMiddle().x ||
      static_cast<int>(image_center.y) != dortmund.getMiddle().y) 
  {
    JWARNING("Image center specified in config files and image center "
	     "specified in distance marker file do not match. " 
	     "It is likely that the mirror has moved slightly since "
	     "the last calibration procedure.\n"
	     "I will use the image center specified "
	     "in the configuration files for both, coordinate "
	     "transformation and distribution of scan lines. Please notice "
	     "that the precision of self localization and localization "
	     "of objects may suffer.");
  }					    
  dortmund.setMiddle(image_center); // use the center from config file
  image2real = dortmund.convert();
  World2Image* w2i = new World2Image(markerFile);
  w2i->setMiddle(image_center);
  real2image = w2i;
  
  ballDetector = new BallDetector(image2real, static_cast<int>(image_center.x), static_cast<int>(image_center.y));
  distcal = new DistanceCalibration (image_center);

  delete image;
}

void ImageProcessingDistanceCalibration::process_image() throw (BadHardwareException) 
{
  Image* image = imgProd->nextImage();
  
  scanResults->clear();     // remove results of the last frame
  scanner->scan(*image, scanResults);

  VisibleObjectList ball;
  ballDetector->searchBall(*image, scanResults->results[COLOR_BALL],
			   image->getTimestamp(), &ball);

  delete image;

  if (MWM.get_game_state().in_game && MWM.get_game_state().refstate==freePlay && ball.objectlist.size()>0) 
  {  
    Angle ballangle = (real2image->map(ball.objectlist[0].pos)-image_center).angle();
	double degball = ballangle.get_deg_180();
    distcal->search_markers (*scanResults, ballangle);
  }

}

ImageProcessingDistanceCalibration::~ImageProcessingDistanceCalibration() throw () 
{
  distcal->writeMarkerFile(markerFile);

  delete distcal;
  delete imgProd;
  delete colClass;
  delete scanResults;
  delete scanner;
  delete image2real;
  delete real2image;
  delete ballDetector;
}


