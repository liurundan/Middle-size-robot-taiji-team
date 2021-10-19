// ColorToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ColorTool.h"
#include "ColorToolDlg.h"
#include "ImageIO.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef MIN 
#define MIN(x,y) (x) < (y) ? (x) : (y)
#endif

#ifndef MAX
#define MAX(x,y) (x) > (y) ? (x) : (y)
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
// CColorToolDlg dialog

CColorToolDlg::CColorToolDlg(CWnd* pParent /*=NULL*/)
: CDialog(CColorToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorToolDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	bufferedImage		= NULL;
	buffer				= NULL;
	producer			= NULL;
    grabbingThread		= NULL;
	m_StatusBar			= NULL;
    parameter_control	= NULL;
	colorField			= NULL;
	lastH = lastI = lastS = 0.0;
	width = 0;
	height = 0;
	
	m_bitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biPlanes			= 1;
	m_bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	m_bitmapInfo.bmiHeader.biSizeImage		= 0;
	m_bitmapInfo.bmiHeader.biXPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biYPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biClrUsed		= 0;
	m_bitmapInfo.bmiHeader.biClrImportant	= 0;
  	 
}

CColorToolDlg::~CColorToolDlg()
{
	if (bufferedImage)  delete bufferedImage; 
	if (m_pMemDcBitmap) delete m_pMemDcBitmap; 
	if (buffer)         delete [] buffer;  
	if (grabbingThread) delete grabbingThread;
	if (producer)	    delete producer;
	if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		m_StatusBar = NULL;
	}
	
	if (parameter_control)
	{
		delete parameter_control;
	}
	if (colorField)
	{
		delete colorField;
		colorField = NULL;
	}
}

void CColorToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorToolDlg)
	DDX_Control(pDX, IDC_COLORSELECTOR, m_colorSelector);
	DDX_Control(pDX, IDC_SMIN_EDIT, m_SminEdit);
	DDX_Control(pDX, IDC_SMAX_EDIT, m_SmaxEdit);
	DDX_Control(pDX, IDC_IMIN_EDIT, m_IminEdit);
	DDX_Control(pDX, IDC_IMAX_EDIT, m_ImaxEdit);
	DDX_Control(pDX, IDC_HMIN_EDIT, m_HminEdit);
	DDX_Control(pDX, IDC_HMAX_EDIT, m_HmaxEdit);
	DDX_Control(pDX, IDC_IMIN_SLIDER, m_IminSlider);
	DDX_Control(pDX, IDC_SMAX_SLIDER, m_SmaxSlider);
	DDX_Control(pDX, IDC_SMIN_SLIDER, m_SminSlider);
	DDX_Control(pDX, IDC_IMAX_SLIDER, m_ImaxSlider);
	DDX_Control(pDX, IDC_HMAX_SLIDER, m_HmaxSlider);
	DDX_Control(pDX, IDC_HMIN_SLIDER, m_HminSlider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorToolDlg, CDialog)
	//{{AFX_MSG_MAP(CColorToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NEXTCOLOR, OnNextcolor)
	ON_BN_CLICKED(IDC_PREVIOUSCOLOR, OnPreviouscolor)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COLORSELECTOR, OnColorselector) // ON_CBN_EDITCHANGE
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_HMIN_EDIT, OnChangeHminEdit)
	ON_EN_CHANGE(IDC_HMAX_EDIT, OnChangeHmaxEdit)
	ON_EN_CHANGE(IDC_SMIN_EDIT, OnChangeSminEdit)
	ON_EN_CHANGE(IDC_SMAX_EDIT, OnChangeSmaxEdit)
	ON_EN_CHANGE(IDC_IMIN_EDIT, OnChangeIminEdit)
	ON_EN_CHANGE(IDC_IMAX_EDIT, OnChangeImaxEdit)
	ON_BN_CLICKED(IDC_SETCOLOR, OnSetcolor)
	ON_BN_CLICKED(IDC_ADDCOLOR, OnAddcolor)
	ON_COMMAND(IDM_MENUITEM_CREATLUT, OnCreatLookupTable)
	ON_COMMAND(IDM_MENUITEM_FILEOPEN, OnFileOpen)
	ON_COMMAND(IDM_MENUITEM_FILESAVE, OnFileSave)
	ON_COMMAND(IDM_MENUITEM_FILESAVE_AS, OnFileSaveAs)
	ON_COMMAND(IDM_MENUITEM_OPENLUT, OnOpenLookupTable)
	ON_COMMAND(IDM_MENUITEM_SAVEASLUT, OnSaveAsLookupTable)
	ON_COMMAND(IDM_MENUITEM_SAVELUT, OnSaveLookupTable)
	ON_COMMAND(IDM_MENUITEM_USELUT, OnUseLookupTable)
	ON_COMMAND(IDM_QUICKLOAD, OnQuickLoad)
	ON_COMMAND(IDM_QUICKSAVE, OnQuickSave)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_SAVELUT, OnUpdateSavelut)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_SAVEASLUT, OnUpdateSaveaslut)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_USELUT, OnUpdateUselut)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(IDM_MENUITEM_SHOW_ORIGIMAGE, OnShowOrigimage)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_SHOW_ORIGIMAGE, OnUpdateShowOrigimage)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_FILESAVE_AS, OnUpdateFilesaveAs)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_FILEOPEN, OnUpdateFileopen)
	ON_UPDATE_COMMAND_UI(IDM_MENUITEM_OPENLUT, OnUpdateOpenlut)
	ON_COMMAND(IDM_CAMERA, OnCamera)
	ON_COMMAND(IDM_PREV_COLOR, OnPreviouscolor)
	ON_COMMAND(IDM_NEXT_COLOR, OnNextcolor)
	ON_UPDATE_COMMAND_UI(IDM_CAMERA, OnUpdateCamera)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorToolDlg message handlers

BOOL CColorToolDlg::OnInitDialog()
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
	m_hAccel    = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));
	m_hAccel2   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR2));
    m_hAccel3   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR3));
    m_hAccel4   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR4));
    m_hAccel5   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR5));
    m_hAccel6   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR6));
    m_hAccel7   = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR7));
	
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );

	colorField = new StatusColor(50,50);
	colorField->Create(NULL, NULL,                                               
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE ,  					       
			CRect(0,0,50,50), this, 0);
	colorField->MoveWindow(930,10,50,50);
	
	//int strPartDim[4] = {100,280,500,-1	};
	//m_StatusBar->SetParts(4,strPartDim);
	
	
	slidersActivated = TRUE;
	showIt			 = true;
	bUseLookupTable  = false;
	bStopReadImage   = false;
    frameCounter     = 0;
	filename = lutFilename = "";
	
	m_colorSelector.InsertString(0,"ALL");
    ColorClassInfoList colorInfos;
	for (unsigned int i=1; i<colorInfos.classList.size(); i++ )
	{
		CString str, sTmp;
		sTmp   = colorInfos.classList[i]->name.c_str();
		sTmp   += "  %d"; 
		int  id = colorInfos.classList[i]->id;
		str.Format(sTmp,id);
		str.TrimLeft();
		m_colorSelector.InsertString(i,str);
		
	}
	string configfile = "config_files\\robotcontrol.cfg";
	ConfigReader vread;
	bool success = vread.append_from_file(configfile.c_str());
	
	try
	{
		//producer        = new ImageProducer(vread,"Camera_usb_0","ImageProducer_usb");
		producer        = new MultiImageProducer(vread);
		grabbingThread	= new GrabbingThread(producer);
		width = grabbingThread->getImage().getWidth();
		height = grabbingThread->getImage().getHeight();

		bufferedImage	= new RGBImage(width,height);
        buffer			= new BYTE[ width * height * 3 ];
		m_memDc.CreateCompatibleDC(GetDC());
		m_pMemDcBitmap = new CBitmap();
		m_pMemDcBitmap->CreateCompatibleBitmap(GetDC(), width, height);

	}
	catch (exception& e)
	{
		MessageBox( e.what() );
		exit(1);
	}
	
    InitSlidersAndLabels();
    bSaveLut = bSaveAsLut = bUseLut = false;
	
    CWnd *pwndXY =  GetDlgItem(IDC_LUTLable);
	pwndXY->SetWindowText("ColorRanges");
	
	grabbingThread->Start();
	
    SetTimer(0,10,NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorToolDlg::OnPaint() 
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
HCURSOR CColorToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CColorToolDlg::OnTimer(UINT nIDEvent) 
{
	if( grabbingThread->getFrameCounter() != frameCounter )
	{
		CClientDC			dc(this);
		CBitmap*			pOldObject = NULL;
		
 		grabbingThread->m_cs.Lock();
		
		RGBTuple rgb, bgr;
		rgb.r = bgr.r = 0;
		rgb.g = bgr.g = 0;
		rgb.b = bgr.b = 0;
		
		for (register int x=0; x < width; x++) 
		{
			for (register int y=0; y < height; y++) 
			{
			    grabbingThread->getImage().getPixelRGB(x,y, &rgb);
				bgr.r = rgb.b;
				bgr.g = rgb.g;
				bgr.b = rgb.r;
 				bufferedImage->setPixelRGB(x,y,bgr ); 		
			}
		}
		unsigned char* buffer =  bufferedImage->getImageBuffer().buffer;

		if( !buffer ) 
			return;
		
		m_bitmapInfo.bmiHeader.biWidth	  = width;
		m_bitmapInfo.bmiHeader.biHeight	  = 0 - height;
		m_bitmapInfo.bmiHeader.biBitCount =  24;
		
		pOldObject = m_memDc.SelectObject(m_pMemDcBitmap);
		
		int result;
		result = ::StretchDIBits(m_memDc.m_hDC,
			0, 0,				// Dest upper-left
			width, height,		// Dest size
			0, 0,				// Source upper-left
			width, height,		// Source size
			buffer,
			(BITMAPINFO*)&m_bitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
		
		ASSERT(result);
		grabbingThread->m_cs.Unlock();
		
		m_memDc.SetViewportOrg(0, 0);
		
		result = dc.BitBlt(0, 0, width, height, &m_memDc, 0, 0, SRCCOPY);
		ASSERT(result);
		
		m_memDc.SelectObject(pOldObject);  
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CColorToolDlg::InitSlidersAndLabels()
{
	//m_colorSelector.SetCurSel(0);
	int colorID = m_colorSelector.GetCurSel();
	//grabbingThread->getSegmentationTool()->setActiveColor(colorID); // lrd init ball valid
	ColorRange range = grabbingThread->getSegmentationTool()->getColorRange(colorID);
	
	m_HminSlider.SetRange(0,360);
	m_HmaxSlider.SetRange(0,360);
	m_HminSlider.SetPos((int)range.getMin(0));
	m_HmaxSlider.SetPos((int)range.getMax(0));
	
	m_SminSlider.SetRange(0,100);
	m_SmaxSlider.SetRange(0,100);
	m_SminSlider.SetPos((int)( range.getMin(1) * 100. ) );
	m_SmaxSlider.SetPos((int)( range.getMax(1) * 100. ) );
	
	m_IminSlider.SetRange(0,100);
	m_ImaxSlider.SetRange(0,100);
	m_IminSlider.SetPos((int)( range.getMin(2) * 100. ) );
	m_ImaxSlider.SetPos((int)( range.getMax(2) * 100. ) );
	
	m_HminEdit.SetValue((int)range.getMin(0));
	m_HmaxEdit.SetValue((int)range.getMax(0));
	
	m_SminEdit.SetValue((int)( range.getMin(1) * 100. ) );
	m_SmaxEdit.SetValue((int)( range.getMax(1) * 100. ) );
	
	m_IminEdit.SetValue((int)( range.getMin(2) * 100. ) );
	m_ImaxEdit.SetValue((int)( range.getMax(2) * 100. ) );
	
	if (colorID == 0)
	{
		ActivateSliders(FALSE);
	}
}

void CColorToolDlg::ActivateSliders(BOOL act)
{
	m_HminSlider.EnableWindow(act);
	m_HmaxSlider.EnableWindow(act);
	m_SminSlider.EnableWindow(act);
	m_SmaxSlider.EnableWindow(act);
	m_IminSlider.EnableWindow(act);
	m_ImaxSlider.EnableWindow(act);
	
	m_HminEdit.EnableWindow(act);
	m_HmaxEdit.EnableWindow(act);
	
	m_SminEdit.EnableWindow(act);
	m_SmaxEdit.EnableWindow(act);
	
	m_IminEdit.EnableWindow(act);
	m_ImaxEdit.EnableWindow(act);
	
	slidersActivated = act;
}

void CColorToolDlg::UpdateRanges()
{
	ColorRange range = grabbingThread->getSegmentationTool()->getColorRange(
		grabbingThread->getSegmentationTool()->getActiveColor() );
	
	m_HminEdit.SetValue((int)range.getMin(0));
	m_HmaxEdit.SetValue((int)range.getMax(0));
	
	m_SminEdit.SetValue((int)( range.getMin(1) * 100. ) );
	m_SmaxEdit.SetValue((int)( range.getMax(1) * 100. ) );
	
	m_IminEdit.SetValue((int)( range.getMin(2) * 100. ) );
	m_ImaxEdit.SetValue((int)( range.getMax(2) * 100. ) );
	
	m_HminSlider.SetPos((int)range.getMin(0));
	m_HmaxSlider.SetPos((int)range.getMax(0));
	
	m_SminSlider.SetPos((int)( range.getMin(1) * 100. ) );
	m_SmaxSlider.SetPos((int)( range.getMax(1) * 100. ) );
	
	m_IminSlider.SetPos((int)( range.getMin(2) * 100. ) );
	m_ImaxSlider.SetPos((int)( range.getMax(2) * 100. ) );
}

void CColorToolDlg::ColorChanged(int c)
{
	grabbingThread->getSegmentationTool()->setActiveColor(c);
	
	if (c==0)
	{
		ActivateSliders(FALSE);
	}
	else
	{
		if (!slidersActivated)
		{
			ActivateSliders(TRUE);
		}
		UpdateRanges();
	}
}

void CColorToolDlg::OnSetcolor() 
{
	int minvalue = (int)( (lastH-10) < 0. ? 0.:(lastH-10.) ); 
	int maxvalue = (int)( (lastH+10) > 360. ? 360.:(lastH+10.) );
	m_HminSlider.SetPos((int)(lastH-10));
	m_HmaxSlider.SetPos((int)(lastH+10));
	m_HminEdit.SetValue(minvalue);
	m_HmaxEdit.SetValue(maxvalue);
	
	minvalue = (int)( (100*lastS-10) < 0. ? 0.:(100*lastS-10.) ); 
	maxvalue = (int)( (100*lastS+10) > 100. ? 100.:(100*lastS+10.) );
	m_SminSlider.SetPos((int)( 100*lastS-10 ) );
	m_SmaxSlider.SetPos((int)( 100*lastS+10 ) );
	m_SminEdit.SetValue(minvalue);
	m_SmaxEdit.SetValue(maxvalue);
	
	minvalue = (int)( (100*lastI-10) < 0. ? 0.:(100*lastI-10.) ); 
	maxvalue = (int)( (100*lastI+10) > 100. ? 100.:(100*lastI+10.) );
	m_IminSlider.SetPos((int)( 100*lastI-10 ) );
	m_ImaxSlider.SetPos((int)( 100*lastI+10 ) );
	m_IminEdit.SetValue(minvalue);
	m_ImaxEdit.SetValue(maxvalue);	
}

void CColorToolDlg::OnAddcolor() 
{
	int minvalue = (int)( (lastH-10) < 0. ? 0.:(lastH-10.) ); 
	int maxvalue = (int)( (lastH+10) > 360. ? 360.:(lastH+10.) );
	m_HminSlider.SetPos((int)( MIN(lastH-10,m_HminSlider.GetPos() ) ) );
	m_HmaxSlider.SetPos((int)( MAX(lastH+10,m_HmaxSlider.GetPos() ) ) );
	m_HminEdit.SetValue((int)( MIN(minvalue,m_HminSlider.GetPos() ) ) );
	m_HmaxEdit.SetValue((int)( MAX(maxvalue,m_HmaxSlider.GetPos() ) ) );
	
	minvalue = (int)( (100*lastS-10) < 0. ? 0.:(100*lastS-10.) ); 
	maxvalue = (int)( (100*lastS+10) > 100. ? 100.:(100*lastS+10.) );
	m_SminSlider.SetPos((int)( MIN(100*lastS-10, m_SminSlider.GetPos() ) ) );
	m_SmaxSlider.SetPos((int)( MAX(100*lastS+10, m_SmaxSlider.GetPos() ) ) );
	m_SminEdit.SetValue((int)( MIN(minvalue, m_SminSlider.GetPos() ) ) );
	m_SmaxEdit.SetValue((int)( MAX(maxvalue, m_SmaxSlider.GetPos() ) ) ); 
	
	minvalue = (int)( (100*lastI-10) < 0. ? 0.:(100*lastI-10.) ); 
	maxvalue = (int)( (100*lastI+10) > 100. ? 100.:(100*lastI+10.) );
	m_IminSlider.SetPos((int)( MIN(100*lastI-10, m_IminSlider.GetPos() ) ) );
	m_ImaxSlider.SetPos((int)( MAX(100*lastI+10, m_ImaxSlider.GetPos() ) ) );
	m_IminEdit.SetValue((int)( MIN(minvalue, m_IminSlider.GetPos() ) ) );
	m_ImaxEdit.SetValue((int)( MAX(maxvalue, m_ImaxSlider.GetPos() ) ) );	
}


void CColorToolDlg::OnNextcolor() 
{
	int num;
	int active;
	num = m_colorSelector.GetCount();
	active = m_colorSelector.GetCurSel();
	m_colorSelector.SetCurSel((active+1)%num);
	ColorChanged((active+1)%num);	
}

void CColorToolDlg::OnPreviouscolor() 
{
	int num;
	int active;
	num = m_colorSelector.GetCount();
	active = m_colorSelector.GetCurSel();
	active = active - 1;
	if (active < 0)
	{
		active = num - 1;
	}
	m_colorSelector.SetCurSel((active));
	ColorChanged((active));	
}

void CColorToolDlg::OnColorselector() 
{
	int colorID;
    colorID = m_colorSelector.GetCurSel();
    ColorChanged(colorID);
}

void CColorToolDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( point.x >= 0 && point.x < width && point.y >=0 && point.y < height )
	{
		int x = point.x;
		int y = point.y;
		YUVTuple yuv;
		RGBTuple rgb;
		
		grabbingThread->getImage().getPixelYUV(x,y,&yuv); // get image form camera
		PixelConversion::convert(yuv,&rgb);

		rgb2hsy(rgb, lastH, lastS, lastI); // use the conical hsl space with y 
		
		stringstream meg;
		meg << "RGB: (" << (int)rgb.r << ", " << (int)rgb.g << ", " << (int)rgb.b 
			<< ")  YUV: (" << (int)yuv.y << ", " << (int)yuv.u << ", " << (int)yuv.v
			<< ")  HSY: (" <<setiosflags(ios::fixed)<<setprecision(3) << lastH 
			<< ", " << lastS << ", " << lastI << ")" << "POINT: (" << x <<", " << y << ")";
		m_StatusBar->SetText(meg.str().c_str(),0,0);
        
		colorField->SetStatusColor(RGB((int)(rgb.r),(int)(rgb.g),(int)(rgb.b)));
		
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CColorToolDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (pScrollBar == (CScrollBar*)&m_HminSlider)
	{
		const ULONG value = (ULONG) m_HminSlider.GetPos();
		m_HminEdit.SetValue(value);
		if (nSBCode == SB_ENDSCROLL )
		{
			hMinChanged(value);
		}
		
	}
	
	if (pScrollBar == (CScrollBar*)&m_HmaxSlider)
	{
		const ULONG value = (ULONG) m_HmaxSlider.GetPos();
		m_HmaxEdit.SetValue(value);
		
		if (nSBCode == SB_ENDSCROLL )
		{
			hMaxChanged(value);
		}
	}
    
	if (pScrollBar == (CScrollBar*)&m_SminSlider)
	{
		const ULONG value = (ULONG) m_SminSlider.GetPos();
		m_SminEdit.SetValue(value);
		if (nSBCode == SB_ENDSCROLL )
		{
			sMinChanged(value);
		}
	}
    
	if (pScrollBar == (CScrollBar*)&m_SmaxSlider)
	{
		const ULONG value = (ULONG) m_SmaxSlider.GetPos();
		m_SmaxEdit.SetValue(value);
		if (nSBCode == SB_ENDSCROLL )
		{
			sMaxChanged(value);
		}
	}
	
	if (pScrollBar == (CScrollBar*)&m_IminSlider)
	{
		const ULONG value = (ULONG) m_IminSlider.GetPos();
		m_IminEdit.SetValue(value);
		if (nSBCode == SB_ENDSCROLL )
		{
			iMinChanged(value);
		}
		
	}
	
	if (pScrollBar == (CScrollBar*)&m_ImaxSlider)
	{
		const ULONG value = (ULONG) m_ImaxSlider.GetPos();
		m_ImaxEdit.SetValue(value);
		
		if (nSBCode == SB_ENDSCROLL )
		{
			iMaxChanged(value);
		}
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorToolDlg::hMinChanged(int min)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMin(0, min);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	if (m_HminSlider.GetPos() != min ) m_HminSlider.SetPos(min);
	if (m_HminEdit.GetValue() != min ) m_HminEdit.SetValue(min);
	
}

void CColorToolDlg::hMaxChanged(int max)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMax(0, max);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	
	if (m_HmaxSlider.GetPos() != max ) m_HmaxSlider.SetPos(max);
	if (m_HmaxEdit.GetValue() != max ) m_HmaxEdit.SetValue(max);
}

void CColorToolDlg::sMinChanged(int min)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMin(1, min / 100.);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	if (m_SminSlider.GetPos() != min ) m_SminSlider.SetPos(min);
	if (m_SminEdit.GetValue() != min ) m_SminEdit.SetValue(min);
}

void CColorToolDlg::sMaxChanged(int max)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMax(1, max / 100.);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	if (m_SmaxSlider.GetPos() != max ) m_SmaxSlider.SetPos(max);
	if (m_SmaxEdit.GetValue() != max ) m_SmaxEdit.SetValue(max);
}

void CColorToolDlg::iMinChanged(int min)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMin(2, min / 100.);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	if (m_IminSlider.GetPos() != min ) m_IminSlider.SetPos(min);
	if (m_IminEdit.GetValue() != min ) m_IminEdit.SetValue(min);
}

void CColorToolDlg::iMaxChanged(int max)
{
	int id = grabbingThread->getSegmentationTool()->getActiveColor();
	ColorRange range =
		grabbingThread->getSegmentationTool()->getColorRange(id);
	range.setMax(2, max / 100.);
	grabbingThread->getSegmentationTool()->setColorRange(id, range);
	if (m_ImaxSlider.GetPos() != max ) m_ImaxSlider.SetPos(max);
	if (m_ImaxEdit.GetValue() != max ) m_ImaxEdit.SetValue(max);
	
}

void CColorToolDlg::OnChangeHminEdit() 
{
	const ULONG value = m_HminEdit.GetValue();
	m_HminSlider.SetPos((int)value);
	hMinChanged(value);
}

void CColorToolDlg::OnChangeHmaxEdit() 
{
	const ULONG value = m_HmaxEdit.GetValue();
	m_HmaxSlider.SetPos((int)value);
	hMaxChanged(value);
}

void CColorToolDlg::OnChangeSminEdit() 
{
	const ULONG value = m_SminEdit.GetValue();
	m_SminSlider.SetPos((int)value);
	sMinChanged(value);
}

void CColorToolDlg::OnChangeSmaxEdit() 
{
	const ULONG value = m_SmaxEdit.GetValue();
	m_SmaxSlider.SetPos((int)value);
	sMaxChanged(value);
}

void CColorToolDlg::OnChangeIminEdit() 
{
	const ULONG value = m_IminEdit.GetValue();
	m_IminSlider.SetPos((int)value);
	iMinChanged(value);
}

void CColorToolDlg::OnChangeImaxEdit() 
{
	const ULONG value = m_ImaxEdit.GetValue();
	m_ImaxSlider.SetPos((int)value);
	iMaxChanged(value);
}

void CColorToolDlg::OnShowOrigimage() 
{
	showIt = !showIt;
	grabbingThread->showOriginalImage(showIt);	
}

void CColorToolDlg::OnUpdateShowOrigimage(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showIt);	
}

void CColorToolDlg::OnCreatLookupTable() 
{
	Time t1;
	
	m_StatusBar->SetText("Create a lookup table from the present settings,This will take a few seconds.Sorry for not responding.",0,0 );
	grabbingThread->getSegmentationTool()->createLookupTable();
	long int elaspedtime = t1.elapsed_sec();
	CString sTmp;
	sTmp.Format("Have finished the creation of the lookup table! Time consumed was %d s. ", elaspedtime );
	m_StatusBar->SetText(sTmp,0,0 );
	bSaveLut = bSaveAsLut = bUseLut = true;
}

void CColorToolDlg::OnFileOpen() 
{
	CString filepath;
	CString filename2;
	CString filter_str = "Ranges file (*.ranges)|*.ranges|All files(*.*)|*.*||";
	
	CFileDialog filedlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, this );
	filedlg.m_ofn.lpstrInitialDir=".//";
	
	if( filedlg.DoModal() == IDOK )
	{
		filepath  = filedlg.GetPathName();
		filename2 = filedlg.GetFileName();
	}
	if( filename2 != "" )
	{
		grabbingThread->getSegmentationTool()->load(filepath.GetBuffer(0));
		UpdateRanges();
		m_StatusBar->SetText("Loaded ranges from file " + filepath,0,0);
		this->filename = filepath.GetBuffer(0);        
	}
	
}

void CColorToolDlg::OnFileSave() 
{
	if( filename == "" )
	{
		OnFileSaveAs();
	}
	else
	{
		SaveRanges();
	}
}

void CColorToolDlg::OnFileSaveAs() 
{
	CString filepath;
	CString fname;
    CString filter_str = "*.ranges";
	CFileDialog filedlg(FALSE,"ranges",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter_str,this);
	filedlg.m_ofn.lpstrInitialDir="..//";
	
	if(filedlg.DoModal()==IDOK)
	{
		filepath = filedlg.GetPathName();
		fname    = filedlg.GetFileName();
	}	
	
	if( fname != "" )
	{
		this->filename = filepath.GetBuffer(0);
		SaveRanges();
	}	
}

void CColorToolDlg::SaveRanges()
{
	grabbingThread->getSegmentationTool()->save(filename);
	CString sTmp = filename.c_str();
	m_StatusBar->SetText("Saved ranges to file " + sTmp,0,0);
}

void CColorToolDlg::OnOpenLookupTable() 
{
	CString filepath;
	CString fname;
	CString filter_str = "LookupTable file (*.lut)|*.lut|All files(*.*)|*.*||";
	
	CFileDialog filedlg(TRUE,"lut",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, this );
	filedlg.m_ofn.lpstrInitialDir=".//";
	
	if( filedlg.DoModal() == IDOK )
	{
		filepath = filedlg.GetPathName();
		fname = filedlg.GetFileName();
	}
	if( fname != "" )
	{
		grabbingThread->getSegmentationTool()->loadLookupTable(filepath.GetBuffer(0));
		m_StatusBar->SetText("Loaded lookuptable from file " + filepath,0,0);
		
		this->lutFilename = filepath.GetBuffer(0);  
		bSaveLut = bSaveAsLut = bUseLut = true;
	}	
}

void CColorToolDlg::OnSaveAsLookupTable() 
{
    CString filepath;
	CString fname;
	CString filter_str = "*.lut";
	CFileDialog filedlg(FALSE,"lut",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter_str,this);
	filedlg.m_ofn.lpstrInitialDir="..//";
	
	if(filedlg.DoModal()==IDOK)
	{
		filepath = filedlg.GetPathName();
		fname    = filedlg.GetFileName();
	}	
	
	if( fname != "" )
	{
		this->lutFilename = filepath.GetBuffer(0);
		DoSaveLookupTable();
	}		
}

void CColorToolDlg::OnSaveLookupTable() 
{
	if (lutFilename == "")
	{
		OnSaveAsLookupTable();
	}
	else
	{
		DoSaveLookupTable();
	}	
}

void CColorToolDlg::DoSaveLookupTable()
{
	grabbingThread->getSegmentationTool()->saveLookupTable(lutFilename);
	CString sTmp = lutFilename.c_str();
	m_StatusBar->SetText("Saved lookup table to file " + sTmp,0,0);
}

void CColorToolDlg::OnUseLookupTable() 
{
	bUseLookupTable = !bUseLookupTable;
	if( bUseLookupTable && !grabbingThread->getSegmentationTool()->haveLookupTable() )
	{
		m_StatusBar->SetText("Could not do that! Please first create a lookup table.",0,0);
		
	}
	else
	{
		CWnd *pwndXY =  GetDlgItem(IDC_LUTLable);
		if (!bUseLookupTable)   pwndXY->SetWindowText("Ranges");
		else                    pwndXY->SetWindowText("LookUp");
		ActivateSliders(!bUseLookupTable);
		GetDlgItem(IDC_PREVIOUSCOLOR)->EnableWindow(!bUseLookupTable);
		GetDlgItem(IDC_NEXTCOLOR)->EnableWindow(!bUseLookupTable);
		
		m_colorSelector.EnableWindow(!bUseLookupTable);
		grabbingThread->getSegmentationTool()->useLookupTable(bUseLookupTable);
		m_StatusBar->SetText("Use the present lookup table to segment the image. Sliders will be deactivated.",0,0);
		
	}
}

void CColorToolDlg::OnQuickLoad() 
{
	string fname = "..\\RobotControl\\config_files\\colors.ranges";
	CFileFind finder;
	CString sTmp = fname.c_str();
	BOOL bExists = finder.FindFile(sTmp);
	if( bExists )
	{
		grabbingThread->getSegmentationTool()->load(fname);
		UpdateRanges();
		m_StatusBar->SetText("Loaded ranges from file " + sTmp,0,0);
		this->filename = fname;       
	}
	
	fname	= "..\\RobotControl\\config_files\\colors.lut";
	sTmp		= fname.c_str();
	bExists	= finder.FindFile(sTmp);
	if( bExists )
	{
		grabbingThread->getSegmentationTool()->loadLookupTable(fname);
		m_StatusBar->SetText("Loaded lookuptable from file " + sTmp,0,0);
		this->lutFilename = fname;  
		
		bSaveLut = bSaveAsLut = bUseLut = true;
	}
	else
		m_StatusBar->SetText("lookuptable is not found! Please check file path.",0,0);
	
}

void CColorToolDlg::OnQuickSave() 
{
	
	string fname = "..\\RobotControl\\config_files\\colors.ranges";
	CString sTmp = fname.c_str();
	
	grabbingThread->getSegmentationTool()->save(fname);
	m_StatusBar->SetText("Saved ranges to file " + sTmp,0,0);
	this->filename = fname;       
	
	fname	= "..\\RobotControl\\config_files\\colors.lut";
	sTmp = fname.c_str();
	
	grabbingThread->getSegmentationTool()->saveLookupTable(fname);
	m_StatusBar->SetText("Saved lookup table to file " + sTmp,0,0);
	this->lutFilename = fname;  
	
}

void CColorToolDlg::OnUpdateSavelut(CCmdUI* pCmdUI) 
{
	if( bSaveLut == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CColorToolDlg::OnUpdateSaveaslut(CCmdUI* pCmdUI) 
{
	if( bSaveAsLut == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);	
}

void CColorToolDlg::OnUpdateUselut(CCmdUI* pCmdUI) 
{
	if( bUseLut == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck(bUseLookupTable);
}

void CColorToolDlg::OnUpdateFilesaveAs(CCmdUI* pCmdUI) 
{
    if( producer->producers[0]->GetImageSourceType() == "FileSource" ) 
	{
		pCmdUI->Enable(FALSE);
		bSaveAsLut = false;
    }
	else
	{
		pCmdUI->Enable();
	}
}

void CColorToolDlg::OnUpdateFileopen(CCmdUI* pCmdUI) 
{
    if( producer->producers[0]->GetImageSourceType() == "FileSource" ) 
	{
		pCmdUI->Enable(FALSE);
    }
	else
		pCmdUI->Enable();	
}

void CColorToolDlg::OnUpdateOpenlut(CCmdUI* pCmdUI) 
{
    if( producer->producers[0]->GetImageSourceType() == "FileSource" ) 
	{
		pCmdUI->Enable(FALSE);
    }
	else
		pCmdUI->Enable();	
}
 
void CColorToolDlg::OnUpdateCamera(CCmdUI* pCmdUI) 
{
	if( producer->producers[0]->GetImageSourceType() == "FileSource" )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable();	
}


// disable menu
void CColorToolDlg::OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu)     
{   
	if( !bSysMenu )   
	{   
		ASSERT(pPopupMenu != NULL);   
        
		CCmdUI   state;                   
		state.m_pMenu = pPopupMenu;   
		ASSERT(state.m_pOther == NULL);   
        
		state.m_nIndexMax  = pPopupMenu->GetMenuItemCount();   
		for(state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++ )   
		{   
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);   
			if( state.m_nID == 0 )   
				continue;     
			
			ASSERT(state.m_pOther == NULL);   
			ASSERT(state.m_pMenu != NULL);   
			if( state.m_nID == (UINT)-1 )   
			{   
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);   
				if(state.m_pSubMenu == NULL || ( state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||   
					state.m_nID == (UINT)-1)   
				{                                              
					continue;   //   first   item   of popup   can't   be   routed   to   
				}   
				state.DoUpdate(this,   FALSE);     //   popups  are   never   auto   disabled   
			}   
			else   
			{   
				state.m_pSubMenu   =   NULL;   
				state.DoUpdate(this,  state.m_nID < 0xF000);   
			}   
		}   
	}   
}   

//enable hot_key to create lookuptable 
BOOL CColorToolDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccel)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel2)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel2,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel3)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel3,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel4)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel4,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel5)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel5,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel6)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel6,pMsg))
		{
			return(true);
		}
	}
	if (m_hAccel7)
	{
		if (::TranslateAccelerator(m_hWnd,m_hAccel7,pMsg))
		{
			return(true);
		}
	}
	
	switch( pMsg->message ) 
	{
	case WM_KEYDOWN:
		{			
			switch(pMsg->wParam) 
			{
			case 'S':
				bStopReadImage = !bStopReadImage;
				break;
			default:
				break;
			} 
			
		}
		break;
	default:
		break;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

CParameterControls* CColorToolDlg::getControlDialog()
{
	while (parameter_control == NULL)
	{
		parameter_control = new CParameterControls;
		parameter_control->Create(IDD_PARAMETER_CONTROLS,this);
		CRect rc_pc;
		parameter_control->GetClientRect(&rc_pc);
		parameter_control->MoveWindow(650,10,rc_pc.Width()+10,rc_pc.Height()+30);
	}
	return parameter_control;
}

void CColorToolDlg::OnCamera() 
{
	if( producer->producers[0]->GetImageSourceType() == "CameraSource") 
	{
		if ( getControlDialog()->GetStyle() & WS_VISIBLE )
		{
			getControlDialog()->ShowWindow(SW_HIDE);
		}
		else
		{
			getControlDialog()->ShowWindow(SW_SHOW);
			getControlDialog()->RefreshControls();
		}
	}
	else if (producer->producers[0]->GetImageSourceType() == "UsbCameraSource")
	{
		UsbCamera* img = UsbCamera::getCamera(); 
		img->DisplayFilterProperties();
	}
}

