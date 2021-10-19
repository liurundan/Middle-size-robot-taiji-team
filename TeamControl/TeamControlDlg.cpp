// TeamControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TeamControl.h"
#include "TeamControlDlg.h"
#include "Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeamControlDlg dialog

CTeamControlDlg::CTeamControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTeamControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeamControlDlg)
	m_commrate = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	field_widget		= NULL;
	refbox_widget		= NULL;

	rrd_widget			= NULL;
	coach_widget		= NULL;
	refstate_machine	= NULL;
	robots.clear();
	dir_of_play.own_half	= 1;
	dir_of_play.team_color	= 1;
	refrobot				= 0;
	cyclerobot				= 0;
	
}

void CTeamControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeamControlDlg)
	DDX_Control(pDX, IDC_REFERENCE_COMBO, reference_select);
	DDX_Control(pDX, IDC_COMM_RATE_SPIN, comm_rate_spin);
	DDX_Text(pDX, IDC_COMMRATE_EDIT, m_commrate);
	DDV_MinMaxInt(pDX, m_commrate, 100, 2000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTeamControlDlg, CDialog)
	ON_MESSAGE( slDisplacementMSg, slDisplacement )
	ON_MESSAGE( robotDisplacementMsg, robotDisplacement )
    ON_COMMAND_RANGE(IDMS_ROBOT1,IDMS_ROBOT_SIMULATOR,OnDispSelectRobot)
	ON_UPDATE_COMMAND_UI_RANGE(IDMS_ROBOT1,IDMS_ROBOT_SIMULATOR,OnUpdateSelectMenu)
	ON_WM_INITMENUPOPUP()
	//{{AFX_MSG_MAP(CTeamControlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_COMMRATE_EDIT, cycleRateChanged)
	ON_CBN_SELCHANGE(IDC_REFERENCE_COMBO, reference_robot_changed)
	ON_COMMAND(IDM_PLAYING_FIELD, OnPlayingField)
	ON_COMMAND(IDM_REFEREE_STATES, OnRefereeStates)
	ON_COMMAND(IDM_ROBOT_DATA, OnRobotData)
	ON_COMMAND(IDM_TRAINER, OnTrainer)
	ON_MESSAGE( refboxSignal, refboxSignalChanged )
	ON_MESSAGE( ownHalfSelect, ownHalfChanged )
	ON_MESSAGE( labelSelect, teamColorChanged )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeamControlDlg message handlers

BOOL CTeamControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CRect rc;
	GetClientRect(&rc);
	SetWindowPos(NULL,0,0,rc.Width()+5,rc.Height()+60,SWP_SHOWWINDOW);

	init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTeamControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTeamControlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTeamControlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTeamControlDlg::OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu)     
{   
  if( !bSysMenu )   
  {   
      ASSERT(pPopupMenu != NULL);   
        
      CCmdUI   state;                   
      state.m_pMenu = pPopupMenu;   
      ASSERT(state.m_pOther == NULL);   
        
      state.m_nIndexMax  = pPopupMenu->GetMenuItemCount();   
      for(state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++ )   
      {   
          state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);   
          if( state.m_nID == 0 )   
                  continue;     
                    
          ASSERT(state.m_pOther == NULL);   
          ASSERT(state.m_pMenu != NULL);   
          if( state.m_nID == (UINT)-1 )   
          {   
              state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);   
              if(state.m_pSubMenu == NULL || ( state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||   
                      state.m_nID == (UINT)-1)   
              {                                              
                 continue;   //   first   item   of popup   can't   be   routed   to   
              }   
              state.DoUpdate(this,   FALSE);     //   popups  are   never   auto   disabled   
          }   
          else   
          {   
              state.m_pSubMenu   =   NULL;   
              state.DoUpdate(this,  state.m_nID < 0xF000);   
          }   
      }   
  }   
} 
  
void CTeamControlDlg::OnDispSelectRobot(UINT nID)
{
	int index = nID - IDMS_ROBOT1;
	if (index > robots.size() - 1 || (!robots[index]) )
	   return;
	if( robots[index]->IsWindowVisible() )
	{
		//robots[index]->ShowWindow(SW_HIDE);
        robots[index]->hide();
	}
	else
		robots[index]->ShowWindow(SW_SHOW);
}

void CTeamControlDlg::OnUpdateSelectMenu(CCmdUI* pCmdUI)
{
	int index = pCmdUI->m_nID - IDMS_ROBOT1;
	if (index > robots.size() - 1 || (!robots[index]) )
	   return;

	if (robots[index]->IsWindowVisible()) 
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);

}

void CTeamControlDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		m_StatusBar = NULL;
	}

    if (refbox_widget)
	{
		delete refbox_widget;
		refbox_widget = NULL;
    }	
    if (coach_widget)
	{
		delete coach_widget;
		coach_widget = NULL;
    }	
	if (refstate_machine) delete refstate_machine;
    if (rrd_widget)
	{
		delete rrd_widget;
		rrd_widget = NULL;
    }	
    if (field_widget)
	{
		delete field_widget;
		field_widget = NULL;
    }	

	for (unsigned int i = 0; i< robots.size(); i++)
	{
		robots[i]->DestroyWindow();
		delete robots[i];
	}

}

void CTeamControlDlg::init()
{
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );

	col_refboxipedit  = COL_RED;
	UDACCEL accel;
	accel.nSec = 1;
	accel.nInc = 25;
 	comm_rate_spin.SetAccel(sizeof(UDACCEL),&accel);
	comm_rate_spin.SetBuddy(GetDlgItem(IDC_COMMRATE_EDIT));
    comm_rate_spin.SetRange(100,2000);
	comm_rate_spin.SetPos(150);
    try
	{
		cfg_file = "config_files\\teamcontrol.cfg";
		cfg.append_from_file(cfg_file.c_str());
		Journal::the_journal.set_mode(cfg);
	}
	catch (RobotsException& e)
	{
		MessageBox(e.what());
	}

	FieldGeometry fg(cfg);
	fieldgeometry = fg;
    
  
	// RefstateMachine
	refstate_machine = new RefereeStateMachine(fieldgeometry);
    
	// RefboxWidget
	refbox_widget = new RefboxWidget;
	refbox_widget->Create(IDD_REFBOXWIDGET,this);
	refbox_widget->init(cfg);
	refbox_widget->refstateChanged(errorState);
	int own_half = 1;
	std::string text;
	if (cfg.get ("own_half", text)) 
	{
		if( text=="yellow" )
		   own_half = 1;
		else if ( text=="blue" )
		   own_half = -1;
	}
    refbox_widget->ownHalfChanged(own_half);
	int team_color=1;
    if (cfg.get ("team_color", text)) 
	{
		if( text == "cyan" )
		  team_color = 1;
		else
		  team_color = -1;
	} 
    // refbox_widget->labelChanged(team_color);
 	CRect rc_ref;
 	refbox_widget->GetClientRect(&rc_ref);
 	refbox_widget->MoveWindow(540,0,rc_ref.Width(),rc_ref.Height()+30);
	refbox_widget->ShowWindow(SW_SHOW);
    
	// RealRobotDataView
    rrd_widget = new CRealRobotData;
	rrd_widget->Create(IDD_REALROBOTDATA,this);
	
	CRect rc_rrd;
	rrd_widget->GetClientRect(&rc_rrd);
	rrd_widget->MoveWindow(0,580,rc_rrd.Width(),rc_rrd.Height()+20);

	// Robots put on
	std::vector<std::string> words;
    std::vector<std::string> robot_names;
	string sTmp = "robots";
	int index = 0;
	for (unsigned int k=0; k<cfg.m_nRows; k++ )
	{
		string stable = &(cfg.m_Table[k].s[0][0]);
        if ( sTmp == stable )
		{
			index = k;
			break;
        }
	}
	index++;
	for (unsigned int k1 = 0; k1 < cfg.m_Table[index].nID; k1++ )
	{
		string srole =  &(cfg.m_Table[index].s[k1][0]);
		words.push_back(srole);
	}

	CMenu* pMenu = GetMenu();
	pMenu->GetSubMenu(4)->DeleteMenu(0,MF_BYPOSITION);
	
	for (unsigned int j = 0; j < words.size(); j++)
	{
		RemoteRobotWidget* rw = new RemoteRobotWidget;
		rw->Create(IDD_REMOTEROBOTWIDGET);
		rw->init_robot(cfg,fieldgeometry,words[j].c_str());
		rw->direction_of_play_changed(dir_of_play);
		robots.push_back(rw);
		robot_names.push_back(rw->get_state().name);
		
		pMenu->GetSubMenu(4)->AppendMenu( MF_CHECKED | MF_STRING, IDMS_ROBOT1+j,rw->get_state().name.c_str());

        CRect rc_rob;
		rw->GetClientRect(&rc_rob);
		if (j > 3)
			rw->MoveWindow(540,585,rc_rob.Width(),rc_rob.Height()+30);
		else
		    rw->MoveWindow(900,180*j,rc_rob.Width(),rc_rob.Height()+30);
		
		bool b;
        if (cfg.get ((words[j]+string("::widget")).c_str(), b)>0 && b)
			rw->ShowWindow(SW_SHOW);
	}
	pMenu->Detach();
    
	// Field widget put on
    field_widget = new FieldOfPlay;
	field_widget->Create(IDD_FIELDWIDGET,this);
    PaintPreferences ppref;
	PaintActionSelect psel;
	
	ppref.show_wm_robot=ppref.show_wm_ball=ppref.show_robot_ids=ppref.show_robot_ball_links=true;
	ppref.show_wm_obstacles=ppref.show_vis_lines=ppref.show_vis_ball=ppref.show_vis_obstacles=false;
	ppref.show_vis_goals=ppref.show_sl_pos=ppref.show_aux_lines=false;
	ppref.show_robot_trace=ppref.show_ball_trace=false;
	ppref.use_exec_time=false;
	ppref.reference_robot=0;
	ppref.zoom.scaling=ppref.zoom.own_half=ppref.field_heading=1;
	
	psel.show_wm_robot=psel.show_wm_ball=psel.show_robot_ids=psel.show_robot_ball_links=true;
	psel.show_wm_obstacles=true;
	psel.show_vis_lines=psel.show_vis_ball=psel.show_vis_obstacles= psel.show_vis_goals=true;
	psel.show_sl_pos=psel.show_aux_lines=false;
	psel.show_robot_trace=psel.show_ball_trace=true;
	psel.use_exec_time=false;
	psel.clear_lines=true;
	psel.zoom_in=psel.zoom_out=psel.zoom_all=psel.zoom_undo=psel.zoom_redo=psel.flip_sides=true;
	psel.flip_goals=false;
	psel.next_refrobot=true;

	field_widget->fw->init(fg,ppref,psel);
	field_widget->fw->robot_names() = robot_names;
 
	CRect rc_fw;
	field_widget->GetClientRect(&rc_fw);
	field_widget->MoveWindow(0,130,rc_fw.Width()+10,rc_fw.Height()+40);
    HICON m_hicon = AfxGetApp()->LoadIcon(IDI_DISPLAYER);
    field_widget->SetIcon(m_hicon,FALSE);
	field_widget->SetIcon(m_hicon,TRUE);
	
	field_widget->ShowWindow(SW_SHOW);
    
	// Coach
	coach_widget = new CoachWidget;
	coach_widget->Create(IDD_COACHWIDGET,this);
	coach_widget->init(cfg);

	CRect rc_coa;
	coach_widget->GetClientRect(&rc_coa);
	coach_widget->MoveWindow(540,385,rc_coa.Width()+5,rc_coa.Height()+30);


	coach_widget->ShowWindow(SW_SHOW);
	
	for ( unsigned int i=0; i<robot_names.size(); i++ )
	{
		reference_select.InsertString(i,robot_names[i].c_str());
	}
    reference_select.SetCurSel(0);

    SetTimer(0,comm_rate_spin.GetPos(),NULL);
}

void CTeamControlDlg::OnTimer(UINT nIDEvent) 
{
    cycle_task();
	CDialog::OnTimer(nIDEvent);
}

void CTeamControlDlg::cycle_task()
{
    unsigned int old_cyclerobot=cyclerobot;
    cyclerobot= (cyclerobot+1)%robots.size();
    
	if( !(rrd_widget->IsWindowVisible() )|| old_cyclerobot != refrobot )
		robots[old_cyclerobot]->robot_data(false);
	robots[cyclerobot]->robot_data(true);
	
	if (refbox_widget->refbox_client) 
	{
		RefboxSignal sig = refbox_widget->refbox_client->listen();
        refbox_widget->ChangeStatusCol();
		refboxSignalChanged(sig,0);
	}
    
    for (unsigned int i1=0; i1<robots.size(); i1++)
      robots[i1]->communicate(refrobot);
	
	const PaintPreferences& ppref = (field_widget->fw->get_preferences());
	refrobot = ppref.reference_robot;
	bool show_obstacles = ppref.show_wm_obstacles;
	bool show_vis_objects = ppref.show_vis_obstacles || ppref.show_vis_lines || 
		ppref.show_vis_ball || ppref.show_vis_goals;
	for (unsigned int i2=0; i2<robots.size(); i2++) 
	{
		robots[i2]->obstacle_data (show_obstacles && i2==ppref.reference_robot);
		robots[i2]->visible_object_request (show_vis_objects && i2==ppref.reference_robot);
	}
	
	CycleInfo ci;
	ci.rloc_vis.resize(robots.size());
	ci.bloc_vis.resize(robots.size());
	for (unsigned int i=0; i<robots.size(); i++) 
	{
		const RemoteRobotState& rrs = (robots[i]->get_state());
		if (rrs.comm_okay) 
		{
			ci.rloc_vis[i]=rrs.robot_pos;
			ci.bloc_vis[i]=rrs.ball_pos;
		}
		else 
		{
			ci.rloc_vis[i].pos=Vec (100000,100000);
			ci.rloc_vis[i].vtrans=Vec(0,0);
			ci.rloc_vis[i].stuck.robot_stuck=false;
			ci.bloc_vis[i].pos=Vec (100000,100000);
			ci.bloc_vis[i].velocity=Vec(0,0);
			ci.bloc_vis[i].pos_known = BallLocation::unknown;
		}
	}
	ci.oloc = robots[ppref.reference_robot]->get_state().obstacles;
	ci.vloc = robots[ppref.reference_robot]->get_state().visible_objects;
	
	field_widget->fw->next_cycle(ci);

	if (rrd_widget->IsWindowVisible())
	{
		robots[refrobot]->robot_data(true);
		rrd_widget->update(robots[refrobot]->get_state().robot_data );
	}
	else
		robots[refrobot]->robot_data(false);

	RemoteTeamState teamstate (robots.size());
	for (unsigned int i3=0; i3<robots.size(); i3++) 
	{
		teamstate.robot_states[i3] = &robots[i3]->get_state();
		teamstate.message_boards[i3] = &robots[i3]->get_message_board();
	}
	if (refstate_machine)
		teamstate.refstate = refstate_machine->get_state();
	if (coach_widget)
		coach_widget->update(teamstate);

	for (unsigned int i4=0; i4<robots.size(); i4++) 
	{
		if (robots[i4]->get_state().comm_okay)
		{
			if(robots[i4]->get_state().fieldgeometry != fieldgeometry )
			{
				CString s;
				s.Format("NOTE: Field geometry of %s and team control do not fit!", robots[i4]->get_state().name.c_str() );
				m_StatusBar->SetText(s,0,0);
			}
		}
	}
	
	robots[refrobot]->showIncomingMsg(refrobot);
}

HBRUSH CTeamControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    
	if ( pWnd->GetDlgCtrlID() == IDC_COMMRATE_EDIT ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(COL_WHITE);
	}

	return hbr;
}

LRESULT CTeamControlDlg::refboxSignalChanged(WPARAM wParam, LPARAM lParam)
{
	RefboxSignal sig = RefboxSignal(wParam);
	RefboxSignal sig2 = refstate_machine->update(sig,Vec::zero_vector,false);
    refstate_machine->update(Vec::zero_vector,Vec::zero_vector,false);
	refbox_widget->refstateChanged(refstate_machine->get_state());

	refbox_widget->goalstatusChanged(sig2); // edit by lrd
	
	if ( sig != SIGnop ) 
	{
		for (unsigned int i=0; i<robots.size(); i++)
			robots[i]->refboxSig( sig );
	} 
	
	return 0;
}


LRESULT CTeamControlDlg::ownHalfChanged(WPARAM wParam, LPARAM lParam)
{
	int n = (int)wParam;
	refbox_widget->ownHalfChanged(n);
	dir_of_play.own_half = n;
    for (unsigned int i=0; i<robots.size(); i++)
		robots[i]->direction_of_play_changed (dir_of_play);

	return 0;
}

LRESULT CTeamControlDlg::teamColorChanged(WPARAM wParam, LPARAM lParam)
{
	int n = (int)wParam;
	refbox_widget->labelChanged(n);
    dir_of_play.team_color = n;
	refstate_machine->set_team_color(n);
    for (unsigned int i=0; i<robots.size(); i++)
       robots[i]->direction_of_play_changed (dir_of_play);
	return 0;
}

void CTeamControlDlg::reference_robot_changed() 
{
    int index = reference_select.GetCurSel();
	robots[refrobot]->no_joystick();
    robots[refrobot]->robot_data (false);
	refrobot = index;
    field_widget->fw->set_reference_robot(refrobot);
}

LRESULT CTeamControlDlg::slDisplacement(WPARAM wParam, LPARAM lParam) 
{
	const CycleInfo& ci = field_widget->fw->get_cycle_info ();
	unsigned int ref = field_widget->fw->get_preferences().reference_robot;
	robots[ref]->slDisplacement (ci.rloc_vis[ref].pos, ci.rloc_vis[ref].heading);
	return 0;
}

void CTeamControlDlg::cycleRateChanged() 
{
    int npos = comm_rate_spin.GetPos();
 	m_commrate = npos;
	KillTimer(0);
	SetTimer(0,m_commrate,NULL);
}

LRESULT CTeamControlDlg::robotDisplacement(WPARAM wParam, LPARAM lParam) 
{
	Vec p = *(Vec*)(wParam);
	Angle h = *((Angle*)(lParam));
	
	unsigned int ref = field_widget->fw->get_preferences().reference_robot;
	robots[ref]->get_message_board().publish_stream () << "GotoPos: " << static_cast<int>(p.x) <<
		' ' << static_cast<int>(p.y) << ' ' << static_cast<int>(h.get_deg()) << '\n';
	return 0;
}

// show windows through menu control
void CTeamControlDlg::OnPlayingField() 
{
	if (field_widget->IsWindowVisible())
		field_widget->ShowWindow(SW_HIDE);
	else
		field_widget->ShowWindow(SW_SHOW);
}

void CTeamControlDlg::OnRefereeStates() 
{
	if (refbox_widget->IsWindowVisible())
		refbox_widget->ShowWindow(SW_HIDE);
	else
		refbox_widget->ShowWindow(SW_SHOW);
}

void CTeamControlDlg::OnRobotData() 
{
	if (rrd_widget->IsWindowVisible())
		rrd_widget->ShowWindow(SW_HIDE);
	else
		rrd_widget->ShowWindow(SW_SHOW);
}

void CTeamControlDlg::OnTrainer() 
{
	if (coach_widget->IsWindowVisible())
		coach_widget->ShowWindow(SW_HIDE);
	else
		coach_widget->ShowWindow(SW_SHOW);
}



