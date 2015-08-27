#include "CDownloadCTL.h"

DownloadCTL::DownloadCTL(void)
{
}

DownloadCTL::~DownloadCTL(void)
{
}

string DownloadCTL::m_sSoftURL="";
string DownloadCTL::m_sUnZipDirName="";
string DownloadCTL::m_sZipFileName="";
char		g_szPath[MAX_PATH]={0};

BOOL DownloadCTL::GetSoftVersionFromSrv( char* url,string&res )
{
	string strRES;
	WriteToString(url,strRES);
	int i = 0;
	if(!AnalyzeDataByLabel("<kouyu-soft-version>","</kouyu-soft-version>",strRES,res)
		||
		!AnalyzeDataByLabel("<kouyu-soft-url>","</kouyu-soft-url>",strRES,m_sSoftURL))
	{
		//MessageBox("版本数据标签解析出现错误");
		return FALSE;
	}
	//zip升级包的名字写入ini文件，解压时会用
	int ls = m_sSoftURL.find_last_of("\/");
	m_sZipFileName = m_sSoftURL.substr(ls+1);
	//把".zip去掉就是解压路径，保持起来
	int pi = m_sZipFileName.find(".");
	m_sUnZipDirName = m_sZipFileName.substr(0,pi);
	//
	char verLocal[MAX_PATH]={0};
	sprintf(verLocal,"%s\\Version.ini",g_szPath);
	WritePrivateProfileString((LPCSTR)"VersionCRL","URL",m_sZipFileName.c_str(),verLocal);

	return TRUE;
}

BOOL DownloadCTL::AnalyzeDataByLabel(char*labelS,char*labelE,string source,string&con)
{
	string::size_type s = source.find(labelS);
	string::size_type e = source.find(labelE);
	if (s == source.npos|| e == source.npos || s>=e )
		return FALSE;
	//提取内容
	size_t le = strlen(labelS);
	size_t co = e - s - le;
	con = source.substr(s+le,co);

	return TRUE;
}

BOOL DownloadCTL::WriteToString(char*url,string&res )
{
	HINTERNET internetOpen = InternetOpen("Internet Explorer", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (internetOpen == NULL)
		return FALSE;
	//请求URL加上时间戳，以防缓存
	string urlstr = url;
	char t[20]={0};
	sprintf(t, "?t=%d", time(0));
	urlstr+=t;
	//打开连接
	HINTERNET internetOpenUrl = InternetOpenUrl(internetOpen, urlstr.c_str(), NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	if (internetOpenUrl == NULL)
	{
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//检查状态码
	DWORD dwStatusCode;
	DWORD dwStatusSize = sizeof(dwStatusCode);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
	if (dwStatusCode!=200)
	{
		InternetCloseHandle(internetOpenUrl);
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//获取接收数据大小
	DWORD dwLengthSize = sizeof(m_iSize);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &m_iSize, &dwLengthSize, NULL);
	if (m_iSize < 0)
	{
		InternetCloseHandle(internetOpenUrl);
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//执行读取数据
	BOOL internetReadFile;
	char buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	DWORD byteRead = 0;
	if (internetOpenUrl != NULL)
	{
		while (true)
		{
			InternetReadFile(internetOpenUrl, buffer, sizeof(buffer), &byteRead);
			if (byteRead == 0)
				break;
			res+=buffer;
		}
	}
	//下载完毕关闭连接
	InternetCloseHandle(internetOpenUrl);
	InternetCloseHandle(internetOpen);
	return TRUE;
}

BOOL DownloadCTL::WriteToLocalFile(char*url,char*localPath)
{
	//首先获取资源文件的信息
	HINTERNET internetOpen = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (internetOpen == NULL)
	{
		printf("InternetOpen fail\n");
		return FALSE;
	}
	//打开连接
	HINTERNET internetOpenUrl = InternetOpenUrl(internetOpen, url, NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	if (internetOpenUrl == NULL)
	{
		printf("InternetCloseHandle fail\n");
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//检查状态码
	DWORD dwStatusCode;
	DWORD dwStatusSize = sizeof(dwStatusCode);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
	if (dwStatusCode!=200)
	{
		InternetCloseHandle(internetOpenUrl);
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//获取接收数据大小
	DWORD dwLengthSize = sizeof(m_iSize);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &m_iSize, &dwLengthSize, NULL);
	if (m_iSize < 0)
	{
		InternetCloseHandle(internetOpenUrl);
		InternetCloseHandle(internetOpen);
		return FALSE;
	}
	//创建本地文件
	HANDLE createFile = CreateFile(localPath, GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (createFile == INVALID_HANDLE_VALUE)
		return FALSE;
	//执行读取数据
	BOOL internetReadFile;
	char buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	DWORD byteRead = 0;
	BOOL hwrite;
	DWORD written;
	DWORD tmpSize = 0;
	int a = 0;
	if (internetOpenUrl != NULL)
	{
		while (true)
		{
			if (!IsDownloadValid())//外部终止进程
				break;
			InternetReadFile(internetOpenUrl, buffer, sizeof(buffer), &byteRead);
			if (byteRead == 0)//读取完毕
				break;
			tmpSize += byteRead;
			hwrite = WriteFile(createFile, buffer, byteRead, &written, NULL);
			if (hwrite == 0)//写入失败
				break;
			//发送消息之前先判断是否接受完毕，接受完毕后要释放文件句柄，因为后面的解压程序会用到该文件
			if (!(tmpSize<m_iSize))
			{
				FlushFileBuffers(createFile);
				CloseHandle(createFile);
			}
			//SendMessage(m_wHWnd,WM_DL_MSG_UPDATE,0,(LPARAM)(&tmpSize));//发送前端更新下载进度
			//printf("process: %0.2f\n", tmpSize*1.0/m_iSize);
		}
	}
	//下载完毕关闭连接
	InternetCloseHandle(internetOpenUrl);
	InternetCloseHandle(internetOpen);

	return TRUE;
}

DWORD DownloadCTL::GetSoftSize()
{
	return m_iSize;
}

void DownloadCTL::Init(HWND hwnd)
{
	char*	pDst = NULL;
	//int		iPos = 0;
	ZeroMemory(g_szPath,MAX_PATH);
	sprintf(g_szPath,_pgmptr);
	pDst = strrchr(g_szPath,'\\');
	int iPos = 0;

	if(pDst != NULL)
	{
		iPos = pDst - g_szPath;
		g_szPath[iPos] = '\0';
	}

	//this->m_wHWnd = hwnd;
}

void DownloadCTL::SetDownloadStatus( BOOL sta /*= FALSE*/ )
{
	this->m_isValidDownload = sta;
}

BOOL DownloadCTL::IsDownloadValid()
{
	return m_isValidDownload;
}

BOOL DownloadCTL::Reset()
{
	SetSoftSize(0);
	SetDownloadStatus(FALSE);

	return TRUE;
}

void DownloadCTL::SetSoftSize( int s )
{
	this->m_iSize = s;
}

string DownloadCTL::GetDownloadURL()
{
	return m_sSoftURL;
}

std::string DownloadCTL::GetUnZipDirName()
{
	return m_sUnZipDirName;
}

std::string DownloadCTL::GetZipFileName()
{
	return m_sZipFileName;
}