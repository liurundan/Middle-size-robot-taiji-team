// TeamControl.h : main header file for the TEAMCONTROL application
//

#if !defined(AFX_TEAMCONTROL_H__44B88FC3_7E74_41FE_A5C0_D503BD613E3B__INCLUDED_)
#define AFX_TEAMCONTROL_H__44B88FC3_7E74_41FE_A5C0_D503BD613E3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTeamControlApp:
// See TeamControl.cpp for the implementation of this class
//

class CTeamControlApp : public CWinApp
{
public:
	CTeamControlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeamControlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTeamControlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAMCONTROL_H__44B88FC3_7E74_41FE_A5C0_D503BD613E3B__INCLUDED_)
