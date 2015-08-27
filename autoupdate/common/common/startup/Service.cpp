#include "stdafx.h"
#include "Service.h"

#include <cacti/config/Portability.h>
#include <cacti/util/StringUtil.h>
#include <cacti/util/FileSystem.h>
#include <alarm/AlarmSender.h>
#include "evtmsg.h"
#include "reqmsg.h"
#include "tag.h"

using namespace std;
using namespace cacti;

#define new DEBUG_NEW

static const int NAME_LENGTH = 64;
static const int VERSION_LENGTH = 64; 
Thread::thread_id Service::m_mainThread;

Service::Service(std::string name, std::string version, std::string dispName)
{
	m_name = name;
	m_version = version;
	m_displayName = dispName;

	m_versionInfo = "VER:";
	m_versionInfo += name.substr(0, NAME_LENGTH);
	for (int i=0; i< (int)(NAME_LENGTH- name.size()); i++)
		m_versionInfo += " ";
	m_versionInfo += "\n";

	m_versionInfo += version.substr(0, VERSION_LENGTH);
	for (int i=0; i< (int)(VERSION_LENGTH+1-version.size()); i++)
		m_versionInfo += " ";
	m_versionInfo += "\n";

	m_version += "build:";
	m_versionInfo +=" build:";
	char month[10];
	int day;
	int year;
	sscanf(__DATE__,"%s %d %d", &month, &day, &year);

	int imonth = 0;
	if (strcmp(month, "Jan") == 0 )
		imonth = 1;
	else if (strcmp(month, "Feb") == 0 )
		imonth = 2;
	else if (strcmp(month, "Mar") == 0 )
		imonth = 3;
	else if (strcmp(month, "Apr") == 0 )
		imonth = 4;
	else if (strcmp(month, "May") == 0 )
		imonth = 5;
	else if (strcmp(month, "Jun") == 0 )
		imonth = 6;
	else if (strcmp(month, "Jul") == 0 )
		imonth = 7;
	else if (strcmp(month, "Aug") == 0 )
		imonth = 8;
	else if (strcmp(month, "Sep") == 0 )
		imonth = 9;
	else if (strcmp(month, "Oct") == 0 )
		imonth = 10;
	else if (strcmp(month, "Nov") == 0 )
		imonth = 11;
	else if (strcmp(month, "Dec") == 0 )
		imonth = 12;

	m_versionInfo += StringUtil::format("%4d-%02d-%02d ", year, imonth, day);
	m_version += StringUtil::format("%4d-%02d-%02d ", year, imonth, day);
	m_versionInfo += __TIME__;

	m_mainThread = Thread::currentId();
}

Service::~Service(void)
{

}

void Service::commonHelp()
{
	printConsole("UI common list:\n");
	printConsole("\thelp : show this help message\n");
	printConsole("\tsnapshot : dump the snapshot(implement depend on service)\n");
	printConsole("\treload : reload the configures(implement depend on service)\n");
	printConsole("\tlogger sys|app level : change logger level\n");
	printConsole("end.\n");

}

void Service::changeSysLogger(int newlevel)
{
	Logger::getInstance(LOGGER_MTL_DISPATCHER).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_MTL_DISPATCHER).setLevel(LogControlID(newlevel));

	Logger::getInstance(LOGGER_MTL_SKELETON).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_MTL_SKELETON).setLevel(LogControlID(newlevel));

	Logger::getInstance(LOGGER_MTL_PEERS).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_MTL_PEERS).setLevel(LogControlID(newlevel));

	Logger::getInstance(LOGGER_SOCKET_CLIENT).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_SOCKET_CLIENT).setLevel(LogControlID(newlevel));

	Logger::getInstance(LOGGER_SOCKET_SERVER).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_SOCKET_SERVER).setLevel(LogControlID(newlevel));

	Logger::getInstance(LOGGER_SOCKET_ALARM).fatal("log level change to %d\n", newlevel);
	Logger::getInstance(LOGGER_SOCKET_ALARM).setLevel(LogControlID(newlevel));

	Service::printConsole("all system logger change to %d\n", newlevel);
}

void Service::dispatchUICommand (std::vector<std::string> &vec)
{
	if(vec.size() < 1 )
		return;

	if(vec[0] == "help")
	{
		help();
		commonHelp();
	}
	else if(vec[0] == "snapshot")
	{
		snapshot();
	}
	else if(vec[0] == "reload")
	{
		reload();
	}
	else if(vec[0] == "logger")
	{
		if(vec.size() > 2)
		{
			if(vec[1] == "sys")
				changeSysLogger(atoi(vec[2].c_str()));
			else
				logger(atoi(vec[2].c_str()));
		}
	}
	else 
	{
		handleUICommand(vec);
	}
}

void Service::printConsole(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	Logger::getInstance(LOGGER_STARTUP_CONSOLE).logva(LogLevel::INFO, fmt, ap);

	char msg[1024];
	vsnprintf(msg, sizeof(msg), fmt, ap);
	AlarmSender* alarm = AlarmSender::getInstance();
	if(alarm)
	{
		alarm->console(msg);
	}
	
	va_end(ap);
}

void Service::printConsole(LogControlID level, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	Logger::getInstance(LOGGER_STARTUP_CONSOLE).logva(level, fmt, ap);

	char msg[1024];
	vsnprintf(msg, sizeof(msg), fmt, ap);
	AlarmSender* alarm = AlarmSender::getInstance();
	if(alarm)
	{
		alarm->console(msg);
	}

	va_end(ap);
}

const char* Service::versioninfo()
{
	return m_versionInfo.c_str();
}

const char* Service::name()
{
	return m_name.c_str();
}

const char* Service::version()
{
	return m_version.c_str();
}

const char* Service::displayName()
{
	return m_displayName.c_str();
}

bool Service::getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm)
{
	return false;
}

bool Service::postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
{
	handleMessage(*utm);
	return true;
}

bool Service::postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead)
{
	handleMessage(*utm);
	return true;
}

void Service::handleMessage(cacti::UserTransferMessage& utm)
{
	switch(utm.getMessageId())
	{
	case _ReqCommand:
		{
			std::string cmd = utm[_TagMessage];
			std::vector<std::string> vec;
		
			cacti::StringUtil::split(vec, cmd, "=: ");
			for( size_t i= 0; i < vec.size() ; i++)
				cacti::StringUtil::toLower(vec[i]);

			dispatchUICommand(vec);
		}
		break;

	case _ReqCheckServiceStatus:
		{
			AlarmSender* alarm = AlarmSender::getInstance();
			if(alarm)
			{
				alarm->registerService();
			}
		}
		break;

	case _EvtStopProcess:
		{
			Service::exit(0);
		}
		break;

	default:
		break;
	}
}


void Service::exit(int retcode)
{
#ifdef WIN32
	ExitProcess(retcode);
#else
	exit(retcode);
#endif

}
