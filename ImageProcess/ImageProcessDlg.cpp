// ImageProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include <fstream>

using namespace std;

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
// CImageProcessDlg dialog

CImageProcessDlg::CImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageProcessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	widget			= NULL;
	producer		= NULL;
	image			= NULL;
    lines           = NULL;
	scanner         = NULL;
    warpImage       = NULL;
    regionMap       = NULL;
	mapping			= NULL;    
	results         = NULL;
	
	useLut = false;
	deWarp = false;
	bShowScnalines = false;   
    bShowResults   = true;
	width = 0;
	height = 0;
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageProcessDlg)
	DDX_Control(pDX, IDC_COLORSELECTOR, m_colorSelector);
	DDX_Control(pDX, IDC_SHOW_RESULTS, m_showresults);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CImageProcessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SHOW_SEGMENTATION, OnShowSegmentation)
	ON_BN_CLICKED(IDC_SHOW_WARP_IMAGE, OnShowWarpImage)
	ON_BN_CLICKED(IDC_SHOW_SCNALINES, OnShowScnalines)
	ON_BN_CLICKED(IDC_SHOW_RESULTS, OnShowResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessDlg message handlers

BOOL CImageProcessDlg::OnInitDialog()
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

    m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );
    
    string configfile = "config_files\\robotcontrol.cfg";
	ConfigReader vread;
	bool success = vread.append_from_file(configfile.c_str());
    
	//colClass.classList[COLOR_BLUE]->findTransitions=true;
    //colClass.classList[COLOR_BALL]->findTransitions=f;
    //colClass.classList[COLOR_BALL]->findOnlyFirst=false;

    try
	{
       	producer = new MultiImageProducer(vread); 
	   	image = producer->nextImage();
		width = image->getWidth();
		height = image->getHeight();

		string  robotMaskFilename = "";
		if (vread.get("ScanLine::robot_mask_file", robotMaskFilename) < 0) 
		{
		   throw InvalidConfigurationException("ScanLine::robot_mask_filename");
		}    
		if( robotMaskFilename != "" ) 
		{
 		     CString sTmp;
			 sTmp  = robotMaskFilename.c_str();
  	 	     m_StatusBar->SetText( "Loading robot mask file: " + sTmp,0,0 );
 		     RobotMask::load(robotMaskFilename);
		}
  
		if (vread.get("ScanLine::image_center_x", centerX) <= 0) 
		{
		   throw InvalidConfigurationException("ScanLine::image_center_x");
		}   
		if (vread.get("ScanLine::image_center_y", centerY) <= 0) 
		{
		   throw InvalidConfigurationException("ScanLine::image_center_y");
		}   
		if (vread.get("ScanLine::inner_scan_radius", innerRadius) <= 0) 
		{
		   throw InvalidConfigurationException("ScanLine::inner_scan_radius");
		}   
		if (vread.get("ScanLine::outer_scan_radius", outerRadius) <= 0) 
		{
		   throw InvalidConfigurationException("ScanLine::outer_scan_radius");
		}   
		if (vread.get("ScanLine::number_of_scanlines", nScanLines) <= 0) 
		{
		   throw InvalidConfigurationException("ScanLine::number_of_scanlines");
		}  
		
 		lines  = new ScanLines(Vec(centerX,centerY), 
					 innerRadius, outerRadius,
					 width, height, nScanLines );
		scanner = new LineScanner(lines, &colClass);
   
 		CRect rect(0,0,width,height);
		widget = new CImageProcessingMonitor(vread,image);
		widget->Create( NULL,NULL,
						WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
						rect,  this,  0 );
	 
	 
		regionMap = new RGBImage(width, height);
 	 
		warpImage = new YUVImage(WARP_WIDTH / 20, WARP_HEIGHT / 20);
		 

		string mapfile;
		if (vread.get("ScanLine::image2world_marker_file", mapfile) <= 0) 
		{
		   throw InvalidConfigurationException("main::"
				  "image2world_marker_file");
		}

		mapping = new World2Image(mapfile);    

        results = new ScanResultList(colClass.classList.size());
	}
	catch (RobotsException& e)
	{
		MessageBox(e.what());
	}
	
    m_showresults.SetCheck(1);

	m_colorSelector.InsertString(0,"ALL");
   
	for (unsigned int i=1; i<colClass.classList.size(); i++ )
	{
		CString str, sTmp;
		sTmp   = colClass.classList[i]->name.c_str();
		sTmp   += "  %d"; 
		int  id = colClass.classList[i]->id;
		str.Format(sTmp,id);
		str.TrimLeft();
		m_colorSelector.InsertString(i,str);
		
	}
	
    int colorID = m_colorSelector.GetCurSel();
    widget->setColorId(colorID);

     
	bStopReadImage = false;
	imgballpos = Vec::zero_vector;
	realballpos =  Vec::zero_vector;
    SetTimer(0,33,NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageProcessDlg::OnPaint() 
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
HCURSOR CImageProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
  
void CImageProcessDlg::OnTimer(UINT nIDEvent) 
{
    results->clear();
	Image* newImage = producer->nextImage();
    
	if( !deWarp && bShowResults ) 
	{
		//scanner->scan(*newImage, results);
		//ballDetector->setVisualize(newImage);
		//ballDetector->searchBall(*newImage,results->results[COLOR_BALL]);
		
		int colorID = m_colorSelector.GetCurSel();
		
        widget->setColorId(colorID);
		widget->setScanResults(results);
	}

	if( useLut ) 
	{
		int c;
		for (int x=0; x < newImage->getWidth(); x++) 
		{
			for (int y=0; y < newImage->getHeight(); y++) 
			{
				c = newImage->getPixelClass(x,y);
				regionMap->setPixelRGB(x,y, colClass.classList[c]->color);
			}
		}
		widget->setImage(regionMap);
	}
	else if( deWarp ) 
	{

		YUVTuple yuv;
        ofstream cerr;
		cerr.open("errotmap.txt");

		for (int x=0; x < warpImage->getWidth(); x++) 
		{
			for (int y=0; y < warpImage->getHeight(); y++) 
			{
				Vec real( ( x - warpImage->getWidth() / 2 ) * 20, 
						   ( y - warpImage->getHeight() / 2 ) * 20 );

				Vec img = mapping->map(real); // image Coordinates
				if( (int)img.x < 0 || (int)img.x > newImage->getWidth() ||
				(int)img.y < 0 || (int)img.y > newImage->getHeight()) 
				{
				  m_StatusBar->SetText( "Error mapping world coords to image ! ",0,0 );
				  
				  cerr << "Error mapping world coords to image. Real: " 
				   << real.x << ", " << real.y << " Image: "
				   << img.x << ", " << img.y << endl;
				  // exit(1);
				  return;
				}
				newImage->getPixelYUV((int)img.x, (int)img.y, &yuv);
				warpImage->setPixelYUV(x,y, yuv);
			}
		}
		cerr.close();

		widget->setImage(warpImage);
	}
	else 
	{
	    widget->setImage(newImage);
	}

	delete image;
	image = newImage;
   
	widget->Invalidate(FALSE); 
	CString str;
	str.Format("ball_ix: %.f\nball_iy:%.f",imgballpos.x,imgballpos.y);
    GetDlgItem(IDC_BALLPOS_IMAGE)->SetWindowText(str);
	str.Format("ball_rx: %.f\nball_ry:%.f",realballpos.x,realballpos.y);
    GetDlgItem(IDC_BALLPOS_REAL)->SetWindowText(str);
	CDialog::OnTimer(nIDEvent);
}

void CImageProcessDlg::OnShowSegmentation() 
{
   useLut = !useLut;
   GetDlgItem(IDC_SHOW_WARP_IMAGE)->EnableWindow(!useLut); 
} 

void CImageProcessDlg::OnShowWarpImage() 
{
   deWarp = !deWarp;
   
   widget->setScanLines(NULL);
   GetDlgItem(IDC_SHOW_SEGMENTATION)->EnableWindow(!deWarp); 
   GetDlgItem(IDC_SHOW_SCNALINES)->EnableWindow(!deWarp);  	
   GetDlgItem(IDC_SHOW_RESULTS)->EnableWindow(!deWarp); 
   
   if( !deWarp )
   {
      if(((CButton*)GetDlgItem(IDC_SHOW_SCNALINES))->GetCheck() == 1 )
         widget->setScanLines(lines);
	  else
         widget->setScanLines(NULL);
   }
   
}
  
void CImageProcessDlg::OnShowScnalines() 
{
   bShowScnalines = !bShowScnalines;
   if (bShowScnalines)
  	 widget->setScanLines(lines);
   else
	 widget->setScanLines(NULL);
	
}

void CImageProcessDlg::OnShowResults() 
{
	bShowResults = !bShowResults;

	m_colorSelector.EnableWindow(bShowResults);
	if( bShowResults ) 
   	  widget->setScanResults(results);
   	else
      widget->setScanResults(NULL);
}

void CImageProcessDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    if(widget)     delete widget;
	if(producer)   delete producer;
 	if(image)      delete image;
    if(scanner)    delete scanner;
    if(warpImage)  delete warpImage;
	if(regionMap)  delete regionMap;
	if(mapping)    delete mapping;
	if(results)    delete results;
	
    if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
	    m_StatusBar = NULL;
	}
}

//enable hot_key to create lookuptable 
BOOL CImageProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	switch( pMsg->message ) 
	{
		case WM_KEYDOWN:
			{			
				switch(pMsg->wParam) 
				{
 					case 'S':
						SetFocus();
						bStopReadImage = !bStopReadImage;
						break;
					default:
						break;
				} 
		
			}
			break;
		case WM_KEYUP:
            SetFocus();
			break;
		default:
			break;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}