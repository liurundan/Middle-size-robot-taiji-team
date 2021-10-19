// RobotControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotControl.h"
#include "RobotControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDlg dialog

CRobotControlDlg::CRobotControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRobotControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRobotControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	the_control_loop = NULL;
	brush.CreateSolidBrush(RGB(100,222,100));

}

void CRobotControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotControlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRobotControlDlg, CDialog)
	//{{AFX_MSG_MAP(CRobotControlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDlg message handlers

BOOL CRobotControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	unsigned int robot_id = -1;

	if( vread.get("robot_number",robot_id) <= 0 || robot_id < 0 )
	{
       JERROR("no config line \"robot_number\" found");
       throw InvalidConfigurationException ("robot_number");
	}
	CString stmp = "";
	//stmp.Format("config_files\\robot_%d\\robotcontrol.cfg",robot_id);
	//string configfile = stmp.GetBuffer(0);
    string configfile = "config_files\\robotcontrol.cfg";
	 
	bool success = vread.append_from_file(configfile.c_str());
    
	unsigned int ui; 
	Journal::the_journal.set_mode(vread);
    vread.get("random_seed", ui); 
    random_seed (ui);

    unsigned int loop_time;
    if (vread.get ("loop_time", loop_time)<=0 || loop_time<=0) 
	{ 
       JERROR("no config line \"loop_time\" found");
       throw InvalidConfigurationException ("loop_time");
	} 
    m_cdc = GetDC();
	
	the_control_loop = new ControlLoop(vread,loop_time,*m_cdc);//开始运行程序
	
	SetTimer(0,loop_time,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRobotControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRobotControlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRobotControlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRobotControlDlg::OnTimer(UINT nIDEvent) 
{
    the_control_loop->loop();
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CRobotControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
 	if ( nCtlColor==CTLCOLOR_STATIC )
	{
       pDC->SetTextColor(RGB(0,0,0));
	   pDC->SetBkMode(OPAQUE);
	   pDC->SetBkColor(RGB(100,222,100));
	}
	
    return brush;
}

void CRobotControlDlg::OnDestroy() 
{
	if (the_control_loop)
	{
		delete the_control_loop;
		the_control_loop = NULL;
	}
	
	CDialog::OnDestroy();

	brush.DeleteObject();
	if (m_cdc->m_hDC)
	{
		m_cdc->SelectObject((CBitmap*)NULL);
		m_cdc->DeleteDC();
	}
	
}

BOOL CRobotControlDlg::PreTranslateMessage(MSG* pMsg)
{
 	switch( pMsg->message ) 
	{
	   case WM_KEYDOWN:
			{			
				char ctmp = (char)(pMsg->wParam);
				the_control_loop->the_user_interface->Setmessage(ctmp);
			}
		    break;
 	   default:
		   break;
	} 
 	
	return CDialog::PreTranslateMessage(pMsg);
}
