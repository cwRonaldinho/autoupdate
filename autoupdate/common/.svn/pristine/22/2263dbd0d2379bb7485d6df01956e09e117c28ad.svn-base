#pragma once

#include "cacti/mtl/TransferClient.h"
#include "cacti/message/ServiceIdentifier.h"
using namespace cacti;

class AlarmTransferClient : public TransferClient
{
	friend class AlarmSender;
public:
	AlarmTransferClient(DispatcherInterface* di);
	void registerService(const std::string& id);
	void registerService();
	bool send(UserTransferMessagePtr utm);

private:
	virtual void onConnected();

private:
	std::string m_serviceId;
	std::string m_procname;
	std::string m_version;
	std::string m_dispName;
	std::string m_starttime;
	std::string m_path;
};

class AlarmSender
{
public:
	static void createInstance(DispatcherInterface* disp, u_short port, const char* version, const char* dispname);
	static void destroyInstance();
	static AlarmSender* getInstance();
	void registerService() { m_client.registerService(); }
	void registerService(const std::string& id){	m_client.registerService(id);	}
	bool send(ServiceIdentifier who, const char* alarmid, u32 level, const strarray& params);
	bool console(const char* msg);
	bool unregisterService();
	bool sendUTM(UserTransferMessage& utm);

private:
	AlarmSender(DispatcherInterface* disp);
	bool handleMessage(UserTransferMessagePtr utm);

private:
	static AlarmSender* m_instance;
	static u_short m_port;
	AlarmTransferClient m_client;
};
