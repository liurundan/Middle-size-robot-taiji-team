#ifndef _FrameGrabber_H_
#define _FrameGrabber_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dshow.h>
#include <atlbase.h>
#include <qedit.h>

#include "Image.h"
#include "RGBImage.h"
#include "ImageSource.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x ) \
if ( NULL != x ) \
{ \
    x->Release( ); \
    x = NULL; \
}
#endif

class SmartString
{
public:
	SmartString():str(NULL) {}
	SmartString(char* pStr):str(NULL)
	{
		if (pStr)
		{
			int size = int(strlen(pStr));
			str = new char[size+1];
			strcpy(str, pStr);
		}
	}
	
	SmartString(SmartString& sStr)
	{
		SetString(sStr.GetBuffer());
	}
	
	~SmartString()
	{
		if (str) delete[] str;
	}
	
	SmartString& operator =(char* pStr)
	{
		SetString(pStr);
		return *this;
	}
	SmartString& operator =(SmartString& sStr)
	{
		SetString(sStr.GetBuffer());
		return *this;
	}
	char* GetBuffer() {return str;}
	
protected:
	void SetString(char *pStr)
	{
		if (str) delete[] str;
		if (!pStr)
			str = NULL;
		else
		{
			int size = int(strlen(pStr));
			str = new char[size + 1];
			strcpy(str, pStr);
		}
	}
	char* str;
};

class DeviceInfo
{
public:
	DeviceInfo():next(NULL), deviceId(-1)
	{;} 
	~DeviceInfo()
	{
		if (next)	delete next;
	}
	SmartString friendlyName;
	int			deviceId;	
	DeviceInfo* next;
};
  
class  FrameGrabber  
{
public:
	FrameGrabber();
	virtual ~FrameGrabber();
	
	void			Init(	int deviceId, bool displayProperties = false, int framerate = 30,
							int iw = 320, int ih = 240, CString mode = "" );
	void			EnumDevices(DeviceInfo *head);
	void			DisplayFilterProperties();
	bool			BindFilter(int deviceId, IBaseFilter **pFilter);
	void			SetCrossBar( int fr = 30, int iiw = 320, int iih = 240, CString mode = "" );
 
	HRESULT			GrabByteFrame();
	long			GetBufferSize() { return bufferSize; }
	long*			GetBuffer()     { return pBuffer;    }
	BYTE*			GetByteBuffer() { return pBYTEbuffer;}
 	
    int				GetWidth() const;
    int				GetHeight() const;
    ImageBuffer		getImage();
	
 protected:
	IGraphBuilder*			pGraph;
	IBaseFilter*			pDeviceFilter;
	IMediaControl*			pMediaControl;
	IBaseFilter*			pSampleGrabberFilter;
	ISampleGrabber*			pSampleGrabber;
	IPin*					pGrabberInput;
	IPin*					pGrabberOutput;
	IPin*					pCameraOutput;
	IMediaEvent*			pMediaEvent;
	IBaseFilter*			pNullFilter;
	IPin*					pNullInputPin;
  	ICaptureGraphBuilder2*  pBuilder;

private:
 	void		ErrMsg(LPTSTR szFormat,...);

    void		FreeMediaType(AM_MEDIA_TYPE& mt);
	bool		flipImage;
	long		bufferSize;
	long*		pBuffer;
	BYTE*		pBYTEbuffer;
    bool		connected;
    int			width;
    int			height;
	ImageBuffer m_buffer;
    bool		bnotify;
	CString     format_mode;
};
#endif 
