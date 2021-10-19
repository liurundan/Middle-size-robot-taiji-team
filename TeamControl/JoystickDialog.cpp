// JoystickDialog.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "JoystickDialog.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JoystickDialog dialog


JoystickDialog::JoystickDialog(CWnd* pParent /*=NULL*/)
	: CDialog(JoystickDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(JoystickDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

JoystickDialog::~JoystickDialog()
{
	dcMem.DeleteDC();
	bm.DeleteObject();
	Pen.DeleteObject();
	Brush.DeleteObject();
}


void JoystickDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JoystickDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(JoystickDialog, CDialog)
	//{{AFX_MSG_MAP(JoystickDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JoystickDialog message handlers

BOOL JoystickDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Pen.CreatePen(PS_INSIDEFRAME,1,RGB(255,255,0));
	Brush.CreateSolidBrush(RGB(255,255,0));
    
	GetClientRect(&rc);
    dcMem.CreateCompatibleDC(GetDC());
	bm.CreateCompatibleBitmap(GetDC(),rc.Width(),rc.Height());
	dcMem.SelectObject(&bm);
	dcMem.SelectObject(Pen);
	dcMem.SelectObject(Brush);

 	vel      = 0.2;
	vrot     = 0.6;

	vel_ini  = 0.2;
	rot_ini  = 0.6;

	max_vel  = 0.8;
	max_vrot = 3.0;

    rot_step = 0.3;
	vel_step = 0.1;
	bkick      = false;
	kick_power = 10;

	dest = DriveVector(Vec(0.0,0.0),0.0,false);
	
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(NULL,540,385,(rect.right-rect.left),(rect.bottom-rect.top),SWP_SHOWWINDOW);
	ShowWindow(SW_HIDE);
	
	return TRUE; 
}
 
BOOL JoystickDialog::PreTranslateMessage(MSG* pMsg)
{
 	switch( pMsg->message ) 
	{
	   case WM_KEYDOWN:
			{			
				switch(pMsg->wParam) 
				{
 					case 'T':
						vrot = vrot > 0 ? rot_ini:-rot_ini;
						vrot = fabs(vrot);
						setVelocity(0.,0.,vrot);
						m_robotmotion = rotate;
						break;
 					case 'R':
						vrot = vrot > 0 ? rot_ini:-rot_ini;
						vrot = -1* fabs(vrot);
						setVelocity(0.,0.,vrot);
						m_robotmotion = rotate;
						break;
					case VK_RIGHT:
						vel = vel > 0 ? vel_ini:-vel_ini;
						vel = -1 * fabs(vel);
						setVelocity(0.,vel,0.);
						m_robotmotion = moveRight;
						break;
					case VK_LEFT:
						vel = vel > 0 ? vel_ini:-vel_ini;
						vel = fabs(vel);
						setVelocity(0.,vel,0.);
						m_robotmotion = moveLeft;
						break;
					case VK_UP:
						vel = vel > 0 ? vel_ini:-vel_ini;
						vel = fabs(vel);
						setVelocity(vel,0.,0.); // in m/s
						m_robotmotion = moveStraightUp;
						break;
					case VK_DOWN:
						vel = vel > 0 ? vel_ini:-vel_ini;
						vel = -1* fabs(vel);
						setVelocity(vel,0.,0.);
						m_robotmotion = moveBackward;
						break;
					case VK_SPACE:
						setVelocity(0.0,0.0,0.0);
						m_robotmotion = stop;
						break;
					case 'A':
						accelerate();
						break;
					case 'D':
						decelerate();
						break;
					case 'P':
						bkick = !bkick;
						setVelocity(0.0,0.0,0.0,bkick,kick_power);
						break;
 					default:
						setVelocity(0.0,0.0,0.0);
						break;
				} 
			}
			Invalidate(FALSE);
		    break;
 	   default: 
		   break;
	} 
 	
	return CDialog::PreTranslateMessage(pMsg);
}

void JoystickDialog::setVelocity(double rvx, double rvy, double rw, bool bk, BYTE power )
{
    dest = DriveVector(Vec(rvx,rvy),rw,bk,power); // in m/s 
}

void JoystickDialog::accelerate()
{
	switch(m_robotmotion)
	{
		case rotate:
			{
				if( fabs(vrot) < 3.0 )
				{
					vrot = vrot > 0 ? vrot + rot_step : vrot - rot_step;
				}
				setVelocity(0.,0.,vrot);
			}
			break;
		case moveStraightUp:
			if (vel < max_vel)
				vel += vel_step;
			setVelocity(vel,0.,0.);
			break;
		case moveBackward:
			if (vel > -max_vel)
				vel -= vel_step;
			setVelocity(vel,0.,0.);
			break;
		case moveLeft:
			if (vel < max_vel)
				vel += vel_step;
			setVelocity(0.,vel,0.);
			break;
		case moveRight:
			if (vel > -max_vel)
				vel -= vel_step;
			setVelocity(0.,vel,0.);
			break;
		default:
			setVelocity(0.0,0.0,0.0);
			break;
	}
}

void JoystickDialog::decelerate()
{
	switch(m_robotmotion)
	{
		case rotate:
			{
				if( fabs(vrot) > 0.3 )
				{
					vrot = vrot > 0.3 ? (vrot - rot_step) : (vrot + rot_step);
				}
				else
					vrot = 0.0;
 			    setVelocity(0.,0.,vrot);
			}break;
		case moveStraightUp:
			if (vel > 0.1 )
				vel -= vel_step;
			else
				vel = 0.0;
			setVelocity(vel,0.,0.);
			break;
		case moveBackward:
			if (vel < -0.1 )
				vel += vel_step;
			else
				vel = 0.0;
			setVelocity(vel,0.,0.);
			break;
		case moveLeft:
			if (vel > 0.1)
				vel -= vel_step;
			else
				vel = 0.0;
			setVelocity(0.,vel,0.);
			break;
		case moveRight:
			if (vel < -0.1)
				vel += vel_step;
			else
				vel = 0.0;
			setVelocity(0.,vel,0.);
			break;
		default:
			setVelocity(0.0,0.0,0.0);
			break;
	}
}

void JoystickDialog::getDriveVectorFromJoystick( DriveVector &drv )
{
   drv = dest;
}

void JoystickDialog::OnPaint() 
{
	CPaintDC dc(this);  
  	
	dcMem.FillSolidRect(rc,RGB(135,228,118));
 	CString str;
	switch(m_robotmotion)
	{
		case stop:
			str.Format("Stop Robot");
			break;
		case moveStraightUp:
			str.Format("Forward Move: %.1f %.1f %.1f ", dest.vtrans.x,dest.vtrans.y,dest.vrot );
			break;
		case moveBackward:
			str.Format("Back Move: %.1f %.1f %.1f ",  dest.vtrans.x,dest.vtrans.y,dest.vrot );
			break;
		case moveLeft:
			str.Format("Left Move: %.1f %.1f %.1f",   dest.vtrans.x,dest.vtrans.y,dest.vrot );
			break;
		case moveRight:
			str.Format("Right Move: %.1f %.1f %.1f ",  dest.vtrans.x,dest.vtrans.y,dest.vrot );
			break;
		case rotate:
			str.Format("Turn: %.1f %.1f %.1f", dest.vtrans.x,dest.vtrans.y,dest.vrot);
			break;
		default:
		    str.Format("Stop Robot");
			break;
	}
 	dcMem.TextOut(10,10,str);
    if(bkick)
	{
 		CString str2 = "Kick";
 	    dcMem.TextOut(10,30,str2);
	}
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
 	
}

void JoystickDialog::OnCancel()
{
	ShowWindow(SW_HIDE);
	::SendMessage(GetParent()->m_hWnd,closejoydialog,0,0);
}
