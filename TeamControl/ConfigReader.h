#ifndef _Configer2_h
#define _Configer2_h
 
#include <string>
using namespace std;

#define TABLE_MAXROW 100
#define TABLE_MAXCOL 11 //含索引ID列 5

typedef struct
{
	int  nID;
	char s[TABLE_MAXCOL-1][64];

}TAG_TABLEREC,*LPTABLEREC;

class ConfigReader
{
	struct KVPair;
     
public:
	ConfigReader( string ac = "robot.cfg" );
	~ConfigReader();
	string m_szPath;//配置文件全路名
    
	bool append_from_file(const char* strfile);
	BOOL Read();//读入文件
	BOOL GetDefine( const LPCTSTR lpszName,CString &szVal); // 获取定义
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
	CString m_szDefine;  // 定义段
	int     m_nDefineLen;
	CString m_szTable;   // 表格段
	int     m_nTableLen;
	void DelNote();    //  去掉注释
	void FormatTable(void); //  规格化表格
};

#endif