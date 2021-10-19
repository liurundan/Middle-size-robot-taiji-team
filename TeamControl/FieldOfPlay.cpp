// FieldOfPlay.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "FieldOfPlay.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FieldOfPlay dialog

FieldOfPlay::FieldOfPlay(CWnd* pParent /*=NULL*/)
: CDialog(FieldOfPlay::IDD, pParent)
{
	//{{AFX_DATA_INIT(FieldOfPlay)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	fw = NULL;
}

FieldOfPlay::~FieldOfPlay()
{
	if (fw)
	{
		fw->DestroyWindow();
		delete fw;
		fw = NULL;
	}
	if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		delete m_StatusBar;
	    m_StatusBar = NULL;
	}

}

void FieldOfPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FieldOfPlay)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(FieldOfPlay, CDialog)
ON_COMMAND_RANGE(ID_BTN_ZOOMIN,ID_BTN_CLOCK,Docommand)
	ON_MESSAGE( slDisplacementMSg, emit_slDisplacementMSg )
	ON_MESSAGE( robotDisplacementMsg, emit_robotDisplacementMsg )
	ON_MESSAGE( coutMessage, Coutmsg )
//{{AFX_MSG_MAP(FieldOfPlay)
ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FieldOfPlay message handlers

BOOL FieldOfPlay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_parent = GetParent();
    
	if (!m_ToolBar.CreateEx( this,TBSTYLE_FLAT ,  WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | 
		CBRS_GRIPPER | CBRS_TOOLTIPS,
		CRect(4,4,0,0) ) ||	!m_ToolBar.LoadToolBar(IDR_TOOLBAR1) )
	{
		TRACE0("failed to create toolbar\n");
		return FALSE;
	}
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() | CBRS_FLYBY |TBSTYLE_DROPDOWN | 
		TBSTYLE_FLAT |TBSTYLE_BUTTON |TBSTYLE_CHECK   );
    for (int i = 8 ; i<23; i++ )
	{
		m_ToolBar.GetButtonStyle(i);
		m_ToolBar.SetButtonStyle(i,TBBS_CHECKBOX);
	}
	m_ToolBar.GetButtonStyle(21);
	m_ToolBar.SetButtonStyle(21,TBBS_BUTTON);
	
	m_ToolBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);    
	
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );

	fw = new FieldWidget;
	GetClientRect(clientrect);
	
	fw->Create( NULL, NULL,                                               
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
		clientrect, this, 0 );
	
	fw->ShowWindow(SW_SHOW);
	
	return TRUE;  
}

void FieldOfPlay::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	if (rc.Width()!=clientrect.Width() || rc.Height() != clientrect.Height())
	{
		 fw->SetWindowPos( NULL,clientrect.TopLeft().x,clientrect.TopLeft().y+30,
			                rc.Width(), rc.Height()-50,SWP_SHOWWINDOW );
		 CRect  sbar;
		 sbar = rc;
		 sbar.top = rc.bottom;
		 m_StatusBar->MoveWindow(rc);
		 clientrect = rc;
	}

}

void FieldOfPlay::Docommand(UINT nID) 
{
	switch( nID - ID_BTN_ZOOMIN )
	{
	   case 0:
		   fw->zoom_in ();
		   break;
	   case 1:
		   fw->zoom_out ();
		   break;
	   case 2:
		   fw->zoom_all ();
		   break;
	   case 7:
		   fw->zoom_undo ();
		   break;
	   case 8:
		   fw->zoom_redo ();
		   break;
	   case 9:
		   fw->flip_side ();
		   break;
	   case 10:
		   fw->flip_goals ();
		   break;
	   case 12:
		   fw->next_refrobot ();
		   break;
	   case 13:
		   fw->show_wm_robot (!(fw->ppref.show_wm_robot));
		   break;
	   case 14:
		   fw->show_wm_ball (!(fw->ppref.show_wm_ball));
		   break;
	   case 15:
		   fw->show_wm_obstacles (!(fw->ppref.show_wm_obstacles));
		   break;
	   case 16:
		   fw->show_vis_lines (!(fw->ppref.show_vis_lines));
		   break;
	   case 18:
		   fw->show_vis_ball (!(fw->ppref.show_vis_ball));
		   break;
	   case 19:
		   fw->show_vis_obstacles (!(fw->ppref.show_vis_obstacles));
		   break;
	   case 20:
		   fw->show_vis_goals (!(fw->ppref.show_vis_goals));
		   break;
	   case 21:
		   fw->show_sl_pos (!(fw->ppref.show_sl_pos));
		   break;
	   case 22:
		   fw->show_aux_lines (!(fw->ppref.show_aux_lines));
		   break;
	   case 23:
		   fw->show_robot_ids (!(fw->ppref.show_robot_ids));
		   break; 
	   case 24:
		   fw->show_robot_ball_links (!(fw->ppref.show_robot_ball_links));
		   break;
	   case 25:
		   fw->show_robot_trace (!(fw->ppref.show_robot_trace));
		   break;
	   case 27:
		   fw->show_ball_trace(!(fw->ppref.show_ball_trace));
		   break;
	   case 28:
		   fw->clear_lines();
		   break;
	   case 29:
		   fw->use_exec_time (!(fw->ppref.use_exec_time));
		   break;
	   default:
		   break;
	}
}

BOOL FieldOfPlay::PreTranslateMessage(MSG* pMsg)
{
	CRect rc;
    fw->GetClientRect(rc);
	Vec bild_diff = fw->widget2field(Vec (0,0))-fw->widget2field (Vec (rc.Width()-1, rc.Height()-1));
	double shift = 0.15*(fabs (bild_diff.x)<fabs(bild_diff.y) ? fabs(bild_diff.x) : fabs(bild_diff.y));
	Vec delta(0,0);
	switch( pMsg->message ) 
	{
	   case WM_KEYDOWN:
		   {			
			   switch(pMsg->wParam) 
			   {
				   case VK_DOWN:
					   delta.y=shift*fw->ppref.zoom.own_half;
					   fw->move_clipping (delta);
					   break;
				   case VK_UP:
					   delta.y=-shift*fw->ppref.zoom.own_half;
					   fw->move_clipping (delta);
					   break;
				   case VK_RIGHT:
					   delta.x=shift*fw->ppref.zoom.own_half;
					   fw->move_clipping (delta);
					   break;
				   case VK_LEFT:
					   delta.x=-shift*fw->ppref.zoom.own_half;
					   fw->move_clipping (delta);
					   break;
				   case VK_ADD:
					   fw->zoom_in ();
					   break;
				   case VK_SUBTRACT:
					   fw->zoom_out ();
					   break;
				   case VK_SPACE:
					   fw->zoom_undo ();
					   break;
				   case VK_RETURN:
					   fw->zoom_redo ();
					   break;
				   case 'C':
					   fw->clear_lines ();
					   break;
				   case 'A':
					   fw->zoom_all ();
					   break;
				   case 'T':
					   fw->toggle_robot_trace ();
					   break;
				   case VK_F1:
					   if (fw->cinfo.rloc_vis.size()>=1)
						   fw->ppref.reference_robot = 0;
					   break;
				   case VK_F2:
					   if (fw->cinfo.rloc_vis.size()>=2)
						   fw->ppref.reference_robot = 1;
					   break;
				   case VK_F3:
					   if (fw->cinfo.rloc_vis.size()>=3)
						   fw->ppref.reference_robot = 2;
					   break;
				   case VK_F4:
					   if (fw->cinfo.rloc_vis.size()>=4)
						   fw->ppref.reference_robot = 3;
					   break;
				   case VK_F5:
					   if (fw->cinfo.rloc_vis.size()>=5)
						   fw->ppref.reference_robot = 4;
					   break;
				   case VK_F6:
					   if (fw->cinfo.rloc_vis.size()>=6)
						   fw->ppref.reference_robot = 5;
					   break;
				   case VK_F7:
					   if (fw->cinfo.rloc_vis.size()>=7)
						   fw->ppref.reference_robot = 6;
					   break;
				   case VK_F8:
					   if (fw->cinfo.rloc_vis.size()>=8)
						   fw->ppref.reference_robot = 7;
					   break;
				   case VK_F9:
					   if (fw->cinfo.rloc_vis.size()>=9)
						   fw->ppref.reference_robot = 8;
					   break;
				   case VK_F10:
					   if (fw->cinfo.rloc_vis.size()>=10)
						   fw->ppref.reference_robot = 9;
					   break;
				   case VK_F11:
					   if (fw->cinfo.rloc_vis.size()>=11)
						   fw->ppref.reference_robot = 0; // 10
					   break;
				   case VK_F12:
					   if (fw->cinfo.rloc_vis.size()>=12)
						   fw->ppref.reference_robot = 11;
					   break;
				   default:
					   ::SendMessage(m_parent->m_hWnd,unresolvedKeyPressEvent,0,0); 
					   break;
			   }
		   } 
		   break;
	 default:
	    break;	   
	}
    return CDialog::PreTranslateMessage(pMsg);
}

LRESULT FieldOfPlay::emit_slDisplacementMSg(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_parent->m_hWnd,slDisplacementMSg,wParam,1);
	Vec p = *(Vec*)(wParam);
	Angle h = *((Angle*)(lParam));
    CString s;
	s.Format("Put sl hint pos: %.fmm, %.fmm, Angle:%.f ",p.x,p.y,h.get_deg());
    m_StatusBar->SetText(s,0,0);
 	return 0;
}

LRESULT FieldOfPlay::emit_robotDisplacementMsg(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(m_parent->m_hWnd,robotDisplacementMsg,wParam,lParam);
	Vec p = *(Vec*)(wParam);
	Angle h = *((Angle*)(lParam));
    CString s;
	s.Format("GotoPos: %.fmm, %.fmm, Angle:%.f ",p.x,p.y,h.get_deg());
    m_StatusBar->SetText(s,0,0);
	return 0;
}

  
LRESULT FieldOfPlay::Coutmsg( WPARAM wParam, LPARAM lParam )
{
	CString s =  *((CString*)(wParam));
    m_StatusBar->SetText(s,0,0);
	return 0;
}

void FieldOfPlay::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}
