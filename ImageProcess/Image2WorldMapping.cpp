#include "StdAfx.h"
#include "Image2WorldMapping.h"

#include <fstream>
#include <iostream>

using namespace std;

Line CoordinateMapping::map(const Line& line) const
{
	return line; 
}

LineSegment CoordinateMapping::map(const LineSegment& line) const
{
	return LineSegment(map(line.getStart()), map(line.getEnd()));
}


void Image2WorldMapping::save(std::string filename) const
{
	ofstream out (filename.c_str(), ios::binary);
	if (!out) 
	{
	  throw RobotsException(__FILE__ ": Could not write mapping to disk.");
	}
	out << width << " " << height << '\n'; 
 
	out.write(reinterpret_cast<char*>(lut),  
		  sizeof(Vec) * width * height); 
	out.close();    
}

Image2WorldMapping::Image2WorldMapping(int width, int height)
  : width(width), height(height)
{
	lut = new Vec[width*height];
}

Image2WorldMapping::Image2WorldMapping(std::string filename) : lut(0)
{
	load(filename);
}

void Image2WorldMapping::load(std::string filename) 
{
	ifstream in (filename.c_str(), ios::binary);
	if (!in) 
	{
	  throw RobotsException(__FILE__ ": Could not read mapping from disk.");
	}
	if (lut !=0) 
	{
	  delete lut;
	}

	char c;
	in >> width;
	in >> height;
	in.get(c);       // in >> c won't work here!
	if (c != '\n') 
	{
	  throw RobotsException(__FILE__": Distance lookuptable has wrong format.");
	}

	lut = new Vec[width*height];

	in.read(reinterpret_cast<char*>(lut),  // with current implementatin of Vec
		sizeof(Vec) * width * height); // a binary read is possible
	in.close();    
}    

Image2WorldMapping::~Image2WorldMapping()
{
	if (lut !=0) 
	{
	  delete [] lut;
	}
}

Vec Image2WorldMapping::map(int x, int y) const
{
	return lut[x+y*width];
}

Vec Image2WorldMapping::map(const Vec& vec) const
{
	return map(static_cast<int>(vec.x), static_cast<int>(vec.y));
}

void Image2WorldMapping::set(int x, int y, const Vec& vec)
{
	lut[x+y*width] = vec;
}

// World2Image ///////////////////////////////////////////////////////

World2Image::World2Image(string filename)
{
	ifstream file(filename.c_str());
	if (!file) 
	{
	  throw RobotsException(__FILE__": World2Image:"
				 " Could not open distance file.");
	}

	// read image dimensions and center
	int centerX, centerY;
	file >> width >> height;
	file >> centerX >> centerY;
	middle = Vec(centerX, centerY);

	// read angle shift
	file >> shift;

	// read number of markers
	int numMarkers;
	file >> numMarkers;

	// read real world distances of markers (mm)
	for (int i=0; i < numMarkers; i++)
	 {
	  double distance;
	  file >> distance;
	  realDistances.push_back(distance);
	}

	// initialize imageDistances ( 360>>shift list of numMarkers entries each )
	imageDistances = vector<vector<double> >(360>>shift, 
						 vector<double>(numMarkers));
	// initialize angleCorrections
	angleCorrections = vector<double> (360>>shift);

	while (!file.eof()) 
	{
	  int angle;
	  double angleCorrection;
	  file >> angle >> angleCorrection;
	  if (file.eof()) 
	  { // end of file (or error)
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

World2Image::~World2Image()
{
}

void World2Image::setMiddle(const Vec& middle)
{
	this->middle = middle;
}

Vec World2Image::getMiddle() const
{
	return middle;
}

Vec World2Image::map(const Vec& vec) const
{
	Vec to = vec;
	double toLength = vec.length();
	int degrees = static_cast<int>( to.angle().get_deg() ) >> shift; 
  
	for (unsigned int i = 0; i < realDistances.size()-1; i++) 
	{
	  if( toLength < realDistances[i+1] ||  
		  i == realDistances.size()-2) 
	  {   
  
			double steigung =    
		  (imageDistances[degrees][i+1] - imageDistances[degrees][i]) /
		  (realDistances[i+1] - realDistances[i]);

		to = to.normalize();
		to *= imageDistances[degrees][i] + steigung*  
		  (toLength - realDistances[i]);

		break;
	  }
	}
	return to + middle;
}

/*
	Image2WorldMapping* mapping = new Image2WorldMapping(640, 480);

	mapping->set(0,0,Vec(1,1));
	mapping->set(1,0,Vec(2,2));
	mapping->set(2,0,Vec(3,3));
	mapping->set(0,1,Vec(4,4));

	mapping->save("test.map");

	mapping->load("test.map");
    
	ofstream cerr;
	cerr.open("maptest.txt",ios::out );

	cerr << "size: " << mapping->getWidth() 
	   << "x" << mapping->getHeight() << endl;
	cerr << mapping->map(0,0).x << " " << mapping->map(0,0).y << endl;
	cerr << mapping->map(1,0).x << " " << mapping->map(1,0).y << endl;
	cerr << mapping->map(0,1).x << " " << mapping->map(0,1).y << endl;
	cerr << mapping->map(1,1).x << " " << mapping->map(1,1).y << endl;
	cerr.close();
*/