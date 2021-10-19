#if !defined(AFX_FIELDOFPLAY_H__8510FB52_E705_4197_A484_8D6698AB890C__INCLUDED_)
#define AFX_FIELDOFPLAY_H__8510FB52_E705_4197_A484_8D6698AB890C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldOfPlay.h : header file
//
#include "CycleInfo.h"
#include "FieldGeometry.h"
#include "PaintPreferences.h"
#include "Frame2D.h"
#include "FieldWidget.h"

/////////////////////////////////////////////////////////////////////////////
// FieldOfPlay dialog

class FieldOfPlay : public CDialog
{
// Construction
public:
	FieldOfPlay(CWnd* pParent = NULL);   // standard constructor
    virtual ~FieldOfPlay();
	
// Dialog Data
	//{{AFX_DATA(FieldOfPlay)
	enum { IDD = IDD_FIELDWIDGET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
    FieldWidget*			fw;

private:
	CToolBar				m_ToolBar;
    CWnd*					m_parent;
	CRect					clientrect;
	
	CStatusBarCtrl*					m_StatusBar;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FieldOfPlay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    afx_msg void Docommand(UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
    afx_msg LRESULT emit_slDisplacementMSg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT emit_robotDisplacementMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Coutmsg(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(FieldOfPlay)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDOFPLAY_H__8510FB52_E705_4197_A484_8D6698AB890C__INCLUDED_)
