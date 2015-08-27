#ifndef INCLUDED_SOCKETADDRESS_H
#define INCLUDED_SOCKETADDRESS_H

#pragma once

#include "cacti/socket/SocketMacro.h"
#include <string>
#include "cacti/logging/LogManager.h"

namespace cacti
{
    // encapsulation IP v4 address
    class SocketAddress
    {
    public:
        SocketAddress();
        SocketAddress(const char *host, u_short port);
        SocketAddress(const SocketAddress &rht);
        SocketAddress & operator = (const SocketAddress &rht);
        void create(const char *host, u_short port);
        void directCreate(sockaddr_in &addr);

        const char *	getHost()const		{return m_host.c_str();  }
        u_short			getPort()const		{return ntohs(m_addr.sin_port); }

        bool bindWith(SOCKET socket);
        bool connectWith(SOCKET socket);
		bool connectWith(SOCKET socket, cacti::Logger& logger);

    protected:
        //resolve the IP address of the host name. and full the m_addr structure
        bool resolve();
    private:        
        sockaddr_in m_addr;
        std::string m_host;
    };
};

#endif	//INCLUDED_SOCKETADDRESS_H
