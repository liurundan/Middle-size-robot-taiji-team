// ImageProcessDlg.h : header file
//

#if !defined(AFX_IMAGEPROCESSDLG_H__3EAD1209_785C_4E28_80DC_062BEBDED01E__INCLUDED_)
#define AFX_IMAGEPROCESSDLG_H__3EAD1209_785C_4E28_80DC_062BEBDED01E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageProcessingMonitor.h"
#include "MultiImageProducer.h"
#include "Image.h"
#include "RGBImage.h"
#include "YUVImage.h"
#include "ConfigReader.h"
#include "ScanLines.h"
#include "Image2WorldMapping.h"
#include "RobotMask.h"
#include "Vec.h"
#include "BallDetector.h"
#include "KoordTransformDortmund.h"

using namespace std;

#define WARP_WIDTH  10000
#define WARP_HEIGHT 10000 

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDlg dialog

class CImageProcessDlg : public CDialog
{
// Construction
public:
	CImageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImageProcessDlg)
	enum { IDD = IDD_IMAGEPROCESS_DIALOG };
	CComboBox	m_colorSelector;
	CButton	m_showresults;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
 
	bool                bStopReadImage;	 
	bool useLut, deWarp, bShowScnalines, bShowResults;
	CImageProcessingMonitor* widget;
	Image*				image;
	Image*				warpImage;
    Image*				regionMap;
  
	MultiImageProducer*		producer;
    
	ColorClassInfoList  colClass;
	ScanLines*			lines;
	LineScanner*		scanner;
	World2Image*		mapping;
	ScanResultList*		results;
    BallDetector*       ballDetector;
    CoordinateMapping*  image2real;
	
	int centerX, centerY;
	int innerRadius, outerRadius;
	int nScanLines; 
    int width;
	int height;
    
	Vec  imgballpos;
	Vec  realballpos;

	CStatusBarCtrl*     m_StatusBar;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImageProcessDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowSegmentation();
	afx_msg void OnShowWarpImage();
	afx_msg void OnShowScnalines();
	afx_msg void OnShowResults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSDLG_H__3EAD1209_785C_4E28_80DC_062BEBDED01E__INCLUDED_)
