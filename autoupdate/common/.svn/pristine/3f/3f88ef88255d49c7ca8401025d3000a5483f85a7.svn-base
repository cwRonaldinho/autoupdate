#pragma once

#include <cacti/kernel/SysObj.h>
#include <cacti/message/TLV.h>
#include <cacti/logging/Logger.h>

#include <boost/shared_ptr.hpp>

#include <vector>

#include "Service.h"

class Commander
{
public:
	Commander();
	~Commander();
	bool startService(u_short alarmport);
	void stopService();

	void handleUICommand(const char* cmd);
	const char*  serviceVersion();
	const char*  serviceName();
	const char*  displayName();

private:
	boost::shared_ptr<Service>	m_service;	// Ä£¿é
};

typedef std::map<std::string, std::string> CmdOptionMap;
class CmdOption
{

public:
	static void setCmdOption(int argc, char* argv[]);
	static bool hasCmdOption(std::string option);
	static std::string getCmdOption(std::string option);

private:
	static CmdOptionMap m_cmdOptionMap;
	static cacti::RecursiveMutex m_lock;


};