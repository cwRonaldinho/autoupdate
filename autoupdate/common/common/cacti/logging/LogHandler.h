#ifndef INCLUDED_LOGGING_LOG_HANDLER_H
#define INCLUDED_LOGGING_LOG_HANDLER_H

#include <cacti/logging/LogRecord.h>
#include <cacti/logging/Filter.h>
#include <cacti/logging/Formatter.h>
#include <cacti/kernel/Mutex.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <map>
#include <set>
#include <list>

namespace cacti 
{
	class LogHandler 
	{
	public:
		LogHandler();
		virtual ~LogHandler(){};

		void publish(const LogRecord& rec);

		void setFormatter(FormatterPtr formatter);
		FormatterPtr getFormatter();

		void addFilter(FilterPtr filter);
		std::list<FilterPtr> getFilters();

		void setName(const std::string& name);
		std::string getName();

	protected:
		// Log in LogHandler specific way.
		// @param event  The LogRecord to log.
		virtual void doPublish(const LogRecord& rec) = 0;
		
		FilterResult checkFilter(const LogRecord& rec);

	protected:
		std::string m_name;
		// only one formatter, but can have many filters
		FormatterPtr m_formatter;	
		std::list<FilterPtr> m_filters;
	};

	typedef boost::shared_ptr<LogHandler> LogHandlerPtr;
}

#endif // INCLUDED_LOGGING_LOG_HANDLER_H
