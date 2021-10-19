#include "StdAfx.h"
#include "WorldModelTypeBase.h"

using namespace std;

WorldModelTypeBase::WorldModelTypeBase ( ) throw ( std::bad_alloc)
{
  own_half=-1;
  no_good_rloc.pos=no_good_rloc.vtrans=Vector(0,0);
  no_good_rloc.vrot=0;
  no_good_rloc.heading=MyAngle::zero;
  no_good_rloc.kick=false;
}

WorldModelTypeBase::~WorldModelTypeBase () throw () 
{
  
}
  
int  WorldModelTypeBase::get_own_half () const throw () { return own_half; }
 
void  WorldModelTypeBase::set_own_half (int h) throw () { own_half = ( h>=0 ? 1 : -1);}
 
 
  
const RobotLocation& WorldModelTypeBase::get_slfilter_robot_location (MyTime&) const throw () {
  return no_good_rloc;
}

 
