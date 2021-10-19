// ImageMask.h : main header file for the IMAGEMASK application
//

#if !defined(AFX_IMAGEMASK_H__F7765C00_A100_4F86_8B87_B5AA1A46F80C__INCLUDED_)
#define AFX_IMAGEMASK_H__F7765C00_A100_4F86_8B87_B5AA1A46F80C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageMaskApp:
// See ImageMask.cpp for the implementation of this class
//

class CImageMaskApp : public CWinApp
{
public:
	CImageMaskApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMaskApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImageMaskApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMASK_H__F7765C00_A100_4F86_8B87_B5AA1A46F80C__INCLUDED_)
