#ifndef USBCAMER_H_INCLUDE
#define USBCAMER_H_INCLUDE

#include <windows.h>
#include <dshow.h>
#include <atlbase.h>
#include <qedit.h>
#include "Image.h"
#include "RGBImage.h"
#include "ImageSource.h"

#define WIN32_LEAN_AND_MEAN

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x ) \
if ( NULL != x ) \
{ \
    x->Release( ); \
    x = NULL; \
}
#endif

class UsbCamera : public ImageSource
{
public:
	UsbCamera();
	virtual ~UsbCamera();
	
    virtual ImageBuffer getImage();
	virtual int			getWidth() const;
	virtual int			getHeight() const;

	static UsbCamera*	getCamera(  int port = 0, int framerate = 30, int width = 320, 
									int height = 240, CString mode = "" );
	static void			destroyUsbCamera();

	void				Init(	int deviceId, bool displayProperties = false, int framerate = 30,
								int iw = 320, int ih = 240, CString mode = "" );
	void				DisplayFilterProperties();
	
	bool				BindFilter(int deviceId, IBaseFilter **pFilter);
	void				SetCrossBar( int fr = 30, int iiw = 320, int iih = 240, CString mode = "" );
	HRESULT				GrabByteFrame();
	long				GetBufferSize()		{ return bufferSize; }
	long*				GetBuffer()			{ return pBuffer;    }
	BYTE*				GetByteBuffer()		{ return pBYTEbuffer;}

public:
	bool				    bisValid;
	
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

	static UsbCamera*		m_camera;
	ImageBuffer				imagebuf;
    
	double                  bytePP;
private:
 	void					ErrMsg(LPTSTR szFormat,...);
    void					FreeMediaType(AM_MEDIA_TYPE& mt);

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