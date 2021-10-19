#include "StdAfx.h"
#include "OmniRobot.h"
#include "RobotProperties.h"
#include "WhiteBoard.h"
#include "Journal.h" 

#include <fstream>
#include <cmath>

using namespace std;
  
const double OmniRobot::RADIUS_m		= 22.5;
const double OmniRobot::vel_rate_m	    = 0.4233; 
const double OmniRobot::vel_inv_rate_m  = 2.3622;	// 300.0 / 127;

const double OmniRobot::max_wheel_vel   = 117.0;
const double OmniRobot::max_wheel_acc   = 167.0;
const double OmniRobot::max_wheel_deacc = 201.0;

OmniRobot::OmniRobot(  ConfigReader &vr) throw(RobotsException, bad_alloc)
{
	unsigned int iport = 6;
	unsigned int ibaud = 19200;
 
	if (vr.get("COM_PORT", iport)<=0) 
	{
	    throw InvalidConfigurationException ("port error!");
	} 
 	if (vr.get("COM_BAUdRATE", ibaud)<=0) 
	{ 
		throw InvalidConfigurationException ("comm rate error!");
	}

	ctrRobot	= new CSerialPort( iport, ibaud );
	ctrRobot->SetRThreshold(READLEN);

    m_command	 = new BYTE[SENDLEN];
    readbuffer   = new BYTE[READLEN];

    m_command[0] = (BYTE)(0xff);
	m_command[1] = (BYTE)(0x00);
	m_command[2] = (BYTE)(0x7f);
	m_command[3] = (BYTE)(0x7f);
	m_command[4] = (BYTE)(0x7f);
	m_command[5] = (BYTE)(0x7f);
	memset(readbuffer,'\0',sizeof(BYTE)*READLEN);
    
	for (unsigned int i=0; i<4; i++) 
	   wheel_vel[i] = 0.0;
	
    for (unsigned int j=0 ;j <READLEN; j++)
		rece_dsp_data[j] = 0.0;

	amIKicking     = false;
	kickBallTime   = 0;
	lastKickStart.update();
	lastKickEnd.update();

	robot_properties.max_velocity                  = 1.5;
	robot_properties.max_acceleration              = 0.5;
	robot_properties.max_deceleration              = 0.8;
	robot_properties.max_rotational_velocity       = 3.0;
	robot_properties.max_rotational_acceleration   = 0.5;
	robot_properties.max_rotational_deceleration   = 1.0;
	robot_properties.max_robot_radius              = 300;
	robot_properties.min_robot_radius              = 200;
	robot_properties.kicker_width                  = 290;
	robot_properties.kicker_distance               = 190;
	robot_properties.robot_width                   = 450;
	robot_properties.robot_length                  = 450;
	robot_properties.omnidirectional_drive         = true;
	robot_properties.drive_vector_delay            = 0;

	robot_properties.read (vr);
    
} 

OmniRobot::~OmniRobot() throw()
{
	delete [] m_command;
	delete [] readbuffer;
	delete ctrRobot;
}
 
void OmniRobot::set_drive_vector(DriveVector dv) throw ()
{
	bool kickCom = false;
	BYTE kickValue = 0;
	Time t_now; 
 
	DriveVector dv_to_set;
    dv_to_set = dv;	              //note!  in m/s
   
	if( !amIKicking && dv.kick && (fabs(lastKickEnd.diff_msec(t_now)) > 400 ))
	{
		kickCom    = true;
		kickValue  = dv.kickballtime;
		amIKicking = true;
		lastKickStart = t_now;
	}
	else if( amIKicking && ( fabs(lastKickStart.diff_msec(t_now) ) > 100 ) )
	{
	    kickCom = true;
		kickValue = 0;
		amIKicking = false;
		lastKickEnd = t_now;
	}
    
	bool robot_changes_dv = true;

	double _r_xm, _r_ym, _r_pm;

	if (robot_changes_dv)
	{
		get_realizable_vel_vec(  lastDriveState.vtrans.x, lastDriveState.vtrans.y, lastDriveState.vrot,
			dv.vtrans.x, dv.vtrans.y, dv.vrot, fabs(t_now.diff_msec(lastDriveStateTime)),
			_r_xm, _r_ym, _r_pm);
	}

	dv_to_set.vtrans.x = _r_xm;
	dv_to_set.vtrans.y = _r_ym;
	dv_to_set.vrot     = _r_pm;
 	lastDriveState     = dv_to_set;
	lastDriveStateTime = t_now;
      
	if( kickCom ) 	kickBallTime = kickValue;
     
	Time t_dv;
	SYSTEMTIME sendtime;
	bool comres = false;
    
	switch(dv_to_set.mode)
	{
		case ROBOTVELOCITY:
			comres = setVel_vec( dv_to_set.vtrans.x,dv_to_set.vtrans.y,dv_to_set.vrot,kickBallTime );
			break;
		case WHEELVELOCITY:
			comres = setVel_wheels( dv_to_set.vaux[0],dv_to_set.vaux[1],dv_to_set.vaux[2],dv_to_set.vaux[3],
									kickBallTime );
			break;
		default:
			break;
	}
     
	t_dv.set(sendtime);
	dv_to_set.kick = amIKicking;

	if (comres) 
	{
       if( WorldModel::is_main_world_model_available() ) 
	   {
	      Time tw_dv = t_dv;
	      tw_dv.add_msec (robot_properties.drive_vector_delay);
	      WorldModel::get_main_world_model().set_drive_vector (dv_to_set, tw_dv);
	   } 
	   else 
	   {
	      JWARNING("comres set but no world model available");
	   } 
	}
    
 	Time        t_odo;
	DriveVector odo;
    Angle		cpc;
    getDataFromControlBoard(odo,cpc);
	if( MWM.get_own_half() < 0 ) 
		cpc += M_PI;

	SYSTEMTIME systime;
	t_odo.get(systime);
	t_odo.set(systime);
	
	if (WorldModel::is_main_world_model_available()) 
	{
		WorldModel::get_main_world_model().set_odometry (odo, t_odo);
		WorldModel::get_main_world_model().set_compassdir(cpc,t_odo);
	} 
	else 
	{
		JWARNING("comres set but no world model available");
	} 
	
	RobotData rd;
	rd.robot_vel[0] = odo.vtrans.x;
	rd.robot_vel[1] = odo.vtrans.y;
	rd.robot_vel[2] = odo.vrot;
	rd.robot_vel[3] = 0.0;
	
	Time rd_time;
	fill_robot_data(readbuffer, dv_to_set, rd, rd_time);
	if (WorldModel::is_main_world_model_available()) 
	{
		WorldModel::get_main_world_model().set_robot_data(rd, rd_time);
	} 
 
} 

bool OmniRobot::setVel_wheels( double _v1, double _v2, double _v3,double _v4, BYTE kickPower )
{
   bool res;
   
   _v1 = 127.0 - _v1;
   if( _v1 > 254.0 )	_v1 = 254.0;
   if( _v1 < 1 )		_v1 = 1.0;

   _v2 = 127.0 - _v2;
   if( _v2 > 254.0 )	_v2 = 254.0;
   if( _v2 < 1 )		_v2 = 1.0;

   _v3 = 127.0 - _v3;
   if( _v3 > 254.0 )	_v3 = 254.0;
   if( _v3 < 1 )		_v3 = 1.0;

   _v4 = 127.0 - _v4;
   if( _v4 > 254.0 )	_v4 = 254.0;
   if( _v4 < 1 )		_v4 = 1.0;

   *(m_command+1) = (BYTE)(kickPower);
   *(m_command+2) = (BYTE)(_v1+0.5);
   *(m_command+3) = (BYTE)(_v2+0.5);
   *(m_command+4) = (BYTE)(_v3+0.5);
   *(m_command+5) = (BYTE)(_v4+0.5);

   res = ctrRobot->WriteToPort( m_command, SENDLEN );
   return res;
}

bool OmniRobot::setVel_vec( double _xm, double _ym, double _phim, BYTE kickPower /* = 0  */,
						   double *v1 /* =0 */, double* v2/* =0 */, 
						   double* v3/* =0 */,double* v4/* =0 */ )
{
	double vw1, vw2, vw3, vw4;
	cmptInvKin_RobotFrame(_xm, _ym, _phim, vw1, vw2, vw3,vw4);
	
	if (v1!=0) *v1=vw1;
	if (v2!=0) *v2=vw2;
	if (v3!=0) *v3=vw3;
	if (v4!=0) *v4=vw4;
	bool res = setVel_wheels( vw1, vw2, vw3, vw4,  kickPower );
    return res;
}

void OmniRobot::getDataFromControlBoard( DriveVector& odo, Angle& cpc )
{
	BYTE  small_camera_ball_dir = 0;
	BYTE  vel = 0;
    BYTE  sum = 0;
	
	for( unsigned int j=0 ;j <READLEN; j++)
		rece_dsp_data[j] = 0.0;
  	for( unsigned int i=0; i<4; i++ ) 
	   wheel_vel[i] = 0.0;

	const BYTE* buffer = ctrRobot->Lock();
	memcpy( readbuffer, buffer, READLEN*sizeof(BYTE) );
	ctrRobot->Unlock();

	for( unsigned int k=1; k<(READLEN-1); k++ )  
		sum += readbuffer[k];     
	
	sum = sum & (0x7f);    // 7bit for check code&& ( sum == readbuffer[ READLEN - 1 ] 
	
	small_camera_ball_dir = readbuffer[ READLEN - 2 ];
    WBOARD->set_small_camera_ball(small_camera_ball_dir);

	if( ( readbuffer[0] == 0xff ) && sum == readbuffer[ READLEN - 1 ] ) 
	{  
		for(unsigned int i=0; i<4; i++)
		{ 
			vel = *(readbuffer+2+i);
			if( vel >= 0x7f ) 
			{
				vel -= 0x7f;
				wheel_vel[i] = -vel;
			}
			else
				wheel_vel[i] = vel;
			rece_dsp_data[i] = wheel_vel[i]; 
		} 
		rece_dsp_data[4] = 360.0 - (readbuffer[1]<<1); 
		rece_dsp_data[5] = readbuffer[6];
		rece_dsp_data[5] *= 0.1;
	} 
	cmptDirectKin_RobotFrame( wheel_vel[0],wheel_vel[1],wheel_vel[2],wheel_vel[3],
		                      odo.vtrans.x, odo.vtrans.y, odo.vrot );
	cpc.set_deg(rece_dsp_data[4]);

}

void OmniRobot::fill_robot_data( const BYTE* src, const DriveVector &dv_set, RobotData &rd, Time &rd_time )
{
   rd.motor_vcc = rece_dsp_data[5]; 
   int compass_vavle = (int)(rece_dsp_data[4]);
   if( MWM.get_own_half() < 0 ) 
		compass_vavle += 180;
   Angle h = Angle::deg_angle(compass_vavle);
   rd.BoardID = h.get_deg();
   if (MWM.usb_camera_valid == false)
   {
	   rd.motors_on = false;
   }
   for (unsigned int k1=0; k1<4; k1++)
   {
	   rd.wheel_vel[k1] = wheel_vel[k1];
   }

   rd.dv_set = dv_set;
   SYSTEMTIME systime;
   rd_time.get(systime);
   rd_time.set(systime);
}

bool OmniRobot::get_realizable_vel_vec(double _act_xm, double _act_ym, double _act_phim, 
									   double _new_xm, double _new_ym, double _new_phim, double _dt_ms, 
									   double & _realizable_xm, double & _realizable_ym, 
									   double & _realizable_phim )
{
	double vw[4];
	cmptInvKin_RobotFrame(_new_xm, _new_ym, _new_phim, vw[0], vw[1], vw[2], vw[3]);
	
	double max_vel=0;
	for( int i=0; i<4; i++ ) 
	{
		if ( fabs(vw[i]) > fabs(max_vel) ) 
			max_vel = vw[i];
	}
 		
	double scale_vel=1.0;
	if (fabs(max_vel) > max_wheel_vel) 
		scale_vel = max_wheel_vel / fabs(max_vel);
	
	for (int j=0; j<4; j++)
		vw[j] = scale_vel * vw[j];
	
	double vw_old[4];
	cmptInvKin_RobotFrame(_act_xm, _act_ym, _act_phim, vw_old[0], vw_old[1], vw_old[2],vw_old[3]);

 	double vw_diff[4];
	double vw_max_diff = 0;

	for (int k=0; k<4; k++)
    {
		vw_diff[k] = vw[k] - vw_old[k];
		if (fabs(vw_diff[k]) > fabs(vw_max_diff)) 
			vw_max_diff = vw_diff[k];
    }
  
    double acc_scale = 1.0;
	
	bool acc[4];
	for(int i1=0; i1<4; i1++) 
		acc[i1] = (vw_diff[i1] * vw_old[i1]) > 0;
	
	if (acc[0] && acc[1] && acc[2] && acc[3] )  
    {
		if( fabs(vw_max_diff) > ( max_wheel_acc * (_dt_ms/1000.0) ) )
			acc_scale = (max_wheel_acc* (_dt_ms/1000.0)) / fabs(vw_max_diff);
    }
	else if (!acc[0] && !acc[1] && !acc[2] && !acc[3])  
    {
		if (fabs(vw_max_diff)>(max_wheel_deacc * (_dt_ms/1000.0)))
			acc_scale = (max_wheel_deacc* (_dt_ms/1000.0)) / fabs(vw_max_diff);
    }
	else  
    {
		double acc;
		if (max_wheel_deacc < max_wheel_acc) 
			acc = max_wheel_acc; 
		else  
			acc = max_wheel_deacc;
		if( fabs(vw_max_diff)>(acc * (_dt_ms/1000.0)) )
			acc_scale = (acc* (_dt_ms/1000.0)) / fabs(vw_max_diff);
    }

	double _realizable_vw[4];
	for (int i2=0; i2<4; i2++) 
    {
		_realizable_vw[i2] = vw_old[i2]+(acc_scale * vw_diff[i2]);
    }

    cmptDirectKin_RobotFrame(_realizable_vw[0],_realizable_vw[1] ,_realizable_vw[2],_realizable_vw[3],
				_realizable_xm, _realizable_ym, _realizable_phim );

	return true;

}

// v=[-127,127], xm=[-3,3]
void OmniRobot::cmptInvKin_RobotFrame( double _xm, double _ym, double _phim, 
									   double & _v1,double & _v2,double & _v3,double & _v4)
{
   	_v1 = ( -70.7 * ( _xm + _ym )   + _phim  * RADIUS_m ) * vel_rate_m;
	_v2 = ( 50.0 * _ym - 86.6 * _xm + _phim  * RADIUS_m ) * vel_rate_m;
	_v3 = ( 86.6 * _xm + 50.0 * _ym + _phim  * RADIUS_m ) * vel_rate_m;
	_v4 = ( 70.7 * ( _xm - _ym )    + _phim  * RADIUS_m ) * vel_rate_m;

}

void OmniRobot::cmptDirectKin_RobotFrame( double _v1,double _v2,double _v3, double _v4, 
										  double & _xm, double & _ym, double & _phim )
{
	_xm    = 0.005 * ( 0.5774 * ( _v3 - _v2 ) + 0.7072 * ( _v4 - _v1 ) ) * vel_inv_rate_m;
	_ym    = 0.005 * 0.8258 * ( _v2 +  _v3 - _v1 - _v4 ) * vel_inv_rate_m;
	_phim  = 0.5   * ( 0.0184  * ( _v1 + _v4 ) + 0.026 * ( _v2 + _v3 ) ) * vel_inv_rate_m;	

}
 
RobotProperties OmniRobot::get_robot_properties () const throw ()
{
	return robot_properties;
}


 