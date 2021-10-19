#ifndef _OMNIROBOT_H
#define _OMNIROBOT_H

#include "RobotType.h"
#include "ConfigReader.h"
#include "WorldModel.h"
#include "SerialPort.h"
#include "Time.h"
#include <iostream>

#ifndef READLEN
#define     READLEN      9    // if READLEN is changed, in robotdata.h 
#endif 
 
#define     SENDLEN      6    // there is another constant has to be changed
 
class OmniRobot : public RobotType
{

public:
    OmniRobot(  ConfigReader& vr) throw(RobotsException, std::bad_alloc);
    ~OmniRobot() throw();

    RobotProperties get_robot_properties () const throw ();
    
    void set_drive_vector (DriveVector dv) throw ();
    void fill_robot_data( const BYTE* src, const DriveVector &dv_set, RobotData &target, Time &target_time );

	
	bool get_realizable_vel_vec(double _act_xm, double _act_ym, double _act_phim,  
			      double _new_xm, double _new_ym, double _new_phim, double _dt_ms,
			      double & _realizable_xm, double & _realizable_ym, double & _realizable_phim);

	void cmptDirectKin_RobotFrame(	double _v1,double _v2,double _v3, double _v4,
									double & _xm, double & _ym, double & _phim );
	void cmptInvKin_RobotFrame( double _xm, double _ym, double _phim, 
								double & _v1,double & _v2,double & _v3,double & _v4);

	bool setVel_wheels( double _v1, double _v2, double _v3,double _v4, BYTE kickPower = 0 );
	
	bool setVel_vec( double _xm, double _ym, double _phim, BYTE kickPower = 0, 
		             double *v1 =0, double* v2=0, double* v3=0,double* v4=0 );
    
	void getDataFromControlBoard(DriveVector& odo, Angle& cpc);

protected:
	CSerialPort*        ctrRobot;
    BYTE*				m_command;
	BYTE*				readbuffer;
    
    double              wheel_vel[4];
    double				rece_dsp_data[READLEN]; 

	bool				blocking_communication;
    bool				amIKicking;
    Time				lastKickStart;
    Time				lastKickEnd;

    DriveVector			lastDriveState;
    Time				lastDriveStateTime;

    RobotProperties		robot_properties;
    double				kickBallTime;

    static const double		RADIUS_m;
	static const double		vel_rate_m;
	static const double		vel_inv_rate_m;
	static const double		max_wheel_vel;
	static const double     max_wheel_acc;
	static const double     max_wheel_deacc;
    
};
 
#endif
