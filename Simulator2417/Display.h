
#if !defined(AFX_DISPLAY_H__E205EEA2_F363_4A96_86A0_ACB9DCCC5FD7__INCLUDED_)
#define AFX_DISPLAY_H__E205EEA2_F363_4A96_86A0_ACB9DCCC5FD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Display.h : header file
#include "WorldModel.h"
#include "LineScanning.h"
#include "VisibleObject.h"

#define FIELD_LENGHT           1200
#define FIELD_WIDTH            800
///////////////////////////�򳡵ĳߴ�///////////////////////////////////////
#define PITCH_DISPLAY_SCALE     (76.8f/FIELD_LENGHT)                //ϵ��:����ʵ���򳡵ĳߴ�仯

#define PITCH_LENGTH_HALF       PITCH_DISPLAY_SCALE*(FIELD_LENGHT/2.0)      // �߽���ʵ�ʰ볤��:FIELD_X/2
#define PTICH_WIDTH_HALF        PITCH_DISPLAY_SCALE*(FIELD_WIDTH/2.0)      // �߽���ʵ�ʰ���:FIELD_Y/2


#define GOAL_WIDTH_HALF         PITCH_DISPLAY_SCALE*100               // ���Ű��:ʵ��100cm
#define GOAL_LENGTH_HALF        PITCH_DISPLAY_SCALE*50                // ������:ʵ��50cm

//������:
#define GOAL_CARE_RANGE_LENGTH  PITCH_DISPLAY_SCALE*150           // �Ͽ��߾���������:ʵ��1500mm 1750
#define GOAL_CARE_RANGE_WIDTH   PITCH_DISPLAY_SCALE*50            // �����߾������:ʵ��500mm  750

//������:
#define PENALTY_LENGTH          PITCH_DISPLAY_SCALE*250              //�Ͽ��߾���������:ʵ��2500mm 3250
#define PENALTY_WIDTH           PITCH_DISPLAY_SCALE*150              //�����߾������:ʵ��1500mm 2250

//����Բ�뾶:
#define PITCH_CENTER_RADIUS     PITCH_DISPLAY_SCALE*100 //6.4f         //����Բʵ�ʰ뾶:1000mm

//��뾶:
#define BAll_RADIUS             PITCH_DISPLAY_SCALE*10//1.2f           //��ʵ�ʰ뾶:mm???
//�����˰뾶:
#define ROBOT_RADIUS            PITCH_DISPLAY_SCALE*25//1.5f           //������ʵ�ʰ뾶:25cm

//�ǻ��뾶:                        
#define CORNER_ARC_RADIUS       PITCH_DISPLAY_SCALE*50 //3.2f          //�ǻ�ʵ�ʰ뾶:500mm 750

//���������������ߺͱ߽���ֵ:            
#define CORNER_LINE_DIST        PITCH_DISPLAY_SCALE*50 //3.2f          //���������������ߺͱ߽���ʵ�ʾ���:500mm

// ����������Ļ�ϵ�����
#define FIELD_CENTER_SCREEN_X     350
#define FIELD_CENTER_SCREEN_Y     250  

/////////////////////////////////////////////////////////////////////////////
// CDisplay window

class CDisplay : public CWnd
{
// Construction
public:
	CDisplay();

// Attributes
public:
   	HDC   hdc ;
	HGLRC hglrc;
    
	Vector ballsimulatedpos;
	Vector BallPosition;
	Vector PlayerPosition[15];
	Vector InitialObjectPosition[15];
    
	ScanResultList*    scanResults;
 	VisibleObjectList lines;
	int	           innerRadius;
	
	double m_player_angle[15];
	
	float m_player_y[15];
	float m_player_x[15];
	float m_ball_y;
	float m_ball_x;
	float lwidth[4];
	float psize[2];
	CRect my_oldRect;
	
	GLUquadricObj *quadObj1;
    GLUquadricObj *quadObj2;
 	GLUquadricObj *quadObj3;
    GLUquadricObj *quadObj4;

	BOOL  bShowPlayerNumber;

	BOOL  bMoveObject;
	BOOL  bIsMoveBall;
	long  int m_Cycle;
    // Operations
public:
    int  MySetPixelFormat(HDC hdc);
 	void DrawBall();
	void DrawSimBall();
    void DrawPlayer();
 	void DrawField();
    void DrawLine( Vector pos1, Vector pos2 );
    void DrawCircle( Vector pos, double radius, double inner = 0.0f );

	void DrawRect( Vector pos1, Vector pos2 ,bool bRect = true);
	void DrawPoint();
    
	void SetPixel(int xcoord, int ycoord, float r, float g, float b);        //���㺯��
    void SetVisualizeScanResult();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplay)
	//}}AFX_VIRTUAL

// Implementation
public:
 	void ShowAllPlayer();
	virtual ~CDisplay();
private:		
	void DisplayAllObject();
 
	// Generated message map functions
protected:
	//{{AFX_MSG(CDisplay)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAY_H__E205EEA2_F363_4A96_86A0_ACB9DCCC5FD7__INCLUDED_)
