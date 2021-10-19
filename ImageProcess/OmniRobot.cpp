#include "StdAfx.h"
#include "OmniRobot.h"
#include "RobotProperties.h"
#include <fstream>
 
#include <cmath>

using namespace std;
  

OmniRobot::OmniRobot(  ConfigReader &vr,CMSComm& m_com ) throw(RobotsException, bad_alloc)
{

  string confline;
  InitializeComm(vr,m_com);
  
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

}

void OmniRobot::set_drive_vector(DriveVector dv) throw ()
{
  	double MaxVel = 80.0;
	double rate   = 127.0 / 300.0;
    double RADIUS = 22.5;
	double Vx     = dv.vtrans.x;
	double Vy     = dv.vtrans.y;
	double w      = dv.vrot;
	double v[4];
  
	v[0] = ( -0.707 * ( Vx + Vy ) + w  * RADIUS ) * rate;
	v[1] = ( 0.5 * Vy - 0.866 * Vx + w  * RADIUS) * rate;
	v[2] = ( 0.866 * Vx + 0.5 * Vy + w  * RADIUS) * rate;
	v[3] = ( 0.707 * ( Vx - Vy ) + w  * RADIUS)   * rate;
 
	double Vmax = fabs( v[0] );
	for( int i = 1; i < 4; i++ )
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
		if( v[i] <1 )
			v[i] = 1.0;
 		m_command.SetAt(i+2,(BYTE)(v[i]+0.5));
	}
	 
	amIKicking   = dv.kick;
	kickBallTime = dv.kickballtime;
	if(amIKicking)
	{
		m_command.SetAt(1,kickBallTime);
		kickBallTime = 0;
		amIKicking   = false;
	}
	else
		m_command.SetAt(1,0x00);
    
   // CImageMaskDlg *mApp = (CImageMaskDlg *)(AfxGetApp()->m_pMainWnd);
   // mApp->m_Com.SetOutput(COleVariant(m_command));
}

RobotProperties OmniRobot::get_robot_properties () const throw ()
{
  return robot_properties;
}

void OmniRobot::InitializeComm(ConfigReader &vr, CMSComm& m_com) throw(RobotsException)
{
  	m_command.RemoveAll();
	m_command.SetSize(6);
	m_command.SetAt(0, 0xff);
	m_command.SetAt(1, 0x55);
	m_command.SetAt(2, 0x7f);
	m_command.SetAt(3, 0x7f);
	m_command.SetAt(4, 0x7f);
	m_command.SetAt(5, 0x7f);
    string sTmp;

	if( m_com.GetPortOpen() )  
	{
	  m_com.SetPortOpen(FALSE);
	}
	if (vr.get("COM_PORT", sTmp)<=0) 
	{
      throw InvalidConfigurationException ("port error!");
	} 

   	if( sTmp ==_T("COM1") )
		m_com.SetCommPort(1);   
	if( sTmp ==_T("COM2") )
		m_com.SetCommPort(2);   
	if( sTmp ==_T("COM5"))
		m_com.SetCommPort(5);   
	if( sTmp ==_T("COM6"))
		m_com.SetCommPort(6);   
	if( sTmp ==_T("COM7"))
		m_com.SetCommPort(7); 

    if (vr.get("COM_BAUdRATE", sTmp)<=0) 
	{ 
 	  throw InvalidConfigurationException ("comm rate error!");
	}
	
 	if( sTmp == _T(" 9600") )
		m_com.SetSettings("9600,n,8,1"); 
	if( sTmp == _T("19200") )
		m_com.SetSettings("19200,n,8,1"); 
	if( sTmp == _T("38400") )
		m_com.SetSettings("38400,n,8,1"); 
    
	m_com.SetInBufferSize(1024);  
	m_com.SetOutBufferSize(512);  

	m_com.SetInBufferCount(0);
	m_com.SetInputMode(1); 
     
	m_com.SetRThreshold(11);

    
	if(!m_com.GetPortOpen()) //´ò¿ª´®¿Ú
	{
	  m_com.SetPortOpen(TRUE);
	}
	else
      throw BadHardwareException("Init_Robot");

	m_com.SetInputLen(0); 
	m_com.GetInput();	 

}


 