#include "stdafx.h"
#include "fsm.h"

#ifndef _NEW_FSM

BaseFsm::BaseFsm() 
:m_LazyLog(NULL)
{

}

BaseFsm::~BaseFsm() 
{
}
u32 BaseFsm::driveFsm(StateObj* stateObj, const ServiceIdentifier& sender, UserTransferMessage& utm)
{
	//printLog("driverFSM this:0x%X, m_inherited:0x%X\n",this, m_inherited);

//	RecursiveMutex::ScopedLock o(m_secure);
	fsmFuncId ffid(utm.getMessageId(), stateObj->getState(), utm.getReturn());

	fsmFuncMap::iterator itEnter = m_fsmFuncMap.find(ffid);
	if(itEnter == m_fsmFuncMap.end())
	{//if no enter function defined, find func rule with ret(-1)
		ffid.ret = FSM_NULL_VALUE;
		itEnter = m_fsmFuncMap.find(ffid);
		if (itEnter == m_fsmFuncMap.end())
		{
			printLog("[%X] <fsm> No rule for msg 0x%X(%d) on state %d(%s)\n",utm.getRes().m_appref, 
				utm.getMessageId(), utm.getReturn(), stateObj->getState(), stateObj->getStateName().c_str());
			return nullFunc(stateObj, sender, utm); //if no func rule with ret(-1), call default exception func 
		}
	}

	ffid.msgId = FSM_NULL_VALUE;
	ffid.ret = FSM_NULL_VALUE;
	printLog("[%X] <fsm> state %d(%s) -> %d(%s) on msg 0x%X(0x%X)\n",utm.getRes().m_appref, 
		stateObj->getState(), stateObj->getStateName().c_str(),
		(*itEnter).second.nextState, (*itEnter).second.nextStateName.c_str(), 
		utm.getMessageId(), utm.getReturn());

	fsmFuncMap::iterator itLeave = m_fsmFuncMap.find(ffid);
	if(itLeave != m_fsmFuncMap.end())
	{
	//find leave func for state
		//printLog("[%X] call leavefunc for state %d(%s)\n", utm.getRes().m_appref,
			//stateObj->getState(), stateObj->getStateName().c_str());
		(m_inherited->*(*itLeave).second.func)(stateObj, sender, utm);
		//printLog("[%X] call leavefunc for state %d(%s) over\n", utm.getRes().m_appref,
			//stateObj->getState(), stateObj->getStateName().c_str());

	}


	


	stateObj->setState((*itEnter).second.nextState);
	stateObj->setStateName((*itEnter).second.nextStateName);
	//printLog("[%X] call enterfunc for state %d(%s)\n", utm.getRes().m_appref,
		//stateObj->getState(), stateObj->getStateName().c_str());
	(m_inherited->*(*itEnter).second.func)(stateObj, sender, utm);
	//printLog("[%X] call enterfunc for state %d(%s) over\n", utm.getRes().m_appref,
		//stateObj->getState(), stateObj->getStateName().c_str());
	return 0;
}

u32 BaseFsm::insertRule(u32 state, u32 msg, u32 ret, u32 newstate, char* statename, fsmFunc func)
{
	fsmFuncId ffid(msg, state, ret);
	fsmFuncDef ffDef(func, newstate, statename);

	pair<fsmFuncMap::iterator, bool> p = m_fsmFuncMap.insert(fsmFuncMap::value_type(ffid, ffDef));

	if(!p.second)
		return -1;
	return 0;

}

void BaseFsm::printLog(const char *format,...)
{
	if (m_LazyLog!= NULL)
	{
		va_list va;
		va_start(va, format);
		char buf[4096] = "";
		vsnprintf(buf, 4095, format, va);
		m_LazyLog->print(buf);
		va_end(va);
	}
}

#else

BaseFsm::BaseFsm(cacti::Logger& logger)
:m_logger(logger)
{
	for(u32 i = 0; i < MAX_STATE_CACHE_INDEX; i++)
		m_stateCache[i] = INVALID_STATE_INDEX_BEGIN;
}

u32 BaseFsm::getStateBeginIndex(u32 stateId)
{
	Rule* rules = getRules();
	u32 stateIndex = stateId % MAX_STATE_CACHE_INDEX;
	if(m_stateCache[stateIndex] != INVALID_STATE_INDEX_BEGIN)
	{
		if(rules[m_stateCache[stateIndex]].m_state == stateId)
			return m_stateCache[stateIndex];
	}

	u32 stateBegin = 0;

	while(rules[stateBegin].m_state != -1)
	{
		if(rules[stateBegin].m_state == stateId)
		{
			m_stateCache[stateIndex] = stateBegin;
			return stateBegin;
		}
		stateBegin++;
	}

	return INVALID_STATE_INDEX_BEGIN;
}

void BaseFsm::driveFsm(StateObj* stateObj, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm)
{
	Rule* rules = getRules();
	u32 stateBegin = getStateBeginIndex(stateObj->getState());
	
	if(stateBegin == INVALID_STATE_INDEX_BEGIN)
	{
		m_logger.error("[%08X] ERROR:FSM not defined state %d\n",
			stateObj->getObjId(), stateObj->getState());
		return;
	}

	bool find = false;
	while(rules[stateBegin].m_state == stateObj->getState())
	{
		if(rules[stateBegin].m_event == utm.getMessageId())
		{
			if(rules[stateBegin].m_eventValue == FSM_NULL_VALUE || rules[stateBegin].m_eventValue == utm.getReturn())
			{
				find = true;
				break;
			}
		}
		stateBegin++;
	}

	if(!find)
	{
		m_logger.info("[%08X] In state %d, RECV EVT 0x%X from (%s),no rule\n",
			stateObj->getObjId(), stateObj->getState(), utm.getMessageId(), sender.toString().c_str());
		noRule(stateObj, sender, utm);
		return;
	}

	leaveState(stateObj, rules[stateBegin].m_nextState, sender, utm);

	u32 oldState = stateObj->getState();

	stateObj->setState(rules[stateBegin].m_nextState);
	u32 objId = stateObj->getObjId();

	entryState(stateObj, oldState, sender, utm);

	// after m_func was called, don't access stateObj anymore, 
	// because stateObj maybe delete in m_func
	(this->*(rules[stateBegin].m_func))(stateObj, sender, utm);

	m_logger.info("[%08X] %s-(%s,%d)->%s\n",
		objId, rules[stateBegin].m_stateName, rules[stateBegin].m_eventName, utm.getReturn(), rules[stateBegin].m_nextStateName);
}

#endif
