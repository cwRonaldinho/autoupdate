#pragma once

#include "cacti/mtl/ServiceSkeleton.h"
#include "startup/Service.h"

using namespace cacti;

#define INIFILE "./test.conf"

//////////////////////////////////////////////////////////////////////////
// 服务
class TestService :
	public ServiceSkeleton
{
public:
	TestService(u32 port, MessageDispatcher* dispatcher);

	bool init();
	
	void onTraceBeginRequest(const ServiceIdentifier& req, UserTransferMessage& utm);
	void onTraceBeginResponse(const ServiceIdentifier& req, UserTransferMessage& utm);

	void onModuleActive(const ServiceIdentifier& req, UserTransferMessage& utm);
	void onModuleDeactive(const ServiceIdentifier& req, UserTransferMessage& utm);
	void onTimeout(const ServiceIdentifier& req, UserTransferMessage& utm);
private:
	MTL_DECLARE_MESSAGE_MAP;
};

//////////////////////////////////////////////////////////////////////////////
// 公用启动代码
class StarService : public Service
{
public:
	StarService();
	bool start();
	void stop();
	void snapshot() {};

private:
	MessageDispatcher* m_dispatcher;
	TestService* m_service;
};