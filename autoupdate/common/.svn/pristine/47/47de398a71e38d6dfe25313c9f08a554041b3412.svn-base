#ifndef INCLUDED_MTL_TRANSFERCLIENT_H
#define INCLUDED_MTL_TRANSFERCLIENT_H

#pragma once

#include "cacti/socket/ActiveClient.h"
#include "cacti/mtl/DispatcherInterface.h"
#include "cacti/message/Stream.h"
#include "sxconst.h"

namespace cacti
{
	// use NetworkPackage as transfer package
	class TransferClient : public ActiveClient
	{
	public:
		TransferClient(u32 dstappid, DispatcherInterface* di, const char* loggername=LOGGER_SOCKET_CLIENT);

		void stop();
		bool sendPackage(NetworkPackage& np);
		u32  getDstAppid();
	private:
		virtual void	onConnected();
		virtual void	onBroken();
		virtual int		onReceive();
		int recvError(int ret);
		bool enqueue(Stream& stream);

	private:
		u32 m_dstappid;
		DispatcherInterface* m_dispatcherInterface;
	};
}
#endif
