#if !defined(AFX_IMAGEPROCESSINGMONITOR_H__65A3349F_04F7_4F45_B574_C90B5DD6421C__INCLUDED_)
#define AFX_IMAGEPROCESSINGMONITOR_H__65A3349F_04F7_4F45_B574_C90B5DD6421C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageProcessingMonitor.h : header file
//
#include "Image.h"
#include "ScanLines.h"
#include "LineScanning.h"
#include "ConfigReader.h"
#include "Frame2D.h"
 
const static double scaleX = 1.86; // half of screen  
const static double scaleY = 1.67;
/////////////////////////////////////////////////////////////////////////////
// CImageProcessingMonitor window

class CImageProcessingMonitor : public CWnd
{
// Construction
public:
	CImageProcessingMonitor(const ConfigReader& config, Image* image);

// Attributes
public:
    HDC			hdc;
	HGLRC		hglrc;
    
	Frame2d     fr;
	Image*      image;
	Image*      bufferedImage;
	int			width;  
	int			height; 
    int			colorID;

	struct tBitmapInfo
	{
		BITMAPINFOHEADER	bmiHeader;
	};
	tBitmapInfo			m_bitmapInfo;
	CDC					m_memDc;
	CBitmap*			m_pMemDcBitmap;  // New bitmap each time size changes
	
	unsigned char*      buffer;

	float				lwidth[4];
	float				psize[2];
	GLint				vp[4];
	CRect				my_oldRect;
    
		

// Operations
public:
	int					MySetPixelFormat(HDC hdc);
    void				setImage( Image* image );

protected:
      ScanLines*		scanLines;
      ScanResultList*	scanResults;

    ColorClassInfoList*		colClass;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingMonitor)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setColorId( int id );
	void ShowImageWithDc(CPaintDC& dc);
	void ShowImage();
	void DrawCircle(Vec p, double r = 2.8 );
 	void DisplayAllObject();
	void DrawPoint( Vec p );
	void DrawLine( Vec p1, Vec p2 );
	void SetVisualizeScanResult();
	void setScanResults(  ScanResultList* scanResults);
	void setScanLines(  ScanLines* scanLines);
	virtual ~CImageProcessingMonitor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingMonitor)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGMONITOR_H__65A3349F_04F7_4F45_B574_C90B5DD6421C__INCLUDED_)
