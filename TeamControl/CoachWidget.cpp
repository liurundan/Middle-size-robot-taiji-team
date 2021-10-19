// CoachWidget.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "CoachWidget.h"
#include "Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CoachWidget dialog


CoachWidget::CoachWidget(CWnd* pParent /*=NULL*/)
	: CDialog(CoachWidget::IDD, pParent)
{
	//{{AFX_DATA_INIT(CoachWidget)
	//}}AFX_DATA_INIT
	coach = NULL;
	

}


void CoachWidget::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CoachWidget)
	DDX_Control(pDX, IDC_MESSAGE_EDIT, message_edit);
	DDX_Control(pDX, IDC_OWNS_BALL_CHECK, owns_ball_mode_check);
	DDX_Control(pDX, IDC_COMM_BALL_CHECK, comm_ball_mode_check);
	DDX_Control(pDX, IDC_BROADCAST_CHECK, broadcast_mode_check);
	DDX_Control(pDX, IDC_POLICY_COMBO, policy_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CoachWidget, CDialog)
	//{{AFX_MSG_MAP(CoachWidget)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OWNS_BALL_CHECK, owns_ball_mode_toggled)
	ON_BN_CLICKED(IDC_COMM_BALL_CHECK, comm_ball_mode_toggled)
	ON_BN_CLICKED(IDC_BROADCAST_CHECK, broadcast_mode_toggled)
	ON_CBN_SELCHANGE(IDC_POLICY_COMBO, policyChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CoachWidget message handlers

void CoachWidget::OnDestroy() 
{
	CDialog::OnDestroy();
	
    if (coach)
	{
		delete coach;
    }	
}

void CoachWidget::init(ConfigReader& cfg)
{
	std::vector<std::string> rids;
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
		rids.push_back(srole);
	}

	coach = new Coach (cfg, rids);
	comm_ball_mode_check.SetCheck(coach->get_comm_ball_mode());
	broadcast_mode_check.SetCheck(coach->get_broadcast_mode());
	owns_ball_mode_check.SetCheck(coach->get_owns_ball_mode());
	std::vector<std::string> policy_ids = coach->get_list_of_policies();

	policy_combo.InsertString(0,"---");
	for (unsigned int i=0; i<policy_ids.size(); i++)
		policy_combo.InsertString(i+1,policy_ids[i].c_str());
	std::vector<std::string> roles = coach->get_list_of_roles();
	if (coach->dynamic_role_change_mode().length()==0)
	{
		int inpos = policy_combo.SelectString(0,"---");
		policy_combo.SetCurSel(inpos);
	}
    else
	{
		string s = coach->dynamic_role_change_mode();
	 	int inpos = policy_combo.SelectString(0, const_cast<char*>(s.c_str()) );
	 	policy_combo.SetCurSel(inpos);
	}
	std::vector<std::string> all_ko_policies = coach->get_list_of_kick_off_procedures ();
    
	CString s = "ddd     ";
 	message_edit.SetWindowText(s);

}

void CoachWidget::update( RemoteTeamState& ts) 
{
	if (coach) 
		coach->update (ts);
	CString s;
	s.Format("%s",(coach->get_role_description()).c_str());
	message_edit.SetWindowText(s);
}


void CoachWidget::owns_ball_mode_toggled() 
{
	bool b = owns_ball_mode_check.GetCheck() > 0 ? true:false;
	if (coach) 
		coach->set_owns_ball_mode (b);
}

void CoachWidget::comm_ball_mode_toggled() 
{
	bool b = comm_ball_mode_check.GetCheck() > 0 ? true:false;
	if (coach) 
		coach->set_comm_ball_mode (b);
	
}

void CoachWidget::broadcast_mode_toggled() 
{
	bool b = broadcast_mode_check.GetCheck() > 0 ? true:false;
	if (coach) 
		coach->set_broadcast_mode (b);
	
}

void CoachWidget::policyChange() 
{
    int pc = policy_combo.GetCurSel();
	CString stmp;
	policy_combo.GetLBText(pc,stmp);
	if (coach) 
	{
		coach->dynamic_role_change_mode (stmp.GetBuffer(0));
		if (coach->dynamic_role_change_mode ().length()==0)
		{
			int pos = policy_combo.SelectString(0,"---");
			policy_combo.SetCurSel(pos);
		}
		else
		{
			int pos = policy_combo.SelectString(0,const_cast<char*>(coach->dynamic_role_change_mode().c_str()));
			policy_combo.SetCurSel(pos);
		}
	}

}

HBRUSH CoachWidget::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetDlgCtrlID() == IDC_MESSAGE_EDIT ) 
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(COL_WHITE);
	}
	return hbr;
}
