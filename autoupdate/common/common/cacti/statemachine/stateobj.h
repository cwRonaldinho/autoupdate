#ifndef INCLUDE_STATEOBJ_H
#define	INCLUDE_STATEOBJ_H
#pragma once
#include "cacti/message/TypeDef.h"
#include "cacti/kernel/SysObj.h"
#include <string>

class StateObj
{
public:
	StateObj()	{m_state = 0;m_objId = 0; m_stateName = "";};
	~StateObj()	{};
public:
	inline u32	getState()				{return m_state;}
	inline void setState(u32 state)		{m_state = state;}
	inline u32 getObjId(){return m_objId;};
	inline void setObjId(u32 objId){m_objId = objId;};
	inline std::string getStateName(){return m_stateName;};
	inline void setStateName(std::string name){m_stateName = name;};
private:
	unsigned long m_state;
	unsigned long m_objId;
	std::string m_stateName;
};


#endif
