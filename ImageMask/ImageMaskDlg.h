// ImageMaskDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_IMAGEMASKDLG_H__2724E7CF_1BAC_4282_8BC8_E6E6B484E752__INCLUDED_)
#define AFX_IMAGEMASKDLG_H__2724E7CF_1BAC_4282_8BC8_E6E6B484E752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Robot.h"
#include "ColorClassifier.h"
#include "ColorClasses.h"
#include "Image.h"
#include "MultiImageProducer.h"
#include "RGBImage.h"
#include "ImageIO.h"
#include "ChainCoding.h"
#include "ImageVarianceFilter.h"
#include "RobotMask.h"

#define Image_Changed    WM_USER+100 

/////////////////////////////////////////////////////////////////////////////
// CImageMaskDlg dialog

class CImageMaskDlg : public CDialog
{
// Construction
public:
	CImageMaskDlg(CWnd* pParent = NULL);	// standard constructor
    
// Dialog Data
	//{{AFX_DATA(CImageMaskDlg)
	enum { IDD = IDD_IMAGEMASK_DIALOG };
	CStatic	m_threshold;
	CSliderCtrl	m_thresholdSlider;
	CMSComm	m_Com;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreatMask();
	void MarkCrosshair(CPoint p, int size = 5 );
	void DrawRect(CPoint point, int size = 3 );
	bool isinarea(CPoint point,int xleft,int xright,int ytop,int ybottom);
	void DrawCircle( CPoint point1, CPoint point2 );
	void thresholdChanged( int threshold );
	void captureoff();
	void writeMask(string filename);
	void recalculateImageCenter();
	void fillRegion( Image * image,const Region & region,const RGBTuple & color );
	void recalculateMask();
	void ShowImage( Image& );
 	Image*          bufferedImage;
	const RobotMask*      mask2;
    int			width;  
	int			height; 
	struct tBitmapInfo
	{
		BITMAPINFOHEADER	bmiHeader;
	};
	tBitmapInfo			m_bitmapInfo;
	CDC					m_memDc;
	CBitmap*			m_pMemDcBitmap;  // New bitmap each time size changes
	unsigned char*      buffer;
    
	
	bool                bSave, bSaveAs, bCapture, bWriteConf;
	bool                bStarttimer;
	
	CPoint              center, pointleft, pointright;
	bool                drawcircle, m_bIsDragingleft, m_bIsDragingright;
	bool                bUsemask, bUsefilemask;
 
	string				filename;
	int					whiteRegionMin;
	int					blackRegionMin;
	int					threshold;
	bool				programStart, capturing;
	bool                state;
	MultiImageProducer* producer;
	
	Image*				image;
	ImageVarianceFilter*  distanceFilter;
	Image*				mask;
	ColorClassifier*	whiteClassifier;
    ColorClassifier*	blackClassifier;
    RegionDetector*		regionDetector;
    Robot*				robot;
	string              configFileName;
	int					centerX, centerY, radius;


	//ImageMaxDistanceFilter* distanceFilter;
	ConfigReader vread;
	CStatusBarCtrl*     m_StatusBar;

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImageMaskDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT setImage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilesave();
	afx_msg void OnUpdateFilesave(CCmdUI* pCmdUI);
	afx_msg void OnFilesaveas();
	afx_msg void OnUpdateFilesaveas(CCmdUI* pCmdUI);
	afx_msg void OnWriteconfig();
	afx_msg void OnUpdateWriteconfig(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu);     
	afx_msg void OnCapture();
	afx_msg void OnDrawCircle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCreatmask();
	afx_msg void OnUseMask();
	afx_msg void OnQuickLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMASKDLG_H__2724E7CF_1BAC_4282_8BC8_E6E6B484E752__INCLUDED_)
