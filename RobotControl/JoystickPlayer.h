#ifndef _JoystickPlayer_h_
#define _JoystickPlayer_h_

#include <stdexcept>
#include "PlayerType.h"
#include "PhysicalMotionModel.h"
#include "ConfigReader.h"
#include "RobotsException.h"

class JoystickPlayer : public PlayerType
{
public:
    JoystickPlayer ( ConfigReader&) throw (InvalidConfigurationException, std::bad_alloc);
    ~JoystickPlayer () throw ();

    DriveVector process_drive_vector (Time) throw ();
protected:
    PhysicalMotionModel motion;

private:
	double max_velocity;
    double max_rot_velocity;
    bool previously_changed;

    unsigned int x_axis, y_axis, phi_axis;      
    double x_diraxis, y_diraxis, phi_diraxis;      
    unsigned int kick_button, accel_button, decel_button;  

};

#endif