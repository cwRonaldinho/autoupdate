#ifndef __RULE__H__
#define __RULE__H__

#include <string>
#include <vector>
using namespace std;

#include "state.h"

class Rule
{
public:
	Rule(){}
	Rule(int iEvent, string sEvent, int status, State state);

	int operator==(const Rule& r);
	Rule& operator=(const Rule& r);

public:
	int m_iEvent;				// 事件对应的整型值
	string m_sEvent;		// 事件对应的字符串值
	int m_status;				// 事件状态
	State m_state;			// 状态
};

class RuleManager
{
public:
	RuleManager(){}

	int GetMatchedRule(const Rule& rule, Rule& retRule);
	int AddRule(Rule* rule);
	int ClearRule();
	
	void SnapShot();
	
private:
	vector<Rule*> m_rules;
};
#endif
