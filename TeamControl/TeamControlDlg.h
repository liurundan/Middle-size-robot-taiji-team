// TeamControlDlg.h : header file
//

#if !defined(AFX_TEAMCONTROLDLG_H__B60CBDCF_B0D9_4C3A_A57C_F90C15A304A7__INCLUDED_)
#define AFX_TEAMCONTROLDLG_H__B60CBDCF_B0D9_4C3A_A57C_F90C15A304A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTeamControlDlg dialog
#include "RemoteRobotWidget.h"
#include "RefboxWidget.h"
#include "RefereeStateMachine.h"
#include "FieldGeometry.h"
#include "Journal.h"
#include "EditPopup.h"
#include <deque>
#include "RealRobotData.h"
#include "FieldOfPlay.h"
#include "CoachWidget.h"
#include "EditPopup.h"

class CTeamControlDlg : public CDialog
{
// Construction
public:
	void cycle_task();
	void init();
	CTeamControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTeamControlDlg)
	enum { IDD = IDD_TEAMCONTROL_DIALOG };
	CComboBox	reference_select;
	CSpinButtonCtrl	comm_rate_spin;
	int		m_commrate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeamControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
   RefboxWidget*				refbox_widget;  
   RefereeStateMachine*			refstate_machine;
   FieldOfPlay*					field_widget;
   CoachWidget*					coach_widget;
   CRealRobotData*			    rrd_widget;
   deque<RemoteRobotWidget*>	robots;

   FieldGeometry				fieldgeometry;
   ConfigReader					cfg;
   string						cfg_file;

   DirectionOfPlay				dir_of_play;
   unsigned int					cyclerobot;
   unsigned int					refrobot;

   COLORREF						col_refboxipedit;
   
   CStatusBarCtrl*				m_StatusBar;
// Implementation

protected:
	HICON m_hIcon;
 
	afx_msg LRESULT slDisplacement(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT robotDisplacement(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu);     
	afx_msg void	OnDispSelectRobot(UINT nID);
	afx_msg void	OnUpdateSelectMenu(CCmdUI* pCmdUI);
	
	afx_msg LRESULT refboxSignalChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ownHalfChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT teamColorChanged(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(CTeamControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void cycleRateChanged();
	afx_msg void reference_robot_changed();
	afx_msg void OnPlayingField();
	afx_msg void OnRefereeStates();
	afx_msg void OnRobotData();
	afx_msg void OnTrainer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAMCONTROLDLG_H__B60CBDCF_B0D9_4C3A_A57C_F90C15A304A7__INCLUDED_)
