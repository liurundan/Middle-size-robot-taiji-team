#include "StdAfx.h"
#include "update_robot_location.h"
#include "math.h"

RobotLocation update_robot_location (  RobotLocation& start, double dt) throw () 
{
  RobotLocation end;
  end.stuck = start.stuck;
  end.kick  = start.kick;
  end.quality = start.quality;
  end.vrot    = start.vrot;
  double turning(start.vrot*dt*1e-3);
  end.heading=start.heading+MyAngle::rad_angle (turning);
  end.vtrans=start.vtrans.rotate (MyAngle::rad_angle (turning).get_deg());
  if (start.vrot==0) 
  {
     end.pos=start.pos+start.vtrans*dt;
  } else 
  {

	Vector vtrans_rob = start.vtrans.rotate (-start.heading.get_deg());
    double sf =  sin(turning)/start.vrot*1e3;
    double cf = ( cos(turning)-1.0)/start.vrot*1e3;
    Vector trans_rob (sf*vtrans_rob.X+cf*vtrans_rob.Y, -cf*vtrans_rob.X+sf*vtrans_rob.Y);
    end.pos=start.pos+trans_rob.rotate (start.heading.get_deg());
  }
  return end;
}

RobotLocation  flip_sides (const RobotLocation& src, int dir) throw () 
{
  if (dir>0) return src;
  RobotLocation dest = src;
  dest.pos =dest.pos*-1;
  dest.heading+=MyAngle::half;
  dest.vtrans=dest.vtrans*-1;
  dest.stuck.pos_of_stuck =dest.stuck.pos_of_stuck*-1;
  return dest;
}