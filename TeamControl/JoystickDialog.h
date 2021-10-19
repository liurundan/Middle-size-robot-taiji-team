#if !defined(AFX_JOYSTICKDIALOG_H__6BA37D1C_B57A_44DA_AE44_ED13B72CFDF1__INCLUDED_)
#define AFX_JOYSTICKDIALOG_H__6BA37D1C_B57A_44DA_AE44_ED13B72CFDF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JoystickDialog.h : header file
//
#include "DriveVector.h"
#include "math.h"

#define		closejoydialog					WM_USER+111

/////////////////////////////////////////////////////////////////////////////
// JoystickDialog dialog

class JoystickDialog : public CDialog
{
// Construction
public:
	JoystickDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~JoystickDialog();
	
    void getDriveVectorFromJoystick( DriveVector &drv );
    void setVelocity(double rvx, double rvy, double rw,bool bk = false, BYTE power = 0 );
	void accelerate();
	void decelerate();
// Dialog Data
	//{{AFX_DATA(JoystickDialog)
	enum { IDD = IDD_JOYSTICKDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	enum motionstate
	{
 		moveStraightUp,
		moveBackward,
		moveLeft,
		moveRight,
		rotate,
		stop,
		start
	};   
	motionstate m_robotmotion;
	DriveVector dest;
    double  vrot;
	double  vel;

	double  rot_step;
	double  vel_step;
	double  max_vel;
	double  max_vrot;
	double  vel_ini;
	double  rot_ini;
    BYTE    kick_power;
	bool    bkick;
	
	CPen    Pen;
	CBrush  Brush;
	CDC     dcMem;
	CBitmap bm;
	CRect   rc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JoystickDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
	//{{AFX_MSG(JoystickDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYSTICKDIALOG_H__6BA37D1C_B57A_44DA_AE44_ED13B72CFDF1__INCLUDED_)
