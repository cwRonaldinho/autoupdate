#ifndef INCLUDED_USER_TRANSFER_MESSAGE
#define INCLUDED_USER_TRANSFER_MESSAGE

#pragma once

#include <cacti/message/TLV.h>
#include "cacti/util/StringUtil.h"
#include "cacti/message/ServiceIdentifier.h"

#include <list>
#include <boost/shared_ptr.hpp>

namespace cacti
{
	static const int NP_VERSION = 0x10;
	static const int MAX_NP_SIZE = 100*1024*1024;

	static const u16 NPT_CONTROL = 1;
	static const u16 NPT_USER    = 2;
	static const u16 NPT_ROUTE   = 3;

	// one node is (key, value), and value is TLV(type, size, data)
	class KVNode
	{
	public:
		KVNode();
		KVNode(u32 key);
		KVNode(const KVNode& other);
		void encode(Stream& stream);
		bool decode(Stream& stream);
		u32  isValidTLV() const;
		KVNode& operator=(const KVNode& other);

	protected:
		friend class UserTransferMessage;
		u32	   m_key;
		TLVPtr m_tlv;
	};

	typedef boost::shared_ptr<KVNode> KVNodePtr;

	typedef std::list<KVNodePtr> NodeList;

	class UserTransferMessage
	{
	public:
		UserTransferMessage();
		UserTransferMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, u32 id, u32 ret = 0);
		UserTransferMessage(const UserTransferMessage& other);
		bool hasKey(u32 key, u16 type) const;
		bool removeKey(u32 key);
		
		TLV& operator[](u32 key);
		const TLV& operator[](u32 key) const;

		UserTransferMessage& operator=(const UserTransferMessage& other);
		
		void setMessageId(u32 id);
		u32  getMessageId() const;

		void setReturn(u32 ret);
		u32  getReturn() const;
		
		void setReq(const ServiceIdentifier& req);
		const ServiceIdentifier& getReq() const;

		void setRes(const ServiceIdentifier& res);
		const ServiceIdentifier& getRes() const;

		void encode(Stream& stream);
		bool decode(Stream& stream);

		void swapSid();

	private:
		NodeList::const_iterator findKey(u32 key) const;

	protected:		
		u32 m_messageId;
		u32 m_ret;					// the message status				
		ServiceIdentifier m_req;	// the requester identifier
		ServiceIdentifier m_res;	// message's responser identifier 
		NodeList m_nodes;			// TLV list
	};
	
	typedef boost::shared_ptr<UserTransferMessage> UserTransferMessagePtr;
	class UTM
	{
	public:
		static UserTransferMessagePtr create(const ServiceIdentifier& req, const ServiceIdentifier& res, u32 id, u32 ret = 0)
		{
			return UserTransferMessagePtr(new UserTransferMessage(req, res, id, ret));
		}
		static UserTransferMessagePtr create(const UserTransferMessage& utm)
		{
			return UserTransferMessagePtr(new UserTransferMessage(utm));
		}
		static UserTransferMessagePtr create(u32 id)
		{
			return UserTransferMessagePtr(new UserTransferMessage(ServiceIdentifier(), ServiceIdentifier(), id));
		}
	};
	class NetworkPackage
	{
	public:
		NetworkPackage(u16 t = NPT_USER, u16 ver = NP_VERSION)
			: m_version(ver)
			, m_type(t)
		{
		}

		void encode(Stream& stream);
		bool decode(Stream& stream);
		void setSrcIdentifier(const ServiceIdentifier& sid)
		{
			m_srcId = sid;
		}
		ServiceIdentifier& getSrcIdentifier()
		{
			return m_srcId;
		}
		void setDstIdentifier(const ServiceIdentifier& sid)
		{
			m_dstId = sid;
		}
		ServiceIdentifier&  getDstIdentifier()
		{
			return m_dstId;
		}
		void setUTM(UserTransferMessagePtr utm)
		{
			m_utm = utm;
		}
		UserTransferMessagePtr getUTM()
		{
			return m_utm;
		}

	private:

	private:
		u16 m_version;
		u16 m_type;
		ServiceIdentifier m_srcId;
		ServiceIdentifier m_dstId;
		UserTransferMessagePtr m_utm;
	};

	typedef boost::shared_ptr<NetworkPackage> NetworkPackagePtr;
}

#endif
