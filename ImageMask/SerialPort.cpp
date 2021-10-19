#include "StdAfx.h"
#include "SerialPort.h"

CSerialPort::CSerialPort(UINT nPort, UINT nBaud)
{
    memset(&m_OverlappedRead, 0, sizeof(m_OverlappedRead));
    memset(&m_OverlappedWrite, 0, sizeof(m_OverlappedWrite));
    m_bOpened		= false;
    m_ReadComThread = NULL;
    m_hIDCom		= NULL;
    m_dwReadLen		= 0;
	m_RThreshold    = 11;
	InitializeCriticalSection( &m_csCommunicationSync );
	OpenPort(nPort, nBaud);
}

CSerialPort::~CSerialPort()
{
   DeleteCriticalSection(&m_csCommunicationSync);
   delete []m_ReadBuf;
   Close();
}

bool CSerialPort::OpenPort(UINT nPort, UINT nBaud)
{
    if( m_bOpened )
        return true;
    
	m_nPortNr = nPort;
    TCHAR szPort[15];
    TCHAR szComParams[50];
    DCB   dcb;
    
	m_ReadBuf = new BYTE[1024];
    memset(m_ReadBuf,'\0',sizeof(BYTE)*1024);
	
	if (m_hIDCom != NULL)
	{
		CloseHandle(m_hIDCom);
		m_hIDCom = NULL;
	}

    wsprintf(szPort, _T("COM%d"), nPort);
    m_hIDCom =  CreateFile( szPort, 
							GENERIC_READ | GENERIC_WRITE, 
							0, 
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
							NULL );
	
    if( m_hIDCom == NULL || m_hIDCom == INVALID_HANDLE_VALUE )
	{
		ProcessErrorMessage("Com port ID invalid!");
		return false;
	}

    memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
    memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutConstant = 1000;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 1000;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 1000;
    CommTimeOuts.WriteTotalTimeoutConstant = 5000;
    
	SetCommTimeouts(m_hIDCom, &CommTimeOuts);

    wsprintf(szComParams, _T("COM%d:%d,N,8,1"), nPort, nBaud);
    m_OverlappedRead.Offset = 0;
    m_OverlappedRead.OffsetHigh = 0;
    m_OverlappedRead.hEvent =  CreateEvent(NULL, true, false, NULL);
    m_OverlappedWrite.Offset = 0;
    m_OverlappedWrite.OffsetHigh = 0;
    m_OverlappedWrite.hEvent =  CreateEvent(NULL, true, false, NULL);

    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hIDCom, &dcb);
    dcb.BaudRate = nBaud;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;

    dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.StopBits = (BYTE)ONESTOPBIT;

    if(!SetCommState(m_hIDCom, &dcb) || !SetupComm(m_hIDCom, 1024, 1024) ||
        m_OverlappedRead.hEvent == NULL || m_OverlappedWrite.hEvent == NULL)
    {
        DWORD dwError =  GetLastError();
        if(m_OverlappedRead.hEvent != NULL)
             CloseHandle(m_OverlappedRead.hEvent);
        if(m_OverlappedWrite.hEvent != NULL)
             CloseHandle(m_OverlappedWrite.hEvent);
        CloseHandle(m_hIDCom);
        
		ProcessErrorMessage("Communicationport initialisation error!");

        return false;
    }
	
    SetCommMask(m_hIDCom, EV_RXCHAR);

    PurgeComm(m_hIDCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    EscapeCommFunction(m_hIDCom, SETDTR);

    m_bOpened     = true;
    m_bExitThread = false;
    SetRThreshold(11);
	
    StartMonitoring();
    return m_bOpened;
}

void CSerialPort::SetRThreshold(UINT datalength)
{
	m_RThreshold = datalength;
}

bool CSerialPort::Close()
{
    if(!m_bOpened || m_hIDCom == NULL)
        return true;
    m_bExitThread = true;
    SetCommMask( m_hIDCom, 0 );
    StopMonitoring();
    
    if( m_OverlappedRead.hEvent != NULL )
         CloseHandle(m_OverlappedRead.hEvent);

    if( m_OverlappedWrite.hEvent != NULL )
         CloseHandle(m_OverlappedWrite.hEvent);

    CloseHandle(m_hIDCom);

    m_bOpened = false;
    m_hIDCom  = NULL;

    return true;
}

bool CSerialPort::WriteToPort(BYTE *Byte, int Len)
{
    if( !m_bOpened || m_hIDCom == NULL )
        return false;

    BOOL  bWriteStat = TRUE;
    DWORD dwBytesWritten;
    PurgeComm(m_hIDCom, PURGE_TXCLEAR | PURGE_TXABORT);
    
	bWriteStat =  WriteFile(m_hIDCom, Byte, Len, &dwBytesWritten, &m_OverlappedWrite);
    
    if( bWriteStat && ( GetLastError() == ERROR_IO_PENDING) )
    {
        DWORD dw =  WaitForSingleObject(m_OverlappedWrite.hEvent, INFINITE );
        if( dw == WAIT_TIMEOUT )
        {
            dwBytesWritten = 0;
            DCB dcb;
            GetCommState(m_hIDCom, &dcb);
            PurgeComm(m_hIDCom, PURGE_TXCLEAR);
            return false;
        }
        else
        {
            GetOverlappedResult(m_hIDCom, &m_OverlappedWrite, &dwBytesWritten, 
									false); 
            m_OverlappedWrite.Offset += dwBytesWritten;
        }
		ProcessErrorMessage("WriteFile()");
		return false;
    }
    return true;
}

int CSerialPort::ReadFromPort(BYTE *lpszBlock, int nRLen)
{

	if( !m_bOpened || m_hIDCom == NULL )
        return 0;

    BOOL  bReadStat = TRUE;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;
    ClearCommError(m_hIDCom, &dwErrorFlags, &ComStat);
    if(!ComStat.cbInQue)
        return 0;
    
    dwBytesRead = (DWORD)ComStat.cbInQue;
    if( nRLen < (int)dwBytesRead )
        dwBytesRead = (DWORD)nRLen;

    bReadStat =  ReadFile( m_hIDCom, lpszBlock, dwBytesRead, &dwBytesRead, 
                        &m_OverlappedRead );

	if(!bReadStat)
	{
		if( GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_OverlappedRead.hEvent, INFINITE);
			return (int)dwBytesRead;
		}
		else
          	ProcessErrorMessage("ReadFile() error!");
		return 0;
	}

    return (int)dwBytesRead;
}


UINT CSerialPort::CommThread(LPVOID pParam)
{
    CSerialPort* pSerialPort = (CSerialPort*)pParam;
    
	BOOL bResult   = TRUE;
	BOOL firstbyte = TRUE;
	
	static int bytesum = 0;

	BYTE	Buffer[1024];
	BYTE	datahead = '\0'; 

    memset( Buffer, '\0', 1024*sizeof(BYTE) );

    DWORD	dwEvent, dwError;
    COMSTAT ComStat;
    int		ReadLen = pSerialPort->m_RThreshold;
    ReadLen *= 2;

	if (pSerialPort->m_hIDCom)
       PurgeComm(pSerialPort->m_hIDCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

 //   memset( pSerialPort->m_ReadBuf, '\0', sizeof(pSerialPort->m_ReadBuf) );
	pSerialPort->ClearReadBuf();
	
    SetCommMask(pSerialPort->m_hIDCom, EV_RXCHAR);
    
    while( !pSerialPort->m_bExitThread )
    {
	   bResult = WaitCommEvent(pSerialPort->m_hIDCom, &dwEvent, NULL);
	   if (!bResult)
	   {
		  pSerialPort->ProcessErrorMessage("WaitCommEvent()");
	   }
	   else
	   {
          bResult = ClearCommError(pSerialPort->m_hIDCom, &dwError, &ComStat);
	      if (ComStat.cbInQue == 0 ) 
			  continue;
	   }

       if( (dwEvent & EV_RXCHAR) && ComStat.cbInQue )
	 	 bytesum++;
 	   
       if( bytesum == ReadLen || ComStat.cbInQue == (DWORD)ReadLen )
	   {
		    EnterCriticalSection(&(pSerialPort->m_csCommunicationSync));
			pSerialPort->m_dwReadLen = pSerialPort->ReadFromPort( Buffer, ReadLen );
		
			if( pSerialPort->m_dwReadLen == ReadLen )
			{
			   for( int i= 0; i< ReadLen; i++)
			   {
                   if( *(Buffer+i) != 0xff )
				      continue;
				   else if( i < 16 )
				   {
				     memcpy(pSerialPort->m_ReadBuf, Buffer+i, (ReadLen/2)*sizeof(BYTE) );
					 break;
				   }
			   }
			}
 
			PurgeComm( pSerialPort->m_hIDCom, PURGE_RXCLEAR | PURGE_RXABORT );

 	        LeaveCriticalSection(&(pSerialPort->m_csCommunicationSync));

			memset( Buffer, '\0', 1024*sizeof(BYTE) );

			bytesum	  = 0;
       }     
    } 
    return 0;
}

bool CSerialPort::StartMonitoring()
{
    if(m_ReadComThread == NULL)
    {
        if(!(m_ReadComThread = AfxBeginThread(CommThread, this)))
            return false;
    }
    return true;
}

bool CSerialPort::StopMonitoring()
{
    if(m_ReadComThread != NULL)
    {
        ::TerminateThread(m_ReadComThread->m_hThread, 0);  
        m_ReadComThread = NULL;
    }
    return true;
}

void CSerialPort::ClearReadBuf()
{
    memset( m_ReadBuf, '\0', sizeof(BYTE)*1024 );
    m_dwReadLen = 0;
}

const BYTE* CSerialPort::Lock()
{
	if (m_ReadBuf)
	  EnterCriticalSection(&m_csCommunicationSync);
	return m_ReadBuf;
}

void CSerialPort::Unlock()
{
   	LeaveCriticalSection(&m_csCommunicationSync);
}

void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", 
				(char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

/*
  if( firstbyte )
		   {
		      pSerialPort->ReadFromPort( &datahead, 1 );
		   	  if( datahead == 0xff )
			  {
				 bytesum++;
				 firstbyte = FALSE;
			  }
			  else
				 bytesum = 0;
		   }
		   else
*/