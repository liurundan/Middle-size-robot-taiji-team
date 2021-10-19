#if !defined(AFX_COACHWIDGET_H__4A7AEB1B_5CFA_4209_A5E6_C657C255E9A1__INCLUDED_)
#define AFX_COACHWIDGET_H__4A7AEB1B_5CFA_4209_A5E6_C657C255E9A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoachWidget.h : header file
//
#include "Coach.h"
/////////////////////////////////////////////////////////////////////////////
// CoachWidget dialog

class CoachWidget : public CDialog
{
// Construction
public:
	CoachWidget(CWnd* pParent = NULL);   // standard constructor
    
	void init(ConfigReader& cfg);
	void update( RemoteTeamState& ts);

// Dialog Data
	//{{AFX_DATA(CoachWidget)
	enum { IDD = IDD_COACHWIDGET };
	CStatic	message_edit;
	CButton	owns_ball_mode_check;
	CButton	comm_ball_mode_check;
	CButton	broadcast_mode_check;
	CComboBox	policy_combo;
	//}}AFX_DATA

public:
	Coach*      coach;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CoachWidget)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CoachWidget)
	afx_msg void OnDestroy();
	afx_msg void owns_ball_mode_toggled();
	afx_msg void comm_ball_mode_toggled();
	afx_msg void broadcast_mode_toggled();
	afx_msg void policyChange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COACHWIDGET_H__4A7AEB1B_5CFA_4209_A5E6_C657C255E9A1__INCLUDED_)
