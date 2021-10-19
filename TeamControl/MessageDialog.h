#if !defined(AFX_MESSAGEDIALOG_H__F775E8B7_33A1_47B7_BF6D_BC9A4AA60C79__INCLUDED_)
#define AFX_MESSAGEDIALOG_H__F775E8B7_33A1_47B7_BF6D_BC9A4AA60C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageDialog.h : header file
//
#define		closeMsgdialog					WM_USER+115
/////////////////////////////////////////////////////////////////////////////
// MessageDialog dialog

class MessageDialog : public CDialog
{
	DECLARE_DYNAMIC(MessageDialog)
// Construction
public:
	MessageDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MessageDialog();
    void SetMessage(CString s);
// Dialog Data
	//{{AFX_DATA(MessageDialog)
	enum { IDD = IDD_MESSAGE_BOARD_DIALOG };
	CStatic	ctrl_out_msg;
	CString	out_msg;
	//}}AFX_DATA

	CWnd*   m_parent;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnCancel();
	// Generated message map functions
	//{{AFX_MSG(MessageDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEDIALOG_H__F775E8B7_33A1_47B7_BF6D_BC9A4AA60C79__INCLUDED_)
