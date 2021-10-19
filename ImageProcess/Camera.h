#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE
  
#include <PdApi.h>
#include "ImageSource.h"

#define arraylen(x)					(sizeof(x)/sizeof(x[0]))
   
class Camera : public ImageSource
{
public:

	Camera();
	~Camera();
 
	bool				OpenCamera();
	void				CloseCamera();

	void				StartImaging(int framerate = 30,int format = fmtYuv422);
	void				StopImaging();
  
	void				ReportError(PdtErr);

 	void                CopyFrame(PdtFrame* pFrame);

      
    virtual ImageBuffer getImage() throw(HardwareException);
 	virtual int			getWidth() const; 
    virtual int			getHeight() const;
	virtual bool		setFramerate(int fps);
    
	static  Camera*     getCamera(int framerate = 30,int format = fmtYuv422);      
    static  void        destroyCamera();
public:  
	bool				isValid;                 

protected:
 	bool                isStarted;
    
	int					firstFrame;              
    int					failure;                 
    int					framerate;               
    int         		format;                  

    ULONG				m_width;
	ULONG				m_height;
    ImageBuffer         m_buffer;
    static Camera*      m_camera;
    
private:

 	PdtHandle			m_handle;

	PdtFrame			m_frames[2];

	bool				m_aborting;
   
	
	static void __stdcall frameDoneCallback(PdtFrame* pFrame);

};
 
#endif 

