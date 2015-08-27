#include "stdafx.h"

#include "LazyLog.h"


#include <stdarg.h>
#include <iomanip>
using namespace std;

#include "cacti/config/Portability.h"
#include "cacti/util/StringUtil.h"
#include "cacti/util/FileSystem.h"


namespace cacti
{
	Timestamp StarFileHandler::calculateNextRolloverTime(const Timestamp& t) const
	{
		return (t + Timestamp(86400)); //      24 * 60 * 60 seconds
	}

	std::string StarFileHandler::getFilename(const Timestamp& t) const
	{
		std::string pattern;
		pattern = ("%Y-%m-%d");
		return m_filename + (".") + t.getFormattedTime(pattern.c_str(), false);
	}
	///////////////////////////////////////////////////////////////////////////

	LazyLog::LazyLog(const char *module, const char* version)
	{
		assert(module);
		assert(version);

		m_enabled = false;
		FileSystem::mkdir("./log");
		m_module = module;
		m_version = version;
		m_logImpl = NULL;
		m_handler = NULL;
	}

	LazyLog::~LazyLog()
	{
	}

	void LazyLog::open()
	{ 
		if(!m_logImpl)
		{
			std::string logfile = "./log/";
			logfile += m_module;
			logfile += ".log";

			m_handler = new StarFileHandler(logfile);
			m_handler->setFormatter(FormatterPtr(new StarFormatter()));
			m_handlerHolder.reset(m_handler);

			m_logImpl = &Logger::getInstance(m_module.c_str());
			m_logImpl->addHandler(m_handlerHolder);
			m_enabled = true;
			print("%s\n", m_module.c_str());
			print("%s\n", m_version.c_str());
		}
	}

	void LazyLog::close()
	{
		if(m_logImpl)
		{
			m_logImpl->fatal("logger off!!!");
			m_enabled = false;
			m_logImpl->setLevel(LogControlID(-1, "DISABLE"));
		}
	}

	void LazyLog::flush()
	{
		if(m_handler)
			m_handler->flush();
	}

	void LazyLog::setLevel(LogControlID level)
	{
		assert(m_logImpl);	// must open 
		if(m_logImpl)
			m_logImpl->setLevel(level);
	}

	void LazyLog::setImmediateFlush(bool flag)
	{
		if(m_handler)
			m_handler->setImmediateFlush(flag); 
	}

	void LazyLog::setMaxSize(long newSize)
	{
		if(m_handler)
			m_handler->setMaxFileSize(newSize);
	}

	void LazyLog::vprint(unsigned int ID, const char *format, va_list argptr)
	{
		if(m_enabled)
		{
			m_logImpl->logva(LogLevel::INFO, ID, format, argptr);
		}
	}

	void LazyLog::vprint(const char *format, va_list argptr)
	{
		if(m_enabled)
		{
			m_logImpl->logva(LogLevel::INFO, format, argptr);
		}
	}


	void LazyLog::print(const char *format,...)
	{
		if(m_enabled)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logva(LogLevel::INFO, format, va);
			va_end(va);
		}
	}
	void LazyLog::trace(LogControlID level, const char *format,...)
	{
		if(m_enabled)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logva(level, format, va);
			va_end(va);
		}
	}

	void LazyLog::print(unsigned int ID, const char *format,...)
	{
		if(m_enabled)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logva(LogLevel::INFO, ID, format, va);
			va_end(va);
		}
	}

	void LazyLog::printnt(const char *format,...)
	{
		if(m_enabled)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logvaNoTime(LogLevel::INFO, format, va);
			va_end(va);
		}
	}
	void LazyLog::tracent(LogControlID level, const char *format,...)
	{
		if(m_enabled)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logvaNoTime(level, format, va);
			va_end(va);
		}
	}

	void LazyLog::printb(const char *title, 
		const unsigned char *buf, int len)
	{
		if(m_enabled)
		{
			std::string binBuf = title;
			StringUtil::formatBinary(buf, len, binBuf);
			m_logImpl->log(LogLevel::INFO, binBuf);
		}
	}

	void LazyLog::traceb(LogControlID level, const char *title, 
		const unsigned char *buf, int len)
	{
		if(m_enabled)
		{
			std::string binBuf = title;
			StringUtil::formatBinary(buf, len, binBuf);
			m_logImpl->log(level, binBuf);
		}
	}

	void LazyLog::printb(unsigned int ID, const char *title, 
		const unsigned char *buf, int len)
	{
		if(m_enabled)
		{
			std::string binBuf = title;
			cacti::StringUtil::formatBinary(buf, len, binBuf);
			m_logImpl->log(LogLevel::INFO, ID, binBuf);
		}
	}

	void LazyLog::traceb(unsigned int ID, LogControlID level, 
		const char *title, 
		const unsigned char *buf, int len)
	{
		if(m_enabled)
		{
			std::string binBuf = title;
			StringUtil::formatBinary(buf, len, binBuf);
			m_logImpl->log(level, ID, binBuf);
		}
	}

	void LazyLog::printnb(const unsigned char *buf,int len)
	{
		if(m_enabled)
		{
			std::string binBuf;
			StringUtil::formatBinary(buf, len, binBuf);
			m_logImpl->log(LogLevel::INFO, binBuf);
		}
	}

	void LazyLog::tracenb(LogControlID level, const unsigned char *buf,int len)
	{
		if(m_enabled)
		{
			std::string binBuf;
			StringUtil::formatBinary(buf, len, binBuf);
		}
	}
	void LazyLog::perror(const char *format,...)
	{
		if(m_logImpl)
		{
			va_list va;
			va_start(va, format);
			m_logImpl->logva(LogLevel::ERR, format, va);
			va_end(va);
		}
	}
	Logger& LazyLog::getLogger()
	{
		assert(m_logImpl);	// must open
		if(!m_logImpl)
		{
			return *m_logImpl;
		}
		else
		{
			return Logger::getInstance("");
		}
	}
}
