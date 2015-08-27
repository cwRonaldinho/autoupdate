#include "stdafx.h"

#include "cacti/socket/ActiveClient.h"
#include "cacti/util/StringUtil.h"
#include "cacti/kernel/SysObj.h"

#define new DEBUG_NEW

namespace cacti
{
	ActiveClient::ActiveClient(int id, const char* loggername /*= LOGGER_SOCKET_CLIENT*/)
		: m_logger(Logger::getInstance(loggername))
		, m_id(id)
	{
		m_logger.setAdditivity(true);

		m_alive = false;
		m_reconnectInterval = DEFAULT_RECONNECT_INTERVAL;
		setRecover();
	}

	ActiveClient::~ActiveClient()
	{
		stop();
	}

	void ActiveClient::setReconnectInterval(int interval)
	{
		m_reconnectInterval = interval;
	}
	void ActiveClient::setLocalAddress(u_short port,const char *addr){
		m_socket.setLocalAddress(SocketAddress(addr, port));
	}

	bool ActiveClient::start(u_short port, const char *addr, bool delay /*= true*/)
	{	
		if(!m_alive)
		{
			m_socket.setServerAddress(SocketAddress(addr, port));
			if(delay)
			{
				// if delay, don't do connect at this time, just start the active thread
				m_thread.start(this, &ActiveClient::threadProcess);
				m_alive = true;
			}
			else
			{
				// if no delay
				// connect immediately, and if connect failed, don't start active thread 
				// if you want to check the server status immediately 
				// just set delay is false
				if(this->connect()) 
				{
					m_thread.start(this, &ActiveClient::threadProcess);
					m_alive = true;
				}
			}
		}
		return connected();
	}

	//stop the service
	void ActiveClient::stop(int timeout)
	{
		if(m_alive)
		{
			m_exitSignaled = true;
			m_socket.close();
			m_thread.join();
			m_logger.warn("[%04d] stopped\n", m_id);
			m_alive = false;
		}
	}

	void ActiveClient::exit()
	{
		if(m_alive)
		{
			m_exitSignaled = true;
		}
	}

	bool ActiveClient::connect()
	{
		if(!connected())
		{						
			m_logger.info("[%04d] CNT: server(%s:%d), local port(%d)\n",
				m_id, 
				m_socket.getServerName(), 
				m_socket.getServerPort(),
				m_socket.getLocalPort());
			if(m_socket.connect())
			{
				m_logger.info("[%04d] CNT: connected, SOCKET(%d)\n", 
				m_id,
				m_socket.getHandle());

				onConnected();
			}
			else
			{
				m_logger.info("[%04d] CNT: failed\n", m_id);
			}
		}
		m_connectedEvent.signal();
		return m_socket.isConnected();
	}

	// main interface for user call
	int ActiveClient::send(const char *buf, int buf_size)
	{
		if(connected())																// and recover OK
		{
			if(m_logger.isDebugEnabled())
			{
				std::string binbuf;
				m_logger.debug("[%04d] SND: %s\n", 
					m_id,
					cacti::StringUtil::formatBinary((unsigned char*)buf, buf_size, binbuf));
			}
			else
			{
				m_logger.info("[%04d] SND: %d bytes\n", 
					m_id, 
					buf_size);
			}

			RecursiveMutex::ScopedLock lock(m_sendnMutex);
			return m_socket.sendn(buf, buf_size);
		}
		else
		{
			m_logger.info("[%04d] WARN: not connected\n", m_id);
			return SOCKET_ERROR;
		}
	}

	bool ActiveClient::connected()
	{
		return m_socket.isConnected();
	}
 
	// receive process
	void ActiveClient::threadProcess()
	{
		m_exitSignaled = false;
		int ret;
		int looptime = m_reconnectInterval*1000;
		int sleeptime = CHECK_RECONNECT_INTERVAL*1000;
		while(true)
		{
#ifndef _DEBUG
			try
			{
#endif // _DEBUG
				while(!connected() && !m_exitSignaled)
				{
					m_logger.info("[%04d] RCT...\n", m_id);
					connect();
					if(connected() || m_exitSignaled)
						break;
					for(int i=0; i<looptime && !m_exitSignaled; i+=CHECK_RECONNECT_INTERVAL)
					{
						cacti::Thread::sleep(Timestamp(0, sleeptime));
					}
					if(m_exitSignaled)
						break;
				}
				if(m_exitSignaled)
					break;
				
				timeval tv;
				tv.tv_sec = 60;
				tv.tv_usec = 0;
				int rc = m_socket.recvReady(&tv);

				if(m_exitSignaled)
					break;

				if(rc == 1)
				{
					//RECV_OK             // recv ok
					//RECV_CLOSE          // connection closed
					//RECV_SOCKET_ERROR   // socket error
					//RECV_USER_ERROR     // user error < 0
					ret = onReceive();
					switch(ret)
					{
					case RECV_OK:
						{
							m_logger.info("[%04d] Received\n", m_id);
						}
						break;
					case RECV_CLOSE:
						{
							m_logger.warn("[%04d] Closed by server\n", m_id);
						}
						break;
					case RECV_SOCKET_ERROR:
						{
							std::string errmsg;
							m_logger.error("[%04d] %s\n", 
								m_id,
								SysError::lastErrorMessage(errmsg));
						}
						break;
					default:	//user failed
						{
						}
						break;
					}
					if(ret != RECV_OK)
					{
						handleBroken();
					}
				}
				else if(rc == 0)
				{
					m_logger.debug("[%04d] SLT timeout\n", m_id);
				}
				else
				{
					handleBroken();
				}
#ifndef _DEBUG
			}
			catch(...)
			{
				m_logger.log(LogLevel::FATAL, m_socket.getHandle(), "exception\n");
			}
#endif // _DEBUG
		}
		m_logger.warn("[%04d] exited\n", m_id);
	}

	void ActiveClient::setRecover()
	{
		m_recovering = true;
	}

	bool ActiveClient::isRecovering()
	{
		return m_recovering;
	}

	bool ActiveClient::waitRecover()
	{
		// at most wait for twice of reconnect interval times;
		m_connectedEvent.wait(m_reconnectInterval+m_reconnectInterval/2);
		return connected();
	}

	void ActiveClient::handleBroken()
	{
		m_socket.disconnect();
		onBroken();
		setRecover();
		m_logger.log(LogLevel::DBG, m_id, "BRK\n");
	}
}
