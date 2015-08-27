#include "stdafx.h"

#include "TransferClient.h"
#include "evtmsg.h"

namespace cacti
{
	TransferClient::TransferClient(u32 dstappid, DispatcherInterface* di, const char* loggername /*= LOGGER_SOCKET_CLIENT*/)
		: ActiveClient(dstappid, loggername)
		, m_dispatcherInterface(di)
		, m_dstappid(dstappid)
	{
	}

	void TransferClient::stop()
	{
		if(m_alive)
		{
			exit();
			char exit[4] = {'\0', '\0', '\0','\0'};
			if(m_socket.isConnected())
			{
				m_socket.send(exit, 4);
			}
			ActiveClient::stop();
		}
	}

	bool TransferClient::sendPackage(NetworkPackage& np)
	{
		Stream stream;
		stream.reserve(4096);

		stream.put32(0);
		np.encode(stream);
		Stream::put32((u8*)stream.getWriteBuffer(0), (u32)stream.size()-4);
		return send(stream.getWriteBuffer(), (int)stream.size()) == stream.size();
	}

	u32 TransferClient::getDstAppid()
	{
		return m_dstappid;
	}

	void TransferClient::onConnected()
	{
		ServiceIdentifier sid(0, 0, m_dstappid);
		m_dispatcherInterface->postMessage(sid, UTM::create(sid, sid, _EvtConnectedToPeer));
	}

	void TransferClient::onBroken()
	{
		ServiceIdentifier sid(0, 0, m_dstappid);
		m_dispatcherInterface->postMessage(sid, UTM::create(sid, sid, _EvtDisconnectedFromPeer));
	}

	int  TransferClient::onReceive()
	{
		Stream stream;
		int  ret;

		//receive the package length at first!
		stream.resize(4);
		ret = m_socket.recv(stream.getWriteBuffer(), (int)stream.size(), Timestamp(5,0));

		//check valid header
		if(ret != 4)
		{
			if(ret > 0)
			{
				m_logger.error("[%04d] RCV: failed, just received(%d) bytes\n", 
					m_id, ret);
			}
			return recvError(ret);
		}

		std::string binbuf;
		m_logger.info("[%04d] RCV: header :\n", 
			m_id,
			cacti::StringUtil::formatBinary((unsigned char*)stream.getWriteBuffer(), (int)stream.size(), binbuf));

		//convert to host byte order
		u32 length;
		stream.get32(length);
		stream.reset();			// reset the decode position

		u32 left = length;
		//package can't just have the length field.
		if(left == 0 || left >= MAX_NP_SIZE)
		{
			m_logger.error("[%04d] RCV: invalid length(%d)\n", m_id, length);
			return RECV_USER_ERROR;
		}
		// alloc the new buffer
		stream.resize(length);
		//receive the left package data
		ret = m_socket.recv(stream.getWriteBuffer(), left, Timestamp(5,0));

		if(ret != left)
		{
			m_logger.error("[%04d] RCV: want(%d) but received(%d)\n",
				m_id, left, ret);

			return recvError(ret);
		}
		if(m_logger.isDebugEnabled())
		{
			binbuf = "";
			m_logger.debug("[%04d] RCV: %s\n", 
				m_id,
				cacti::StringUtil::formatBinary((unsigned char*)stream.getWriteBuffer(), (int)stream.size(), binbuf));
		}
		else
		{
			m_logger.info("[%04d] RCV: %d bytes\n", 
				m_id, 
				stream.size());
		}

		if(!enqueue(stream))
		{
			m_logger.info("[%04d] RCV: enqueue failed\n", m_id);
			return RECV_USER_ERROR;
		}
		return RECV_OK;
	}

	int TransferClient::recvError(int ret)
	{			
		if(ret == 0)
			return RECV_CLOSE;
		else if(ret > 0)
			return RECV_USER_ERROR;
		else
		{
			std::string errmsg;
			m_logger.error("[%04d] RCV: %s\n", 
				m_id, 
				SysError::lastErrorMessage(errmsg));

			return RECV_SOCKET_ERROR;
		}
	}

	bool TransferClient::enqueue(Stream& stream)
	{
		NetworkPackagePtr npp(new NetworkPackage);
		if(!npp->decode(stream))
			return false;

		ServiceIdentifier& src = npp->getSrcIdentifier();
		src.m_appref = 0;			// 必须值这个值，否则和TCP的ConnectionID冲突
		ServiceIdentifier& dst = npp->getDstIdentifier();
		dst.m_appref = m_dstappid;
		m_dispatcherInterface->postMessage(src, dst, npp->getUTM(), false);
		return true;
	}
}
