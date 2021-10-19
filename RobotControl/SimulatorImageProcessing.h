#ifndef _SimulatorImageProcessing_h_
#define _SimulatorImageProcessing_h_

#include "ImageProcessingType.h"
#include "geometry.h"
#include "SimClient.h"
#include "ConfigReader.h"
#include <deque>


class SimulatorImageProcessing : public ImageProcessingType 
{
private:
	SimClient* the_sim_client;         
	double noise_level;                
	double mis_probability;            
	double goal_mis_probability;       
	double ball_mis_probability;       
	double line_vision_radius;              
	std::deque<LineSegment> lines;     
	std::deque<MyArc> arcs;              
public:
	SimulatorImageProcessing (ConfigReader&) throw (std::bad_alloc, InvalidConfigurationException);
	~SimulatorImageProcessing () throw ();
	void process_image () throw ();
};



#endif
