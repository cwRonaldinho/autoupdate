#include "stdafx.h"

#include "AlarmOperator.h"
#include "cacti/message/Stream.h"

using namespace cacti;

AlarmOperator::AlarmOperator(const char* addr, u_short port)
	: m_address(addr)
	, m_port(port)
	, m_logger(Logger::getInstance(LOGGERALARMOPR))
{     
}

AlarmOperator::~AlarmOperator()
{
	if(m_connector.isConnected())
		m_connector.disconnect();
}

bool AlarmOperator::connect()
{
	if(!m_connector.isConnected())
		return m_connector.connect(SocketAddress(m_address.c_str(), m_port));

	return true;
}

UserTransferMessagePtr AlarmOperator::receive()
{
	Stream stream;
	int  ret;

	//receive the package length at first!
	stream.resize(4);
	ret = m_connector.recv(stream.getWriteBuffer(), (int)stream.size(), Timestamp(5,0));

	//check valid header
	if(ret != 4)
	{
		if(ret > 0)
		{
			m_logger.error("RCV: failed, just received(%d) bytes\n", 
				ret);
		}
		return UserTransferMessagePtr();
	}

	std::string binbuf;

	//convert to host byte order
	u32 length;
	stream.get32(length);
	stream.reset();			// reset the decode position

	u32 left = length;
	//package can't just have the length field.
	if(left == 0 || left >= MAX_NP_SIZE)
	{
		m_logger.error("RCV: invalid length(%d)\n", length);
		return UserTransferMessagePtr();
	}
	// alloc the new buffer
	stream.resize(length);
	//receive the left package data
	ret = m_connector.recv(stream.getWriteBuffer(), left, Timestamp(5,0));

	if(ret != left)
	{
		m_logger.error("RCV: want(%d) but received(%d)\n",
			left, ret);

		return UserTransferMessagePtr();
	}

	NetworkPackagePtr npp(new NetworkPackage);
	if(!npp->decode(stream))
		return UserTransferMessagePtr();

	return npp->getUTM();
}

UserTransferMessagePtr AlarmOperator::doCommand(UserTransferMessagePtr utm)
{
	if(connect())
	{
		NetworkPackage np;
		np.setUTM(utm);

		Stream stream;
		stream.reserve(4096);

		stream.put32(0);
		np.encode(stream);
		Stream::put32((u8*)stream.getWriteBuffer(0), (u32)stream.size()-4);
		if(m_connector.send(stream.getWriteBuffer(), (int)stream.size()) == stream.size())
		{
			return receive();
		}
	}
	return UserTransferMessagePtr();
}

bool AlarmOperator::doEvent(UserTransferMessagePtr utm)
{
	if(connect())
	{
		NetworkPackage np;
		np.setUTM(utm);

		Stream stream;
		stream.reserve(4096);

		stream.put32(0);
		np.encode(stream);
		Stream::put32((u8*)stream.getWriteBuffer(0), (u32)stream.size()-4);
		if(m_connector.send(stream.getWriteBuffer(), (int)stream.size()) == stream.size())
		{
			return true;
		}
	}
	return false;
}
