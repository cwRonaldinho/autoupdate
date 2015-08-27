#ifndef INCLUDED_STREAM_STREAM_H
#define INCLUDED_STREAM_STREAM_H

#pragma once

#include <cacti/message/TypeDef.h>
#include <string>
#include <vector>

namespace cacti
{
	typedef std::vector<char > StreamContainer;
	class Stream
	{
	public:
		Stream();

		//integer encode routine
		void put64(u64 value);
		void put32(u32 value);
		void put16(u16 value);
		void put8 (u8  value);

		//string encode routine
		void putStream(const u8* stream, int length);
		void putString(const std::string &value);

		/// integer decode routine
		bool get64(u64& value);
		bool get32(u32& value);
		bool get16(u16& value);
		bool get8 (u8 & value);
		bool getStream(u8* stream, size_t length);
		bool getString(std::string& value);


		static inline void put32(u8* buf, u32 val)
		{
			buf[0] = (u8(val >> 24));
			buf[1] = (u8(val >> 16));
			buf[2] = (u8(val >> 8));
			buf[3] = (u8(val));
		}
		static inline u32 get32(u8* buf)
		{
			u32 accumulator = (u8)buf[0];					// 1 byte
			accumulator = (accumulator << 8) | (u8)buf[1];	// 2
			accumulator = (accumulator << 8) | (u8)buf[2];	// 3
			accumulator = (accumulator << 8) | (u8)buf[3];	// 4
			return accumulator;
		}

		/// how many buffer left when decoding
		size_t left() const		
		{ 
			return size() - m_offset; 
		};
        /// decoder offset position
		size_t offset() const	
		{ 
			return m_offset;
		};
		void   reset(int offset = 0);

		// clear the stream buffer, not free the memory!
        void   clear();
        // free the stream buffer's memory
        void   free();

		size_t size() const
		{
			return m_container.size();
		}
		void resize(StreamContainer::size_type newSize)
		{
			m_container.resize(newSize);
		}
		void reserve(StreamContainer::size_type count)
		{
			m_container.reserve(count);
		}

		/**
		 * return the buffer which can be write directly, 
		 * must call resize method to allocate enough buffer!
		 */
		char *getWriteBuffer(size_t offset = 0);

	protected:
        StreamContainer m_container;
        size_t          m_offset;		//indicate the decode position
	};
};

#endif // INCLUDED_STREAM_STREAM_H
