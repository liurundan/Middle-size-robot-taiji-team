// StatusColor.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "StatusColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Param.h"
/////////////////////////////////////////////////////////////////////////////
// StatusColor

StatusColor::StatusColor(int w, int h) : width(w), height(h)
{
	statusColor = COL_RED;
	msg			= "";
}

StatusColor::~StatusColor()
{
	if (dcMem.m_hDC)
	{
		dcMem.SelectObject((CBitmap*)NULL);
		statusfont.DeleteObject();
		bm.DeleteObject();
		dcMem.DeleteDC();
	}
}
 
BEGIN_MESSAGE_MAP(StatusColor, CWnd)
	//{{AFX_MSG_MAP(StatusColor)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// StatusColor message handlers
int StatusColor::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    rc = CRect(0,0,width,height);
    

	dcMem.CreateCompatibleDC(GetDC());
	bm.CreateCompatibleBitmap(GetDC(),rc.Width(),rc.Height());
	dcMem.SelectObject(&bm);
 	int nPointSize = 100;
	if (!statusfont.CreatePointFont(nPointSize, _T("Times New Roman"),&dcMem)) //_T("MS Sans Serif")
	{
		statusfont.CreateStockObject(SYSTEM_FONT);
	}
	dcMem.SelectObject(&statusfont);
	return 0;
}

void StatusColor::OnPaint() 
{
	CPaintDC dc(this);
    dcMem.FillSolidRect(rc,statusColor);
	dcMem.TextOut(2,2,msg);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
}

void StatusColor::SetStatusColor(COLORREF c, CString s) 
{
	msg = s;
	statusColor = c;
	Invalidate();
}

