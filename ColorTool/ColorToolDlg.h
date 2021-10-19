// ColorToolDlg.h : header file
//

#if !defined(AFX_COLORTOOLDLG_H__5028DE10_7E6B_40B0_891E_D1C7F0CF6EB4__INCLUDED_)
#define AFX_COLORTOOLDLG_H__5028DE10_7E6B_40B0_891E_D1C7F0CF6EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigReader.h"
#include "ColorClasses.h"
#include "GrabbingThread.h"
#include "EditPopup.h"
#include "Image.h"
#include "MultiImageProducer.h"
#include "ColorTools.h"
#include "RGBImage.h"
#include "ImageIO.h"
#include "ParameterControls.h"
#include "StatusColor.h"

/////////////////////////////////////////////////////////////////////////////
// CColorToolDlg dialog
 

class CColorToolDlg : public CDialog
{
// Construction
public:
	void DoSaveLookupTable();
	void SaveRanges();
    
	void ColorChanged(int c);
	void UpdateRanges();
	void ActivateSliders(BOOL act);
	void InitSlidersAndLabels();
	void  hMinChanged(int min);
	void  hMaxChanged(int max);
	void  sMinChanged(int min);
	void  sMaxChanged(int max);
	void  iMinChanged(int min);
	void  iMaxChanged(int max);
   // void  OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu);     

	CColorToolDlg(CWnd* pParent = NULL);	// standard constructor
    ~CColorToolDlg();
	
public:
	bool                bStopReadImage;
private:
	StatusColor*		colorField;
// Dialog Data
	//{{AFX_DATA(CColorToolDlg)
	enum { IDD = IDD_COLORTOOL_DIALOG };
	CComboBox	m_colorSelector;
 	CEditPopupUlong	m_SminEdit;
  	CEditPopupUlong	m_SmaxEdit;
  	CEditPopupUlong	m_IminEdit;
  	CEditPopupUlong	m_ImaxEdit;
  	CEditPopupUlong	m_HminEdit;
  	CEditPopupUlong	m_HmaxEdit;
	CSliderCtrl	m_IminSlider;
	CSliderCtrl	m_SmaxSlider;
	CSliderCtrl	m_SminSlider;
	CSliderCtrl	m_ImaxSlider;
	CSliderCtrl	m_HmaxSlider;
	CSliderCtrl	m_HminSlider;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
    CStatusBarCtrl* m_StatusBar;
	HACCEL          m_hAccel,m_hAccel2,m_hAccel3,m_hAccel4,m_hAccel5,m_hAccel6,m_hAccel7;
 // Implementation
    MultiImageProducer*  producer;
 	Image*          bufferedImage;
    GrabbingThread* grabbingThread;

	struct tBitmapInfo
	{
		BITMAPINFOHEADER	bmiHeader;
	};
	tBitmapInfo			m_bitmapInfo;
	CDC					m_memDc;
	CBitmap*			m_pMemDcBitmap;  // New bitmap each time size changes
	unsigned char*      buffer;
	
	int					frameCounter;
	int					width;
	int					height;
    double				lastH, lastI, lastS;
    BOOL				slidersActivated;
	bool				showIt, bSaveLut, bSaveAsLut, bUseLut;
	bool				bUseLookupTable;
	string				filename, lutFilename;

private:
	CParameterControls*    getControlDialog();
    CParameterControls*    parameter_control;
protected:
	HICON m_hIcon;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
	//{{AFX_MSG(CColorToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNextcolor();
	afx_msg void OnPreviouscolor();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnColorselector();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeHminEdit();
	afx_msg void OnChangeHmaxEdit();
	afx_msg void OnChangeSminEdit();
	afx_msg void OnChangeSmaxEdit();
	afx_msg void OnChangeIminEdit();
	afx_msg void OnChangeImaxEdit();
	afx_msg void OnSetcolor();
	afx_msg void OnAddcolor();
	afx_msg void OnCreatLookupTable();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnOpenLookupTable();
	afx_msg void OnSaveAsLookupTable();
	afx_msg void OnSaveLookupTable();
	afx_msg void OnUseLookupTable();
	afx_msg void OnQuickLoad();
	afx_msg void OnQuickSave();
	afx_msg void OnUpdateSavelut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveaslut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUselut(CCmdUI* pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu);     
	afx_msg void OnShowOrigimage();
	afx_msg void OnUpdateShowOrigimage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilesaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileopen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenlut(CCmdUI* pCmdUI);
	afx_msg void OnCamera();
	afx_msg void OnUpdateCamera(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTOOLDLG_H__5028DE10_7E6B_40B0_891E_D1C7F0CF6EB4__INCLUDED_)
