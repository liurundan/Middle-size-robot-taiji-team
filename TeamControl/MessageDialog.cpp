// MessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "MessageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MessageDialog dialog

IMPLEMENT_DYNAMIC(MessageDialog,CDialog)
MessageDialog::MessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MessageDialog)
	out_msg = _T("");
	//}}AFX_DATA_INIT
	m_parent = pParent;
}

MessageDialog::~MessageDialog()
{
;
}

void MessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MessageDialog)
	DDX_Control(pDX, IDC_MSG, ctrl_out_msg);
	DDX_Text(pDX, IDC_MSG, out_msg);
	//}}AFX_DATA_MAP
}
 
BEGIN_MESSAGE_MAP(MessageDialog, CDialog)
	//{{AFX_MSG_MAP(MessageDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MessageDialog message handlers

BOOL MessageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
	GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if( fun )
			fun(this->GetSafeHwnd(),0,128,2); 
		FreeLibrary(hInst); 
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MessageDialog::OnPaint() 
{
	CPaintDC dc(this);
}
  
void MessageDialog::SetMessage(CString s)
{
/*	static unsigned int linemsg = 0;
	if (linemsg < 10 )
	{
 	   out_msg += s;
	   linemsg++;
	}
	else
	{
	   out_msg = "";
	   linemsg = 0;
	}*/
	if (out_msg.GetLength() < 250 )
	{
		out_msg += s;
	}
	else
		out_msg.Empty();
    ctrl_out_msg.SetWindowText(out_msg);
	
}

void MessageDialog::OnCancel()
{
	ShowWindow(SW_HIDE);
	::SendMessage(m_parent->m_hWnd,closeMsgdialog,0,0);
}
