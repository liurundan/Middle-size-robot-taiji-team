#include "StdAfx.h"
#include "ErrorMinimiserSL.h"
#include "Journal.h"
#include "WorldModel.h"
#include "update_robot_location.h"
#include "random.h"
#include <cmath>

using namespace std;

#define DEBUG_INTERNAL_SL 0    
#define DEBUG_EXTERNAL_SL 0   

#if DEBUG_INTERNAL_SL
#define DEBUG_SL 1
#else 
#if DEBUG_EXTERNAL_SL
#define DEBUG_SL 1
#else
#define DEBUG_SL 0   
#endif
#endif

namespace 
{
	double maxx = 10;  
	double maxy = 10;   
	double gsx = 10;  
	double gsy = 10;  
}

ErrorMinimiserSL::ErrorMinimiserSL (  ConfigReader& reader, const OdometryContainer& odo,
									const CompassContainer& com,
									const FieldGeometry& fg) throw (std::bad_alloc) 
									: odobox (odo),combox(com),plog (NULL), external_alternatives (1)
{
	maxx = 0.5*fg.field_length+fg.goal_band_width;
	maxy = 0.5*fg.field_width+fg.side_band_width;
	gsx = 0.5*fg.field_length;
	gsy = 0.5*fg.goal_width;
	
	field_lut = new FieldLUT (fg, 50);
	double err_width;
	if( reader.get("ErrorMinimiserSL::error_width", err_width) < 0 )
		err_width = 250;
	double dist_param;
	
	if( reader.get("ErrorMinimiserSL::distance_weight_parameter", dist_param)<0)
		dist_param = 1e50;   
	consider_yellow_goal = consider_blue_goal = false;
	reader.get ("ErrorMinimiserSL::consider_yellow_goal", consider_yellow_goal);
	reader.get ("ErrorMinimiserSL::consider_blue_goal", consider_blue_goal);
	use_internal_alternatives = true;
	reader.get ("ErrorMinimiserSL::use_internal_alternatives", use_internal_alternatives);
	num_external_alternatives = 1;
	reader.get ("ErrorMinimiserSL::number_external_alternatives", num_external_alternatives);
	max_lines = 1000;
	reader.get ("ErrorMinimiserSL::max_lines", max_lines);
	external_alternatives.resize(num_external_alternatives);
	
	number_particles = 60;
	reader.get ("ErrorMinimiserSL::number_particles", number_particles);
	min_match_error  = 6.0;
	reader.get ("ErrorMinimiserSL::min_match_error", min_match_error );
    use_global_localization = false;
	reader.get ("ErrorMinimiserSL::use_global_localization", use_global_localization );

	consider_compass = false;
	reader.get("ErrorMinimiserSL::consider_compass",consider_compass);
	
	vis_optimiser = new VisualPositionOptimiser(*field_lut, err_width, dist_param);
	
#if DEBUG_SL
	std::string fname;
	if (!reader.get ("write_world_model_info", fname))
		fname = "wminfo";
	fname += ".ppos";
	plog = new std::ofstream (fname.c_str());
	if( !plog ) 
	{
		JERROR("could not open particle log file");
		delete plog;
		plog = NULL;
	}
#endif
 
	reset ();
}

ErrorMinimiserSL::~ErrorMinimiserSL () throw () 
{
	delete field_lut;
	delete vis_optimiser;
	if (plog) 
	{
		plog->flush();
		delete plog;
	}
}

namespace 
{
	inline double hoch6 (double x)
	{
		double y=x*x*x;
		return y*y;
	}
}


void ErrorMinimiserSL::update_alternative (AltPos& altpos, const VisibleObjectList& lines, 
										   const VisibleObjectList& goals, 
										   bool internal_alternatives, bool is_main_alternative) 
{
	
#if DEBUG_INTERNAL_SL
	Time now;
	int dir = WorldModel::get_main_world_model().get_own_half();
#endif
	
	Time current_update    = lines.timestamp;
	RobotLocation delta_xv = odobox.movement (latest_update, current_update);
	
	Vec   old_pos;
	Angle old_heading;
	altpos.rpos_filter.get (old_pos, old_heading);
	Vec trans_welt   = delta_xv.pos.rotate(old_heading);
	Vec odometry_pos = old_pos+delta_xv.pos.rotate(old_heading);
	Angle odometry_heading = old_heading+delta_xv.heading;
	Vec  visual_pos = odometry_pos;
	Angle visual_heading = odometry_heading;
	double ref_error = vis_optimiser->optimise (visual_pos, visual_heading, 
		lines, lines.objectlist.size()>20 ? 10 : 20, max_lines);
	
	if (internal_alternatives && lines.objectlist.size()>=10) 
	{
		
#if DEBUG_INTERNAL_SL
		(*plog) << now << '\t' << dir*visual_pos.x << '\t' << dir*visual_pos.y 
			<< '\t' << (visual_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 3 << '\n';
		(*plog) << now << '\t' << dir*visual_pos.x << '\t' << dir*visual_pos.y 
			<< '\t' << (visual_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 4 << '\n';      
		LOUT << "SL Interanl search position: " << ref_error << ' ' 
			<< visual_pos.x << ' ' << visual_pos.y << ' ' << visual_heading.get_deg() << '\n';
#endif
		
		Vec offsets [] = { Vec(-600,0), Vec(0,600), Vec(600,0), Vec(0,-600) };
		double min_error = ref_error;
		int min_index=-1;
		double std_head = sqrt( altpos.rpos_filter.get_heading_variance () );
		double sfac = 1.7-0.7/(static_cast<double>(cycles_since_reset)+1.0);   
		for (unsigned int i=0; i<4; i++) 
		{
			Vec trial_pos = odometry_pos+offsets[i];
			Angle trial_heading = odometry_heading+Angle::rad_angle(std_head*nrandom());;
			double err = vis_optimiser->optimise( trial_pos, trial_heading, 
				lines, lines.objectlist.size()>20 ? 10 : 20, max_lines) ;
			if (err<min_error && sfac*err<ref_error) 
			{
				min_index=i;
				min_error=err;
				visual_pos=trial_pos;
				visual_heading=trial_heading;
			}
			
#if DEBUG_INTERNAL_SL
			(*plog) << now << '\t' << dir*trial_pos.x << '\t' << dir*trial_pos.y 
				<< '\t' << (trial_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 3 << '\n';
			LOUT << "SL Interanl search position: " << err << ' ' << trial_pos.x << ' ' <<
				trial_pos.y << ' ' << trial_heading.get_deg() << '\n';
#endif
		}
		
#if DEBUG_INTERNAL_SL
		LOUT << "SL select internal search position: " << min_index+2 << '\n'; 
#endif
		
	}
	
	double ddphi;
	Vec ddxy;
	altpos.latest_error = vis_optimiser->analyse( ddxy, ddphi, visual_pos, visual_heading, lines, max_lines);
	double fnum = 16.0/(lines.objectlist.size()+4.0)+0.7;   
	Vec var_xy (trans_welt.x*trans_welt.x+fnum*225*hoch6(log(fabs(ddxy.y)+1e-6)+7), 
		trans_welt.y*trans_welt.y+fnum*225*hoch6(log(fabs(ddxy.x+1e-6))+7));  
	
	double var_phi = fnum*3.0461741978670859865/(ddphi*ddphi+1-6);  
	if (lines.objectlist.size()>3)
		altpos.rpos_filter.update(delta_xv.pos, delta_xv.heading, visual_pos, visual_heading, var_xy, var_phi );
	else
		altpos.rpos_filter.update( delta_xv.pos, delta_xv.heading);
	Vec   filter_pos;
	Angle filter_heading;
	double latest_quality = altpos.rpos_filter.get (filter_pos, filter_heading);
	
	Angle cpc_dir = combox.get_compass(current_update); // use cpc to correct heding
	
	if( consider_compass && fabs( (filter_heading.get_deg() - (cpc_dir+	Angle::half).get_deg() ) < 30 ) )
	{
		altpos.rpos_filter.mirror();
	}
	
#if DEBUG_INTERNAL_SL
	double latest_quality = altpos.rpos_filter.get( filter_pos, filter_heading);
#else
	altpos.rpos_filter.get(filter_pos, filter_heading);
#endif
	
#if DEBUG_INTERNAL_SL
	(*plog) << now << '\t' << dir*old_pos.x << '\t' << dir*old_pos.y << '\t' << (old_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 1 << '\n';
	(*plog) << now << '\t' << dir*odometry_pos.x << '\t' << dir*odometry_pos.y << '\t' << (odometry_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 2 << '\n';
	(*plog) << now << '\t' << dir*visual_pos.x << '\t' << dir*visual_pos.y << '\t' << (visual_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 5 << '\n';
	(*plog) << now << '\t' << dir*filter_pos.x << '\t' << dir*filter_pos.y << '\t' << (filter_heading+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 6 << '\n';
	LOUT << "SL Odometrie-away: " << delta_xv.pos.x << ' ' << delta_xv.pos.y << ' ' << delta_xv.heading.get_deg() << '\n';
	LOUT << "SL Image processing error and 2nd derivatives: " << altpos.latest_error << ' ' << ddxy.x << ' ' << ddxy.y << ' ' << ddphi << '\n';
	LOUT << "SL Filter quality: " << latest_quality << '\n';
#endif
    
	if (consider_yellow_goal || consider_blue_goal) 
	{
		unsigned int num_true_dir=0;    
		unsigned int num_false_dir=0;   
		unsigned int num_fail=0;    
		vector<VisibleObject>::const_iterator visit = goals.objectlist.begin();
		vector<VisibleObject>::const_iterator visend = goals.objectlist.end();
		int c;   
		Vec rpos;
		Angle rhead;
		altpos.rpos_filter.get (rpos, rhead);
		while (visit<visend) 
		{
			if (visit->object_type==VisibleObject::blue_goal || 
				visit->object_type==VisibleObject::blue_goal_post_left || 
				visit->object_type==VisibleObject::blue_goal_post_right )
 				c = +1;
  			else
 				c = -1;
 
 			Angle goal_angle    = rhead+visit->pos.angle();
			double goal_distance = visit->pos.length();
			if ((gsx+gsy-(rpos.x+rpos.y)<0) || (gsx+gsy+(rpos.y-rpos.x)<0) || 
				(gsx-rpos.x<0) || (gsx+gsy+(rpos.x+rpos.y)<0) || 
				(gsx+gsy-(rpos.y-rpos.x)<0) || (gsx+rpos.x<0)) 
			{
				num_fail++;
			} 
			else 
			{
				if ((c==+1 && consider_blue_goal) || (c==-1 && consider_yellow_goal)) 
				{
					Angle blue_goal_right = (Vec (gsx,-gsy)-rpos).angle();
					Angle blue_goal_left = (Vec (gsx,gsy)-rpos).angle();
					Angle yellow_goal_right = (Vec (-gsx,gsy)-rpos).angle();
					Angle yellow_goal_left = (Vec (-gsx,-gsy)-rpos).angle();
					double blue_goal_distance = (Vec (gsx,0)-rpos).length();
					double yellow_goal_distance = (Vec (-gsx,0)-rpos).length();
					if (goal_angle.in_between (blue_goal_right, blue_goal_left) && goal_distance+1000>blue_goal_distance)
					{
						
						if (c==1)
						{
							num_true_dir++;
						}
						else
							num_false_dir++;
					} 
					else if (goal_angle.in_between (yellow_goal_right, yellow_goal_left) && 
						goal_distance+1000>yellow_goal_distance)
					{
						
						if (c==1)
 							num_false_dir++;
 						else
						{
							num_true_dir++;
						}
					} 
					else
						num_fail++;
				}
			}
			visit++;
		}
		
		altpos.ema_right_goal *= 0.75;
		altpos.ema_wrong_goal *= 0.75;
		if ( num_true_dir>0 && num_false_dir == 0 ) 
			altpos.ema_right_goal += 0.25*num_true_dir;
		if (num_true_dir==0 && num_false_dir>0)
		{
			altpos.ema_wrong_goal+=0.25*num_false_dir;
			if (altpos.ema_wrong_goal>altpos.ema_right_goal+0.2 && altpos.ema_wrong_goal>0.45) 
			{
				altpos.rpos_filter.mirror();
				altpos.heading_checked=true;
				double sw=altpos.ema_wrong_goal;
				altpos.ema_wrong_goal=altpos.ema_right_goal;
				altpos.ema_right_goal=sw;
				
				if (is_main_alternative)
					JWARNING ("SL: switching global orientation");
			}
		}
		
#if DEBUG_INTERNAL_SL
		LOUT << "SL Goalcheck: fail=" << num_fail << " true=" << num_true_dir << " false=" << num_false_dir << '\n';
#endif
	}
}

bool ErrorMinimiserSL::update (const VisibleObjectList& lines, const VisibleObjectList& goals) throw () 
{
 	if( lines.objectlist.size() > 5 ) 
	{
		vis_optimiser->calculate_distance_weights(lines, max_lines);
		update_alternative (main_position, lines, goals, use_internal_alternatives, true);
		for (unsigned int i1=0; i1<num_external_alternatives; i1++)
			update_alternative (external_alternatives[i1], lines, goals, false, false);
		cycles_since_reset++;
		latest_update = lines.timestamp;
		
		Vec p_main, p1, p2;
		Angle h_main, h1, h2;
		Time now;
		main_position.rpos_filter.get (p_main, h_main);
		for (unsigned int  i=0; i<num_external_alternatives; i++) 
		{
			external_alternatives[i].rpos_filter.get (p1,h1);
			double dt = now.diff_msec(external_alternatives[i].init_time);
			bool replace = false;
			replace |= (fabs(p1.x)>maxx) || (fabs(p1.y)>maxy);    
			replace |= ((p1-p_main).squared_length()<40000 && 
				(h1-h_main).in_between (Angle::eleven_twelvth, Angle::twelvth));  // 330~30
			replace |= ((p1+p_main).squared_length()<40000 && 
				(h1+h_main).in_between (Angle::eleven_twelvth, Angle::twelvth));  
			replace |= (dt>=2000);   
			replace |= (dt>=1000 &&  external_alternatives[i].winning_coefficient<0.2);
			replace |= (dt>=500 && external_alternatives[i].winning_coefficient<0.1);
			replace |= (dt>=150 && external_alternatives[i].winning_coefficient<0.001);   
			if (replace) 
			{
				AltPos z;
				external_alternatives[i]=z;
				z.rpos_filter.get (p1,h1);
			}
			for (unsigned int j=i+1; j<num_external_alternatives; j++) 
			{
				external_alternatives[j].rpos_filter.get (p2,h2);
				
				if (((p1-p2).squared_length()<40000 && 
					(h1-h2).in_between (Angle::eleven_twelvth, Angle::twelvth)) || 
					((p1+p2).squared_length()<40000 && (h1+h2).in_between (Angle::eleven_twelvth, Angle::twelvth))) 
				{
					if (external_alternatives[j].winning_coefficient < external_alternatives[i].winning_coefficient) 
					{
						AltPos z;
						external_alternatives[j]=z;
					}
					else 
					{
						external_alternatives[i]=external_alternatives[i]; // whether use i = j;
						AltPos z;
						external_alternatives[j]=z;
						external_alternatives[i].rpos_filter.get (p1,h1);
					}
				}
			}
		}
		
		double min_error = main_position.latest_error - 3 - 10.0 *
			( max_lines <lines.objectlist.size() ? max_lines : lines.objectlist.size() ) / 200;  
		int min_index = -1;
		for ( unsigned int i2=0; i2<num_external_alternatives; i2++)
		{
			if (external_alternatives[i2].latest_error<min_error) 
			{
				min_error=external_alternatives[i2].latest_error;
				min_index=i;
			}
		}
		main_position.winning_coefficient = 0.95*main_position.winning_coefficient+
			( min_index == -1 ? 0.05 : 0.0 );
		for ( unsigned int j=0; j<num_external_alternatives; j++)
		{
			external_alternatives[j].winning_coefficient = 
			0.95*external_alternatives[j].winning_coefficient+(min_index==static_cast<int>(j) ? 0.05 : 0.0); 
		}
		
		double max_win = main_position.winning_coefficient;
		int max_index=-1;
		for ( unsigned int k=0; k<num_external_alternatives; k++)
		{
			if (external_alternatives[k].winning_coefficient>max_win) 
			{
				max_win=external_alternatives[k].winning_coefficient;
				max_index=i;
			}
		}
		if (max_index!=-1 && max_win>=0.3) 
		{
			Vec p1, p2;
			Angle h1, h2;
			main_position.rpos_filter.get (p1,h1);
			main_position = external_alternatives[max_index];
			main_position.rpos_filter.get (p2,h2);
			if (!main_position.heading_checked) 
			{
				main_position.ema_right_goal=0.5;
				main_position.ema_wrong_goal=0.5;
				if ((h1-h2).in_between (Angle::quarter, Angle::three_quarters))
					main_position.rpos_filter.mirror();       
			}
			AltPos z;
			external_alternatives[max_index] = z;
			
#if DEBUG_EXTERNAL_SL
			LOUT << "SL external replacement by alternative " << max_index+1 << '\n';
#endif
			JWARNING ("SL: auto robot displacement");
		}
		
#if DEBUG_EXTERNAL_SL
		int dir = WorldModel::get_main_world_model().get_own_half();
		Vec p;
		Angle h;
		for (unsigned int i=0; i<num_external_alternatives; i++) 
		{
			external_alternatives[i].rpos_filter.get (p,h);
			(*plog) << now << '\t' << dir*p.x << '\t' << dir*p.y << '\t' 
				<<  (h+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 10 << '\n';
			LOUT << "SL Alternative " << i+1 << ": " 
				<< now.diff_msec(external_alternatives[i].init_time) << '\t'
				<< external_alternatives[i].winning_coefficient << '\t' << external_alternatives[i].latest_error 
				<< '\t' << dir*p.x << '\t' << dir*p.y << '\t' << (h+(dir>0 ? Angle::zero : Angle::half)).get_deg() << '\n';
		}
		main_position.rpos_filter.get (p,h);
		(*plog) << now << '\t' << dir*p.x << '\t' << dir*p.y << '\t' 
			<< (h+(dir>0 ? Angle::zero : Angle::half)).get_rad() << '\t' << 20 << '\n';
		LOUT << "SL Hypothese: " << now.diff_msec(main_position.init_time) 
			<< '\t' << main_position.winning_coefficient << '\t' << main_position.latest_error 
			<< '\t' << dir*p.x << '\t' << dir*p.y << '\t' << (h+(dir>0 ? Angle::zero : Angle::half)).get_deg() << '\n';
#endif

		MWM.pos_err = main_position.latest_error;
		if( use_global_localization && 
			( main_position.latest_error > min_match_error ) ) 
		{
			choose_main_pos(lines,goals,main_position);
		}
		
		return true;    
  } 
  else
	  return false;
}

RobotLocation ErrorMinimiserSL::get(Time t) const throw () 
{
	RobotLocation rloc;
	rloc.quality = main_position.rpos_filter.get(rloc.pos, rloc.heading);
	return odobox.add_movement (rloc, latest_update, t);
}

void ErrorMinimiserSL::reset() throw () 
{
	AltPos z;
	main_position = z;
	latest_update.update();
	cycles_since_reset=0;
	JWARNING ("SL: manual random reset");
}

void ErrorMinimiserSL::reset(Vec p) throw () 
{
	AltPos z (p);
	main_position = z;
	latest_update.update();
	cycles_since_reset=0;
	JWARNING ("SL: manual position reset");
}

void ErrorMinimiserSL::reset(Vec p, Angle h) throw () 
{
	AltPos z (p,h);
	main_position = z;
	latest_update.update();
	cycles_since_reset=0;
	JWARNING ("SL: manual position and heading reset");
}

ErrorMinimiserSL::AltPos::AltPos() 
{
	rpos_filter.set (Vec(urandom(-maxx,maxx),urandom(-maxy,maxy)), Angle::zero, Vec(1e10, 1e10), 400);
	winning_coefficient=0;
	latest_error=1e100;
	heading_checked=false;
	ema_right_goal=0.51;
	ema_wrong_goal=0.5;
}

ErrorMinimiserSL::AltPos::AltPos(Vec p) 
{
	rpos_filter.set (p, Angle::zero, Vec(500,500), 400);
	winning_coefficient=0;
	latest_error=0;
	heading_checked=false;
	ema_right_goal=0.5;
	ema_wrong_goal=0.5;
}

ErrorMinimiserSL::AltPos::AltPos(Vec p, Angle h)
{
	rpos_filter.set (p, h, Vec(500,500), 0.1);
	winning_coefficient=0;
	latest_error=0;
	heading_checked=true;
	ema_right_goal=0.5;
	ema_wrong_goal=0.5;
}
 
void ErrorMinimiserSL::choose_main_pos(const VisibleObjectList &lines, const VisibleObjectList &goals, AltPos &main_pos)
{
    Time current_update;
 	Vec p, pTemp, pMin;
	Angle h, hTemp, hMin;
	main_pos.rpos_filter.get(p,h);
	Angle  cpc_dir = combox.get_compass(current_update);  
    double offset  = 0.8;
	double err     = 0;
	double min_err = 1e5;
	double err_ref = vis_optimiser->error(p,h,lines,max_lines);

	for( unsigned int i = 0; i < number_particles; i++ )
	{
		pTemp = Vec(urandom(-maxx,maxx),urandom(-maxy,maxy));
		hTemp = urandom(cpc_dir.get_rad()-offset,cpc_dir.get_rad()+offset);
		err = vis_optimiser->error(pTemp,hTemp,lines,max_lines);
		if( err < min_err )
		{
			pMin = pTemp;
			hMin = hTemp;
			min_err = err;
		}
	}
 	if( min_err < err_ref )
		main_pos.rpos_filter.set( pMin, hMin,Vec(1e2,1e2),1e2 );

}

