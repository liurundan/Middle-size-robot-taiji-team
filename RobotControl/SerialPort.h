#ifndef _CSerialPort_H_
#define _CSerialPort_H_

class CSerialPort
{
public:
	CSerialPort(UINT nPort, UINT nBaud);
	virtual	~CSerialPort();
	
    bool				OpenPort(UINT nPort, UINT nBaud);
    bool				Close();

	bool				StartMonitoring();
	bool				StopMonitoring();
	bool				WriteToPort(BYTE *Byte, int Len);
    int					ReadFromPort(BYTE *lpszBlock, int nRLen);
    void				ClearReadBuf();
    const BYTE*			Lock();
	void				Unlock();
    void				SetRThreshold(UINT datalength);

protected:
	void				ProcessErrorMessage(char* ErrorText);
	static UINT			CommThread(LPVOID pParam);
    
    CWinThread*			m_ReadComThread;
	CRITICAL_SECTION	m_csCommunicationSync;
	bool				m_bOpened, m_bExitThread;
    HANDLE              m_hIDCom;
    UINT                m_nPortNr;
    int                 m_dwReadLen;
	UINT				m_RThreshold;
    OVERLAPPED          m_OverlappedRead, m_OverlappedWrite;
private:
	BYTE*		m_ReadBuf;

};

#endif