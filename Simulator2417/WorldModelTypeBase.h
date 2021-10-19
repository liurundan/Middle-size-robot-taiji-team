#ifndef _WorldModelTypeBase_
#define _WorldModelTypeBase_

#include "WorldModelType.h" 
#include <fstream>

class WorldModelTypeBase:public WorldModelType 
{
public:
	 WorldModelTypeBase ( ) throw ( std::bad_alloc);
    ~WorldModelTypeBase () throw ();
    
     int get_own_half () const throw ();
   
    const RobotLocation& get_slfilter_robot_location (MyTime&) const throw ();

  
    virtual RobotLocation get_robot (MyTime) const throw () =0;
   
    
    virtual MyTime get_timestamp_latest_update () const throw () =0;    
    void  set_own_half (int) throw ();
     
    const VisibleObjectList& get_visible_objects () throw () { return empty_vol; }
protected:
    FieldGeometry       field_geometry;     
 
    int                 own_half;           
  
    VisibleObjectList   empty_vol;
 
private:
	RobotLocation no_good_rloc;
};
#endif