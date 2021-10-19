#include "stdafx.h"
#include "Camera.h"

Camera* Camera::m_camera = NULL;

Camera::Camera( ) : ImageSource()
{
	m_handle = 0;
    
	for (register int i = 0; i < arraylen(m_frames); i++)
		m_frames[i].Buffer = 0;
	
	m_aborting = false;
	isStarted  = false;
    isValid    = false;
	
	m_width  = 0;
	m_height = 0;
	
}

Camera::~Camera()
{
	ASSERT(m_handle == 0);
	
	for (register int i = 0; i < arraylen(m_frames); i++)
	{
		ASSERT(m_frames[i].Buffer == 0);
	}
}


bool Camera::OpenCamera()
{
	PdtCamListItem		item;
    isValid = false;
	if (m_handle)
	{
        PdCloseCamera(m_handle);
		m_handle = 0;
	}
    
	if (PdListCameras(&item, 1, NULL) >= 1)
	{
		PdtErr			errcode;
		
		if ((errcode = PdOpenCamera(item.UniqueId, &m_handle)) == errSuccess)
		{
			// StartImaging();
			isValid = true;
			return isValid;
		}
		else
			ReportError(errcode);
	}
	else
	{
		AfxMessageBox("尚未连接摄像机,检查设备连线! ",MB_OK | MB_ICONINFORMATION);
		exit(1);
		return false;
	}
	return false;
}


void Camera::CloseCamera()
{
	if (m_handle)
	{
		StopImaging();
		PdCloseCamera(m_handle);
		m_handle = 0;
	}
}


void Camera::StartImaging(int framerate /* = 30 */,int format /* = fmtYuv422 */)
{
	PdtCaptureMode		captureMode;
	unsigned long		frameSize;
	PdtErr				errcode;
	
	try
	{
		// Make sure the camera is idle.
		errcode = PdGetCaptureMode(m_handle, &captureMode);
		if (errcode != errSuccess)
			throw errcode;
		ASSERT(captureMode == modeIdle);
		
		// Get Camera information
        errcode = PdSetRegion( m_handle,0,0,640,480 );
		if (errcode != errSuccess)
			throw errcode;
        errcode =  PdSetRateDivider( m_handle,0 );
        if (errcode != errSuccess)
			throw errcode;
		
		errcode = PdSetFormat(m_handle,format);//fmtRgb24
		if (errcode != errSuccess)
			throw errcode;
		
		// Set new capture mode.
		errcode = PdSetCaptureMode(m_handle, modeStreaming);
		if (errcode != errSuccess)
			throw errcode;
        
		bool success = setFramerate(framerate);
		if( !success ) 
		{
			errcode = errFault;
			throw   errcode;
		}
		// Allocate frame buffers.
		errcode = PdGetFrameSize(m_handle, &frameSize, NULL, NULL);
		if (errcode != errSuccess)
			throw errcode;
		
		for (register int i = 0; i < arraylen(m_frames); i++)
		{
			ASSERT(m_frames[i].Buffer == 0);
			
			m_frames[i].BufferSize = frameSize;
			m_frames[i].Buffer = new BYTE[frameSize];
			m_frames[i].Context[0] = this;
			
			errcode = PdQueueFrame(m_handle, &m_frames[i], frameDoneCallback);
			if (errcode != errSuccess)
				throw errcode;
		} 
		
		
		isStarted = true;
	}
	catch( PdtErr errcode )
	{
		ReportError(errcode);
	}
	
}

void Camera::StopImaging()
{
	m_aborting = true;
	PdAbortFrames(m_handle);
	m_aborting = false;
	
	PdSetCaptureMode(m_handle, modeIdle);
	
	for (register int i = 0; i < arraylen(m_frames); i++)
	{
		delete [] m_frames[i].Buffer;
		m_frames[i].Buffer = 0;
	}
	
}

Camera* Camera::getCamera(int framerate /* = 30 */,int format /* = fmtYuv422 */)
{
	if (m_camera == NULL )
	{  
		m_camera = new Camera();
		if( m_camera->OpenCamera() )
		{
			m_camera->StartImaging(framerate,format);
		}
		
	}
	return m_camera;
} 

void Camera::destroyCamera()
{
	if (m_camera)
	{
		m_camera->CloseCamera();
		delete m_camera;
	}
}

void Camera::ReportError(PdtErr errcode)
{
	CString	s;
	s.Format("Camera Error code: %u", errcode);
	AfxMessageBox(s);
	exit(1);
}

void __stdcall Camera::frameDoneCallback(PdtFrame* pFrame)
{
	Camera*				pThis = (Camera*) pFrame->Context[0];
	
	if (pThis->m_aborting)
		return;
	
	if (pFrame->Status == errSuccess)
	{
		pThis->CopyFrame(pFrame);
	}
	else if (pFrame->Status != errCancelled)
	{
		pThis->ReportError(pFrame->Status);
	}
	
	if (pFrame->Status == errSuccess)
	{
		PdtErr			errcode;
		
		errcode = PdQueueFrame(pThis->m_handle, pFrame, frameDoneCallback);
		
		if ((errcode != errSuccess) && (errcode != errCancelled))
			pThis->ReportError(errcode);
	}
} 

void  Camera::CopyFrame(PdtFrame* pFrame)
{
   	int              outputFormat;
	double           bytePP;
    
	if(m_aborting)
		return;
	
    format   = pFrame->Format;
    m_width  = pFrame->Width;
	m_height = pFrame->Height;
	
    const char*			pSrc = (char*)pFrame->Buffer;
	
	switch(format) 
	{
	case fmtYuv422:
		outputFormat = ImageBuffer::FORMAT_YUV422; bytePP = 2.; break;
	case fmtRgb24:
		outputFormat = ImageBuffer::FORMAT_RGB; bytePP = 3.; break;
	case fmtMono8:
		outputFormat = ImageBuffer::FORMAT_MONO; bytePP = 1.; break;
	default:
		outputFormat = ImageBuffer::FORMAT_MONO16; bytePP = 2.; break;
    }
	
    m_buffer = ImageBuffer(m_width,m_height,outputFormat,(unsigned char*)pSrc,
		(int)(m_width*m_height*bytePP) );
	
}

ImageBuffer Camera::getImage() throw(HardwareException)
{
	return          m_buffer;
}

bool Camera::setFramerate(int fps)
{
	PdtErr				errcode;
	unsigned long framerate = 10;
	framerate = fps * 1e3;
	
	unsigned long MaxRate,MinRate,value;
	errcode = PdGetParamRange(m_handle,paramFrameRate,&MinRate,&MaxRate,&value);
	if (errcode != errSuccess)
		return false;
	
	errcode = PdSetParam(m_handle,paramFrameRate,framerate); // f/1000s 30fps*1000 
	if (errcode != errSuccess)
		return false;
    return true;
}

int	Camera::getWidth() const
{
	return m_width;
}

int	Camera::getHeight() const
{
	return m_height;
}




