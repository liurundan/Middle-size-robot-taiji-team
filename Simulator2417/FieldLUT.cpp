#include "StdAfx.h"
#include "FieldLUT.h"
#include "math.h"

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
  grad  = new  Vector [4*x_res*y_res];

  error_outside = (fg.side_band_width>fg.goal_band_width ? fg.side_band_width : fg.goal_band_width);

  double max_val = 1e100;

  double* arr_ptr=array;
  const double* end_arr_ptr=array+4*x_res*y_res;
  while (arr_ptr<end_arr_ptr) 
    *(arr_ptr++) = max_val;

  draw_line_segment (Vector (-0.5*fg.field_length,0.5*fg.goal_width ), Vector (-0.5*fg.field_length-500,0.5*fg.goal_width+500 ));
  draw_line_segment (Vector (0.5*fg.field_length,0.5*fg.goal_width), Vector (0.5*fg.field_length+500,0.5*fg.goal_width+500));
  draw_line_segment (Vector (-0.5*fg.field_length,-0.5*fg.goal_width), Vector(-0.5*fg.field_length-500,-0.5*fg.goal_width-500));
  draw_line_segment (Vector (0.5*fg.field_length,-0.5*fg.goal_width), Vector (0.5*fg.field_length+500,-0.5*fg.goal_width-500));
 
  draw_line_segment (Vector (-0.5*fg.field_length,0.5*fg.field_width ), Vector (0.5*fg.field_length,0.5*fg.field_width));   // Seitenlinie
  draw_line_segment (Vector (-0.5*fg.field_length,-0.5*fg.field_width ), Vector (0.5*fg.field_length,-0.5*fg.field_width));   // Seitenlinie
  draw_line_segment (Vector (0.5*fg.field_length,0.5*fg.field_width), Vector (0.5*fg.field_length,-0.5*fg.field_width));  // Torauslinie
  draw_line_segment (Vector (-0.5*fg.field_length,0.5*fg.field_width), Vector (-0.5*fg.field_length,-0.5*fg.field_width));  // Torauslinie
  
  draw_line_segment (Vector (0,-0.5*fg.field_width), Vector(0,0.5*fg.field_width));       
   
  if((fg.goal_area_width>0) && (fg.goal_area_length>0)) 
  {
	draw_line_segment (Vector(0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vector(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width ));  
	draw_line_segment (Vector(-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vector(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vector(0.5*fg.field_length-fg.goal_area_length,0.5*fg.goal_area_width ), 
					   Vector(0.5*fg.field_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vector(0.5*fg.field_length-fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vector(0.5*fg.field_length,-0.5*fg.goal_area_width ));
 	draw_line_segment (Vector(-0.5*fg.field_length+fg.goal_area_length,0.5*fg.goal_area_width ), 
					   Vector(-0.5*fg.field_length,0.5*fg.goal_area_width ));
	draw_line_segment (Vector(-0.5*fg.field_length+fg.goal_area_length,-0.5*fg.goal_area_width ), 
					   Vector(-0.5*fg.field_length,-0.5*fg.goal_area_width )); 
 }
   
  if ((fg.penalty_area_width>0) && (fg.penalty_area_length>0)) 
  {
    draw_line_segment (Vector(0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vector(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vector(0.5*fg.field_length-fg.penalty_area_length,0.5*fg.penalty_area_width ), Vector(0.5*fg.field_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vector(0.5*fg.field_length-fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vector(0.5*fg.field_length,-0.5*fg.penalty_area_width ));
    draw_line_segment (Vector(-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vector(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vector(-0.5*fg.field_length+fg.penalty_area_length,0.5*fg.penalty_area_width ), Vector(-0.5*fg.field_length,0.5*fg.penalty_area_width ));
    draw_line_segment (Vector(-0.5*fg.field_length+fg.penalty_area_length,-0.5*fg.penalty_area_width ), Vector(-0.5*fg.field_length,-0.5*fg.penalty_area_width ));
  }
  if (fg.center_circle_radius>0  ) 
  {
    draw_arc (Vector(0,0), fg.center_circle_radius, MyAngle::zero, MyAngle::half);  // Mittelkreis-Segment
    draw_arc (Vector(0,0), fg.center_circle_radius, MyAngle::half, MyAngle::zero);  // Mittelkreis-Segment
  }
  if (fg.corner_arc_radius>0) 
  {
    draw_arc (Vector(0.5*fg.field_length,0.5*fg.field_width), fg.corner_arc_radius, MyAngle::half, MyAngle::three_quarters);   
    draw_arc (Vector(-0.5*fg.field_length,0.5*fg.field_width), fg.corner_arc_radius, MyAngle::three_quarters, MyAngle::zero);    
    draw_arc (Vector(0.5*fg.field_length,-0.5*fg.field_width), fg.corner_arc_radius, MyAngle::quarter, MyAngle::half);   
    draw_arc (Vector(-0.5*fg.field_length,-0.5*fg.field_width ), fg.corner_arc_radius, MyAngle::zero, MyAngle::quarter);   
  }
  if (fg.penalty_marker_distance>0)
  {
    draw_dot (Vector(0.5*fg.field_length-fg.penalty_marker_distance,0 ));   
    draw_dot (Vector(-0.5*fg.field_length+fg.penalty_marker_distance,0 ));   
  }
   
  for (unsigned int xi = 1; xi<2*x_res-1; xi++)
    for (unsigned int yi=1; yi<2*y_res-1; yi++) 
	{   
      double lo = array[xi+2*x_res*yi-1+2*x_res];
      double lm = array[xi+2*x_res*yi-1];
      double lu = array[xi+2*x_res*yi-1-2*x_res];
      double ro = array[xi+2*x_res*yi+1+2*x_res];
      double rm = array[xi+2*x_res*yi+1];
      double ru = array[xi+2*x_res*yi+1-2*x_res];
      double mo = array[xi+2*x_res*yi+2*x_res];
      double mu = array[xi+2*x_res*yi-2*x_res];
      grad[xi+2*x_res*yi].X = 0.125*(-lo+ro-2*lm+2*rm-lu+ru)/static_cast<double>(cell_size);
      grad[xi+2*x_res*yi].Y = 0.125*(lo-lu+2*mo-2*mu+ro-ru)/static_cast<double>(cell_size);
    }
  
  for (unsigned int xi1 = 1; xi1<2*x_res-1; xi1++) 
  {
    grad[xi1] = grad[xi1+2*x_res];
    grad[xi1+2*x_res*(2*y_res-1)] = grad[xi1+2*x_res*(2*y_res-2)];
  }
  for (unsigned int yi= 1; yi<2*y_res-1; yi++) 
  {
    grad[2*x_res*yi] = grad[2*x_res*yi+1];
    grad[2*x_res*yi+2*x_res-1] = grad[2*x_res*yi+2*x_res-2];
  }
  
  grad[0]=(grad[1]+grad[2*x_res])*0.5;
  grad[2*x_res-1]=(grad[2*x_res-2]+grad[4*x_res-1])*0.5;
  grad[2*x_res*(y_res-1)]=(grad[2*x_res*(y_res-2)]+grad[2*x_res*(y_res-1)+1])*0.5;
  grad[4*x_res*y_res-1]=(grad[4*x_res*y_res-2]+grad[4*x_res*y_res-2*x_res-1])*0.5;

#if TEST_FIELDLUT
  {
    ofstream foo ("fieldlut_f22.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
  		  foo.put(static_cast<unsigned int>(255-array[xi+2*x_res*(2*y_res-yi-1)]/10));
  }
  {
    ofstream foo ("fieldlut_gx22.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
	foo.put(static_cast<unsigned int>(127+127*grad[xi+2*x_res*(2*y_res-yi-1)].X));
  }
  {
    ofstream foo ("fieldlut_gy22.pgm");
    foo << "P5\n" << 2*y_res << ' ' << 2*x_res << " 255\n";
    for (unsigned int xi = 0; xi<2*x_res; xi++)
      for (unsigned int yi = 0; yi<2*y_res; yi++)
	foo.put(static_cast<unsigned int>(127+127*grad[xi+2*x_res*(2*y_res-yi-1)].Y));
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

double  FieldLUT::distance (const  Vector& p) const throw () 
{
  int xind = static_cast<int>(floor (p.X/cell_size)+x_res);
  int yind = static_cast<int>(floor (p.Y/cell_size)+y_res);
  if ((xind<0) || (yind<0) || (xind>=2*static_cast<int>(x_res)) || (yind>=2*static_cast<int>(y_res)))
    return error_outside;   
  return array[static_cast<unsigned int>(xind+2*x_res*yind)];
}

 Vector  FieldLUT::gradient (const Vector& p) const throw () 
 {
  int xind = static_cast<int>(floor (p.X/cell_size)+x_res);
  int yind = static_cast<int>(floor (p.Y/cell_size)+y_res);
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

void FieldLUT::draw_line_segment (Vector start, Vector end) 
{
  LineSegment line (start, end);
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vector pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), 
		  (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,line.distance (pp));
    }
}

void FieldLUT::draw_arc (Vector center, double radius, MyAngle start, MyAngle end) 
{
  MyArc arc (center, radius, start, end);
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vector pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), 
		  (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,arc.distance (pp));
    }
}

void FieldLUT::draw_dot (Vector p) 
{
  for (unsigned int xi=0; xi<2*x_res; xi++)
    for (unsigned int yi=0; yi<2*y_res; yi++) 
	{
      Vector pp ((static_cast<double>(xi)-static_cast<double>(x_res)+0.5)*static_cast<double>(cell_size), 
		  (static_cast<double>(yi)-static_cast<double>(y_res)+0.5)*static_cast<double>(cell_size));
      update (xi,yi,(p-pp).getMagnitude());
    }
}

