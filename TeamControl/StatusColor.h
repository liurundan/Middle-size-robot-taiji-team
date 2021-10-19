#if !defined(AFX_STATUSCOLOR_H__E875CDC8_3E9D_4331_9F90_CE68CF141D0C__INCLUDED_)
#define AFX_STATUSCOLOR_H__E875CDC8_3E9D_4331_9F90_CE68CF141D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StatusColor window

class StatusColor : public CWnd
{
// Construction
public:
	StatusColor(int w, int h);
    void SetStatusColor(COLORREF c, CString s = "" );
// Attributes
public:
	CDC					dcMem;
	CBitmap				bm;
	CRect				rc;
    int                 width;
	int					height;
    COLORREF			statusColor;
    
	CString             msg;
	CFont				statusfont;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~StatusColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(StatusColor)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSCOLOR_H__E875CDC8_3E9D_4331_9F90_CE68CF141D0C__INCLUDED_)
