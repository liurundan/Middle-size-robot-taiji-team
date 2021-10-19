#ifndef _Configer2_h
#define _Configer2_h
 
#include <string>
using namespace std;

#define TABLE_MAXROW 100
#define TABLE_MAXCOL 5 //������ID��

typedef struct
{
	int  nID;
	char s[TABLE_MAXCOL-1][64];

}TAG_TABLEREC,*LPTABLEREC;

class ConfigReader
{
	struct KVPair;
     
public:
	ConfigReader( string ac = "config_files\\robot_number.cfg" ) throw();
	~ConfigReader() throw();
	string m_szPath;//�����ļ�ȫ·��
    
	bool append_from_file(const char* strfile);
	BOOL Read();//�����ļ�
	BOOL GetDefine( const LPCTSTR lpszName,CString &szVal); // ��ȡ����
    int  get( const LPCTSTR lpszName, int& value )  throw();
	int  get( const LPCTSTR lpszName, unsigned int&)  throw ();
    int	 get( const LPCTSTR lpszName, long int&)  throw ();             
    int  get( const LPCTSTR lpszName, unsigned long int&)  throw ();    
    int  get( const LPCTSTR lpszName, double&)  throw ();               
    int  get( const LPCTSTR lpszName, float&)  throw ();                
    int  get( const LPCTSTR lpszName, char&)  throw ();                  
    int  get( const LPCTSTR lpszName, bool&)  throw ();                  
    int  get( const LPCTSTR lpszName, string&)  throw ();       
	//{{TABLE
	TAG_TABLEREC m_Table[TABLE_MAXROW];
	int m_nRows;
	//}}
	private: 
	char m_cbuf[1024*48];
	int  m_nBufSize;     // = 1024*48;
	int  m_nDataSize;
	CString m_szDefine;  // �����
	int     m_nDefineLen;
	CString m_szTable;   // ����
	int     m_nTableLen;
	void DelNote();    //  ȥ��ע��
	void FormatTable(void); //  ��񻯱��
};

#endif