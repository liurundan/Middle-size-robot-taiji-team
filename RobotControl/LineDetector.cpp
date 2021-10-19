#include "StdAfx.h"
#include "LineDetector.h"

#include "LineScanning.h"
#include "ColorClasses.h"
#include "WorldModel.h"
#include "WhiteBoard.h"
#include "FieldMapper.h"
#include "Image.h"
#include "Painter.h"
#include <cmath>

LineDetector::LineDetector(const CoordinateMapping* mapping, 
						   FieldMapper * fieldmapper,
						   double minLineWidth,
						   double maxLineWidth,
						   bool use2Steps,
						   bool checkForGreen,
						   bool cutOutsideField,
						   bool useMaximalDistance,
						   double maximalDistance,
						   bool useHalfField,
						   bool useFieldMapper) 
						   : mapping(mapping),fieldmapper(fieldmapper), minLineWidth(minLineWidth), maxLineWidth(maxLineWidth),
						   use2Steps(use2Steps), checkForGreen(checkForGreen), 
						   cutOutsideField(cutOutsideField), useHalfField(useHalfField),
						   useMaximalDistance(useMaximalDistance), maximalDistance(maximalDistance),useFieldMapper(useFieldMapper), vis(0)
{
}

void LineDetector::searchLines(const ScanResult* scanResult, Time time, 
							   VisibleObjectList* vol, bool writeWM)
							   throw (RobotsException)
{
    if (scanResult->id != COLOR_LINE) 
	{
		throw RobotsException(__FILE__ 
			": Expected results of color class COLOR_LINE "
			"but received results of another class.");
    }
    
    for ( unsigned int i=0; i < scanResult->transitions.size(); i+=2 ) 
	{
		if (scanResult->transitions.size() <= i+1) // a pair left?
		{ 
			throw RobotsException(__FILE__
				": The transition list is not organized "
				"pair-wise. One end transision is missng.");
		}
		
		const Transition& transStart = scanResult->transitions[i];
		const Transition& transEnd   = scanResult->transitions[i+1];
		
		if ( transStart.type != Transition::START ||
			transEnd.type   != Transition::END) 
		{
			throw RobotsException(__FILE__
				": The transition list is not organized "
				"pair-wise. Each start transtition has to be "
				"followed by exactly one endtransision!");
		}
		
		// plausibility check
		
		if( checkLinePlausibility(transStart, transEnd, time ) ) // add to world model?
		{ 
			Vec middle = ( mapping->map(transStart.toPos) +
				mapping->map(transEnd.fromPos) );
			middle *= 0.5;   
			
			if (vis) 
			{
				Vec imgMiddle = (transStart.toPos + transEnd.fromPos) * .5;
				visualize(imgMiddle);
			}
			
			if (vol) // VisibleObjectList available?
			{      
				vol->objectlist.push_back( VisibleObject(middle, VisibleObject::white_line) );
			}
			// add to world model only, if writeWM set
			if (writeWM) 
			{ 
				WorldModel::get_main_world_model().set_visual_information(
					VisibleObject (middle, VisibleObject::white_line), time  );     
			}
		}
    }
    if(vis) 
		vis = 0;  // visualization finished
}

bool LineDetector::checkLinePlausibility(const Transition& transStart,
										 const Transition& transEnd, const Time& time) const
{
    if (checkForGreen) // check if FIELD before and after line
	{  
		if (use2Steps) 
		{
			if (transStart.twoStep != COLOR_FIELD ||
				transEnd.twoStep != COLOR_FIELD) 
			{
				return false;
			}
		}
		else if (transStart.from != COLOR_FIELD ||
			transEnd.to != COLOR_FIELD) 
		{ 
			return false;
		}
    }
    else  // check for same color class
	{ 
		if (use2Steps) 
		{
			if (transStart.twoStep != transEnd.twoStep) 
			{
				return false;
			}
		}
		else if (transStart.from != transEnd.to) 
		{ 
			// same color before and after line
			return false;
		}
    }
    if (useMaximalDistance) 
	{
		Frame2d robot2world = WhiteBoard::getTheWhiteBoard()->getRel2AbsFrame(time);
		Vec globalCoord = robot2world * mapping->map(transStart.toPos);
		
		if (globalCoord.length()  > maximalDistance) 
		{
			return false;
		}
    }
	
    if (cutOutsideField) 
	{
		const FieldGeometry& fg = MWM.get_field_geometry ();
		Frame2d robot2world = WhiteBoard::getTheWhiteBoard()->getRel2AbsFrame(time);
		
		Vec globalCoord = robot2world * mapping->map(transStart.toPos); // map gain a relative(robot) cordinate
		
		double maxX = 0.5 * fg.field_length+ 300;
		double maxY = 0.5 * fg.field_width + 300;
		
		if (fabs(globalCoord.x) > maxX || fabs(globalCoord.y) > maxY) 
		{
			return false;
		}
    }      
	
    if (useHalfField) 
	{  
		Frame2d robot2world = WhiteBoard::getTheWhiteBoard()->getRel2AbsFrame(time);
		Vec globalCoord = robot2world * mapping->map(transStart.toPos);
		
		globalCoord *= MWM.get_own_half();
		
		if(globalCoord.x > 300) 
		{
			return false;
		}
    }
    
    if (useFieldMapper) 
	{  
		if (! fieldmapper->insideField((int)transStart.fromPos.x,
			(int)transStart.fromPos.y))
			return false;
	}
	
    double width = ( mapping->map(transStart.toPos) - 
		mapping->map(transEnd.fromPos) ).length();
    
	// check distance between the two transitions
    if (width < minLineWidth ||	width > maxLineWidth) 
	{
		return false;
    }
	
    return true;                // passed all tests
}

void LineDetector::visualize(const Vec& line) const
{
	Painter p(*vis);
	p.setColor(60, 120, 240);
	p.markRect((int)line.x, (int)line.y, 2);
}