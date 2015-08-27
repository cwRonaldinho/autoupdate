#include "DataSource.h"

TDataSource::TDataSource()
{
}

TDataSource::~TDataSource()
{
	list<ServiceIdentifier> *sidList = NULL;
	for (map<u32, list<ServiceIdentifier>*>::iterator it = m_serviceIDMap.begin(); it != m_serviceIDMap.end(); ++it)
	{
		sidList = it->second;

		if (sidList)
		{
			sidList->clear();
			delete sidList;
			sidList = NULL;
		}
 	}
}

// 添加SID
// 返回: 0成功  其它失败
u32 TDataSource::addServiceIDByApport(u32 apport, const ServiceIdentifier& sid)
{
	list<ServiceIdentifier> *sidList = NULL;
	map<u32, list<ServiceIdentifier>*>::iterator it = m_serviceIDMap.find(apport);
	if (it == m_serviceIDMap.end())			// key不存在，新建
	{
		sidList = new list<ServiceIdentifier>;
		sidList->push_back(sid);
		m_serviceIDMap.insert(pair<u32, list<ServiceIdentifier>*>(apport, sidList));
	}
	else									// key存在，追加
	{
		sidList = it->second;
		sidList->push_back(sid);
	}

	return 0;
}

// 删除SID
// 返回: 0成功  其它失败
u32 TDataSource::removeServiceIDByApport(u32 apport, const ServiceIdentifier& sid)
{
	list<ServiceIdentifier> *sidList = NULL;
	map<u32, list<ServiceIdentifier>*>::iterator it = m_serviceIDMap.find(apport);
	if (it == m_serviceIDMap.end())			// key不存在
	{
		return 1;
	}
	
	sidList = it->second;
	
	// 遍历链表，删除对应节点
	for (list<ServiceIdentifier>::iterator sidIt = sidList->begin(); sidIt != sidList->end(); ++sidIt)
	{
		if ((*sidIt) == sid)				// 找到对应节点
		{
			sidList->erase(sidIt);
			return 0;
		}
	}

	return 1;
}

// 获取SID
// 返回: 0成功  其它失败
u32	TDataSource::getServiceIDByApport(u32 apport, ServiceIdentifier& sid)
{
	list<ServiceIdentifier> *sidList = NULL;
	map<u32, list<ServiceIdentifier>*>::iterator it = m_serviceIDMap.find(apport);
	if (it == m_serviceIDMap.end())			// key不存在
	{
		return 1;
	}

	sidList = it->second;

	// 轮询链表
	if (sidList->size() > 0)
	{
		sid = sidList->front();
		sidList->pop_front();
		sidList->push_back(sid);
		return 0;
	}

	return 1;
}

// 快照
void TDataSource::snapShot()
{
	list<ServiceIdentifier> *sidList = NULL;
	for (map<u32, list<ServiceIdentifier>*>::iterator it = m_serviceIDMap.begin(); it != m_serviceIDMap.end(); ++it)
	{
		sidList = it->second;

		printf("appport: %d\n", it->first);

		// 遍历链表
		for (list<ServiceIdentifier>::iterator sidIt = sidList->begin(); sidIt != sidList->end(); ++sidIt)
		{
			printf("ServiceID: %s\n", ((ServiceIdentifier)(*sidIt)).toString().c_str());
		}
	}
}