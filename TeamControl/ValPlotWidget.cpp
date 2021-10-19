// ValPlotWidget.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "ValPlotWidget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValPlotWidget

CValPlotWidget::CValPlotWidget(int w, int h) : width(w), height(h)
{
	m_pMemDcBitmap = NULL;
	max_val = 1;
	min_val = 0;
	length  = 10;
}

CValPlotWidget::~CValPlotWidget()
{	
	if (m_pMemDcBitmap) 
	{
		delete m_pMemDcBitmap;
		m_pMemDcBitmap = NULL;
	} 
}


BEGIN_MESSAGE_MAP(CValPlotWidget, CWnd)
	//{{AFX_MSG_MAP(CValPlotWidget)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CValPlotWidget message handlers

int CValPlotWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_memDc.CreateCompatibleDC( GetDC() );
	m_pMemDcBitmap = new CBitmap();
	m_pMemDcBitmap->CreateCompatibleBitmap( GetDC(), width, height);
	m_clientRect = CRect(0,0,width,height);
	
	return 0;
}

void CValPlotWidget::OnPaint() 
{
	CPaintDC dc(this); 
	
	m_memDc.SelectStockObject(NULL_BRUSH);
	CBitmap*			pOldObject = NULL;
	pOldObject = m_memDc.SelectObject(m_pMemDcBitmap);
    
	m_memDc.FillSolidRect(m_clientRect,RGB(30,178,245));
    
	CPen blackpen(PS_SOLID,1,RGB(0,0,0));
    m_memDc.SelectObject(&blackpen);
	float range = max_val - min_val;
    scale = CPoint( width/length, (height-1)/100.0 );
	
	CString str;
	str.Format("%.f",min_val);
	CPen pen1(PS_SOLID,1,RGB(0,255,0));
    m_memDc.SelectObject(&pen1);
	m_memDc.TextOut( 0, 100*scale.y, str );
    m_memDc.MoveTo(  0, 100*scale.y );
	m_memDc.LineTo( values.size()*scale.x, 100*scale.y );
	
	CPen pen2(PS_SOLID,1,RGB(255,0,0));
    m_memDc.SelectObject(&pen2);
	str.Format("%.f",max_val);
	m_memDc.TextOut( 0, 10*scale.y, str );
    m_memDc.MoveTo(  0,  0 );
	m_memDc.LineTo( values.size()*scale.x, 0 );
    
	CPen pen3(PS_SOLID,1,RGB(0,0,0));
    m_memDc.SelectObject(&pen3);
    
	if (min_val <= 0 && max_val >= 0) 
	{
		CPen pen4(PS_SOLID,1,RGB(0,255,0));
        m_memDc.SelectObject(&pen4);
		
		m_memDc.MoveTo(  0,  (int) (100.0 - (( (-min_val) / range) * 100.0 ) *scale.y ) );
		m_memDc.LineTo(  values.size()*scale.x, (int) (100.0 - (( (- min_val) / range) * 100.0 )*scale.y ) );
		
		m_memDc.SelectObject(&pen3);
	} 
    
	for (int i=0; i<((int) values.size()); i++)
    {
		//int scalval=(int) (100.0 - ((values[i] / max_val) * 100.0));
		int scalval = (int) (100.0 - (( (values[i] - min_val) / range) * 100.0 ));
		scalval *= scale.y;
		m_memDc.SetPixel(i,scalval,RGB(0,0,0));
    }
	
	m_memDc.SetViewportOrg(0, 0);
    int bresult;
	bresult = dc.BitBlt(2, 0, width, height, &m_memDc, 0, 0, SRCCOPY);
	ASSERT(bresult);
	
	m_memDc.SelectObject(pOldObject);
}

void CValPlotWidget::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CValPlotWidget::init_widget(float _max_val, float _length)
{
	max_val = _max_val;
	length  = _length;
	min_val = 0;
}

void CValPlotWidget::init_widget(float _min_val, float _max_val, float _length)
{
	max_val = _max_val;
	min_val = _min_val;
	length  = _length;
}

void  CValPlotWidget::push(float val)
{
	if (values.size() >= length)
		values.erase(values.begin());
	values.push_back(val);
}