#include "StdAfx.h"
#include "Image.h"

#include "ImageSource.h"
#include "ColorClassifier.h"

Image::Image(const ColorClassifier* classifier)
    : classifier(classifier), timestamp(Time())
{
	if (classifier == 0) 
	{
	  this->classifier = DefaultClassifier::getInstance();
	}
}

int Image::getWidth() const { return buffer.width; }
int Image::getHeight() const { return buffer.height; }

int Image::getNativeFormat() const { return buffer.format; }

const ColorClassifier* Image::getClassifier() const
{
	return classifier;
}

void  Image::setClassifier(const ColorClassifier* classifier)
{
	if (classifier != 0 )
	{
	  this->classifier = classifier;
	}
	else 
	{
	  this->classifier = DefaultClassifier::getInstance();
	}
}

void Image::setBlackBorder()
{
	RGBTuple black = { 0, 0, 0 };
	setBorder(black);
}

void Image::setWhiteBorder()
{
	RGBTuple white = { 255, 255, 255 };
	setBorder(white);
}

void Image::setBorder(const RGBTuple& rgb) 
{
	int w = getWidth()-1;
	int h = getHeight()-1;

	for (int x=w; x >= 0; x--) 
	{
	  setPixelRGB(x, 0, rgb);
	  setPixelRGB(x, h, rgb);
	}
	for (int y=h; y >= 0; y--) 
	{
	  setPixelRGB(0, y, rgb);
	  setPixelRGB(w, y, rgb);
	}
}      