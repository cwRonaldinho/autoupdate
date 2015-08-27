#ifndef __DATA_SOURCE_H
#define __DATA_SOURCE_H

#include <map>
#include <list>
#include "cacti/message/ServiceIdentifier.h"

using namespace cacti;
using namespace std;

class TDataSource
{
public:
	TDataSource();
	~TDataSource();

public:
	u32		addServiceIDByApport(u32 apport, const ServiceIdentifier& sid);			// 添加SID
	u32		removeServiceIDByApport(u32 apport, const ServiceIdentifier& sid);		// 删除SID
	u32		getServiceIDByApport(u32 apport, ServiceIdentifier& sid);				// 获取SID
	// TODO: 遍历操作待考虑

	void	snapShot();

private:
	TDataSource(const TDataSource& ret){}	// 不允许拷贝
	
private:
	map<u32, list<ServiceIdentifier>*> m_serviceIDMap;			// ServiceID map, key为apport，value为动态创建，析构时需释放

};
#endif