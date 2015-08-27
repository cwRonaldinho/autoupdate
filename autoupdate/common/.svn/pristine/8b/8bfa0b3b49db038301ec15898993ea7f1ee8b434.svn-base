#ifndef INCLUDED_MTL_SERVICEMANAGER_H
#define INCLUDED_MTL_SERVICEMANAGER_H

#pragma once

#include "cacti/kernel/SysObj.h"
#include "cacti/util/BoundedQueue.h"
#include "cacti/message/TransferMessage.h"

#include <map>

namespace cacti
{
	typedef BoundedQueue<NetworkPackagePtr> NetworkPackageQueue;	// network queue
	typedef boost::shared_ptr<NetworkPackageQueue> NetworkPackageQueuePtr;

	struct ServiceData
	{
		ServiceData()
		{
			m_status = SM_NONE;
			m_peerNotified = 1;
		}

		enum 
		{
			SM_NONE,
			SM_HOOK,
			SM_INITED,
		}m_status;
		u8 m_peerNotified;
		NetworkPackageQueuePtr m_queue;
	};

	typedef std::map<u32, ServiceData> ServiceQueue;		// port to network queue
	// 注意：put和get不能上锁，否则会锁死！
	class ServiceManager
	{
	public:		
		ServiceManager();
		void putPackage(u32 port, NetworkPackagePtr utm);
		bool tryPutPackage(u32 port, NetworkPackagePtr utm);
		bool tryPutPackageHead(u32 port, NetworkPackagePtr utm);
		NetworkPackagePtr getPackage(u32 port);
		NetworkPackagePtr getPackage(u32 port, u32 wait);
		
		bool addHook(u32 port);
		void delHook(u32 port);
		
		void setInited(u32 port);
		
		std::vector<u32> getInitedService();
		void setActiveService(UserTransferMessage& utm);
		void setActiveService(UserTransferMessage& utm, u32 port);

		void lock()			{ m_lock.lock();	};
		void unlock()		{ m_lock.unlock();	};
	private:
		NetworkPackageQueuePtr findQueue(u32 port);

	private:
		u32 m_starttime;
		ServiceQueue m_services;
		RecursiveMutex m_lock;
	};
}

#endif

