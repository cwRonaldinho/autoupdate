#include "stdafx.h"

#include "TransferMessage.h"

#define new DEBUG_NEW

namespace cacti
{
	// for my lazy
	#define FAIL_RETURN(exp) {if(!(exp)) return false;}
	
	static TLV s_default;

	KVNode::KVNode()
	{
		m_key = 0;
	}

	KVNode::KVNode(u32 key)
	{
		m_key = key;
		m_tlv.reset(new TLV);
	}

	KVNode::KVNode(const KVNode& other)
	{
		*this = other;	// using the operator=
	}
	void KVNode::encode(Stream& stream)
	{
		stream.put32(m_key);
		m_tlv->encode(stream);
	}
	bool KVNode::decode(Stream& stream)
	{
		FAIL_RETURN(stream.get32(m_key));
		m_tlv = TLVPtr(TLV::decode(stream));
		if(!m_tlv)
			return false;
		return true;
	}

	u32  KVNode::isValidTLV() const
	{
		return m_tlv->getType() != TLV::TYPE_INVALID;
	}

	KVNode& KVNode::operator =(const KVNode& other)
	{
		if(this != &other)
		{
			this->m_key = other.m_key;
			this->m_tlv= TLVPtr(new TLV(*other.m_tlv));
		}
		return *this;
	}

	UserTransferMessage::UserTransferMessage()
	{
		m_messageId = 0;
		m_ret = 0;
	}

	UserTransferMessage::UserTransferMessage(const UserTransferMessage& other)
	{
		*this = other;	// using the operator= 
	}

	UserTransferMessage::UserTransferMessage(const ServiceIdentifier& req, const ServiceIdentifier& res, u32 id, u32 ret/* = 0*/)
		: m_req(req)
		, m_res(res)
		, m_messageId(id)
		, m_ret(ret)
	{
	}

	void UserTransferMessage::setMessageId(u32 id)
	{
		m_messageId = id;
	}
	u32  UserTransferMessage::getMessageId() const
	{
		return m_messageId;
	}
	void UserTransferMessage::setReturn(u32 ret)
	{
		m_ret = ret;
	}
	u32  UserTransferMessage::getReturn() const 
	{
		return m_ret;
	}
	void UserTransferMessage::setReq(const ServiceIdentifier& req)
	{
		m_req = req;
	}
	const ServiceIdentifier& UserTransferMessage::getReq() const
	{
		return m_req;
	}
	void UserTransferMessage::setRes(const ServiceIdentifier& res)
	{
		m_res = res;
	}
	const ServiceIdentifier& UserTransferMessage::getRes() const
	{
		return m_res;
	}

	bool UserTransferMessage::hasKey(u32 key, u16 type) const
	{
		NodeList::const_iterator it = findKey(key);
		if(it != m_nodes.end())
		{
			assert((*it));

			// check TLV type
			if((*it)->m_tlv->getType() == type)
			{
				return true;
			}
		}
		return false;	
	}

	bool UserTransferMessage::removeKey(u32 key)
	{
		NodeList::iterator it = m_nodes.begin();
		for( ; it != m_nodes.end(); ++it)
		{
			if((*it)->m_key == key)
			{
				m_nodes.erase(it);
				return true;
			}
		}
		return false;
	}

	// for utm[key] = v1 and non-const access;
	TLV& UserTransferMessage::operator[](u32 key)
	{
		NodeList::const_iterator it = findKey(key);

		if(it != m_nodes.end())
		{
			return *((*it)->m_tlv);
		}
		else
		{
			// create a new node
            KVNodePtr node(new KVNode(key));
			m_nodes.push_back(node);
			return *(node->m_tlv);
		}
	}

	// for const utm; u8 v1 = utm[key] only;
	const TLV& UserTransferMessage::operator[](u32 key) const
	{
		NodeList::const_iterator it = findKey(key);

		if(it != m_nodes.end())
		{
			return *((*it)->m_tlv);
		}
		else
		{
			return s_default;
		}
	}

	UserTransferMessage& UserTransferMessage::operator=(const UserTransferMessage& other)
	{
		if(this != &other)
		{
			this->m_messageId = other.m_messageId;
			this->m_ret = other.m_ret;
			this->m_req = other.m_req;
			this->m_res = other.m_res;
			this->m_nodes.clear();
			NodeList::const_iterator it = other.m_nodes.begin();
			while(it != other.m_nodes.end())
			{
				this->m_nodes.push_back(KVNodePtr(new KVNode(**it)));
				++it;
			}
		}
		return *this;
	}

	NodeList::const_iterator UserTransferMessage::findKey(u32 key) const
	{
		NodeList::const_iterator it = m_nodes.begin();
		for( ; it != m_nodes.end(); ++it)
		{
			if((*it)->m_key == key)
				break;
		}
		return it;
	}

	void UserTransferMessage::encode(Stream& stream)
	{
		static const u32 utmOffset = (sizeof(m_messageId) + sizeof(m_ret) + sizeof(m_req) + sizeof(m_res));

		size_t prevOffset = stream.size();
		stream.put32(m_messageId);
		stream.put32(m_ret);
		m_req.encode(stream);
		m_res.encode(stream);
		
		stream.put32((u32)m_nodes.size());
		u32 realsize = 0;
		NodeList::iterator it = m_nodes.begin();
		while(it != m_nodes.end())
		{
			KVNodePtr& node= *it;
			if(node->isValidTLV())
			{
				node->encode(stream);
				++realsize;
			}
			++it;
		}
		// modify the real TLV node size
		Stream::put32((u8*)stream.getWriteBuffer(prevOffset+utmOffset), realsize);
	}

	bool UserTransferMessage::decode(Stream& stream)
	{
		FAIL_RETURN(stream.get32(m_messageId));
		FAIL_RETURN(stream.get32(m_ret));
		FAIL_RETURN(m_req.decode(stream));
		FAIL_RETURN(m_res.decode(stream));

		u32 size; 
		FAIL_RETURN(stream.get32(size));

		for(u32 i=0; i<size; ++i)
		{
			KVNodePtr nodeptr(new KVNode);
			if(!nodeptr->decode(stream))
			{
				return false;
			}
			else
			{
				m_nodes.push_back(nodeptr);
			}
		}
		return true;
	}

	void UserTransferMessage::swapSid()
	{
		/*
		ServiceIdentifier tmp = m_req;
		m_res = m_req;
		m_req = tmp;
		*/
		ServiceIdentifier tmp = m_req;
		m_req = m_res;
		m_res = tmp;
	}

	//////////////////////////////////////////////////////////////////////////
	void NetworkPackage::encode(Stream& stream)
	{
		stream.put16(m_version);
		stream.put16(m_type);
		m_srcId.encode(stream);
		m_dstId.encode(stream);
		
		m_utm->encode(stream);
	}
	bool NetworkPackage::decode(Stream& stream)
	{
		FAIL_RETURN(stream.get16(m_version));
		FAIL_RETURN(stream.get16(m_type));
		FAIL_RETURN(m_srcId.decode(stream));
		FAIL_RETURN(m_dstId.decode(stream));

		m_utm = UserTransferMessagePtr(new UserTransferMessage);
		FAIL_RETURN(m_utm->decode(stream));
		return true;
	}

}
