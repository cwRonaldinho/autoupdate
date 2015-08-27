#ifndef INCLUDED_SOCKETWRAPPER_H
#define INCLUDED_SOCKETWRAPPER_H

#pragma once

#include "cacti/socket/SocketAddress.h"
#include "cacti/util/Timestamp.h"
#include "cacti/logging/LogManager.h"

namespace cacti
{
    class AbstractSocket
    {
    public:
        AbstractSocket();

        operator SOCKET() const;
		// create socket
		bool create(int type=TCP_SOCKET);
        // close socket
        int close();

        //select action support
        bool isSelected(fd_set *fds) const;
        void setSelect(fd_set *fds) const;

        //config action support
        bool setOption(int optname, void *optval, int optlen, int level = SOL_SOCKET) const;
        bool getOption(int optname, void *optval, int *optlen,int level = SOL_SOCKET) const;

		void invalid();
        //check the socket is valid or not
        bool isValid() const;

        //check the socket event
        bool recvReady(const timeval *tv) const;
        bool sendReady(const timeval *tv) const;

		unsigned int getHandle()
		{
			return (unsigned int)m_socket;
		}
    protected:
        SOCKET		m_socket;
    };

	class SocketChannel : public AbstractSocket
	{
	public:
		//send/recv manager
		//synchronization way
		int send(const char *outbuf, int want) const;
		int recv(char *inbuf, int size) const;

		int sendn(const char *outbuf, int want) const;
		int recvn(char *buf, int want) const;

		//asynchronism way
		int recv(char *buf, int want, const Timestamp& timeout) const;
		int recvHttp(char *buf, int maxSize, const Timestamp& timeout) const;

	private:
		int getContentLengthFromHttp(const char *src, int len) const;		// 解析http消息中的content-length值 add by tw 20140322
	};

    class SocketConnector : public SocketChannel
    {
    public:
        SocketConnector();
        SocketConnector(const SocketAddress& serverAddress);
		void setServerAddress(const SocketAddress& serverAddress);
        //连接关联
        bool connect();
		bool connect(cacti::Logger& logger);
        bool connect(const SocketAddress& serverAddress);
		bool connect(const SocketAddress& serverAddress, cacti::Logger& logger);
        void disconnect();        
		void setLocalAddress(const  SocketAddress& localAddress);
        bool		isConnected()	const { return m_connected && isValid(); }
        const char* getServerName()	const { return m_severAddress.getHost(); }
        u_short		getServerPort()	const { return m_severAddress.getPort();};
		const char* getLocalName()  const { return m_localAddress.getHost();};
		u_short     getLocalPort()  const { return m_localAddress.getPort();};

    private:
        bool		    m_connected;
        SocketAddress	m_severAddress;
		SocketAddress   m_localAddress;
    };

    class AcceptedSocket : public SocketChannel
    {
    public:
        AcceptedSocket::AcceptedSocket();
        AcceptedSocket::AcceptedSocket(SOCKET sock);
        void attach(SOCKET sock);

        const char *getPeerName()const {return m_peerAddress.getHost();}
        u_short		getPeerPort()const {return m_peerAddress.getPort();}
    private:
        SocketAddress	m_peerAddress;
    };

	class SocketAcceptor : public AbstractSocket
    {
    public:
        SocketAcceptor();
        SocketAcceptor(const SocketAddress &addr);
		void setServiceAddress(const SocketAddress &serviceAddress);
        //bind and accept
        bool		bind();
        bool		bind(const SocketAddress &addr);
		void		unbind();
        AcceptedSocket accept();

        //check 
        bool		isBinded()const { return (m_binded && isValid());}
        const char *getHost()const  { return m_serviceAddress.getHost();}
        u_short		getPort()const  { return m_serviceAddress.getPort();}
    private:
        bool		  m_binded;
        SocketAddress m_serviceAddress;

        //
        enum 
        {
            LISTEN_SIZE = SOMAXCONN
        };        
    };
};
#endif	//INCLUDED_SOCKETWRAPPER_H
