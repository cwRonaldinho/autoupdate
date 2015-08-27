#ifndef INCLUDED_MTL_SERVICESKELETON_H
#define INCLUDED_MTL_SERVICESKELETON_H

#pragma once

#include "cacti/message/TransferMessage.h"
#include "cacti/kernel/SysObj.h"
#include "cacti/logging/Logger.h"
#include "cacti/util/Timer.h"
#include "alarm/AlarmParams.h"
#include "alarm/AlarmSender.h"

#include "evtmsg.h"

namespace cacti
{

	class MessageDispatcher;

	class ServiceSkeleton
	{
	public:
		ServiceSkeleton(u32 portid, MessageDispatcher* dispatcher);
		// core function
		bool start();
		void stop();
		bool getMessage(ServiceIdentifier& req, UserTransferMessagePtr& utm);
		bool postMessage(const ServiceIdentifier& receiver, const UserTransferMessage& utm);
		bool postMessage(const ServiceIdentifier& receiver, UserTransferMessagePtr utm);
		bool postSelfMessage(const UserTransferMessage& utm);
		bool postSelfMessage(UserTransferMessagePtr utm);
		ServiceIdentifier myIdentifier(u32 ref = 0);
		ServiceIdentifier cfgServerIdentifier();

		bool isRemoteConfigure();
		// timer function
		TimerID setTimer(u32  expires, UserTransferMessagePtr utm);
		void killTimer(TimerID id);
		// alarm function
		void alarm(const char* alarmid, u32 level, const std::vector<AlarmParams>& params);
		void alarm(const char* alarmid, u32 level, const strarray& params);
		std::string& getServiceName();
		MessageDispatcher* getDispatcher();
	protected:
		// do the service initialize action
		virtual bool init();
		// do the service un-initialize action
		virtual void uninit();
		
		// call then the service is ready
		void ready();

		// switch default case 
		void onDefaultMessage(const ServiceIdentifier& req, UserTransferMessage utm); 
	private:
		void pullMessage();
		void onSystemMessage(const ServiceIdentifier& req, UserTransferMessagePtr utm);
		virtual void onMessagePtr(const ServiceIdentifier& req, UserTransferMessagePtr utm);
		virtual void onMessage(const ServiceIdentifier& req, UserTransferMessage& utm) {};

	private:
		Logger&				m_logger;
		MessageDispatcher*	m_dispatcher;		// 
		ServiceIdentifier	m_cfgServerId;		// 
		ServiceIdentifier	m_identifier;		// identifier of myself
		std::string			m_serviceName;		// display name -- from config server
		volatile bool		m_stopSignaled;		//
		Thread				m_pullthread;		// pull thread for the message
		
	};


#define MTL_DECLARE_MESSAGE_MAP \
	void onMessage(const ServiceIdentifier& req, UserTransferMessage& utm);

#define	MTL_MESSAGE_DECL(method)		\
	void method(const ServiceIdentifier& req, UserTransferMessage& utm);

#define	MTL_MESSAGE_IMPL(ClassName, method)		\
	void ClassName::method(const ServiceIdentifier& req, UserTransferMessage& utm)

#define MTL_BEGIN_MESSAGE_MAP(ClassName) \
	void ClassName::onMessage(const ServiceIdentifier& req, UserTransferMessage& utm)  { \
		switch(utm.getMessageId())  	{

#define MTL_ON_MESSAGE(messageID, funp) \
	case messageID: \
		funp(req,utm); \
		break;	

#define MTL_END_MESSAGE_MAP  \
	  default: onDefaultMessage(req,utm);  } }
	
}
#endif	//INCLUDED_MTL_SERVICESKELETON_H
