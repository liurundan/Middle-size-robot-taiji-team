// Display.cpp : implementation file
//

#include "stdafx.h"
#include "Simulator.h"
#include "Display.h"
#include "SimulatorDlg.h"
#include "Geometry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplay

CDisplay::CDisplay( )
{
  scanResults = new ScanResultList(9);
}

CDisplay::~CDisplay()
{
	wglMakeCurrent(NULL, NULL) ; 
	wglDeleteContext(hglrc);	//ɾ����Ⱦ������
	::ReleaseDC (m_hWnd, hdc) ; //�ͷ��豸������
	delete scanResults;
}


BEGIN_MESSAGE_MAP(CDisplay, CWnd)
	//{{AFX_MSG_MAP(CDisplay)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers

int CDisplay::MySetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
	    1,                                // �汾�� 
	    PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
	    PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
	    PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
	    PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
	    24,                               // 24 λ��ɫ��� 
	    0, 0, 0, 0, 0, 0,                 // ������ɫλ 
	    0,                                // û�з�͸���Ȼ��� 
	    0,                                // ������λλ 
	    0,                                // ���ۼӻ��� 
	    0, 0, 0, 0,                       // �����ۼ�λ 
	    32,                               // 32 λ��Ȼ���     
	    0,                                // ��ģ�建�� 
	    0,                                // �޸������� 
	    PFD_MAIN_PLANE,                   // ���� 
	    0,                                // ���� 
	    0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 
	
	int  iPixelFormat; 
 
// Ϊ�豸������õ���ƥ������ظ�ʽ 
	if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat Failed", NULL, MB_OK);
		return 0;
	}
	 
// ������ƥ������ظ�ʽΪ��ǰ�����ظ�ʽ 
	if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat Failed", NULL, MB_OK);
		return 0;
	}

	return 1;
}

int CDisplay::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
  
// ���õ�ǰ�Ļ�ͼ���ظ�ʽ
	MySetPixelFormat(::GetDC(m_hWnd));

// ��û�ͼ������
	hdc = ::GetDC(m_hWnd);
// ������Ⱦ������
	hglrc = wglCreateContext(hdc);
// ʹ��ͼ������Ϊ��ǰ�����̵ֳĵ�ǰ��ͼ������ 
	wglMakeCurrent(hdc, hglrc);	
////////////////////////////////////////////////////////////////////////////	
	GLfloat fPointsize[2];
    glGetFloatv (GL_POINT_SIZE_RANGE,fPointsize);

//  ��Ĵ�С���ã�
	psize[0]=fPointsize[0]*6;
    psize[1]=fPointsize[0]*18;
		
    GLfloat fLinewide[3];
    glGetFloatv (GL_LINE_WIDTH_RANGE,fLinewide);  

// �߿��С���ã�
	lwidth[0]=fLinewide[0]*0.01;
    lwidth[1]=fLinewide[0]*4;
    lwidth[2]=fLinewide[0]*8;
    lwidth[3]=fLinewide[0]*10;
	GLfloat Light_position[] = {0.0,-10.0,0.0,1.0	};
    glLightfv(GL_FRONT,GL_POSITION,Light_position);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  		
	glNewList(3,GL_COMPILE);
	DrawPlayer();
	glEndList(); 
 
	bShowPlayerNumber = FALSE;
	bMoveObject       = FALSE;
 	bIsMoveBall       = FALSE;
    m_Cycle           = 0;
    for( int i=1; i<=12; i++ )
	{
      PlayerPosition[i] = UnknownDoubleValue;
	  m_player_angle[i] = UnknownAngleValue;
	  if( i < 7 )
          InitialObjectPosition[i] = Vector( -500+(i-1)*80,-480); // -700 100 -700
      else
          InitialObjectPosition[i] = Vector( -380+(i-1)*80,-480); // -400 100 -700
	}
	BallPosition = UnknownDoubleValue;
	
	return 0;

}

void CDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    static BOOL bBusy = FALSE;
	if (bBusy) { return;	}
	bBusy = TRUE;
    //  ����OpenGL��ͼ��������ͼ�λ��� 
		
  	glClearColor(0.0f,0.7f,0.0f,0.0f); 	                //������ɫ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//�����ɫ�������Ȼ���

	glPushMatrix();
 	
    DisplayAllObject();
 	 
    glPopMatrix();
    
	SwapBuffers(hdc);

	//TextOut( hdc,350,120,"1", 1);

	bBusy = FALSE;
}

void CDisplay::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if(cy > 0)
    { 
        my_oldRect.right = cx;
        my_oldRect.bottom = cy;

        glMatrixMode(GL_MODELVIEW);//GL_PROJECTION);  // ����͸�Ӿ���
        glLoadIdentity();             // ��ʼ������
        // �ӳ�����
		glFrustum(-1.0,1.0,-1.0*cy/cx,1.0*cy/cx,5.0,9.0);  //����ռ䷶Χ
		glViewport(0, 0, cx, cy);                          //������ʾ��Χ 
     }	
}
 
void CDisplay::DisplayAllObject()
{
 	glTranslatef(0.0f,0.0f,-6.0f);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LINE_STIPPLE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
  
	glPushMatrix();
    glScalef(0.026f,0.026f,1.0f);    // ��������
	
	DrawField();
  	ShowAllPlayer();
	DrawSimBall();
	DrawBall();
    
	SetVisualizeScanResult();
	
 	glPopMatrix();
  	glFlush();
    //  glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
 
}

void CDisplay::DrawBall()
{    
  	 quadObj2 = gluNewQuadric();
     
	 glPushMatrix();
     glTranslatef( BallPosition.getX()*PITCH_DISPLAY_SCALE,
		           BallPosition.getY()*PITCH_DISPLAY_SCALE,0.0f ); // ����任
     glColor3f(1.0f,0.0f,0.0f);
 	 glLineWidth(lwidth[1]);
	 gluQuadricDrawStyle(quadObj2,GLU_LINE);
 	 gluDisk(quadObj2,0,BAll_RADIUS,40,1.0);
 	  
	 glPopMatrix();	
 	 gluDeleteQuadric(quadObj2);
}

void CDisplay::DrawSimBall()
{
     glPushMatrix();
	 glColor3f(1.0f,1.0f,1.0f);
 	 DrawCircle( ballsimulatedpos*PITCH_DISPLAY_SCALE, 3*PITCH_DISPLAY_SCALE );
	 DrawCircle( ballsimulatedpos*PITCH_DISPLAY_SCALE, 20*PITCH_DISPLAY_SCALE,20*PITCH_DISPLAY_SCALE );
     glPopMatrix();
	 glFlush();
}
void CDisplay::DrawPlayer()
{
	glPushMatrix();
    quadObj1 = gluNewQuadric();  
  
  	glLineWidth(lwidth[0]);
	gluQuadricDrawStyle(quadObj1,GLU_LINE);
 	gluDisk(quadObj1,0,ROBOT_RADIUS,40,1.0);
    
    glColor3f(0.24f,0.24f,0.29f);
 	glLineWidth(lwidth[0]*0.01);
 	gluQuadricDrawStyle(quadObj1,GLU_LINE);
 	gluDisk(quadObj1,ROBOT_RADIUS+0.1,ROBOT_RADIUS+0.1,40,1.0);
	gluDisk(quadObj1,ROBOT_RADIUS/3.5,ROBOT_RADIUS/3.5,40,1.0);
 
	glPopMatrix();
	gluDeleteQuadric(quadObj1);
}

void CDisplay::ShowAllPlayer()
{
  for( int i=1; i<=12; i++ )
  {
 	 glColor3f(0.0f,1.0f,1.0f);
     glPushMatrix();
	 
	 if( PlayerPosition[i] == UnknownDoubleValue )
	 {
		 if( i > 6 ) 
            glColor3f(1.0f,1.0f,0.0f);

  		 glTranslatef( InitialObjectPosition[i].getX()*PITCH_DISPLAY_SCALE,
	  	               InitialObjectPosition[i].getY()*PITCH_DISPLAY_SCALE,0.0f );
		 m_player_angle[i] = 0;
         
	 }
	 else
	 {
         if( i > 6 ) 
            glColor3f(1.0f,1.0f,0.0f);
		 glTranslatef( PlayerPosition[i].getX()*PITCH_DISPLAY_SCALE,
		              PlayerPosition[i].getY()*PITCH_DISPLAY_SCALE,0.0f );
	 } 
     glCallList(3);
     if( bShowPlayerNumber == TRUE  )// || PlayerPosition[i] == UnknownDoubleValue
	 {
 		wglUseFontBitmaps(wglGetCurrentDC(),0,56,1000); // 48-56 a-e
		glListBase(1000);
		glRasterPos3f(0.0f,0.0f,0.0f);
		switch(i)
		{
		 case 1:
		 case 7:
		   glCallLists(1,GL_UNSIGNED_BYTE,"1"); // 48
		   break;
		 case 2:
		 case 8: 
		   glCallLists(1,GL_UNSIGNED_BYTE,"2");
		  break;
		 case 3:
		 case 9: 
		   glCallLists(1,GL_UNSIGNED_BYTE,"3");
		  break;
		 case 4:
		 case 10: 
		   glCallLists(1,GL_UNSIGNED_BYTE,"4");
		  break;
		 case 5:
		 case 11:
		   glCallLists(1,GL_UNSIGNED_BYTE,"5");
		   break;
		 case 6:
		 case 12:
		   glCallLists(1,GL_UNSIGNED_BYTE,"6");
 		  break;
 		 default:
		  break;
		}
	 }
     glRotatef(m_player_angle[i],0.0,0.0,1.0);
	 glColor3f(1.0f,0.0f,0.0f);
     glLineWidth(lwidth[0]/10);
 	 DrawLine( Vector(0.0,0.0),Vector((ROBOT_RADIUS+0.1),0.0) );
 	 glPopMatrix();
	 glFlush();

  }
}
 
void CDisplay::DrawField()
{
    GLfloat offsetGoal = 0.2;
	// draw the goals
	glColor3f(0.0f,0.0f,1.0f);    // goal left��ɫ
	glLineWidth(lwidth[1]*0.6);
 	DrawRect( Vector(-PITCH_LENGTH_HALF,GOAL_WIDTH_HALF),
		      Vector(-(PITCH_LENGTH_HALF+GOAL_LENGTH_HALF)-0.5,-GOAL_WIDTH_HALF), false );
 	glColor3f(1.0f,1.0f,0.0f);    // goal right��ɫ
 	DrawRect( Vector(PITCH_LENGTH_HALF,GOAL_WIDTH_HALF),
		      Vector((PITCH_LENGTH_HALF+GOAL_LENGTH_HALF)+0.5,-GOAL_WIDTH_HALF), false );
 
	// draw goal outline ����������ŵ���Χ����
    glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(lwidth[1]*0.3);
 	DrawRect( Vector(-PITCH_LENGTH_HALF,GOAL_WIDTH_HALF+offsetGoal),
		      Vector(-(PITCH_LENGTH_HALF+GOAL_LENGTH_HALF)-0.5-offsetGoal,-GOAL_WIDTH_HALF-offsetGoal), false );
    DrawRect( Vector(PITCH_LENGTH_HALF,GOAL_WIDTH_HALF+offsetGoal),
		      Vector((PITCH_LENGTH_HALF+GOAL_LENGTH_HALF)+0.5+offsetGoal,-GOAL_WIDTH_HALF-offsetGoal), false );
 
    // draw middle line 
	glLineWidth(lwidth[0]*0.01);         //�ߵĴ�Сlwidth[0]	  
  	DrawLine( Vector(0.0,PTICH_WIDTH_HALF),
		      Vector(0.0,-PTICH_WIDTH_HALF) );
    
	// penalty area left
	DrawRect( Vector(-PITCH_LENGTH_HALF,PENALTY_LENGTH),
		      Vector(-(PITCH_LENGTH_HALF-PENALTY_WIDTH),-PENALTY_LENGTH), false );
    // goal area left 
 	DrawRect( Vector(-PITCH_LENGTH_HALF,GOAL_CARE_RANGE_LENGTH),
		      Vector(-(PITCH_LENGTH_HALF-GOAL_CARE_RANGE_WIDTH),-GOAL_CARE_RANGE_LENGTH), false );
   // penalty area right
	DrawRect( Vector(PITCH_LENGTH_HALF,PENALTY_LENGTH),
		      Vector(( PITCH_LENGTH_HALF-PENALTY_WIDTH ),-PENALTY_LENGTH ), false );	
   // goal area right
	DrawRect( Vector(PITCH_LENGTH_HALF,GOAL_CARE_RANGE_LENGTH),
		      Vector((PITCH_LENGTH_HALF-GOAL_CARE_RANGE_WIDTH),-GOAL_CARE_RANGE_LENGTH ), false );	
 	
	// draw outline
 	GLfloat offset = 0.2;
 	glLineWidth(lwidth[0]);           //�ߵĴ�Сlwidth[2]
	DrawRect( Vector(-PITCH_LENGTH_HALF,PTICH_WIDTH_HALF),
		      Vector(PITCH_LENGTH_HALF,-PTICH_WIDTH_HALF) );
    
	glLineWidth(lwidth[1]*0.3);
//	DrawRect( Vector(-PITCH_LENGTH_HALF-offset,PTICH_WIDTH_HALF+offset),
//		      Vector(PITCH_LENGTH_HALF+offset,-PTICH_WIDTH_HALF-offset) );

	// draw middle circle and arcs
    quadObj3 = gluNewQuadric();
	// draw field center
	glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(lwidth[0]*100);
	gluQuadricDrawStyle(quadObj3,GLU_LINE);
 	gluDisk(quadObj3,PITCH_CENTER_RADIUS,PITCH_CENTER_RADIUS,40,1.0);
 
	//  draw four arcs	 
    glTranslatef(-PITCH_LENGTH_HALF,PTICH_WIDTH_HALF,0.0f);           //λ�ñ仯(��λ)
    gluPartialDisk(quadObj3,CORNER_ARC_RADIUS,CORNER_ARC_RADIUS,40.0,1.0,90.0,90.0);
   	glTranslatef(PITCH_LENGTH_HALF*2,0,0.0f);
    gluPartialDisk(quadObj3,CORNER_ARC_RADIUS,CORNER_ARC_RADIUS,40.0,1.0,180.0,90.0);
    glTranslatef(0,-PTICH_WIDTH_HALF*2,0.0f);
    gluPartialDisk(quadObj3,CORNER_ARC_RADIUS,CORNER_ARC_RADIUS,40.0,1.0,-90.0,90.0);
    glTranslatef(-PITCH_LENGTH_HALF*2,0,0.0f);
    gluPartialDisk(quadObj3,CORNER_ARC_RADIUS,CORNER_ARC_RADIUS,40.0,1.0,0.0,90.0);
 	glTranslatef(PITCH_LENGTH_HALF,PTICH_WIDTH_HALF,0.0f);
	glPopMatrix();
	glFlush();
    gluDeleteQuadric(quadObj3);
	
	DrawPoint();
}
 	
void CDisplay::DrawRect( Vector pos1, Vector pos2, bool bRect )
{
   	glBegin(GL_LINE_STRIP);
	if( bRect == true )
	{
      glVertex2d( pos1.getX(),pos1.getY() );	
 	  glVertex2d( -pos1.getX(),pos1.getY());
 	  glVertex2d( pos2.getX(),pos2.getY());	
 	  glVertex2d( -pos2.getX(),pos2.getY());
 	  glVertex2d(  pos1.getX(),pos1.getY() );
	}
	else
	{
	  glVertex2d( pos1.getX(),pos1.getY() );	
 	  glVertex2d( pos2.getX(),-pos2.getY());
 	  glVertex2d( pos2.getX(),pos2.getY());	
 	  glVertex2d( pos1.getX(),-pos1.getY());
	}
 	glEnd(); 
}

void CDisplay::DrawLine( Vector pos1, Vector pos2 )
{
   	glBegin(GL_LINE_STRIP);
    glVertex2d( pos1.getX(),pos1.getY());	
 	glVertex2d( pos2.getX(),pos2.getY());
  	glEnd(); 
}

void CDisplay::DrawCircle( Vector pos, double radius, double inner )
{
   	glTranslated( pos.getX(),pos.getY(),0.0 );
	quadObj4 = gluNewQuadric();
	glPushMatrix();
  	glLineWidth(lwidth[0]);
 	gluQuadricDrawStyle(quadObj4,GLU_LINE);
 // glPointSize (psize[0]/100);
 // gluQuadricDrawStyle(quadObj4,GLU_POINT);
 	gluDisk(quadObj4,inner,radius,60,1.0);
    glPopMatrix();
	glFlush();
	glTranslated(-pos.getX(),-pos.getY(),0.0);
    gluDeleteQuadric(quadObj4);
}

void CDisplay::DrawPoint()
{
/*	glPointSize (psize[0]*2);      //��Ĵ�Сpsize[0]
 	glBegin(GL_POINTS);
 	glColor3f(1.0f,1.0f,1.0f);      
 	glVertex2f(0.0f,0.0f);  //���ĵ�:      
 	glVertex2f(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),0.0f); //����1:      
 	glVertex2f((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),0.0f);  //����2:  
*/
	// �����ɫΪ��ɫ:
//	glColor3f(0.0,0.0f,0.0f);  
 /*	glVertex2f(0.0f,PTICH_WIDTH_HALF/2.0f);//����3:
 	glVertex2f(0.0f,-PTICH_WIDTH_HALF/2.0f);	//����4: 
 	glVertex2f(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),PTICH_WIDTH_HALF/2.0f);   //����5:    
 	glVertex2f(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),-PTICH_WIDTH_HALF/2.0f);	//����6:      
 	glVertex2f((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),PTICH_WIDTH_HALF/2.0f);	//����7:       
 	glVertex2f((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),-PTICH_WIDTH_HALF/2.0f);	//����8: 
 	glEnd();*/
 	
	///////////////////////////////////////////////////////////////////////////
   //	CSimulatorDlg * pMainDlg=(CSimulatorDlg*)(AfxGetApp()->m_pMainWnd);
	glColor3f(1.0f,1.0f,1.0f);
	DrawCircle( Vector(0.0f,0.0f), PITCH_DISPLAY_SCALE*5 );
	DrawCircle( Vector(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),0.0f), PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),0.0f), PITCH_DISPLAY_SCALE*5 );

	glColor3f(0.0,0.0f,0.0f); 
	DrawCircle( Vector(0.0f,PTICH_WIDTH_HALF/2.0f), PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector(0.0f,-PTICH_WIDTH_HALF/2.0f), PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),PTICH_WIDTH_HALF/2.0f), 
		        PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector(-(PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),-PTICH_WIDTH_HALF/2.0f), 
		        PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),PTICH_WIDTH_HALF/2.0f), 
		        PITCH_DISPLAY_SCALE*5 );
    DrawCircle( Vector((PITCH_LENGTH_HALF-PTICH_WIDTH_HALF/2.0f),-PTICH_WIDTH_HALF/2.0f), 
		        PITCH_DISPLAY_SCALE*5 );

 	// �ҷ��Ǳ����ɫΪ: 
 	glColor3f(0.0f,0.0f,1.0f);    // ��ɫ
  	DrawCircle( Vector(-(PITCH_LENGTH_HALF+CORNER_LINE_DIST),(PTICH_WIDTH_HALF+CORNER_LINE_DIST)),//������:
		        PITCH_DISPLAY_SCALE*10 );
	DrawCircle( Vector(-(PITCH_LENGTH_HALF+CORNER_LINE_DIST),-(PTICH_WIDTH_HALF+CORNER_LINE_DIST)),//������:
		        PITCH_DISPLAY_SCALE*10 );
	// �Է��Ǳ����ɫΪ: 
  	glColor3f(1.0f,1.0f,0.0f);    // ��ɫ
    DrawCircle( Vector((PITCH_LENGTH_HALF+CORNER_LINE_DIST),(PTICH_WIDTH_HALF+CORNER_LINE_DIST)),//������: 
		        PITCH_DISPLAY_SCALE*10 );
	DrawCircle( Vector((PITCH_LENGTH_HALF+CORNER_LINE_DIST),-(PTICH_WIDTH_HALF+CORNER_LINE_DIST)),//������:
		        PITCH_DISPLAY_SCALE*10 );
}

void CDisplay::SetPixel(int xcoord, int ycoord, float r, float g, float b)        //���㺯��
{
	glPointSize (psize[0]);
    glBegin(GL_POINTS);
    glColor3f(r, g, b);
    glVertex2i(xcoord,ycoord);
    glEnd();
	glFlush();
}
 
void CDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
    if( bShowPlayerNumber == FALSE )
	    MessageBox("��ʾ��Ա����ʱ��������CPU������Դ", _T("�Ƿ���ʾ��Ա���룿"), 
		            MB_OK | MB_ICONINFORMATION);
	bShowPlayerNumber = !bShowPlayerNumber;

	CWnd::OnRButtonDown(nFlags, point);
}

void CDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	double xscale = 2.069;//3.102;
	double yscale = 2.051;//3.078;
  	Vector MousePos = Vector( (double)( (point.x - FIELD_CENTER_SCREEN_X)*xscale ),  
		                      -(double)( (point.y - FIELD_CENTER_SCREEN_Y)*yscale )  );
   
	double dist,dist2,MinDist = 10000;
	int    MoveableObject = -1;
	for( int i=5; i<=9; i++ )
	{
		if (PlayerPosition[i] == UnknownDoubleValue)
			continue;
		dist = MousePos.getDistanceTo( PlayerPosition[i]);
		if (dist < MinDist)
		{
			MinDist = dist;
            MoveableObject = i;
		}
    }
	if( BallPosition != UnknownDoubleValue ) 
	{
       dist2 = MousePos.getDistanceTo(BallPosition);
	  if ( dist2 < dist )
	  {
         MoveableObject = 0;
		 bIsMoveBall    = TRUE;
	  }
	}
	if ( MoveableObject == -1 ) 
		return;
	if ( dist2 < 100 ||  dist < 100 ) 
 	  bMoveObject = TRUE;
 	CWnd::OnLButtonDown(nFlags, point);
}
 
void CDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
 	if( bMoveObject == TRUE )
        bMoveObject = FALSE;
 	bIsMoveBall    = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}

void CDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
  	double xscale = 2.069;//3.102;
	double yscale = 2.051;//3.078;
	
	CString StrMousePos;
    CSimulatorDlg * pMainDlg=(CSimulatorDlg*)(AfxGetApp()->m_pMainWnd);
 	StrMousePos.Format("X:%.2f  Y:%.2f",(float)( (point.x - FIELD_CENTER_SCREEN_X)*xscale ),
		                 -(float)( (point.y - FIELD_CENTER_SCREEN_Y)*yscale)  );
 	CWnd *pWnd=	pMainDlg->GetDlgItem(IDC_ShowAbsPosition);
 
    
	pWnd->SetWindowText(StrMousePos);   
 	if( bMoveObject == TRUE )
	{
 	  Vector MousePos = Vector( (double)( (point.x - 350)*xscale ),  
		                        -(double)( (point.y - 250)*yscale ) ); 
	   if ( bIsMoveBall == TRUE )
	   {
 		  BallPosition = MousePos;
          pMainDlg->Server->SetBallRealGlobalPos(BallPosition);
	   }
 	   else
	   {
  		  PlayerPosition[8] = MousePos;
		  pMainDlg->Server->SetAgentRealGlobalPos(MousePos);
		  pMainDlg->Server->SetAgentRealHeading( Vector::normalizeAngle( 360*drand48() ) );
	   }
 	   Invalidate(FALSE);
	}
 	CWnd::OnMouseMove(nFlags, point);
}

void CDisplay::SetVisualizeScanResult()
{
   	
   glPointSize ( psize[0]*0.5 );       
   glBegin(GL_POINTS);
  
   glColor3f(1.0f,0.0f,0.0f);
   
/*	 for( int i=0;i < scanResults.results.size(); i++ )
	{ 
     for (int j=0; j < scanResults.results[i]->points.size();j++ ) 
	 {
        Vector poi = scanResults.results[i]->points[j];
		poi =  poi*0.1;
  	     glVertex2f(poi.X*PITCH_DISPLAY_SCALE,poi.Y*PITCH_DISPLAY_SCALE );
 	 }
	} */
	for (int k = 0;k < lines.objectlist.size();k++ )
	{
		Vector poi = lines.objectlist[k].pos;
		poi = poi * 0.1;
        glVertex2f(poi.X*PITCH_DISPLAY_SCALE,poi.Y*PITCH_DISPLAY_SCALE );
	}
  	glEnd();
	glFlush();  
 	 
     
    Frame2d FR; // ���Ҫ��ʾ����ɨ��������ת������
    FR.set_position(PlayerPosition[8]*10.0);//
    FR.set_angle(m_player_angle[8]);
   	glColor3f(1.0f,0.0f,0.0f);
 	glLineWidth(lwidth[0]*0.001);       //�ߵĴ�Сlwidth[0]	  
    
 	for( int i=0;i < scanResults->results.size(); i++ )
	{ 
		if( i==2 || i==3 )  
		{
 	 for (int j=0; j < scanResults->results[i]->points.size();j++ ) 
	 {
        
		Vector poi = scanResults->results[i]->points[j];
		poi = FR*poi*0.1;
	 	Vector pos = PlayerPosition[8];
		AngDeg dir = (poi - pos).getDirection();
		pos = pos + Vector(innerRadius*0.1,dir,POLAR);
 	    DrawLine( pos*PITCH_DISPLAY_SCALE , poi*PITCH_DISPLAY_SCALE ); 
	 }}
	}
  	glEnd();
	glFlush();
      

}
