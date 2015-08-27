#include "stdafx.h"

#include "MessageDispatcher.h"

#include "ServiceSkeleton.h"
#include <cacti/config/xhash_map.h>
#include "alarm/AlarmSender.h"
#include "evtmsg.h"
#include "sxconst.h"
#include "sxcode.h"

#define new DEBUG_NEW

namespace cacti
{
	ServiceSkeleton::ServiceSkeleton(u32 portid, MessageDispatcher* dispatcher)
		: m_logger(Logger::getInstance(LOGGER_MTL_SKELETON))
		, m_stopSignaled(true)
	{
		assert(dispatcher);
		m_dispatcher = dispatcher;

		m_identifier.m_appid   = m_dispatcher->appId();
		m_identifier.m_appport = portid;
		m_logger.setAdditivity(true);
	}

	bool ServiceSkeleton::start()
	{
		// we set APPID here, because in ctor MessageDispatcher maybe not running yet!
		m_identifier.m_appid   = m_dispatcher->appId();
		m_cfgServerId.m_appid = m_dispatcher->cfgAppId();
		m_cfgServerId.m_appport = AppPort::_apCfgServer;
		m_serviceName = m_dispatcher->getAppName();
		if(!m_dispatcher->hook(m_identifier.m_appport))
		{
			m_logger.fatal("[%04d] HOOK failed! check duplicate port id\n",m_identifier.m_appport);
			return false;
		}

		if(!init())
		{
			m_logger.fatal("[%04d] INIT failed!\n",m_identifier.m_appport);
			stop();
			return false;
		}
		m_stopSignaled = false;
		m_pullthread.start(this, &ServiceSkeleton::pullMessage);
		
		m_dispatcher->inited(m_identifier.m_appport);

		m_logger.alert("[%04d] START\n", 
			m_identifier.m_appport);
		
		return true;
	}
	void ServiceSkeleton::stop()
	{
		uninit();

		if(!m_stopSignaled)
		{
			m_stopSignaled = true;
			UserTransferMessagePtr utm = UTM::create(m_identifier, m_identifier, _EvtStopPull);
			postSelfMessage(utm);
			m_pullthread.join();
		}
		m_dispatcher->unhook(m_identifier.m_appport);

		// method 1:
		//APV apv;
		//apv.push_back(m_displayName);			// for string value
		//apv.push_back(m_identifier.m_appid);	// for u32 value
		//alarm("mtl.service_exit", AL_NORMAL, apv);
		
		// method 2:
		strarray params;
		params.push_back(m_serviceName);		// for string value
		params.push_back(u32tos(m_identifier.m_appid));	// for u32 value
		alarm("mtl.service_exit", AL_NORMAL, params);

		// or method 3:
		//strarray params(5);		// fix size
		//params[1] = m_displayName;		// for string value
		//params[3] = u32tos(m_identifier.m_appid);	// for u32 value
		//alarm("mtl.service_exit", AL_NORMAL, params);

		m_logger.alert("[%04d] STOP\n", m_identifier.m_appport);
	}

	bool ServiceSkeleton::getMessage(ServiceIdentifier& req, UserTransferMessagePtr& utm)
	{
		return m_dispatcher->getMessage(m_identifier.m_appport, req, utm);
	}
	bool ServiceSkeleton::postMessage(const ServiceIdentifier& receiver, const UserTransferMessage& utm)
	{
		return postMessage(receiver, UTM::create(utm));
	}
	bool ServiceSkeleton::postMessage(const ServiceIdentifier& receiver, UserTransferMessagePtr utm)
	{
		return m_dispatcher->postMessage(m_identifier, receiver, utm);
	}
	bool ServiceSkeleton::postSelfMessage(const UserTransferMessage& utm)
	{
		return postSelfMessage(UTM::create(utm));
	}
	bool ServiceSkeleton::postSelfMessage(UserTransferMessagePtr utm)
	{
		return m_dispatcher->postMessage(m_identifier, m_identifier, utm, true);
		return false;
	}

	ServiceIdentifier ServiceSkeleton::myIdentifier(u32 ref /*= 0*/)
	{
		ServiceIdentifier tmp = m_identifier;
		tmp.m_appref = ref;
		return tmp;
	}
	ServiceIdentifier ServiceSkeleton::cfgServerIdentifier()
	{
		return m_cfgServerId;
	}
	bool ServiceSkeleton::isRemoteConfigure()
	{
		return m_dispatcher->isRemoteConfigure();
	};

	TimerID ServiceSkeleton::setTimer(u32  expires, UserTransferMessagePtr utm)
	{
		return m_dispatcher->setTimer(expires, m_identifier, utm);
	}

	void ServiceSkeleton::killTimer(TimerID id)
	{
		m_dispatcher->killTimer(id);
	}

	void ServiceSkeleton::alarm(const char* alarmid, u32 level, const std::vector<AlarmParams>& params)
	{
		AlarmSender* alarm = AlarmSender::getInstance();
		if(alarm)
		{
			strarray pp(params.size());
			for(size_t i=0; i<params.size(); ++i)
				pp[i] = params[i].value;

			alarm->send(myIdentifier(), alarmid, level, pp);
		}
	}

	void ServiceSkeleton::alarm(const char* alarmid, u32 level, const strarray& params)
	{
		AlarmSender* alarm = AlarmSender::getInstance();
		if(alarm)
		{
			alarm->send(myIdentifier(), alarmid, level, params);
		}
	}

	std::string& ServiceSkeleton::getServiceName()
	{
		return m_serviceName;
	}
	MessageDispatcher* ServiceSkeleton::getDispatcher()
	{
		return m_dispatcher;
	}

	bool ServiceSkeleton::init()
	{
		return true;
	}
	void ServiceSkeleton::uninit()
	{
		return ;
	}

	// call then the service is ready
	void ServiceSkeleton::ready()
	{
		// @TODO 
		// send ready message to all peers!
	}

	void ServiceSkeleton::pullMessage()
	{
		m_logger.info("[%04d] pull thread running...\n", m_identifier.m_appport);
		try
		{
			while(!m_stopSignaled)
			{
				ServiceIdentifier req;
				UserTransferMessagePtr utm;
				if(!m_dispatcher->getMessage(m_identifier.m_appport, req, utm))
				{
					m_logger.fatal("[%04d] pull failed\n", 
						m_identifier.m_appport);
				}
				else if(!utm)
				{
					m_logger.fatal("[%04d] pull null ptr\n", 
						m_identifier.m_appport);
				}
				else
				{
					// reset the req's appref, because it's value only used for MTL itself			
					req.m_appref = 0;
					onSystemMessage(req, utm);
				}
			}

		}
		catch (...)
		{
			m_logger.fatal("[%04d] pull thread exception\n",m_identifier.m_appport);
			HANDLE proc=OpenProcess(PROCESS_ALL_ACCESS,FALSE,getpid());
			TerminateProcess(proc,0);
		}
		m_logger.fatal("[%04d] pull thread stopped!\n", m_identifier.m_appport);
	}

	void ServiceSkeleton::onSystemMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		switch(utm->getMessageId())
		{
		case _EvtStopPull:
			if(req == m_identifier)
			{
				m_logger.info("[%04d] stop event signaled\n", 
					m_identifier.m_appport);
			}
			else
			{
				m_logger.info("[%04d] invalid stop event from %s\n",
					m_identifier.m_appport, req.toString().c_str());
			}
			break;
		default:
			{
				onMessagePtr(req, utm);
			}
		}
	}
	void ServiceSkeleton::onMessagePtr(const ServiceIdentifier& req, UserTransferMessagePtr utm)
	{
		onMessage(req, *utm);
	}
	void ServiceSkeleton::onDefaultMessage(const ServiceIdentifier& req, UserTransferMessage utm)
	{
		m_logger.debug("[%04d] default message(%s, %s, %X) from (%s) bypass\n", 
			m_identifier.m_appport,
			utm.getReq().toString().c_str(),
			utm.getRes().toString().c_str(),
			utm.getMessageId(),
			req.toString().c_str());
	}
}
