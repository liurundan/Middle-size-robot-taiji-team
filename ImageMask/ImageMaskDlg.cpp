// ImageMaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageMask.h"
#include "ImageMaskDlg.h"

#include "MorphologicOperators.h"
#include "Painter.h"

#include <fstream>
#include <cmath>
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
// CImageMaskDlg dialog

CImageMaskDlg::CImageMaskDlg(CWnd* pParent /*=NULL*/)
: CDialog(CImageMaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageMaskDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	robot			= NULL;
	image			= NULL;
	bufferedImage	= NULL;
	buffer			= NULL;
	producer        = NULL;
    whiteClassifier	= NULL;
    blackClassifier	= NULL;
    regionDetector	= NULL;
    distanceFilter	= NULL;
	mask			= NULL;
	mask2           = NULL;
    
	m_bitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biPlanes			= 1;
	m_bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	m_bitmapInfo.bmiHeader.biSizeImage		= 0;
	m_bitmapInfo.bmiHeader.biXPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biYPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biClrUsed		= 0;
	m_bitmapInfo.bmiHeader.biClrImportant	= 0;
}

void CImageMaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageMaskDlg)
	DDX_Control(pDX, IDC_SHOWSHRESHOLD, m_threshold);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_thresholdSlider);
	DDX_Control(pDX, IDC_MSCOMM1, m_Com);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImageMaskDlg, CDialog)
//{{AFX_MSG_MAP(CImageMaskDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(Image_Changed, setImage )
	ON_COMMAND(IDM_FILESAVE, OnFilesave)
	ON_UPDATE_COMMAND_UI(IDM_FILESAVE, OnUpdateFilesave)
	ON_COMMAND(IDM_FILESAVEAS, OnFilesaveas)
	ON_UPDATE_COMMAND_UI(IDM_FILESAVEAS, OnUpdateFilesaveas)
	ON_COMMAND(IDM_WRITECONFIG, OnWriteconfig)
	ON_UPDATE_COMMAND_UI(IDM_WRITECONFIG, OnUpdateWriteconfig)
	ON_WM_HSCROLL()
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_CAPTURE, OnCapture)
	ON_BN_CLICKED(IDC_DRAW_CIRCLE, OnDrawCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CREATMASK, OnCreatmask)
	ON_BN_CLICKED(IDC_USE_MASK, OnUseMask)
	ON_COMMAND(IDM_QUICK_LOAD, OnQuickLoad)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageMaskDlg message handlers

BOOL CImageMaskDlg::OnInitDialog()
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
    m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0,0,0,0), this, 0 );
 	
	programStart	= true;
    capturing		= false;
	state			= false;
	bStarttimer     = false;
	bSave = bSaveAs =  bWriteConf = false;
	GetDlgItem(IDC_CREATMASK)->EnableWindow(FALSE);
	GetDlgItem(IDC_USE_MASK)->EnableWindow(FALSE);
	
    threshold		= 3;
	m_thresholdSlider.SetRange(0,255);
	m_thresholdSlider.SetPos(threshold);
	CString sTmp;
	sTmp.Format("%d",3);
	m_threshold.SetWindowText(sTmp);
	
	whiteRegionMin	= 4000;
    blackRegionMin	= 1000;
	
	centerX			= 320;
	centerY			= 240;
	
	filename		= "config_files\\image_mask.ppm";
    configFileName  = "config_files\\image_center.cfg";
	
    string configfile = "config_files\\robotcontrol.cfg";
	bool success = vread.append_from_file(configfile.c_str());
	
    if (!success) 
	{
		MessageBox("read conf.cfg error!");
		return true;
    }
	
    try
	{
		producer = new MultiImageProducer(vread); 
		image = producer->nextImage();
		width = image->getWidth();
		height = image->getHeight();
		
		bufferedImage	= new RGBImage(width,height);
		buffer			= new BYTE[ width * height * 3 ];
		m_memDc.CreateCompatibleDC(GetDC());
		m_pMemDcBitmap = new CBitmap();
		m_pMemDcBitmap->CreateCompatibleBitmap(GetDC(), width, height);
		
		ShowImage(*image);
		// producer = new ImageProducer(vread,"Camera_default","ImageProducer_File");
	}
	catch (RobotsException& e)
	{
		MessageBox(e.what());
	}
	
	radius			= 240;
    center          = CPoint(320,240);
    pointleft       = CPoint(120,240);
	pointright      = CPoint(520,240);
    drawcircle        = false;
    m_bIsDragingleft  = false;
	m_bIsDragingright = false;
	bUsemask          = false;
    bUsefilemask      = false;
   	
    distanceFilter  = new ImageDistanceFilter();
    //distanceFilter  = new ImageMaxDistanceFilter();
	
    whiteClassifier = new WhiteClassifier();
    blackClassifier = new BlackClassifier();
    regionDetector  = new RegionDetector();
    
	robot = 0;
	try 
	{
        robot = new Robot(vread);
	} 
	catch (RobotsException& e) 
	{
        robot = 0;
        MessageBox(e.what());
	}  
	catch (std::exception& e)
	{
		robot = 0;
        MessageBox(e.what());
	}
	
	SetTimer(0,30,NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageMaskDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageMaskDlg::OnPaint() 
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
HCURSOR CImageMaskDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CImageMaskDlg::OnTimer(UINT nIDEvent) 
{
    //  delete image;
	//	image = producer->nextImage();
	
	if( nIDEvent == 1 )
	{
		captureoff();
		return;
	}
	else
	{
		delete image;
		image = producer->nextImage();
		if (capturing) 
		{
			distanceFilter->add(*image);
			
			if( robot ) 
			{   
				robot->set_drive_vector(DriveVector(Vec(0.,0.), 0.5, false));
			}
			else 
			{ 
				SendMessage(Image_Changed);
			}
		}
		else if( programStart )
		{
			SendMessage(Image_Changed);
		}
		else 
		{
			if( mask ) 
			{
				SendMessage(Image_Changed,1);
			}
		}
	} 
	
	CDialog::OnTimer(nIDEvent);
}

void CImageMaskDlg::OnCapture() 
{
    state = !state;
	
	if( state ) 
	{   // capture on
		m_threshold.EnableWindow(FALSE);
		m_thresholdSlider.EnableWindow(FALSE);
		bSave   = false;    
		bSaveAs = false;
		
		distanceFilter->initialize(image->getWidth(), image->getHeight());    
		
		programStart = false;
		capturing    = true;
        
		if( robot ) 
		{
			GetDlgItem(IDC_CAPTURE)->EnableWindow(FALSE);
			SetTimer(1,9000,NULL);
			bStarttimer = true;
		}
	} 
    else 
	{      // capture switched off
		if( distanceFilter->getNumImagesAdded() < 1 ) 
		{
			MessageBox("Please grab at least 2 images");
			
			//captureButton->setOn(true);  // grab at least two images, set button on 
			state = false;
			OnCapture();
			return;
		}
		
		if (robot) 
		{
			GetDlgItem(IDC_CAPTURE)->EnableWindow(TRUE);
		}
		
		this->recalculateMask();
		this->recalculateImageCenter();
		
		//textLabel1->setEnabled(true);
		m_threshold.EnableWindow(TRUE);
		m_thresholdSlider.EnableWindow(TRUE);
		bSave   = true;    
		bSaveAs = true;
		
		capturing = false;
	}	 
}

void CImageMaskDlg::captureoff()
{
	state = true;
	OnCapture();
	if( bStarttimer )
	{
		KillTimer(1);
		bStarttimer = false;
	}
}

void CImageMaskDlg::recalculateMask()
{
	if (mask) delete mask;
	mask = 0;
	mask = distanceFilter->createMask(threshold);
	
	Erosion erosion = Erosion(3);
	Image* tmp = erosion(*mask);
	delete mask;
	mask = tmp;
	
	mask->setBlackBorder();
	
	RGBTuple black = { 0, 0, 0  };
	mask->setClassifier(whiteClassifier);
	vector<Region> regions;
	
	regionDetector->findRegions(*mask, 1, &regions);
	
	for (unsigned int i=0; i < regions.size(); i++) 
	{
		if (regions[i].getArea() < whiteRegionMin)
		{
			fillRegion(mask, regions[i], black);
		}
	}
	
	mask->setWhiteBorder();
	
	mask->setClassifier(blackClassifier);
	RGBTuple white = { 255, 255, 255 };
	mask->setClassifier(blackClassifier);
	
	regions.clear();
	regionDetector->findRegions(*mask, 1, &regions);
	
	for (i=0; i < regions.size(); i++)
	{
		if (regions[i].getArea() < blackRegionMin)
		{
			fillRegion(mask, regions[i], white);
		}
	}
	
	for ( i=0; i < 4; i++) 
	{
		tmp = erosion(*mask);
		delete mask;
		mask = tmp;
	}
	
	mask->setBlackBorder();  
}


void CImageMaskDlg::thresholdChanged(int threshold)
{
	this->threshold = threshold;
	if( !capturing && !programStart ) 
	{
		this->recalculateMask();
		this->recalculateImageCenter();
	}
}

void CImageMaskDlg::recalculateImageCenter()
{
	double cX = 0;
	double cY = 0;
	int count = 0;
	mask->setClassifier(whiteClassifier);  
	for (int x=0; x < mask->getWidth(); x++) 
	{
		for (int y=0; y < mask->getHeight(); y++) 
		{
			if (mask->getPixelClass(x,y) == 1) 
			{
				cX += x; cY += y;
				count++;
			}
		}
	}
	centerX = (int)(cX / count);
	centerY = (int)(cY / count);
	
	CString sTmp;
	sTmp.Format("New center calculated:   %d ,  %d ", centerX, centerY);
	m_StatusBar->SetText(sTmp,0,0);
}

void CImageMaskDlg::fillRegion(Image *image, const Region &region, const RGBTuple &color)
{
	int x = region.x;
	int y = region.y;
	
	for (unsigned int i=0; i < region.chainCode.size(); i++) 
	{
		int dir = region.chainCode[i];
		
		if (dir == 3 || dir == 0) 
		{     
			int xImg = x;
			while (image->getPixelClass(xImg,y) == region.colClass && 
                xImg < image->getWidth()) 
			{
				image->setPixelRGB(xImg, y, color);
				xImg++;
			}
		} 
		x += ChainCoder::xDir[dir];
		y += ChainCoder::yDir[dir];
	} 
	
	int xImg = x;
	while (image->getPixelClass(xImg,y) == region.colClass && 
		xImg < image->getWidth()) 
	{
		image->setPixelRGB(xImg, y, color);
		xImg++;
	}  
}

void CImageMaskDlg::ShowImage( Image& image )
{
	
	CClientDC			dc(this);
	CBitmap*			pOldObject = NULL;
	
	RGBTuple rgb = { 0, 0, 0 };
	RGBTuple bgr = { 0, 0, 0 };
    RGBTuple tem = { 255, 255, 255 };
	
	for (int x=0; x < width; x++) 
	{
		for (int y=0; y < height; y++) 
		{
			image.getPixelRGB(x,y, &rgb);
			bgr.r = rgb.b;
			bgr.g = rgb.g;
			bgr.b = rgb.r;
			
			if( bUsemask && mask )
			{
				mask->getPixelRGB(x,y,&tem);
			}
			
			if( ( bUsefilemask && mask2 && !mask2->isValid(x,y) ) || 
				tem.r < 127 )
			{
				bgr.r = ((bgr.r + 250) / 2);
				bgr.g = ((bgr.g + 175) / 2);
				bgr.b = ((bgr.b + 175) / 2);
			}
			
			bufferedImage->setPixelRGB(x,y, bgr);
			
		}
	} 
	
	if( mask || mask2 )
	{
		Painter painter(*bufferedImage);
		painter.setColor(Painter::white);
		painter.markCrosshair(centerX,centerY,5); 
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
	
	m_memDc.SetViewportOrg(0, 0);
	
	result = dc.BitBlt(0, 0, width, height, &m_memDc, 0, 0, SRCCOPY);
	ASSERT(result);
	
	m_memDc.SelectObject(pOldObject);  
	
}

LRESULT CImageMaskDlg::setImage(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 1 )
	{
		ShowImage(*mask);
	}	
	else
		ShowImage(*image);
	
	return 0;
}

void CImageMaskDlg::OnFilesave() 
{
	if (! mask || capturing) 
	{
		MessageBox("Have to create a mask first.");
		return;
	}
	
	if (filename == "") 
	{
		OnFilesaveas();
	}
	else 
	{
		
		bWriteConf = true;
		CString sTmp = filename.c_str();
		writeMask(filename);
		m_StatusBar->SetText("Saved mask in " + sTmp,0,0);
	}	
}

void CImageMaskDlg::OnFilesaveas() 
{
	if (! mask || capturing ) 
	{
		MessageBox("Have to create a mask first.");
		return;
	}	
	CString filepath;
	CString filter_str = "Protable Pixmap (*.ppm)|*.ppm|All files(*.*)|*.*||";
	
	CFileDialog filedlg(FALSE,"ppm",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter_str, NULL );
	filedlg.m_ofn.lpstrInitialDir=".//";
	
	if( filedlg.DoModal() == IDOK )
	{
		filepath  = filedlg.GetPathName();
	}
	filename = filepath.GetBuffer(0);
    
	bWriteConf = true;
	configFileName  = "image_processing.cfg";
	
	CString sTmp = filename.c_str();
	writeMask(filename);
	m_StatusBar->SetText("Saved mask in " + sTmp,0,0);
	
}

void CImageMaskDlg::writeMask(string filename)
{
	ImageIO* imgIO = new PPMIO();
	try 
	{
		imgIO->write(*mask, filename);
	} 
	catch (RobotsException& e) 
	{
		MessageBox("Could not write image to file:\n%s", e.what());
	}
	delete imgIO;
}

void CImageMaskDlg::OnWriteconfig() 
{
    ofstream conf( configFileName.c_str() );    
    if (! conf.is_open() )  
	{
		MessageBox((string(__FILE__)+string(": Could not open file ")+configFileName).c_str());
		return;
    }
	
    conf <<  "#DEFINE_BEGIN"<<endl; 
	conf << "ScanLine::robot_mask_file  =  config_files\\image_mask.ppm" << endl;
	conf << "ScanLine::image_center_x   =  "  << centerX << endl;
	conf << "ScanLine::image_center_y   =  "  << centerY << endl;
    conf <<  "#DEFINE_END"<<"\n";
	
    conf.close();	
    CString sTmp = configFileName.c_str();
	m_StatusBar->SetText("Saved image_processing config file in " + sTmp,0,0);
}

void CImageMaskDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (pScrollBar == (CScrollBar*)&m_thresholdSlider)
	{
		const ULONG value = (ULONG) m_thresholdSlider.GetPos();
		CString sTmp;
		sTmp.Format("%d",value);
		m_threshold.SetWindowText(sTmp);
		if (nSBCode == SB_ENDSCROLL )
		{
			thresholdChanged(value);
		}
		
	}	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageMaskDlg::OnUpdateFilesave(CCmdUI* pCmdUI) 
{
	if( bSave == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);	
}

void CImageMaskDlg::OnUpdateFilesaveas(CCmdUI* pCmdUI) 
{
	if( bSaveAs == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);		
}

void CImageMaskDlg::OnUpdateWriteconfig(CCmdUI* pCmdUI) 
{
	if( bWriteConf == true )
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);	
} 

void CImageMaskDlg::OnInitMenuPopup(CMenu*   pPopupMenu,   UINT   nIndex,   BOOL bSysMenu)     
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

void CImageMaskDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    if (m_StatusBar)
	{	
		m_StatusBar->DestroyWindow();
		m_StatusBar = NULL;
	}
	
	if (robot)			delete robot;
	if (image)          delete image;
	if (bufferedImage)  delete bufferedImage; 
    if (m_pMemDcBitmap) delete m_pMemDcBitmap; 
    if (buffer)         delete [] buffer;  
    if (producer)	    delete producer;
	
	if (whiteClassifier) delete whiteClassifier;
    if (blackClassifier) delete blackClassifier;
    if (regionDetector)	delete regionDetector;
    if (distanceFilter) delete distanceFilter;
	if (mask)			delete mask;
	if (mask2)			delete mask2;
	
}

void CImageMaskDlg::OnDrawCircle() 
{
	KillTimer(0);
    
	GetDlgItem(IDC_CREATMASK)->EnableWindow(TRUE);
	
	drawcircle = true;
    DrawCircle(pointleft, pointright);
    DrawRect(pointleft);
	DrawRect(pointright);
	MarkCrosshair(center);
}

void CImageMaskDlg::DrawCircle(CPoint point1, CPoint point2)
{
	CClientDC dc(this);
	dc.SetROP2(R2_XORPEN); // R2_NOT
	dc.SelectStockObject(NULL_BRUSH);
    CPen yellowpen(PS_SOLID,1,RGB(255,255,0));
    dc.SelectObject(&yellowpen);
	
	int dis=(int)sqrt( ( point1.x-point2.x ) * ( point1.x-point2.x ) +
		( point1.y-point2.y ) * ( point1.y-point2.y ) );
	
	int x1 = ( point1.x+point2.x ) / 2 - dis / 2;
	int x2 = ( point1.x+point2.x ) / 2 + dis / 2;
	int y1 = ( point1.y+point2.y ) / 2 - dis / 2;
	int y2 = ( point1.y+point2.y ) / 2 + dis / 2;
	dc.Ellipse( x1, y1, x2, y2 );
	// 
}

void CImageMaskDlg::DrawRect( CPoint point, int size )
{
   	CClientDC dc(this);
	dc.SetROP2(R2_XORPEN);
	dc.SelectStockObject(NULL_BRUSH);
	CPen redpen(PS_SOLID,1,RGB(255,255,255));
	dc.SelectObject(&redpen);
    
	CRect rect;
	
	rect.left	= point.x-size;
	rect.top	= point.y-size;
	rect.right	= point.x+size;
	rect.bottom	= point.y+size;
	dc.Rectangle(rect);
	
}

bool CImageMaskDlg::isinarea(CPoint point, int xleft, int xright, int ytop, int ybottom)
{
	if (point.x < xright && point.x > xleft &&
		point.y < ybottom && point.y > ytop )
		return true;
	else
		return false;
}

void CImageMaskDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if( drawcircle == true ) 
	{
		if( isinarea( point,pointleft.x-5, pointleft.x+5, 
			pointleft.y-5, pointleft.y+5 ) )
			m_bIsDragingleft  = true; 
		if( isinarea( point,pointright.x-5, pointright.x+5, 
			pointright.y-5, pointright.y+5 ))
			m_bIsDragingright = true;
	}	
	CDialog::OnLButtonDown(nFlags, point);
}

void CImageMaskDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bIsDragingleft || m_bIsDragingright ) 
	{
		DrawCircle( pointleft, pointright ); // erase old circle
		
		if( m_bIsDragingleft )
		{
			DrawRect(pointleft);  // erase old point
			pointleft = point;
			DrawRect(pointleft); // draw new point
		}
		if( m_bIsDragingright )
		{
			DrawRect(pointright);  
			pointright = point;
			DrawRect(pointright);
		}
		DrawCircle( pointleft, pointright ); // draw new circle
        
		MarkCrosshair(center);
		center = CPoint( ( pointleft.x + pointright.x )*0.5,( pointleft.y + pointright.y )*0.5 );
		MarkCrosshair(center);
		
		centerX = center.x;
		centerY = center.y;
		radius = static_cast<int>( sqrt( (pointleft.x-pointright.x)*(pointleft.x-pointright.x) + 
			(pointleft.y-pointright.y)*(pointleft.y-pointright.y) ) * 0.5 );
		CString sTmp;
        sTmp.Format("Current mouse pos is  %d ,  %d ", point.x, point.y );
        m_StatusBar->SetText(sTmp,0,0);
	}	
	CDialog::OnMouseMove(nFlags, point);
}

void CImageMaskDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( drawcircle == true && ( m_bIsDragingleft || m_bIsDragingright ) ) 
	{
        m_bIsDragingleft  = false; 
		m_bIsDragingright =	false;
		
		CString sTmp;
        sTmp.Format("New center calculated:  %d ,  %d ; radius is %d", center.x, center.y, radius );
        m_StatusBar->SetText(sTmp,0,0);
	}	
	CDialog::OnLButtonUp(nFlags, point);
}

void CImageMaskDlg::MarkCrosshair(CPoint p, int size)
{
	CClientDC dc(this);
	dc.SetROP2(R2_XORPEN);
	dc.SelectStockObject(NULL_BRUSH);
	CPen whitepen(PS_SOLID,1,RGB(255,255,255));
	dc.SelectObject(&whitepen);
	
	dc.MoveTo(p.x-size,p.y);
	dc.LineTo(p.x+size,p.y);
	dc.MoveTo(p.x,p.y-size);
	dc.LineTo(p.x,p.y+size);
}

void CImageMaskDlg::CreatMask()
{
	//Éú³ÉÑÚÄ£
	const int w = image->getWidth();
	const int h = image->getHeight();
	mask = new RGBImage(w,h);
	RGBTuple white = { 255, 255, 255 };
    RGBTuple black = { 0, 0, 0 };
	
	int squared_radius = radius * radius;
    int tempdis;
	for( int x=0; x < w; x++ ) 
	{
		for (int y=0; y < h; y++) 
		{           
			tempdis = ( x - center.x ) * ( x - center.x ) + 
				( y - center.y ) * ( y - center.y );
			
			if( tempdis <= squared_radius ) 
				mask->setPixelRGB(x, y, white);
			else  
				mask->setPixelRGB(x, y, black);
		}
	}    
	mask->setBlackBorder();
	
}

void CImageMaskDlg::OnCreatmask() 
{	
	//
    CreatMask();
	ShowImage(*mask);
	bSave   = true;    
	bSaveAs = true;
    GetDlgItem(IDC_USE_MASK)->EnableWindow(TRUE);
	
}

void CImageMaskDlg::OnUseMask() 
{
   	bUsemask     = !bUsemask;
	drawcircle   = false;
	bUsefilemask = false; 
	SetTimer(0,50,NULL);
}

void CImageMaskDlg::OnQuickLoad() 
{
	string robotMaskFilename ="";
    string conf3 = "config_files\\image_center.cfg";
	ConfigReader vr3(conf3);
	if( vr3.get("ScanLine::robot_mask_file", robotMaskFilename) < 0 ) 
	{
		MessageBox("ScanLine::robot_mask_filename");
    }   
    if (vr3.get("ScanLine::image_center_x", centerX) <= 0 ) 
	{
		MessageBox("ScanLine::image_center_x");
    }   
    if (vr3.get("ScanLine::image_center_y", centerY) <= 0 ) 
	{
		MessageBox("ScanLine::image_center_y");
    }   
	
	if (robotMaskFilename != "" )
	{
		RobotMask::load(robotMaskFilename);
    }
    else 
	{
		MessageBox("No robot mask specified!");
    }
	
	mask2 = RobotMask::getMask();
	bUsefilemask = true;
	
	CString sTmp;
	sTmp.Format( "Loaded config and mask from file: config_files\\image_center.cfg " 
		" center is  %d ,  %d ", centerX, centerY );
	m_StatusBar->SetText(sTmp,0,0);
}
