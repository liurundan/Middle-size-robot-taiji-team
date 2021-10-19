#if !defined(AFX_FIELDWIDGET_H__A7958AC3_3E34_4FDD_B217_CBE0363F546F__INCLUDED_)
#define AFX_FIELDWIDGET_H__A7958AC3_3E34_4FDD_B217_CBE0363F546F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldWidget.h : header file
//
#include "CycleInfo.h"
#include "FieldGeometry.h"
#include "PaintPreferences.h"
#include "Frame2D.h"

#define		slDisplacementMSg			WM_USER+104
#define		robotDisplacementMsg		WM_USER+105
#define		preferencesChanged			WM_USER+106
#define		cycleChanged				WM_USER+107
#define		unresolvedKeyPressEvent		WM_USER+108
#define		unresolvedMouseRect			WM_USER+109
#define		coutMessage					WM_USER+110

/////////////////////////////////////////////////////////////////////////////
// FieldWidget window

class FieldWidget : public CWnd
{
// Construction
public:
	FieldWidget();
    void init (const FieldGeometry&);
    void init (const FieldGeometry&, const PaintPreferences&, const PaintActionSelect& );
    const PaintPreferences& get_preferences () const;
    const CycleInfo& get_cycle_info () const;
    void set_reference_robot (unsigned int);
    std::vector<std::string>& robot_names ();
    const std::vector<std::string>& robot_names () const ;

	void zoom_in ();
    void zoom_out ();
    void zoom_all ();
    void zoom_undo ();
    void zoom_redo ();
    void flip_side ();
    void flip_goals ();
    void next_refrobot ();
    void show_wm_robot (bool);
    void show_wm_ball (bool);
    void show_wm_obstacles (bool);
    void show_vis_lines (bool);
    void show_vis_ball (bool);
    void show_vis_obstacles (bool);
    void show_vis_goals (bool);
    void show_sl_pos (bool);
    void show_aux_lines (bool);
    void show_robot_ids (bool);
    void show_robot_ball_links (bool);
    void show_robot_trace (bool);
    void show_ball_trace(bool);
    void clear_lines();
    void use_exec_time (bool);
	
    void toggle_robot_trace (); 
	
    void next_cycle (const CycleInfo&);                    

    void move_clipping (Vec);
    void zoom_rect (Vec, Vec);
	void clippingChanged();
 
	Vec widget2field (Vec) const;
    Vec field2widget (Vec) const; 
	
	
    PaintPreferences	ppref;
    CycleInfo			cinfo;
protected:
    void paintEvent(CDC&);

private:
	CPoint Coordposf(double,double);
	CPoint Coordposi(double,double);
private:
	CToolBar			m_ToolBar;
    CWnd*				m_parent;

    FieldGeometry		fgeometry;
    std::vector<std::string> robot_ids;

    int					mouse_mode;
    Vec					p_click;
    
	Vec                 mp_mouse;

    unsigned int		dock_height;
    
	CPen    penWhite;
	CPen    penWhiteDot2;
	CPen    penBlack;
	CPen    penYellow;
	CPen    penBlue;
	CPen    penRed;
	CPen    penRed2;
	CPen    penRedDot;
	CPen    penRedDot2;
	CPen    penDarkRedDot;
	
	CPen    penCyan;
	CPen    penCyanDot;
	CPen    penDarkCyan;
	CPen    penMagenta;

    CBrush  brushBlack;
    CBrush  brushWhite;
	CBrush  brushRed;
	CBrush  brushCyan;
	CBrush  brushDarkCyan;
	CBrush  brushYellow;

    CFont   playerFont;

	CDC     dcMem;
	CBitmap bm;
	CRect   rc;
  
	Frame2d mapping_f;
    Frame2d mapping_i;
    int     xoffset;
	int     yoffset;
	double  xscale,yscale;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FieldWidget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FieldWidget();

	// Generated message map functions
protected:
	//{{AFX_MSG(FieldWidget)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDWIDGET_H__A7958AC3_3E34_4FDD_B217_CBE0363F546F__INCLUDED_)
