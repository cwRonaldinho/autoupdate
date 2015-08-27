#include "stdafx.h"

#include <cacti/logging/ConsoleHandler.h>

#include <iostream>

using namespace std;

#define new DEBUG_NEW

namespace
{
	static cacti::RecursiveMutex s_mutex;
}
namespace cacti
{
	ConsoleHandler::ConsoleHandler(PARAMMAP& param)
	{
		if(param["where"] == "stderr")
			m_stream = stderr;
		else 
			m_stream = stdout;

		if(param["flush"] == "false")
			m_immediateFlush = false;
		else
			m_immediateFlush = true;			
	}

	ConsoleHandler::ConsoleHandler(bool logToStdErr /*= false*/, 
								   bool immediateFlush /*= true*/)
		: m_immediateFlush(immediateFlush)
	{
		if(logToStdErr)
			m_stream = stderr;
		else
			m_stream = stdout;
	}

	ConsoleHandler::~ConsoleHandler()
	{
	}

	void ConsoleHandler::flush()
	{
		RecursiveMutex::ScopedLock lock(s_mutex);
		{
			fflush(m_stream);			
		}
	}


	void ConsoleHandler::doPublish(const LogRecord& rec)
	{
		RecursiveMutex::ScopedLock lock(s_mutex);
		{
			if(rec.format)
				vfprintf(m_stream, rec.format, rec.args);
			else
				fprintf(m_stream, "%s", rec.message.c_str());
			if(m_immediateFlush)
				fflush(m_stream);
		}
	}
}
