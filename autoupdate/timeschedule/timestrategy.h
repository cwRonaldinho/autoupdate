#pragma once

#include <time.h>
#include <stdio.h>

class TimeStrategy
{
public:
	~TimeStrategy() { printf("~TimeStrategy\n"); }
	virtual int CheckOnTime(struct tm curTime) = 0;

	virtual void SnapShot() = 0;
};

// 每天固定时间
class DayStrategy : public TimeStrategy
{
public:
	DayStrategy(unsigned int hour, unsigned int minute, unsigned int second);

	int CheckOnTime(struct tm curTime);

	void SnapShot();

private:
	unsigned int m_hour;									// 小时  有效值:0-23
	unsigned int m_minute;									// 分钟  有效值:0-59
	unsigned int m_second;									// 秒数  有效值:0-59
};

// 每月固定时间
class MonthStrategy : public TimeStrategy
{
public:
	MonthStrategy(unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);

	int CheckOnTime(struct tm curTime);

	void SnapShot();

private:
	unsigned int m_day;										// 日期  有效值:1-31
	unsigned int m_hour;									// 小时  有效值:0-23
	unsigned int m_minute;									// 分钟  有效值:0-59
	unsigned int m_second;									// 秒数  有效值:0-59
};