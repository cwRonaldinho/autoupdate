#ifndef INCLUDED_LOGMANAGER_H
#define INCLUDED_LOGMANAGER_H

#include <cacti/kernel/SysObj.h>

#include <cacti/logging/Logger.h>

#include <string>
#include <map>
#include <vector>

namespace cacti
{
	class LogManager 
	{
		// friend class for delete m_defaultManager
		// 
	private:
		LogManager();
		LogManager(const LogManager&);
		LogManager& operator = (const LogManager&);
		virtual ~LogManager();

	public:
		typedef std::map<std::string, LoggerPtr> LoggerMap;

		static LogManager& getDefaultManager();

		virtual Logger& getInstance(const std::string& name);
		static void shutdown();

	protected:
		virtual LoggerPtr innerGetExistingInstance(const std::string& name);

		virtual LoggerPtr innerGetInstance(const std::string& name);

		LoggerMap m_loggerMap;

		mutable RecursiveMutex m_loggerMutex;

		static RecursiveMutex s_mngMutex;
		static LogManager* s_defaultManager;
	};        
}
#endif
