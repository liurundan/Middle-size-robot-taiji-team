// RealRobotData.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "RealRobotData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealRobotData dialog


CRealRobotData::CRealRobotData(CWnd* pParent /*=NULL*/)
	: CDialog(CRealRobotData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealRobotData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	startpoint       = CPoint(5,5);
	width            = 100;
	height			 = 120;
	valplot_widget.resize(5);
	valplot_widget.clear();
}

CRealRobotData::~CRealRobotData()
{
	for ( unsigned int i=0; i<valplot_widget.size(); i++ )
		delete valplot_widget[i];
}

void CRealRobotData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealRobotData)
	DDX_Control(pDX, IDC_ROBOT_VELOCITY, rob_vel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealRobotData, CDialog)
	//{{AFX_MSG_MAP(CRealRobotData)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealRobotData message handlers

BOOL CRealRobotData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect( startpoint.x, startpoint.y, 
				startpoint.x+width, startpoint.y+height );
	for( unsigned int i=0; i<5; i++ )
	{
       CValPlotWidget* pw = new CValPlotWidget(width,height);
	   pw->Create(	NULL, NULL,                                                       
					WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
					rect, this, 0);
	   pw->MoveWindow(i*106,5,100,120);
	   valplot_widget.push_back(pw);
	   if (i > 0) 
		   pw->init_widget(-127,127,100);
	   else
		   pw->init_widget(30,100);
	}
   
	return TRUE;   
}

void CRealRobotData::OnPaint() 
{
	CPaintDC dc(this);  
}

void CRealRobotData::update(const RobotData &rd)
{
	CString sTmp;
    
	for (unsigned int i = 0; i< 5; i++)
	{
		if (i > 0)
			valplot_widget[i]->push(rd.wheel_vel[i-1]);
		else
			valplot_widget[i]->push(rd.motor_vcc); 
		valplot_widget[i]->Invalidate(FALSE);
	}
	sTmp.Format("vx:%.2f vy:%.2f w:%.2f",rd.robot_vel[0],
					rd.robot_vel[1],rd.robot_vel[2]);
	rob_vel.SetWindowText(sTmp);
    
	sTmp.Format("wheel_vel:%.f, %.f, %.f, %.f",rd.wheel_vel[0],rd.wheel_vel[1],
											rd.wheel_vel[2],rd.wheel_vel[3]);
    GetDlgItem(IDC_MOTORVEL_0)->SetWindowText(sTmp);
	sTmp.Format("VCC: %.1f V, head: %d",rd.motor_vcc,rd.BoardID );
    GetDlgItem(IDC_VCC)->SetWindowText(sTmp);
 
}
