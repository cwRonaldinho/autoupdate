#include "stdafx.h"

#include "cacti/socket/SocketAddress.h"

#define new DEBUG_NEW

namespace cacti
{
    SocketAddress::SocketAddress()
        : m_host("")
    {
        memset(&m_addr, 0, sizeof(sockaddr_in));
    }
	SocketAddress::SocketAddress(const char *host, u_short port)
    {
        create(host, port);
    }

    SocketAddress::SocketAddress(const SocketAddress &rht)
        : m_host(rht.m_host)
    {
        memcpy(&m_addr, &rht.m_addr, sizeof(m_addr));
    }

    SocketAddress & SocketAddress::operator = (const SocketAddress &rht)
    {
        if(this != &rht)
        {
            m_host = rht.m_host;
            memcpy(&m_addr, &rht.m_addr, sizeof(m_addr));
        }
        return *this;
    }

    void SocketAddress::create(const char *host, u_short port)
    {
        if(host)
            m_host = host;
        else
            m_host = "";
        memset(&m_addr, 0, sizeof(sockaddr_in));
        m_addr.sin_port = htons(port);
        resolve();
    }

    void SocketAddress::directCreate(sockaddr_in &addr)
    {
		if(addr.sin_family == AF_INET)
		{
			//just copy the sockaddr_in structure
			memcpy(&m_addr, &addr, sizeof(sockaddr_in));
			//use the ip address as the host string
			m_host = inet_ntoa(addr.sin_addr);
		}
    }

    // try to resolve the m_host, if it's a hostname
    // and set the address, if m_host is an IP address then use direct!
    // if m_host is empty then address from INADDR_ANY
    // otherwise 
    bool SocketAddress::resolve()
    {
        m_addr.sin_family = AF_INET;
        if(!m_host.empty())
        {
            m_addr.sin_addr.s_addr = inet_addr(getHost());
            //not IP address but host name string
            //try to resolve the host name's ip address
            if(m_addr.sin_addr.s_addr == INADDR_NONE)
            {
                struct hostent *host = NULL;
                host = gethostbyname(getHost());
                if(host)
                {
                    memcpy(&m_addr.sin_addr, host->h_addr_list[0], host->h_length);
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        return true;
    }

    bool SocketAddress::bindWith(SOCKET socket)
    {
        return ::bind(socket, (sockaddr *)&m_addr, sizeof(m_addr)) != SOCKET_ERROR;
    }

    bool SocketAddress::connectWith(SOCKET socket)
    {
        return ::connect(socket, (sockaddr *)&m_addr, sizeof(m_addr)) != SOCKET_ERROR;
    }

	bool SocketAddress::connectWith(SOCKET socket, cacti::Logger& logger)
	{
		
		int ret = ::connect(socket, (sockaddr *)&m_addr, sizeof(m_addr));
		if (ret != 0)
		{
			logger.error("Socket connect failed, errno = %d\n", WSAGetLastError());
		}
		return ret != SOCKET_ERROR;
	}
}
