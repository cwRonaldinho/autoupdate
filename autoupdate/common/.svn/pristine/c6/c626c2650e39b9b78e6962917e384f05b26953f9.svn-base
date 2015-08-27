#include "stdafx.h"

#include <cacti/logging/LogManager.h>

#include <cacti/logging/LogLevel.h>
#include <cacti/logging/ConsoleHandler.h>

#include <boost/shared_ptr.hpp>

namespace
{
	class FreeLogManager
	{
	public:
		~FreeLogManager()
		{
			cacti::LogManager::shutdown();
		}
	};
}
namespace cacti 
{
	FreeLogManager s_freeLogManager;
	RecursiveMutex LogManager::s_mngMutex;
	LogManager* LogManager::s_defaultManager = 0;
	// must place below s_defaultManager

	LogManager& LogManager::getDefaultManager() 
	{
		// use double check lock
		if(!s_defaultManager)
		{
			RecursiveMutex::ScopedLock lock(s_mngMutex);
			{
				if(!s_defaultManager)
				{
					s_defaultManager = new LogManager;
				}
			}
		}
		
		return *s_defaultManager;
	}

	LogManager::LogManager() 
	{
	}

	LogManager::~LogManager() 
	{
	}

	Logger& LogManager::getInstance(const std::string& name) 
	{
		RecursiveMutex::ScopedLock lock(m_loggerMutex);
		return *innerGetInstance(name);
	}

	// assume lock is held
	LoggerPtr LogManager::innerGetExistingInstance(const std::string& name) 
	{
		LoggerPtr result;

		LoggerMap::iterator it = m_loggerMap.find(name);
		if (m_loggerMap.end() != it) 
		{
			result = it->second;
		}

		return result;
	}

	// assume lock is held
	LoggerPtr LogManager::innerGetInstance(const std::string& name) 
	{
		LoggerPtr result = innerGetExistingInstance(name);

		if (!result)
		{
			// the root
			if (name == "") 
			{
				result.reset(new Logger(name, LoggerPtr(), LogLevel::INFO));
				result->addHandler(LogHandlerPtr(new ConsoleHandler(true, true)));
			} 
			else 
			{
				std::string parentName;
				size_t dotIndex = name.find_last_of('.');
				if (name.length() <= dotIndex) 
				{
					parentName = "";
				} 
				else 
				{
					parentName = name.substr(0, dotIndex);
				}
				LoggerPtr parent = innerGetInstance(parentName);
				result.reset(new Logger(name, parent, LogLevel::NOTSET));
			}	  
			m_loggerMap[name] = result;
		}
		return result;
	}

	bool loadConfigure(const char* xmlfile)
	{
		return false;		
	}
	// free the system, all objects will be free after the s_defaultManager 
	// was deleted
	void LogManager::shutdown() 
	{
		delete s_defaultManager;
		s_defaultManager = 0;
	}
}
