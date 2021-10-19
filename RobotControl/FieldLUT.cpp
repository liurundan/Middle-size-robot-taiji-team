#include "StdAfx.h"
#include "FieldLUT.h"

#include "geometry.h"
#include <cmath>

#include <fstream>
#define TEST_FIELDLUT 0

 
using namespace std;

FieldLUT::~FieldLUT () throw () 
{
  delete [] array;
}

FieldLUT::FieldLUT (const FieldGeometry& fg, unsigned int c) throw (std::bad_alloc) : cell_size(c), array(NULL)
{
  x_res = static_cast<unsigned int>(ceil((0.5*fg.field_length+fg.goal_band_width)/static_cast<double>(cell_size)));
  y_res = static_cast<unsigned int>(ceil((0.5*fg.field_width+fg.side_band_width)/static_cast<double>(cell_size)));
  array = new double [4*x_res*y_res];
  grad = new  Vec [4*x_res*y_res];

  error_outside = (fg.side_band_width>fg.goal_band_width ? fg.side_band_width : fg.goal_band_width);
 
  double max_val = 1e100;

  double* arr_ptr=array;
  const double* end_arr_ptr=array+4*x_res*y_res;
  while (arr_ptr<end_arr_ptr) 
    *(arr_ptr++) = max_val;

  draw_line_segment (Vec (-0.5*fg.field_length,0.5*fg.goal_width ), Vec (-0.5*fg.field_length-500,0.5*fg.goal_width+500 ));
  draw_line_segment (Vec (0.5*fg.field_length,0.5*fg.goal_width), Vec (0.5*fg.field_length+500,0.5*fg.goal_width+500));
  draw_line_segment (Vec (-0.5*fg.field_length,-0.5*fg.goal_width), Vec(-0.5*fg.field_length-500,-0.5*fg.goal_width-500));
  draw_line_segment (Vec (0.5*fg.field_length,-0.5*fg.goal_width), Vec (0.5*fg.field_length+500,-0.5*fg.goal_width-500));
 
  draw_line_segment (Vec (-0.5*fg.field_length,0.5*fg.field_width ), Vec (0.5*fg.field_length,0.5*fg.field_width));   // Seitenlinie
  draw_line_segment (Vec (-0.5*fg.field_length,-0.5*fg.field_width ), Vec (0.5*fg.field_length,-0.5*fg.field_width));   // Seitenlinie
  draw_line_segment (Vec (0.5*fg.field_length,0.5*fg.field_width), Vec (0.5*fg.field_length,-0.5*fg.field_width));  // Torauslinie
  draw_line_segment (Vec (-0.5*fg.field_length,0.5*fg.field_width), Vec (-0.5*fg.field_length,-0.5*fg.field_width));  // Torauslinie
  
  draw_line_segment (Vec (0,-0.5*fg.field_width), Vec(0,0.5*fg.field_width));  

  if (fg.lineset_type == 0)
    draw_line_segment (Vec (0,-0.5*fg.field_width), Vec(0,0.5*fg.field_width));  
  else 
  {
    draw_line_segment (Vec (-0.5*fg.field_width,0), Vec(-430,0));     
    draw_line_segment (Vec (0.5*fg.field_width,0), Vec(430,0));     
  }
  if ((fg.goal_area_width>0) && (fg.goal_area_length>0)) 
  {
 	draw_line_segment (Vec(0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vec(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width ));  
	draw_line_segment (Vec(-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vec(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vec(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width ), 
					   Vec(0.5*fg.field_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vec(0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vec(0.5*fg.field_length,-0.5*fg.goal_area_width ));
 	draw_line_segment (Vec(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width ), 
					   Vec(-0.5*fg.field_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vec(-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vec(-0.5*fg.field_length,-0.5*fg.goal_area_width )); 
  }
  if ((fg.penalty_area_width>0) && (fg.penalty_area_length>0)) 
  {
    draw_line_segment (Vec(0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vec(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vec(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width ), Vec(0.5*fg.field_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vec(0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vec(0.5*fg.field_length,-0.5*fg.penalty_area_width ));
    draw_line_segment (Vec(-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vec(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vec(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width ), Vec(-0.5*fg.field_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vec(-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vec(-0.5*fg.field_length,-0.5*fg.penalty_area_width ));
  }
  if (fg.center_circle_radius>0 && fg.lineset_type==0) 
  {
    draw_arc (Vec(0,0), fg.center_circle_radius, Angle::zero, Angle::half);  // Mittelkreis-Segment
    draw_arc (Vec(0,0), fg.center_circle_radius, Angle::half, Angle::zero);  // Mittelkreis-Segment
  }
  if (fg.corner_arc_radius>0)
  {
    draw_arc (Vec(0.5*fg.field_length,0.5*fg.field_width), fg.corner_arc_radius, Angle::half, Angle::three_quarters);   
    draw_arc (Vec(-0.5*fg.field_length,0.5*fg.field_width), fg.corner_arc_radius, Angle::three_quarters, Angle::zero);    
    draw_arc (Vec(0.5*fg.field_length,-0.5*fg.field_width), fg.corner_arc_radius, Angle::quarter, Angle::half);   
    draw_arc (Vec(-0.5*fg.field_length,-0.5*fg.field_width ), fg.corner_arc_radius, Angle::zero, Angle::quarter);   
  }
  if (fg.penalty_marker_distance>0) 
  {
    draw_dot (Vec(0.5*fg.field_length-fg.penalty_marker_distance,0 ));   
    draw_dot (Vec(-0.5*fg.field_length+fg.penalty_marker_distance,0 ));   
  }
  if (fg.lineset_type==1 || fg.lineset_type==2) 
  {
    double dir = (fg.lineset_type==1 ? 1 : -1);
    draw_line_segment (Vec(0, dir*470), Vec(615, dir*-480));
    draw_line_segment (Vec(0, dir*470), Vec(-615, dir*-480));
    draw_line_segment (Vec(615, dir*-480), Vec(-615, dir*-480));
    draw_line_segment (Vec(0, dir*300), Vec(0,dir*-270));
    draw_line_segment (Vec(325, dir*-270), Vec(-325, dir*-270));
  }

  for ( unsigned int xi = 1; xi<2*x_res-1; xi++)
    for ( unsigned int yi=1; yi<2*y_res-1; yi++) 
	{   
      double lo = array[xi+2*x_res*yi-1+2*x_res];
      double lm = array[xi+2*x_res*yi-1];
      double lu = array[xi+2*x_res*yi-1-2*x_res];
      double ro = array[xi+2*x_res*yi+1+2*x_res];
      double rm = array[xi+2*x_res*yi+1];
      double ru = array[xi+2*x_res*yi+1-2*x_res];
      double mo = array[xi+2*x_res*yi+2*x_res];
      double mu = array[xi+2*x_res*yi-2*x_res];
      grad[xi+2*x_res*yi].x = 0.125*(-lo+ro-2*lm+2*rm-lu+ru)/static_cast<double>(cell_size);
      grad[xi+2*x_res*yi].y = 0.125*(lo-lu+2*mo-2*mu+ro-ru)/static_cast<double>(cell_size);
    }
  for (unsigned int xi1 = 1; xi1<2*x_res-1; xi1++) 
  {
    grad[xi1] = grad[xi1+2*x_res];
    grad[xi1+2*x_res*(2*y_res-1)] = grad[xi1+2*x_res*(2*y_res-2)];
  }
  for ( unsigned int yi1= 1; yi1<2*y_res-1; yi1++) 
  {
    grad[2*x_res*yi1] = grad[2*x_res*yi1+1];
    grad[2*x_res*yi1+2*x_res-1] = grad[2*x_res*yi1+2*x_res-2];
  }
  grad[0]=0.5*(grad[1]+grad[2*x_res]);
  grad[2*x_res-1]=0.5*(grad[2*x_res-2]+grad[4*x_res-1]);
  grad[2*x_res*(y_res-1)]=0.5*(grad[2*x_res*(y_res-2)]+grad[2*x_res*(y_res-1)+1]);
  grad[4*x_res*y_res-1]=0.5*(grad[4*x_res*y_res-2]+grad[4*x_res*y_res-2*x_res-1]);

#if TEST_FIELDLUT
  {
    ofstream foo ("fieldlut_f.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
	foo.put(static_cast<unsigned int>(255-array[xi+2*x_res*(2*y_res-yi-1)]/10));
  }
  {
    ofstream foo ("fieldlut_gx.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
	foo.put(static_cast<unsigned int>(127+127*grad[xi+2*x_res*(2*y_res-yi-1)].x));
  }
  {
    ofstream foo ("fieldlut_gy.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
	foo.put(static_cast<unsigned int>(127+127*grad[xi+2*x_res*(2*y_res-yi-1)].y));
  }
#endif

}

void FieldLUT::update (unsigned int xi, unsigned int yi, double v) 
{
  if (xi>=2*x_res || yi>=2*y_res)
    return;
  if (array[xi+2*x_res*yi]>v)
    array[xi+2*x_res*yi]=v;
}

double  FieldLUT::distance (const  Vec& p) const throw () 
{
  int xind = static_cast<int>(floor (p.x/cell_size)+x_res);
  int yind = static_cast<int>(floor (p.y/cell_size)+y_res);
  if ((xind<0) || (yind<0) || (xind>=2*static_cast<int>(x_res)) || (yind>=2*static_cast<int>(y_res)))
    return error_outside;   
  return array[static_cast<unsigned int>(xind+2*x_res*yind)];
}

Vec  FieldLUT::gradient (const Vec& p) const throw () 
{
  int xind = static_cast<int>(floor (p.x/cell_size)+x_res);
  int yind = static_cast<int>(floor (p.y/cell_size)+y_res);
  if (xind<0)  
    xind=0;
  if (xind>=2*static_cast<int>(x_res))
    xind=2*static_cast<int>(x_res)-1;
  if (yind<0)
    yind=0;
  if (yind>=2*static_cast<int>(y_res))
    yind=2*static_cast<int>(y_res)-1;
  return grad[static_cast<unsigned int>(xind+2*x_res*yind)];  
}

void FieldLUT::draw_line_segment (Vec start, Vec end) 
{
  LineSegment line (start, end);
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vec pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,line.distance (pp));
    }
}

void FieldLUT::draw_arc (Vec center, double radius, Angle start, Angle end) 
{
  MyArc arc (center, radius, start, end);
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vec pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,arc.distance (pp));
    }
}

void FieldLUT::draw_dot (Vec p) 
{
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vec pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,(p-pp).length());
    }
}
