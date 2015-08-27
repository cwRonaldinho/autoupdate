#ifndef INCLUDED_MTL_TRANSFER_CLIENT_H
#define INCLUDED_MTL_TRANSFER_CLIENT_H

#pragma once

#include "cacti/socket/ActiveServer.h"
#include "cacti/mtl/DispatcherInterface.h"
#include "sxconst.h"

namespace cacti
{
	class TransferServer : public ActiveServer
	{
	public:
		TransferServer(DispatcherInterface* di, const char* loggername = LOGGER_SOCKET_SERVER);
		
		bool sendPackage(unsigned int clientid, NetworkPackage& np);

	private:
		virtual bool onAccept(ConnectionIdentifier client) { return true; };
		virtual void onBroken(ConnectionIdentifier client);
		//onReceive must return the following result
		//RECV_OK             // recv ok
		//RECV_CLOSE          // connection closed
		//RECV_SOCKET_ERROR   // socket error
		//RECV_USER_ERROR     // user error < 0
		virtual int		onReceive(ConnectionIdentifier client);

	private:
		int recvError(int ret);
		bool enqueue(ConnectionIdentifier clt, Stream& stream);

	private:
		 DispatcherInterface* m_dispatcherInterface;
	};
}
#endif
