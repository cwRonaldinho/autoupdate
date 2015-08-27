#include "stdafx.h"

#include <cacti/message/TLV.h>
#include <memory>
#include <climits>

namespace cacti
{
	static const u32 TLV_MAX_LENGTH = INT_MAX/2;
	static const u32 TLV_MAX_ARRAY_COUNT = 1024*1024;
	static const u32 TLV_MAX_STRING_LENGTH = 100*1024*1024;

	TLV::TLV()
	{
		m_type = TYPE_INVALID;
		m_length = 0;
		m_value = NULL;
	}
	
	TLV::TLV(u8 val)
	{
		m_value = NULL;
		operator=(val);
	}
	TLV::TLV(u32 val)
	{
		m_value = NULL;
		operator=(val);
	}
	
	TLV::TLV(float val)
	{
		m_value = NULL;
		operator = (val);

	}

	TLV::TLV(const char* val)
	{
		m_value = NULL;
		operator=(val);
	}
	
	TLV::TLV(const std::string& val)
	{
		m_value = NULL;
		operator=(val);
	}

	TLV::TLV(const ServiceIdentifier& sid)
	{
		m_value = NULL;
		operator=(sid);
	}

	TLV::TLV(const u8array& val)
	{
		m_value = NULL;
		operator=(val);
	}
	TLV::TLV(const u32array& val)
	{
		m_value = NULL;
		operator=(val);
	}
	TLV::TLV(const ptrarray& val)
	{
		m_value = NULL;
		operator=(val);
	}
	TLV::TLV(const strarray& val)
	{
		m_value = NULL;
		operator=(val);
	}

	TLV::TLV(const idarray& val)
	{
		m_value = NULL;
		operator=(val);
	}
	// copy construct
	TLV::TLV(const TLV& other)
	{
		m_value = NULL;
		*this = other;
	}

	// assignment
	TLV& TLV::operator = (const TLV& other)
	{
		if(this != &other)
		{
			m_type = other.m_type;
			m_length = other.m_length;
			delete[] m_value;
			m_value = new u8[other.getLength()];
			for(size_t i=0; i<m_length; i++)
				m_value[i] = other.m_value[i];
		}
		return *this;
	}

	TLV::~TLV()
	{
		delete[] m_value;
	}

	u32 TLV::getType() const
	{
		return m_type;
	}

	u32 TLV::getLength() const
	{
		return m_length;
	}

	u8* TLV::getValue() const
	{
		return m_value;
	}

	void TLV::encode(Stream& stream)
	{
		if(m_value != NULL && m_type != TYPE_INVALID)
		{
			stream.Stream::put32(m_type);
			stream.Stream::put32(m_length);
			stream.putStream(m_value, m_length);
		}
	}

	TLV* TLV::decode(Stream& stream)
	{
		std::auto_ptr<TLV> tlv(new TLV);
		if(!stream.get32(tlv->m_type))
			return 0;
		
		if(!stream.get32(tlv->m_length))
		{
			return 0;
		}
		tlv->m_value = new u8[tlv->m_length];
		// we must allocate the m_value, but if it's zero length,
		// we don't need to get the stream
		if(tlv->m_length > 0)
		{
			if(!stream.getStream(tlv->m_value, tlv->m_length))
			{
				return 0;
			}
		}
		return tlv.release();
	}	

	TLV::operator u8() const
	{
		u8 ret = 0;
		if(m_type == TYPE_INT_8 && m_value && m_length >= 1)
		{
			ret = m_value[0];
		}
		return ret;
	}

	TLV::operator u32()  const
	{
		u32 ret = 0;
		if(m_type == TYPE_INT_32 && m_value && m_length >= 4)
		{
			ret = Stream::get32(m_value);
		}
		return ret;
	}
	TLV::operator float() const
	{
		float ret = 0.0;
		if(m_type == TYPE_FLOAT && m_value && m_length >= 4 )
		{
			u32 ival = Stream::get32(m_value);
			ret =  *((float*) &ival);

		}
		return ret;
	}

	TLV::operator std::string() const
	{
		std::string str;
		if(m_type == TYPE_STRING && m_value)
		{
			str.assign(m_value, m_value+m_length) ;
		}
		return str;
	}

	TLV::operator ServiceIdentifier() const
	{
		ServiceIdentifier sid;
		if(m_type == TYPE_IDENTIFIER && m_value && m_length >= 3*4)
		{
			sid.m_appid = Stream::get32(m_value);
			sid.m_appport = Stream::get32(m_value + 4);
			sid.m_appref = Stream::get32(m_value + 8);
		}
		return sid;
	}

	TLV::operator u8array() const
	{
		u8array ret;
		if(m_type == TYPE_ARRAY_INT_8 && m_value)
		{
			ret.resize(m_length);
			std::copy(m_value, m_value+m_length, ret.begin());
		}
		return ret;
	}
	TLV::operator u32array() const
	{
		u32array ret;
		if(m_type == TYPE_ARRAY_INT_32 && m_value)
		{
			size_t count = m_length/4;
			ret.resize(count);
			for(size_t i=0, j=0; i<count; ++i, j+=4)
			{
				ret[i] = Stream::get32(m_value+j);
			}
		}
		return ret;
	}
	TLV::operator ptrarray() const
	{
		ptrarray ret;
		if(m_type == TYPE_ARRAY_STRING && m_value && m_length >= 2)
		{
			u32 count = Stream::get32(m_value);
			ret.resize(count);
			u32 len;
			for(size_t i=0, j=4; i<count && j+4<=m_length; ++i)		// j will be change in the loop body
			{
				len = Stream::get32(m_value+j);								// the string length
				
				j += 4;
				ret[i] = (char*)(m_value+j);
				j += len+1;											// plus 1 for the '\0' length;
			}
		}
		return ret;
	}
	TLV::operator strarray() const
	{
		strarray ret;
		if(m_type == TYPE_ARRAY_STRING && m_value && m_length >= 2)
		{
			u32 count = Stream::get32(m_value);
			ret.resize(count);
			u32 len;
			for(size_t i=0, j=4; i<count && j+4<=m_length; ++i)		//j will be change in the loop body
			{
				len = Stream::get32(m_value+j);
				// verify the len isn't out of range
				if(j+len > m_length)
					break;

				j += 4;
				ret[i].assign(m_value+j, m_value+j+len);
				j += len+1;											// plus 1 for the '\0' length;
			}
		}
		return ret;
	}
	TLV::operator idarray() const
	{
		idarray ret;
		if(m_type == TYPE_ARRAY_IDENTIFIER && m_value)
		{
			size_t count = m_length/(3*4);
			ret.resize(count);
			for(size_t i=0, j=0; i<count; ++i, j+=3*4)
			{
				ret[i].m_appid = Stream::get32(m_value+j);
				ret[i].m_appport = Stream::get32(m_value+j+4);
				ret[i].m_appref = Stream::get32(m_value+j+8);
			}
		}
		return ret;
	}
	TLV& TLV::operator = (u8 val)
	{
		m_type = TYPE_INT_8;
		m_length = 1;
		realloc();
		m_value[0] = val;

		return *this;
	}

	TLV& TLV::operator = (u32 val)
	{
		m_type = TYPE_INT_32;
		m_length = 4;
		realloc();
		Stream::put32(m_value, val);

		return *this;
	}

	TLV& TLV::operator = (float val)
	{
		m_type = TYPE_FLOAT;
		m_length = 4;
		realloc();
		u32 *ival = (u32*) &val;
		Stream::put32(m_value,*ival);

		return  *this;

	}

	TLV& TLV::operator = (const char* val)
	{
		assert(val);
		m_type = TYPE_STRING;
		m_length = (u32)strlen(val);
		if(m_length < TLV_MAX_STRING_LENGTH)
		{
			realloc();
			memcpy(m_value, val, m_length);
		}
		return *this;
	}
	TLV& TLV::operator = (const std::string& val)
	{
		m_type = TYPE_STRING;
		m_length = (u32)val.size();
		if(m_length < TLV_MAX_STRING_LENGTH)
		{
			realloc();
			memcpy(m_value, val.c_str(), m_length);
		}
		return *this;
	}

	TLV& TLV::operator = (const ServiceIdentifier& sid)
	{
		m_type = TYPE_IDENTIFIER;
		m_length = 3*4;
		realloc();
		Stream::put32(m_value, sid.m_appid);
		Stream::put32(m_value+4, sid.m_appport);
		Stream::put32(m_value+8, sid.m_appref);
		return *this;
	}

	TLV& TLV::operator = (const u8array& val)
	{
		if(val.size() > TLV_MAX_ARRAY_COUNT)
			return *this;

		m_type = TYPE_ARRAY_INT_8;
		m_length = 1*(u32)val.size();
		realloc();
		for(size_t i=0; i<m_length; i++)
			m_value[i] = val[i];
		return *this;
	}
	TLV& TLV::operator = (const u32array& val)
	{
		if(val.size() > TLV_MAX_ARRAY_COUNT)
			return *this;

		m_type = TYPE_ARRAY_INT_32;
		m_length = 4*(u32)val.size();
		realloc();		

		for(size_t i=0, j=0; j<val.size(); ++j)	// i will be change in the loop body
		{
			Stream::put32(m_value+i, val[j]);
			i += 4;
		}
		return *this;
	}
	// [count] [length][string][\0] ... [length][string][\0]
	// "length" not include the '\0' size
	TLV& TLV::operator = (const ptrarray& val)
	{
		if(val.size() > TLV_MAX_ARRAY_COUNT)
			return *this;

		m_type = TYPE_ARRAY_STRING;
		std::vector<size_t> strlength(val.size());	// remember every string's length

		int checksize = 0;							// in case the overflow
		for(size_t i=0; i<val.size(); ++i)
		{
			if(val[i] == NULL)
				strlength[i] = 0;
			else
				strlength[i] = strlen(val[i]);
			
			if(strlength[i] > TLV_MAX_STRING_LENGTH)
				return *this;

			checksize += (int)strlength[i];
		}

		checksize += 4+(4+1)*(u32)val.size();	// "total count" + 
												// the "length" and '\0' field(every string) size
		if(checksize >= 0 && checksize < TLV_MAX_LENGTH)	// check for overflow
		{
			m_length = checksize;
			realloc();
			size_t len;

			Stream::put32(m_value, (u32)val.size());					// put the count
			for(size_t i=4, j=0; j<val.size(); ++j)		// i will be change in the loop body
			{
				len = strlength[j];
				Stream::put32(m_value+i, (u32)len);
				i += 4;
				if(len != 0)
					memcpy(m_value+i, val[j], len);

				i+= len;
				m_value[i++] = '\0';
			}
		}
		return *this;
	}
	TLV& TLV::operator = (const strarray& val)
	{
		if(val.size() > TLV_MAX_LENGTH)
			return *this;

		m_type = TYPE_ARRAY_STRING;
		int checksize = 0;								// in case the overflow
		for(size_t i=0; i<val.size(); ++i)
		{
			if(val[i].length() > TLV_MAX_STRING_LENGTH)
				return *this;

			checksize += (int)val[i].length();
		}
		checksize += 4+(4+1)*(int)val.size();	// "total count" + 
												// the "length" and '\0' field(every string) size
		if(checksize >= 0 && checksize < TLV_MAX_LENGTH)	// check overflow
		{
			m_length = checksize;
			realloc();
			size_t len;

			Stream::put32(m_value, (u16)val.size());			// put the count
			for(size_t i=4, j=0; j<val.size(); ++j)		// i will be change in the loop body
			{
				len = val[j].size();
				Stream::put32(m_value+i, (u32)len);
				i += 4;
				memcpy(m_value+i, val[j].c_str(), len);
				i += len;
				m_value[i++] = '\0';
			}
		}	
		return *this;
	}

	TLV& TLV::operator = (const idarray& val)
	{
		if(val.size() > TLV_MAX_ARRAY_COUNT)
			return *this;

		m_type = TYPE_ARRAY_IDENTIFIER;
		m_length = 3*4*(u32)val.size();
		realloc();

		for(size_t i=0, j=0; j<val.size(); ++j)	// i will be change in the loop body
		{
			Stream::put32(m_value+i, val[j].m_appid);
			i += 4;
			Stream::put32(m_value+i, val[j].m_appport);
			i += 4;
			Stream::put32(m_value+i, val[j].m_appref);
			i += 4;
		}
		return *this;
	}
}
