#ifndef INCLUDE_MESSAGE_SERVICE_IDENTIFIER_H
#define INCLUDE_MESSAGE_SERVICE_IDENTIFIER_H
#pragma warning(disable:4996)

#pragma once

#include "cacti/message/Stream.h"
#include "cacti/config/Portability.h"

namespace cacti
{
	// for my lazy
#define FAIL_RETURN(exp) {if(!(exp)) return false;}

	struct ServiceIdentifier
	{
		u32 m_appid;
		u32 m_appport;
		u32 m_appref;


		ServiceIdentifier::ServiceIdentifier()
		{
			m_appid		= 0;
			m_appport	= 0;
			m_appref	= 0;
		}

		ServiceIdentifier(u32 appid,u32 appport, u32 appref)
		{	
			m_appid = appid;
			m_appport = appport;
			m_appref = appref;

		};
		std::string toString() const
		{
			char buf[64];
			snprintf(buf, sizeof(buf)-1, "%d.%d.%x", m_appid, m_appport, m_appref);
			return buf;
		};
		static ServiceIdentifier fromString(const char* str)
		{
			ServiceIdentifier sid;
			sscanf(str, "%d.%d.%x", &sid.m_appid, &sid.m_appport, &sid.m_appref);
			return sid;
		}
		std::string toString1()	const
		{
			char buf[64];
			snprintf(buf, sizeof(buf)-1, "/m/%d", m_appid);
			return buf;
		};
		std::string	toString2() const
		{
			char buf[64];
			snprintf(buf, sizeof(buf)-1, "/m/%d/%d", m_appid, m_appport );
			return buf;
		};
		std::string toString3() const
		{
			char buf[64];
			snprintf(buf, sizeof(buf)-1, "/m/%d/%d/%x", m_appid, m_appport, m_appref );
			return buf;
		};
		bool isValid() const{	return	m_appid != 0; };

		bool ServiceIdentifier::operator < (const ServiceIdentifier& sid) const
		{
			if (m_appref > sid.m_appref)
				return false;

			if (m_appref < sid.m_appref)
				return true;

			if (m_appid > sid.m_appid)
				return false;

			if (m_appid < sid.m_appid)
				return true;

			if (m_appport>= sid.m_appport)
				return false;
			return true;
		}

		bool ServiceIdentifier::operator ==(const ServiceIdentifier& sid) const
		{
			return 	((m_appref == sid.m_appref) && (m_appid == sid.m_appid) && (m_appport == sid.m_appport));

		}

		bool ServiceIdentifier::equal(const ServiceIdentifier& sid)  const
		{
			return 	((m_appref == sid.m_appref) && (m_appid == sid.m_appid) && (m_appport == sid.m_appport));		
		}



		void ServiceIdentifier::encode(Stream& stream)
		{
			stream.put32(m_appid);
			stream.put32(m_appport);
			stream.put32(m_appref);
		}

		bool ServiceIdentifier::decode(Stream& stream)
		{
			FAIL_RETURN(stream.get32(m_appid));
			FAIL_RETURN(stream.get32(m_appport));
			FAIL_RETURN(stream.get32(m_appref));
			return true;
		}

		void ServiceIdentifier::clear()
		{
			m_appid = 0;
			m_appport = 0;
			m_appref = 0;
		}
	};
	inline size_t hash_value(const ServiceIdentifier& si)
	{
		size_t val = 2166136261U;
		val = 16777619U * val ^ (size_t)si.m_appid;
		val = 16777619U * val ^ (size_t)si.m_appport;
		val = 16777619U * val ^ (size_t)si.m_appref;
		return val;
	}

	static const ServiceIdentifier NullSid;

}

#endif
