#include "StdAfx.h"
#include "ConfigReader.h"
 

ConfigReader::ConfigReader( string ac ) throw() :m_szPath(ac) 
{
	int i;
	//m_szPath="robot.cfg";
	m_nBufSize=1024*48;
	m_nDataSize=0;
	m_nDefineLen=0;
	m_nTableLen=0;

	for(i=0;i<m_nBufSize;i++)
	   m_cbuf[i]=0;
	ZeroMemory(&m_Table,sizeof(m_Table));
	m_nRows=0;

	Read();
}

ConfigReader::~ConfigReader() throw()
{
;
}
 	
BOOL ConfigReader::Read()
{
	CFile fl;
	
	if(!fl.Open(m_szPath.c_str(),CFile::modeRead|CFile::typeBinary))
	    return FALSE;
	DWORD dwl=fl.GetLength();
	if(dwl > (DWORD)m_nBufSize)
	    dwl=(DWORD)m_nBufSize;
	m_nDataSize=(int)( fl.Read(m_cbuf,dwl) );
	fl.Close();
	DelNote();//去掉注释

	return TRUE;
}

void ConfigReader::DelNote()//去掉注释,分成定义段和表格段
{
	CString szt = "";
	char cc,ccp = 0;
	int i;
	for( i=0; i<m_nDataSize; i++ )
	{
		cc = m_cbuf[i];
		if(cc != '/')
		{ 
			szt += cc;
			ccp =  cc;
		}
		else//cc=='/'
		{
			if(i<m_nDataSize)
			{
				if(m_cbuf[i+1]=='/')//注释开始
				{
					while((i<m_nDataSize)&&(m_cbuf[i]!=0x0d)) 
						i++;
					if(m_cbuf[i]==0x0d) 
						i--;
					ccp=0;
				}
		        else
				{
					szt+=cc; 
					ccp=cc; 
				}
			}
		}
	}
	m_nDataSize=szt.GetLength();
	sprintf(m_cbuf,"%s",szt);
	int ns=szt.Find("#DEFINE_BEGIN",0);
	int ne=szt.Find("#DEFINE_END",0);
	if( ( ns==-1 ) || ( ne==-1 ) ) 
		return;
	
	ns += 13;
	m_szDefine = "";
	for( i=ns; i<ne; i++ ) 
		m_szDefine += szt[i];
	
	m_nDefineLen = m_szDefine.GetLength();
	ns = szt.Find("#TABLE_BEGIN",0);
	ne = szt.Find("#TABLE_END",0);
	ns += 12;
	if( (ns==-1) || (ne==-1) ) 
		return;
	m_szTable = "";
	for( i=ns; i<ne; i++ ) 
		m_szTable += szt[i];
	m_nTableLen = m_szTable.GetLength();
	FormatTable();//规格化表格
}

BOOL ConfigReader::GetDefine( const LPCTSTR lpszName,CString &szVal) // 获取定义
{
	CString szName = lpszName;
	if( szName.IsEmpty() ) 
		return FALSE;
	int nLen = szName.GetLength();
	int ns   = m_szDefine.Find(lpszName,0);
	if( ns == -1 ) 
		return FALSE;
	szVal = "";
	int i = ns + nLen;
	BOOL bStartCopy = FALSE;
	char cc;
	while( i < m_nDefineLen )
	{
		cc = m_szDefine[i];
		if( ( cc != 0x0D ) && ( cc != 0x0A ) && ( cc !='\t' ) && ( cc != 0x20 ) &&
			( cc != '=' ) )
		{
		  if( bStartCopy == FALSE ) 
			bStartCopy = TRUE;
		   szVal += m_szDefine[i];
		}
		else
		{
			if( bStartCopy ) 
				break;
		} 
		i++;
	}
	return TRUE;
}

int ConfigReader::get( const LPCTSTR lpszName, int& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = atoi(sv);
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, unsigned int& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = atoi(sv);
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, long int& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = atol(sv);
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, unsigned long int& value ) throw()
{
  CString sv;
  char*   stop;
  int     base=0;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = strtoul(sv,&stop,base);
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, double& value ) throw()
{
  CString sv;
  char*   stop;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = strtod(sv,&stop);
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, float& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = (float)(atof(sv));
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, char& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = sv[0];
  return 1;
}

int ConfigReader::get( const LPCTSTR lpszName, bool& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  if (sv == "true" )
  {
	  value = true;
      return 1;
  }
  if (sv == "false") 
  {
	  value = false;
      return 1;
  }
  return -1;
}

int ConfigReader::get( const LPCTSTR lpszName, string& value ) throw()
{
  CString sv;
  BOOL success = GetDefine( lpszName, sv );
  if( !success )  
     return -1;
  value = sv.GetBuffer(0);
  return 1;
}

void ConfigReader::FormatTable()//规格化表格
{
	int i    = 0;
	char cc;
	int nRow = 0;
	CString szLine = "",szID = "", szVal = "", szCol = "";
	int nCol = 0;
	while( i < m_nTableLen )
	{
		cc = m_szTable[i];
		if( ( cc == 0x0A ) || ( cc == '\t' ) )
			cc = 0x20;
		if( cc == 0X0D )//处理一行
		{
			szLine.TrimLeft();
			szLine.TrimRight();
			if( !szLine.IsEmpty() )
			{
				int nl = szLine.GetLength();
				int j;
				char c;
				szID = "";
				for( j = 0; j < nl; j++ )//取szID
				{
					c = szLine[j];
					if( c == 0x20 )
					   break;
					szID += c;
				}
				m_Table[nRow].nID = atoi(szID);
				BOOL bStart;
				bStart = FALSE;
				szVal = "";
				while( j < nl ) //取szCols
				{
					c = szLine[j];
					if( ( bStart == FALSE ) && ( c == 0x20 ) )
						j++;
					else
					{
						bStart=TRUE;
						szVal+=c;j++;
					}
				}
				szVal.TrimLeft();
				szVal.TrimRight();
				nCol = 0;
				nl   = szVal.GetLength();
				j    = 0;
				szCol = "";
				while( j < nl )
				{
					c = szVal[j];
					if( c == ',' )
					{
						if( szCol.IsEmpty() ) 
							break;
						else
						{
							if( nCol < TABLE_MAXCOL-1 )
							{
								if(szCol.GetLength() <64)
								  sprintf( &(m_Table[nRow].s[nCol][0]),"%s",szCol );
								nCol++; TRACE("szCol=%s\n",szCol);
							} 
						} 
	                    szCol = "";
					} 
					else szCol += c;
					j++;
				} 
				if( !szCol.IsEmpty() )
				{
					if( nCol < TABLE_MAXCOL-1 )
					{
						if( szCol.GetLength() <64 )
						  sprintf( m_Table[nRow].s[nCol],"%s",szCol );
						nCol++; 
						TRACE("szCol=%s\n",szCol);
					}
				}
				if( nCol > 0 )
				  nRow++;
				if(m_nRows >=TABLE_MAXROW)
				{
					m_nRows = nRow;
					return;
				}
				szLine="";
			}
		}
	    else szLine+=cc;
		i++;
	}
	m_nRows=nRow;
}

bool ConfigReader::append_from_file(const char* strfile)
{
	CStringArray str0;
    m_szPath = strfile;
	Read();
	int num  = 0;

	if( get("file_num",num) < 0 )
	{
		AfxMessageBox("config_file_num error!");
		return false;
	} 
	
    CString fname;
	string  sTmp = "";

	for(int i=0; i<num; i++ )
	{
	  fname.Format("conf%d",i);
      
	  if( get(fname,sTmp) < 0 )
	  { 
		AfxMessageBox("config_file_name not found!");
		return false;
	  }  
	  str0.Add(sTmp.c_str());
	}
    
	CString fileName = "all.cfg";
 	CString StrLine ="";
 	CStdioFile InFile,OFile;
    
	bool firstline = true;
	int  index = 0;
	OFile.Open(fileName,CFile::modeCreate | CFile::modeWrite);
	 
	for(int j=0;j<str0.GetSize();j++)
	{ 
 		InFile.Open(str0.GetAt(j),CFile::modeRead);
 		do
		{ 
		  if( j == str0.GetSize() - 1 && firstline == true ) 
		  { 
 			 index++;
			 if( index == 2 )
			 { 
				 StrLine = "\n";
				 firstline = false;
			 } 
		  } 
		  
          OFile.WriteString(StrLine);
		  OFile.WriteString("\n");
		   
		} 
	    while(InFile.ReadString(StrLine));
 
		InFile.Close();
 	 
	}

	OFile.Close(); 
	
    m_szPath = fileName;
	Read();

	return true;
}





