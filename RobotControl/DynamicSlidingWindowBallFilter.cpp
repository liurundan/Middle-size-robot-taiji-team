#include "StdAfx.h"
#include "DynamicSlidingWindowBallFilter.h"
#include "WorldModel.h"
#include <cmath>
#include "WhiteBoard.h"

using namespace std;

#define DEBUG_BALLFILTER 0    
#define LOG_BALLFILTER 0    
#define USE_SMALL_CAMERA 1

DynamicSlidingWindowBallFilter::DynamicSlidingWindowBallFilter( ConfigReader& reader) throw (std::bad_alloc) : 
pos_list (30), pos_for_raised (6), plog(NULL)
{
	rescale_ring_buffer(1);
	
	int vals[2];
	if (reader.get ("BallFilter::history_length_0", vals[0]) > 0 &&
		reader.get ("BallFilter::history_length_1", vals[1]) > 0 ) 
	{
		max_buffer_size=vals[1];
		min_buffer_size=vals[0];
	} 
	else 
	{
		max_buffer_size=10;
		min_buffer_size=3;
	}

 	if (min_buffer_size<2)
		min_buffer_size=2;
	if (max_buffer_size<min_buffer_size)
		max_buffer_size=min_buffer_size;
	if (!reader.get ("BallFilter::max_error", max_error))
		max_error=500;
	
	if ( reader.get ("BallFilter::raised_threshold_0", vals[0])> 0
		 && reader.get ("BallFilter::raised_threshold_1", vals[1])> 0 ) 
	{
		raised_hysterese_lower2 = vals[0]*vals[0];
		raised_hysterese_upper2 = vals[1]*vals[1];
	} 
	else 
	{
		raised_hysterese_lower2 = 49;
		raised_hysterese_upper2 = 100;
	}
  	
	first_call=true;
	ref_bpos=Vec::zero_vector;
	ref_bvel=Vec::zero_vector;
	ball_raised=false;

	small_camera_ball_dir = 0;
	
#if LOG_BALLFILTER
	string s;
	reader.get ("write_world_model_info", s);
	plog = new ofstream ((s+string(".ppos")).c_str());
	if (!(*plog)) 
	{
		delete plog;
		plog=NULL;
	}
#endif
}

DynamicSlidingWindowBallFilter::~DynamicSlidingWindowBallFilter () throw () 
{
	if (plog) 
	{
		(*plog) << flush;
		delete plog;
	}
}

void DynamicSlidingWindowBallFilter::update (const VisibleObjectList& vis, const RobotLocation& cr) throw () 
{
	if (vis.objectlist.size()>0) 
	{
		Vec p = cr.pos+vis.objectlist[0].pos.rotate(cr.heading);
		
		if( vis.objectlist.size() > 1 )      // get image process results from usb
		   p = cr.pos+vis.objectlist[1].pos.rotate(cr.heading);
		update (p, vis.timestamp, cr.pos);
	}
}

void DynamicSlidingWindowBallFilter::update (const Vec p, const Time t, const Vec r) throw () 
{
#if DEBUG_BALLFILTER
	LOUT << "Ball gesehen an Position " << p << '\n';
#endif
	if  (!inside_field (p)) 
	{
#if DEBUG_BALLFILTER
		LOUT << "Ballfilter: Ball auserhalb gesehen\n";
#endif
		ball_raised=true;
		return;
	}
	
	PairTimePos newp;
	newp.pos = p;
	newp.timestamp = t;
	
	pos_for_raised.get() = newp;
	pos_for_raised.step();
	
	if (t.diff_msec(ref_time)>1000) 
	{  
		rescale_ring_buffer (1);
		first_call=true;
	}
	
	if (first_call) 
	{
		for (unsigned int i=0; i<pos_list.size(); i++)
			pos_list.set (newp, i);
		first_call = false;
	} 
	else 
	{
		if (pos_list.size()<max_buffer_size) //pos_list 用于保存上一次球的位置
		{
			try
			{
				pos_list.insert (newp);
			}
			catch(std::bad_alloc&)
			{
				pos_list.set (newp);
			}
		}
		else
			pos_list.set (newp);
		pos_list.step();  
	}
	
	Vec p_predict = ref_bpos+t.diff_msec(ref_time)*ref_bvel;
	double error = (p_predict-p).length();
	double max_allowed_error = max_error * ( 0.5 + sqrt( (r-p_predict).length()+100 ) / 100 );    
	
#if DEBUG_BALLFILTER
	LOUT << "BallFilter error: " << error << ' ' << latest_error << ' ' << max_allowed_error << '\n';
#endif
	
	if ((error>max_allowed_error && latest_error>max_allowed_error) || (error>2500)) 
	{
		rescale_ring_buffer (min_buffer_size);
		latest_error = 0;  
	} 
	else
		latest_error = error;
	
	Time old_time = ref_time;
	Vec  old_bpos = ref_bpos;
	ref_time = t;
	
	bool successfull_update = update_motion_model ();     
	if (successfull_update && (r-ref_bpos).squared_length()<1e6)
		ref_bpos = p;  
	
#if DEBUG_BALLFILTER
	LOUT << "Ballfilter LS |v|: " << ref_bvel.length() << '\n';
#endif
	
	if (successfull_update) 
	{      
		if (ref_bvel.squared_length()<raised_hysterese_upper2) 
		{    
			if (pos_list.size()>=min_buffer_size) 
			{
				if (!ball_raised)
					latest_motion_model = ref_time;    
				else if (ref_bvel.squared_length()<raised_hysterese_lower2) 
				{  
					latest_motion_model = ref_time;
					ball_raised=false;
				} 
				else 
				{
					rescale_ring_buffer (min_buffer_size);
					ref_bvel=Vec::zero_vector;
				}
			} 
			else 
			{
				ref_bvel=Vec::zero_vector;   
			}
		}
		else 
		{
			rescale_ring_buffer(1);
			first_call  = true;
			ball_raised = true;   
			ref_bvel = Vec::zero_vector;
			ref_bpos = old_bpos;
			ref_time = old_time;
#if DEBUG_BALLFILTER
			LOUT << "BallFilter Plausibilitaetscheck: massive Probleme v>10m/s\n";
#endif
		}
	} 
	else
	{
		ref_bpos = p;
		ref_bvel = Vec::zero_vector;
	}
	
#if LOG_BALLFILTER
	LOUT << "BallFilter length " << pos_list.size() << '\n';
#endif
#if LOG_BALLFILTER
	Time now;
	unsigned int nn = pos_list.size();
	for (unsigned int i=0; i<nn; i++) 
	{
		(*plog) << now << '\t' << pos_list.get(i).pos.x << '\t' << pos_list.get(i).pos.y 
			<< '\t' << 0 << '\t' << i+1 << '\n';
	}
#endif
}

BallLocation DynamicSlidingWindowBallFilter::get (const Time t) const throw () 
{
	BallLocation dest;
	double td = static_cast<double>(t.diff_msec (ref_time));
	dest.pos = ref_bpos + td * ref_bvel;

	dest.velocity = ref_bvel;
	
	double n = static_cast<double>(pos_list.size());
	double f = ( n > 9.5 ? 1.0 : 1.0 - 0.0046875*(n-10.0)*(n-10.0) );
	dest.quality = f * 2000.0 / ( 2000.0+ fabs(td) );  
	dest.lastly_seen = ref_time;
	dest.pos_known = (t.diff_msec (latest_motion_model)<2000 ?  
		(ball_raised ? BallLocation::raised : BallLocation::known) : BallLocation::unknown);
  	
	if ( dest.pos_known == BallLocation::unknown || 
		( dest.pos_known == BallLocation::raised && t.diff_msec (latest_motion_model)>=1000 ) )
	{
		if ( t.diff_msec (comm_ball_pos_time)<1000 ) 
		{
			dest.pos = comm_ball_pos;
			dest.velocity = Vec::zero_vector;
			dest.pos_known = BallLocation::communicated;
		}
#if USE_SMALL_CAMERA
		else if( small_camera_ball_dir > 0 ) 
		{
			dest.pos = small_camera_ball_pos;
			dest.velocity = Vec::zero_vector;
			dest.pos_known = BallLocation::small_camera; // get ball info from small camera
		}
#endif
	}
 
	if ( dest.pos_known == BallLocation::raised || dest.pos_known == BallLocation::unknown )
	{
		dest.pos = pos_for_raised.get().pos;
		dest.velocity = Vec::zero_vector;
	}
	
	return dest;
}

bool DynamicSlidingWindowBallFilter::update_motion_model () throw () 
{
	const unsigned int n = pos_list.size();
	double sum_ts = 0;     
	double sum_ts2 = 0;    
	Vec sum_pos (0,0);      
	Vec sum_ts_pos (0,0);   
	double n_eff=0;       
	for (unsigned int i=0; i<n; i++) 
	{
		const PairTimePos& tmp = pos_list.get();
		pos_list.step();
		double tau = static_cast<double>(tmp.timestamp.diff_msec (ref_time));
		if (tau>-2000) 
		{
			Vec x = tmp.pos;
			sum_ts+=tau;
			sum_ts2+=tau*tau;
			sum_pos+=x;
			sum_ts_pos+=tau*x;
			n_eff++;
		}
	}
	if (n_eff>1.5) 
	{  
		const double weight_decay=10000;
		sum_ts2+=weight_decay;
		double det = n_eff*sum_ts2-sum_ts*sum_ts;
		if ( fabs(det)<1e-5) 
		{
			
			ref_bpos = sum_pos/n_eff;
			ref_bvel = Vec::zero_vector;
			return false;
		} 
		else 
		{
			ref_bpos = (sum_ts2*sum_pos-sum_ts*sum_ts_pos)/det;
			ref_bvel = (-sum_ts*sum_pos+static_cast<double>(n)*sum_ts_pos)/det;
			return true;
		}
	} 
	else 
	{
		return false;
	}
	return true;
}

void DynamicSlidingWindowBallFilter::rescale_ring_buffer (unsigned int n) throw () 
{
	while (pos_list.size()>n)
		pos_list.erase();
}

// need to edit notice by lrd 
bool DynamicSlidingWindowBallFilter::inside_field (Vec p) const throw () 
{
	const FieldGeometry& fg = (WorldModel::get_main_world_model().get_field_geometry());
	if (fabs(p.x)>0.5*fg.field_length+fg.goal_band_width+400)     
		return false;
	if (fabs(p.y)>0.5*fg.field_width+fg.side_band_width+400)   
		return false;
	return true;
}

void DynamicSlidingWindowBallFilter::comm_ball( Vec p ) throw () 
{
	comm_ball_pos = MWM.get_own_half()*p;
	comm_ball_pos_time.update();
}

void DynamicSlidingWindowBallFilter::small_camera_ball(BYTE dir,const RobotLocation& rp) throw()
{
	Vec robot_pos = rp.pos;
	small_camera_ball_dir = dir;
  	switch(small_camera_ball_dir)
	{
		case 0x11:
			small_camera_ball_pos = Vec(320.0,-380.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x12:
			small_camera_ball_pos = Vec(180.0,-480.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x13:
			small_camera_ball_pos = Vec(0.0,-500.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x21:
			small_camera_ball_pos = Vec(-150.0,-420.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x22:
			small_camera_ball_pos = Vec(-290.0,-340.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x23:
			small_camera_ball_pos = Vec(-390.0,230.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x31:
			small_camera_ball_pos = Vec(-440.0,-100.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x32:
			small_camera_ball_pos = Vec(-440.0,100.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x33:
			small_camera_ball_pos = Vec(-390.0,220.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x41:
			small_camera_ball_pos = Vec(320.0,380.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x42:
			small_camera_ball_pos = Vec(180.0,480.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x43:
			small_camera_ball_pos = Vec(0.0,500.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		default:
			break;
	}
}

/*  robot 5;
	Vec robot_pos = rp.pos;
	small_camera_ball_dir = dir;
  	double px = 150.0;
	double py = 660.0;
	switch(small_camera_ball_dir)
	{
		case 0x11:
			small_camera_ball_pos = Vec(px,-py).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x12:
			small_camera_ball_pos = Vec(0.0,-py).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x13:
			small_camera_ball_pos = Vec(-px,-py).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x21:
			small_camera_ball_pos = Vec(-py,-px).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x22:
			small_camera_ball_pos = Vec(-py,0.0).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x23:
			small_camera_ball_pos = Vec(-py,px).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x31:
			small_camera_ball_pos = Vec(-px,py).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x32:
			small_camera_ball_pos = Vec(0.0,py).relativeToGlobal(rp.pos,rp.heading);
			break;
		case 0x33:
			small_camera_ball_pos = Vec(px,py).relativeToGlobal(rp.pos,rp.heading);
			break;
 		default:
			break;
	}
*/