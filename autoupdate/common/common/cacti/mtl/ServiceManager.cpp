#include "stdafx.h"

#include "cacti/mtl/ServiceManager.h"
#include "cacti/logging//Logger.h"
#include "tag.h"

#define new DEBUG_NEW

namespace
{
	enum 
	{
		SIZE_PER_QUEUE = 4096,
	};
}

namespace cacti
{
	ServiceManager::ServiceManager()
	{
		m_starttime = (u32) time(NULL);
	}
	void ServiceManager::putPackage(u32 port, NetworkPackagePtr utm)
	{
		NetworkPackageQueuePtr npq = findQueue(port);
		if(npq)
			npq->put(utm);
	}
	bool ServiceManager::tryPutPackage(u32 port, NetworkPackagePtr utm)
	{
		NetworkPackageQueuePtr npq = findQueue( port);
		if(npq)
			return npq->tryPut(utm);
		else
			return false;
	}
	bool ServiceManager::tryPutPackageHead(u32 port, NetworkPackagePtr utm)
	{
		NetworkPackageQueuePtr npq = findQueue( port);
		if(npq)
			return npq->tryPutHead(utm);
		else
			return false;
	}
	NetworkPackagePtr ServiceManager::getPackage(u32 port)
	{
		NetworkPackagePtr utm;

		NetworkPackageQueuePtr npq = findQueue(port);
		if(npq)
			npq->get(utm);

		return utm;
	}
	
	NetworkPackagePtr ServiceManager::getPackage(u32 port, u32 wait)
	{
		NetworkPackagePtr utm;

		NetworkPackageQueuePtr npq = findQueue(port);
		if(npq)
		{
			if(wait != 0)
				npq->get(utm, wait);
			else			
				npq->tryGet(utm);
		}

		return utm;
	}


	bool ServiceManager::addHook(u32 port)
	{	
		RecursiveMutex::ScopedLock sl(m_lock);

		if(findQueue(port))
		{
			return false;
		}
		else
		{
			m_services[port].m_queue = NetworkPackageQueuePtr(new NetworkPackageQueue(SIZE_PER_QUEUE));		
			m_services[port].m_status = ServiceData::SM_HOOK;
			return true;
		}
	}

	void ServiceManager::delHook(u32 port)
	{
		RecursiveMutex::ScopedLock sl(m_lock);
		if(!findQueue(port))
			m_services.erase(port);
	}

	void ServiceManager::setInited(u32 port)
	{
		if(findQueue(port))
		{
			m_services[port].m_status = ServiceData::SM_INITED;
		}
	}
	
	std::vector<u32> ServiceManager::getInitedService()
	{
		RecursiveMutex::ScopedLock sl(m_lock);
		std::vector<u32> allservices;
		ServiceQueue::iterator it = m_services.begin();
		while(it != m_services.end())
		{
			if(it->first != AppPort::_apMessageD && it->second.m_status == ServiceData::SM_INITED)
				allservices.push_back(it->first);
			++it;
		}
		return allservices;
	}

	void ServiceManager::setActiveService(UserTransferMessage& utm, u32 port)
	{
		RecursiveMutex::ScopedLock sl(m_lock);
		ServiceQueue::iterator it = m_services.find(port);
		if(it != m_services.end())
		{
			std::vector<u32> ports;
			ports.push_back(port);
			utm[_TagPortID] = ports;

			std::vector<u8>  firsttime;
			firsttime.push_back(it->second.m_peerNotified);
			it->second.m_peerNotified = 0;
			utm[_TagFirstTimeConnected] = firsttime;

			utm[_TagAppStartTime] = m_starttime;
		}
	}
	
	void ServiceManager::setActiveService(UserTransferMessage& utm)
	{
		RecursiveMutex::ScopedLock sl(m_lock);
		std::vector<u32> allservices;
		std::vector<u8>  firsttime;
		ServiceQueue::iterator it = m_services.begin();
		while(it != m_services.end())
		{
			if(it->first != AppPort::_apMessageD && it->second.m_status == ServiceData::SM_INITED)
			{
				allservices.push_back(it->first);
				firsttime.push_back(it->second.m_peerNotified);
				it->second.m_peerNotified = 0;
			}
			++it;
		}
		utm[_TagPortID] = allservices;
		utm[_TagFirstTimeConnected] = firsttime;
		utm[_TagAppStartTime] = m_starttime;
	}

	NetworkPackageQueuePtr ServiceManager::findQueue(u32 port)
	{
		RecursiveMutex::ScopedLock sl(m_lock);

		ServiceQueue::iterator it = m_services.find(port);
		if(it != m_services.end())
		{
			return it->second.m_queue;
		}
		return NetworkPackageQueuePtr();
	}
}
