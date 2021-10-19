#ifndef _ErrorMinimiserWorldModel_
#define _ErrorMinimiserWorldModel_

#include "WorldModelTypeBase.h"
#include "ErrorMinimiserSL.h"
#include "OdometryContainer.h"
#include "VisualContainer.h"

class ErrorMinimiserWorldModel
{
public:
     ErrorMinimiserWorldModel ( ) throw ( bad_alloc);
     ~ErrorMinimiserWorldModel () throw ();

    RobotLocation get_robot (MyTime) const throw ();
   
    MyTime get_timestamp_latest_update () const throw ();
     void set_drive_vector (DriveVector, MyTime) throw ();
    void set_odometry (DriveVector, MyTime) throw ();
	void set_odometry(RobotLocation  ) throw();
    void set_visual_information (const VisibleObjectList&) throw ();
    void set_visual_information (const VisibleObject&, MyTime) throw ();
    void update_localisation () throw ();
    void reset () throw ();
    void reset (  Vector) throw ();
    void reset (  Vector, const MyAngle) throw ();
    const VisibleObjectList& get_visible_objects () throw ();
protected:
private:
    OdometryContainer odobox; 
 	FieldGeometry     FG;
    VisualContainer   visbox;                      
    ErrorMinimiserSL  sl; 
	MyTime latest_vis_timestamp;                   
 	
};
#endif
