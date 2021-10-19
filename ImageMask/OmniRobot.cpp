#include "StdAfx.h"
#include "OmniRobot.h"
#include "RobotProperties.h"
//#include "Journal.h" 

#include <fstream>
#include <cmath>

using namespace std;
  
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
    
	for (int i=0; i<4; i++) 
	{
	   v[i] = 0.0;
	   wheel_vel[i] = 0.0;
	}
	
    for (unsigned int j=0 ;j <READLEN; j++)
	{
		rece_dsp_data[j] = 0.0;
    } 

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
	bool comres = false;

	DriveVector dv_to_set = dv;
    dv_to_set.vtrans = dv.vtrans * 1e-2;
  	double MaxVel = 80.0; // 127
	double rate   = 0.4233; // 127.0 / 300.0;
    double RADIUS = 22.5;
	double Vx     = dv.vtrans.x;
	double Vy     = dv.vtrans.y;
	double w      = dv.vrot;
  
	v[0] = ( -0.707 * ( Vx + Vy ) + w  * RADIUS ) * rate;
	v[1] = ( 0.5 * Vy - 0.866 * Vx + w  * RADIUS) * rate;
	v[2] = ( 0.866 * Vx + 0.5 * Vy + w  * RADIUS) * rate;
	v[3] = ( 0.707 * ( Vx - Vy ) + w  * RADIUS)   * rate;
 
	double Vmax = fabs( v[0] );
	for( unsigned int i = 1; i < 4; i++ )
		if( Vmax < fabs( v[i] ) )
			Vmax = fabs( v[i] );               
 
	if( Vmax > MaxVel )
	{
		for( i=0;i<4;i++)
			v[i] = v[i] * MaxVel / Vmax;
		
	}
	for( i = 0; i < 4; i++ )
	{
		v[i] = 127.0 - v[i];
		if( v[i] > 254.0 )
			v[i] = 254.0;
		if( v[i] < 1 )
			v[i] = 1.0;
 	    *(m_command+i+2) = (BYTE)(v[i]+0.5);
		if (Vx == 0.0 && Vy == 0.0 && w == 0.0 ) 
		{
            *(m_command+i+2) = 0x7f;
		}
	}
	 
	amIKicking   = dv.kick;
	kickBallTime = dv.kickballtime;
	if(amIKicking)
	{
		*(m_command+1) = kickBallTime;
		kickBallTime = 0;
		amIKicking   = false;
	}
	else
		*(m_command+1) = 0x00;
 
	Time t_dv;
 	comres = ctrRobot->WriteToPort( m_command, SENDLEN );
    
	if (comres) 
	{
    /*   if( WorldModel::is_main_world_model_available() ) 
	   {
	      Time tw_dv = t_dv;
	      tw_dv.add_msec (robot_properties.drive_vector_delay);
	      WorldModel::get_main_world_model().set_drive_vector (dv_to_set, tw_dv);
	   } 
	   else 
	   {
	      JWARNING("comres set but no world model available");
	   } */
	}

	if( comres ) 
	{
		double vx = 0, vy = 0, w = 0;
		double rate = 2.3622; // 300.0 / 127;
		BYTE   sum = 0;

		Time        t_odo;
		DriveVector odo;
		Angle cpc;
		//for( unsigned int i=0; i < 6; i++ )
		//rece_dsp_data[i]=0;
 
		for( int k=1; k<(READLEN-1); k++ )  
			sum += readbuffer[k];     
		
		sum = sum & (0x7f);  // 7bit for check code

		BYTE bdir[] = { 0, 0, 0, 0 };
        
		const BYTE* buffer = ctrRobot->Lock();
		memcpy(readbuffer,buffer,READLEN*sizeof(BYTE));
		ctrRobot->Unlock();

		if( ( readbuffer[0] == 0xff ) && ( sum == readbuffer[ READLEN - 1 ] ) ) 
		{  
			for( i=0; i<4; i++)
			{ 
				BYTE vel = *(readbuffer+2+i);
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
        
		vx = 0.5 * ( 0.5774 * ( wheel_vel[2] - wheel_vel[1] ) + 0.7072 * ( wheel_vel[3] - wheel_vel[0] ) ) * rate;
		vy = 0.5 * 0.8258 * ( wheel_vel[1] +  wheel_vel[2] - wheel_vel[0] - wheel_vel[3] ) * rate;
		w  = 0.5 * ( 0.0184  * ( wheel_vel[0] + wheel_vel[3] ) + 0.026 * ( wheel_vel[1] + wheel_vel[2] ) ) * rate;	

		odo.vtrans.x = vx*1e-2; // in m/s
	    odo.vtrans.y = vy*1e-2;
	    odo.vrot     = w;
	    odo.kick     = amIKicking;  

		cpc.set_deg(rece_dsp_data[4]);
        
		SYSTEMTIME systime;
		t_odo.get(systime);
		t_odo.set(systime);
/*		
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
		} */
	}
}

RobotProperties OmniRobot::get_robot_properties () const throw ()
{
	return robot_properties;
}

/*
void OmniRobot::fill_robot_data( const BYTE* src, const DriveVector &dv_set, RobotData &rd, Time &rd_time )
{
   memcpy(rd.readdata, src, READLEN*sizeof(BYTE));
   rd.motor_vcc = rece_dsp_data[5]; 
   rd.BoardID = (int)(rece_dsp_data[4]);
   
   for (unsigned int k1=0; k1<4; k1++)
   {
	   rd.wheel_vel[k1] = wheel_vel[k1];
   }

   rd.dv_set = dv_set;
   SYSTEMTIME systime;
   rd_time.get(systime);
   rd_time.set(systime);
}*/




 