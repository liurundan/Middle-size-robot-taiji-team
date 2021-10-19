#if !defined(AFX_REALROBOTDATA_H__857A82CD_8019_4ACD_89DD_EAE6601F0141__INCLUDED_)
#define AFX_REALROBOTDATA_H__857A82CD_8019_4ACD_89DD_EAE6601F0141__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealRobotData.h : header file
//
#include "ValPlotWidget.h"
#include "RobotData.h"
#include <deque>

/////////////////////////////////////////////////////////////////////////////
// CRealRobotData dialog

class CRealRobotData : public CDialog
{
// Construction
public:
	CRealRobotData(CWnd* pParent = NULL);   // standard constructor
    virtual ~CRealRobotData();
	void update( const RobotData &rd );

	std::deque<CValPlotWidget*>       valplot_widget;
   
    CPoint			 startpoint;
	int				 width;
	int				 height;
 // Dialog Data
	//{{AFX_DATA(CRealRobotData)
	enum { IDD = IDD_REALROBOTDATA };
	CStatic	rob_vel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealRobotData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealRobotData)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALROBOTDATA_H__857A82CD_8019_4ACD_89DD_EAE6601F0141__INCLUDED_)
