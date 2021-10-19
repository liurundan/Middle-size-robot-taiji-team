#if !defined(AFX_REFBOXWIDGET_H__4A5462DA_21A9_409D_82BE_25AB126D41C8__INCLUDED_)
#define AFX_REFBOXWIDGET_H__4A5462DA_21A9_409D_82BE_25AB126D41C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RefboxWidget.h : header file
//
#include "ConfigReader.h"
#include "RefboxClient.h"
#include "GameState.h"
#include "StatusColor.h"
#include <deque>

#define		ownHalfSelect		WM_USER+100
#define		labelSelect			WM_USER+101
#define		refboxSignal		WM_USER+102

/////////////////////////////////////////////////////////////////////////////
// RefboxWidget dialog
class RefboxWidget : public CDialog
{
// Construction
public:
	void ownHalfChanged(int n);
	void labelChanged(int n);
	void refstateChanged( RefereeState rs );
	void goalstatusChanged( RefboxSignal rsg );
	RefboxWidget(CWnd* pParent = NULL);   // standard constructor

    virtual ~RefboxWidget();
	
	void init(ConfigReader&);
	void disconnectRefboxClient();
	void connectRefboxClient();
    void ChangeStatusCol();
// Dialog Data
	//{{AFX_DATA(RefboxWidget)
	enum { IDD = IDD_REFBOXWIDGET };
	CStatic	refbox_ip;
	CButton	refbox_connect;
	//}}AFX_DATA

public:
    RefboxClient*				refbox_client;
private:
	int							team_color;
	int							own_half;
	RefereeState				refstate;
	COLORREF					owngoal, oppgoal;
	COLORREF					ownlabel, opplable;
	CWnd*						m_pOwner;
    
	int							ownGoalscore;
	int							oppGoalscore;

	CString						refbox_ip_edit;
	int							refbox_port;
    StatusColor*				statuscolor;
	CStatusBarCtrl*				m_StatusBar;
    
	std::deque<StatusColor*>    disp_msg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RefboxWidget)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT outconnectMessage(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Generated message map functions
	//{{AFX_MSG(RefboxWidget)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void changeOwnHalfPressed();
	afx_msg void changeLabelPressed();
	afx_msg void stopPressed();
	afx_msg void readyPressed();
	afx_msg void startPressed();
	afx_msg void kickOffOwnPressed();
	afx_msg void throwInOwnPressed();
	afx_msg void goalKickOwnPressed();
	afx_msg void cornerKickOwnPressed();
	afx_msg void freeKickOwnPressed();
	afx_msg void penaltyKickOwnPressed();
	afx_msg void kickOffOpponentPressed();
	afx_msg void throwInOpponentPressed();
	afx_msg void goalKickOpponentPressed();
	afx_msg void cornerKickOpponentPressed();
	afx_msg void freeKickOpponentPressed();
	afx_msg void penaltyKickOpponentPressed();
	afx_msg void refboxConnectionChanged();
	afx_msg void droppedBallPressed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFBOXWIDGET_H__4A5462DA_21A9_409D_82BE_25AB126D41C8__INCLUDED_)
