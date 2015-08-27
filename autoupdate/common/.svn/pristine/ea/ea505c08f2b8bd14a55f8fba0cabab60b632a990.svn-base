#include "stdafx.h"

#include <cacti/message/Stream.h>

#include <string>
#include <assert.h>

#define new DEBUG_NEW

namespace cacti
{
	Stream::Stream()
	{
		reset();
	}
	void Stream::put64(u64 value)
	{
		put8(u8(value >> 56));
		put8(u8(value >> 48));
		put8(u8(value >> 40));
		put8(u8(value >> 32));
		put8(u8(value >> 24));
		put8(u8(value >> 16));
		put8(u8(value >> 8));
		put8(u8(value));
	}
	//integer encode routine
	void Stream::put32(u32 value)
	{
		put8(u8(value >> 24));
		put8(u8(value >> 16));
		put8(u8(value >> 8));
		put8(u8(value));
	}
	void Stream::put16(u16 value)
	{
		put8(u8(value >> 8));
		put8(u8(value));
	}
	void Stream::put8 (u8  value)
	{
		m_container.push_back(value);
	}

	void Stream::putStream(const u8* stream, int length)
	{
		m_container.insert(m_container.end(), stream, stream+length);
	}

	void Stream::putString(const std::string &value)
	{
		m_container.insert(m_container.end(), value.c_str(),value.c_str()+value.length()+1);
	}

	//* the follow dec method must use u8 type conversion, 
	//* coz the base type of stream container is "char"
	//* if no type conversion, the shift action will not correct
	bool Stream::get64(u64& value)
	{
		if(left() < sizeof(u64))
			return false;

		u32 accumulator = (u8)m_container[m_offset++];					// 1 byte
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++];	// 2
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 3
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 4
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 5
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 6
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 7
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; //8
		value = accumulator;
		return true;
	}


	bool Stream::get32(u32& value)
	{
		if(left() < sizeof(u32))
			return false;

		u32 accumulator = (u8)m_container[m_offset++];					// 1 byte
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++];	// 2
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 3
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++]; // 4
		value = accumulator;
		return true;
	}

	bool Stream::get16(u16& value)
	{
		if(left() < sizeof(u16))
			return false;

		u16 accumulator = (u8)m_container[m_offset++];		//get the first byte;
		accumulator = (accumulator << 8) | (u8)m_container[m_offset++];	//second
		value = accumulator;
		return true;
	}

	bool Stream::get8 (u8 & value)
	{
		if(left() < sizeof(u8))
			return false;

		value = (u8)m_container[m_offset++];
		return true;
	}

	bool Stream::getStream(u8* stream, size_t length)
	{
		if( left() < length )
			return false;
		if ( 0 == length )
			return true;
		memcpy(stream, &m_container[m_offset], length);
		m_offset += length;
		return true;
	}
	bool Stream::getString(std::string& value)
	{
		for( u32 i = 0 ; i < left() ; i++)
		{
			if( 0 == m_container[m_offset+i] ) 
			{
				value = (char*)&m_container[m_offset];
				m_offset += i+1;
				return true;
			}
		}
		return false;
	}
	
	void   Stream::reset(int offset/* = 0*/)
	{
		if(offset == 0)
		{
			m_offset = 0;
		}
		else
		{
			m_offset += offset;
		}
		//check overflow or underflow
		if(m_offset<0)
		{
			m_offset = 0;
		}
		else if(m_offset > size())
		{
			m_offset = size();
		}
	}

	/// clear the stream buffer, not free the memory!
	void   Stream::clear()
	{
		m_container.clear();
	}

	/// free the stream buffer's memory
	void   Stream::free()
	{
		reset();
		StreamContainer().swap(m_container);
	}


	// return the buffer which can be write directly, 
	// must call resize method to alloc enough buffer!
	char *Stream::getWriteBuffer(size_t offset/* = 0*/)
	{
		assert(offset>=0 && offset <= m_container.size());
		return &m_container[offset];
	}
}
