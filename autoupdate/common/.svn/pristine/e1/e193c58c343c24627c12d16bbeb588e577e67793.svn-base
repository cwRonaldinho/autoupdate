#ifndef INCLUDED_ACTIVESERVER_H
#define INCLUDED_ACTIVESERVER_H

#pragma once

#include "cacti/socket/SocketWrapper.h"
#include "cacti/kernel/SysObj.h"
#include "cacti/util/SeqGen.h"
#include "cacti/util/NonCopyable.h"

#include "cacti/logging/Logger.h"

namespace cacti
{
    //if somebody want to support more client, redefine FD_SETSIZE please,
    //but make sure redefine before include winsock2.h and this file
    static const int MAX_CLIENT	= (FD_SETSIZE-2);
        
	class ConnectionIdentifier
    {
    public:
        unsigned int getId()	const { return m_clientid;	}
        SOCKET		 getSocket()const { return m_socketid;	}
                
        unsigned int		m_clientid;	// the identify of this client
        AcceptedSocket		m_socketid;
    };
    
    // TCP server with select I/O model
	class ActiveServer
    {
    public:
		enum 
		{
			INVALID_CONNECTION_ID = SequenceGenerator::INVALID_ID,
		};

		ActiveServer(const char* loggername = "socket.asvr");
        virtual ~ActiveServer();
        
        //start the service
        bool			start(u_short port, const char *addr = NULL);
        //stop the service
        void			stop(int timeout = ~0);
        
        //main interface for user call :
        int				send(unsigned int clientid, const char *buf, int buf_size);
        //support for owner check the server status
        bool			binded()			{ return m_socket.isBinded(); }

		void stopClient(unsigned int clientId);
    protected:
        
    private:
        // initialize object state
        void            reset();

        virtual bool onAccept(ConnectionIdentifier client)  { return true; }
        virtual void onBroken(ConnectionIdentifier client)  { }
		//onReceive must return the following result
		//RECV_OK             // recv ok
		//RECV_CLOSE          // connection closed
		//RECV_SOCKET_ERROR   // socket error
		//RECV_USER_ERROR     // user error < 0
        virtual int		onReceive(ConnectionIdentifier client) = 0;
        
        virtual bool	reparable(int errcode);
               
        //start the listen thread
        void			startListener();
        
        SOCKET          makeSelector();
        void			recover();
        
        //client manager functions
        bool			insertClient(const AcceptedSocket &client);
        void			eraseClient(int index);
        void			cleanClients();
        
        AcceptedSocket  getClientSocket(unsigned int clientid);
        
        void			checkClientEvent();

        virtual void    threadProcess();

    protected:
		Logger		   &m_logger;
        SocketAcceptor	m_socket;

	private:
		//connected client count
		int				m_clients;
        ConnectionIdentifier m_client[MAX_CLIENT];
        SequenceGenerator m_seqGenerator;
        
        bool			m_running;
        volatile bool	m_exitSignaled;
		cacti::Thread   m_thread;
        fd_set			m_readfds;

		RecursiveMutex  m_sendnMutex;
    };
}

#endif
