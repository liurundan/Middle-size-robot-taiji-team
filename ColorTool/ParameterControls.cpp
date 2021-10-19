// ParameterControls.cpp : implementation file
//

#include "stdafx.h"
#include "colortool.h"
#include "ParameterControls.h"
#include "Camera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParameterControls dialog


CParameterControls::CParameterControls(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterControls::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParameterControls)
	//}}AFX_DATA_INIT
}


void CParameterControls::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParameterControls)
	DDX_Control(pDX, IDC_REDGAIN_EDIT, m_ctlRedGainEdit);
	DDX_Control(pDX, IDC_BLUEGAIN_EDIT, m_ctlBlueGainEdit);
	DDX_Control(pDX, IDC_BLUEGAIN_SLIDER, m_ctlBlueGainSlider);
	DDX_Control(pDX, IDC_REDGAIN_SLIDER, m_ctlRedGainSlider);
	DDX_Control(pDX, IDC_GAIN_SLIDER, m_ctlGainSlider);
	DDX_Control(pDX, IDC_GAIN_EDIT, m_ctlGainEdit);
	DDX_Control(pDX, IDC_EXPOSURE_SLIDER, m_ctlExpSlider);
	DDX_Control(pDX, IDC_EXPOSURE_EDIT, m_ctlExpEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParameterControls, CDialog)
	//{{AFX_MSG_MAP(CParameterControls)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EXPOSURE_EDIT, OnChangeExposureEdit)
	ON_EN_CHANGE(IDC_GAIN_EDIT, OnChangeGainEdit)
	ON_EN_CHANGE(IDC_REDGAIN_EDIT, OnChangeRedgainEdit)
	ON_EN_CHANGE(IDC_BLUEGAIN_EDIT, OnChangeBluegainEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameterControls message handlers

void CParameterControls::RefreshControls()
{
	const PdtHandle		handle = Camera::getCamera()->GetHandle();
	PdtErr				errcode;
    
	try
	{
 
		// Exposure.
		{
			errcode = PdIsParamSupported(handle, paramExposure);
			if ((errcode != errSuccess) && (errcode != errNotSupported))
				throw errcode;

			if (errcode == errSuccess)
			{
				ULONG			min, max, value;

				if ((errcode = PdGetParamRange(handle, paramExposure, &min, &max, NULL)) != errSuccess)
					throw errcode;

				if ((errcode = PdGetParam(handle, paramExposure, &value)) != errSuccess)
					throw errcode;

				m_ctlExpEdit.SetValue(value);
				m_ctlExpSlider.SetRange(min, max);
				m_ctlExpSlider.SetPos(value);

 
				m_ctlExpEdit.EnableWindow(TRUE);
				m_ctlExpSlider.EnableWindow(TRUE);
			}
			else
			{
				m_ctlExpEdit.EnableWindow(FALSE);
				m_ctlExpSlider.EnableWindow(FALSE);
			}
		}

		// Gain.
		{
			errcode = PdIsParamSupported(handle, paramGain);
			if ((errcode != errSuccess) && (errcode != errNotSupported))
				throw errcode;

			if (errcode == errSuccess)
			{
				ULONG			min, max, value;

				if ((errcode = PdGetParamRange(handle, paramGain, &min, &max, NULL)) != errSuccess)
					throw errcode;

				if ((errcode = PdGetParam(handle, paramGain, &value)) != errSuccess)
					throw errcode;

				m_ctlGainEdit.SetValue(value);
				m_ctlGainSlider.SetRange(min, max);
				m_ctlGainSlider.SetPos(value);

				m_ctlGainEdit.EnableWindow(TRUE);
				m_ctlGainSlider.EnableWindow(TRUE);
			}
			else
			{
				m_ctlGainEdit.EnableWindow(FALSE);
				m_ctlGainSlider.EnableWindow(FALSE);
			}
		}

		// red gain
		{
            errcode = PdIsParamSupported(handle, paramRedGain);
			if ((errcode != errSuccess) && (errcode != errNotSupported))
				throw errcode;

			if (errcode == errSuccess)
			{
				ULONG			min, max, value;

				if ((errcode = PdGetParamRange(handle, paramRedGain, &min, &max, NULL)) != errSuccess)
					throw errcode;

				if ((errcode = PdGetParam(handle, paramRedGain, &value)) != errSuccess)
					throw errcode;

				m_ctlRedGainEdit.SetValue(value);
				m_ctlRedGainSlider.SetRange(min, max);
				m_ctlRedGainSlider.SetPos(value);

				m_ctlRedGainEdit.EnableWindow(TRUE);
				m_ctlRedGainSlider.EnableWindow(TRUE);
			}
			else
			{
				m_ctlRedGainEdit.EnableWindow(FALSE);
				m_ctlRedGainSlider.EnableWindow(FALSE);
			}
		}

		// blue gain
		{
            errcode = PdIsParamSupported(handle, paramRedGain);
			if ((errcode != errSuccess) && (errcode != errNotSupported))
				throw errcode;

			if (errcode == errSuccess)
			{
				ULONG			min, max, value;

				if ((errcode = PdGetParamRange(handle, paramBlueGain, &min, &max, NULL)) != errSuccess)
					throw errcode;

				if ((errcode = PdGetParam(handle, paramBlueGain, &value)) != errSuccess)
					throw errcode;

				m_ctlBlueGainEdit.SetValue(value);
				m_ctlBlueGainSlider.SetRange(min, max);
				m_ctlBlueGainSlider.SetPos(value);

				m_ctlBlueGainEdit.EnableWindow(TRUE);
				m_ctlBlueGainSlider.EnableWindow(TRUE);
			}
			else
			{
				m_ctlBlueGainEdit.EnableWindow(FALSE);
				m_ctlBlueGainSlider.EnableWindow(FALSE);
			}
		}

	}
	catch (PdtErr e)
	{
        Camera::getCamera()->ReportError(e);
	}

}

void CParameterControls::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	const PdtHandle		handle = Camera::getCamera()->GetHandle();
	PdtErr				errcode;


	// Exposure
	if (pScrollBar == (CScrollBar*)&m_ctlExpSlider)
	{
		const ULONG value = (ULONG) m_ctlExpSlider.GetPos();

		m_ctlExpEdit.SetValue(value);

//		if (nSBCode == SB_ENDSCROLL) // Final position
		{
			if ((errcode = PdSetParam(handle, paramExposure, value)) != errSuccess)
				Camera::getCamera()->ReportError(errcode);

		}
	}

	// Gain
	if (pScrollBar == (CScrollBar*)&m_ctlGainSlider)
	{
		const ULONG value = (ULONG) m_ctlGainSlider.GetPos();

		m_ctlGainEdit.SetValue(value);

		if (nSBCode == SB_ENDSCROLL) // Final position
		{
			if ((errcode = PdSetParam(handle, paramGain, value)) != errSuccess)
				Camera::getCamera()->ReportError(errcode);
		}
	}
 		// Red Gain
	if (pScrollBar == (CScrollBar*)&m_ctlRedGainSlider)
	{
		const ULONG value = (ULONG) m_ctlRedGainSlider.GetPos();

		m_ctlRedGainEdit.SetValue(value);

		if (nSBCode == SB_ENDSCROLL) // Final position
		{
			ULONG  WBmode;
			errcode = PdGetParam(handle, paramWhitebalanceMode,&WBmode );
			if( errcode == errSuccess )
			{
				if( WBmode != whitebalanceManual )
				{
				    errcode = PdSetParam(handle, paramWhitebalanceMode, whitebalanceManual );
				    if(errcode != errSuccess)
			 	       Camera::getCamera()->ReportError(errcode);
				}
			}
			else
			Camera::getCamera()->ReportError(errcode);
			
 			if ((errcode = PdSetParam(handle, paramRedGain, value)) != errSuccess)
				Camera::getCamera()->ReportError(errcode);
		}
	}
	
	// Blue Gain
	if (pScrollBar == (CScrollBar*)&m_ctlBlueGainSlider)
	{
		const ULONG value = (ULONG) m_ctlBlueGainSlider.GetPos();

		m_ctlBlueGainEdit.SetValue(value);

		if (nSBCode == SB_ENDSCROLL) // Final position
		{
			ULONG  WBmode;
			errcode = PdGetParam(handle, paramWhitebalanceMode,&WBmode );
			if( errcode == errSuccess )
			{
				if( WBmode != whitebalanceManual )
				{
				    errcode = PdSetParam(handle, paramWhitebalanceMode, whitebalanceManual );
				    if(errcode != errSuccess)
			 	       Camera::getCamera()->ReportError(errcode);
				}
			}
			else
			Camera::getCamera()->ReportError(errcode);
 			
			if ((errcode = PdSetParam(handle, paramBlueGain, value)) != errSuccess)
				Camera::getCamera()->ReportError(errcode);
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CParameterControls::OnChangeExposureEdit() 
{
    const PdtHandle		handle = Camera::getCamera()->GetHandle();
	const ULONG			value = m_ctlExpEdit.GetValue();
	PdtErr				errcode;


	m_ctlExpSlider.SetPos((int)value);


	if ((errcode = PdSetParam(handle, paramExposure, value)) != errSuccess)
		Camera::getCamera()->ReportError(errcode);
	
}

void CParameterControls::OnChangeGainEdit() 
{
	const PdtHandle		handle = Camera::getCamera()->GetHandle();
	const ULONG			value = m_ctlGainEdit.GetValue();
	PdtErr				errcode;


	m_ctlGainSlider.SetPos((int)value);

	if ((errcode = PdSetParam(handle, paramGain, value)) != errSuccess)
		Camera::getCamera()->ReportError(errcode);
	
}

void CParameterControls::OnChangeRedgainEdit() 
{
	const PdtHandle		handle = Camera::getCamera()->GetHandle();
	const ULONG			value = m_ctlRedGainEdit.GetValue();
	PdtErr				errcode;
    ULONG  WBmode;
    
	errcode = PdGetParam(handle, paramWhitebalanceMode,&WBmode );
	if( errcode == errSuccess )
	{
       if( WBmode != whitebalanceManual )
	   {
         errcode = PdSetParam(handle, paramWhitebalanceMode, whitebalanceManual );
         if(errcode != errSuccess)
           Camera::getCamera()->ReportError(errcode);
	   }
	}
	else
      Camera::getCamera()->ReportError(errcode);
 
 	m_ctlRedGainSlider.SetPos((int)value);

	if ((errcode = PdSetParam(handle, paramRedGain, value)) != errSuccess)
		Camera::getCamera()->ReportError(errcode);	
}

void CParameterControls::OnChangeBluegainEdit() 
{
	const PdtHandle		handle = Camera::getCamera()->GetHandle();
	const ULONG			value  = m_ctlBlueGainEdit.GetValue();
	PdtErr				errcode;
    
	ULONG  WBmode;
    errcode = PdGetParam(handle, paramWhitebalanceMode,&WBmode );
	if( errcode == errSuccess )
	{
       if( WBmode != whitebalanceManual )
	   {
         errcode = PdSetParam(handle, paramWhitebalanceMode, whitebalanceManual );
         if(errcode != errSuccess)
           Camera::getCamera()->ReportError(errcode);
	   }
	}
	else
      Camera::getCamera()->ReportError(errcode);
	
	m_ctlBlueGainSlider.SetPos((int)value);

	if ((errcode = PdSetParam(handle, paramBlueGain, value)) != errSuccess)
		Camera::getCamera()->ReportError(errcode);	
}
