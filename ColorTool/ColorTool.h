// ColorTool.h : main header file for the COLORTOOL application
//

#if !defined(AFX_COLORTOOL_H__96D3ED05_46EE_4951_A89C_96D203417114__INCLUDED_)
#define AFX_COLORTOOL_H__96D3ED05_46EE_4951_A89C_96D203417114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CColorToolApp:
// See ColorTool.cpp for the implementation of this class
//

class CColorToolApp : public CWinApp
{
public:
	CColorToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CColorToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTOOL_H__96D3ED05_46EE_4951_A89C_96D203417114__INCLUDED_)
