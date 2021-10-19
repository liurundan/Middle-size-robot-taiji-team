#if !defined(AFX_VALPLOTWIDGET_H__AFB75D84_2E44_4ACA_9A16_E04977B41054__INCLUDED_)
#define AFX_VALPLOTWIDGET_H__AFB75D84_2E44_4ACA_9A16_E04977B41054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ValPlotWidget.h : header file
//
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CValPlotWidget window

class CValPlotWidget : public CWnd
{
// Construction
public:
	CValPlotWidget(int w, int h);
	void init_widget(float _max_val, float _length);
	void init_widget(float min_val, float _max_val, float _length);
	void push(float val);
// Attributes
public:
    CDC					m_memDc;
	CBitmap*			m_pMemDcBitmap;  // New bitmap each time size changes
	CRect				m_clientRect;
	int					width;  
	int					height;
	float min_val, max_val, length;
    std::vector<float>  values;
	CPoint              scale; 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValPlotWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CValPlotWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(CValPlotWidget)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALPLOTWIDGET_H__AFB75D84_2E44_4ACA_9A16_E04977B41054__INCLUDED_)
