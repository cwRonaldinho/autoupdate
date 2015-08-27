#ifndef INCLUDED_MESSAGE_TLV_H
#define INCLUDED_MESSAGE_TLV_H

#pragma once

#include <cacti/message/Stream.h>

#include <assert.h>

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include "cacti/message/ServiceIdentifier.h"

namespace cacti
{
	typedef std::vector<u8>  u8array;
	typedef std::vector<u32> u32array;
	typedef std::vector<char*> ptrarray;
	typedef std::vector<std::string> strarray;
	typedef std::vector<float> floatarray;
	typedef std::vector<ServiceIdentifier> idarray;

	class TLV
	{
	public:
		enum TLV_TYPES
		{
			TYPE_INVALID=0,
			// basic type
			TYPE_INT_8	= 1,	// 8bit int value
			TYPE_INT_32	= 3,	// 32bit int value
			TYPE_STRING = 4,	// null terminal string
			TYPE_FLOAT  = 5,
			TYPE_IDENTIFIER = 6,
			// array type
			TYPE_ARRAY_INT_8  = 11,
			TYPE_ARRAY_INT_32 = 13,
			TYPE_ARRAY_STRING = 14,
			TYPE_ARRAY_FLOAT  = 15,
			TYPE_ARRAY_IDENTIFIER = 16,
			TYPE_END,		// end flag, it isn't a valid type
		};
		TLV();
		TLV(u8 val);
		TLV(u32 val);
		TLV(float val);
		TLV(const char* val);
		TLV(const std::string& val);
		TLV(const ServiceIdentifier& sid);

		TLV(const u8array& val);
		TLV(const u32array& val);
		TLV(const ptrarray& val);
		TLV(const strarray& val);
		TLV(const idarray& val);

		TLV(const TLV& other);
		TLV& operator = (const TLV& other);

		virtual ~TLV();
		
		u32  getType() const;

		u32  getLength() const;
		u8*  getValue() const;
		// encode/decode method
		void encode(Stream& stream);
		static TLV* decode(Stream& stream);

	public:
		// 
		operator u8() const;
		operator u32() const;
		operator float() const;
		operator std::string() const;
		operator ServiceIdentifier() const;

		operator u8array() const;
		operator u32array() const;
		operator ptrarray() const;
		operator strarray() const;
		operator idarray() const;


		TLV& operator = (u8 val);
		TLV& operator = (u32 val);
		TLV& operator = (float val);
		TLV& operator = (const char* val);		// the same as string
		TLV& operator = (const std::string& val);
		TLV& operator = (const ServiceIdentifier& sid);

		TLV& operator = (const u8array& val);
		TLV& operator = (const u32array& val);
		TLV& operator = (const ptrarray& val);
		TLV& operator = (const strarray& val);
		TLV& operator = (const idarray& val);

	private:
		inline void realloc()
		{
			delete[] m_value;
			m_value = new u8[m_length];
		}

	private:
		u32 m_type;
		u32 m_length;			// the length of m_value

		u8* m_value;
	};



	typedef boost::shared_ptr<TLV> TLVPtr;
}
#endif	// INCLUDED_MESSAGE_TLV_H
