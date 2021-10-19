#include "StdAfx.h"
#include "ErrorMinimiserSL.h"
#include "update_robot_location.h"
#include "math.h"

#include <fstream>
#include <iomanip>

#define DEBUG_SL       0
#define DEBUG_LINES    0

#define DEBUG_EXTERNAL_SL 0
#define DEBUG_ODOMETRY_SL 0

using namespace std;
 
namespace 
{
  double maxx = 10;   
  double maxy = 10;   
  double gsx  = 10;  
  double gsy  = 10;  
}
 
ErrorMinimiserSL::ErrorMinimiserSL ( const OdometryContainer& odo, const FieldGeometry& fg) throw (std::bad_alloc) : odobox (odo), external_alternatives(1) 
{
  maxx = 0.5*fg.field_length+fg.goal_band_width;
  maxy = 0.5*fg.field_width+fg.side_band_width;
  gsx = 0.5*fg.field_length;
  gsy = 0.5*fg.goal_width;
  field_lut = new FieldLUT (fg, 50);
  double err_width;
  err_width = 300;     // 250
  double dist_param;

  dist_param = 2500;   // 1e50
  consider_yellow_goal=consider_blue_goal=true;
   
  use_internal_alternatives = true;
  num_external_alternatives = 3; // 1
  max_lines = 100;

  external_alternatives.resize(num_external_alternatives);
  
  vis_optimiser = new VisualPositionOptimiser( *field_lut, err_width, dist_param );
  
  //for test odo
  rp.pos     = 0;
  rp.heading = 0;
  //reset(Vector(0,0),MyAngle::zero);
  reset( );
}

ErrorMinimiserSL::~ErrorMinimiserSL () throw () 
{
  delete field_lut;
  delete vis_optimiser;
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
										   const VisibleObjectList& goals, bool internal_alternatives,
										   bool is_main_alternative) 
{
  MyTime        current_update = lines.timestamp;
  // notice odometry is relative with time change
  //RobotLocation delta_xv2 = odobox.movement (latest_update, current_update);
  RobotLocation delta_xv  = odobox.Getodometry();
  Vector        old_pos; 
  MyAngle       old_heading;
  altpos.rpos_filter.get (old_pos, old_heading);
  Vector  trans_welt  = delta_xv.pos.rotate(old_heading);

  Vector  odometry_pos     = old_pos + delta_xv.pos.rotate(old_heading);
  MyAngle odometry_heading = MyAngle::rad_angle( (old_heading + delta_xv.heading).get_rad_pi() );

  Vector  visual_pos       = odometry_pos;
  MyAngle visual_heading   = odometry_heading;
 
  double  ref_error = vis_optimiser->optimise (visual_pos, visual_heading, lines,
	                  lines.objectlist.size()>20 ? 10 : 20, max_lines );

  if( internal_alternatives && lines.objectlist.size()>=10  ) 
  {
 
    Vector offsets [] = { Vector(600,0), Vector(0,600), Vector(-600,0), Vector(0,-600) };
    double min_error = ref_error;
    int    min_index = -1;
    double std_head = sqrt(altpos.rpos_filter.get_heading_variance ());
    double sfac     = 1.7-0.7/(static_cast<double>(cycles_since_reset)+1.0);    
    for (unsigned int i=0; i<4; i++) 
	{
      Vector  trial_pos = odometry_pos+offsets[i];
      MyAngle trial_heading = odometry_heading+MyAngle::rad_angle(std_head*nrandom());// drand()*1e3 lead to data floor
      double err = vis_optimiser->optimise (trial_pos, trial_heading, lines, 
		  lines.objectlist.size()>20 ? 10 : 20, max_lines);
      if (err<min_error && sfac*err<ref_error) 
	  {
		min_index=i;
		min_error=err;
		visual_pos=trial_pos;
		visual_heading=trial_heading;
      }
	}
  }  

  double ddphi;
  Vector ddxy;
  altpos.latest_error = vis_optimiser->analyse (ddxy, ddphi, visual_pos, visual_heading, 
	                                            lines, max_lines);
  double fnum = 16.0/(lines.objectlist.size()+4.0)+0.7;
  
  Vector var_xy(trans_welt.Y*trans_welt.Y+fnum*225*hoch6(log(fabs(ddxy.X)+1e-6)+7), 
            trans_welt.X*trans_welt.X+fnum*225*hoch6(log(fabs(ddxy.Y+1e-6))+7)); 
  
  double var_phi = fnum*3.0461741978670859865/(ddphi*ddphi+1-6);  
 
  if (lines.objectlist.size()>3)
    altpos.rpos_filter.update (delta_xv.pos, delta_xv.heading, visual_pos, visual_heading, var_xy, var_phi);
  else
    altpos.rpos_filter.update (delta_xv.pos, delta_xv.heading);
  /*
  Vector  filter_pos;
  MyAngle filter_heading;
  altpos.rpos_filter.get(filter_pos,filter_heading);*/
  if (consider_yellow_goal || consider_blue_goal) 
  {
    unsigned int num_true_dir=0;    
    unsigned int num_false_dir=0;   
    unsigned int num_fail=0;    
    vector<VisibleObject>::const_iterator visit = goals.objectlist.begin();
    vector<VisibleObject>::const_iterator visend = goals.objectlist.end();
    int     c;   
    Vector  rpos;
    MyAngle rhead;
    altpos.rpos_filter.get (rpos, rhead);
    while (visit<visend) 
	{
      if (visit->object_type==VisibleObject::blue_goal ||
		  visit->object_type==VisibleObject::blue_goal_post_left ||
		  visit->object_type==VisibleObject::blue_goal_post_right )
	    c=+1;
      else
	    c=-1;
      MyAngle goal_angle    = rhead+visit->pos.angle();
      double  goal_distance = visit->pos.getMagnitude();
      if ((gsx+gsy-(rpos.X+rpos.Y)<0) || (gsx+gsy+(rpos.X-rpos.Y)<0) || 
		  (gsx-rpos.X<0) || (gsx+gsy+(rpos.X+rpos.Y)<0) || 
		  (gsx+gsy-(rpos.X-rpos.Y)<0) || (gsx+rpos.X<0) )  // edit condition
	  {
	   num_fail++;
	  } 
	  else 
	  {
	     if ((c==+1 && consider_blue_goal) || (c==-1 && consider_yellow_goal)) 
		 {
		  MyAngle blue_goal_right   = (Vector (-gsx,gsy)-rpos).angle();// - -
		  MyAngle blue_goal_left    = (Vector (-gsx,-gsy)-rpos).angle();// - +
		  MyAngle yellow_goal_right = (Vector (gsx,-gsy)-rpos).angle();// + +
		  MyAngle yellow_goal_left  = (Vector (gsx,gsy)-rpos).angle();// + -
		  double  blue_goal_distance   = (Vector (-gsx,0)-rpos).getMagnitude();
		  double  yellow_goal_distance = (Vector (gsx,0)-rpos).getMagnitude();
		  if( goal_angle.in_between( blue_goal_right, blue_goal_left ) && 
			  goal_distance+1000>blue_goal_distance ) 
		  {
			 
			if (c==1)
			  num_true_dir++;
			else
			  num_false_dir++;
		  } 
		  else if( goal_angle.in_between( yellow_goal_right, yellow_goal_left) &&
			       goal_distance+1000 > yellow_goal_distance ) 
		  {
 			if (c==1)
			  num_false_dir++;
			else
			  num_true_dir++;
		  } 
		  else
			num_fail++;
		 } 
      }
      visit++;
    }
    altpos.ema_right_goal*=0.75;
    altpos.ema_wrong_goal*=0.75;
	
    if( num_true_dir > 0 && num_false_dir==0 ) 
      altpos.ema_right_goal+=0.25*num_true_dir;
    if( num_true_dir == 0 && num_false_dir > 0 ) 
	{
       altpos.ema_wrong_goal+=0.25*num_false_dir;
       if (altpos.ema_wrong_goal>altpos.ema_right_goal+0.2 && altpos.ema_wrong_goal>0.45) 
	   {
		 altpos.rpos_filter.mirror();
		 altpos.heading_checked=true;
		 double sw=altpos.ema_wrong_goal;
		 altpos.ema_wrong_goal=altpos.ema_right_goal;
		 altpos.ema_right_goal=sw;
	   }
	}
  }
}
 
bool ErrorMinimiserSL::update (const VisibleObjectList& lines, const VisibleObjectList& goals) throw () 
{
   if( lines.objectlist.size() > 5 ) 
   {
    vis_optimiser->calculate_distance_weights (lines, max_lines);
    update_alternative (main_position, lines, goals, use_internal_alternatives, true);
 	for (unsigned int i1=0; i1<num_external_alternatives; i1++)
      update_alternative (external_alternatives[i1], lines, goals, false, false);
    cycles_since_reset++;
    latest_update = lines.timestamp;

    Vector  p_main, p1, p2;
    MyAngle h_main, h1, h2;
    MyTime now;
    main_position.rpos_filter.get (p_main, h_main);

    for (unsigned int i=0; i<num_external_alternatives; i++) 
	{
      external_alternatives[i].rpos_filter.get (p1,h1);
      double dt = now.diff_msec(external_alternatives[i].init_time);
      bool replace = false;
      replace |= (fabs(p1.X)>maxx) || (fabs(p1.Y)>maxy);   
      replace |= ((p1-p_main).getSqrMagnitude()<40000 &&
		  (h1-h_main).in_between (MyAngle::eleven_twelvth, MyAngle::twelvth));   
      replace |= ((p1+p_main).getSqrMagnitude()<40000 && 
		  (h1+h_main).in_between (MyAngle::eleven_twelvth, MyAngle::twelvth));  
      replace |= (dt>=2000);   
      replace |= (dt>=1000 &&  external_alternatives[i].winning_coefficient<0.2);
      replace |= (dt>=500 && external_alternatives[i].winning_coefficient<0.1);
      replace |= (dt>=150 && external_alternatives[i].winning_coefficient<0.001);  // in 120 ms  
      if (replace) 
	  {
		AltPos z;
		external_alternatives[i]=z;
		z.rpos_filter.get (p1,h1);
      }
      for (unsigned int j=i+1; j<num_external_alternatives; j++) 
	  {
		external_alternatives[j].rpos_filter.get (p2,h2);
		if (((p1-p2).getSqrMagnitude()<40000 && 
			(h1-h2).in_between (MyAngle::eleven_twelvth, MyAngle::twelvth)) || 
			((p1+p2).getSqrMagnitude()<40000 && 
			(h1+h2).in_between (MyAngle::eleven_twelvth, MyAngle::twelvth))) 
		{
		  if (external_alternatives[j].winning_coefficient < external_alternatives[i].winning_coefficient) 
		  {
			AltPos z;
			external_alternatives[j]=z;
		  } 
		  else 
		  {
			external_alternatives[i]=external_alternatives[i]; // old version is i = i;
			AltPos z;
			external_alternatives[j]=z;
			external_alternatives[i].rpos_filter.get (p1,h1);
		  }
		}
      }
    }

    double min_error = main_position.latest_error - 3 - 
		   10.0 * ( max_lines < lines.objectlist.size() ? max_lines : lines.objectlist.size() ) / 200;   
    
	int min_index=-1;
    for (unsigned int i2=0; i2<num_external_alternatives; i2++)
      if (external_alternatives[i2].latest_error<min_error)
	  {
		min_error = external_alternatives[i2].latest_error;
		min_index = i;
      }
	  
    main_position.winning_coefficient = 0.95*main_position.winning_coefficient+(min_index==-1 ? 0.05 : 0.0);
    for (unsigned int j=0; j<num_external_alternatives; j++)
      external_alternatives[j].winning_coefficient = 0.95*external_alternatives[j].winning_coefficient+
	                                                ( min_index==static_cast<int>(j) ? 0.05 : 0.0); 
    
    double max_win = main_position.winning_coefficient;
    int max_index  = -1;
    for( unsigned int k=0; k<num_external_alternatives; k++ )
      if (external_alternatives[k].winning_coefficient>max_win) 
	  {
		max_win=external_alternatives[k].winning_coefficient;
		max_index=k;
      }
    if (max_index!=-1 && max_win>=0.3) 
	{
      Vector p1, p2;
      MyAngle h1, h2;
      main_position.rpos_filter.get (p1,h1);
      main_position = external_alternatives[max_index];
      main_position.rpos_filter.get (p2,h2);
      if (!main_position.heading_checked) 
	  {
		main_position.ema_right_goal=0.5;
		main_position.ema_wrong_goal=0.5;
		if ((h1-h2).in_between (MyAngle::quarter, MyAngle::three_quarters))
		  main_position.rpos_filter.mirror();        
      }
      AltPos z;
      external_alternatives[max_index]=z;
      // JWARNING ("SL: auto robot displacement");
    }
    return true;    
  } 
  else     
    return false; 
 
}

RobotLocation ErrorMinimiserSL::get (MyTime t) const throw () 
{
  RobotLocation rloc;
  rloc.quality = main_position.rpos_filter.get (rloc.pos, rloc.heading);
  //  return odobox.add_movement (rloc, latest_update, t);
  return rloc; 
  
}

void ErrorMinimiserSL::reset () throw () 
{
  AltPos z;
  main_position = z;
  latest_update.update();
  cycles_since_reset=0;
}

void ErrorMinimiserSL::reset (Vector p) throw () 
{
  AltPos z (p);
  main_position = z;
  latest_update.update();
  cycles_since_reset=0;
}

void ErrorMinimiserSL::reset (Vector p, MyAngle h) throw ()
{
  AltPos z (p,h);
  main_position = z;
  latest_update.update();
  cycles_since_reset=0;
  
  // for test begin localize robot 
  ParPos pp;
  pp.pos = p;
  pp.heading = h;
  pp.quality = 0;
  initParticles(pp);
}

ErrorMinimiserSL::AltPos::AltPos( ) 
{
  rpos_filter.set (Vector(urandom(-maxx,maxx),urandom(-maxy,maxy)), MyAngle::zero, Vector(1e10, 1e10), 400);
  winning_coefficient=0;
  latest_error=1e100;
  heading_checked=false;
  ema_right_goal=0.51;
  ema_wrong_goal=0.5;
}

ErrorMinimiserSL::AltPos::AltPos( Vector p ) 
{
  rpos_filter.set (p, MyAngle::zero, Vector(500,500), 400);
  winning_coefficient=0;
  latest_error=0;
  heading_checked=false;
  ema_right_goal=0.5;
  ema_wrong_goal=0.5;
}

ErrorMinimiserSL::AltPos::AltPos( Vector p, MyAngle h ) 
{
  rpos_filter.set (p, h, Vector(500,500), 0.1);
  winning_coefficient=0;
  latest_error=0;
  heading_checked=true;
  ema_right_goal=0.5;
  ema_wrong_goal=0.5;
}

/************************************************************************/
/*                for lrd test                                          */
/************************************************************************/
 
ErrorMinimiserSL::ParPos::ParPos()
{
  pos     = Vector(urandom(-maxx,maxx),urandom(-maxy,maxy));
  heading = MyAngle::zero;
  quality = 0;
}

ErrorMinimiserSL::ParPos::ParPos(Vector p)
{
  pos     = p;
  heading = MyAngle::zero;
  quality = 0;
}

ErrorMinimiserSL::ParPos::ParPos(Vector p,MyAngle a)
{
  pos     = p;
  heading = a;
  quality = 0;
}
 
void ErrorMinimiserSL::initParticles()
{
  ParPos posInitial;
  for( int i = 0; i < iNrParticles; i++ )
  {
       particlesPosAgent[i].pos = posInitial.pos;
	   particlesPosAgent[i].heading = posInitial.heading;
	   particlesPosAgent[i].quality = posInitial.quality;
  }
}

void ErrorMinimiserSL::initParticles( ParPos posInitial )
{
   double std_pos = 0.1;
   double std_head = 0.001;
   for( int i = 0; i < iNrParticles; i++ )
   {
       particlesPosAgent[i].pos = posInitial.pos + n2random() * std_pos;
  	   particlesPosAgent[i].heading =  posInitial.heading +
		                               MyAngle::rad_angle(std_head*nrandom()); 
	   particlesPosAgent[i].quality = posInitial.quality;
   }
}

void ErrorMinimiserSL::resampleParticles( int iLeft, ParPos pp )
{
  double std_pos  = 0.006;
  double std_head = 0.1;
  for( int  i = 0;i < iNrParticles; i++ )
  {
	 particlesPosAgent[i].pos = rp.pos + n2random() * std_pos;
	 particlesPosAgent[i].heading = rp.heading + nrandom() * std_head;
  }
}

void ErrorMinimiserSL::updateParticles( ParPos pp )
{
  Vector delta_pos = pp.pos;

  for( int i = 0; i < iNrParticles; i++ )
  {
 	 particlesPosAgent[i].pos += delta_pos.rotate(particlesPosAgent[i].heading);
	 particlesPosAgent[i].heading += pp.heading;
	 delta_pos = pp.pos;
  }
}

int ErrorMinimiserSL::checkParticles( const VisibleObjectList& lines, RobotLocation delta_xv )
{
  int i=0, iNrLeft = 0, iLength = iNrParticles;
  double   err;
  double   min_err = 1e100;
  double   Number  = -1;
  
  Vector        old_pos     = rp.pos ;
  MyAngle       old_heading = rp.heading;
 
  Vector  odometry_pos     = old_pos + delta_xv.pos.rotate(old_heading);
  MyAngle odometry_heading = old_heading + delta_xv.heading;
  
  double  ref_error = vis_optimiser->error(odometry_pos, odometry_heading, lines,
	                  max_lines );

  if( lines.objectlist.size()>5 ) 
  {
	vis_optimiser->calculate_distance_weights (lines, max_lines);
    
    for( i = 0; i < iNrParticles; i++ )
	{
        Vector  trial_pos       = particlesPosAgent[i].pos;
        MyAngle trial_heading   = particlesPosAgent[i].heading;
   
        err = vis_optimiser->error(trial_pos, trial_heading, lines,
	                  max_lines );
 		particlesPosAgent[i].quality = err;
		if( err < min_err   ) 
		{
           min_err = err;
		   Number  = i;
		} 
	}
   /*
	for( i = 0; i < iLength; i++ )
	{
      err = particlesPosAgent[i].quality;
	  if( err < 0.01 )
	    particlesPosAgent[iNrLeft++] = particlesPosAgent[i];
	}*/
 
  }
 if( min_err > ref_error ) 
    Number = -1;
  return Number;

}

RobotLocation ErrorMinimiserSL::averageParticles( ParPos posArray[], int iLength )
{
   RobotLocation rloc;
   rloc.pos = 0;
   rloc.heading = 0;
   if ( iLength == 0)
      return rloc;
   double x = 0, y = 0, z = 0;
   for( int i = 0; i < iLength; i++ )
   {
	  x += posArray[i].pos.X;
	  y += posArray[i].pos.Y;
	  z += posArray[i].heading.get_rad();
   }

   rloc.pos = Vector( x/(double)iLength, y/(double)iLength );
   MyAngle heading = MyAngle::rad_angle(z/(double)iLength);
   rloc.heading = heading.get_rad_pi();
   
   return rloc;
}

 /*
bool ErrorMinimiserSL::update (const VisibleObjectList& lines, const VisibleObjectList& goals) throw () 
{
    int iNrLeft;
 	vis_optimiser->calculate_distance_weights (lines, max_lines);
    
	RobotLocation delta_xv = odobox.Getodometry();
	Vector  trans_welt  = delta_xv.pos.rotate(rp.heading);

 	ParPos delta_value;
	delta_value.pos = delta_xv.pos;
	delta_value.heading = delta_xv.heading;

    rp.pos += delta_xv.pos.rotate(rp.heading); // check finish
    rp.heading += delta_xv.heading;
/*
	//updateParticles( delta_value );
     double std_pos  =  0.1;
     double std_head =  0.01;

//	double coeff = 2*1e-3;
//	double var_xy = trans_welt.getMagnitude() * coeff;
//	double var_phi = delta_xv.heading.get_rad() * coeff;
    for( int  i = 0;i < iNrParticles; i++ ) // resample particles
	{ 
	 particlesPosAgent[i].pos.X = rp.pos.X + nrandom() * std_pos;
     particlesPosAgent[i].pos.Y = rp.pos.Y + nrandom() * std_pos;
	 particlesPosAgent[i].heading = rp.heading + nrandom() * std_head;
	}   

   #if DEBUG_SL
	ofstream foo;
 	foo.open("initpar.txt",ios::out|ios::app);
	foo<<setprecision(2)<<rp.pos.X<<" "<< rp.pos.Y
		<<" "<< rp.heading.get_deg_180()<<'\n';
	for( int j = 0; j < iNrParticles; j++ )
	{ 
 	 	vis_optimiser->calculate_distance_weights (lines, max_lines);
      
        Vector  trial_pos       = particlesPosAgent[j].pos;
        MyAngle trial_heading   = particlesPosAgent[j].heading;
   
        double  ref_error = vis_optimiser->error(trial_pos, trial_heading, lines,
	                  max_lines );
		
		foo<<setiosflags(ios::fixed);
	    foo<<setprecision(2)<<particlesPosAgent[j].pos.X<<' '<<particlesPosAgent[j].pos.Y<<' '
 		<<particlesPosAgent[j].heading.get_deg_180()<<"  error: "
		<<setprecision(5)<<ref_error<<"  line:"<<setprecision(0)<<lines.objectlist[0].pos.X
		<<lines.objectlist[0].pos.Y<<'\n';
	} 
  	foo.close();
  #endif*/

/*	   
    iNrLeft = checkParticles(lines,delta_xv);

	if( iNrLeft != -1 )
	{
     //averageParticles( particlesPosAgent, iNrLeft) 
		rp.pos = particlesPosAgent[iNrLeft].pos; 
		rp.heading = particlesPosAgent[iNrLeft].heading;
	}
	else
	{
	  rp.pos += delta_xv.pos.rotate(rp.heading);
      rp.heading += delta_xv.heading;
	}*/

    // resampleParticles( iNrLeft, delta_value);
 /*
	Vector visual_pos = rp.pos;
	MyAngle visual_heading = rp.heading;
	double ddphi;
	Vector ddxy;
	vis_optimiser->analyse (ddxy, ddphi, visual_pos, visual_heading, 
											lines, max_lines);
	double fnum = 16.0/(lines.objectlist.size()+4.0)+0.7;    
	Vector var_xy (trans_welt.X*trans_welt.X+fnum*225*hoch6( abs(ddxy.X)+1e-6 +7), 
			 trans_welt.Y*trans_welt.Y+fnum*225*hoch6( abs(ddxy.Y)+1e-6 +7)); 
    double var_phi = fnum*3.0461741978670859865/(ddphi*ddphi+1-6);  


	return true;
}*/	
/*
  #if DEBUG_LINES
	ofstream foo;
 	foo.open("lines.txt",ios::out|ios::app);
	foo<<" line: "<<setprecision(0)<<lines.objectlist[0].pos.X
		<<lines.objectlist[0].pos.Y<<'\n';
   	foo.close();
  #endif
*/