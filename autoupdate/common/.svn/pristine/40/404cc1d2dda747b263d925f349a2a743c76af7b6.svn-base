#ifndef INCLUDED_MTL_SERVICESTATUSMANAGER_H
#define INCLUDED_MTL_SERVICESTATUSMANAGER_H

#pragma once

#include "cacti/kernel/SysObj.h"
#include "cacti/message/TypeDef.h"
#include "cacti/message/TransferMessage.h"
#include "cacti/socket/SocketAddress.h"
#include "cacti/logging/Logger.h"
#include "cacti/mtl/TransferClient.h"
#include "cacti/mtl/TransferServer.h"
#include <map>
// 
// peers manager 
// manage all application, include myself and all connected peers
// every application contains the application data
// an application data will have many service inside
// service data store the application's service's status data
// 

namespace cacti
{
	struct ApplicationData
	{
		ApplicationData()
			: m_tcpclient(0)
			, m_appid(0)
			, m_connectPort(0)
			, m_passiveConnection(TransferServer::INVALID_CONNECTION_ID)
		{
			m_activeServices.reserve(64);
		}
		bool isActiveService(u32 port)
		{
			return std::find(m_activeServices.begin(), m_activeServices.end(), port) != m_activeServices.end();
		}
		u32			  m_appid;
		std::string   m_appName;

		std::string   m_primAddress;
		std::string   m_backAddress;
		u_short		  m_connectPort;

		unsigned int  m_passiveConnection;
		TransferClient* m_tcpclient;
		std::map<u32, u32> m_starttimes;
		std::vector<u32> m_activeServices;
	};

	class PeersManager
	{
	public:
		PeersManager(DispatcherInterface* di);
		~PeersManager();
		void loadFromFile(const char* filename);

		void connectAll();
		void closeAll();
		void dumpConnectionMap();
	
		bool isServiceActive(u32 app, u32 port);

		// deprecated method
		u32  onServiceActive(u32 app, u32 port, u32 starttime);

		std::vector<u32> getAllPeerApplication();
		std::vector<ServiceIdentifier> getAllActiveServices();
		std::vector<ServiceIdentifier> getAllActiveServices(u32 app);

		bool sendPackage(u32 dstappid, NetworkPackage& np);
		u32  clearPasvConnection(unsigned int pasvid);
		void updatePasvConnection(const ServiceIdentifier& sid, unsigned int pasvid);
		unsigned int getPasvConnection(const ServiceIdentifier& sid);
		ApplicationData getApplicationInfo(u32 appid);
		bool isPeerDead(u32 appid);
	
	private:
		int findanonymousApps(u32 appid);

	private:
		std::map<u32, ApplicationData> m_applications;	// app id to application

		cacti::RecursiveMutex m_applock;
		std::vector<ApplicationData> m_anonymousApps;	// anonymous application

		cacti::Logger& m_logger;
		cacti::Logger& m_connMap;
		DispatcherInterface* m_dispatchInterface;
	};
}

#endif // INCLUDED_MTL_SERVICESTATUSMANAGER_H
