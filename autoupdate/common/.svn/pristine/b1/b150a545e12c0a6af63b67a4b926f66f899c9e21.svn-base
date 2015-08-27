#include "stdafx.h"

#include <cacti/logging/LoggerStream.h>

#include <cacti/logging/Logger.h>

#define new DEBUG_NEW

namespace cacti 
{

	LoggerStream::LoggerStream(Logger& logger, const LogControlID& level) 
		: m_logger(logger)
		, m_level(level)
		, m_buffer(0)
	{
	}

	LoggerStream::~LoggerStream() 
	{ 
		flush();
	}

	LoggerStream& LoggerStream::operator<<(LoggerStream::Separator separator) 
	{
		flush();
		return *this;
	}

	void LoggerStream::flush() 
	{
		if(m_buffer)
		{
			getLogger().log(getLevel(), m_buffer->str());
			delete m_buffer;
			m_buffer = 0;
		}
	}
}
