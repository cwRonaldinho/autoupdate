#ifndef INCLUDE_FSM_H
#define INCLUDE_FSM_H
#pragma once
#include "cacti/message/TransferMessage.h"
#include "cacti/logging/LazyLog.h"
#include "stateobj.h"

#define FSM_NULL_VALUE	(0xF1234567)

#ifndef _NEW_FSM 

#include <map>

using namespace std;
using namespace cacti;

class BaseFsm
{

public:
	BaseFsm();
	virtual ~BaseFsm();

	u32 driveFsm(StateObj* stateObj, const ServiceIdentifier& sender, UserTransferMessage& utm);
	virtual u32 nullFunc(StateObj* stateObj, const ServiceIdentifier& sender, UserTransferMessage& utm) {return 0;};	
	void printLog(const char *format,...);


protected:

	struct fsmFuncId
	{
		u32			msgId;
		u32			stateId;
		u32			ret;

		fsmFuncId(u32 msg, u32 state, u32 r)
			:msgId(msg),stateId(state),ret(r) {}

		bool operator < (const fsmFuncId& ffid) const
		{
			if (msgId >ffid.msgId)
				return false;

			if (msgId <ffid.msgId)
				return true;

			if (stateId >ffid.stateId)
				return false;

			if (stateId <ffid.stateId)
				return true;

			if (ret>= ffid.ret)
				return false;
			return true;

		}

		bool operator == (const fsmFuncId& ffid) const
		{
			return 	((msgId ==ffid.msgId) && (stateId == ffid.stateId) && (ret == ffid.ret));
		}

	};
	typedef u32 (BaseFsm::*fsmFunc)(StateObj* stateObj,const ServiceIdentifier& sender, UserTransferMessage& utm);

	struct fsmFuncDef
	{
		fsmFunc		func;
		u32			nextState;
		string		nextStateName;

		fsmFuncDef(fsmFunc f, u32 state, char* statename)
			:func(f), nextState(state), nextStateName(statename){}
	};


	typedef	map<fsmFuncId, fsmFuncDef>	fsmFuncMap;

	fsmFuncMap		m_fsmFuncMap;
	cacti::LazyLog* m_LazyLog;
	BaseFsm*		m_inherited;

//	RecursiveMutex	m_secure; 	


	u32 insertRule(u32 state, u32 msg, u32 ret, u32 newstate, char* statename, fsmFunc func);

private:
	virtual u32	initFsm() =0 ;

};
#define FSM_INIT	initFsm();

#ifdef  WIN32
#define FSM_BEGIN_RULE_MAP(className)\
	u32 className::initFsm() { \
	u32 temp = (u32)this; \
	m_inherited = (BaseFsm* )temp;
#else
#define FSM_BEGIN_RULE_MAP(className)\
	u32 className::initFsm() { \
	m_inherited = this; 
#endif

#define FSM_END_RULE_MAP\
	return 0; }


#define FSM_DEFINE_RULE(state, msg, ret, newstate, func) \
	insertRule(state, msg, ret, newstate, #newstate, (fsmFunc)func);

#define FSM_DEFINE_LEAVEFUNC(state, func) \
	insertRule(state, FSM_NULL_VALUE, FSM_NULL_VALUE , state, "",(fsmFunc)func);




#else

#include <cacti/logging/Logger.h>

class BaseFsm;

typedef void (BaseFsm::*SMFunc)(StateObj* stateObj, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm);


struct Rule
{
	unsigned long m_state;
	char* m_stateName;
	unsigned long m_event;
	char* m_eventName;
	unsigned long m_eventValue;
	unsigned long m_nextState;
	char* m_nextStateName;
	SMFunc m_func;
};

#define DECLARE_RULE  static Rule m_rules[]; Rule* getRules(){return m_rules;};
#define FSM_BEGIN_RULE_MAP(smClass) Rule smClass::m_rules[] = {
#define FSM_DEFINE_RULE(state, event, value, nextState, func) \
{state, #state, event, #event, value, nextState, #nextState, (void (BaseFsm::*)(StateObj*, const cacti::ServiceIdentifier&, cacti::UserTransferMessage&))&func},

#define FSM_END_RULE_MAP() {-1, "", -1, "", -1, -1, "", 0}};

#define MAX_STATE_CACHE_INDEX			200
#define INVALID_STATE_INDEX_BEGIN		0xFFFFFFFF

class BaseFsm
{
public:
	BaseFsm(cacti::Logger& logger);
	~BaseFsm(){};
public:
	void driveFsm(StateObj* stateObj, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm);
	inline void nullFunc(StateObj* stateObj, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm){}; //donothing
	virtual void noRule(StateObj* stateObj, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm){};
	virtual void leaveState(StateObj* stateObj, u32 newState, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm){};
	virtual void entryState(StateObj* stateObj, u32 oldState, const cacti::ServiceIdentifier& sender, cacti::UserTransferMessage& utm){};
private:
	virtual Rule* getRules() = 0;
	cacti::Logger& m_logger;
	u32 m_stateCache[MAX_STATE_CACHE_INDEX];
	u32 getStateBeginIndex(u32 stateId);
};

#endif

#endif
