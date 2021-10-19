#include "StdAfx.h"
#include "GrabbingThread.h"
#include "ColorTools.h"
#include "MultiImageProducer.h"
#include "Image.h"
#include "RGBImage.h"
#include "ColorClasses.h"
  

GrabbingThread::GrabbingThread(MultiImageProducer* producer)
: producer(producer), image(0), bufferedImage(0), segmentTool(0),
  colorInfos(0), showOrig(true),frameCounter(0),m_hShutdownEvent(0),m_hReadimageEvent(0)
{
 
  image			= producer->nextImage();
  bufferedImage = new RGBImage(image->getWidth(), image->getHeight());
  segmentTool   = new HSISegmentationTool();
  colorInfos    = new ColorClassInfoList();

  if(m_hShutdownEvent != NULL)
	ResetEvent(m_hShutdownEvent);
  m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  if(m_hReadimageEvent != NULL)
	ResetEvent(m_hReadimageEvent);
  m_hReadimageEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  m_hEventArray[0] = m_hShutdownEvent;
  m_hEventArray[1] = m_hReadimageEvent;
  
  m_bThreadAlive   = FALSE;
}

GrabbingThread::~GrabbingThread()
{ 
 	do
	{
	  SetEvent(m_hShutdownEvent);
	} 
	while( m_bThreadAlive ); 
    TRACE("Thread ended\n");
	
	if (image)			delete image;
	if (bufferedImage)	delete bufferedImage;
	if (segmentTool)	delete segmentTool;
	if (colorInfos)		delete colorInfos;
    
}

BOOL GrabbingThread::Start()
{
	if (!(m_Thread = AfxBeginThread(run, this)))
	  return FALSE;
	SetEvent(m_hReadimageEvent);
	
	TRACE("Thread started\n");
	return TRUE;	
}

UINT GrabbingThread::run(LPVOID pParam)
{
  GrabbingThread *grab = (GrabbingThread*) pParam;
  grab->m_bThreadAlive = TRUE;
   
  DWORD Event = 0;
  while(1)
  {
 	  
 	Event = WaitForMultipleObjects(2,grab->m_hEventArray,FALSE,INFINITE);
 
	switch(Event)
	{
		case 0:	
			{ 
				grab->m_bThreadAlive = FALSE;
  				AfxEndThread(100);
				break;
			} 
 		case 1:
			{
				grab->ReceiveImage();
				break;
			} 
		default:
			break;
	} 

  }
    
  return 0;
}

void GrabbingThread::ReceiveImage()
{
 	delete image;
  	image = producer->nextImage();
	image->setClassifier(segmentTool);

	unsigned char c;
	RGBTuple rgb;
	rgb.r = 0;
	rgb.g = 0;
	rgb.b = 0;

	m_cs.Lock();
	for (register int x=0; x <image->getWidth(); x++) 
	{
		for (register int y=0; y <image->getHeight(); y++) 
		{
		  	c = image->getPixelClass(x,y);
		  	if(c == COLOR_IGNORE)
			{
				if( showOrig ) 
				  image->getPixelRGB(x,y, &rgb);

			    bufferedImage->setPixelRGB(x,y, rgb);
			}
		  	else 
			{
				bufferedImage->setPixelRGB(x,y, colorInfos->classList[c]->color); 		
			}
		}
	}
	frameCounter++; 
	m_cs.Unlock();

}
 
void GrabbingThread::showOriginalImage(bool show) 
{
    showOrig = show;
}

BOOL GrabbingThread::RestartReceive()
{
	m_Thread->ResumeThread();
	return TRUE;
}

BOOL GrabbingThread::StopReceiveImage()
{
	m_Thread->SuspendThread();
	return TRUE;
}
 