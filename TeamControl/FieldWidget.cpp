// FieldWidget.cpp : implementation file
//

#include "stdafx.h"
#include "teamcontrol.h"
#include "FieldWidget.h"
#include <cmath>
#include "Param.h"
#include "stringconvert.h"
#include <deque>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FieldWidget

FieldWidget::FieldWidget()
{
	penWhite.CreatePen(PS_SOLID,1,COL_WHITE);
	penWhiteDot2.CreatePen(PS_DOT,2,COL_WHITE);
	penBlack.CreatePen(PS_SOLID,2,COL_BLACK);
	penYellow.CreatePen(PS_SOLID,1,COL_YELLOW);
	penBlue.CreatePen(PS_SOLID,1,COL_BLUE);
	penRed.CreatePen(PS_SOLID,1,COL_RED);
	penRed2.CreatePen(PS_SOLID,2,COL_RED);
	penRedDot.CreatePen(PS_DOT,1,COL_RED);
	penRedDot2.CreatePen(PS_DOT,2,COL_RED);
	penMagenta.CreatePen(PS_SOLID,2,COL_MAGENTA);
	penCyan.CreatePen(PS_SOLID,2,COL_CYAN);
    penDarkCyan.CreatePen(PS_SOLID,2,COL_DARKCYAN);
    penCyanDot.CreatePen(PS_DOT,1,COL_CYAN);
    penDarkRedDot.CreatePen(PS_DOT,1,COL_DARKRED);

	brushBlack.CreateSolidBrush(COL_BLACK);
	brushWhite.CreateSolidBrush(COL_WHITE);
	brushRed.CreateSolidBrush(COL_RED);
	brushCyan.CreateSolidBrush(COL_CYAN);
	brushDarkCyan.CreateSolidBrush(COL_DARKCYAN);
	brushYellow.CreateSolidBrush(COL_YELLOW);

	rc = CRect(0,0,0,0);
}

FieldWidget::~FieldWidget()
{
	penWhite.DeleteObject();
	penWhiteDot2.DeleteObject();
	penBlack.DeleteObject();
	penYellow.DeleteObject();
	penBlue.DeleteObject();
	penRed.DeleteObject();
	penRed2.DeleteObject();
	penRedDot.DeleteObject();
	penRedDot2.DeleteObject();
	penDarkRedDot.DeleteObject();
	
	penCyan.DeleteObject();
	penMagenta.DeleteObject();
	penDarkCyan.DeleteObject();
    penCyanDot.DeleteObject();
	
	brushBlack.DeleteObject();
	brushWhite.DeleteObject();
	brushRed.DeleteObject();
	brushCyan.DeleteObject();
	brushDarkCyan.DeleteObject();
	brushYellow.DeleteObject();
	
	if (dcMem.m_hDC)
	{
		dcMem.SelectObject((CFont*)NULL);
		playerFont.DeleteObject();
		dcMem.SelectObject((CBitmap*)NULL);
		bm.DeleteObject();
		dcMem.DeleteDC();
	}

}


BEGIN_MESSAGE_MAP(FieldWidget, CWnd)
	//{{AFX_MSG_MAP(FieldWidget)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// FieldWidget message handlers
namespace
{
	inline int rd (const double& x) 
	{
		return static_cast<int>(floor(x+0.5));
	} 
}

int FieldWidget::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_parent = GetParent();
/*
	GetClientRect(&rc);
    dcMem.CreateCompatibleDC(GetDC());
	bm.CreateCompatibleBitmap(GetDC(),rc.Width(),rc.Height());
    dcMem.SelectObject(&bm);
	
	dcMem.SelectStockObject(NULL_BRUSH);
	*/
	return 0;
}

void FieldWidget::OnPaint() 
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	if (clientRect.Width() != rc.Width() || clientRect.Height() != rc.Height() ) 
	{
		if (dcMem.m_hDC)
		{
			dcMem.SelectObject((CFont*)NULL);
			playerFont.DeleteObject();
			dcMem.SelectObject((CBitmap*)NULL);
			bm.DeleteObject();
			dcMem.DeleteDC();
		}
		if (!dcMem.CreateCompatibleDC(&dc))
		{
			return;
		}

		if (!bm.CreateCompatibleBitmap(&dc,clientRect.Width(),clientRect.Height())) 
		{
			dcMem.DeleteDC();
		}

		bm.SetBitmapDimension(clientRect.Width(),clientRect.Height());
		dcMem.SelectObject(&bm);

	}
	
	GetClientRect(&rc);

 	paintEvent(dcMem);
	dc.BitBlt(0,0,clientRect.Width(),clientRect.Height(),&dcMem,0,0,SRCCOPY);
}

void FieldWidget::init (const FieldGeometry& fg) 
{
	ppref.show_wm_robot = ppref.show_wm_ball = ppref.show_wm_obstacles = true;
	ppref.show_vis_lines = ppref.show_vis_ball = ppref.show_vis_obstacles = 
	ppref.show_vis_goals = ppref.show_sl_pos = false;
	ppref.show_robot_trace = ppref.show_ball_trace = false;
	ppref.show_robot_ids = ppref.show_robot_ball_links = false;
	ppref.show_aux_lines = true;
	ppref.zoom.own_half = ppref.field_heading = +1;
	ppref.reference_robot = 0;
	ppref.use_exec_time = true;
	
	PaintActionSelect paselect;
	paselect.show_wm_robot=paselect.show_wm_ball=paselect.show_wm_obstacles=true;
	paselect.show_vis_lines=paselect.show_vis_ball=paselect.show_vis_obstacles=paselect.show_vis_goals=true;
	paselect.show_sl_pos=paselect.show_aux_lines=true;
	paselect.show_robot_ids=paselect.show_robot_ball_links=false;
	paselect.show_robot_trace=paselect.show_ball_trace=true;
	paselect.clear_lines=true;
	paselect.zoom_in=paselect.zoom_out=paselect.zoom_all=true;
	paselect.zoom_undo=paselect.zoom_redo=true;
	paselect.flip_sides=paselect.flip_goals=true;
	paselect.use_exec_time=true;
	paselect.next_refrobot=false;
	
	init(fg, ppref, paselect );
}

void FieldWidget::init(const FieldGeometry& fg, const PaintPreferences& pp, const PaintActionSelect& paselect) 
{
	fgeometry = fg;
	ppref = pp;
	
	zoom_all();
	ppref.old_zooms.clear ();
	mouse_mode  =0;
	dock_height = 40; // dock_height=48
}

CPoint FieldWidget::Coordposf(double x, double y)
{
	Vec pos  = mapping_f * Vec(x,y);
	CPoint p = CPoint(rd(pos.x),rd(pos.y));
	return p; 
}

CPoint FieldWidget::Coordposi(double x, double y)
{
	Vec pos  = mapping_i * Vec(x,y);
	CPoint p = CPoint(rd(pos.x),rd(pos.y));
	return p; 
}

void FieldWidget::paintEvent( CDC& paint )
{

	double s  = ppref.zoom.own_half * ppref.zoom.scaling;
	double sf = ppref.field_heading;
	
	mapping_f.set_position( Vec( 0.5*rc.Width()-s*ppref.zoom.image_center.y,
		0.5*rc.Height()-s*ppref.zoom.image_center.x+0.5*dock_height));
	mapping_f.n_x = sf*s;
	mapping_f.n_y = 0;

	mapping_i.set_position( Vec( 0.5*rc.Width()-s*ppref.zoom.image_center.x,
		0.5*rc.Height()-s*ppref.zoom.image_center.y+0.5*dock_height) );
	mapping_i.n_x = s;
	mapping_i.n_y = 0;
	
	paint.FillSolidRect(rc,RGB(8,164,0));
	
	paint.SelectObject(&penWhite);
 	
	paint.MoveTo(	Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width ).x,
					Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width ).y);
	paint.LineTo(	Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.field_width).x,
					Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.field_width).y);
	paint.LineTo(	Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.field_width).x,
					Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.field_width).y);
	paint.LineTo(	Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.field_width).x,
					Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.field_width).y);
	paint.LineTo(	Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width).x,
					Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width).y);

 	if (fgeometry.lineset_type==0)
	{
	  paint.MoveTo( Coordposi(0,-0.5*fgeometry.field_width).x,
		Coordposi(0,-0.5*fgeometry.field_width).y);
	  paint.LineTo( Coordposi(0,0.5*fgeometry.field_width).x,
		Coordposi(0,0.5*fgeometry.field_width).y);
	} 
 	else 
	{
		paint.MoveTo( Coordposi(0,-0.5*fgeometry.field_width).x,
			Coordposi(0,-0.5*fgeometry.field_width).y);
		paint.LineTo( Coordposi(0,-430).x,
			Coordposi(0,-430).y);
		paint.MoveTo( Coordposi(0,0.5*fgeometry.field_width).x,
			Coordposi(0,0.5*fgeometry.field_width).y);
		paint.LineTo( Coordposi(0,430).x,
			Coordposi(0,430).y);
	}
/*    paint.Ellipse( Coordposi(-fgeometry.center_circle_radius, -fgeometry.center_circle_radius).x,
				   Coordposi(-fgeometry.center_circle_radius, -fgeometry.center_circle_radius).y,
				   Coordposi(fgeometry.center_circle_radius, fgeometry.center_circle_radius).x,
				   Coordposi(fgeometry.center_circle_radius, fgeometry.center_circle_radius).y);*/

	if( fgeometry.center_circle_radius>0 && fgeometry.lineset_type==0 )
		paint.Arc( Coordposi(-fgeometry.center_circle_radius,-fgeometry.center_circle_radius ).x,Coordposi(-fgeometry.center_circle_radius,-fgeometry.center_circle_radius ).y,
				   Coordposi(fgeometry.center_circle_radius,fgeometry.center_circle_radius ).x,Coordposi(fgeometry.center_circle_radius,fgeometry.center_circle_radius).y,
                   Coordposi(0,fgeometry.center_circle_radius).x,Coordposi(0,fgeometry.center_circle_radius).y,
                   Coordposi(0,fgeometry.center_circle_radius).x,Coordposi(0,fgeometry.center_circle_radius).y   ); 
 	if (fgeometry.lineset_type==1 || fgeometry.lineset_type==2 ) 
	{
		double dir = (fgeometry.lineset_type==1 ? 1 : -1);

		paint.MoveTo( Coordposi(dir*470,0 ).x,
			Coordposi(dir*470,0).y);
		paint.LineTo( Coordposi(dir*-480,615 ).x,
			Coordposi(dir*-480,615).y);
		paint.LineTo( Coordposi( dir*-480,-615).x,
			Coordposi(dir*-480,-615 ).y);
		paint.LineTo( Coordposi(dir*470,0).x,
			Coordposi(dir*470,0 ).y);

		paint.MoveTo( Coordposi(dir*300,0 ).x,
			Coordposi(dir*300,0 ).y);
		paint.LineTo( Coordposi(dir*-270,0).x,
			Coordposi(dir*-270,0).y);

		paint.MoveTo( Coordposi(dir*-270,325 ).x,
			Coordposi(dir*-270,325 ).y);
		paint.LineTo( Coordposi(dir*-270,-325).x,
			Coordposi(dir*-270,-325).y);
	} 

	if (fgeometry.corner_arc_radius>0) 
	{   
		paint.Arc(Coordposi(-0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y,
			      Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y, 
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width).x,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width).y  );
	
		paint.Arc(Coordposi(-0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width).x,
			      Coordposi(-0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width).y,
			      Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y); 

		paint.Arc(Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y,
			      Coordposi(0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length+fgeometry.corner_arc_radius,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width).x,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,-0.5*fgeometry.field_width).y, 
			      Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y);

		paint.Arc(Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y,
			      Coordposi(0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width+fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length+fgeometry.corner_arc_radius,0.5*fgeometry.field_width+fgeometry.corner_arc_radius).y,
			      Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).x,
			      Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.field_width-fgeometry.corner_arc_radius).y,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width).x,
			      Coordposi(0.5*fgeometry.field_length-fgeometry.corner_arc_radius,0.5*fgeometry.field_width).y);
	}

	if (fgeometry.penalty_area_length>0 && fgeometry.penalty_area_width>0) 
	{
		paint.MoveTo( Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length, -0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_area_length,-0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_area_length,-0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_area_length,0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_area_length,0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.penalty_area_width ).y);

		paint.MoveTo( Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_area_length,-0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_area_length,-0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_area_length,0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_area_length,0.5*fgeometry.penalty_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.penalty_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.penalty_area_width ).y);
	}

	if (fgeometry.goal_area_length>0 && fgeometry.goal_area_width>0) 
	{
		paint.MoveTo( Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.goal_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.goal_area_width ).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length+fgeometry.goal_area_length,-0.5*fgeometry.goal_area_width ).x,
					  Coordposi(-0.5*fgeometry.field_length+fgeometry.goal_area_length,-0.5*fgeometry.goal_area_width ).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length+fgeometry.goal_area_length,0.5*fgeometry.goal_area_width ).x,
					  Coordposi( -0.5*fgeometry.field_length+fgeometry.goal_area_length,0.5*fgeometry.goal_area_width).y);
		paint.LineTo( Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.goal_area_width ).x,
					  Coordposi( -0.5*fgeometry.field_length,0.5*fgeometry.goal_area_width).y);
		
		paint.MoveTo( Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.goal_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.goal_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length-fgeometry.goal_area_length,-0.5*fgeometry.goal_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length-fgeometry.goal_area_length,-0.5*fgeometry.goal_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length-fgeometry.goal_area_length,0.5*fgeometry.goal_area_width ).x,
					  Coordposi(0.5*fgeometry.field_length-fgeometry.goal_area_length,0.5*fgeometry.goal_area_width ).y);
		paint.LineTo( Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.goal_area_width).x,
					  Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.goal_area_width ).y);
	}
 
	int m_radius = 50;
	paint.SelectObject(&brushWhite);
	
    paint.Ellipse( Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_marker_distance-m_radius,0-m_radius).x,
				   Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_marker_distance-m_radius,0-m_radius).y,
				   Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_marker_distance+m_radius,0+m_radius).x,
				   Coordposi(-0.5*fgeometry.field_length+fgeometry.penalty_marker_distance+m_radius,0+m_radius).y);
    paint.Ellipse( Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_marker_distance-m_radius,0-m_radius).x,
				   Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_marker_distance-m_radius,0-m_radius).y,
				   Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_marker_distance+m_radius,0+m_radius).x,
				   Coordposi(0.5*fgeometry.field_length-fgeometry.penalty_marker_distance+m_radius,0+m_radius).y);

     
	paint.SelectObject(&penYellow);

	paint.MoveTo(	Coordposi( -0.5*fgeometry.field_length,-0.5*fgeometry.goal_width).x,
					Coordposi(-0.5*fgeometry.field_length,-0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(-0.5*fgeometry.field_length-fgeometry.goal_length,-0.5*fgeometry.goal_width ).x,
					Coordposi(-0.5*fgeometry.field_length-fgeometry.goal_length,-0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(-0.5*fgeometry.field_length-fgeometry.goal_length,0.5*fgeometry.goal_width ).x,
					Coordposi(-0.5*fgeometry.field_length-fgeometry.goal_length,0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.goal_width ).x,
					Coordposi(-0.5*fgeometry.field_length,0.5*fgeometry.goal_width ).y);

	paint.SelectObject(&penBlue);
	
	paint.MoveTo(	Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.goal_width ).x,
					Coordposi(0.5*fgeometry.field_length,-0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(0.5*fgeometry.field_length+fgeometry.goal_length,-0.5*fgeometry.goal_width ).x,
					Coordposi(0.5*fgeometry.field_length+fgeometry.goal_length,-0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(0.5*fgeometry.field_length+fgeometry.goal_length,0.5*fgeometry.goal_width ).x,
					Coordposi(0.5*fgeometry.field_length+fgeometry.goal_length,0.5*fgeometry.goal_width ).y);
	paint.LineTo(	Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.goal_width ).x,
					Coordposi(0.5*fgeometry.field_length,0.5*fgeometry.goal_width ).y);
	
    vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
    vector<BallLocation>& bloc  = (ppref.use_exec_time ? cinfo.bloc_exec : cinfo.bloc_vis);
 	
	if (ppref.show_wm_robot) 
	{
		for (unsigned int i=0; i<rloc.size(); i++) 
		{
			CBrush br;
			if (rloc[i].stuck.robot_stuck)
			{
				paint.SelectObject(&penDarkCyan);
				br.CreateSolidBrush(COL_DARKCYAN);
			}
			else
			{
				paint.SelectObject(&penCyan);
				br.CreateSolidBrush(COL_CYAN);
			}
			
			Vec vertices [] = { Vec (156,175 ), Vec (74,223), Vec (-230,48), 
				Vec (-230,-48), Vec (74,-223), Vec ( 156,-175), Vec (230,-140), 
				Vec (156,-140), Vec (156,140), Vec (230,140), Vec (200,-100), 
				Vec (200,100) };
			for (unsigned int j=0; j<12; j++) 
				vertices[j] = vertices[j].rotate(rloc[i].heading)+rloc[i].pos;
			
			CPoint rob[6];
			for (unsigned int k=0; k<6; k++)
				rob[k] = Coordposi(vertices[k].x, vertices[k].y);
			
			CRgn argn;
			argn.CreatePolygonRgn(rob,6,1);
			paint.FillRgn(&argn,&br);
            br.DeleteObject();
			
			paint.MoveTo( Coordposi(vertices[6].x, vertices[6].y).x,
				Coordposi(vertices[6].x, vertices[6].y).y);
			paint.LineTo( Coordposi(vertices[7].x, vertices[7].y).x,
				Coordposi(vertices[7].x, vertices[7].y).y );
			paint.MoveTo( Coordposi(vertices[8].x, vertices[8].y).x,
				Coordposi(vertices[8].x, vertices[8].y).y);
			paint.LineTo( Coordposi(vertices[9].x, vertices[9].y).x,
				Coordposi(vertices[9].x, vertices[9].y).y );
		    
			if(rloc[i].kick) 
			{
				paint.MoveTo( Coordposi(vertices[10].x, vertices[10].y).x,
					Coordposi(vertices[10].x, vertices[10].y).y);
				paint.LineTo( Coordposi(vertices[11].x, vertices[11].y).x,
					Coordposi(vertices[11].x, vertices[11].y).y );
			}
			paint.MoveTo( Coordposi(rloc[i].pos.x, rloc[i].pos.y).x,
				Coordposi(rloc[i].pos.x, rloc[i].pos.y).y);
			paint.LineTo( Coordposi(rloc[i].pos.x+500*rloc[i].vtrans.x, rloc[i].pos.y+500*rloc[i].vtrans.y).x,
				Coordposi(rloc[i].pos.x+500*rloc[i].vtrans.x, rloc[i].pos.y+500*rloc[i].vtrans.y).y );

		}
	}
	
	if (ppref.show_robot_ids) 
	{
  		for (unsigned int i=0; i<robot_ids.size() && i<rloc.size(); i++) 
		{
			CString s;
			if( i == ppref.reference_robot ) 
	            s.Format("%s+",robot_ids[i].c_str());
			else
	            s.Format("%s",robot_ids[i].c_str());
			Vec p = field2widget (rloc[i].pos);
		    paint.TextOut(p.x,p.y,s);
		} 
	}  

	if (ppref.show_robot_trace) 
	{
		for (unsigned int i=0; i+1<ppref.rtrace.size(); i++) 
		{
			paint.SelectObject(&penCyan);
			 
			paint.MoveTo( Coordposi(ppref.rtrace[i].pos.x-10, ppref.rtrace[i].pos.y-10).x,
				Coordposi(ppref.rtrace[i].pos.x-10, ppref.rtrace[i].pos.y-10).y);
			paint.LineTo( Coordposi(ppref.rtrace[i].pos.x+10, ppref.rtrace[i].pos.y+10).x,
				Coordposi(ppref.rtrace[i].pos.x+10, ppref.rtrace[i].pos.y+10).y );
			paint.MoveTo( Coordposi(ppref.rtrace[i].pos.x-10, ppref.rtrace[i].pos.y+10).x,
				Coordposi(ppref.rtrace[i].pos.x-10, ppref.rtrace[i].pos.y+10).y);
			paint.LineTo( Coordposi(ppref.rtrace[i].pos.x+10, ppref.rtrace[i].pos.y-10).x,
				Coordposi(ppref.rtrace[i].pos.x+10, ppref.rtrace[i].pos.y-10).y );
            
 			paint.SelectObject(&penCyanDot);
			paint.MoveTo( Coordposi(ppref.rtrace[i].pos.x, ppref.rtrace[i].pos.y).x,
				Coordposi(ppref.rtrace[i].pos.x, ppref.rtrace[i].pos.y).y);
			paint.LineTo( Coordposi(ppref.rtrace[i+1].pos.x, ppref.rtrace[i+1].pos.y).x,
				Coordposi(ppref.rtrace[i+1].pos.x, ppref.rtrace[i+1].pos.y).y );

		}
	}
	
	if (ppref.show_wm_ball) 
	{
		for (unsigned int i=0; i<bloc.size(); i++) 
		{
			if (bloc[i].pos_known == BallLocation::known)
				paint.SelectObject(&penRed2);
			else if (bloc[i].pos_known == BallLocation::unknown)
				paint.SelectObject(&penRedDot);
			else if (bloc[i].pos_known == BallLocation::raised)
				paint.SelectObject(&penRed);
			else if (bloc[i].pos_known == BallLocation::communicated || 
				     bloc[i].pos_known == BallLocation::small_camera )   // lrd edit
				paint.SelectObject(&penRedDot2);
			else
				paint.SelectObject(&penRedDot);
			
			paint.Arc(  Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).x,
						Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).y,
						Coordposi(bloc[i].pos.x+0.5*fgeometry.ball_diameter, bloc[i].pos.y+0.5*fgeometry.ball_diameter).x,
						Coordposi(bloc[i].pos.x+0.5*fgeometry.ball_diameter, bloc[i].pos.y+0.5*fgeometry.ball_diameter).y,
						Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).x,
						Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).y,
						Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).x,
						Coordposi(bloc[i].pos.x-0.5*fgeometry.ball_diameter, bloc[i].pos.y-0.5*fgeometry.ball_diameter).y);
			paint.MoveTo( Coordposi(bloc[i].pos.x, bloc[i].pos.y).x,
				Coordposi(bloc[i].pos.x, bloc[i].pos.y).y);
			paint.LineTo( Coordposi(bloc[i].pos.x+500*bloc[i].velocity.x, bloc[i].pos.y+500*bloc[i].velocity.y).x,
				Coordposi(bloc[i].pos.x+500*bloc[i].velocity.x, bloc[i].pos.y+500*bloc[i].velocity.y).y );
			
		}
	}
    
	if (ppref.show_robot_ball_links) 
	{
		paint.SelectObject(&penDarkRedDot);
		for (unsigned int i=0; i<bloc.size() && i<rloc.size(); i++) 
		{
			paint.MoveTo( Coordposi(rloc[i].pos.x, rloc[i].pos.y).x,
				Coordposi(rloc[i].pos.x, rloc[i].pos.y).y);
			paint.LineTo( Coordposi(bloc[i].pos.x, bloc[i].pos.y).x,
				Coordposi(bloc[i].pos.x, bloc[i].pos.y).y );
		}
	}

	if (ppref.show_ball_trace)
	{
		for (unsigned int i=0; i+1<ppref.btrace.size(); i++) 
		{
		    paint.SelectObject(&penRed);

			paint.MoveTo( Coordposi(ppref.btrace[i].pos.x-10, ppref.btrace[i].pos.y-10).x,
				Coordposi(ppref.btrace[i].pos.x-10, ppref.btrace[i].pos.y-10).y);
			paint.LineTo( Coordposi(ppref.btrace[i].pos.x+10, ppref.btrace[i].pos.y+10).x,
				Coordposi(ppref.btrace[i].pos.x+10, ppref.btrace[i].pos.y+10).y );
			paint.MoveTo( Coordposi(ppref.btrace[i].pos.x-10, ppref.btrace[i].pos.y+10).x,
				Coordposi(ppref.btrace[i].pos.x-10, ppref.btrace[i].pos.y+10).y);
			paint.LineTo( Coordposi(ppref.btrace[i].pos.x+10, ppref.btrace[i].pos.y-10).x,
				Coordposi(ppref.btrace[i].pos.x+10, ppref.btrace[i].pos.y-10).y );

		    paint.SelectObject(&penRedDot);
			paint.MoveTo( Coordposi(ppref.btrace[i].pos.x, ppref.btrace[i].pos.y).x,
				Coordposi(ppref.btrace[i].pos.x, ppref.btrace[i].pos.y).y);
			paint.LineTo( Coordposi(ppref.btrace[i+1].pos.x, ppref.btrace[i+1].pos.y).x,
				Coordposi(ppref.btrace[i+1].pos.x, ppref.btrace[i+1].pos.y).y );

		}
	}
 
	if (ppref.show_wm_obstacles) 
	{
		Vec refpos = rloc.size()>ppref.reference_robot ? rloc[ppref.reference_robot].pos : Vec(0,0);
		paint.SelectObject(&penBlack);
		for (unsigned int i=0; i<cinfo.oloc.pos.size(); i++) 
		{
			Vec midpoint = cinfo.oloc.pos [i];
			double width = cinfo.oloc.width [i];
			Vec robot_obstacle = (midpoint-refpos).normalize();
			Vec left_point = midpoint+0.5*width*robot_obstacle.rotate_quarter();
			Vec right_point = midpoint-0.5*width*robot_obstacle.rotate_quarter();
			Vec back_point = midpoint+100*robot_obstacle;
			paint.MoveTo( Coordposi(left_point.x, left_point.y).x,
				Coordposi(left_point.x, left_point.y).y);
			paint.LineTo( Coordposi(right_point.x, right_point.y).x,
				Coordposi(right_point.x, right_point.y).y );
			paint.MoveTo( Coordposi(midpoint.x, midpoint.y).x,
				Coordposi(midpoint.x, midpoint.y).y);
			paint.LineTo( Coordposi(back_point.x, back_point.y).x,
				Coordposi(back_point.x, back_point.y).y );
			
		}
	}
	
	if (ppref.show_vis_lines || ppref.show_vis_ball || ppref.show_vis_obstacles || ppref.show_vis_goals) 
	{
		Vec refpos = rloc.size() >ppref.reference_robot ? rloc[ppref.reference_robot].pos : Vec(0,0);
		Angle refhead = rloc.size()>ppref.reference_robot ? rloc[ppref.reference_robot].heading : Angle::zero;
		for (unsigned int i=0; i<cinfo.vloc.objectlist.size(); i++) 
		{
			Vec p = cinfo.vloc.objectlist[i].pos.rotate (refhead)+refpos;
			if (cinfo.vloc.objectlist[i].object_type == VisibleObject::ball && ppref.show_vis_ball) 
			{
				CPen penDarkRedDash(PS_DASH,2,COL_DARKRED);
				paint.SelectObject(&penDarkRedDash);
				paint.Arc(  Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).x,
					Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).y,
					Coordposi(p.x+0.5*fgeometry.ball_diameter, p.y+0.5*fgeometry.ball_diameter).x,
					Coordposi(p.x+0.5*fgeometry.ball_diameter, p.y+0.5*fgeometry.ball_diameter).y,
					Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).x,
					Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).y,
					Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).x,
					Coordposi(p.x-0.5*fgeometry.ball_diameter, p.y-0.5*fgeometry.ball_diameter).y);
				penDarkRedDash.DeleteObject();
				
			} 
			else if (cinfo.vloc.objectlist[i].object_type == VisibleObject::white_line && ppref.show_vis_lines) 
			{
				CPen penLightGray(PS_SOLID,2,COL_LIGHTGRAY);
				paint.SelectObject(&penLightGray);
				paint.Arc(  Coordposi(p.x-30, p.y-30).x,
					Coordposi(p.x-30, p.y-30).y,
					Coordposi(p.x+30, p.y+30).x,
					Coordposi(p.x+30, p.y+30).y,
					Coordposi(p.x-30, p.y-30).x,
					Coordposi(p.x-30, p.y-30).y,
					Coordposi(p.x-30, p.y-30).x,
					Coordposi(p.x-30, p.y-30).y);
				penLightGray.DeleteObject();
				
			} 
			else if ((cinfo.vloc.objectlist[i].object_type == VisibleObject::obstacle || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::teammate || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::opponent) && ppref.show_vis_obstacles) 
			{
				CPen penDarkGray(PS_DASH,2,COL_DARKGRAY);
				paint.SelectObject(&penDarkGray);
				double width = cinfo.vloc.objectlist[i].width;
				Vec robot_obstacle = (p-refpos).normalize();
				Vec left_point = p+0.5*width*robot_obstacle.rotate_quarter();
				Vec right_point = p-0.5*width*robot_obstacle.rotate_quarter();
				Vec back_point = p+100*robot_obstacle;
				
				paint.MoveTo( Coordposi(left_point.x, left_point.y).x,
					Coordposi(left_point.x, left_point.y).y);
				paint.LineTo( Coordposi(right_point.x, right_point.y).x,
					Coordposi(right_point.x, right_point.y).y );
				paint.MoveTo( Coordposi(p.x, p.y).x,
					Coordposi(p.x, p.y).y);
				paint.LineTo( Coordposi(back_point.x, back_point.y).x,
					Coordposi(back_point.x, back_point.y).y );
				penDarkGray.DeleteObject();
			} 
			else if ((cinfo.vloc.objectlist[i].object_type == VisibleObject::blue_goal || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::blue_goal_post_left || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::blue_goal_post_right) && ppref.show_vis_goals) 
			{
				CPen penBlue2(PS_SOLID,2,COL_BLUE);
				paint.SelectObject(&penBlue2);
				paint.Arc(  Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y,
					Coordposi(p.x+50, p.y+50).x,
					Coordposi(p.x+50, p.y+50).y,
					Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y,
					Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y);
				penBlue2.DeleteObject();
			} 
			else if ((cinfo.vloc.objectlist[i].object_type == VisibleObject::yellow_goal || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::yellow_goal_post_left || 
				cinfo.vloc.objectlist[i].object_type == VisibleObject::yellow_goal_post_right) && ppref.show_vis_goals)
			{
				CPen penYellow2(PS_SOLID,2,COL_YELLOW);
				paint.SelectObject(&penYellow2);
				paint.Arc(  Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y,
					Coordposi(p.x+50, p.y+50).x,
					Coordposi(p.x+50, p.y+50).y,
					Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y,
					Coordposi(p.x-50, p.y-50).x,
					Coordposi(p.x-50, p.y-50).y);
				penYellow2.DeleteObject();
			}
		}
	}
     
	if (ppref.show_sl_pos) 
	{
		paint.SelectObject(&penDarkCyan);
		double psum=0;
 		
		for (unsigned int j=0; j<cinfo.ppos.size(); j++)
			psum+=cinfo.ppos[j].value;
		double pfactor = cinfo.ppos.size()/psum;
		for (unsigned int i=0; i<cinfo.ppos.size(); i++) 
		{
			double symsize = sqrt(cinfo.ppos[i].value*pfactor);
			if (symsize<0.1) symsize=0.1;
			Vec p0 = cinfo.ppos[i].pos;
			Vec p1 = p0+60*symsize*Vec::unit_vector(cinfo.ppos[i].heading+Angle::quarter);
			paint.Arc(  Coordposi(p0.x-20*symsize, p0.y-20*symsize).x,
				Coordposi(p0.x-20*symsize, p0.y-20*symsize).y,
				Coordposi(p0.x+20*symsize, p0.y+20*symsize).x,
				Coordposi(p0.x+20*symsize, p0.y+20*symsize).y,
				Coordposi(p0.x-20*symsize, p0.y-20*symsize).x,
				Coordposi(p0.x-20*symsize, p0.y-20*symsize).y,
				Coordposi(p0.x-20*symsize, p0.y-20*symsize).x,
				Coordposi(p0.x-20*symsize, p0.y-20*symsize).y);
			
			paint.MoveTo( Coordposi(p0.x, p0.y).x, Coordposi(p0.x, p0.y).y);
			paint.LineTo( Coordposi(p1.x, p1.y).x, Coordposi(p1.x, p1.y).y );
			
		}
	}
    
	if (ppref.show_aux_lines)
	{
		CPen penlaux(PS_SOLID,2,COL_BLACK);
		paint.SelectObject(&penlaux);
		deque<string> parts;
		split_string (parts, cinfo.paintmsg);
		parts.push_back ("end_of_commands");
		deque<double> params;
		string latest_command = "";
        while (parts.size()>0) 
		{
			double arg;
			if (latest_command=="word") 
			{
				if (parts.size()>=3) 
				{ 
					paint.SelectObject(&penlaux);
					Vec p1, p2;
					string2double (p1.x, parts.front());
					parts.pop_front();
					string2double (p1.y, parts.front());
					parts.pop_front();
					p2 = sf>0 ? field2widget (p1) : Vec(rc.Width()-1, rc.Height()-1)-field2widget (p1);
					paint.TextOut(static_cast<int>(p2.x), static_cast<int>(p2.y),parts.front().c_str());
				} 
				else
					parts.clear();
			}
			else if (string2double(arg, parts.front())) 
			{
				params.push_back(arg);
			}
			else
			{
				if (latest_command == "line")
				{
					for (unsigned int i=3; i<params.size(); i+=2)
					{
						paint.MoveTo(Coordposf(params[i-3], params[i-2]).x,Coordposf(params[i-3], params[i-2]).y);
						paint.LineTo(Coordposf(params[i-1], params[i]).x,Coordposf(params[i-1], params[i]).y);
					}
				}
				else if (latest_command=="circle" && params.size()>=3)
				{
					paint.Arc(  Coordposi(params[0]-params[2], params[0]-params[2]).x,
						Coordposi(params[0]-params[2], params[0]-params[2]).y,
						Coordposi(params[0]+params[2], params[0]+params[2]).x,
						Coordposi(params[0]+params[2], params[0]+params[2]).y,
						Coordposi(params[0]-params[2], params[0]-params[2]).x,
						Coordposi(params[0]-params[2], params[0]-params[2]).y,
						Coordposi(params[0]-params[2], params[0]-params[2]).x,
						Coordposi(params[0]-params[2], params[0]-params[2]).y);
				}
				else if (latest_command=="arc" && params.size()>=5)
				{
					double sax = params[0]+params[0]*cos(params[4]);
					double say = params[0]+params[0]*sin(params[4]);
					double eax = params[0]+params[0]*cos(params[5]);
					double eay = params[0]+params[0]*sin(params[5]);
					
					paint.Arc(  Coordposi(params[0]-params[2], params[0]-params[2]).x,
						Coordposi(params[0]-params[2], params[0]-params[2]).y,
						Coordposi(params[0]+params[2], params[0]+params[2]).x,
						Coordposi(params[0]+params[2], params[0]+params[2]).y,
						Coordposi(sax, say).x,
						Coordposi(sax, say).y,
						Coordposi(eax, eay).x,
						Coordposi(eax, eay).y);
				}
				else if (latest_command=="cross") 
				{
					for (unsigned int i=1; i<params.size(); i+=2)
					{
						paint.MoveTo(Coordposf(params[i-1]-35.0, params[i]-35.0).x,Coordposf(params[i-1]-35.0, params[i]-35.0).y);
						paint.LineTo(Coordposf( params[i-1]+35.0, params[i]+35.0).x,Coordposf( params[i-1]+35.0, params[i]+35.0).y);
						paint.MoveTo(Coordposf(params[i-1]-35.0, params[i]+35.0).x,Coordposf(params[i-1]-35.0, params[i]+35.0).y);
						paint.LineTo(Coordposf(params[i-1]+35.0, params[i]-35.0).x,Coordposf(params[i-1]+35.0, params[i]-35.0).y);
						
					}
				}
				else if (latest_command=="arrow") 
				{
					for (unsigned int i=3; i<params.size(); i+=4) 
					{
						double x1 = params[i-3];
						double y1 = params[i-2];
						double x2 = params[i-1];
						double y2 = params[i];
						paint.MoveTo(Coordposf(x1, y1).x,Coordposf(x1, y1).y);
						paint.LineTo(Coordposf(x2, y2).x,Coordposf(x2, y2).y);
						double len = 0.1+sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
						double unit_x = (x1-x2)/len;
						double unit_y = (y1-y2)/len;
						paint.MoveTo(	Coordposf(x2, y2).x,Coordposf(x2, y2).y);
						paint.LineTo(	Coordposf(x2+200*unit_x+50*unit_y, y2+200*unit_y-50*unit_x).x,
							Coordposf(x2+200*unit_x+50*unit_y, y2+200*unit_y-50*unit_x).y);
						paint.MoveTo(	Coordposf(x2, y2).x,Coordposf(x2, y2).y);
						paint.LineTo(	Coordposf(x2+200*unit_x-50*unit_y, y2+200*unit_y+50*unit_x).x,
							Coordposf(x2+200*unit_x-50*unit_y, y2+200*unit_y+50*unit_x).y);
						
					}
				}
				else if (latest_command=="color" && params.size()>=3) 
				{
					penlaux.CreatePen(PS_SOLID,1,RGB(static_cast<int>(params[0]), 
						static_cast<int>(params[1]), static_cast<int>(params[2])) );
				}
				else if (latest_command=="white")
					penlaux.CreatePen(PS_SOLID,1,COL_WHITE);
				else if (latest_command=="grey")
					penlaux.CreatePen(PS_SOLID,1,RGB(162,162,162));
				else if (latest_command=="black")
					penlaux.CreatePen(PS_SOLID,1,COL_BLACK);
				else if (latest_command=="yellow")
					penlaux.CreatePen(PS_SOLID,1,COL_YELLOW);
				else if (latest_command=="orange")
					penlaux.CreatePen(PS_SOLID,1,RGB(241,255,119));
				else if (latest_command=="dark_red")
					penlaux.CreatePen(PS_SOLID,1,RGB(152, 0, 0));
				else if (latest_command=="red")
					penlaux.CreatePen(PS_SOLID,1,RGB(225, 84, 0));
				else if (latest_command=="light_red")
					penlaux.CreatePen(PS_SOLID,1,RGB(255, 107, 107));
				else if (latest_command=="violet")
					penlaux.CreatePen(PS_SOLID,1,RGB(231, 63, 229));
				else if (latest_command=="dark_blue")
					penlaux.CreatePen(PS_SOLID,1,RGB(114, 61, 179));
				else if (latest_command=="blue")
					penlaux.CreatePen(PS_SOLID,1,RGB(90, 71, 227));
				else if (latest_command=="light_blue")
					penlaux.CreatePen(PS_SOLID,1,RGB(97, 116, 255));
				else if (latest_command=="turquoise")
					penlaux.CreatePen(PS_SOLID,1,RGB(128, 237, 192));
				else if (latest_command=="dark_green")
					penlaux.CreatePen(PS_SOLID,1,RGB(0, 97, 18));
				else if (latest_command=="green")
					penlaux.CreatePen(PS_SOLID,1,RGB(84, 213, 7));
				else if (latest_command=="light_green")
					penlaux.CreatePen(PS_SOLID,1,RGB(96, 255, 0));
				else if (latest_command=="brown")
					penlaux.CreatePen(PS_SOLID,1,RGB(142, 85, 10));
				else if (latest_command=="solid")
					penlaux.CreatePen(PS_SOLID,1,COL_RED);
				else if (latest_command=="dashed")
					penlaux.CreatePen(PS_DASH,1,COL_RED);
				else if (latest_command=="dotted")
					penlaux.CreatePen(PS_DOT,1,COL_RED);
				else if (latest_command=="thin")
					penlaux.CreatePen(PS_SOLID,1,COL_RED);
				else if (latest_command=="thick")
					penlaux.CreatePen(PS_SOLID,2,COL_RED);
				params.clear();
				latest_command = parts.front();
				paint.SelectObject(penlaux);
				
			 }
			 parts.pop_front();
		 }
		 penlaux.DeleteObject();
	}
	// draw auxiliary line set by mouse
	if (ppref.mem.size()>0)
	{
		paint.SelectObject(&penMagenta);
		for (unsigned int i=0; i<ppref.mem.size(); i++)
		{
			paint.MoveTo( Coordposi(ppref.mem[i].getStart().x,ppref.mem[i].getStart().y).x, 
				Coordposi(ppref.mem[i].getStart().x,ppref.mem[i].getStart().y).y);
			paint.LineTo( Coordposi(ppref.mem[i].getEnd().x,ppref.mem[i].getEnd().y).x, 
				Coordposi(ppref.mem[i].getEnd().x,ppref.mem[i].getEnd().y).y );
		}
	}

	if (mouse_mode != 0)
	{
		paint.SelectStockObject(NULL_BRUSH);
		switch(mouse_mode)
		{
		case 1: // movement windows
			{
				paint.SelectObject(&penWhiteDot2);
				Vec mp = widget2field ( mp_mouse );
				paint.MoveTo( Coordposi(p_click.x,p_click.y).x, 
					Coordposi(p_click.x,p_click.y).y);
				paint.LineTo( Coordposi(mp.x,mp.y).x, 
					Coordposi(mp.x,mp.y).y );
				
			}
			break;
		case 2: // zoom
			{
				paint.SelectObject(&penWhiteDot2);
				Vec mp = widget2field ( mp_mouse );
				paint.Rectangle(Coordposi(p_click.x,p_click.y).x, Coordposi(p_click.x,p_click.y).y,
								Coordposi(mp.x,mp.y).x,Coordposi(mp.x,mp.y).y);
 
			}
			break;
		case 3: // point and direction
			{
				CPen ps2(PS_SOLID,2,COL_WHITE);
				CPen psg2(PS_SOLID,2,COL_LIGHTGRAY);
				CBrush br(COL_RED);
				paint.SelectObject(&ps2);
				Vec mp = widget2field ( mp_mouse );
				paint.MoveTo(	Coordposi(p_click.x,p_click.y).x, 
								Coordposi(p_click.x,p_click.y).y);
				paint.LineTo(	Coordposi(mp.x,mp.y).x, 
								Coordposi(mp.x,mp.y).y );
				
				paint.SelectObject(&psg2);
                paint.SelectObject(&br);
                paint.Pie( Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).x, 
						   Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).y,
					       Coordposi(p_click.x+2.0/ppref.zoom.scaling,p_click.y+2.0/ppref.zoom.scaling).x, 
						   Coordposi(p_click.x+2.0/ppref.zoom.scaling,p_click.y+2.0/ppref.zoom.scaling).y,
					       Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).x,
						   Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).y,
						   Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).x, 
						   Coordposi(p_click.x-2.0/ppref.zoom.scaling,p_click.y-2.0/ppref.zoom.scaling).y
						  );
				ps2.DeleteObject();
				psg2.DeleteObject();
				br.DeleteObject();
			}
			break;
		case 4: // sl reposition
			{
			    CBrush br;
				br.CreateSolidBrush(COL_WHITE);
				paint.SelectObject(penWhiteDot2);
				Vec mp = widget2field ( mp_mouse );
				Vec vertices [] = { Vec (156,175 ), Vec (74,223), Vec (-230,48), 
					Vec (-230,-48), Vec (74,-223), Vec ( 156,-175), Vec (230,-140), 
					Vec (156,-140), Vec (156,140), Vec (230,140), Vec (200,-100), 
					Vec (200,100) };
				for (unsigned int j=0; j<12; j++) 
					vertices[j] = vertices[j].rotate((mp-p_click).angle()-Angle::quarter)+p_click;
				
				CPoint rob[6];
				for (unsigned int k=0; k<6; k++)
					rob[k] = Coordposi(vertices[k].x, vertices[k].y);
				
				CRgn argn;
				argn.CreatePolygonRgn(rob,6,1);
				paint.FillRgn(&argn,&br);
				br.DeleteObject();
				
				paint.MoveTo( Coordposi(vertices[6].x, vertices[6].y).x,
					Coordposi(vertices[6].x, vertices[6].y).y);
				paint.LineTo( Coordposi(vertices[7].x, vertices[7].y).x,
					Coordposi(vertices[7].x, vertices[7].y).y );
				paint.MoveTo( Coordposi(vertices[8].x, vertices[8].y).x,
					Coordposi(vertices[8].x, vertices[8].y).y);
				paint.LineTo( Coordposi(vertices[9].x, vertices[9].y).x,
					Coordposi(vertices[9].x, vertices[9].y).y );
  
			}
			break;
		case 5: // sl windows
			{
				paint.SelectObject(&penWhiteDot2);
				Vec mp = widget2field ( mp_mouse );
				paint.Rectangle(Coordposi(p_click.x,p_click.y).x, Coordposi(p_click.x,p_click.y).y,
								Coordposi(mp.x,mp.y).x,Coordposi(mp.x,mp.y).y);
			}
			break;
		case 6: // draw auxiliary line set
			{
				paint.SelectObject(penWhite);
				Vec mp = widget2field ( mp_mouse );
				paint.MoveTo( Coordposi(p_click.x,p_click.y).x, 
					Coordposi(p_click.x,p_click.y).y);
				paint.LineTo( Coordposi(mp.x,mp.y).x, 
					Coordposi(mp.x,mp.y).y );
				}
			break;
		case 8: // struct to drive at position
			{
				CBrush br;
				br.CreateSolidBrush(COL_RED);
				CPen pr2(PS_DOT,2,COL_RED);
				Vec mp = widget2field ( mp_mouse );
				Vec vertices [] = { Vec (156,175 ), Vec (74,223), Vec (-230,48), 
					Vec (-230,-48), Vec (74,-223), Vec ( 156,-175), Vec (230,-140), 
					Vec (156,-140), Vec (156,140), Vec (230,140), Vec (200,-100), 
					Vec (200,100) };
				for (unsigned int j=0; j<12; j++) 
					vertices[j] = vertices[j].rotate((mp-p_click).angle()-Angle::quarter)+p_click;
				
				CPoint rob[6];
				for (unsigned int k=0; k<6; k++)
					rob[k] = Coordposi(vertices[k].x, vertices[k].y);
				
				CRgn argn;
				argn.CreatePolygonRgn(rob,6,1);
				paint.FillRgn(&argn,&br);
				
				paint.MoveTo( Coordposi(vertices[6].x, vertices[6].y).x,
					Coordposi(vertices[6].x, vertices[6].y).y);
				paint.LineTo( Coordposi(vertices[7].x, vertices[7].y).x,
					Coordposi(vertices[7].x, vertices[7].y).y );
				paint.MoveTo( Coordposi(vertices[8].x, vertices[8].y).x,
					Coordposi(vertices[8].x, vertices[8].y).y);
				paint.LineTo( Coordposi(vertices[9].x, vertices[9].y).x,
					Coordposi(vertices[9].x, vertices[9].y).y );

				br.DeleteObject();
				pr2.DeleteObject();
  
			}
			break;
		default:
			break;
		}
	}
}

const PaintPreferences& FieldWidget::get_preferences () const 
{
	return ppref;
}

void FieldWidget::next_cycle (const CycleInfo& nci) 
{
	cinfo = nci;   
	vector<RobotLocation>& rloc =(ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
	vector<BallLocation>& bloc = (ppref.use_exec_time ? cinfo.bloc_exec : cinfo.bloc_vis);
	if (ppref.show_robot_trace && rloc.size()>ppref.reference_robot)
		ppref.rtrace.push_back (rloc[ppref.reference_robot]);
	if (ppref.show_ball_trace && bloc.size()>ppref.reference_robot)
		ppref.btrace.push_back (bloc[ppref.reference_robot]);
	Invalidate(FALSE);
	::SendMessage(m_parent->m_hWnd,cycleChanged,0,0);
}

const CycleInfo& FieldWidget::get_cycle_info () const 
{
	return cinfo;
}

void FieldWidget::set_reference_robot (unsigned int i) 
{
	ppref.reference_robot = i;
}

std::vector<std::string>& FieldWidget::robot_names () 
{
	return robot_ids;
}

const std::vector<std::string>& FieldWidget::robot_names () const 
{
	return robot_ids;
}

void FieldWidget::clippingChanged () 
{
	ppref.old_zooms.push_back (ppref.zoom);
	ppref.redo_zooms.clear();
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);
	Invalidate();
}

void FieldWidget::move_clipping (Vec delta) 
{
	ppref.zoom.image_center+=delta;
	clippingChanged();
}

void FieldWidget::zoom_rect (Vec p1, Vec p2) 
{
	double fw = fabs (p1.x-p2.x);
	double fl = fabs (p1.y-p2.y);
	CRect client;
	GetClientRect(client);
	
	double iw = client.Width();
	double ih = client.Height()-dock_height;
	double scale_h = iw/fl;
	double scale_v = ih/fw;
	
	ppref.zoom.scaling = (scale_h<scale_v ? scale_h : scale_v);
	ppref.zoom.image_center = 0.5*(p1+p2);
	clippingChanged();
}

void FieldWidget::zoom_in () 
{
	ppref.zoom.scaling*=1.2;
	clippingChanged();
}

void FieldWidget::zoom_out ()
{
	ppref.zoom.scaling/=1.2;
	clippingChanged();
}

void FieldWidget::zoom_all () 
{
	double fw = fgeometry.field_width+2*fgeometry.side_band_width;
	double fl = fgeometry.field_length+2*fgeometry.goal_band_width;
	CRect client;
	GetClientRect(client);
	double iw = client.Width();
	double ih = client.Height()-dock_height;
	double scale_h = iw/fl;
	double scale_v = ih/fw;
	ppref.zoom.scaling = (scale_h<scale_v ? scale_h : scale_v);
	ppref.zoom.image_center = Vec(0,0);
	clippingChanged();
}

void FieldWidget::zoom_undo () 
{
	if (ppref.old_zooms.size()>1) 
	{
		ppref.redo_zooms.push_back (ppref.old_zooms.back());
		ppref.old_zooms.pop_back();
		ppref.zoom = ppref.old_zooms.back();
		Invalidate();
	}
}

void FieldWidget::zoom_redo () 
{
	if (ppref.redo_zooms.size()>=1) 
	{
		ppref.zoom = ppref.redo_zooms.back();
		ppref.redo_zooms.pop_back();
		ppref.old_zooms.push_back (ppref.zoom);
		Invalidate();
	}  
}

void FieldWidget::flip_side () 
{
	ppref.zoom.own_half*=-1;
	clippingChanged();
}

void FieldWidget::flip_goals () 
{
	ppref.field_heading*=-1;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);
	Invalidate();
}

Vec FieldWidget::widget2field (Vec u) const 
{
	Vec z;
	double s = ppref.zoom.own_half * ppref.zoom.scaling;
	CRect client;
	GetClientRect(client);
	
	z.x=(u.x-0.5*client.Width())/s+ppref.zoom.image_center.x;
	z.y=(u.y-0.5*dock_height-0.5*client.Height())/s+ppref.zoom.image_center.y;
	return z;
}

Vec FieldWidget::field2widget (Vec z) const 
{
	Vec u;
	double s = ppref.zoom.own_half*ppref.zoom.scaling;
	CRect client;
	GetClientRect(client);
	
	u.x = 0.5*client.Width() + s * ( z.x - ppref.zoom.image_center.x );
	u.y = 0.5*client.Height()+0.5*dock_height+s*(z.y-ppref.zoom.image_center.y);
	return u;
}

// ----------------------------------------------------------------------
// 
// ----------------------------------------------------------------------

void FieldWidget::show_wm_robot (bool b) 
{
	ppref.show_wm_robot = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);
	Invalidate();
}

void FieldWidget::show_wm_ball (bool b) 
{
	ppref.show_wm_ball= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);
	Invalidate();
}

void FieldWidget::show_wm_obstacles (bool b) 
{
	ppref.show_wm_obstacles= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_vis_lines (bool b) 
{
	ppref.show_vis_lines= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_vis_ball (bool b) 
{
	ppref.show_vis_ball= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_vis_obstacles (bool b) 
{
	ppref.show_vis_obstacles= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_vis_goals (bool b) 
{
	ppref.show_vis_goals= b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_sl_pos (bool b) 
{
	ppref.show_sl_pos = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_robot_ids (bool b) 
{
	ppref.show_robot_ids = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_robot_ball_links (bool b) 
{
	ppref.show_robot_ball_links = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_aux_lines (bool b) 
{
	ppref.show_aux_lines = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::use_exec_time (bool b) 
{
	ppref.use_exec_time = b;
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::toggle_robot_trace () 
{
	show_robot_trace (!ppref.show_robot_trace);
}

void FieldWidget::show_robot_trace (bool b) 
{
	vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
	ppref.show_robot_trace = b;
	if (b) 
	{
		if (rloc.size()>ppref.reference_robot)
			ppref.rtrace.push_back (rloc[ppref.reference_robot]);
	} 
	else
		ppref.rtrace.clear();
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::show_ball_trace (bool b) 
{
	vector<BallLocation>& bloc = (ppref.use_exec_time ? cinfo.bloc_exec : cinfo.bloc_vis);
	ppref.show_ball_trace = b;
	if (b) 
	{
		if (bloc.size()>ppref.reference_robot)
			ppref.btrace.push_back (bloc[ppref.reference_robot]);
	}
	else
		ppref.btrace.clear();
	::SendMessage(m_parent->m_hWnd,preferencesChanged,0,0);;
	Invalidate();
}

void FieldWidget::clear_lines () 
{
	ppref.mem.clear();
	Invalidate();
}

void FieldWidget::next_refrobot () 
{
	ppref.reference_robot = (ppref.reference_robot+1)%cinfo.rloc_vis.size();
}


void FieldWidget::OnMouseMove(UINT nFlags, CPoint point) 
{
    mp_mouse = Vec(point.x, point.y);
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}

void FieldWidget::OnLButtonDown(UINT nFlags, CPoint point) 
{
    p_click = widget2field (Vec(point.x, point.y));
    if ( !( ::GetKeyState(VK_CONTROL) < 0 ) && !(::GetKeyState(VK_SHIFT) < 0) ) 
       mouse_mode = 1;
	else if (::GetKeyState(VK_SHIFT) < 0 && !(::GetKeyState(VK_CONTROL) < 0) )
	{
		mouse_mode = 3;
	}
	else if (::GetKeyState(VK_CONTROL) < 0 && !(::GetKeyState(VK_SHIFT) < 0) )
	{
		mouse_mode = 4;
	}
	else if ((::GetKeyState(VK_CONTROL) < 0)  && (::GetKeyState(VK_SHIFT) < 0 ) )
	{
		mouse_mode = 8;
	}
	else
	{
		mouse_mode = 0;
		Invalidate();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void FieldWidget::OnRButtonDown(UINT nFlags, CPoint point) 
{
    p_click = widget2field (Vec(point.x, point.y));
    if ( !( ::GetKeyState(VK_CONTROL) < 0 ) && !(::GetKeyState(VK_SHIFT) < 0) ) 
       mouse_mode = 2;
	else if (::GetKeyState(VK_SHIFT) < 0 && !(::GetKeyState(VK_CONTROL) < 0) )
	{
		mouse_mode = 6;
	}
	else if (::GetKeyState(VK_CONTROL) < 0 && !(::GetKeyState(VK_SHIFT) < 0) )
	{
		mouse_mode = 7;
	}
	else if ((::GetKeyState(VK_CONTROL) < 0)  && (::GetKeyState(VK_SHIFT) < 0 ) )
	{
		mouse_mode = 5;
	}
	else
	{
		mouse_mode = 0;
		Invalidate();
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void FieldWidget::OnLButtonUp(UINT nFlags, CPoint point) 
{
    Vec p_click2 = widget2field (Vec(point.x, point.y));
	vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
	CString s = "";
    Vec pos = Vec::zero_vector;
	Angle h = 0;

	Vec slpos = Vec::zero_vector;
	Angle slh = 0;

    switch(mouse_mode)
	{
		case 1:
			move_clipping (p_click-p_click2);
			Invalidate ();
     		break;
		case 2:
			zoom_rect (p_click, p_click2);
			Invalidate ();
			break;
		case 3:
			s.Format("x= %.fmm, y=%.fmm, phi=%.f? l=%.fmm",p_click.x,p_click.y,(p_click2-p_click).angle().get_deg(),
				     (p_click2-p_click).length() );
			::SendMessage(m_parent->m_hWnd,coutMessage,(WPARAM)&s,0);
			break;
		case 4:
			{
				if (rloc.size()>ppref.reference_robot) 
				{
					rloc[ppref.reference_robot].pos = p_click;
					rloc[ppref.reference_robot].heading = (p_click2-p_click).angle()-Angle::quarter;
					slpos = p_click;
					slh   = (p_click2-p_click).angle()-Angle::quarter;
					Invalidate();
					::SendMessage(m_parent->m_hWnd,slDisplacementMSg,(WPARAM)(&slpos),(LPARAM)(&slh));
				}
			}
			break;
		case 5:
			::SendMessage(m_parent->m_hWnd,unresolvedMouseRect,0,0);
 			break;
		case 6:
			if ((p_click2-p_click).squared_length()>=100)
				ppref.mem.push_back (LineSegment (p_click,  p_click2));
			else 
			{
				ppref.mem.push_back (LineSegment (p_click-Vec(10,10),  p_click+Vec(10,10)));
				ppref.mem.push_back (LineSegment (p_click-Vec(-10,10),  p_click+Vec(-10,10)));	
			}
			Invalidate ();
			break;
		case 7:
			{
				double mindist=1e300;
				unsigned int minindex=0;
				vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
				for (unsigned int i=0; i<rloc.size(); i++)
				{
					double d=(rloc[i].pos-p_click2).length();
					if (d<mindist) 
					{
						mindist=d;
						minindex=i;
					}
				}
				if (mindist<1000) 
				{
					ppref.reference_robot=minindex;
				}
			}
			break;
		case 8:
			{
			   pos = p_click;
               h = (p_click2-p_click).angle()-Angle::quarter;
			   ::SendMessage(m_parent->m_hWnd,robotDisplacementMsg,(WPARAM)(&pos),(LPARAM)(&h));
			}
		default:
    		break;
    }	
    mouse_mode = 0;
	CWnd::OnLButtonUp(nFlags, point);
}

void FieldWidget::OnRButtonUp(UINT nFlags, CPoint point) 
{
    Vec p_click2 = widget2field (Vec(point.x, point.y));
	vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
	CString s = "";
    Vec pos = Vec::zero_vector;
	Angle h = 0;

    Vec slpos = Vec::zero_vector;
	Angle slh = 0;

	switch(mouse_mode)
	{
		case 1:
			move_clipping (p_click-p_click2);
			Invalidate ();
     		break;
		case 2:
			zoom_rect (p_click, p_click2);
			Invalidate ();
			break;
		case 3:
			s.Format("x= %.fmm, y=%.fmm, phi=%.f? l=%.fmm",p_click.x,p_click.y,(p_click2-p_click).angle().get_deg(),
				     (p_click2-p_click).length() );
			::SendMessage(m_parent->m_hWnd,coutMessage,(WPARAM)&s,0);
			break;
		case 4:
			{
				if (rloc.size()>ppref.reference_robot) 
				{
					rloc[ppref.reference_robot].pos = p_click;
					rloc[ppref.reference_robot].heading = (p_click2-p_click).angle()-Angle::quarter;
					slpos = p_click;
					slh   = (p_click2-p_click).angle()-Angle::quarter;
					Invalidate();
					::SendMessage(m_parent->m_hWnd,slDisplacementMSg,(WPARAM)(&slpos),(LPARAM)(&slh));
				}
			}
			break;
		case 5:
			::SendMessage(m_parent->m_hWnd,unresolvedMouseRect,0,0);
 			break;
		case 6:
			if ((p_click2-p_click).squared_length()>=100)
				ppref.mem.push_back (LineSegment (p_click,  p_click2));
			else 
			{
				ppref.mem.push_back (LineSegment (p_click-Vec(10,10),  p_click+Vec(10,10)));
				ppref.mem.push_back (LineSegment (p_click-Vec(-10,10),  p_click+Vec(-10,10)));	
			}
			Invalidate ();
			break;
		case 7:
			{
				double mindist=1e300;
				unsigned int minindex=0;
				vector<RobotLocation>& rloc = (ppref.use_exec_time ? cinfo.rloc_exec : cinfo.rloc_vis);
				for (unsigned int i=0; i<rloc.size(); i++)
				{
					double d=(rloc[i].pos-p_click2).length();
					if (d<mindist) 
					{
						mindist=d;
						minindex=i;
					}
				}
				if (mindist<1000) 
				{
					ppref.reference_robot=minindex;
				}
			}
			break;
		case 8:
			{
			   pos = p_click;
               h = (p_click2-p_click).angle()-Angle::quarter;
			   ::SendMessage(m_parent->m_hWnd,robotDisplacementMsg,0,0);
			}
			break;
		default:
    		break;
    } 	
    mouse_mode = 0;	
	CWnd::OnRButtonUp(nFlags, point);
}

void FieldWidget::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();
	// TODO: Add your message handler code here
	
}
