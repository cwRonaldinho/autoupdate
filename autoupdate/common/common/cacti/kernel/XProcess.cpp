#include "stdafx.h"

#ifdef WIN32
	#include <process.h>
	#include <Psapi.h>
	#pragma comment(lib, "Psapi.lib")

#else
	#include <stdlib.h> 
	#include <unistd.h> 
	#include <sys/types.h> 
	#include <sys/wait.h> 
#endif

#include "cacti/util/StringUtil.h"

#include <vector>
#include <string>

#include "XProcess.h"

namespace cacti
{
	bool createl(char* argv[], const std::string& workpath, XProcess::exit_type* exitcode);
	bool createw(const std::string& cmdline, const std::string& workpath, XProcess::exit_type* exitcode);


	bool createw(const std::string& cmdline, const std::string& workpath, XProcess::exit_type* exitcode)
	{
#ifdef WIN32
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		memset(&pi, 0, sizeof(pi));

		if( !CreateProcessA( NULL,
			(LPSTR)cmdline.c_str(),
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW,
			NULL,
			workpath.empty() ? NULL : workpath.c_str(),
			&si,
			&pi)
			) 
		{
			return false;
		}
		if(exitcode)
		{
			// wait for process exit
			WaitForSingleObject(pi.hProcess, INFINITE);
			GetExitCodeProcess(pi.hProcess, exitcode);
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return true;
#else
		std::vector<std::string> vec;
		cacti::StringUtil::split(vec, cmdline, " ");
		if(vec.size()<1)
		{
			printf("cmdline empty\n");
			return false;
		}
		char** argv = new char*[vec.size()+1];
		argv[0] = (char*)vec[0].c_str();
		for(size_t i=1; i<vec.size(); i++)
		{
			argv[i] = (char*) vec[i].c_str();
		}
		argv[vec.size()] = NULL;
		bool ret = createl(argv, workpath, exitcode);
		delete [] argv;
		return ret;
#endif
	}

	bool createl(char* argv[], const std::string& workpath, XProcess::exit_type* exitcode)
	{
#ifdef WIN32
		std::string cmdline;
		int idx = 0;
		while(argv[idx] != NULL)
		{
			cmdline += argv[idx];
			cmdline += " ";
			idx++;
		}

		return createw(cmdline, workpath, exitcode);
#else
		pid_t pid;
		pid = fork();
		if(pid == 0)
		{ 
			if(!workpath.empty())
				chdir(workpath.c_str());		
			if(execvp(argv[0], argv) < 0)
			{
				printf("execvp failed, %d\n", errno);
				exit(51);
			}
		}
		else
		{ 
			if(exitcode)
			{
				int status;
				pid=waitpid(pid, &status, 0);
				*exitcode=WEXITSTATUS(status);
			}
		} 
		return true;
#endif
	}

	bool XProcess::create(const std::string& cmdline, const std::string& workpath, exit_type& exitcode)
	{
		return createw(cmdline, workpath, &exitcode);
	}

	bool XProcess::create(const std::string& cmdline, const std::string& workpath)
	{
		return createw(cmdline, workpath, NULL);
	}

	// argv 是列表方式，第一个元素是进程名字，之后是进程需要的参数列表，最后跟一个NULL表示结束
	bool XProcess::create(char* argv[], const std::string& workpath, exit_type& exitcode)
	{
		return createl(argv, workpath, &exitcode);
	}
	bool XProcess::create(char* argv[], const std::string& workpath)
	{
		return createl(argv, workpath, NULL);
	}
	bool XProcess::isrunning(pid_t procid)
	{
#ifdef WIN32
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procid);
		if(handle != NULL)
		{
			CloseHandle(handle);
			return true;
		}
		else
		{
			printf("OpenProcess(%d) by processid, error:%d\n", procid, ::GetLastError());
			return false;
		}
#else
		std::string pidstr = StringUtil::format("%d", procid);
		char * argv[4];
		argv[0] = "ps";
		argv[1] = "-p";
		argv[2] = (char*) pidstr.c_str();
		argv[3] = NULL;

		cacti::XProcess::exit_type exitcode;
		cacti::XProcess::create(argv, "", exitcode);
		return exitcode == 0;
#endif
	}
	bool XProcess::isrunning(const std::string& procname)
	{
#ifdef WIN32
		DWORD allproc[1024], needed;
		if(!EnumProcesses(allproc, sizeof(allproc), &needed))
		{
			printf("EnumProcesses() API error:%d\n", ::GetLastError());
		}   

		DWORD procount = needed / sizeof(DWORD);

		for(DWORD i=0; i<procount; i++)   
		{   
			HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, allproc[i]);
			if(hproc)
			{
				HMODULE hmod;
				if(EnumProcessModules(hproc, &hmod, sizeof(hmod), &needed))   
				{
					TCHAR modname[_MAX_PATH] = "";
					GetModuleBaseName(hproc, hmod, modname, sizeof(modname));
					char* exep = strstr(modname, ".exe");
					if(exep)
						*exep = '\0';
					if(!stricmp(modname, procname.c_str()))   
					{
						return true;
					}   
				}
				else
				{
					printf("EnumProcessModules() API error:%d\n", ::GetLastError());
				}
				CloseHandle(hproc);
			}
			else
			{
				printf("OpenProcess(%d) by name, error:%d\n", allproc[i], ::GetLastError());
			}
		}
		return false;
#else
		char * argv[4];
		argv[0] = "ps";
		argv[1] = "-C";
		argv[2] = (char*) procname.c_str();
		argv[3] = NULL;
		cacti::XProcess::exit_type exitcode;
		cacti::XProcess::create(argv, "", exitcode);
		return exitcode == 0;
#endif
	}
}
