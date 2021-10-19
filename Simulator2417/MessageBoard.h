#ifndef _MessageBoard_
#define _MessageBoard_

#include "Geometry.h"
#include <afxmt.h>
#include "LineScanning.h"

class MessageBoard
{
   Vector          Data[2][10];
   ScanResultList* scanResults;

   CCriticalSection m_cs;
 public:
	  MessageBoard();
	  virtual ~MessageBoard();
  ReadData(Vector* receivedata,int j,ScanResultList* Results  = NULL );
  WriteData(const Vector* receivedata,ScanResultList* Results = NULL );
 
};
#endif