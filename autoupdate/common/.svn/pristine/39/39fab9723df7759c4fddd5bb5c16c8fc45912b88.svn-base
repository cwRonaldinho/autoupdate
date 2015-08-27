#include "StdAfx.h"
#include <stdlib.h>
#include "cacti/mtl/MessageDispatcher.h"
#include "cacti/util/Timestamp.h"
#include "cacti/util/IniFile.h"
#include "cacti/logging/BALog.h"
#include "evtmsg.h"
#include "reqmsg.h"
#include "resmsg.h"

#include <map>

#include <hash_map>

#include "TestService.h"

MTL_BEGIN_MESSAGE_MAP(TestService)
	MTL_ON_MESSAGE(_ReqTraceBegin, onTraceBeginRequest);
	MTL_ON_MESSAGE(_ResTraceBegin, onTraceBeginResponse);
	MTL_ON_MESSAGE(_EvtModuleActive, onModuleActive);
	MTL_ON_MESSAGE(_EvtModuleDeactive, onModuleDeactive);
	MTL_ON_MESSAGE(_EvtTimeOut, onTimeout);
MTL_END_MESSAGE_MAP


TestService::TestService(u32 port, MessageDispatcher* dispatcher)
	: ServiceSkeleton(port, dispatcher) 
{

}

#define TEST_COUNT 10000
#define PER_COUNT  200

static int recCount = 0;

bool TestService::init()
{
	Service::printConsole("My service init\n");

	//for(int i=0; i<TEST_COUNT; ++i)
	//{
	//	if(setTimer(5*1000, UTM::create(myIdentifier(0), myIdentifier(1), _EvtTimeOut)) == ActiveTimer::INVALID_ID)
	//		Service::printConsole("Set timer failed\n");
	//}

	return true;
}

void TestService::onTimeout(const ServiceIdentifier& req, UserTransferMessage& utm)
{
}

void TestService::onTraceBeginRequest(const ServiceIdentifier& req, UserTransferMessage& utm)
{
	recCount++;
	if(recCount % 1000 == 0)
		Service::printConsole("%d, %d\n", (int)time(NULL), recCount);

	//utm.setMessageId(_ResTraceBegin);
	//postMessage(req, utm);
}

void TestService::onTraceBeginResponse(const ServiceIdentifier& req, UserTransferMessage& utm)
{
	//utm.setMessageId(_ReqTraceBegin);
	//postMessage(req, utm);
}

void TestService::onModuleActive(const ServiceIdentifier& req, UserTransferMessage& utm)
{
	Service::printConsole("app(%d) port(%d) active\n", req.m_appid, req.m_appport);

	ServiceIdentifier sid;

	UserTransferMessagePtr rutm = UTM::create(sid, sid, _ReqTraceBegin);
	(*rutm)[1] = "message";
	(*rutm)[2] = (u8)1;
	(*rutm)[3] = (u32)1;
	std::vector<u32> cc(10);
	for(int i=0; i<10; i++)
		cc[i] = i;

	(*rutm)[4] = cc;

	if(req.m_appid == 11)
	{
		Service::printConsole("testing...\n");
		for(int j=0; j<TEST_COUNT; ++j)
		{
			DWORD first = ::GetTickCount();
			for(int i=0; i<PER_COUNT; ++i)
			{
				// send test package to peer
				postMessage(req, rutm);
			}
			Service::printConsole("%d\n", j);
			DWORD last = GetTickCount();
			//if(last - first < 50)
			//{
			//	Thread::sleep(Timestamp(0, (50-(last-first))*1000));
			//}
		}

		Service::printConsole("all test triggered\n");
	}
}

void TestService::onModuleDeactive(const ServiceIdentifier& req, UserTransferMessage& utm)
{
	Service::printConsole("app(%d) port(%d) deactive\n", req.m_appid, req.m_appport);
}

//////////////////////////////////////////////////////////////////////////
StarService::StarService()
{
	m_dispatcher = NULL;
	m_service = NULL;
}

enum MediaType 
{
	stNone=0,
	stTT = 1,
};

bool StarService::start()
{
	BALog logger("test", "");
	logger.open();
	logger.printnt("no time test,%d\n", 1);

	UserTransferMessage utm(ServiceIdentifier(1,2,3), ServiceIdentifier(4,5,6), 7, 8);
	utm[1] = (u8)1;
	utm[2] = (u32)0x12345678;
	utm[3] = "test char*";
	utm[4] = std::string("test string");

	Stream ss;
	utm.encode(ss);

	logger.printb("bin", (const unsigned char*)ss.getWriteBuffer(), (int)ss.size());

	stdext::hash_map<ServiceIdentifier, std::string> testmap;

	ServiceIdentifier sid1(1,1,1);
	ServiceIdentifier sid2(2,2,2);
	testmap[sid1] = "test1";
	testmap[sid2] = "test2";

	printf("%s%s\n", testmap[sid1].c_str(), testmap[sid2].c_str());
	printf("%s\n", testmap.find(sid1) != testmap.end() ? "found" : "not found");
	sid1.m_appid = 2;
	printf("%s\n", testmap.find(sid1) != testmap.end() ? "found" : "not found");
	

	m_dispatcher = new MessageDispatcher();
	if(!m_dispatcher->start(INIFILE))
	{
		stop();
		return false;
	}

	//Thread::sleep(Timestamp(10, 0));
	cacti::IniFile ini;
	ini.open(INIFILE);

	m_service = new TestService(ini.readInt("service", "GUID", 70), m_dispatcher);
	if(!m_service->start())
	{
		Service::printConsole("Start the service failed\n");
		stop();
		return false;
	}
	


	Service::printConsole("All my service started\n");
	return true;
}
void StarService::stop()
{
	if(m_service)
		m_service->stop();
	if(m_dispatcher)
		m_dispatcher->stop();

	delete m_service;
	m_service = NULL;
	delete m_dispatcher;
	m_dispatcher = NULL;
}

boost::shared_ptr<Service> createService()
{
	return boost::shared_ptr<Service>(new StarService);
}
