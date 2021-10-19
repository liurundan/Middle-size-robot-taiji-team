#if !defined(AFX_REMOTEROBOTWIDGET_H__75AF111B_F178_48C4_88D4_764158ECA73D__INCLUDED_)
#define AFX_REMOTEROBOTWIDGET_H__75AF111B_F178_48C4_88D4_764158ECA73D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteRobotWidget.h : header file
//

#include "RemoteRobotState.h"
#include "ConfigReader.h"
#include "stringconvert.h"
#include "PlayerRole.h"
#include "RobotsUdpCommunication.h"
#include "RefereeStateMachine.h"
#include "StatusColor.h"
#include "JoystickDialog.h"
#include "MessageDialog.h"
#include <deque>
/////////////////////////////////////////////////////////////////////////////
// RemoteRobotWidget dialog

class RemoteRobotWidget : public CDialog
{
// Construction
public:
	RemoteRobotWidget(CWnd* pParent = NULL);   // standard constructor
    virtual ~RemoteRobotWidget();
	void hide();
	void visible_object_request( bool b);
	void obstacle_data( bool b);
	MessageBoard& get_message_board ();
	void robot_data (bool b);
	void no_joystick();
	void joystick(DriveVector dv);
	void slDisplacement(Vec p, Angle h);
	void communicate(unsigned int rid = 1 );
	void refboxSig( RefboxSignal sig);
	void update_display();
	const RemoteRobotState& get_state();
	void direction_of_play_changed( DirectionOfPlay dp);
	void init_robot( ConfigReader& cfg, const FieldGeometry& fg, const char* rid);
	void enableClicked();
	void disableClicked();
	void connectClicked();
	void disconnectClicked();
	
	void showIncomingMsg(unsigned int rid = 1);
	JoystickDialog* getJoystickDialog();
	MessageDialog*	getMessageBoard();
// Dialog Data
	//{{AFX_DATA(RemoteRobotWidget)
	enum { IDD = IDD_REMOTEROBOTWIDGET };
	CButton	message_check;
	CButton	joystick_check;
	CButton	activate;
	CButton	connect_robot;
	CStatic	ip_edit;
	CComboBox	role_select;
	CComboBox	refstate_select;
	CComboBox	player_select;
	//}}AFX_DATA

private:

	RobotsUdpCommunication*			comm;
	RemoteRobotState				state;
	RefereeStateMachine*			rsm;
	int								numComNotOK;
 
	JoystickDialog*				    joystick_widget;
	MessageDialog*					message_widget;
    COLORREF						col_ipedit;
    COLORREF						col_nameedit;
    COLORREF						col_ownhalfedit;
    COLORREF						col_teamedit;
    COLORREF						col_vcc;
	
    CStatusBarCtrl*					m_StatusBar;

	deque<StatusColor*>             robot_col;
	CString                         robot_ip;
	CString							robot_name;
	double							vcc;
	StatusColor*                    robot_state;
	StatusColor*                    robot_type;

	Time							update_time;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RemoteRobotWidget)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    afx_msg LRESULT CloseJoyDialog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT CloseMsgDialog(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(RemoteRobotWidget)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void change_team_clicked();
	afx_msg void refstateChanged();
	afx_msg void playerChanged();
	afx_msg void roleChanged();
	afx_msg void OnConnectrobotCheck();
	afx_msg void OnActivateCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEROBOTWIDGET_H__75AF111B_F178_48C4_88D4_764158ECA73D__INCLUDED_)
