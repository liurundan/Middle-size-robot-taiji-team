#if !defined(AFX_PARAMETERCONTROLS_H__5F43E722_A149_48A8_B13B_1D63A61F28AE__INCLUDED_)
#define AFX_PARAMETERCONTROLS_H__5F43E722_A149_48A8_B13B_1D63A61F28AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterControls.h : header file
//
#include "EditPopup.h"
#include "LogSlider.h"
/////////////////////////////////////////////////////////////////////////////
// CParameterControls dialog
typedef enum
{
	whitebalanceCalibrated	  = 0,
	whitebalanceManual	      = 1,
    whitebalanceAuto          = 2,
	whitebalanceone_push_auto = 3

} PdtWhiteBalanceMode;

class CParameterControls : public CDialog
{
// Construction
public:
	CParameterControls(CWnd* pParent = NULL);   // standard constructor
    void RefreshControls();
// Dialog Data
	//{{AFX_DATA(CParameterControls)
	enum { IDD = IDD_PARAMETER_CONTROLS };
	CEditPopupUlong	m_ctlRedGainEdit;
	CEditPopupUlong	m_ctlBlueGainEdit;
	CSliderCtrl	m_ctlBlueGainSlider;
	CSliderCtrl	m_ctlRedGainSlider;
	CSliderCtrl	m_ctlGainSlider;
	CEditPopupUlong	m_ctlGainEdit;
	CLogSlider	m_ctlExpSlider;
	CEditPopupTime	m_ctlExpEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameterControls)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParameterControls)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeExposureEdit();
	afx_msg void OnChangeGainEdit();
	afx_msg void OnChangeRedgainEdit();
	afx_msg void OnChangeBluegainEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERCONTROLS_H__5F43E722_A149_48A8_B13B_1D63A61F28AE__INCLUDED_)
