#include "stdafx.h"

#include <cacti/logging/Logger.h>
#include <cacti/logging/ConsoleHandler.h>
#include <cacti/logging/FileHandler.h>
#include <cacti/logging/ConfigureLoader.h>
#include <cacti/util/FileSystem.h>
#include <cacti/kernel/XProcess.h>
#include <cacti/logging/LogManager.h>
#include "cacti/util/FileSystem.h"
#include "alarm/AlarmSender.h"
#include "cacti/util/MD5.h"

#include <iostream>
#include <set>
#include <string>

#ifndef WIN32
#ifdef __linux__
#	include <sys/prctl.h>
#endif
#	include <sys/time.h>
#	include <sys/resource.h>
uid_t	uid	= 0;
#else

#pragma comment(lib, "user32.lib")
#include <process.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")


void myInvalidParameterHandler(const wchar_t* expression,
							   const wchar_t* function, 
							   const wchar_t* file, 
							   unsigned int line, 
							   uintptr_t pReserved)
{
#ifdef _DEBUG
	wprintf(L"Invalid parameter detected in function %s."
		L" File: %s Line: %d\n", function, file, line);
	wprintf(L"Expression: %s\n", expression);
#endif
}
#endif

#include "XnuxDaemon.h"
#include "Service.h"
#include "Commander.h"

#include "cacti/util/IniFile.h"


void loadLoggerConfig();
int  stopWin32Proc();
void createChild(int argc, char* argv[]);
void doWork(bool daemonMode);

#define LOGGERXML "./logger.xml"

using namespace std;
using namespace cacti;

#ifdef NTSERVICE_MODE
	#ifndef WIN32
	#error NT Service mode just support windows platform
	#endif
	#include "NTService.h"
	int doNTService(int argc, char* argv[]);
#endif
// supported option
// daemon  -- run in daemon mode, default is debug mode
// stop    -- [try to] stop the running process(only WIN32 platform support this option)
// alarmport=port -- set the alarm port, default alarm port set to MONITOR_AGENT_PORT
int main(int argc, char* argv[])
{
#if defined(WIN32) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	std::locale::global(std::locale(""));

	CmdOption::setCmdOption(argc,argv);

#ifdef NTSERVICE_MODE
	return doNTService(argc, argv);
#endif

#ifndef WIN32
	uid	= getuid();
	setuid(0);
#endif
	cacti::FileSystem::mkdir("./log");
	bool daemonMode = CmdOption::hasCmdOption("daemon");

#ifdef WIN32
	_set_invalid_parameter_handler(myInvalidParameterHandler);
	// only WIN32 need support stop command line, 
	// LINUX platform will stop by kill signal.
	if(CmdOption::hasCmdOption("stop"))
	{
		return stopWin32Proc();
	}
#endif

	if(daemonMode)
	{
#ifdef WIN32 
		// if no "child" option means, it's father process
		// otherwise it's a child process already
		if(!CmdOption::hasCmdOption("child"))
			createChild(argc, argv);
#else
		createChild(argc, argv);
#endif
	}

	loadLoggerConfig();

	// load APC configure
	cacti::AbstractPath::initRols("./apc.conf");

	bootupSocket();

	// do the real work
	doWork(daemonMode);

	shutdownSocket();

	return 0;
}

void loadLoggerConfig()
{
	// set the syslog handler
	TTICFormatter* ttic = new TTICFormatter;
	ttic->logIndex(false);
	FormatterPtr tticPtr(ttic);

	LogHandlerPtr syshandler = LogHandlerPtr(new StarFileHandler("./log/sys.log"));
	LogHandlerPtr consolehandler = LogHandlerPtr(new StarFileHandler("./log/console.log"));
	syshandler->setFormatter(tticPtr);
	consolehandler->setFormatter(tticPtr);

	Logger& syslog = Logger::getInstance(LOGGER_STARTUP_SYS);
	syslog.addHandler(syshandler);
	Logger::getInstance(LOGGER_STARTUP_CONSOLE).addHandler(consolehandler);

	/// for MTL
	LogHandlerPtr mtlhandler = LogHandlerPtr(new StarFileHandler("./log/mtl.log"));
	StarFormatter* ttic2 = new StarFormatter;
	ttic2->logIndex(true);
	FormatterPtr ttic2Ptr(ttic2);

	mtlhandler->setFormatter(ttic2Ptr);
	// for MTL dispatcher
	Logger::getInstance(LOGGER_MTL_DISPATCHER).addHandler(mtlhandler);
	Logger::getInstance(LOGGER_MTL_SKELETON).addHandler(mtlhandler);
	Logger::getInstance(LOGGER_MTL_PEERS).addHandler(mtlhandler);

	// for TIMER
#ifdef _DEBUG
	Logger::getInstance(LOGGER_MTL_TIMER).setLevel(LogLevel::DBG);
#else
	Logger::getInstance(LOGGER_MTL_TIMER).setLevel(LogLevel::INFO);
#endif

	// for SOCKET
	LogHandlerPtr sockethandler = LogHandlerPtr(new StarFileHandler("./log/mtl-stk.log"));
	StarFormatter* ttic3 = new StarFormatter;
	ttic3->logIndex(true);
	FormatterPtr ttic3Ptr(ttic3);
	sockethandler->setFormatter(ttic3Ptr);

	Logger::getInstance(LOGGER_SOCKET_CLIENT).addHandler(sockethandler);
	Logger::getInstance(LOGGER_SOCKET_SERVER).addHandler(sockethandler);

#ifdef _DEBUG
	Logger::getInstance(LOGGER_SOCKET_CLIENT).setLevel(LogLevel::DBG);
	Logger::getInstance(LOGGER_SOCKET_SERVER).setLevel(LogLevel::DBG);
#else
	Logger::getInstance(LOGGER_SOCKET_CLIENT).setLevel(LogLevel::WARN);
	Logger::getInstance(LOGGER_SOCKET_SERVER).setLevel(LogLevel::WARN);
#endif

	// for ALARM socket
	LogHandlerPtr alarmhandler = LogHandlerPtr(new StarFileHandler("./log/alarm-stk.log"));
	alarmhandler->setFormatter(FormatterPtr(new StarFormatter));
	Logger::getInstance(LOGGER_SOCKET_ALARM).addHandler(alarmhandler);
#ifdef _DEBUG
	Logger::getInstance(LOGGER_SOCKET_ALARM).setLevel(LogLevel::DBG);
#else
	Logger::getInstance(LOGGER_SOCKET_ALARM).setLevel(LogLevel::WARN);
#endif

	Logger::getInstance(LOGGER_STARTUP_SYS).addHandler(LogHandlerPtr(new ConsoleHandler()));
	Logger::getInstance(LOGGER_STARTUP_CONSOLE).addHandler(LogHandlerPtr(new ConsoleHandler()));
}

#ifdef WIN32
int stopWin32Proc()
{
	DWORD processid;
	DWORD threadid;
	ifstream ifs;
	ifs.open(PIDFILENAME);
	if(ifs.is_open())
	{
		ifs >> processid;
		ifs >> threadid;
		BOOL ret= PostThreadMessage(threadid, WM_QUIT, 0, 0);
		if(!ret)
		{
			cout << "Can't post message, error:" << GetLastError() << endl;
			return -3;
		}
		ifs.close();
	}
	else
	{
		cout << "Can't open PID file:" << PIDFILENAME << endl;
		return -2;
	}
	cout << "Stopping";
	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);
	if(hprocess == NULL)
	{
		cout << endl << "Stopped" << endl;
	}
	else
	{
		int checkint = 300;
		int retry = (3*60*1000)/checkint;	// 3分钟
		while(retry--)
		{
			if(WaitForSingleObject(hprocess, checkint) == WAIT_OBJECT_0)
			{
				cout << endl << "Stopped" << endl;
				break;
			}
			cout << ".";
		}
		if(retry <= 0)
		{
			cout << "Try stop PID[" << processid << "] FAILED, Terminate it!" << endl;
			if(!TerminateProcess(hprocess, 9951))
				cout << "Terminate FAILED" << endl;
		}

		CloseHandle(hprocess);
	}
	unlink(PIDFILENAME);
	return 12;
}
#endif

void createChild(int argc, char* argv[])
{
#ifdef WIN32
	// check weather the process already running...
	ifstream ifp;
	ifp.open(PIDFILENAME);
	if(ifp.is_open())
	{
		std::string procname = argv[0];
		std::string::size_type pos = procname.find_last_of('\\');
		if(pos != std::string::npos)
			procname = procname.substr(pos+1);

		bool running = false;
		DWORD procid;
		ifp >> procid;

		HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procid);
		if(hproc != NULL)
		{
			HMODULE hmod;
			DWORD needed;
			if(EnumProcessModules(hproc, &hmod, sizeof(hmod), &needed))   
			{
				TCHAR modname[_MAX_PATH] = "";
				GetModuleBaseName(hproc, hmod, modname, sizeof(modname));
				if(!stricmp(modname, procname.c_str()))   
				{
					running = true;
				}   
			}
			CloseHandle(hproc);			
		}
		if(running)
		{
			cout << procname << " already running, process ID is " << procid << endl;
			exit(12);
		}	
	}
	//
	std::string childprocess;
	for(int i=0; i<argc; i++)
	{
		childprocess += argv[i];
		childprocess += " ";
	}

	// append a "child option to the command line
	childprocess += "child";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	memset(&pi, 0, sizeof(pi));

	if( !CreateProcessA( NULL,
		(LPSTR)childprocess.c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,                // creation flags. 
		NULL,
		NULL,
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{
		cout << "Can't create a daemon" << endl;
		exit(-1);
	}
	cout << "Using daemon mode" << endl;
	// save the processId	
	ofstream ofp;
	ofp.open(PIDFILENAME);
	if(ofp.is_open())
	{
		//ofp << pi.dwProcessId << endl << pi.dwThreadId << endl;
		char	acProId[64] = {0};
		char	acThrId[64] = {0};
		sprintf(acProId, "%d", pi.dwProcessId);
		sprintf(acThrId, "%d", pi.dwThreadId);
		ofp << acProId << endl << acThrId << endl;
		ofp.close();
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	exit(0);

#else

	gid_t   gid;
	gid = getgid();
	setgroups(1, &gid);

	cout << "Using daemon mode" << endl;

	pid_t runningPID = getRunningPID();
	if(runningPID > 0)
	{
		cout << argv[0] << " already running, PID is " << runningPID << endl;
		exit(12);
	}
	sigBlock();
	makeDaemon();

	// daemon process will go on
	#ifdef __linux__
	prctl(PR_SET_DUMPABLE,1,42,42,42);
	#endif

	rlimit          rl;
	getrlimit( RLIMIT_CORE, &rl );
	if ( rl.rlim_max != -1 ) {
		rl.rlim_max     = -1;
	};
	rl.rlim_cur     = rl.rlim_max;
	if ( setrlimit( RLIMIT_CORE, &rl ) != 0 ){
		printf("ERRO: fail to setrlimit, %s\n", 
			strerror(errno) );
	};
#endif
}

#if _MSC_VER >=1400
const std::wstring to_wcs( const std::string& src )
{
	std::string prev = setlocale(LC_CTYPE,NULL);	// 保存原来的locale
	setlocale( LC_CTYPE, "chs" );					// 设置当前locale为chs，这在非简体中文平台上不可缺少

	size_t count1 = mbstowcs(NULL, src.c_str(), 0); // 计算新字符串长度
	std::wstring des(count1, L' ');
	size_t count2 = mbstowcs(&des[0], src.c_str(), count1 ); // 转化
	assert(count1 == count2);

	setlocale( LC_CTYPE, prev.c_str()); // 恢复到原来的locale
	return des;
}
#endif

void doWork(bool daemonMode)
{
	Logger& syslog = Logger::getInstance(LOGGER_STARTUP_SYS);

	syslog.info("中文测试\n");
	u_short alarmport = (u_short)atoi(CmdOption::getCmdOption("alarmport").c_str());
	if(daemonMode)
	{
		Commander cmd;
		if(cmd.startService(alarmport))
		{
			syslog.info("Running in daemon\n");
#ifdef WIN32
			BOOL ret;
			MSG msg;
			while((ret = GetMessage( &msg, NULL, 0, 0 )) != 0)
			{ 
				if(ret == -1)
				{
					syslog.info("GetMessage FAILED, ret=%d, err=%d\n", ret, GetLastError());
				}
				else
				{
					TranslateMessage(&msg); 
					DispatchMessage(&msg); 
				}
			}
			unlink(PIDFILENAME);
#else
			sigWait();
#endif
		}
		else
		{
#ifndef WIN32
			stopDaemon(0);	// just for remove the PID file, .^_^.
#endif
		}
		cmd.stopService();
		syslog.info("Done!\n");
	}
	else		// running in console
	{
		syslog.info("Using console mode\n");
		Commander cmd;

#ifdef WIN32
		// change console title 
		TCHAR modname[MAX_PATH];
		GetModuleFileNameA(NULL, modname, sizeof(modname));
		std::string pcname = modname;
		std::string::size_type pos = pcname.find_last_of('\\');
		if(pos != std::string::npos)
			pcname = pcname.substr(pos+1);

		std::string consoleTitle = "ShineMedia - ";

		consoleTitle += "[";
		consoleTitle += pcname;
		consoleTitle += "] ";
		consoleTitle += cmd.serviceVersion();
		SetConsoleTitle(consoleTitle.c_str());
		DWORD consoleMode;
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdIn, &consoleMode);
		SetConsoleMode(hStdIn,consoleMode & ~0x40);

#endif
		if(cmd.startService(alarmport))
		{
			syslog.info("Running...type \"quit\" to exit\n");
			// wait for the process to exit
			std::string input;
			while(true)
			{
				std::getline(cin,input);
				if(input == "quit")
				{
					syslog.info( "Are your sure want to exit(yes/NO)?");
					std::cin >> input;
					if(input == "yes")
					{
						syslog.fatal("Service quiting normally......\n");
						break;
					}
				}
				else{
					cmd.handleUICommand(input.c_str());
				};
			}
			cmd.stopService();
			syslog.info("Done!\n");
		}
		else
		{
			cmd.stopService();
			syslog.fatal("Service exit!\n");
		}
	}
}

#ifdef NTSERVICE_MODE
class ShineService : public NTService
{
public:
	ShineService(Commander& cmd)
		: NTService(cmd.serviceName(), cmd.serviceVersion(), cmd.displayName())
		, m_commander(cmd)
	{
	}
	virtual bool OnInit()
	{
		loadLoggerConfig();
		cacti::AbstractPath::initRols("./apc.conf");
		bootupSocket();
		return true;
	}
	virtual void Run()
	{
		Logger& syslog = Logger::getInstance(LOGGER_STARTUP_SYS);
		
		if(m_commander.startService(0))
		{
			syslog.info("Running\n");
		}
		NTService::Run();
	}
	virtual void OnStop()
	{
		m_commander.stopService();
		shutdownSocket();
	}
private:
	Commander& m_commander;
};

int doNTService(int argc, char* argv[])
{
	std::string modpath = FileSystem::getModulePath();
	SetCurrentDirectory(modpath.c_str());

	bootupSocket();
	cacti::FileSystem::mkdir("./log");
	Commander cmd;
	ShineService ntservice(cmd);
	if (!ntservice.ParseStandardArgs(argc, argv)) {
		ntservice.StartService();
	}
	shutdownSocket();
	// 到这里，服务已经停止
	return ntservice.m_status.dwWin32ExitCode;
}

#endif