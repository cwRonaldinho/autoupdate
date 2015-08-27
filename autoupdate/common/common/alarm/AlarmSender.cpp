#include "stdafx.h"
#include "AlarmSender.h"
#include "evtmsg.h"
#include "tag.h"
#include "cacti/util/FileSystem.h"

#pragma warning(disable:4355)

AlarmTransferClient::AlarmTransferClient(DispatcherInterface* di)
	: TransferClient(1, di, LOGGER_SOCKET_ALARM)
{
#ifdef __linux
	char modname[MAX_PATH] = {};
	getcwd(modname, MAX_PATH);
	m_path = modname;
#else
	char modname[_MAX_PATH] = {};
	GetModuleFileNameA(NULL, modname, sizeof(modname));
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	
	_splitpath(modname, drive, dir, fname, ext ); // C4996
	m_procname = fname;
	m_path = drive;
	m_path += dir;
#endif
	m_starttime = Timestamp(time(NULL)).getFormattedTime("%Y-%m-%d %H:%M:%S");
}

void AlarmTransferClient::onConnected()
{
	// register the service id
	registerService();
}

void AlarmTransferClient::registerService(const std::string& id)
{
	m_serviceId = id;
	if(m_procname.empty())
		m_procname = m_serviceId;

	registerService();
}

void AlarmTransferClient::registerService()
{
	UserTransferMessagePtr utm = UTM::create(_EvtRegisterService);
	(*utm)[_TagServiceId] = m_serviceId;
	(*utm)[_TagServiceName] = m_procname;
	(*utm)[_TagStartTime] = m_starttime;
	(*utm)[_TagFilePath] = m_path;
	(*utm)[_TagMessage] = m_version;
	(*utm)[_TagAppName] = m_dispName;

	NetworkPackage np;
	np.setUTM(utm);
	sendPackage(np);
}

bool AlarmTransferClient::send(UserTransferMessagePtr utm)
{
	NetworkPackage np;
	(*utm)[_TagServiceId] = m_serviceId;
	np.setUTM(utm);
	return sendPackage(np);
}

cacti::RecursiveMutex g_alarmMutex;

AlarmSender::AlarmSender(DispatcherInterface* disp)
	: m_client(disp) 
{  
}

AlarmSender* AlarmSender::m_instance = NULL;
u_short AlarmSender::m_port = 0;

void AlarmSender::createInstance(DispatcherInterface* disp, u_short port, const char* version, const char* dispname)
{
	if(!m_instance)
	{
		cacti::RecursiveMutex::ScopedLock lock(g_alarmMutex);
		if(!m_instance)
		{
			m_port = port;
			m_instance = new AlarmSender(disp);
			m_instance->m_client.start(m_port, "127.0.0.1", false);
		}
	}
	else
	{
		cacti::RecursiveMutex::ScopedLock lock(g_alarmMutex);
		if(m_instance && m_port != port)
		{
			m_instance->m_client.stop();
			delete m_instance;
			m_port = port;
			m_instance = new AlarmSender(disp);
			m_instance->m_client.start(m_port, "127.0.0.1", false);
		}
	}
	m_instance->m_client.m_version = version;
	m_instance->m_client.m_dispName = dispname;
}

void AlarmSender::destroyInstance()
{
	if(m_instance)
	{
		cacti::RecursiveMutex::ScopedLock lock(g_alarmMutex);
		if(m_instance)
		{
			m_instance->m_client.stop();
			delete m_instance;
			m_instance = NULL;
		}
	}	
}

AlarmSender* AlarmSender::getInstance()
{
	cacti::RecursiveMutex::ScopedLock lock(g_alarmMutex);
	return m_instance;
}

bool AlarmSender::send(ServiceIdentifier who, const char* alarmid, u32 level, const strarray& params)
{
	UserTransferMessagePtr utm = UTM::create(who, ServiceIdentifier(), _EvtAlarmMessage);
	(*utm)[_TagMessageId]	= alarmid;
	(*utm)[_TagGroupLevel]	= level;
	(*utm)[_TagMessage]		= params;

	return m_client.send(utm);
}

bool AlarmSender::console(const char* msg)
{
	UserTransferMessagePtr utm = UTM::create(ServiceIdentifier(), ServiceIdentifier(), _EvtAlarmConsole);
	(*utm)[_TagMessage]	= msg;

	return m_client.send(utm);
}

bool AlarmSender::unregisterService()
{
	UserTransferMessagePtr utm = UTM::create(_EvtUnregisterService);
	return m_client.send(utm);
}

bool AlarmSender::sendUTM(UserTransferMessage& utm)
{
	return m_client.send(UTM::create(utm));
}