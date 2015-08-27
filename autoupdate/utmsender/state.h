#ifndef __STATE__H__
#define __STATE__H__

#include <string>
using namespace std;

class State
{
public:
	State(const char* func="") {m_funcName = func;}		// ÒþÊ½×ª»»
	State(const string& funcName){m_funcName = funcName;}

	State& operator=(const State& s);
	int operator==(const State& s);
	
	string GetFuncName() const { return m_funcName; }
	void SetFuncName(std::string val) { m_funcName = val; }
	
private:
	string m_funcName;	
};

#endif
