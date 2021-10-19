#include "StdAfx.h"
#include "VisualPositionOptimiser.h"
#include "math.h"
#include <fstream>
#include <iomanip>

using namespace std;

#define DEBUG_VISUALOPTIMISER 0

VisualPositionOptimiser::VisualPositionOptimiser ( const FieldLUT& fl, 
double c1, double d1) throw () : the_field_lut (fl), c(c1), c2(c1*c1), d2(d1*d1), weights(300) 
{;}

double VisualPositionOptimiser::calculate_distance_weights (const VisibleObjectList& vis,
															unsigned  int max_lines) throw () 
{
  unsigned int nlines=(max_lines>vis.objectlist.size() ? vis.objectlist.size() : max_lines);
  double ret=0;
  weights.resize (nlines);
  double reference = 1500*1500;
  vector<VisibleObject>::const_iterator vis_it = vis.objectlist.begin();
  vector<double>::iterator weight_it = weights.begin();
  for (unsigned int i=0; i<nlines; i++) 
  {
    ret += ((*weight_it) = (reference+d2)/(d2+vis_it->pos.getSqrMagnitude()));
    weight_it++;
    vis_it++;
  }
  return ret;
}

void VisualPositionOptimiser::error (double& err, double& dx, double& dy, double& dphi,
									 double x, double y, double phi, 
									 const VisibleObjectList& vis, unsigned int max_lines) const throw () {
  unsigned int nlines=(max_lines>vis.objectlist.size() ? vis.objectlist.size() : max_lines);
  double sinphi = sin(phi);
  double cosphi = cos(phi);
  static err2 = 0;
  err=dx=dy=dphi=0;
  vector<VisibleObject>::const_iterator vis_it = vis.objectlist.begin();
  vector<VisibleObject>::const_iterator vis_itend = vis.objectlist.begin()+nlines;
  vector<double>::const_iterator weight_it = weights.begin();
  while (vis_it < vis_itend) 
  {
    Vector vp( x+cosphi*vis_it->pos.X-sinphi*vis_it->pos.Y, // mm unit
		       y+sinphi*vis_it->pos.X+cosphi*vis_it->pos.Y );   // gesehene weise Linie in Weltkoordinaten
    double dist = the_field_lut.distance (vp);    
    double ef = c2+dist*dist;
    err+=(*weight_it)*(1-c2/ef);
    double derrddist = (2*c2*dist)/(ef*ef);    
    Vector ddistdpos = the_field_lut.gradient (vp);    
    dx+=(*weight_it)*derrddist*ddistdpos.X;   // Gradient: x-Anteil
    dy+=(*weight_it)*derrddist*ddistdpos.Y;   // Gradient: y-Anteil
    dphi+=(*weight_it)*derrddist*(ddistdpos.X*(-sinphi*vis_it->pos.X-cosphi*vis_it->pos.Y)+
		                          ddistdpos.Y*(cosphi*vis_it->pos.X-sinphi*vis_it->pos.Y));   // Gradient: phi-Anteil
    vis_it++;
    weight_it++;
  }
  err2 = err;
}

double VisualPositionOptimiser::optimise (Vector& xy, MyAngle& h, const VisibleObjectList& vis, 
										  unsigned int niter, unsigned int max_lines) const throw ()
 {
  double param [3];   
  param[0]=xy.X;
  param[1]=xy.Y;
  param[2]=h.get_rad();
  double grad [3];  
  double stepwidth [3] = {40,40,0.1};  
  double latest_grad [3] = {0,0,0};   
  double err;  // error value

  for (unsigned int i=0; i<niter; i++) 
  {
    error (err, grad[0], grad[1], grad[2], param[0], param[1], param[2], vis, max_lines);
     
    double delta_angle = (h-MyAngle::rad_angle(param[2])).get_rad_pi();
    err += 2.5*delta_angle*delta_angle;
    grad[2] += 5*delta_angle;

#if DEBUG_VISUALOPTIMISER
	ofstream foo;
	static int i = 0;
	foo.open("VisualOptimiser.txt",ios::out|ios::app);
	foo<<setiosflags(ios::fixed);
 	foo<<setprecision(2) << "VisualOptimiser: "<<i++<<' '
		<< param[0] << ' ' << param[1] << ' ' << param[2]*180/M_PI << ' ' << 
		grad[0] << ' ' << grad[1] << ' ' << grad[2] << '\n';
	foo.close();
#endif
    // 根据梯度值优化的粒子参数
    for (unsigned int j=0; j<3; j++) 
	{
       if (grad[j]==0)
	    latest_grad[j]=0;
      else 
	  {
		 
		if (grad[j]*latest_grad[j]>0)
		  stepwidth[j]*=1.2;
		else if (grad[j]*latest_grad[j]<0)
		  stepwidth[j]*=0.5;
		latest_grad[j]=grad[j];

 		if (grad[j]>0)
		  param[j]-=stepwidth[j];
		else if (grad[j]<0)
		  param[j]+=stepwidth[j];
      }
    }
  }
  xy.X=param[0];
  xy.Y=param[1];
  h.set_rad(param[2]);
  return err;
}

double VisualPositionOptimiser::analyse (Vector& hxy, double& hphi, Vector xy, 
										 MyAngle h, const VisibleObjectList& vis, unsigned int max_lines) const throw () {
  unsigned int nlines=(max_lines>vis.objectlist.size() ? vis.objectlist.size() : max_lines);
  double phi = h.get_rad();
  double sinphi = sin (phi);
  double cosphi = cos(phi);
  
  double err=0;  
  hxy.X=hxy.Y=hphi=0;
  double derr;   
  double dderr;   
  double dist;   
  Vector ddist;   
  Vector pos;
  Vector dposdphi;
  Vector ddposdphi2;
  
  double t1, t2;   

  vector<VisibleObject>::const_iterator vis_it = vis.objectlist.begin();
  vector<VisibleObject>::const_iterator vis_itend = vis.objectlist.begin()+nlines;
  vector<double>::const_iterator weight_it = weights.begin();
  while (vis_it < vis_itend) {
    pos.X = xy.X+cosphi*vis_it->pos.X-sinphi*vis_it->pos.Y;
    pos.Y = xy.Y+sinphi*vis_it->pos.X+cosphi*vis_it->pos.Y;   
    dist = the_field_lut.distance (pos);   
    err+=(*weight_it)*(1-c2/(c2+dist*dist));
    if (dist<2*c)
	{   
      derr=dist/c2;   
      dderr=1/c2;   
      ddist = the_field_lut.gradient (pos);   
      dposdphi.X = -sinphi*vis_it->pos.X-cosphi*vis_it->pos.Y;
      dposdphi.Y = cosphi*vis_it->pos.X-sinphi*vis_it->pos.Y;
      ddposdphi2.X = -cosphi*vis_it->pos.X+sinphi*vis_it->pos.Y;
      ddposdphi2.Y = -sinphi*vis_it->pos.X-cosphi*vis_it->pos.Y;
      
      hxy.X += (*weight_it)*dderr*ddist.X*ddist.X;
      hxy.Y += (*weight_it)*dderr*ddist.Y*ddist.Y;
      t1 = ddist.X*dposdphi.X+ddist.Y*dposdphi.Y;
      t2 = ddist.X*ddposdphi2.X+ddist.Y*ddposdphi2.Y;
      hphi += (*weight_it)*(dderr*t1*t1+derr*t2);
    }
    vis_it++;
    weight_it++;
  }
  return err;
}

double VisualPositionOptimiser::error(Vector pos, MyAngle ang, const VisibleObjectList &vis,
									  unsigned int max_lines )
{
  unsigned int nlines=(max_lines>vis.objectlist.size() ? vis.objectlist.size() : max_lines);
  double phi = ang.get_rad();

  double sinphi = sin(phi);
  double cosphi = cos(phi);
  double x = pos.X;
  double y = pos.Y;
  double err = 0;
  vector<VisibleObject>::const_iterator vis_it = vis.objectlist.begin();
  vector<VisibleObject>::const_iterator vis_itend = vis.objectlist.begin()+nlines;
  vector<double>::const_iterator weight_it = weights.begin();
  while (vis_it < vis_itend) 
  {
    Vector vp( x+cosphi*vis_it->pos.X-sinphi*vis_it->pos.Y, // mm unit
		       y+sinphi*vis_it->pos.X+cosphi*vis_it->pos.Y );    
    double dist = the_field_lut.distance (vp);    
    //double ef = c2+dist*dist;
    //err+=(*weight_it)*(1-c2/ef);
    err += dist;
    vis_it++;
    weight_it++;
  }
  if( nlines != 0 )
  {
    err = err / (double)(nlines);
  }
  return err;
}
