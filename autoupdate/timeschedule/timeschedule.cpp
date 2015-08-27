#include "timeschedule.h"
#include "timeobserver.h"
#include "timestrategy.h"
#include <windows.h>
#include <process.h>

// 线程函数
void ThreadFun(void* p)
{
	TimeSchedule *ts = (TimeSchedule*)p;
	while (0 == ts->Runing())
	{
		Sleep(1000);
		ts->CheckOnTime();
	}
}

ObserverInfo::ObserverInfo(TimeObserver* observer, TimeStrategy* strategy) 
: m_observer(observer), m_strategy(strategy)
{

}

TimeSchedule::TimeSchedule()
{
	m_bStop = 0;
}

TimeSchedule::~TimeSchedule()
{
	Stop();

	for (list<ObserverInfo*>::iterator it = m_observerList.begin(); it != m_observerList.end(); ++it)
	{
		ObserverInfo *p = *it;
		delete p;
		p = NULL;
	}

	m_observerList.clear();
}

/************************************************************************/
/* 
功能: 开始计时
参数:
返回:
*/
/************************************************************************/
void TimeSchedule::Start()
{
	_beginthread(ThreadFun, 0, this);
}

/************************************************************************/
/* 
功能: 往本调度器注册时间观察者
参数:
	observer: 观察者
	strategy: 时间策略
返回:
*/
/************************************************************************/
void TimeSchedule::Register(TimeObserver* observer, TimeStrategy* strategy)
{
	ObserverInfo *pInfo = new ObserverInfo(observer, strategy);

	m_observerList.push_back(pInfo);
}

/************************************************************************/
/* 
功能: 在本调度器中注销时间观察者
参数:
	observer: 观察者
	strategy: 时间策略
返回:
*/
/************************************************************************/
void TimeSchedule::UnRegister(TimeObserver* observer, TimeStrategy* strategy)
{
	ObserverInfo *pInfo = new ObserverInfo(observer, strategy);

	for (list<ObserverInfo*>::iterator it = m_observerList.begin(); it != m_observerList.end();)
	{
		if (observer == (*it)->m_observer && strategy == (*it)->m_strategy)
		{
			ObserverInfo *p = *it;
			delete p;
			p = NULL;

			m_observerList.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

/************************************************************************/
/* 
功能: 检查本调度器内所有时间观察者是否到期
参数:
返回:
*/
/************************************************************************/
void TimeSchedule::CheckOnTime()
{
	time_t rawTime;
	time(&rawTime);

	struct tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	
	// 遍历所有观察者，进行到期判断
	for (list<ObserverInfo*>::iterator it = m_observerList.begin(); it != m_observerList.end(); ++ it)
	{
		if ((*it)->m_strategy->CheckOnTime(timeInfo))
		{
			(*it)->m_observer->Notify();
		}
	}
}

/************************************************************************/
/* 
功能: 输出快照信息
参数:
返回:
*/
/************************************************************************/
void TimeSchedule::SnapShot()
{
	printf("Snapshot:\n");
	int count = 1;
	for (list<ObserverInfo*>::iterator it = m_observerList.begin(); it != m_observerList.end(); ++ it)
	{
		printf("observer info %d:\n", count++);
		//printf("\tobserver: 0x%08x\n", (void*)(*it)->m_observer);
		printf("\t");
		(*it)->m_strategy->SnapShot();
	}
}