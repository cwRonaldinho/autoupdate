#include "stdafx.h"

#include <stdio.h>
#include <cacti/logging/LogRecord.h>
#include <cacti/kernel/Thread.h>

#define new DEBUG_NEW

namespace 
{
	std::string makeThreadName()
	{
		char buffer[16];
		sprintf(buffer, "%lu", cacti::Thread::currentId());
		return buffer;
	}
}

namespace cacti
{    
    LogRecord::LogRecord(const std::string& loggerName, 
                               const char* format,
							   va_list args,
                               const LogControlID& level,
							   bool hasIndex /*= false*/,
							   unsigned int index/* = 0*/,
							   const Timestamp& ts /*= Timestamp::getNowTime() */) 
		: loggerName(loggerName)
		, format(format)
		, args(args)
		, level(level)
		, threadName(makeThreadName())
		, hasIndex(hasIndex)
		, index(index)
		, timestamp(ts)
	{
	}

	LogRecord::LogRecord(const std::string& loggerName, 
		const std::string& message,
		const LogControlID& level,
		bool hasIndex /*= false*/,
		unsigned int index/* = 0*/,
		const Timestamp& ts /*= Timestamp::getNowTime() */) 
		: loggerName(loggerName)
		, message(message)
		, format(NULL)
		, args(NULL)
		, level(level)
		, threadName(makeThreadName())
		, hasIndex(hasIndex)
		, index(index)
		, timestamp(ts)
	{
	}
}
