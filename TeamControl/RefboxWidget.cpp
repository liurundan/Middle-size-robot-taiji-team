// RefboxWidget.cpp : implementation file
//

#include "stdafx.h"
#include "TeamControl.h"
#include "RefboxWidget.h"
#include "Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RefboxWidget dialog


RefboxWidget::RefboxWidget(CWnd* pParent /*=NULL*/)
: CDialog(RefboxWidget::IDD, pParent)
{
	//{{AFX_DATA_INIT(RefboxWidget)
	//}}AFX_DATA_INIT
	ownGoalscore = 0;
	oppGoalscore = 0;

    refbox_client = NULL;
	statuscolor   = NULL;
	
	disp_msg.clear();
}

RefboxWidget::~RefboxWidget()
{
    if (refbox_client) delete refbox_client;
	if (statuscolor)
	{
		statuscolor->DestroyWindow();
		delete statuscolor;
		statuscolor = NULL;
	}
	if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		m_StatusBar = NULL;
	}
	for (unsigned int i = 0; i<disp_msg.size(); i++ )
	{
		disp_msg[i]->DestroyWindow();
		delete disp_msg[i];
		disp_msg[i] = NULL;
	}

} 

void RefboxWidget::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RefboxWidget)
	DDX_Control(pDX, IDC_REFBOX_IP, refbox_ip);
	DDX_Control(pDX, IDC_REFBOXCONNECT_CHECK, refbox_connect);
	//}}AFX_DATA_MAP
}
 
BEGIN_MESSAGE_MAP(RefboxWidget, CDialog)
	ON_MESSAGE( connectMessage, outconnectMessage )

	//{{AFX_MSG_MAP(RefboxWidget)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHANGEHALF, changeOwnHalfPressed)
	ON_BN_CLICKED(IDC_CHANGELABEL, changeLabelPressed)
	ON_BN_CLICKED(IDC_STOP, stopPressed)
	ON_BN_CLICKED(IDC_READY, readyPressed)
	ON_BN_CLICKED(IDC_START, startPressed)
	ON_BN_CLICKED(IDC_OWNKICKOFF, kickOffOwnPressed)
	ON_BN_CLICKED(IDC_OWNTHROWIN, throwInOwnPressed)
	ON_BN_CLICKED(IDC_OWNGOALKICK, goalKickOwnPressed)
	ON_BN_CLICKED(IDC_OWNCORNERKICK, cornerKickOwnPressed)
	ON_BN_CLICKED(IDC_OWNFREEKICK, freeKickOwnPressed)
	ON_BN_CLICKED(IDC_OWNPENALTYKICK, penaltyKickOwnPressed)
	ON_BN_CLICKED(IDC_OPPKICKOFF, kickOffOpponentPressed)
	ON_BN_CLICKED(IDC_OPPTHROWIN, throwInOpponentPressed)
	ON_BN_CLICKED(IDC_OPPGOALKICK, goalKickOpponentPressed)
	ON_BN_CLICKED(IDC_OPPCORNERKICK, cornerKickOpponentPressed)
	ON_BN_CLICKED(IDC_OPPFREEKICK, freeKickOpponentPressed)
	ON_BN_CLICKED(IDC_OPPPENALTYKICK, penaltyKickOpponentPressed)
	ON_BN_CLICKED(IDC_REFBOXCONNECT_CHECK, refboxConnectionChanged)
	ON_BN_CLICKED(IDC_DROPBALL, droppedBallPressed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RefboxWidget message handlers

BOOL RefboxWidget::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    statuscolor = new StatusColor(100,20);
	statuscolor->Create(NULL, NULL,                                               
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
		CRect(0,0,100,20), this, 0 );
	statuscolor->MoveWindow(20,310,100,20);
	statuscolor->ShowWindow(SW_SHOW);

	StatusColor* sc = new StatusColor(150,20);
	sc->Create(NULL, NULL,                                               
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
		CRect(0,0,150,20), this, 0 );
	sc->MoveWindow(140,5,150,20);
	sc->ShowWindow(SW_SHOW);
	disp_msg.push_back(sc);

	for( unsigned int i =0; i < 6; i++ )
	{
		sc = new StatusColor(100,20);
		sc->Create(NULL, NULL,                                               
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
			CRect(0,0,100,20), this, 0 );
		disp_msg.push_back(sc);
	}
    disp_msg[1]->MoveWindow(20,268,100,20);
	disp_msg[2]->MoveWindow(250,268,100,20);
	
    disp_msg[3]->MoveWindow(20,210,100,20);
	disp_msg[4]->MoveWindow(20,235,100,20);
    disp_msg[5]->MoveWindow(250,210,100,20);
	disp_msg[6]->MoveWindow(250,235,100,20);

    m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );

	m_pOwner = GetParent();	

 	refstateChanged(errorState);
	team_color = 1;
	own_half   = 1;
	disp_msg[3]->SetStatusColor(COL_YELLOW,"    yellow     ");
	disp_msg[4]->SetStatusColor(COL_CYAN,"    cyan       ");
	disp_msg[5]->SetStatusColor(COL_BLUE,"    blue       ");
	disp_msg[6]->SetStatusColor(COL_MAGENTA,"    magenta    ");
    
	CString s;
	s.Format("%d          ",ownGoalscore);
	disp_msg[1]->SetStatusColor(COL_WHITE,s);
	s.Format("%d          ",oppGoalscore);
	disp_msg[2]->SetStatusColor(COL_WHITE,s);
    
	
	refbox_ip.SetWindowText("localhost       ");
    
	return TRUE;   
	               
}

void RefboxWidget::init(ConfigReader& cfg)
{
	refbox_client = new RefboxClient("refbox_client.log",this);
	std::string text;
	if (cfg.get("refbox_IP",text)) 
	{
		refbox_ip_edit = _T(text.c_str());
		CString s;
		s.Format("%s     ", refbox_ip_edit);
		refbox_ip.SetWindowText(s);
	}
	int pt;
	if (cfg.get("refbox_port",pt))
		refbox_port = pt;
	else
		refbox_port = 28097;

}

HBRUSH RefboxWidget::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    
	if ( pWnd->GetDlgCtrlID() == IDC_OWNGOALCOLOR ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(owngoal);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_OPPGOALCOLOR ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(oppgoal);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_OWNLABELCOLOR ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(ownlabel);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_OPPLABELCOLOR ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(opplable);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_CURRENTREFEREESTATE ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(COL_WHITE);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_OWNSCORE || pWnd->GetDlgCtrlID() == IDC_OPPSCORE || 
		pWnd->GetDlgCtrlID() == IDC_REFBOX_IP ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(COL_WHITE);
	}
 	
 	return hbr;
}

void RefboxWidget::refstateChanged(RefereeState rs)
{
	refstate = rs;
	disp_msg[0]->SetStatusColor(COL_WHITE,const_cast<char*>(referee_state_names[rs]));
}

void RefboxWidget::goalstatusChanged( RefboxSignal rsg )
{
	if ( rsg == SIGownGoalScored ) 
	{
		ownGoalscore++;
	}
	else if (rsg == SIGopponentGoalScored ) 
	{
		oppGoalscore++;
	}
	CString s;
	s.Format("%d          ",ownGoalscore);
	disp_msg[1]->SetStatusColor(COL_WHITE,s);
	s.Format("%d          ",oppGoalscore);
	s.Format("%d          ",oppGoalscore);
	disp_msg[2]->SetStatusColor(COL_WHITE,s);


}

void RefboxWidget::ChangeStatusCol()
{
	if (refbox_client->is_okay())
		statuscolor->SetStatusColor(COL_GREEN,"Communication");
	else if (refbox_client->is_connected())
		statuscolor->SetStatusColor(COL_BLUE,"Communication");
	else  
		statuscolor->SetStatusColor(COL_RED,"Communication");
}

void RefboxWidget::refboxConnectionChanged() 
{
	bool b = refbox_connect.GetCheck() > 0 ? true:false;
	if (b) 
		connectRefboxClient();
	else
		disconnectRefboxClient();	
}

void RefboxWidget::connectRefboxClient()
{
	refbox_client->connect( refbox_ip_edit.GetBuffer(0), refbox_port );
}

void RefboxWidget::disconnectRefboxClient()
{
	refbox_client->disconnect();
}
    
LRESULT RefboxWidget::outconnectMessage(WPARAM wParam, LPARAM lParam)
{
	CString s =  *((CString*)(wParam));
    m_StatusBar->SetText(s,0,0);
	return 0;
}
 
void RefboxWidget::labelChanged(int n)
{
	team_color = n;
	if( n > 0 )
	{
		disp_msg[4]->SetStatusColor(COL_CYAN,"    cyan       ");
		disp_msg[6]->SetStatusColor(COL_MAGENTA,"    magenta    ");
	} 
	else 
	{
		disp_msg[4]->SetStatusColor(COL_MAGENTA,"    magenta    ");
		disp_msg[6]->SetStatusColor(COL_CYAN,"    cyan       ");
	}
	Invalidate(FALSE);
}

void RefboxWidget::ownHalfChanged(int n)
{
	own_half = n;
	if( n > 0 ) 
	{
		disp_msg[3]->SetStatusColor(COL_YELLOW,"    yellow     ");
		disp_msg[5]->SetStatusColor(COL_BLUE,"    blue       ");
	} 
	else 
	{
		disp_msg[3]->SetStatusColor(COL_BLUE,"    blue       ");
		disp_msg[5]->SetStatusColor(COL_YELLOW,"    yellow     ");
	} 
	Invalidate(FALSE);
}
 
void RefboxWidget::changeOwnHalfPressed() 
{
	int n = (own_half>0 ? -1 : +1);
	::SendMessage( m_pOwner->m_hWnd,ownHalfSelect, n, 0 );
}

void RefboxWidget::changeLabelPressed() 
{
	int n = (team_color>0 ? -1 : +1);
	::SendMessage( m_pOwner->m_hWnd,labelSelect, n, 0 );
}

void RefboxWidget::stopPressed() 
{
	RefboxSignal rs = SIGstop;
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, rs, 0 );
}

void RefboxWidget::readyPressed() 
{
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGready, 0 );
}

void RefboxWidget::startPressed() 
{
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGstart, 0 );
}

void RefboxWidget::kickOffOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanKickOff, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaKickOff, 0 );
}

void RefboxWidget::throwInOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanThrowIn, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaThrowIn, 0 );
	
}

void RefboxWidget::goalKickOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanGoalKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaGoalKick, 0 );
	
}

void RefboxWidget::cornerKickOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanCornerKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaCornerKick, 0 );
	
}

void RefboxWidget::freeKickOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanFreeKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaFreeKick, 0 );
	
}

void RefboxWidget::penaltyKickOwnPressed() 
{
  if (team_color>0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanPenalty, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaPenalty, 0 );
	
}

void RefboxWidget::kickOffOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanKickOff, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaKickOff, 0 );
	
}

void RefboxWidget::throwInOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanThrowIn, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaThrowIn, 0 );
}

void RefboxWidget::goalKickOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanGoalKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaGoalKick, 0 );
	
}

void RefboxWidget::cornerKickOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanCornerKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaCornerKick, 0 );
	
}

void RefboxWidget::freeKickOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanFreeKick, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaFreeKick, 0 );
 	
}

void RefboxWidget::penaltyKickOpponentPressed() 
{
  if (team_color<0)
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGcyanPenalty, 0 );
  else
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGmagentaPenalty, 0 );
}


void RefboxWidget::droppedBallPressed() 
{
	::SendMessage( m_pOwner->m_hWnd,refboxSignal, SIGdroppedBall, 0 );
}

BOOL RefboxWidget::PreTranslateMessage(MSG* pMsg)
{
	//GetDlgItem(IDC_STOP)->SetFocus(); 
	//GetDlgItem(IDC_START)->SetFocus();
	switch( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case 'A': startPressed();GetDlgItem(IDC_STOP)->SetFocus();	break;
			case 'F': stopPressed();	break;
			default: break;
			}
		}break;
		default: break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}