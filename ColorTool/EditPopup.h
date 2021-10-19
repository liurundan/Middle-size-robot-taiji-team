#if !defined(AFX_EDITPOPUP_H__FB727470_D84F_4441_B03E_5B4334F0EBFE__INCLUDED_)
#define AFX_EDITPOPUP_H__FB727470_D84F_4441_B03E_5B4334F0EBFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPopup.h : header file
//

class CEditPopup : public CEdit
{
public:
	CEditPopup();
	virtual ~CEditPopup();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditPopup)
	//}}AFX_VIRTUAL

protected:
	void refreshValue();

private:
	void SetWindowText(LPCTSTR lpszString)  { CWnd::SetWindowText(lpszString); }

	// Generated message map functions
	//{{AFX_MSG(CEditPopup)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdate();
	afx_msg BOOL OnChangeReflect();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	bool				m_userUpdate;
	bool				m_allowEnChange;

	void runDlg(DWORD);

	virtual CString getValstr() = 0;
	virtual bool setValstr(CString s) = 0;
};


class CEditPopupUlong : public CEditPopup
{
public:
	CEditPopupUlong() : CEditPopup() { }

	void SetValue(ULONG value);
	ULONG GetValue()						{ return m_value; }

private:
	CString getValstr();
	bool setValstr(CString s);
	ULONG				m_value;
};


class CEditPopupDouble : public CEditPopup
{
public:
	CEditPopupDouble() : CEditPopup() { }

	void SetValue(double value);
	double GetValue()						{ return m_value; }

private:
	CString getValstr();
	bool setValstr(CString s);
	double				m_value;
};


class CEditPopupTime : public CEditPopup
{
public:
	CEditPopupTime() : CEditPopup() { }

	void SetValue(ULONG value); // In microseconds
	ULONG GetValue()						{ return m_value; }

private:
	CString getValstr();
	bool setValstr(CString s);
	ULONG				m_value;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITPOPUP_H__FB727470_D84F_4441_B03E_5B4334F0EBFE__INCLUDED_)
