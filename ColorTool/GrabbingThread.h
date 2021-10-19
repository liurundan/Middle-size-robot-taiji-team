#ifndef _GRABBINGTHREAD_H_
#define _GRABBINGTHREAD_H_
#include <afxmt.h>

class Image;
class MultiImageProducer;
class ColorClassInfoList;
class HSISegmentationTool;
  
class GrabbingThread
{
	
public:
	GrabbingThread( MultiImageProducer* producer);
	~GrabbingThread();
  
    BOOL                    Start();
	int					    getFrameCounter() const { return frameCounter; }
    Image&					getImage()            {  return *bufferedImage; }
	HSISegmentationTool*	getSegmentationTool() { return segmentTool; }
	void					showOriginalImage(bool show);
    void                    ReceiveImage();
	
	BOOL					StopReceiveImage();
	BOOL					RestartReceive();
	CCriticalSection        m_cs;

protected:
 	static UINT				run(LPVOID pParam);  
	
protected: 
	MultiImageProducer*			producer;
	Image*					image;
	Image*					bufferedImage;
	HSISegmentationTool*	segmentTool;
	ColorClassInfoList*		colorInfos; 
	bool					showOrig;
	int						frameCounter;
    
	CWinThread*             m_Thread;
    HANDLE				    m_hShutdownEvent;
    HANDLE                  m_hReadimageEvent;
	HANDLE				    m_hEventArray[2];

    BOOL				    m_bThreadAlive;
    
};

#endif