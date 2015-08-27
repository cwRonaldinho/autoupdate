#include "stdafx.h"
#include "utmsenderSrv.h"
#include "utmsenderSrvservice.h"

// 全局变量
USService *g_usservice = NULL;

boost::shared_ptr<Service> createService()
{
	return boost::shared_ptr<Service> (new USServer());
}
	
bool USServer::start()
{
	m_pMessageDispatcher = new cacti::MessageDispatcher();

	if( !m_pMessageDispatcher )
	{
		return false;
	}
	if( !m_pMessageDispatcher->start(SYSTEM_CFG_FILE))
	{
		Service::printConsole("MTL running...Failed\n");
		return false;
	}

	// 从配置中读取appport
	m_appport = GetPrivateProfileInt("simulator", "appport", 50, SYSTEM_CFG_FILE);
		
	Service::printConsole("MTL running....OK\n");
	m_pAlarmSrvService = new USService(m_appport, m_pMessageDispatcher);
	if( !m_pAlarmSrvService )
	{
		return false;
	}
	if(!m_pAlarmSrvService->start())
	{
		delete m_pAlarmSrvService;
		m_pAlarmSrvService = NULL;

		m_pMessageDispatcher->stop();
		delete m_pMessageDispatcher;
		m_pMessageDispatcher = NULL;

		return false;
	}
	g_usservice = m_pAlarmSrvService;

	// __TW__TEST
	Service::printConsole("-------------------\n");
	//Service::printConsole("1: create alarm\n2: update alarm\n3: request config\n4: web stop accountting\n5: Update config\n");
	Service::printConsole("\nChosse action: ");

	return true;
}

void USServer::stop()
{
	Service::printConsole("system stop...\n");

	if( m_pAlarmSrvService )
	{
		m_pAlarmSrvService->stop();
		delete m_pAlarmSrvService;
	}

	if( m_pMessageDispatcher)
	{
		m_pMessageDispatcher->stop();
		delete m_pMessageDispatcher;
	}

}

void USServer::snapshot()
{
	m_pAlarmSrvService->SnapShot();
}

// void USServer::createLogger(const char* name)
// {
// 	char path[100];
// 	sprintf_s(path, "./log/%s.log", name);
// 	LogHandlerPtr tasHandler = LogHandlerPtr(new StarFileHandler(path));
// 	StarFormatter* ttic2 = new StarFormatter;
// 	ttic2->logIndex(false);
// 	ttic2->autoNewLine(false);
// 	FormatterPtr ttic2Ptr(ttic2);
// 	tasHandler->setFormatter(ttic2Ptr);
// 	Logger::getInstance(name).addHandler(tasHandler);
// 	Logger::getInstance(name).setLevel(LogLevel::DBG);
// }

void USServer::handleUICommand(std::vector<std::string> & vec)
{
	if (vec[0] == "startsession" || vec[0] == "1")
	{
		m_pAlarmSrvService->m_testUnit.StartSession();
	}
	else if (vec[0] == "mysnapshot" || vec[0] == "2")
	{
		m_pAlarmSrvService->m_testUnit.SnapShot();
	}
	else if (vec[0] == "recognizeresult" || vec[0] == "3")
	{
		m_pAlarmSrvService->m_testUnit.PostRecognizeResult();
	}
	else if (vec[0] == "4")
	{
		m_pAlarmSrvService->m_testUnit.TestTlv();
	}
	else if (vec[0] == "5")
	{
		m_pAlarmSrvService->m_testUnit.TestTlv();
	}

	Service::printConsole("-------------------\n");
	//Service::printConsole("1: create alarm\n2: update alarm\n3: request config\n4: web stop accountting\n5: Update config\n10: load config\n");
	Service::printConsole("Chosse action: ");
}