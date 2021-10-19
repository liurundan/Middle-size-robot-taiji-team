#include "StdAfx.h"
#include "KoordTransformDortmund.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

Image2WorldDortmund::Image2WorldDortmund(double w, double h, 
										 double offsetX, double offsetY, double real_width,
										 	double al, double ch, double dis_ct ):width(w),height(h),
		axis_length(al),camera_height(ch),dis_CameraToRobot(dis_ct),ball_radius(105)
{
	axis_length_offsetX = offsetX;
	axis_length_offsetY = offsetY;
    
	focal_dis = ( axis_length * w ) / real_width;
	theta = atan(offsetY/offsetX);
    beta  = atan(offsetY/(camera_height*sin(theta)));
}

Image2WorldDortmund::Image2WorldDortmund(string filename)
{
	ifstream file(filename.c_str());
	if (!file) 
	{
	  throw RobotsException(__FILE__": Could not open distance file.");
	}

 	int centerX, centerY;
	file >> width >> height;
	file >> centerX >> centerY;
	middle = Vec(centerX, centerY);

 	file >> shift;

 	int numMarkers;
	file >> numMarkers;
    
 	for (int i=0; i < numMarkers; i++) 
	{
	  double distance;
	  file >> distance;
	  realDistances.push_back(distance);
	}
     
 	imageDistances = vector<vector<double> >(360>>shift, 
						 vector<double>(numMarkers));

 	angleCorrections = vector<double> (360>>shift);

	while (!file.eof()) 
	{
	  int angle;
	  double angleCorrection;
	  file >> angle >> angleCorrection;
	  if (file.eof()) 
	  {  
		break;
	  }

	  angleCorrections[angle] = angleCorrection;

	  for (int i=0; i < numMarkers; i++) 
	  {
		double distance;
		file >> distance;
		if (file.eof()) 
		{
		  throw RobotsException(__FILE__
					 ": File ended although another distance "
					 "entry was expected.");
		}
	
		
		imageDistances[angle][i] = distance;
	  }
	}
	file.close();
}

Image2WorldDortmund::~Image2WorldDortmund()
{
}

Vec Image2WorldDortmund::map(const Vec& vec) const
{
	Vec to = vec-middle;
	double toLength = to.length();
	int degrees = static_cast<int>( to.angle().get_deg() ) >> shift;
 
	for (unsigned int i = 0; i < imageDistances[degrees].size()-1; i++) 
	{
	  if (toLength < imageDistances[degrees][i+1] ||  
		  i == imageDistances[degrees].size()-2) 
	  {    

		    double steigung =    
		      (realDistances[i+1] - realDistances[i]) /
		      (imageDistances[degrees][i+1] - imageDistances[degrees][i]);

			to = to.normalize();
			to *= realDistances[i] + steigung * //y=x1 + steigung * (x-x1)
			  (toLength - imageDistances[degrees][i]);

 			to = to.rotate(Angle::deg_angle(angleCorrections[degrees]));

		break;
	  }
	}
	return to;
}

Image2WorldMapping* Image2WorldDortmund::convert() const
{
	Image2WorldMapping* mapping = new Image2WorldMapping(width, height);

	for (int x = 0; x < width; x++) 
	{
	  for (int y = 0; y < height; y++) 
	  {
		mapping->set(x,y, map(Vec(x,y)));
	  }
	}
	return mapping;
}  

void Image2WorldDortmund::setMiddle(const Vec& middle)
{
	this->middle = middle;
}

Vec Image2WorldDortmund::getMiddle() const
{
	return middle;
}

Vec Image2WorldDortmund::mapUsb(const Vec& vec) const
{
  	double alfa_x = atan( ( height * 0.5 - vec.y ) / focal_dis  ); 
	double alfa_y = atan( ( width * 0.5 - vec.x ) / focal_dis );  
	
	double ball_x = ( camera_height-ball_radius ) * tan(beta+alfa_x);
	double ball_y = ( camera_height-ball_radius ) * tan(alfa_y) / cos(beta+alfa_x);
	ball_y = ( ball_y - ball_x * tan(theta) ) * cos(theta);
	ball_x = dis_CameraToRobot + (ball_y * sin(theta) + ball_x ) / cos(theta);
	
	Vec to(ball_x,ball_y);
 	return to;
}

Image2WorldMapping* Image2WorldDortmund::convertUsb() const
{
	Image2WorldMapping* mapping = new Image2WorldMapping(width, height);

	for (int x = 0; x < width; x++) 
	{
	  for (int y = 0; y < height; y++) 
	  {
		mapping->set(x,y, mapUsb(Vec(x,y)));
	  }
	}
	return mapping;
}  

 
