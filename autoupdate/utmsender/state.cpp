#include "state.h"

State& State::operator=(const State& s)
{	
	this->m_funcName = s.m_funcName;
	return *this;
}

int State::operator==(const State& s)
{
	if (s.m_funcName == m_funcName)
	{
		return 1;
	}
	
	return 0;
}