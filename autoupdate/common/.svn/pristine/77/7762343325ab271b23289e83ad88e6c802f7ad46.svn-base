#ifndef INCLUDED_DISPATCHER_INTERFACE_H
#define INCLUDED_DISPATCHER_INTERFACE_H

#include "cacti/message/TypeDef.h"
#include "cacti/message/ServiceIdentifier.h"
#include "cacti/message/TransferMessage.h"

#pragma once

namespace cacti
{
	// << interface >> 
	class DispatcherInterface
	{
	public:
		virtual bool getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm) = 0;
		virtual bool postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm) = 0;
		virtual bool postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead) = 0;
	};

	class ClientDispatcher : public DispatcherInterface
	{
	private:
		virtual bool getMessage(u32 portid, ServiceIdentifier& req, UserTransferMessagePtr& utm)
		{
			return false;
		}
		virtual bool postMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
		{
			return handleMessage(req.m_appref, utm);
		}
		virtual bool postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead)
		{
			return handleMessage(res.m_appref, utm);
		}
		
	protected:
		virtual bool handleMessage(u32 clientid, UserTransferMessagePtr utm) = 0;
	};

	class ServiceDispatcher : public ClientDispatcher
	{
	public:
		virtual bool postMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, UserTransferMessagePtr utm, bool atHead)
		{
			return handleMessage(req.m_appref, utm);
		}

	};
}
#endif
