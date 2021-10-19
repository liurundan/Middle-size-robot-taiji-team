// RobotControlDlg.h : header file
//

#if !defined(AFX_ROBOTCONTROLDLG_H__E75F055A_B849_48ED_99FF_DF23DE16C6B0__INCLUDED_)
#define AFX_ROBOTCONTROLDLG_H__E75F055A_B849_48ED_99FF_DF23DE16C6B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlLoop.h"
#include "Journal.h"
#include "random.h"
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDlg dialog

class CRobotControlDlg : public CDialog
{
// Construction
public:
	CRobotControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRobotControlDlg)
	enum { IDD = IDD_ROBOTCONTROL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
    ControlLoop*		the_control_loop;
    ConfigReader		vread;

	CDC*				m_cdc;
	CBrush				brush;

// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
	//{{AFX_MSG(CRobotControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCONTROLDLG_H__E75F055A_B849_48ED_99FF_DF23DE16C6B0__INCLUDED_)
