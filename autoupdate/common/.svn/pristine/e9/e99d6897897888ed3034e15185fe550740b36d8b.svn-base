#ifndef INCLUDED_CACTI_SOCKETMACRO_H
#define INCLUDED_CACTI_SOCKETMACRO_H

#pragma once

//for *NUX
#if defined( __linux__ ) || defined( __sun )
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <sys/ioctl.h>

	#define  SOCKET int
	#ifndef INVALID_SOCKET
		#define  INVALID_SOCKET SOCKET(~0)
	#endif
	#define SOCKET_ERROR -1
	inline   int closesocket(int fd) { return ::close(fd); }
	inline	bool	bootupSocket(){ return true; };
	inline	int	shutdownSocket(){ return 0; };
	
	#ifndef INADDR_NONE 
	#define INADDR_NONE ((unsigned long) -1) 
	#endif 

#else   //for windows
	#include <Windows.h>
	#pragma  comment(lib , "ws2_32.lib")

    #ifndef WINSOCK_VERSION
    #define WINSOCK_VERSION		MAKEWORD(2, 2)
    #endif

#define shutdownSocket()	WSACleanup()
	#define socklen_t int
    inline bool	bootupSocket()
    {
        WSADATA wsa;
        int ret;
        ret = WSAStartup(WINSOCK_VERSION, &wsa);
        return ret == 0;
    }

#endif  //__linux__ or __solaris__

enum SOCKET_TYPE
{ 
    IP_SOCKET, 
    UDP_SOCKET, 
    TCP_SOCKET,
};

enum RECV_RET
{
    RECV_OK             = 0,
    RECV_CLOSE          = 1,              
    RECV_SOCKET_ERROR   = 2,
	RECV_WANT_CLOSE		= 3,
    RECV_USER_ERROR     = -1,
};

#endif  //INCLUDED_CACTI_SOCKETMACRO_H
