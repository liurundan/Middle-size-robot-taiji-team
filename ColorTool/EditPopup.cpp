 
//===== INCLUDE FILES =========================================================

#include "stdafx.h"
#include "EditPopup.h"


//===== #DEFINES ==============================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_EDIT					1000


//===== TYPE DEFINITIONS ======================================================

// The pop up dialog used to change the CEdit value.
class CEditPopupDlg : public CDialog
{
public:
	CEditPopupDlg() : CDialog() { };

	int DoModal(CWnd* pParent, CRect editRect, CString text, DWORD sel);

	CString				result;		// Edit box string, when IDOK returned

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:

	virtual int DoModal() { return CDialog::DoModal(); }

	//{{AFX_DATA(CEditPopupDlg)
	CEdit				m_ctlEdit;
	CButton				m_ctlOk;
	CButton				m_ctlCancel;
	//}}AFX_DATA

	// Generated message map functions
	//{{AFX_MSG(CEditPopupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CRect				m_editRect;		// Screen location of edit box
	CString				m_text;			// Initial text and selection
	DWORD				m_sel;
};


//===== FUNCTION PROTOTYPES ===================================================

//===== DATA (PUBLIC) =========================================================

//===== DATA (PRIVATE) ========================================================

//===== IMPLEMENTATION ========================================================

//----- CEditPopupDlg ---------------------------------------------------------

void CEditPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CEditPopupDlg)
	DDX_Control(pDX, IDC_EDIT, m_ctlEdit);
	DDX_Control(pDX, IDOK, m_ctlOk);
	DDX_Control(pDX, IDCANCEL, m_ctlCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CEditPopupDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CEditPopupDlg::DoModal
(
	CWnd*				pParent,
	CRect				editRect,
	CString				text,
	DWORD				sel
)
{
#pragma pack(push)
#pragma pack(2)
	struct tDialogTemplate
	{
		DLGTEMPLATE			dlgTemplate;
		WORD				dlgMenuArray[1];
		WORD				dlgClassArray[1];
		WORD				dlgTitleArray[1];
		DLGITEMTEMPLATE		editTemplate;
		WORD				editClassArray[2];
		WORD				editTitleArray[1];
		WORD				editDataArray[1];
		WORD				_reserved1;
		DLGITEMTEMPLATE		okTemplate;
		WORD				okClassArray[2];
		WORD				okTitleArray[1];
		WORD				okDataArray[1];
		WORD				_reserved2;
		DLGITEMTEMPLATE		cancelTemplate;
		WORD				cancelClassArray[2];
		WORD				cancelTitleArray[1];
		WORD				cancelDataArray[1];
	};
#pragma pack(pop)

	tDialogTemplate			dtl;

	memset(&dtl, 0, sizeof(dtl));

	dtl.dlgTemplate.style			= WS_POPUP | WS_DLGFRAME;
	dtl.dlgTemplate.dwExtendedStyle	= 0;
	dtl.dlgTemplate.cdit			= 3;
	dtl.dlgTemplate.x				= 0;
	dtl.dlgTemplate.y				= 0;
	dtl.dlgTemplate.cx				= 1;
	dtl.dlgTemplate.cy				= 1;

	dtl.editTemplate.style			= WS_VISIBLE | WS_BORDER | WS_TABSTOP;
	dtl.editTemplate.dwExtendedStyle= 0;
	dtl.editTemplate.x				= 0;
	dtl.editTemplate.y				= 0;
	dtl.editTemplate.cx				= 1;
	dtl.editTemplate.cy				= 1;
	dtl.editTemplate.id				= IDC_EDIT;
	dtl.editClassArray[0]			= 0xFFFF;
	dtl.editClassArray[1]			= 0x0081;	// Edit

	dtl.okTemplate.style			= WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP;
	dtl.okTemplate.dwExtendedStyle	= 0;
	dtl.okTemplate.x				= 0;
	dtl.okTemplate.y				= 0;
	dtl.okTemplate.cx				= 1;
	dtl.okTemplate.cy				= 1;
	dtl.okTemplate.id				= IDOK;
	dtl.okClassArray[0]				= 0xFFFF;
	dtl.okClassArray[1]				= 0x0080;	// Button

	dtl.cancelTemplate.style		= WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP;
	dtl.cancelTemplate.dwExtendedStyle= 0;
	dtl.cancelTemplate.x			= 0;
	dtl.cancelTemplate.y			= 0;
	dtl.cancelTemplate.cx			= 1;
	dtl.cancelTemplate.cy			= 1;
	dtl.cancelTemplate.id			= IDCANCEL;
	dtl.cancelClassArray[0]			= 0xFFFF;
	dtl.cancelClassArray[1]			= 0x0080;	// Button

	BOOL result = InitModalIndirect(&dtl, pParent);
	ASSERT(result);

	m_editRect = editRect;
	m_text = text;
	m_sel = sel;

	return DoModal();
}


BOOL CEditPopupDlg::OnInitDialog() 
{
	const int			dlgBorderX = GetSystemMetrics(SM_CXDLGFRAME);
	const int			dlgBorderY = GetSystemMetrics(SM_CYDLGFRAME);
	const int			buttonWidth = 40;
	const int			buttonHeight = 16;
	const int			border = 8;
	int					width;


	CDialog::OnInitDialog();

	//
	// Here is where we size and position our dialog.  (We do it here rather
	// than above because the units are pixels here.)
	//
	// We want the edit box for this dialog to be the same size and position
	// as the CEditPopup control's box.
	//

	width = buttonWidth*2 + border;
	if (m_editRect.Width() > width)
		width = m_editRect.Width();
	
	SetWindowPos(&wndTop,
				 m_editRect.left - border - dlgBorderX,
				 m_editRect.top - border - dlgBorderY,
				 width + border*2 + dlgBorderX*2,
				 m_editRect.Height() + buttonHeight + border*3 + dlgBorderY*2,
				 SWP_NOZORDER | SWP_SHOWWINDOW);

	m_ctlEdit.SetWindowPos(&wndTop,
						   border,
						   border,
						   m_editRect.Width(),
						   m_editRect.Height(),
						   SWP_NOZORDER | SWP_SHOWWINDOW);

	m_ctlCancel.SetWindowText("Cancel");
	m_ctlCancel.SetWindowPos(&wndTop,
							 border,
							 m_editRect.Height() + border*2,
							 buttonWidth,
							 buttonHeight,
							 SWP_NOZORDER | SWP_SHOWWINDOW);

	m_ctlOk.SetWindowText("OK");
	m_ctlOk.SetWindowPos(&wndTop,
						 border*2 + buttonWidth,
						 m_editRect.Height() + border*2,
						 buttonWidth,
						 buttonHeight,
						 SWP_NOZORDER | SWP_SHOWWINDOW);

	//
	// Set text and selection.
	//
	m_ctlEdit.SetFocus();
	m_ctlEdit.SetWindowText(m_text);
	m_ctlEdit.SetSel(m_sel);

	return FALSE;  // We set the focus
}


void CEditPopupDlg::OnOk()
{
	m_ctlEdit.GetWindowText(result);

	EndDialog(IDOK);
}


//----- CEditPopup ------------------------------------------------------------

CEditPopup::CEditPopup()
{
	m_userUpdate = false;
	m_allowEnChange = false;
}

CEditPopup::~CEditPopup()
{
}


BEGIN_MESSAGE_MAP(CEditPopup, CEdit)
	//{{AFX_MSG_MAP(CEditPopup)
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChangeReflect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CEditPopup::runDlg(DWORD sel)
{
	CRect				screenRect;
	CString				text;
	CEditPopupDlg		dlg;


	GetWindowRect(&screenRect);
	GetWindowText(text);

	// Run the dialog...
	if (dlg.DoModal(this, screenRect, text, sel) == IDOK)
	{
		// Is the value valid?
		if (setValstr(dlg.result) == true)
		{
			// Notify parent with EN_CHANGE message

			CWnd* pParent = GetParent();

			if (pParent)
			{
				m_allowEnChange = true;
				pParent->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE),
									 (LPARAM)m_hWnd);
			}
		}
	}

	// Always refresh with the latest value.
	refreshValue();
}


void CEditPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonUp(nFlags, point);


	if (!(GetStyle() & ES_READONLY))
		runDlg(-1<<16);
}


void CEditPopup::OnUpdate()
{
	if (m_userUpdate)
		runDlg(GetSel());
}


BOOL CEditPopup::OnChangeReflect()
{
	// Return TRUE to block message to parent
	BOOL result = m_allowEnChange ? FALSE : TRUE;

	m_allowEnChange = false;

	return result;
}


void CEditPopup::refreshValue()
{
	CString s = getValstr();

	m_userUpdate = false;	// No dialog when we SetWindowText()
	SetWindowText(s);
	m_userUpdate = true;
}


//-----	CEditPopup derived classes --------------------------------------------

void CEditPopupUlong::SetValue(ULONG value)
{
	m_value = value;
	refreshValue();
}


CString CEditPopupUlong::getValstr()
{
	CString				s;

	s.Format("%u", m_value);

	return s;
}


bool CEditPopupUlong::setValstr(CString s)
{
	return (sscanf(s, "%u", &m_value) == 1);
}


void CEditPopupDouble::SetValue(double value)
{
	m_value = value;
	refreshValue();
}


CString CEditPopupDouble::getValstr()
{
	CString				s;

	s.Format("%lf", m_value);

	return s;
}


bool CEditPopupDouble::setValstr(CString s)
{
	return (sscanf(s, "%lf", &m_value) == 1);
}


void CEditPopupTime::SetValue(ULONG value)
{
	m_value = value;
	refreshValue();
}


CString CEditPopupTime::getValstr()
{
	CString				s;

	s.Format("%lf", m_value);

	if (m_value < 1000)
		s.Format("%u us", m_value);
	else if (m_value < 1000000)
		s.Format("%.4g ms", (double)m_value / 1000.0F);
	else
		s.Format("%.4g s", (double)m_value / 1000000.0F);

	return s;
}


bool CEditPopupTime::setValstr(CString s)
{
	CString				number;
	CString				units;
	double				asFloat;


	s.TrimLeft();
	s.TrimRight();

	number = s.SpanIncluding("0123456789.");

	units = s.Right(s.GetLength() - number.GetLength());
	units.TrimLeft();

	if (sscanf(number, "%lf", &asFloat) != 1)
		return false;

	if (units.IsEmpty() || (units == "u") || (units == "us") || (units == "usec"))
	{
	}
	else if ((units == "m") || (units == "ms") || (units == "msec"))
	{
		asFloat *= 1000.0F;
	}
	else if ((units == "s") || (units == "sec"))
	{
		asFloat *= 1000000.0F;
	}
	else
		return false;

	m_value = (ULONG)(asFloat + 0.5F);
	return true;
}

