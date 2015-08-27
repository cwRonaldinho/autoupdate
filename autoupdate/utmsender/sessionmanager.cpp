#include "sessionmanager.h"
#include "sessionobject.h"

extern void mprint(const char* fmt, ...);

TSessionManager::TSessionManager()
{
	m_curSessionID = 0;
	m_nSessionMaxNum = 500;
}

TSessionManager::~TSessionManager()
{
	ClearAll();
}

void TSessionManager::ClearAll()
{
	TLuaSession *obj;
	map<int, TLuaSession*>::iterator it;
	for (it = m_sessionTable.begin(); it != m_sessionTable.end(); ++it)
	{
		obj = it->second;
		delete obj;
		obj = NULL;

		m_sessionTable.erase(it->first);
	}
}

int TSessionManager::AddSessionObject(TLuaSession* obj)
{
	if(!obj)
		return -1; //输入参数不正确

	if(GetCount() >= m_nSessionMaxNum)
	{
		printf("[0x%08X]Warning 队列数超过设定容量%d!进入map!"
			,obj->GetID(),m_nSessionMaxNum);
	}

	// 如果超过SessionTable的最大容量，则返回失败
	if(GetCount() >= 1024)
		return -1;

	TLuaSession* retObj = NULL;
	if(m_sessionTable.find(obj->m_SessionID) != m_sessionTable.end())
	{
		return -2; //已经有相同ID的对象存在
	}

	m_sessionTable.insert(pair<int, TLuaSession*>(obj->m_SessionID, obj));

	return 0; 
}

TLuaSession* TSessionManager::GetSessionObject(int SessionID)
{
	map<int, TLuaSession*>::iterator it;
	if((it = m_sessionTable.find(SessionID)) != m_sessionTable.end())
	{
		return it->second;
	}

	return NULL;
}

void  TSessionManager::DeleteSessionObject(int SessionID)
{
	map<int, TLuaSession*>::iterator it;
	if((it = m_sessionTable.find(SessionID)) != m_sessionTable.end())
	{
		if(it->second)
		{
			delete (TLuaSession*)it->second;
		}
		m_sessionTable.erase(SessionID);
	}

	return ;
}

int TSessionManager::GenerateSessionID()
{
	m_curSessionID++;
	if(m_curSessionID > 0x0FFFFFFE)
		m_curSessionID = 0;

	return m_curSessionID;
}

void TSessionManager::Snapshot(FILE* fp)
{
	mprint("Total session count: %d\n", m_sessionTable.size());

	map<int, TLuaSession*>::iterator it;
	for (it = m_sessionTable.begin(); it != m_sessionTable.end(); ++it)
	{
		mprint("session id: 0x%08x\n", it->first);
	}
}
 
// RTCS_CTstring TSessionManager::GetSnapshot()
// {
// 	TLuaSession* ret_obj;
// 	int ret_sessionid;
// 	MP_POSITION pos = NULL;
// 
// 	//Lock();
// 
// 	int TotalCount = GetCount();
// 	char szTemp[1024]= "";
// 	sprintf(szTemp, "Total Session Count = %d.\n", TotalCount);
// 
// 	RTCS_CTstring szSnapshot = szTemp;
// 
// 	pos = GetStartPosition();
// 	while(pos)
// 	{
// 		ret_obj = NULL;
// 		GetNextAssoc(pos, ret_sessionid, ret_obj);
// 		if(ret_obj)
// 		{
// 			szSnapshot += GetSnapshot();
// 		}
// 	}
// 
// 	//Unlock();
// 	return szSnapshot;
// }