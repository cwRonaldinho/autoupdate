// 自动覆盖程序

#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

#include <io.h>
#include <direct.h>

// 版本号
#define VERSION_CURRENT "autocover.version=1.0.0"

#define SYSTEM_CFG_FILE  "./autoupdate/autoupdate.ini"

#define UPDATE_FLAG_FILE "./autoupdate/updateflag.dat"

#define COVER_SOURCE_DIR "./autoupdate/downloaded"

// 覆盖的目标目录
#define COVER_DST_DIR "./"

// 覆盖程序本身所在目录
#define COVER_SELF_DIR "./autoupdate"

#define MAIN_EXE_PATH ".\\" // 主进程路径
#define DEFAULT_MAIN_EXE_NAME "sale.exe"		// 默认主exe程序名

int CopyDir(const char * pSrc,const char *pDes);
int DelDir(const char * pSrc);

int StartProgram(char* path, char* name);

BOOL FindAndKillProcessByName(LPCTSTR strProcessName);

// 全局变量 
char g_mainExeName[256];     // 主exe程序名

// 写版本号
void WriteVersion()
{
	char szVersionFile[256];
	sprintf_s(szVersionFile, 256, "%s/version_cover.txt", COVER_SELF_DIR);
	printf("%s\n", szVersionFile);

	FILE *pf;
	fopen_s(&pf, szVersionFile, "w");
	if (pf == NULL)
	{
		printf("open version file fail\n");
		return;
	}

	fwrite(VERSION_CURRENT, 1, strlen(VERSION_CURRENT), pf);
	fclose(pf);
}

int main(int argc, char* argv[])
{
	::CreateMutex(NULL, FALSE, "autocovermutex"); /*创建一个互斥体*/
	if(ERROR_ALREADY_EXISTS == GetLastError()) 
	{ 
		printf("已有本程序实例正在运行...\n");
		return FALSE; 
	}

	WriteVersion();

	// 查询是否需要更新
	int needUpdate = GetPrivateProfileInt("info", "needupdate", 0, UPDATE_FLAG_FILE);
	if (1 != needUpdate) // 不需要更新
	{
		printf("不需要更新，继续运行主程序...\n");
		getchar();
		return 0;
	}

	// 从配置处获取主进程名
	GetPrivateProfileString("autocover", "mainexename", DEFAULT_MAIN_EXE_NAME, g_mainExeName, sizeof(g_mainExeName), SYSTEM_CFG_FILE);
	printf("主进程名:%s\n", g_mainExeName);

	// 关闭主exe 结束进程名
	BOOL bRet = FindAndKillProcessByName(g_mainExeName);
	if (FALSE == bRet)
	{
		printf("end main exe failed, exit.\n");
		getchar();
		return 1;
	}
	printf("结束主进程\n");

	// 结束其它进程
	for (int i=1; ; i++)
	{
		char key[100];
		sprintf_s(key, 100, "processname%d", i);
		char exeName[100];
		GetPrivateProfileString("kill", key, "", exeName, 100, SYSTEM_CFG_FILE);

		printf("process%d:%s\n", i, exeName);

		if (0 == strcmp("", exeName))
		{
			break;
		}

		bRet = FindAndKillProcessByName(exeName);
		if (FALSE == bRet)
		{
			printf("结束进程 %s 失败\n", exeName);
		}
		else
		{
			printf("结束进程 %s\n", exeName);
		}
	}

	printf("正在更新，请稍等\n更新中...\n");

	// 用下载的新文件覆盖原文件
	int retCopy = CopyDir(COVER_SOURCE_DIR, COVER_DST_DIR);
	if (retCopy < 0)  // 拷贝失败
	{
		printf("拷贝更新文件失败...\n");
		StartProgram(MAIN_EXE_PATH, g_mainExeName);
		getchar();
		return 2;
	}

	int retDel = DelDir(COVER_SOURCE_DIR);
	if (retDel < 0)  // 拷贝失败
	{
		printf("删除更新文件失败...\n");
		StartProgram(MAIN_EXE_PATH, g_mainExeName);
		getchar();
		return 3;
	}

	// 删除更新标志文件
	FILE *pf = fopen(UPDATE_FLAG_FILE, "w");
	if (!pf) // 如果打开失败，表明autoupdate程序可能正在操作此文件，等待几秒后再行操作
	{
		Sleep(3000);
		pf = fopen(UPDATE_FLAG_FILE, "w");		
	}

	if (!pf)
	{
		printf("打开标志文件失败...\n");
		StartProgram(MAIN_EXE_PATH, g_mainExeName);
		getchar();
		return 4;
	}

	char *content = "[info]\nneedupdate=0\n";
	fwrite(content, 1, strlen(content), pf);
	fclose(pf);		

	printf("更新完成，正在启动主程序...\n");
	StartProgram(MAIN_EXE_PATH, g_mainExeName);

	//printf("按任意键关闭本窗口\n");
	//getchar();
	
	return 0;
}

// 重启主线程
// path:路径  name:程序名
int StartProgram(char* path, char* name)
{
	// 切换工作目录
	chdir(path);

	char tmp[256];
	getcwd(tmp, 256);
	printf("current work dir: %s\n", tmp);

	// 不需要组装全路径，因为已经切换了工作目录，所以直接执行exe即可
// 	char cmdline[256];
// 	sprintf_s(cmdline, 256, "%s\\%s", path, name);
// 
// 	printf("%s\n", cmdline);

	STARTUPINFO si = { sizeof(si) }; 
	PROCESS_INFORMATION pi; 

	si.dwFlags = STARTF_USESHOWWINDOW; 
	si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口
	//char cmdline[] =TEXT(path);
	BOOL bRet = ::CreateProcess ( 
		NULL,
		name, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改	 
		NULL, 
		NULL, 
		FALSE, 
		CREATE_NEW_CONSOLE, 
		NULL, 
		NULL, 
		&si, 
		&pi); 

	int error = GetLastError();
	if(bRet) 
	{ 
		::CloseHandle (pi.hThread); 
		::CloseHandle (pi.hProcess); 

		printf("新进程的进程ID号：%d \n", pi.dwProcessId); 
		printf("新进程的主线程ID号：%d \n", pi.dwThreadId); 
	} 
	else
	{
		printf("error code:%d\n",error );
		printf("启动主程序失败，请手动重启。按任意键关闭本程序\n");
		getchar();
		return 5;
	}
	return 0;
}

#define BUF_SIZE 2048
int copyFile(const char * pSrc,const char *pDes)
{
	FILE *in_file, *out_file;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;
	long len = 0;
	if ( (in_file = fopen(pSrc, "rb")) == NULL )
	{
		perror(pSrc);
		return -2;
	}
	if ( (out_file = fopen(pDes, "wb")) == NULL )
	{
		perror(pDes);
		return -3;
	}
	while ( (bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0 )
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if ( bytes_in != bytes_out )
		{
			perror("Fatal write error.\n");
			return -4;
		}
		len += bytes_out;
		//printf("copying file .... %d bytes copy\n", len);
	}
	fclose(in_file);
	fclose(out_file);
	return 1;
}

/*********************************************************************
功能：复制(非空)目录
参数：pSrc，原目录名
     pDes，目标目录名
返回：<0，失败
     >0，成功
*********************************************************************/
int CopyDir(const char * pSrc,const char *pDes)
{
    if (NULL == pSrc || NULL == pDes)	return -1;
    mkdir(pDes);
    char dir[MAX_PATH] = {0};
    char srcFileName[MAX_PATH] = {0};
    char desFileName[MAX_PATH] = {0};
    char *str = "\\*.*";
    strcpy(dir,pSrc);
    strcat(dir,str);
    //首先查找dir中符合要求的文件
    long hFile;
    _finddata_t fileinfo;
    if ((hFile = _findfirst(dir,&fileinfo)) != -1)
    {
        do
        {
            strcpy(srcFileName,pSrc);
            strcat(srcFileName,"\\");
            strcat(srcFileName,fileinfo.name);
            strcpy(desFileName,pDes);
            strcat(desFileName,"\\");
            strcat(desFileName,fileinfo.name);
            //检查是不是目录
            //如果不是目录,则进行处理文件夹下面的文件
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
				printf("src:%s, dst:%s\n", srcFileName, desFileName);
                copyFile(srcFileName,desFileName);
            }
            else//处理目录，递归调用
            {
                if ( strcmp(fileinfo.name, "." ) != 0 && strcmp(fileinfo.name, ".." ) != 0 )
                {
                    CopyDir(srcFileName,desFileName);
                }
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
        return 1;
    }
    return -3;
}

/*********************************************************************
功能：递归删除目录下所有文件
参数：pSrc，目录名
返回：<0，失败
     >0，成功
*********************************************************************/
int DelDir(const char * pSrc)
{
	if (NULL == pSrc)	return -1;

	char dir[MAX_PATH] = {0};
	char srcFileName[MAX_PATH] = {0};
	char *str = "\\*.*";
	strcpy(dir,pSrc);
	strcat(dir,str);
	//首先查找dir中符合要求的文件
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir,&fileinfo)) != -1)
	{
		do
		{
			strcpy(srcFileName,pSrc);
			strcat(srcFileName,"\\");
			strcat(srcFileName,fileinfo.name);
			
			//检查是不是目录
			//如果不是目录,则进行处理文件夹下面的文件
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				DeleteFile(srcFileName);
			}
			else//处理目录，递归调用
			{
				if ( strcmp(fileinfo.name, "." ) != 0 && strcmp(fileinfo.name, ".." ) != 0 )
				{
					DelDir(srcFileName);
					// 删除目录本身
					DeleteFile(srcFileName);
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
		return 1;
	}
	return -3;
}

#include <tlhelp32.h>
BOOL FindAndKillProcessByName(LPCTSTR strProcessName)
{
	if(NULL == strProcessName)
	{
		return FALSE;
	}
	HANDLE handle32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == handle32Snapshot)
	{
		return FALSE;
	}

	PROCESSENTRY32 pEntry;       
	pEntry.dwSize = sizeof( PROCESSENTRY32 );

	BOOL bFound = FALSE;

	//Search for all the process and terminate it
	if(Process32First(handle32Snapshot, &pEntry))
	{
		if (!stricmp(pEntry.szExeFile, strProcessName))
		{			
			bFound = TRUE;
		}
		while((!bFound)&&Process32Next(handle32Snapshot, &pEntry))
		{
			//
			//printf("%s\n", pEntry.szExeFile);
			if (!stricmp(pEntry.szExeFile, strProcessName))
			{
				bFound = TRUE;
			}
		}
		if(bFound)
		{
			CloseHandle(handle32Snapshot);
			HANDLE handLe =  OpenProcess(PROCESS_TERMINATE , FALSE, pEntry.th32ProcessID);
			BOOL bResult = TerminateProcess(handLe,0);
			return bResult;
		}
	}

	CloseHandle(handle32Snapshot);

	if (!bFound)		// 未找到进程名，视为关闭成功
	{
		printf("未找到指定进程\n");
		return TRUE;
	}

	return FALSE;
}

