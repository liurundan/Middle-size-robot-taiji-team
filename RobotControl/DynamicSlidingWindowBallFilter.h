#ifndef _DynamicSlidingWindowBallFilter_h
#define _DynamicSlidingWindowBallFilter_h

#include "ConfigReader.h"
#include "DynamicRingBuffer.h"
#include "RingBuffer.h"
#include "VisibleObject.h"
#include "BallLocation.h"
#include "RobotLocation.h"
#include <fstream>
  
class DynamicSlidingWindowBallFilter
{
public:
	DynamicSlidingWindowBallFilter ( ConfigReader& ) throw (std::bad_alloc);
	~DynamicSlidingWindowBallFilter () throw ();

	void update (const Vec, const Time, const Vec) throw ();

	void update (const VisibleObjectList&, const RobotLocation&) throw ();
	void comm_ball (Vec) throw ();
	BallLocation get (Time) const throw ();
 
	void small_camera_ball(BYTE,const RobotLocation&) throw();

private:
	struct PairTimePos 
	{
		Vec pos;             
		Time timestamp;    
	};
 
	DynamicRingBuffer<PairTimePos> pos_list;    
	RingBuffer<PairTimePos>		pos_for_raised;    

	bool first_call;                      
	unsigned int max_buffer_size;        
	unsigned int min_buffer_size;        

	Time ref_time;               
	Vec ref_bpos;                
	Vec ref_bvel;               

	Time latest_motion_model;    

	double latest_error;          
	double max_error;            
	bool ball_raised;             

	double raised_hysterese_lower2;      
	double raised_hysterese_upper2;     
	std::ofstream* plog;         

	bool update_motion_model () throw ();                
	void rescale_ring_buffer (unsigned int) throw ();   
	bool inside_field (Vec) const throw ();             

	Vec comm_ball_pos;            
	Time comm_ball_pos_time;  
	
	BYTE small_camera_ball_dir;
	Vec  small_camera_ball_pos;
	
};



#endif