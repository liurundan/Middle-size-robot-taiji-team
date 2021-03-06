#include "StdAfx.h"
#include "MorphologicOperators.h"

Dilatation::Dilatation(int size): ImageOperator(), size(size)
{
	if (size < 3 || size % 2 == 0) {
	  throw RobotsException(__FILE__ ": Invalid size argument.");
	}
}

Dilatation::~Dilatation()
{}

Image* Dilatation::operator() (const Image& image) const
{
	Image* newImage = image.clone();

	RGBTuple rgb;
	RGBTuple white = { 255, 255, 255 };

	for (int x = 0; x < image.getWidth(); x++) 
	{
	  for (int y = 0; y < image.getHeight(); y++) 
	  {
		newImage->getPixelRGB(x,y, &rgb);
		
		if (rgb.r > 127) 
		{          
		  continue;                 
		}

		bool set = false;

		for (int mx = x - size/2; !set && mx <= x + size/2; mx++) 
		{
		  for (int my = y - size/2; !set && my <= y + size/2; my++) 
		  {
			if (mx < 0 || my < 0 || 
				mx >= image.getWidth() || my >= image.getHeight()) 
			{
			  continue;
			}
			image.getPixelRGB(mx,my, &rgb);
			if (rgb.r > 127)
			{     
			  newImage->setPixelRGB(x,y, white);
			  set = true;          
			}
		  }
		}
	  }
	}
	return newImage;
}

// erosion //////////////////////////////////////////////////////////////

Erosion::Erosion(int size)
: ImageOperator(), size(size)
{
	if (size < 3 || size % 2 == 0) 
	{
	  throw RobotsException(__FILE__ ": Invalid size argument.");
	}
}

Erosion::~Erosion()
{}

Image* Erosion::operator() (const Image& image) const
{
	Image* newImage = image.clone();

	RGBTuple rgb;
	RGBTuple black = { 0, 0, 0 };

	for (int x = 0; x < image.getWidth(); x++) 
	{
	  for (int y = 0; y < image.getHeight(); y++) 
	  {
		newImage->getPixelRGB(x,y, &rgb);
		if (rgb.r <= 127) 
		{         
		  continue;                 
		}

		bool set = false;

		for (int mx = x - size/2; (!set) && mx <= x + size/2; mx++) 
		{
		  for (int my = y - size/2; (!set) && my <= y + size/2; my++) 
		  {
			if (mx < 0 || my < 0 || 
			mx >= image.getWidth() || my >= image.getHeight()) 
			{
			  continue;
			}
			image.getPixelRGB(mx,my, &rgb);
			if (rgb.r <= 127)
			{    
			  newImage->setPixelRGB(x,y, black);
			  set = true;           
			}
		  }
		}
	  }
	}
	return newImage;
}
