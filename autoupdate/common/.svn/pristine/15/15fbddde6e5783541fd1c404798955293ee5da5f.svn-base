#ifndef INCLUDED_MTL_CLIENT_H
#define INCLUDED_MTL_CLIENT_H

#pragma once

namespace cacti
{
	//	@TODO.--------------------------------
	//	the client to retrieve MTL configuration information.
	//
	class MtlClient	: public ActiveClient
	{
	public:
		MtlClient() : ActiveClient(0){ m_cfginfo = ""; };
		virtual ~MtlClient(){};
		std::string		m_cfginfo;
	private:
		int	onReceive(){
			Stream	stream;
			int		ret;
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
				return RECV_OK;
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
				printf("[%04d] RCV: want(%d) but received(%d)\n",
					m_id, left, ret);
			}
			else{
				char*	b2	= new char[ length + 16 ];
				stream.getStream((u8*)b2, length );
				b2[length]	= '\0';
				m_cfginfo	= b2;
				delete	b2;
			};
			return RECV_OK;
		};
	};
}
//
#endif