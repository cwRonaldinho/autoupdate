#pragma once

#include <cacti/message/TransferMessage.h>
#include <cacti/logging/Logger.h>
#include <cacti/mtl/DispatcherInterface.h>
#include <boost/shared_ptr.hpp>

#include <string>

using namespace cacti;


/************************************************************************/
/* CaCTI module boot entry												*/
/************************************************************************/
class Service : public DispatcherInterface
{
public:
	Service(std::string name, std::string version, std::string dispName);
	virtual ~Service(void);

	virtual bool start() = 0;
	virtual void stop() = 0;

	const char* versioninfo();
	const char* name();
	const char* version();
	const char* displayName();

	void	dispatchUICommand(std::vector<std::string> & vec);
	static void printConsole(const char* fmt, ...);
	static void printConsole(cacti::LogControlID level, const char* fmt, ...);
	static void exit(int exitcode =0);

private:
	void Service::handleMessage(cacti::UserTransferMessage& utm);
	// handle other command
	virtual void handleUICommand(std::vector<std::string> & vec){};
	// UI interfaces
	virtual void help(){};
	virtual void snapshot(){};
	virtual void reload(){};
	virtual void logger(int newlevel){};

	void commonHelp();
	void changeSysLogger(int newlevel);
	// global display function
	static const char* getConsoleLoggerName();

private:
	virtual bool getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm);
	virtual bool postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm);
	virtual bool postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead);

private:
	std::string m_name;
	std::string m_version;	
	std::string m_displayName;
	std::string m_versionInfo;
	static Thread::thread_id m_mainThread;

};

// return the real object
boost::shared_ptr<Service> createService();

// derived class must implement this method and return a "Service" implementation
//static Service* getSystem();
