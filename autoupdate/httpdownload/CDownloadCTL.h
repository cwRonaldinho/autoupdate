#pragma once

#include <Windows.h>
#include <WinInet.h>
#include <string>
#include <time.h>
using namespace std;

#pragma comment(lib, "wininet.lib")

#define BUFF_SIZE			1024*4			//128KB
#define BUFF_SIZE_MIN		1024			//KB
#define WM_DL_MSG_UPDATE	(WM_USER + 100)

class DownloadCTL
{
public:
	DownloadCTL(void);
public:
	~DownloadCTL(void);
public:
	BOOL GetSoftVersionFromSrv(char* url,string&ver);//ªÒ»°∞Ê±æ
	BOOL WriteToLocalFile(char*url,char*localPath);
	BOOL WriteToString(char*url,string& res);
	DWORD GetSoftSize();
	void SetSoftSize(int s);
	void Init(HWND hwnd=0);
	void SetDownloadStatus(BOOL sta = FALSE);
	BOOL IsDownloadValid();
	BOOL Reset();
	string GetDownloadURL();
	string GetUnZipDirName();
	string GetZipFileName();
	BOOL AnalyzeDataByLabel(char*labelS,char*labelE,string source,string&con);

private:
	DWORD m_iSize;
	BOOL m_isValidDownload;
	HWND m_wHWnd;
	static string m_sSoftURL;
	static string m_sUnZipDirName;
	static string m_sZipFileName;
};
