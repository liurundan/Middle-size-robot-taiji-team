#include "StdAfx.h"
#include <cmath>
#include "update_robot_location.h"

RobotLocation  update_robot_location (const RobotLocation& start, double dt) throw () 
{
  RobotLocation end;
  end.stuck=start.stuck;
  end.kick=start.kick;
  end.quality=start.quality;
  end.vrot=start.vrot;
  double turning = start.vrot*dt*1e-3;
  end.heading=start.heading+Angle::rad_angle (turning);
  end.vtrans=start.vtrans.rotate (Angle::rad_angle (turning));
  if (start.vrot==0) 
  {
    end.pos=start.pos+start.vtrans*dt;
  }
  else 
  {
    Vec vtrans_rob = start.vtrans.rotate (-start.heading);
    double sf = sin(turning)/start.vrot*1e3;
    double cf = (cos(turning)-1.0)/start.vrot*1e3;
    Vec trans_rob (sf*vtrans_rob.x+cf*vtrans_rob.y, -cf*vtrans_rob.x+sf*vtrans_rob.y);
    end.pos=start.pos+trans_rob.rotate (start.heading);
  }
  return end;
}

RobotLocation  flip_sides (const RobotLocation& src, int dir) throw ()
{
  if (dir>0) return src;
  RobotLocation dest = src;
  dest.pos*=-1;
  dest.heading+=Angle::half;
  dest.vtrans*=-1;
  dest.stuck.pos_of_stuck*=-1;
  return dest;
}

BallLocation  flip_sides (const BallLocation& src, int dir) throw () 
{
  if (dir>0) return src;
  BallLocation dest = src;
  dest.pos*=-1;
  dest.velocity*=-1;
  return dest;
}

ObstacleLocation  flip_sides (const ObstacleLocation& src, int dir) throw () 
{
  if (dir>0) return src;
  ObstacleLocation dest = src;
  std::vector<Vec>::iterator it = dest.pos.begin();
  std::vector<Vec>::iterator itend = dest.pos.end();
  while (it<itend)
    (*(it++)) *= -1;
  return dest;
}