#include "stdafx.h"

#include "cacti/mtl/TransferServer.h"
#include "cacti/message/TransferMessage.h"
#include "cacti/message/Stream.h"
#include "cacti/util/StringUtil.h"

#include "evtmsg.h"

namespace cacti
{
	TransferServer::TransferServer(DispatcherInterface* di, const char* loggername /*= LOGGER_SOCKET_SERVER*/)
		: m_dispatcherInterface(di)
		, ActiveServer(loggername)
	{
		assert(di);
	}

	void TransferServer::onBroken(ConnectionIdentifier client)
	{
		ServiceIdentifier sid(0, 0, client.getId());
		m_dispatcherInterface->postMessage(sid, UTM::create(sid, sid, _EvtClientClosed));
	}

	bool TransferServer::sendPackage(unsigned int clientid, NetworkPackage& np)
	{
		Stream stream;
		stream.reserve(4096);

		stream.put32(0);
		np.encode(stream);
		Stream::put32((u8*)stream.getWriteBuffer(0), (u32)stream.size()-4);

		return send(clientid, stream.getWriteBuffer(), (int)stream.size()) == stream.size();
	}

	int TransferServer::onReceive(ConnectionIdentifier client)
	{
		Stream stream;
		int  ret;

		//receive the package length at first!
		stream.resize(4);
		ret = client.m_socketid.recv(stream.getWriteBuffer(), (int)stream.size(), Timestamp(5,0));

		//check valid header
		if(ret != 4)
		{
			if(ret > 0)
			{
				m_logger.log(LogLevel::ERR, client.m_clientid, 
					"RCV: failed, just received(%d) bytes\n", ret);
			}
			return recvError(ret);
		}

		std::string binbuf;
		m_logger.log(LogLevel::INFO, client.m_clientid, 
			"RCV: header :%s\n", 
			cacti::StringUtil::formatBinary((unsigned char*)stream.getWriteBuffer(), (int)stream.size(), binbuf));

		//convert to host byte order
		u32 length;
		stream.get32(length);
		if(length == 0)
		{
			m_logger.log(LogLevel::INFO, client.m_clientid, "RCV: client want close!\n");
			return RECV_WANT_CLOSE;
		}
		stream.reset();			// reset the decode position

		u32 left = length;
		//package can't just have the length field.
		if(left == 0 || left >= MAX_NP_SIZE)
		{
			m_logger.log(LogLevel::ERR, client.m_clientid,
				"RCV: invalid length(%d)\n", length);
			return RECV_USER_ERROR;
		}
		// alloc the new buffer
		stream.resize(length);
		//receive the left package data
		ret = client.m_socketid.recv(stream.getWriteBuffer(), left, Timestamp(5,0));

		if(ret != left)
		{
			m_logger.log(LogLevel::ERR, client.m_clientid,
				"RCV: want(%d) but received(%d)\n",
				left, ret);

			return recvError(ret);
		}
		if(m_logger.isDebugEnabled())
		{
			binbuf = "";
			m_logger.log(LogLevel::DBG, client.m_clientid, "RCV: %s\n", 
				cacti::StringUtil::formatBinary((unsigned char*)stream.getWriteBuffer(), (int)stream.size(), binbuf));
		}
		else
		{
			m_logger.log(LogLevel::INFO, client.m_clientid, "RCV: %d bytes\n", 
				stream.size());
		}

		if(!enqueue(client, stream))
		{
			m_logger.log(LogLevel::INFO, client.m_clientid, "RCV: enqueue failed\n");
			return RECV_USER_ERROR;
		}
		return RECV_OK;
	}

	int TransferServer::recvError(int ret)
	{			
		if(ret == 0)
			return RECV_CLOSE;
		else if(ret > 0)
			return RECV_USER_ERROR;
		else
		{
			std::string errmsg;
			m_logger.log(LogLevel::ERR, "RCV: %s\n", SysError::lastErrorMessage(errmsg));

			return RECV_SOCKET_ERROR;
		}
	}

	bool TransferServer::enqueue(ConnectionIdentifier clt, Stream& stream)
	{
		NetworkPackagePtr npp(new NetworkPackage);
		if(!npp->decode(stream))
			return false;

		ServiceIdentifier& sid = npp->getSrcIdentifier();
		
		sid.m_appref = clt.m_clientid;
		m_dispatcherInterface->postMessage(sid, npp->getDstIdentifier(), npp->getUTM(), false);

		return true;
	}
}
