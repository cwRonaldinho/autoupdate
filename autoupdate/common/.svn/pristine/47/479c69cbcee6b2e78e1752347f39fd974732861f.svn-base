#ifndef INCLUDED_CACTI_ACTIVECLIENT_H
#define INCLUDED_CACTI_ACTIVECLIENT_H

#pragma once

#include "cacti/socket/ActiveClient.h"
#include "cacti/socket/SocketWrapper.h"
#include "cacti/kernel/SysObj.h"
#include "cacti/logging/Logger.h"

namespace cacti
{
	class ActiveClient
	{
	public:
		ActiveClient(int id, const char* loggername = "socket.aclt");
		virtual ~ActiveClient();

		void setReconnectInterval(int interval);
		//start the service
		// delay -- true  : connection will be established in the future
		//			false : block until the connection established
		void setLocalAddress(u_short port,const char *addr);
		bool			start(u_short port, const char *addr, bool delay = true);
		void			exit();
		//stop the service
		void			stop(int timeout = ~0);

		bool			connected();

		//main interface for user call :
		int				send(const char *buf, int bufsize);

	private:
		bool connect();

		void setRecover();
		bool isRecovering();
		bool waitRecover();

		virtual void    threadProcess();
		void			handleBroken();
		virtual void	onConnected()		{ };
		virtual void	onBroken()			{ };
		virtual int		onReceive() = 0;
	protected:
		int				m_id;
		SocketConnector m_socket;
		cacti::Logger&  m_logger;
		bool			m_alive;
		volatile bool	m_exitSignaled;
		cacti::Thread   m_thread;

	private:

		bool			m_recovering;
		int				m_reconnectInterval;
		cacti::Event	m_connectedEvent;

		RecursiveMutex  m_sendnMutex;

		enum 
		{
			CHECK_RECONNECT_INTERVAL = 500,
			DEFAULT_RECONNECT_INTERVAL = 3,		// reconnect timer interval (s)
		};
	};
}
#endif	//INCLUDED_CACTI_ACTIVECLIENT_H
