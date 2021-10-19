// SimulatorDlg.h : header file
//

#if !defined(AFX_SIMULATORDLG_H__E803D7DC_51A7_4B80_9639_4E58C3077EB3__INCLUDED_)
#define AFX_SIMULATORDLG_H__E803D7DC_51A7_4B80_9639_4E58C3077EB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 添加COpenGL类的说明文件
#include "Display.h"
#include "WorldModel.h"
#include "SoccerServer.h"
#include "Agent.h"

// test 
#include "ScanLines.h"
#include "FieldLUT.h"
#include "FieldGeometry.h"

/////////////////////////////////////////////////////////////////////////////
// CSimulatorDlg dialog

class CSimulatorDlg : public CDialog
{
// Construction
public:
	CSimulatorDlg(CWnd* pParent = NULL);	// standard constructor
 	
	~CSimulatorDlg(void);
// Dialog Data
	//{{AFX_DATA(CSimulatorDlg)
	enum { IDD = IDD_SIMULATOR_DIALOG };
	CListBox	m_ListData;
	CButton	m_StepExecute;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	
    BOOL           bIsStepExecute;
   	WorldModel*    WM;
    SoccerServer*  Server;
	Command*       CM;
	Agent*         Robot;
	
	ofstream      fouterror;
 	double        elaspedtime;
	// test
    FieldGeometry FG;
// Implementation
   void         ShowDetails();

 protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimulatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStartSimulate();
	afx_msg void OnStopSimulate();
	afx_msg void OnNextCycle();
	afx_msg void OnStepExecute();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULATORDLG_H__E803D7DC_51A7_4B80_9639_4E58C3077EB3__INCLUDED_)
