#include "StdAfx.h"
#include "RobotPositionKalmanFilter.h"
#include "math.h"
#include <fstream>
#include <iomanip>

using namespace std;

#define DEBUG_KFILTER 0

#define ZWEIPI 6.283185307179586232

void RobotPositionKalmanFilter::update (Vector delta_pos, MyAngle delta_heading, 
										Vector vis_pos, MyAngle vis_heading, Vector var_vis_pos,
										double var_vis_heading) throw () 
{
  // 卡尔曼增益为：k=v1/(v1+v2)，v1对应上个周期的估计方差，v2对应新息的方差
  // pos.x = pos.x + k * ( vis_pos.x - pos.x )
  // v2 = ( 1 - k ) * v1; 
  update (delta_pos, delta_heading, true);
 
  #if DEBUG_KFILTER
  ofstream foo;
  static int i=0;
  foo.open("KF.txt",ios::out|ios::app);
   
  foo<<setiosflags(ios::fixed);
  
  foo<<"KFilter  视觉信息:"<<i++<<":"<<setprecision(2)<< vis_pos.X << ' ' 
	  << vis_pos.Y << ' ' <<   vis_heading.get_deg() 
	  << ' ' << sqrt(var_vis_pos.X) << ' ' << sqrt(var_vis_pos.Y) 
	  << ' ' << sqrt(var_vis_heading) << '\n';
  foo<<"KFilter  预测信息:"<<setprecision(2)<< pos.X << ' ' 
	  << pos.Y << ' ' <<   heading*180/M_PI 
	  << ' ' << sqrt(var_pos.X) << ' ' << sqrt(var_pos.Y) 
	  << ' ' << sqrt(var_heading) << '\n';
  foo.close();
  #endif
  
  double v1, v2;
  v1=var_pos.X;
  v2=var_vis_pos.X;
  pos.X = (v2*pos.X+v1*vis_pos.X)/(v1+v2);
  var_pos.X = (v1*v2)/(v1+v2);
  
  v1=var_pos.Y;
  v2=var_vis_pos.Y;
  pos.Y = (v2*pos.Y+v1*vis_pos.Y)/(v1+v2);
  var_pos.Y = (v1*v2)/(v1+v2);
  
  v1=var_heading;
  v2=var_vis_heading;
  double vis_head=vis_heading.get_rad();
  
  if (heading-vis_head<=-M_PI)
   vis_head-=ZWEIPI;
  heading = (v2*heading+v1*vis_head)/(v1+v2);
  var_heading = (v1*v2)/(v1+v2);
  
  if (heading>M_PI)
    heading-=ZWEIPI;
  if (heading<=-M_PI)
    heading+=ZWEIPI; 
  
}

void RobotPositionKalmanFilter::update (Vector delta_pos, MyAngle delta_heading, bool vis_available) throw ()
{
 #if DEBUG_KFILTER
 ofstream foo;
  
 foo.open("KF.txt",ios::out|ios::app);
 foo<<setiosflags(ios::fixed);  
 foo<<"KFilter前次融合结果:"<<setprecision(2)<< pos.X << ' ' << pos.Y 
	 << ' ' << heading*180/M_PI << ' ' << sqrt(var_pos.X) 
	  << ' ' << sqrt(var_pos.Y) << ' ' << sqrt(var_heading) << '\n';
 foo.close();
 #endif
 
 delta_pos=delta_pos.rotate(MyAngle::rad_angle(heading));  
 
 Vector std_delta_pos (max (0.375*fabs(delta_pos.X),100.0), max (0.375*fabs(delta_pos.Y),100.0));
  if (vis_available) 
    delta_pos=delta_pos*0.75;
  // aehnliche Ueberlegung bei der Rotation
  double delta_head_pi = delta_heading.get_rad();
  if (delta_head_pi>M_PI)
   delta_head_pi-=ZWEIPI;

  double std_delta_head = ( max (0.375*fabs(delta_head_pi),0.048));
  double delta_head = (vis_available ? 0.75 : 1.0)*delta_head_pi;

  // Varianzen und Positionen miteinander verrechnen:
  // p(k|k-1) = Ap(k-1|k-1)A' + Q
  var_pos.X+=std_delta_pos.X*std_delta_pos.X;
  pos.X+=delta_pos.X;
  var_pos.Y+=std_delta_pos.Y*std_delta_pos.Y;
  pos.Y+=delta_pos.Y;
  var_heading+=std_delta_head;
  heading += delta_head;
  if (heading>M_PI)
    heading-=ZWEIPI;
  if (heading<=-M_PI)
    heading+=ZWEIPI;
 
}

void RobotPositionKalmanFilter::set (Vector p, MyAngle h, Vector vp, double vh) throw () {
  pos=p;
  heading=h.get_rad();
  if (heading>M_PI)
    heading-=ZWEIPI;
  var_pos=vp;
  var_heading=vh;
}

double RobotPositionKalmanFilter::get (Vector& p, MyAngle& h) const throw () {
  p=pos;
  h.set_rad(heading);
  double sum_var = var_pos.X+var_pos.Y+1e6*var_heading;  // Annahme 0.1 rad Abweichung wiegen soviel wie 10 cm
  return 2e5/(2e5+sum_var);  // plausible Unschaerfefunktion
}

Vector RobotPositionKalmanFilter::get_position_variance () const throw () {
  return var_pos;
}

double RobotPositionKalmanFilter::get_heading_variance () const throw () 
{
  return var_heading;
}

void RobotPositionKalmanFilter::mirror () throw () 
{
  pos=pos*-1;
  heading+=M_PI;
  if (heading>M_PI)
    heading-=ZWEIPI;
}