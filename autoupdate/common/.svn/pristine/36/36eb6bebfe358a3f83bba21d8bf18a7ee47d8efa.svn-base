#include "stdafx.h"

#include "cacti/socket/ActiveServer.h"
#include "cacti/util/StringUtil.h"

#define new DEBUG_NEW

namespace cacti
{
    using std::string;

    static const int REPARE_TIME	= 3000;
      
    ActiveServer::ActiveServer(const char* loggername /*= LOGGER_SOCKET_SERVER*/)
		: m_logger(Logger::getInstance(loggername))
    {
		m_logger.setAdditivity(true);
        reset();
    }
    
    ActiveServer::~ActiveServer()
    {
        stop();
    }

    void ActiveServer::reset()
    {
        m_running   = false;
        m_socket.invalid();
        m_exitSignaled = false;
        m_clients   = 0;
    }
    
    bool ActiveServer::start(u_short port, const char *addr/* = NULL*/)
    {
        if(!m_running)
        {
            if(m_socket.bind(SocketAddress(addr, port)))
            {
                m_running = true;
                startListener();
            }
            else
            {				
				std::string errmsg;
				m_logger.log(LogLevel::ERR, "[SERVER] bind failed, %s\n", 
					SysError::lastErrorMessage(errmsg));
            }
        }
        return m_running;
    }
    
    void ActiveServer::startListener()
    {
        m_exitSignaled = false;
		m_thread.start(this, &ActiveServer::threadProcess);
    }
    
    void ActiveServer::stop(int timeout/* = INFINITE*/)
    {
        if(m_running)
        {
            m_exitSignaled = true;
            //force the select action failed and return;
			SocketAddress selfaddr(m_socket.getHost(), m_socket.getPort());
			SocketConnector conn(selfaddr);
			if(!conn.connect())
			{
				m_logger.log(LogLevel::WARN, "can't trigger server stop\n");
				m_socket.close();
			}
			else
			{
				conn.disconnect();
			}

			m_thread.join();
			m_socket.close();

            m_running = false;
            cleanClients();
            // reset running stat 
            reset();
        }
    }
    
    int ActiveServer::send(unsigned int clientid, const char *buf, int buf_size)
    {
        AcceptedSocket client = getClientSocket(clientid);

        if(client.isValid())
        {
			if(m_logger.isDebugEnabled())
			{
				std::string binbuf;
				m_logger.log(LogLevel::DBG, clientid, "SND: %s\n", 
					cacti::StringUtil::formatBinary((unsigned char*)buf, buf_size, binbuf));
			}
			else
			{
				m_logger.log(LogLevel::INFO, clientid, "SND: %d bytes\n", buf_size);
			}
			RecursiveMutex::ScopedLock lock(m_sendnMutex);
			return client.sendn(buf, buf_size);
        }
        else
        {
			m_logger.log(LogLevel::ERR, clientid, "can't find the special client id\n");
            return SOCKET_ERROR;
        }
    }
    
    SOCKET ActiveServer::makeSelector()
    {
        FD_ZERO(&m_readfds);
        //this server
        m_socket.setSelect(&m_readfds);

		SOCKET maxSocket(m_socket);					//using SOCKET operator
        for(int i=0; i<m_clients; ++i)
        {
            m_client[i].m_socketid.setSelect(&m_readfds);
			if (maxSocket<m_client[i].m_socketid)	//using SOCKET operator
				maxSocket=m_client[i].m_socketid;
        }        
		return maxSocket;
    }
    
    void ActiveServer::recover()
    {
        m_logger.fatal("[SERVER] server recover begin ...\n");
        //close all connection
        cleanClients();
        m_logger.fatal("[SERVER] server recover end\n");
    }
    
    //check the errcode and return true if that error reparable.
    bool ActiveServer::reparable(int errcode)
    {
        return false;
    }
    
    bool ActiveServer::insertClient(const AcceptedSocket &client)
    {
        if(m_clients < MAX_CLIENT)
        {
            m_client[m_clients].m_clientid = m_seqGenerator.next();
            m_client[m_clients].m_socketid = client;

			if(!onAccept(m_client[m_clients]))
			{
				m_logger.log(LogLevel::ERR, m_client[m_clients].getId(), "Not accept\n");
				return false;
			}
			m_logger.log(LogLevel::INFO, m_client[m_clients].getId(), "Accepted, SOCKET(0x%X), %s:%d\n",
                m_client[m_clients].getSocket(), client.getPeerName(), client.getPeerPort());
            ++m_clients;
            return true;
        }
        return false;
    }
    
    void ActiveServer::eraseClient(int index)
    {
        if(index <0 || index > m_clients)
            return ;
        
        m_logger.log(LogLevel::INFO, m_client[index].getId(), "Deleted, SOCKET(0x%X)\n",
                                          m_client[index].getSocket());
        
        onBroken(m_client[index]);
        m_client[index].m_socketid.close();
        //move the last value into i's position
        //the last position will be fill by other insert action
        //using the operator=
        if(index != (--m_clients))
            m_client[index] = m_client[m_clients];
    }
    
	void ActiveServer::stopClient(unsigned int clientId)
	{
		for(int i = 0; i < m_clients; ++i)
		{
			if(m_client[i].m_clientid == clientId)
			{
				eraseClient(i);
				return;
			}
		}

	}

    void ActiveServer::cleanClients()
    {
        for(int i=0; i< m_clients; ++i)
        {
            m_logger.log(LogLevel::INFO, m_client[i].getId(), "Deleted, SOCKET(0x%X)\n",
                m_client[i].getSocket());
            onBroken(m_client[i]);
            m_client[i].m_socketid.close();
        }
        //reset client number counter
        m_clients = 0;
    }
    
    AcceptedSocket ActiveServer::getClientSocket(unsigned int clientid)
    {
        for(int i=0; i<m_clients; ++i)
        {
            if(m_client[i].m_clientid == clientid)
                return m_client[i].m_socketid;
        }
        // not find socket, just return an invalid socket
        return AcceptedSocket();
    }
    
    //the main job of this thread
    void ActiveServer::threadProcess()
    {
        int ret;
		m_logger.log(LogLevel::INFO, 0, "Server running..., listening(%s:%d)\n",
			m_socket.getHost(), m_socket.getPort());
        for(;;)
        {
#ifndef _DEBUG
			try
			{
#endif // _DEBUG
				// get the select socket list
				SOCKET maxSocket=makeSelector();
				ret = ::select((int)maxSocket+1, &m_readfds, NULL, NULL, NULL);
				// we check the exit flag here again, because we block before
				if(m_exitSignaled)
				{
					break;
				}
				if(ret > 0)
				{
					//check accept request
					if(m_socket.isSelected(&m_readfds))
					{
						AcceptedSocket client = m_socket.accept();
						if(client.isValid())
						{
							if(!insertClient(client))
							{
								m_logger.log(LogLevel::INFO, 0, "insert client failed\n");
								client.close();
							}
						}
						else
						{
							std::string errmsg;
							m_logger.log(LogLevel::INFO, 0, "accept client failed, %s\n", SysError::lastErrorMessage(errmsg));
						}
					}
					//check all clients event
					else
					{
						checkClientEvent();
					}
				}
				else
				{
					std::string errmsg;
					m_logger.log(LogLevel::INFO, 0, "select failed, error(%d)\n", SysError::lastErrorMessage(errmsg));
					//recover the server's error status
					recover();
					continue ;
				}
#ifndef _DEBUG
			}
			catch(...)
			{
				m_logger.log(LogLevel::INFO, 0, "Server exception\n");
			}
#endif // _DEBUG
		}
        m_logger.log(LogLevel::INFO, 0, "Server exit now\n");
    }
    
    void ActiveServer::checkClientEvent()
    {
        char *buffer = NULL;
        int  length	 = -1;
        int  ret;
        int  closed_count = 0;
        int  closed_client[MAX_CLIENT] = {0};
        for(int i=0; i< m_clients; ++i)
        {
            if(m_client[i].m_socketid.isSelected(&m_readfds))
            {
				//RECV_OK             // recv ok
				//RECV_CLOSE          // connection closed
				//RECV_SOCKET_ERROR   // socket error
				//RECV_USER_ERROR     // user error < 0
                ret = onReceive(m_client[i]);
                switch(ret)
                {
                case RECV_OK:
                    {
                        m_logger.log(LogLevel::INFO, m_client[i].getId(), "Received\n");
                    }
                    break;
                case RECV_CLOSE:
                    {
                        m_logger.log(LogLevel::INFO, m_client[i].getId(), "Closed by peer\n");
                        closed_client[closed_count++] = i;
                    }
                    break;
				case RECV_WANT_CLOSE:
					{
						m_logger.log(LogLevel::INFO, m_client[i].getId(), "Require close\n");
						closed_client[closed_count++] = i;
					}
					break;
                case RECV_SOCKET_ERROR:
                    {
						std::string errmsg;
                        m_logger.log(LogLevel::INFO, m_client[i].getId(), "WRN:%s\n", SysError::lastErrorMessage(errmsg));
                        closed_client[closed_count++] = i;
                    }
                    break;
                default:	//user failed
                    {
                        //check whether the error reparable
                        if(reparable(ret))
                        {
                            m_logger.log(LogLevel::INFO, m_client[i].getId(), "WRN:reparable error(0x%X)\n", ret);
                        }
                        else
                        {
                            //if unrepairable then close the peer connection
                            m_logger.log(LogLevel::INFO, m_client[i].getId(), "WRN:unrepairable error(0x%X)\n", ret);
                            closed_client[closed_count++] = i;
                        }
                    }
                    break;
                }
            }
        }
        //remove all closed client from the client array
        for(int j=0; j<closed_count; ++j)
        {
            eraseClient(closed_client[j]);
        }
    }
}
