#include "stdafx.h"

#if	defined(WIN32)
#	include <winsock2.h>
#endif

#include "cacti/socket/SocketWrapper.h"

#define new DEBUG_NEW

namespace cacti
{
    AbstractSocket::AbstractSocket()
		: m_socket(INVALID_SOCKET)
    {
    }

    AbstractSocket::operator SOCKET() const
    {
        return m_socket;
    }

	bool AbstractSocket::create(int type)
	{
		if(type == TCP_SOCKET)
			m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		else if(type == UDP_SOCKET)
			m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		else
			m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

		int bufsize = 64*1024;
		setOption(SO_RCVBUF, &bufsize, sizeof(bufsize));
		setOption(SO_SNDBUF, &bufsize, sizeof(bufsize));

		return (m_socket != INVALID_SOCKET);
	}

    int AbstractSocket::close()
    {
        int ret(0);
		if(m_socket != INVALID_SOCKET)
        {
#ifdef WIN32
			shutdown(m_socket, SD_SEND);
#else
			linger lg;
			lg.l_onoff = 1;
			lg.l_linger = 0;
			//setsockopt(m_socket, SOL_SOCKET, SO_LINGER, &lg, sizeof(struct linger));
			::shutdown(m_socket, SHUT_RDWR);
#endif
			ret=::closesocket(m_socket);
			invalid();
        }
		return ret;
    }

    // Description         : check whether the socket in a fd_set
    // Input Parameters    : fds -- the fd_set
    bool AbstractSocket::isSelected(fd_set *fds) const
    {
        return FD_ISSET(m_socket, fds) == 1;
    }

    // Description         : set the socket into a fd_set
    // Input Parameters    : fds -- the fd_set
    void AbstractSocket::setSelect(fd_set *fds) const
    {
        FD_SET(m_socket, fds);
    }

    bool AbstractSocket::setOption(int optname, void *optval, int optlen, int level/* = SOL_SOCKET*/) const
    {
		return (::setsockopt(m_socket, level, optname, (char *)optval, optlen) != SOCKET_ERROR);
    }

    bool AbstractSocket::getOption(int optname, void *optval, int *optlen,int level /*= SOL_SOCKET*/) const
    {
		return (::getsockopt(m_socket, level, optname, (char *)optval, (socklen_t*)optlen) != SOCKET_ERROR);
    }

	void AbstractSocket::invalid()
	{
		m_socket = INVALID_SOCKET;
	}

    //////////////////////////////////////////////////////////////////////////////
    // Function Name       : isValid
    // Description         : check whether socket is valid
    // Return Value        : yes/no
    //////////////////////////////////////////////////////////////////////////////
    bool AbstractSocket::isValid() const
    {
        return m_socket != INVALID_SOCKET;
    }

    // Description         : select to see can we read now
    // Input Parameters    : tv -- the select timeval
    bool AbstractSocket::recvReady(const timeval *tv) const
    {
        fd_set fd_recv;
        FD_ZERO(&fd_recv);
        FD_SET(m_socket, &fd_recv);

        int ret = ::select((int)m_socket+1, &fd_recv, NULL, NULL, (timeval*)tv);
        if(ret == 1)
            return true;
        else
            return false;
    }

    // Description         : select to see can we send now
    // Input Parameters    : tv -- the select timeval
    bool AbstractSocket::sendReady(const timeval *tv) const
    {
        fd_set fd_send;
        FD_ZERO(&fd_send);
        FD_SET(m_socket, &fd_send);

        int ret = ::select((int)m_socket+1, NULL, &fd_send, NULL, (timeval*)tv);
        if(ret == 1)
            return true;
        else
            return false;
    }
    
	//////////////////////////////////////////////////////////////////////////////
	// Input Parameters    : outbuf -- buffer will be send
	//                       want   -- how much byte you want to be send
	//                       as you known, SOCKET API may not send all data you want!
	//                       if you want to make sure send all data, use sendAll method
	// Return Value        : If no error occurs, recv returns the number of bytes received. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	///                      Otherwise, a value of SOCKET_ERROR is returned
	int SocketChannel::send(const char *outbuf, int want) const
	{
		return ::send(m_socket, outbuf, want, 0);
	}

	// Input Parameters    : inbuf -- where data will be store
	//                     : size  -- size of inbuf
	// Return Value        : If no error occurs, recv returns the number of bytes received. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	///                      Otherwise, a value of SOCKET_ERROR is returned
	int SocketChannel::recv(char *inbuf, int size) const
	{
		return ::recv(m_socket, inbuf, size, 0);
	}

	// Input Parameters    : outbuf -- the data which want to be send
	//                       want   -- how many bytes you wan to send
	// Return Value        : If no error occurs, recv returns the number of bytes send. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	///                      Otherwise, a value of SOCKET_ERROR is returned
	int SocketChannel::sendn(const char *outbuf, int want) const
	{
		int left;
		int idx;
		int ret;
		left = want;
		idx  = 0;

		int nc = 0;
		while(left > 0)
		{
			nc++;
			ret = ::send(m_socket, &outbuf[idx], left, 0);
			// send error, or has been close by peer
			if(ret == 0 || ret == SOCKET_ERROR)
				break;
			left -= ret;
			idx  += ret;
		}
		if(nc != 1)
			printf("<DEBUG>sendn:%d\n", nc);
		// left == 0 means we do the job successfully
		return left == 0 ? idx : ret;
	}

	// Input Parameters    : inbuf -- where to store data
	//                       want  -- how much data we want
	// Return Value        : If no error occurs, recv returns the number of bytes received. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	///                      Otherwise, a value of SOCKET_ERROR is returned
	int SocketChannel::recvn(char *inbuf, int want) const
	{
		int ret;
		int read = 0;
		while(read < want)
		{
			ret = recv(&inbuf[read], want-read);
			// send error, or has been close by peer
			if(ret == 0 || ret == SOCKET_ERROR)
				break;
			read += ret;
		}
		// read == want means we do the job successfully
		return read == want ? read : ret;
	}

	// Input Parameters    : inbuf -- where to store data
	//                       want  -- how much data we want
	//                       timeout -- timeout(unit: s)
	// Return Value        : If no error occurs, recv returns the number of bytes received. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	//                       Otherwise, a value of SOCKET_ERROR is returned
	//                       Timeout will look as SOCKET_ERROR
	int SocketChannel::recv(char *inbuf, int want, const Timestamp& timeout) const
	{
		int ret;
		int read;

		timeval tv = {1, 0};	// default is check timeout per 1 second
		timeval *ptv = &tv;
		if(timeout == Timestamp::INFINITE_TIME)	
			ptv = NULL;         // NULL indicate select recvReady wait for ever

		Timestamp endtime = Timestamp::getNowTime();
		endtime += timeout;

		read = 0;
		while(read < want)
		{
			if(recvReady(ptv))
			{
				ret = recv(inbuf+read, want-read);
				if(ret == 0 || ret == SOCKET_ERROR)
					break;
				// if no error, ret is the real size we had recv
				read += ret;
			} 
			if(Timestamp::getNowTime() >= endtime)
			{
				ret = SOCKET_ERROR;
				break;
			}
		}
		// read == want means we do the job successfully
		return read == want ? read : ret;
	}

	// Input Parameters    : inbuf -- where to store data
	//                       maxSize -- the max size of received data
	//                       timeout -- timeout(unit: s)
	// Return Value        : If no error occurs, recv returns the number of bytes received. 
	//                       If the connection has been gracefully closed, the return value is zero. 
	//                       Otherwise, a value of SOCKET_ERROR is returned
	//                       Timeout will look as SOCKET_ERROR
	// 接收http数据
	int SocketChannel::recvHttp(char *inbuf, int maxSize, const Timestamp& timeout) const
	{
		int ret;
		int read;

		timeval tv = {1, 0};	// default is check timeout per 1 second
		timeval *ptv = &tv;
		if(timeout == Timestamp::INFINITE_TIME)	
			ptv = NULL;         // NULL indicate select recvReady wait for ever

		Timestamp endtime = Timestamp::getNowTime();
		endtime += timeout;

		read = 0;
		while(read < maxSize)
		{
			if(recvReady(ptv))
			{
				ret = recv(inbuf+read, maxSize-read);			// 每次最多接收指定字节数据
				if(ret == 0 || ret == SOCKET_ERROR)
					break;
				read += ret;

				// 解析Content-Length值
				int clLen = getContentLengthFromHttp(inbuf, read);
				if (clLen >= 0)		// 包体长度大于0
				{
					// 判断body长度是否与Content-Length值一致
					const char *posHeaderEnd = strstr(inbuf, "\r\n\r\n");
					if (posHeaderEnd)
					{
						int lenBeforeBody = posHeaderEnd - inbuf + strlen("\r\n\r\n");		// 包体前所有数据的总长度
						if ((read - lenBeforeBody) >= clLen)
						{
							break;
						}
					}
				}
				else if (-2 == clLen)	// 无"Content-Length"值，停止接收
				{
					break;
				}
			} 
			if(Timestamp::getNowTime() >= endtime)
			{
				ret = SOCKET_ERROR;
				break;
			}
		}

		return read;
	}

	// Input Parameters    : src -- source http raw data
	//                       len -- 裸数据长度
	// Return Value        : -1 http源数据中无content-length值
	//                       -2 消息头已经接收全，但header中无content-length
	//                       n  http消息中的content-length值
	// 解析http消息中的content-length值
	int SocketChannel::getContentLengthFromHttp(const char *src, int len) const
	{
		// 查找数据中是否有"\r\n\r\n"
		if (!strstr(src, "\r\n\r\n"))		// 未找到，表明http数据中的消息头还不全
		{
			return -1;
		}

		int ret = -1;

		// 查找content-length的值
		char *pLowerSrc = new char[len+1];
		memcpy_s(pLowerSrc, len, src, len);
		pLowerSrc[len] = '\0';
		_strlwr_s(pLowerSrc, len+1);		// 将源数据转换为小写

		const char *clKey = "content-length:";
		const char *pCLHeader = strstr(pLowerSrc, clKey);
		if (pCLHeader)
		{
			// 查找content-length的header结束位置
			const char* pCLEnd = strstr(pCLHeader, "\r\n");
			char sCLValue[100];
			if (pCLEnd)
			{
				strncpy_s(sCLValue, pCLHeader + strlen(clKey), pCLEnd - pCLHeader - strlen(clKey));
				ret = atoi(sCLValue);
			}
		}
		else								// 有些http消息中无"Content-Length"字段
		{
			// 无"Content-Length"时，返回-2，以防止recv处循环的时间过长
			ret = -2;
		}

		delete []pLowerSrc;
		pLowerSrc = NULL;
		return ret;
	}

    SocketConnector::SocketConnector()
		: m_connected(false)
    {
    }
	
	//////////////////////////////////////////////////////////////////////////////
    SocketConnector::SocketConnector(const SocketAddress &serverAddress)
		: m_severAddress(serverAddress)
		, m_connected(false)
    {
    }

	void SocketConnector::setServerAddress(const SocketAddress &serverAddress)
	{
		m_severAddress = serverAddress;
	}

	void SocketConnector::setLocalAddress(const SocketAddress& localAddress){
		m_localAddress=localAddress;
	}

    bool SocketConnector::connect()
    {
		if(isConnected())
		{
			disconnect();
		}
		else
		{
			AbstractSocket::close();
		}

		// if SOCKET not created yet! create it first!
        if(isValid() || create())
        {
			if(m_severAddress.connectWith(m_socket))
			{
				sockaddr_in	local_addr;
				memset((void*)&local_addr, 0, sizeof(sockaddr_in));
				socklen_t len(sizeof(sockaddr_in));
				int ret=::getsockname(m_socket,(sockaddr *)&local_addr, &len);
				if (ret==0)
				{
					m_localAddress.directCreate(local_addr);
				}
				m_connected = true;			
			}
        }
		return m_connected;
    }

	bool SocketConnector::connect(cacti::Logger& logger)
	{
		if(isConnected())
		{
			logger.info("In SocketConnector::connect(), socket is already connected\n");
			disconnect();
		}
		else
		{
			AbstractSocket::close();
		}

		// if SOCKET not created yet! create it first!
		if(isValid() || create())
		{
			if(m_severAddress.connectWith(m_socket, logger))
			{
				sockaddr_in	local_addr;
				memset((void*)&local_addr, 0, sizeof(sockaddr_in));
				socklen_t len(sizeof(sockaddr_in));
				int ret=::getsockname(m_socket,(sockaddr *)&local_addr, &len);
				if (ret==0)
				{
					m_localAddress.directCreate(local_addr);
				}
				m_connected = true;			
			}
		}
		return m_connected;
	}

    // Description         : connect to server, this method offer the server address
    //                       and it will cover the original server address(if exist)
    // Input Parameters    : serverAddress -- the server address
    bool SocketConnector::connect(const SocketAddress &serverAddress)
    {
        m_severAddress = serverAddress;
        return connect();
    }
    
	bool SocketConnector::connect(const SocketAddress &serverAddress, cacti::Logger& logger)
	{
		m_severAddress = serverAddress;
		return connect(logger);
	}

    void SocketConnector::disconnect()
	{
        m_connected = false;
        AbstractSocket::close();
    }

    ///////////////////////////////////////////////////////////////////////////

    AcceptedSocket::AcceptedSocket()
    {
    };

    AcceptedSocket::AcceptedSocket(SOCKET sock)
    {
        this->attach(sock);
    };

    //////////////////////////////////////////////////////////////////////////////
    // Function Name       : attach
    // Description         : attach a accepted socket
    // Input Parameters    : sock -- the socket
    //////////////////////////////////////////////////////////////////////////////
    void AcceptedSocket::attach(SOCKET sock)
    {
        //attach the socket to AbsSocket at first
        m_socket = sock;
        //and then get the remote address and port
        if(isValid())
        {
            //get the peer name from the connected socket
            sockaddr_in peer_addr;
            int      addr_len = sizeof(sockaddr);
			::getpeername(m_socket, (sockaddr *)&peer_addr, (socklen_t*)&addr_len);
            m_peerAddress.directCreate(peer_addr);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    SocketAcceptor::SocketAcceptor()
		: m_binded(false)
    {
    };

    SocketAcceptor::SocketAcceptor(const SocketAddress &addr)
		: m_serviceAddress(addr)
    {
    }

	void SocketAcceptor::setServiceAddress(const SocketAddress &serviceAddress)
	{
		m_serviceAddress = serviceAddress;
	}

    bool SocketAcceptor::bind()
    {
		if(isBinded())
			unbind();

		// if SOCKET not created yet! create it first!
        if(isValid() || create())
        {
			//the the server socket option
			int alive  = 1;
			setOption(SO_KEEPALIVE, (char *)&alive, sizeof(alive));
			//set the SO_REUSEADDR socket parameter

#ifndef _WIN32
			int reuseaddr = 1;
			setOption(SO_REUSEADDR, (char *)&reuseaddr, sizeof(reuseaddr));
#endif

			if( m_serviceAddress.bindWith(m_socket) &&
                ::listen(m_socket, LISTEN_SIZE) != SOCKET_ERROR)
            {
				m_binded = true;
                return true;
            }
        }
        return false;
    }

	bool SocketAcceptor::bind(const SocketAddress &addr)
	{
		m_serviceAddress = addr;
		return bind();
	}

	void SocketAcceptor::unbind()
	{
		m_binded = false;
		AbstractSocket::close();
	}

    AcceptedSocket SocketAcceptor::accept()
    {
        return AcceptedSocket(::accept(m_socket, NULL, NULL));
	}
}
