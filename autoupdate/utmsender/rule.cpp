#include "rule.h"

//#include "global.h"

Rule::Rule(int iEvent, string sEvent, int status, State state) : m_iEvent(iEvent), m_sEvent(sEvent), m_status(status), m_state(state)
{

}

int Rule::operator==(const Rule& r)
{
	if (r.m_iEvent != this->m_iEvent)
	{
		return 0;
	}
	
	if (this->m_status == -1)		// status为-1表示匹配任何状态值
	{
		return 1;
	}
	
	return this->m_status==r.m_status;
}

Rule& Rule::operator=(const Rule& r)
{	
	this->m_iEvent = r.m_iEvent;
	this->m_sEvent = r.m_sEvent;
	this->m_status = r.m_status;
	this->m_state = r.m_state;
	return *this;
}

/* 
功能: 查询匹配的规则
参数:
	rule: 查询条件
	retRule: 查询到的规则
返回:
	0: 获取到匹配规则
	1: 未获取到匹配规则
*/
int RuleManager::GetMatchedRule(const Rule& rule, Rule& retRule)
{
	// __TW__DEBUG
	//SnapShot();
	
	vector<Rule*>::iterator it = m_rules.begin();
	for (; it!=m_rules.end(); ++it)
	{
		if (**it == rule)
		{
			retRule = **it;
			return 0;
		}
	}

	return 1;
}

/* 
功能: 添加规则
参数:
	rule: 规则
返回:
	0: 成功
	1: 失败
*/
int RuleManager::AddRule(Rule* rule)
{
	if (!rule)
	{
		return 1;
	}
	
	vector<Rule*>::iterator it = m_rules.begin();
	for (; it!=m_rules.end(); ++it)
	{
		if (**it == *rule)
		{
			return 1;
		}
	}

	m_rules.push_back(rule);	
	return 0;
}

/* 
功能: 清空规则
参数:
返回:
	0: 成功
	1: 失败
*/	
int RuleManager::ClearRule()
{
	for (vector<Rule*>::iterator it=m_rules.begin(); it!=m_rules.end(); ++it)
  {
    Rule* tmp = *it;
    delete tmp;
  }

  m_rules.clear();
  
	return 0;
}

void RuleManager::SnapShot()
{
	//PrintConsole("RuleManager", "Rule manager snapshot:");
	vector<Rule*>::iterator it = m_rules.begin();
	for (; it!=m_rules.end(); ++it)
	{
		//PrintConsole("RuleManager", "rule: %s %d %s", (*it)->m_sEvent.c_str(), (*it)->m_status, (*it)->m_state.GetFuncName().c_str());	
	}
}
