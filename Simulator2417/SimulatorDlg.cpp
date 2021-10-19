// SimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Simulator.h"
#include "SimulatorDlg.h"
 

#include <fstream>
#include <iomanip> // use to control data fomat

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CSimulatorDlg dialog

CSimulatorDlg::CSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimulatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
 
void CSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimulatorDlg)
	DDX_Control(pDX, IDC_LISTLOCALIZE_DATA, m_ListData);
	DDX_Control(pDX, IDC_StepExecute, m_StepExecute);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimulatorDlg, CDialog)
	//{{AFX_MSG_MAP(CSimulatorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_StartSimulate, OnStartSimulate)
	ON_BN_CLICKED(IDC_StopSimulate, OnStopSimulate)
	ON_BN_CLICKED(IDC_NextCycle, OnNextCycle)
	ON_BN_CLICKED(IDC_StepExecute, OnStepExecute)
	ON_BN_CLICKED(IDC_Reset, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulatorDlg message handlers

BOOL CSimulatorDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
   	// 定义OpenGL绘图窗口的矩形大小

	CRect rect(0, 0, 700, 500); //560*420
    
    CDisplay* m_pDisplay = new CDisplay;

    // 创建CDisplayL类对象:
	m_pDisplay->Create( NULL,                                                      //缺省的窗口
						NULL,                                                      //无窗口名称
						WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE , //定义窗口风格						       
						rect,                                                      //窗口的大小
						this,                                     // 指定当前对话框为其父窗口指针
						0);   
    //WS_SYSMENU | WS_CAPTION WS_THICKFRAME
     
    //Server->init(  &message2, m_pDisplay );
	//CM.init(      &message2, &WM        );
	//Robot.init(   &message2, &WM, &CM, m_pDisplay);
    MessageBoard *message2 = new MessageBoard; 
	WM         = new WorldModel;
	Server     = new SoccerServer(message2,m_pDisplay);
	CM         = new Command(message2,WM);
	Robot      = new Agent( message2,WM,CM,m_pDisplay);
	unsigned int ui = 8746;
	random_seed(ui);

 	bIsStepExecute = FALSE;

    for(int i=0;i<3;i++)
	{
		CString str;
		if( i == 0 )
 		  str.Format("T0:BALL_Position( NULL, NULL , NULL )",i);
 		else if( i == 1 )
	      str.Format("T0:Ideal_Position( NULL, NULL ),Heading( NULL )",i);
		else
          str.Format("T0:ResultPosition( NULL, NULL ),Heading( NULL )",i);

	    str.TrimLeft();
	    m_ListData.InsertString(i,str);
	}  
 
 	return TRUE;  // return TRUE  unless you set the focus to a control
}
 
void CSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CSimulatorDlg::OnPaint() 
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
HCURSOR CSimulatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 添加CMyDlgOpenGlg类的虚构函数
CSimulatorDlg::~CSimulatorDlg()
{
//	if(m_pDisplay)
//	{
//	  delete m_pDisplay; // 释放m_pDisplay指针
//	}	
    //delete    message2;
	delete    WM;
	delete    Server;
	delete    CM;
    delete    Robot;
 
}

void CSimulatorDlg::OnTimer(UINT nIDEvent) 
{
 	Server->Run();

	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值

    Robot->Run();
    
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒 
	elaspedtime = dfTim*1000;
 

    ShowDetails();
 
/*	
	//  test fieldlut 
 	Vector pos( 4530,  -3052);
    for( unsigned int i=0; i < FG.lines.size(); i++ )
	{
		LineSegment    ln1 = FG.lines[i];
	    double dist = ln1.distance(pos);
		if (dist < realmindist) {        realmindist = dist;	}
	}
	for( unsigned int k=0; k < FG.arcs.size(); k++) 
	{
		MyArc ac = FG.arcs[k];
	    double dist2 = ac.distance(pos);
		if (dist2 < realmindist){      realmindist = dist2;		}
	}*/
 //   FieldLUT FL(FG,50);
  //  double dist = FL.distance( pos );
  //	Vector ddistdpos = FL.gradient(pos );
  
  
 /*	LineSegment ls(Vector(0,1),Vector(4,1));
	double dis1 = ls.distance(Vector(5,2));
    
	MyLine ln = MyLine(Vector(0,0),Vector(0,5));
	MyLine ln2 = MyLine(Vector(5,0),Vector(5,5));
	vector<Vector> a3 = intersect(ls,ln2);
	Vector a4 = a3[0]; 
	Circle cc = Circle(Vector(0,0),1.0);
    vector<Vector> res4 = tangent_point(cc,Vector(5,0));
    Vector a9 = res4[0];
	Vector a10= res4[1];

	Circle cc1 = Circle(Vector(3,1),1.0);
	vector<Vector> res2 = intersect(cc1,cc);
	int isize = res2.size();
    Vector a5 = res2[0];
	Vector a6 = res2[1];
	vector<Vector> res = intersect(ln,cc);
	Vector a1 = res[0];
	Vector a2 = res[1];
    double dis2 = ln.distance(Vector(2, -3));

	MyArc ac = MyArc(Vector(0,0),1,0,90);
    vector<Vector> res3 = intersect(ln,ac);
    int isize2 = res3.size();
    Vector a7 = res3[0];
	Vector a8 = res3[1];
 

	Vector a(1, 1);
	Vector b( 1, -1);
	Angle winkel;

 	winkel=Rad2Deg(a.angle(b));
 
 
 
 	Vector pos(12,6); Vector head(1,0) ;Vector c(5,9);
	Vector look,look2;
	head=head.rotate( 30 );
	look=pos+c.rotate( head.getDirection() );
    
	c = Vector(5,9);
	Frame2d f;
	f.set_position(pos.X,pos.Y);
	f.set_angle(30);
	MyArc ac2=f*ac;
    
	FieldGeometry FQ;
    LineSegment ls3 = LineSegment(Vector(2,-7000),Vector(2,1000));
	for( int i =0; i < FQ.lines.size(); i++)
	{
		LineSegment ls5 = FQ.lines[i];
		vector<Vector> res4 = intersect(ls3,ls5);
	}

    LineSegment ls4 = LineSegment(Vector(0,0),Vector(-9000,6000));
 	for (int j=0; j<FQ.arcs.size();j++) 
	{
		MyArc ac8 = FQ.arcs[j];
		vector<Vector> res5 = intersect(ac8,ls4);
	}
	
    //ScanLines SL(Vector(10,10),300,30000,60);  
//	for (int k = 0; k < SL.scanLines.size(); k++)
//	{
  //     LineSegment ls5 = SL.scanLines[k];    
//	} 

 	Frame2d f1,f2,f3;
	Vec y(3,3);
	f1.set_position(10,10);
	f2.set_position(20,10);

	f3=f1*f2;
	cout <<f1<<f1*y<<f2<<f2*y<<f3<<f3*y<<endl;*/
  	CDialog::OnTimer(nIDEvent);
}
 
void CSimulatorDlg::ShowDetails()
{
    CString str0,str1,str2;
 /*	str0.Format("T%d:BALL_Position( %.2f, %.2f, 0.0 )",
		        WM->getCurrentCycle(),
		        Server->GetBallRealPos().getX(),
				Server->GetBallRealPos().getY() );

    str1.Format("T%d:Ideal_Position( %.2f, %.2f ),Heading( %.2f )",
		        WM->getCurrentCycle(),
		        Server->GetAgentRealGlobalPos().getX(),
				Server->GetAgentRealGlobalPos().getY(),
				Server->GetAgentRealHeading() );

	str2.Format("T%d:ResultPosition( %.2f, %.2f ),Heading( %.2f )",
		        WM->getCurrentCycle(),
		        WM->getAgentGlobalPosition().getX(),
				WM->getAgentGlobalPosition().getY(),
				WM->getAgentGlobalBodyAngle() );
	str0.TrimLeft();
	str1.TrimLeft();
  	str2.TrimLeft();
    m_ListData.DeleteString(0);
    m_ListData.InsertString(0,str0);
    m_ListData.DeleteString(1);WM->getCurrentCycle()
    m_ListData.InsertString(1,str1);
   	m_ListData.DeleteString(2);
	m_ListData.InsertString(2,str2);*/
    MyTime t;
	//t.update();
	str0.Format("T:%.3f ms: Error( △X:%.2f, △Y %.2f, △Dist %.2f Time %d ms)",
		    elaspedtime,
		    fabs(Server->GetAgentRealGlobalPos().getX()-WM->getAgentGlobalPosition().getX()),
			fabs(Server->GetAgentRealGlobalPos().getY()-WM->getAgentGlobalPosition().getY()),
			( Server->GetAgentRealGlobalPos()-
			  WM->getAgentGlobalPosition() ).getMagnitude() );
	str0.TrimLeft();
    m_ListData.DeleteString(0);
    m_ListData.InsertString(0,str0);
    str1.Format( " Time: %d ms ",t.get_usec() ); 
	str1.TrimLeft();
    m_ListData.DeleteString(1); 
    m_ListData.InsertString(1,str1); 
/*	
	fouterror.open("D:\\LRD_E\\LiuFei_thesis\\program\\Simulator\\Data\\error.txt",ios::out|ios::app);
//	fouterror.open("error.txt",ios::out|ios::app );
    fouterror<<setiosflags(ios::fixed); // 
	fouterror<<"T"<<(int)(WM->getCurrentCycle())<<"△X:"<<setprecision(2)<<fabs(Server->GetAgentRealGlobalPos().getX()-WM->getAgentGlobalPosition().getX())<<" ";
	fouterror<<"T"<<(int)(WM->getCurrentCycle())<<"△Y:"<<setprecision(2)<<fabs(Server->GetAgentRealGlobalPos().getY()-WM->getAgentGlobalPosition().getY())<<" ";
	fouterror<<"T"<<(int)(WM->getCurrentCycle())<<"△Dist:"<<setprecision(2)<<(Server->GetAgentRealGlobalPos()-WM->getAgentGlobalPosition()).getMagnitude()<<" ";
    fouterror<<"\n";

	fouterror.close(); */
	/*  m_ListData.AddString(str1);
	m_ListData.AddString(str2);*/
	
	CString str;
	str.Format("%.2f",WM->getAgentGlobalPosition().getX());
	str.TrimLeft();
	CWnd *pWndXY = GetDlgItem(IDC_ShowPosX);
	pWndXY->SetWindowText(str);
    
 	str.Format("%.2f",WM->getAgentGlobalPosition().getY());
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowPosY);
	pWndXY->SetWindowText(str);

	str.Format("%.2f",WM->getAgentGlobalBodyAngle());
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowHeading);
	pWndXY->SetWindowText(str);

	str.Format("%.2f",Server->GetAgentRealGlobalPos().getX());
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowRealRobotX);
	pWndXY->SetWindowText(str);

	str.Format("%.2f",Server->GetAgentRealGlobalPos().getY());
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowRealRobotY);
	pWndXY->SetWindowText(str);

	str.Format("%.2f",Server->GetAgentRealHeading() );
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowRealRobotHeading);
	pWndXY->SetWindowText(str);
  
	str.Format("%d",WM->getCurrentCycle());
	str.TrimLeft();
	pWndXY = GetDlgItem(IDC_ShowCycle);
	pWndXY->SetWindowText(str);
}
 
void CSimulatorDlg::OnStartSimulate() 
{
 	bIsStepExecute = FALSE;
//	fouterror.open("D:\\LRD_E\\LiuFei_thesis\\program\\Simulator\\Data\\error.txt",ios::out);
//	fouterror.close(); 
 	SetTimer(0,CONTROL_INTERVAL,NULL);
 
   // FieldLUT FL(FG,50);

}

void CSimulatorDlg::OnStopSimulate() 
{
 	KillTimer(0);
	WM->setCurrentCycle(0);
}
 
void CSimulatorDlg::OnNextCycle() 
{
  if( bIsStepExecute == TRUE ) 
  {
 	Server->Run();
	Robot->Run();
    ShowDetails();
  }
  else
	  return;
}

void CSimulatorDlg::OnStepExecute() 
{
	const ULONG	 value = m_StepExecute.GetCheck();
	if (value == 1) 
	{
      	KillTimer(0);
		bIsStepExecute = TRUE;
	}
	else
	{
		bIsStepExecute = FALSE;
		return;
	}
//	m_pDisplay->Invalidate(FALSE);
}
 
void CSimulatorDlg::OnReset() 
{
   KillTimer(0);
   Server->reset();
   Robot->reset();
   SetTimer(0,CONTROL_INTERVAL,NULL); 
}
