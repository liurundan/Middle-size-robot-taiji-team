// RemoteRobotWidget.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "RemoteRobotWidget.h"
#include "Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RemoteRobotWidget dialog


RemoteRobotWidget::RemoteRobotWidget(CWnd* pParent /*=NULL*/)
: CDialog(RemoteRobotWidget::IDD, pParent)
{
	//{{AFX_DATA_INIT(RemoteRobotWidget)
	//}}AFX_DATA_INIT
	col_ipedit			= COL_RED;
	col_nameedit		= COL_RED;
	col_ownhalfedit		= COL_CYAN;
	col_teamedit		= COL_MAGENTA;
	col_vcc				= COL_RED;
    vcc					= 0.0;
	robot_col.clear();	
	
	joystick_widget     = NULL;
    message_widget      = NULL;
	robot_state			= NULL;
	robot_type			= NULL;
	
}

RemoteRobotWidget::~RemoteRobotWidget()
{
	if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		m_StatusBar = NULL;
	}
	for (unsigned int i = 0; i<robot_col.size(); i++)
	{
		robot_col[i]->DestroyWindow();
		delete robot_col[i];
		robot_col[i] =  NULL;
	}
	
	if (joystick_widget)
	{
		joystick_widget->DestroyWindow();
		delete joystick_widget;
		joystick_widget = NULL;
    }	
	
	if (message_widget)
	{
		message_widget->DestroyWindow();
		delete message_widget;
		message_widget = NULL;
    }
	
	if (robot_state)
	{
		robot_state->DestroyWindow();
		delete robot_state;
		robot_state = NULL;
    }	
	if (robot_type)
	{
		robot_type->DestroyWindow();
		delete robot_type;
		robot_type = NULL;
    }	
	
}

void RemoteRobotWidget::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RemoteRobotWidget)
	DDX_Control(pDX, IDC_MESSAGE_CHECK, message_check);
	DDX_Control(pDX, IDC_JOYSTICK_CHECK, joystick_check);
	DDX_Control(pDX, IDC_ACTIVATE_CHECK, activate);
	DDX_Control(pDX, IDC_CONNECTROBOT_CHECK, connect_robot);
	DDX_Control(pDX, IDC_IP_EDIT, ip_edit);
	DDX_Control(pDX, IDC_ROLE_SELECT, role_select);
	DDX_Control(pDX, IDC_REFSTATE_SELECT, refstate_select);
	DDX_Control(pDX, IDC_PLAYER_SELECT, player_select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RemoteRobotWidget, CDialog)
ON_MESSAGE( closejoydialog, CloseJoyDialog )
ON_MESSAGE( closeMsgdialog, CloseMsgDialog )

//{{AFX_MSG_MAP(RemoteRobotWidget)
ON_WM_DESTROY()
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_CHANGETEAM_BUTTON, change_team_clicked)
ON_CBN_SELCHANGE(IDC_REFSTATE_SELECT, refstateChanged)
ON_CBN_SELCHANGE(IDC_PLAYER_SELECT, playerChanged)
ON_CBN_SELCHANGE(IDC_ROLE_SELECT, roleChanged)
ON_BN_CLICKED(IDC_CONNECTROBOT_CHECK, OnConnectrobotCheck)
ON_BN_CLICKED(IDC_ACTIVATE_CHECK, OnActivateCheck)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RemoteRobotWidget message handlers

BOOL RemoteRobotWidget::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );
	
	for (unsigned int k = 0; k<5; k++)
	{
		StatusColor* rc = new StatusColor(100,20);
		rc->Create( NULL, NULL,                                               
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
			CRect(0,0,100,20), this, 0 );
		robot_col.push_back(rc);
		rc->ShowWindow(SW_SHOW);
	}
    robot_col[0]->SetStatusColor(COL_RED,"Communication");
	robot_col[0]->MoveWindow(5,8,100,20);
    robot_col[1]->SetStatusColor(COL_RED,state.name.c_str());
	robot_col[1]->MoveWindow(5,38,100,20);
	
    robot_col[2]->SetStatusColor(COL_RED,"cyan");
	robot_col[2]->MoveWindow(285,88,100,20);
    robot_col[3]->SetStatusColor(COL_RED,"yellow");
	robot_col[3]->MoveWindow(285,108,100,20);
	
	joystick_check.EnableWindow(FALSE);
	message_check.EnableWindow(FALSE);
	
	CString s;
	s.Format("VCC: %.1f V", vcc);
    robot_col[4]->SetStatusColor(COL_RED,s);
	robot_col[4]->MoveWindow(108,38,100,20);
    
	unsigned int wsc = 125;
	robot_state =  new StatusColor(wsc,20);
	robot_state->Create( NULL, NULL,                                               
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
		CRect(0,0,wsc,20), this, 0 );
	robot_type =  new StatusColor(wsc,20);
	robot_type->Create( NULL, NULL,                                               
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
		CRect(0,0,wsc,20), this, 0 );
	robot_type->MoveWindow(210,29,wsc,20);
	robot_state->MoveWindow(210,49,wsc,20);
	
	
	comm = NULL;
	rsm  = NULL;
	state.comm_started = state.comm_okay = false;
	state.in_game = false;
	state.tribots_team.team_color=1;
	state.tribots_team.own_half=1;
	state.team = 1;
	state.playertype = "";
	state.playerrole = "";
	state.refstate = errorState;
	state.use_joystick = false;
	state.robot_data_request =false;
	state.obstacle_data_request = false;
	
    for(int i=0; i<num_referee_states; i++)
        refstate_select.InsertString(i,referee_state_names [i]);
	
    for (unsigned int j=0; j<num_roles; j++)
        role_select.InsertString( j, playerRoleNames[j]);
	
	numComNotOK=100;
	return TRUE;  
}

HBRUSH RemoteRobotWidget::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( pWnd->GetDlgCtrlID() == IDC_NAME_EDIT ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(col_nameedit);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_TEAMCOLOR_EDIT ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(col_teamedit);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_OWNHALF_EDIT ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(col_ownhalfedit);
	}
	if ( pWnd->GetDlgCtrlID() == IDC_IP_EDIT || pWnd->GetDlgCtrlID() == IDC_ROBOT_PLAYER_TYPE ||
		pWnd->GetDlgCtrlID() == IDC_ROBOT_REFSTATE ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(COL_WHITE); 
	}
	
	if ( pWnd->GetDlgCtrlID() == IDC_VCC_VIEW ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(col_vcc); 
	}
	
	return hbr;
}

void RemoteRobotWidget::OnDestroy() 
{
	CDialog::OnDestroy();

	if (comm) delete comm;
	if (rsm) delete rsm;	
}

void RemoteRobotWidget::OnActivateCheck() 
{
	bool b = activate.GetCheck() > 0 ? true:false;
	if (b)
		enableClicked();
	else
		disableClicked();	
}

void RemoteRobotWidget::enableClicked() 
{
	bool b = true;
	if (comm)
	{
		comm->putInGame (b);
	}	
}

void RemoteRobotWidget::disableClicked() 
{
	bool b = false;
	if (comm)
		comm->putInGame (b);	
}

void RemoteRobotWidget::OnConnectrobotCheck() 
{
	bool b = connect_robot.GetCheck() > 0 ? true:false;
	if (b)
        connectClicked();
	else
		disconnectClicked();
}

void RemoteRobotWidget::connectClicked() 
{
    if (!comm) 
	{
		state.ip = robot_ip.GetBuffer(0);
		int posdoppelpunkt = state.ip.find(":");
		if( posdoppelpunkt > 1 ) 
		{
			int used_port  = state.port;
			string portstr = string(state.ip.substr(posdoppelpunkt+1,state.ip.length()-posdoppelpunkt));
			used_port = atoi(portstr.c_str());
			state.ip  = state.ip.substr(0,posdoppelpunkt);
			
			CString str;
			str.Format(" IP  :%s,  Port:  %d", state.ip.c_str(), used_port );
			m_StatusBar->SetText(str,0,0);
			
			comm = new RobotsUdpCommunication(false, used_port, state.ip.c_str());
		} 
		else 
		{
			comm = new RobotsUdpCommunication(false, state.port, state.ip.c_str());
		}
		
		if (comm) 
		{
			state.comm_started=true;
			
			comm->clear_to_send();
			comm->clearRequests();
		}
	}	
	update_display();
}

void RemoteRobotWidget::disconnectClicked() 
{
	if (comm)
		delete comm;
	comm = NULL;
	state.comm_started = false;
	state.comm_okay	 = false;
	state.in_game	 = false;
	update_display();	
}

void RemoteRobotWidget::refboxSig(RefboxSignal sig)
{
	Vec no_good;
	RefboxSignal rs = rsm->update (sig, no_good, false);
	if (comm)
		comm->putRefboxSignal(rs);
	
}

void RemoteRobotWidget::playerChanged() 
{
	int index = player_select.GetCurSel();
	CString str;
	player_select.GetLBText(index,str);
	if (comm)
		comm->putPlayerType(str.GetBuffer(0));
}

void RemoteRobotWidget::refstateChanged() 
{
	int index = refstate_select.GetCurSel();
	RefereeState rs = RefereeState(index);
	switch (rs) 
	{
	case stopRobot: refboxSig (SIGstop); break;
	case freePlay: refboxSig (SIGstart); break;
	case preOwnKickOff: refboxSig (SIGownKickOff); break;
	case preOpponentKickOff: refboxSig (SIGopponentKickOff); break;
	case preOwnFreeKick: refboxSig (SIGownFreeKick); break;
	case preOpponentFreeKick: refboxSig (SIGopponentFreeKick); break;
	case preOwnGoalKick: refboxSig (SIGownGoalKick); break;
	case preOpponentGoalKick: refboxSig (SIGopponentGoalKick); break;
	case preOwnCornerKick: refboxSig (SIGownCornerKick); break;
	case preOpponentCornerKick: refboxSig (SIGopponentCornerKick); break;
	case preOwnThrowIn: refboxSig (SIGownThrowIn); break;
	case preOpponentThrowIn: refboxSig (SIGopponentThrowIn); break;
	case preOwnPenalty: refboxSig (SIGownPenalty); break;
	case preOpponentPenalty: refboxSig (SIGopponentPenalty); break;
	case postOpponentKickOff: case postOpponentGoalKick: 
	case postOpponentCornerKick: case postOpponentThrowIn:
	case postOpponentPenalty: case postOpponentFreeKick: 
	case ownPenalty: case opponentPenalty: refboxSig (SIGready); break;
	case errorState: refboxSig (SIGstop); break;
	}
}

void RemoteRobotWidget::communicate(unsigned int rid)
{
//	if (!IsWindowVisible())
//	{
//		hide();
//	}
	CString srid;
	srid.Format("Robot%d",rid+1);
    CString robname = state.name.c_str(); 
	if( srid == robname ) 
	{
        joystick_check.EnableWindow(TRUE);
		message_check.EnableWindow(TRUE);
		bool b = joystick_check.GetCheck() > 0 ? true : false;
		if(b)
		{
			getJoystickDialog()->ShowWindow(SW_SHOW);
			DriveVector dv;
			getJoystickDialog()->getDriveVectorFromJoystick(dv);
			joystick(dv);
		}
		else
		{
			no_joystick();
			getJoystickDialog()->ShowWindow(SW_HIDE);
		}
		
		if( message_check.GetCheck() )
		{
			getMessageBoard()->ShowWindow(SW_SHOW);
			CString s;
			for (unsigned int i=0; i<get_message_board().get_outgoing().size(); i++)
			{
				s += "MessageBoard exit:\n";
				CString stmp;
				stmp.Format("%s    ",get_message_board().get_outgoing()[i].c_str());
				s +=  stmp;
				s += '\n';
				getMessageBoard()->SetMessage(s);
			}
		}
		else
			getMessageBoard()->ShowWindow(SW_HIDE);
		
	}
	else
	{
        joystick_check.EnableWindow(FALSE);
		message_check.EnableWindow(FALSE);
	}
	
	bool extended_request;
	if( !state.comm_okay )
	{
		if( numComNotOK > 4 ) 
			extended_request = true;
		else 
			extended_request = false;
		numComNotOK++;
	} 
	else
	{
		numComNotOK = 0;
		extended_request = false;
	}
	
	if (comm && state.comm_started)
	{
		if (state.use_joystick)
			comm->putRemoteCtr(state.joy_drive);
		if (state.mboard.get_outgoing().size()>0)
			comm->putMessageList (state.mboard.get_outgoing());
		state.mboard.clear();
		
		comm->setGameStateRequest();
		comm->setOwnHalfRequest();
		comm->setBallLocationRequest();
		comm->setRobotLocationRequest();
		comm->setPlayerTypeRequest();
		
		if (extended_request) 
		{
			comm->setPlayerTypeListRequest();
			comm->setFieldGeometryRequest();
		}
		
		if( state.obstacle_data_request )
			comm->setObstacleLocationRequest();
		if( state.visible_object_request )
			comm->setVisibleObjectListRequest();
		
		if (state.robot_data_request)
			comm->setRobotDataRequest();
		
		if (extended_request)	
			comm->putRequests(true); 
		else 	
			comm->putRequests();
		
		comm->send();
		state.comm_okay = comm->receive_all();
		
		if (state.comm_okay) 
		{
			std::vector<std::string> ptl;
			if (comm->getPlayerTypeList(ptl)) 
			{
				bool pltlist_changed = ( ptl.size() != static_cast<unsigned int>(player_select.GetCount()) );
				if (!pltlist_changed) 
				{
					for (unsigned int i=0; i<static_cast<unsigned int>(player_select.GetCount()); i++) 
					{
						bool entry_found=false;
						for (unsigned int j=0; j<ptl.size(); j++)
						{
							CString str;
							player_select.GetLBText(j,str);
							std::string s = str.GetBuffer(0);
							if( ptl[j] == s ) //std::string(player_select->text(j).ascii())) 
							{
								entry_found = true;
								break;
							}
							if( !entry_found ) 
							{
								pltlist_changed=true;
								break;
							}
						}
					}
				}
				if (pltlist_changed) 
				{
					player_select.Clear();
					for (unsigned int i=0; i<ptl.size(); i++) 
					{
						player_select.InsertString(i,ptl[i].c_str());
						//player_select->insertItem (ptl[i].c_str());
					}
				}
			}
			comm->getFieldGeometry(state.fieldgeometry);
			
			if (state.obstacle_data_request) 
			{
				comm->getObstacleLocation(state.obstacles);
			}
			
			if (state.visible_object_request) 
			{
				comm->getVisibleObjectList (state.visible_objects);
			}
			
			// GameState
			GameState gs;
			if (comm->getGameState(gs)) 
			{
				state.refstate=gs.refstate;
				state.in_game=gs.in_game;
			} 
			else 
			{
				state.refstate = errorState;
				state.in_game  = false;
			}
			
			// OwnHalf
			int oh;
			if (comm->getOwnHalf (oh)) 
			{
				if (oh!=state.team*state.tribots_team.own_half) 
				{
					state.team *= -1;
					rsm->set_team_color (state.team*state.tribots_team.team_color);
				}
			}
			
			comm->getBallLocation(state.ball_pos);
			comm->getRobotLocation(state.robot_pos);
			
			// PlayerType
			char buffer[200];
			comm->getPlayerType(buffer);
			if (state.playertype != buffer ) 
			{
				state.playertype = buffer;
				unsigned int index=0;
				for (unsigned int j=0; j<static_cast<unsigned int>(player_select.GetCount()); j++)
				{
					CString str;
					player_select.GetLBText(j,str);
					std::string s = str.GetBuffer(0);
					
					if( std::string(buffer) == s ) 
					{
						index=j;
						break;
					}
					player_select.SetCurSel(index);
					//player_select->setCurrentItem (index);
				}
			}
			
			if (comm->getRobotData(state.robot_data)) 
			{
				// vcc_view->display (static_cast<int>(state.robot_data.motor_vcc));
				CString stmp;
				if( state.robot_data.motors_on == false)
				{
				   stmp.Format("VCC: %.1f V, USB lost!!!", state.robot_data.motor_vcc );
				}
				else
				   stmp.Format("VCC: %.1f V", state.robot_data.motor_vcc );
				
				static double vcc = state.robot_data.motor_vcc;
				bool bcommwork = true;
				if( vcc == state.robot_data.motor_vcc ) 
				{
					if( update_time.elapsed_msec() > 5000 ) 
                        bcommwork = false;
				}
				else
				{
					update_time.update();
					bcommwork = true;
				}
				vcc = state.robot_data.motor_vcc;

				if( state.robot_data.motor_vcc < 23 )
					robot_col[4]->SetStatusColor(COL_RED,stmp);
				else if( state.robot_data.motor_vcc < 24 )
					robot_col[4]->SetStatusColor(COL_YELLOW,stmp);
				else
					robot_col[4]->SetStatusColor(COL_GREEN,stmp);
			}
			
			// Messages
			std::vector<std::string> msgList;
			comm->getMessageList (msgList);
			state.mboard.receive (msgList);
			std::string mline = state.mboard.scan_for_prefix ("NewRole:");
			if (mline.length()>0) 
			{
				std::deque<std::string> parts;
				split_string (parts, mline);
				if (mline.size()>1) 
				{
					int pindex = role_select.SelectString(0,parts[1].c_str());
					role_select.SetCurSel(pindex);
					state.playerrole = parts[1];
				}
			}
			
		}
		
		comm->clear_to_send();
		comm->clearRequests();
  } 
  else 
  {
	  state.mboard.clear();
  }
  update_display ();  
  
}

void RemoteRobotWidget::showIncomingMsg(unsigned int rid /* = 1 */)
{
	CString srid;
	srid.Format("Robot%d",rid+1);
    CString robname = state.name.c_str(); 
	if( srid == robname ) 
	{
		message_check.EnableWindow(TRUE);
		if( message_check.GetCheck() )
		{
			getMessageBoard()->ShowWindow(SW_SHOW);
			CString s;
			for (unsigned int i=0; i<get_message_board().get_incoming().size(); i++)
			{
				s += "MessageBoard entrance:\n";
				CString stmp;
				stmp.Format("%s    ",get_message_board().get_incoming()[i].c_str());
				s +=  stmp;
				s += '\n';
				getMessageBoard()->SetMessage(s);
			}
		}
		else
			getMessageBoard()->ShowWindow(SW_HIDE);
	}
	else
		message_check.EnableWindow(FALSE);
}

void RemoteRobotWidget::init_robot(ConfigReader& cfg, const FieldGeometry& fg, const char* rid)
{
    state.name = rid;
	state.ip   = "";
	state.port = 6012;
	
	cfg.get ((string(rid)+string("::name")).c_str(), state.name);
	cfg.get ((string(rid)+string("::ip")).c_str(), state.ip);
	cfg.get ((string(rid)+string("::port")).c_str(), state.port);
	
    SetWindowText(state.name.c_str());
    robot_ip = state.ip.c_str();
    ip_edit.SetWindowText(robot_ip);
	
	rsm = new RefereeStateMachine(fg);
	rsm->set_team_color(state.team*state.tribots_team.team_color);
}

void RemoteRobotWidget::update_display()
{
	//  name_edit = state.name.c_str();
	if (state.comm_started) 
	{
		if (state.comm_okay)
			col_ipedit = COL_GREEN;
		else if (numComNotOK < 4) 
			col_ipedit = COL_DARKGREEN;
		else
			col_ipedit = COL_BLUE;
	}
	else
		col_ipedit = COL_RED;

	if (state.in_game) activate.SetCheck(1);
	else               activate.SetCheck(0);
	
	col_nameedit    = (state.comm_started ? (state.in_game ? COL_GREEN : COL_RED) : COL_WHITE);
	col_ownhalfedit = (state.team*state.tribots_team.own_half>0 ? COL_YELLOW : COL_BLUE);
	col_teamedit    = (state.team*state.tribots_team.team_color>0 ? COL_CYAN : COL_MAGENTA);
	
	robot_col[0]->SetStatusColor(col_ipedit,"Communication");
	robot_col[1]->SetStatusColor(col_nameedit,state.name.c_str());
	
	CString  own_half_edit   = (state.team*state.tribots_team.own_half>0 ? "yellow" : "blue");
	CString  teamcolor_edit  = (state.team*state.tribots_team.team_color>0 ? "cyan" : "magenta");
	
	robot_col[2]->SetStatusColor(col_teamedit,teamcolor_edit);
	robot_col[3]->SetStatusColor(col_ownhalfedit,own_half_edit);
	
	CString rsn = referee_state_names[state.refstate];
	int pos = refstate_select.SelectString(0,rsn);
	refstate_select.SetCurSel(pos);
	int pos1 = player_select.SelectString(0,state.playertype.c_str());
	player_select.SetCurSel(pos1);
	// ref_state.SetWindowText(rsn);
	// player_type.SetWindowText(state.playertype.c_str());
//	robot_state->SetStatusColor(COL_WHITE,rsn);
//	robot_type->SetStatusColor(COL_WHITE,state.playertype.c_str());
	CString ball_pos_str,robot_pos_str;
	ball_pos_str.Format("Ball(%.f, %.f)",state.ball_pos.pos.x,state.ball_pos.pos.y);
	robot_pos_str.Format("Pos(%.f, %.f, %.f)",state.robot_pos.pos.x,state.robot_pos.pos.y,state.robot_pos.heading.get_deg());
	robot_state->SetStatusColor(COL_WHITE,ball_pos_str);
	robot_type->SetStatusColor(COL_WHITE,robot_pos_str);
	
}

void RemoteRobotWidget::change_team_clicked() 
{
	int n = -state.team*state.tribots_team.own_half;
	if (comm)
		comm->putOwnHalf (n);
	
}

void RemoteRobotWidget::direction_of_play_changed(DirectionOfPlay dp)
{
	if (dp.own_half!=state.tribots_team.own_half || dp.team_color!=state.tribots_team.team_color) 
	{
		if (dp.team_color!=state.tribots_team.team_color)
			rsm->set_team_color (-state.team*state.tribots_team.team_color);
		state.tribots_team=dp;
		int n = state.team*state.tribots_team.own_half;
		if (comm)
			comm->putOwnHalf (n);
	}
}

const RemoteRobotState& RemoteRobotWidget::get_state()
{
    return state;
}

void RemoteRobotWidget::slDisplacement(Vec p, Angle h)
{
	if(comm) 
	{
		float x = p.x;
		float y = p.y;
		float a = h.get_rad();
		comm->putSLHint (x, y, a);
	}
}

void RemoteRobotWidget::joystick(DriveVector dv)
{
	state.use_joystick = true;
	state.joy_drive = dv;
}

void RemoteRobotWidget::no_joystick()
{
	state.use_joystick=false;
}

void RemoteRobotWidget::robot_data(bool b)
{
	state.robot_data_request = b;
}


MessageBoard& RemoteRobotWidget::get_message_board()
{
	return state.mboard;
}

void RemoteRobotWidget::obstacle_data(bool b)
{
	state.obstacle_data_request = b;
}

void RemoteRobotWidget::visible_object_request(bool b)
{
	state.visible_object_request = b;
}

void RemoteRobotWidget::hide()
{
	state.in_game=false;
	joystick_check.SetCheck(0);
	message_check.SetCheck(0);
	connect_robot.SetCheck(0);
	disconnectClicked();
	::ShowWindow(this->m_hWnd,SW_HIDE);
}

void RemoteRobotWidget::roleChanged() 
{
	int index = role_select.GetCurSel();
	CString str;
	role_select.GetLBText(index,str);	
	state.mboard.publish_stream() << "ChangeRole: " << str.GetBuffer(0) << '\n';
}

JoystickDialog* RemoteRobotWidget::getJoystickDialog()
{
	while (joystick_widget == NULL)
	{
		joystick_widget = new JoystickDialog;
		joystick_widget->Create(IDD_JOYSTICKDIALOG,this);
		
		CRect rc_js;
		joystick_widget->GetClientRect(&rc_js);
		joystick_widget->MoveWindow(350,0,rc_js.Width(),rc_js.Height()+30);
	}
	return joystick_widget;
}

MessageDialog* RemoteRobotWidget::getMessageBoard()
{
	while (message_widget == NULL)
	{
		message_widget = new MessageDialog(this);
		message_widget->Create(IDD_MESSAGE_BOARD_DIALOG,this);
	}
	return message_widget;
	
}

LRESULT RemoteRobotWidget::CloseJoyDialog( WPARAM wParam, LPARAM lParam )
{
	if (joystick_check.GetCheck() )
	{
		joystick_check.SetCheck(0);
	}
	return 0;
}

LRESULT RemoteRobotWidget::CloseMsgDialog( WPARAM wParam, LPARAM lParam )
{
	if (message_check.GetCheck() )
	{
		message_check.SetCheck(0);
	}
	return 0;
}

