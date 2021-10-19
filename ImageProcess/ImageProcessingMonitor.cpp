// ImageProcessingMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessingMonitor.h"
#include "ImageProcessDlg.h"

#include "RGBImage.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingMonitor

CImageProcessingMonitor::CImageProcessingMonitor(const ConfigReader& config,
												  Image* img ):scanLines(0),scanResults(0)
{
	bufferedImage = NULL; 
    buffer = NULL; 
 	
    image  = img;
 	colClass = new ColorClassInfoList(config);

	m_bitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biPlanes			= 1;
	m_bitmapInfo.bmiHeader.biCompression	= BI_RGB;
	m_bitmapInfo.bmiHeader.biSizeImage		= 0;
	m_bitmapInfo.bmiHeader.biXPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biYPelsPerMeter	= 72;
	m_bitmapInfo.bmiHeader.biClrUsed		= 0;
	m_bitmapInfo.bmiHeader.biClrImportant	= 0;
}

CImageProcessingMonitor::~CImageProcessingMonitor()
{
	wglMakeCurrent(NULL, NULL) ; 
	wglDeleteContext(hglrc);	//删除渲染描述表
	::ReleaseDC (m_hWnd, hdc) ; //释放设备描述表
    
	// if(image)       delete image;
	if(bufferedImage)  delete bufferedImage; 
	 
	if(buffer)        delete [] buffer;
	delete			colClass;
	delete          m_pMemDcBitmap;
}


BEGIN_MESSAGE_MAP(CImageProcessingMonitor, CWnd)
	//{{AFX_MSG_MAP(CImageProcessingMonitor)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageProcessingMonitor message handlers
int CImageProcessingMonitor::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER,                 // 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    24,                               // 24 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 
	
	int  iPixelFormat; 
 
 	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
 	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}

int CImageProcessingMonitor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
 	MySetPixelFormat(::GetDC(m_hWnd));

 	hdc = ::GetDC(m_hWnd);
 	hglrc = wglCreateContext(hdc);
 	wglMakeCurrent(hdc, hglrc);
	
    m_memDc.CreateCompatibleDC(GetDC());
	m_pMemDcBitmap = new CBitmap();
	m_pMemDcBitmap->CreateCompatibleBitmap(GetDC(), 640, 480);
    
    width  = 640;
	height = 480;
 
	bufferedImage = new RGBImage(640,480);
    buffer = new BYTE[ 640 * 480 * 3 ];
 
 	GLfloat fPointsize[2];
    glGetFloatv (GL_POINT_SIZE_RANGE,fPointsize);

 	psize[0]=fPointsize[0]*6;
    psize[1]=fPointsize[0]*18;
		
    GLfloat fLinewide[3];
    glGetFloatv (GL_LINE_WIDTH_RANGE,fLinewide);  

 	lwidth[0]=fLinewide[0]*0.01;
    lwidth[1]=fLinewide[0]*4;
    lwidth[2]=fLinewide[0]*8;
    lwidth[3]=fLinewide[0]*10;
	GLfloat Light_position[] = {0.0,-10.0,0.0,1.0	};
    glLightfv(GL_FRONT,GL_POSITION,Light_position);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();             
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
    
	glOrtho( 0., 639., 0., 479., -1., 1. );
	gluPerspective( 50., 640. / 480. , 1., 10. );
 	glViewport(0, 0, 1280, 800 );                          
    glGetIntegerv( GL_VIEWPORT, vp );
	
	return 0;
}

void CImageProcessingMonitor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
 	ShowImageWithDc(dc);
	
//  static BOOL bBusy = FALSE;
// 	if (bBusy) { return;}
// 	bBusy = TRUE;
/* 
  	glClearColor(1.0f,1.0f,1.0f,0.0f); 	                 
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 
   	glPushMatrix();
  	ShowImage();  
    //  DisplayAllObject();
    SetVisualizeScanResult();
    glPopMatrix();
  	SwapBuffers(hdc);*/
 // bBusy = FALSE; 
 
}

void CImageProcessingMonitor::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if(cy > 0)
    { 
        my_oldRect.right  = cx;
        my_oldRect.bottom = cy;
	}		
}

void CImageProcessingMonitor::setImage(  Image* image )
{
	this->image = image;
}
 
void CImageProcessingMonitor::setScanLines(  ScanLines *scanLines)
{
    this->scanLines = scanLines;    
}

void CImageProcessingMonitor::setScanResults(  ScanResultList *scanResults)
{
    this->scanResults = scanResults; 
}

void CImageProcessingMonitor::setColorId(int id)
{
    colorID = id;
}
  
void CImageProcessingMonitor::ShowImageWithDc(CPaintDC& dc)
{
    RGBTuple rgb = { 0, 0, 0 };
	RGBTuple bgr = { 0, 0, 0 };

//	CClientDC dc(this);
//	m_memDc.SetROP2(R2_NOT);
	m_memDc.SelectStockObject(NULL_BRUSH);

	for (int x=0; x < width; x++) 
	{
      for (int y=0; y < height; y++) 
	  {
 		  image->getPixelRGB(x,y, &rgb);
          bgr.r = rgb.b;
		  bgr.g = rgb.g;
		  bgr.b = rgb.r;
  		  bufferedImage->setPixelRGB(x,y, bgr);
	  }
	} 
	
 	unsigned char* buffer =  bufferedImage->getImageBuffer().buffer;

	if( !buffer ) 
		return;		
 
    m_bitmapInfo.bmiHeader.biWidth	  = width;
	m_bitmapInfo.bmiHeader.biHeight	  = 0 - height;
	m_bitmapInfo.bmiHeader.biBitCount =  24;
    CBitmap*			pOldObject = NULL;

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
 	
	if( scanLines != 0 ) 
	{ 
  		  CPen whitepen(PS_SOLID,1,RGB(255,255,255));
          m_memDc.SelectObject(&whitepen);
		  for (unsigned int i=0; i < scanLines->scanLines.size(); i++) 
		  {
			Vec start = scanLines->scanLines[i].getStart();
			Vec end   = scanLines->scanLines[i].getEnd();
			m_memDc.MoveTo(CPoint(start.x,start.y));
	        m_memDc.LineTo(CPoint(end.x,end.y));
		  }
    }
    if (scanResults != 0) 
	{
		  for (unsigned int c=0; c < colClass->classList.size(); c++) 
		  {
             if( ( colorID != -1 ) && ( colorID != 0 ) && c != colorID ) 
			 {
				 continue;
             }
			 // draw transitions
			 for( unsigned int i=0; i < scanResults->results[c]->transitions.size();
				  i++ ) 
				{ 
				  const Transition& trans = scanResults->results[c]->transitions[i];
				  if (trans.type == Transition::START) 
				  {  
					 CPen pen1(PS_SOLID,1,RGB(colClass->classList[c]->color.r,
						                     colClass->classList[c]->color.g,
											 colClass->classList[c]->color.b ));
                     m_memDc.SelectObject(&pen1);
                      				 
					 m_memDc.Ellipse( (int)trans.toPos.x-2,(int)trans.toPos.y-2,
						         (int)trans.toPos.x+2,(int)trans.toPos.y+2 );
				  }
				  else 
				  {
			         CPen pen2(PS_SOLID,1,RGB(colClass->classList[c]->color.r / 2,
						                     colClass->classList[c]->color.g / 2,
											 colClass->classList[c]->color.b / 2 ));
				 
					 m_memDc.SelectObject(&pen2);
                      				 
					 m_memDc.Ellipse( (int)trans.fromPos.x-2,(int)trans.fromPos.y-2,
						         (int)trans.fromPos.x+2,(int)trans.fromPos.y+2 );
				  }
				}  

			 
			 for(   i = 0; 
				    i < scanResults->results[c]->points.size();
				    i++ ) 
				{
 					  const Vec& point = scanResults->results[c]->points[i];
					  
					  m_memDc.SetPixel( (int)(point.x),(int)(point.y),RGB( colClass->classList[c]->color.r,
						                        colClass->classList[c]->color.g,
											    colClass->classList[c]->color.b ));
				} 

		  }
    }

 	m_memDc.SetViewportOrg(0, 0);
    
	result = dc.BitBlt(0, 0, width, height, &m_memDc, 0, 0, SRCCOPY);
	ASSERT(result);

	m_memDc.SelectObject(pOldObject); 
   
}

void CImageProcessingMonitor::OnMouseMove(UINT nFlags, CPoint point) 
{
 	
	CString strMousePos;
	strMousePos.Format("Current mouse position is: X = %d, Y = %d",(int)((point.x)),(int)((point.y)) );		 
	CImageProcessDlg *pmaindlg = (CImageProcessDlg*)(AfxGetApp()->m_pMainWnd);

	pmaindlg->m_StatusBar->SetText(strMousePos,0,0);
	
	CWnd::OnMouseMove(nFlags, point);
	 
}

void CImageProcessingMonitor::SetVisualizeScanResult()
{
 	glTranslatef(0.0f,0.0f,-6.0f);
    glEnable(GL_COLOR_MATERIAL);
   
 	glPushMatrix();
  
	if (scanLines != 0) 
	{ 
 		  glColor3ub(255,255,255);
 		  
		  for (unsigned int i=0; i < scanLines->scanLines.size(); i++) 
		  {
			Vec start = scanLines->scanLines[i].getStart();
			Vec end   = scanLines->scanLines[i].getEnd();
			DrawLine(start,end);
		  }
    }
    if (scanResults != 0) 
	{
		  for (unsigned int c=0; c < colClass->classList.size(); c++) 
		  {
             
			 // draw transitions
			 for( unsigned int i=0; i < scanResults->results[c]->transitions.size();
				  i++ ) 
				{ 
			      
				  const Transition& trans = scanResults->results[c]->transitions[i];
				  if (trans.type == Transition::START) 
				  {  
				     glColor3ub( colClass->classList[c]->color.r,
						        colClass->classList[c]->color.g,
							    colClass->classList[c]->color.b );
 					 DrawCircle( trans.toPos );
				  }
				  else 
				  {
			         glColor3ub( colClass->classList[c]->color.r / 2,
						       colClass->classList[c]->color.g / 2,
							   colClass->classList[c]->color.b / 2 );
				      
					 DrawCircle( trans.fromPos );
				  }
				}  
 			 
			 for(   i = 0; 
				    i < scanResults->results[c]->points.size();
				    i++ ) 
				{
					  glColor3ub( colClass->classList[c]->color.r,
						        colClass->classList[c]->color.g,
							    colClass->classList[c]->color.b );
 					  const Vec& point = scanResults->results[c]->points[i];
					  DrawPoint( point );
				} 

		  }
    }
    
	glPopMatrix();
	glFlush();
	glDisable(GL_COLOR_MATERIAL);
   
}
 

void CImageProcessingMonitor::DrawLine(Vec p1, Vec p2)
{
	Vec p11( p1.x * scaleX, p1.y * scaleY );
	Vec p22( p2.x * scaleX, p2.y * scaleY );
  
	glPushMatrix();

    glLineWidth(0.01f); 
   	glBegin(GL_LINES);  
    
    glVertex2d( p11.x, (vp[3] - p11.y  + vp[1]) );	
 	glVertex2d( p22.x, (vp[3] - p22.y  + vp[1]) );
  	glEnd(); 
	glPopMatrix();
	glFlush();
}


void CImageProcessingMonitor::DrawPoint( Vec p )
{
    Vec p1( p.x * scaleX, p.y * scaleY );

	glPushMatrix();

    glPointSize(1.0f);       
 	glBegin(GL_POINTS);
  	glVertex2d( p1.x , (vp[3] - p1.y  + vp[1]) );   
	glEnd(); 
    
	glPopMatrix();
	glFlush();
}
    
void CImageProcessingMonitor::DrawCircle(Vec p, double r /* = 2  */)
{
    Vec p1( p.x * scaleX, p.y * scaleY );

	glTranslatef( p1.x, (vp[3] - p1.y + vp[1]), 0.0f );

	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
    GLfloat radius = r;  
   	glPointSize(0.001f);
  	gluQuadricDrawStyle(quadObj,GLU_LINE); 
  	gluDisk( quadObj, radius, radius, 360, 1.0 );
    gluDeleteQuadric(quadObj);
    
	glTranslatef( -p1.x, -(vp[3] - p1.y + vp[1]), 0.0f );
  
}
  
void CImageProcessingMonitor::ShowImage()
{
 	RGBTuple rgb = { 0, 0, 0 };
    if (!image)
 	  return;
	
 	for( int x=0; x < width; x++ ) 
	{
      for (int y=0; y < height; y++) 
	  {
	     
		  image->getPixelRGB(x,y, &rgb);
            
 		  bufferedImage->setPixelRGB(x,480-y-1, rgb);
	  }
	} 
	
 	unsigned char* buffer =  bufferedImage->getImageBuffer().buffer;

	if( !buffer ) 
		return;	
  	 
	glDrawPixels(640,480,GL_RGB,GL_UNSIGNED_BYTE,buffer);
   	
}
 
void CImageProcessingMonitor::DisplayAllObject()
{
   	glTranslatef(0.0f,0.0f,-6.0f);
    glEnable(GL_COLOR_MATERIAL);
   
	glPushMatrix();
   	
  	glColor3f(1.0f,0.0f,0.0f);   
   
    DrawLine(Vec(320,0), Vec(400,240));
    
    DrawCircle(Vec(160,300));
	DrawCircle(Vec(160,100));
    DrawPoint(Vec(500,200)); 
  	 
 	glPopMatrix();
  	glFlush(); 
  	
    glDisable(GL_COLOR_MATERIAL);
     
}


