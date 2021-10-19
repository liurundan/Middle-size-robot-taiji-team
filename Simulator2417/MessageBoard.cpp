#include "StdAfx.h"
#include "MessageBoard.h"
 
/*****************************************************************************/
/********************** CLASS MessageBoard ***************************************/
/*****************************************************************************/

MessageBoard::MessageBoard()
{
  scanResults = new ScanResultList(9);
}

MessageBoard::~MessageBoard()
{
  delete scanResults;
}

MessageBoard::WriteData(const Vector* receivedata, ScanResultList* Results)
{
  int i;
  m_cs.Lock();
  scanResults->clear();
  if( Results != NULL )
  {
 	for( int i=0;i < Results->results.size(); i++ )
	{ 
  	 for (int j=0; j < Results->results[i]->points.size();j++ ) 
	 {
 		Vector pos = Results->results[i]->points[j];
	    scanResults->results[i]->points.push_back(pos);
	 } 
	}
  }
  for (i=0;i<10;i++) 
  {
	  Data[1][i]=Data[0][i];
  }
  for (i=0;i<10;i++)
  {
	  Data[0][i]=*(receivedata+i);
  }
  m_cs.Unlock();
}

MessageBoard::ReadData(Vector *receivedata,int j,ScanResultList* Results)
{
  int i;
  m_cs.Lock();
  if( Results != NULL )
  {
    for( int i=0;i < scanResults->results.size(); i++ )
	{ 
  	 for (int j=0; j < scanResults->results[i]->points.size();j++ ) 
	 {
 		Vector pos = scanResults->results[i]->points[j];
	    Results->results[i]->points.push_back(pos);
	 } 
	}
  }
  for (i=0;i<10;i++)
  {
    *(receivedata+i) = Data[j][i]; 
  }
  m_cs.Unlock();

}