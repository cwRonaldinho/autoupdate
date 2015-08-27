#include "stdafx.h"

#include "cacti/mtl/PeersManager.h"
#include "cacti/util/IniFile.h"
#include "cacti/util/Timestamp.h"
#include "cacti/logging/FileHandler.h"

#include "tag.h"

namespace cacti
{
	PeersManager::PeersManager(DispatcherInterface* di)
		: m_logger(Logger::getInstance(LOGGER_MTL_PEERS))
		, m_connMap(Logger::getInstance("mtl.connmap"))
		, m_dispatchInterface(di)
	{
		m_logger.setAdditivity(true);

// 		LogHandlerPtr handler(new FileHandler( std::string("./log/peers.map")));
// 		m_connMap.addHandler(handler);
	}

	PeersManager::~PeersManager()
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ApplicationData& ad = it->second;

			if(ad.m_tcpclient)
			{
				delete ad.m_tcpclient;
			}
			++it;
		}
	}

	void PeersManager::loadFromFile(const char* filename)
	{
		IniFile ini;
		ini.open(filename);

		int index = 1;
		char key[64];
		ApplicationData ad;
		while(true)
		{
			snprintf(key, sizeof(key)-1, "peer%d", index++);
			std::string url = ini.readString("peers", key, "");
			if(url.empty())
				break;
			
			std::string::size_type pos = url.find_first_of("@");
			std::string ids;
			std::string address;
			if(pos != std::string::npos)
			{
				ids = url.substr(0, pos-0);
				address = url.substr(pos+1);
				
				std::string::size_type pos = ids.find_first_of("/");
				if(pos != std::string::npos)
				{
					ad.m_appName = ids.substr(0, pos-0);
					ad.m_appid = atoi(ids.substr(pos+1).c_str());
				}
				else
				{
					m_logger.warn("[PM] invalid configure item(%s), no /\n", url.c_str());
					continue;
				}

				pos = address.find_first_of(":");
				if(pos != std::string::npos)
				{
					std::string addr = address.substr(0, pos-0);
					ad.m_connectPort = atoi(address.substr(pos+1).c_str());
					pos = addr.find_first_of("|");
					if(pos != std::string::npos)
					{
						ad.m_primAddress = addr.substr(0, pos-0);
						ad.m_backAddress = addr.substr(pos+1);
					}
					else
					{
						ad.m_primAddress = addr;
						ad.m_backAddress = "";
					}
				}
				else
				{
					m_logger.warn("[PM] invalid configure item(%s), no port\n", url.c_str());
					continue;
				}

				// 
				ad.m_tcpclient = new TransferClient(ad.m_appid, m_dispatchInterface);
				m_applications.insert(std::make_pair(ad.m_appid, ad));

				m_logger.alert("[PM] add peers(%s,%d), primary address(%s) backup address(%s) port(%d)\n",
					ad.m_appName.c_str(),
					ad.m_appid,
					ad.m_primAddress.c_str(), ad.m_backAddress.c_str(), ad.m_connectPort);
			}
			else
			{
				m_logger.warn("[PM] invalid configure item(%s), no @ \n", url.c_str());
				continue;
			}			
		}
	}

	void PeersManager::connectAll()
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ApplicationData& ad = it->second;

			if(ad.m_tcpclient)
			{
				ad.m_tcpclient->start(ad.m_connectPort, ad.m_primAddress.c_str());
				m_logger.info("[PM] connect to (%s:%d)\n", ad.m_primAddress.c_str(), ad.m_connectPort);
			}
			++it;
		}	
	}

	void PeersManager::closeAll()
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ApplicationData& ad = it->second;

			if(ad.m_tcpclient)
			{
				ad.m_tcpclient->exit();
			}
			++it;
		}
		Thread::yield();

		it = m_applications.begin();
		while(it != m_applications.end())
		{
			ApplicationData& ad = it->second;

			if(ad.m_tcpclient)
			{
				ad.m_tcpclient->stop();
			}
			++it;
		}
	}

	int PeersManager::findanonymousApps(u32 appid)
	{
		for(int i=0; i<(int)m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_appid == appid)
				return i;
		}
		return -1;
	}

	bool PeersManager::isServiceActive(u32 app, u32 port)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(app);
		if(it != m_applications.end())
		{
			ApplicationData& ad = it->second;
			return ad.isActiveService(port);
		}
		else
		{
			// check anonymous
			cacti::RecursiveMutex::ScopedLock sclock(m_applock);
			int idx = findanonymousApps(app);
			if(idx == -1)
				return false;

			return m_anonymousApps[idx].isActiveService(port);
		}
	}

	// return 0, no this application
	// return 1, active
	// return 2, reactive
	// return 3, duplicate active
	u32  PeersManager::onServiceActive(u32 app, u32 port, u32 starttime)
	{
		// 这里上锁仅仅是为了锁住对ApplicationData的访问
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);

		std::map<u32, ApplicationData>::iterator it = m_applications.find(app);
		if(it != m_applications.end())
		{
			ApplicationData& ad = it->second;

			// 如果发现有个服务的_apMessageD没有登记在active列表的话，补上
			if(!ad.isActiveService((u32)AppPort::_apMessageD))
				ad.m_activeServices.push_back((u32)AppPort::_apMessageD);
			
			if(!ad.isActiveService(port))
			{
				// first time active 
				if(starttime != 0)
					ad.m_starttimes[port] = starttime;

				ad.m_activeServices.push_back(port);
				dumpConnectionMap();
				return 1;
			}
			else
			{
				// already active
				if(starttime != 0 && ad.m_starttimes[port] != starttime)
				{
					ad.m_starttimes[port] = starttime;
					dumpConnectionMap();
					return 2;
				}
				else
				{
					return 3;
				}
			}
		}
		else
		{
			// check anonymous
			cacti::RecursiveMutex::ScopedLock sclock(m_applock);
			int idx = findanonymousApps(app);
			if(idx == -1)
			{
				// create a new application data for anonymous
				ApplicationData ad;
				ad.m_appid = app;
				ad.m_appName = StringUtil::format("_%s_%d", "_anonymous_", app);
				ad.m_activeServices.push_back(AppPort::_apMessageD);
				ad.m_activeServices.push_back(port);
				if(starttime != 0)
					ad.m_starttimes[port] = starttime;
				m_anonymousApps.push_back(ad);
				dumpConnectionMap();
				return 0;
			}
			else
			{
				ApplicationData& ad = m_anonymousApps[idx];
				if(!ad.isActiveService((u32)AppPort::_apMessageD))
					ad.m_activeServices.push_back((u32)AppPort::_apMessageD);

				if(!ad.isActiveService(port))
				{
					// first time active 
					if(starttime != 0)
						ad.m_starttimes[port] = starttime;

					ad.m_activeServices.push_back(port);
					dumpConnectionMap();
					return 1;
				}
				else
				{
					// already active
					if(starttime != 0 && ad.m_starttimes[port] != starttime)
					{
						ad.m_starttimes[port] = starttime;
						dumpConnectionMap();
						return 2;
					}
					else
					{
						return 3;
					}
				}
			}
		}
	}
	
	void PeersManager::dumpConnectionMap()
	{
		// 这里上锁仅仅是为了锁住对ApplicationData的访问
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);

		m_connMap.info("\n%s:\n", 
			Timestamp::getNowTime().getFormattedTime("%m%d %H:%M:%S").c_str());

		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ApplicationData& ad = it->second;
			m_connMap.info("[%d, %s] \n", 
				ad.m_appid, ad.m_appName.c_str());
			m_connMap.info("\t addr=%s|%s:%d\n", 
				ad.m_primAddress.c_str(), ad.m_backAddress.c_str(), ad.m_connectPort);
			m_connMap.info("\t pasv=%08X\n", ad.m_passiveConnection);
			m_connMap.info("\t svrs=");
			u32 starttime = 0;
			for(size_t i=0; i<ad.m_activeServices.size(); ++i)
			{
				m_connMap.info("%d ", ad.m_activeServices[i]);
				starttime = ad.m_starttimes[ad.m_activeServices[i]];
			}
			m_connMap.info("\n\t boot timestamp:%s\n", Timestamp((time_t)starttime).getFormattedTime("%Y%m%d %H:%M:%S").c_str());
			++it;
		}

		// check anonymous
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			ApplicationData& ad = m_anonymousApps[i];
			m_connMap.info("[%d, %s] \n", 
				ad.m_appid, ad.m_appName.c_str());
			m_connMap.info("\t addr=%s|%s:%d\n", 
				ad.m_primAddress.c_str(), ad.m_backAddress.c_str(), ad.m_connectPort);
			m_connMap.info("\t pasv=%08X\n", ad.m_passiveConnection);
			m_connMap.info("\t svrs=");
			u32 starttime = 0;
			for(size_t i=0; i<ad.m_activeServices.size(); ++i)
			{
				m_connMap.info("%d ", ad.m_activeServices[i]);
				starttime = ad.m_starttimes[ad.m_activeServices[i]];
			}
			m_connMap.info("\n\t boot timestamp:%s\n", Timestamp((time_t)starttime).getFormattedTime("%Y%m%d %H:%M:%S").c_str());
		}
	}

	bool PeersManager::sendPackage(u32 dstappid, NetworkPackage& np)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(dstappid);
		if(it != m_applications.end())
		{
			ApplicationData& ad = it->second;
			if(ad.m_tcpclient)
			{
				return ad.m_tcpclient->sendPackage(np);
			}
		}

		// anonymous application no tcp client
		return false;
	}

	std::vector<u32> PeersManager::getAllPeerApplication()
	{
		std::vector<u32> ret;
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ret.push_back(it->first);
			++it;
		}

		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			ret.push_back(m_anonymousApps[i].m_appid);
		}
		return ret;
	}

	std::vector<ServiceIdentifier> PeersManager::getAllActiveServices(u32 app)
	{
		std::vector<ServiceIdentifier> ret;
		std::map<u32, ApplicationData>::iterator it = m_applications.find(app);
		if(it != m_applications.end())
		{
			ServiceIdentifier sid;
			sid.m_appid = it->first;
			ApplicationData& ad = it->second;
			for(size_t i=0; i<ad.m_activeServices.size(); ++i)
			{
				sid.m_appport = ad.m_activeServices[i];
				if(sid.m_appport != AppPort::_apMessageD)
					ret.push_back(sid);
			}
		}
		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_appid == app)
			{
				ServiceIdentifier sid;
				ApplicationData& ad = m_anonymousApps[i];
				sid.m_appid = ad.m_appid;
				for(size_t i=0; i<ad.m_activeServices.size(); ++i)
				{
					sid.m_appport = ad.m_activeServices[i];
					if(sid.m_appport != AppPort::_apMessageD)
						ret.push_back(sid);
				}

			}
		}
		return ret;
	}

	std::vector<ServiceIdentifier> PeersManager::getAllActiveServices()
	{
		std::vector<ServiceIdentifier> ret;
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			ServiceIdentifier sid;
			sid.m_appid = it->first;
			ApplicationData& ad = it->second;
			for(size_t i=0; i<ad.m_activeServices.size(); ++i)
			{
				sid.m_appport = ad.m_activeServices[i];
				if(sid.m_appport != AppPort::_apMessageD)
					ret.push_back(sid);
			}
			++it;
		}

		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			ServiceIdentifier sid;
			ApplicationData& ad = m_anonymousApps[i];
			sid.m_appid = ad.m_appid;
			for(size_t i=0; i<ad.m_activeServices.size(); ++i)
			{
				sid.m_appport = ad.m_activeServices[i];
				if(sid.m_appport != AppPort::_apMessageD)
					ret.push_back(sid);
			}
		}

		return ret;
	}

	u32 PeersManager::clearPasvConnection(unsigned int pasvid)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.begin();
		while(it != m_applications.end())
		{
			if(it->second.m_passiveConnection == pasvid)
			{
				it->second.m_passiveConnection = TransferServer::INVALID_CONNECTION_ID;
				return it->first;
			}
			++it;
		}

		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_passiveConnection == pasvid)
			{
				m_anonymousApps[i].m_passiveConnection = TransferServer::INVALID_CONNECTION_ID;
				return m_anonymousApps[i].m_appid;
			}
		}
		return 0;
	}

	void PeersManager::updatePasvConnection(const ServiceIdentifier& sid, unsigned int pasvid)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(sid.m_appid);
		if(it != m_applications.end())
		{
			it->second.m_passiveConnection = pasvid;
			return ;
		}
		
		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_appid == sid.m_appid)
			{
				m_anonymousApps[i].m_passiveConnection = pasvid;
				return ;
			}
		}
	}
	unsigned int PeersManager::getPasvConnection(const ServiceIdentifier& sid)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(sid.m_appid);
		if(it != m_applications.end())
		{
			return it->second.m_passiveConnection;
		}

		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_appid == sid.m_appid)
			{
				return m_anonymousApps[i].m_passiveConnection;
			}
		}
		return 0;
	}

	ApplicationData PeersManager::getApplicationInfo(u32 appid)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(appid);
		if(it != m_applications.end())
		{
			return it->second;
		}

		// anonymous
		cacti::RecursiveMutex::ScopedLock sclock(m_applock);
		for(size_t i=0; i<m_anonymousApps.size(); ++i)
		{
			if(m_anonymousApps[i].m_appid == appid)
			{
				return m_anonymousApps[i];
			}
		}
		return ApplicationData();
	}

	bool PeersManager::isPeerDead(u32 appid)
	{
		std::map<u32, ApplicationData>::iterator it = m_applications.find(appid);
		if(it != m_applications.end())
		{
			ApplicationData& ad = it->second;
			if(ad.m_passiveConnection != ActiveServer::INVALID_CONNECTION_ID)
				return false;

			if(!ad.m_tcpclient)
				return true;

			return !ad.m_tcpclient->connected();
		}
		else
		{
			// anonymous
			cacti::RecursiveMutex::ScopedLock sclock(m_applock);
			for(size_t i=0; i<m_anonymousApps.size(); ++i)
			{
				if(m_anonymousApps[i].m_appid == appid)
				{
					if(m_anonymousApps[i].m_passiveConnection != ActiveServer::INVALID_CONNECTION_ID)
						return false;

					// anonymous no tcp client
				}
			}
		}

		return false;
	}
}
