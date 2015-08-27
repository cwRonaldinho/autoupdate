#include "stdafx.h"

#include <cacti/logging/LogHandler.h>

#include <cacti/kernel/SysObj.h>


namespace cacti
{
	LogHandler::LogHandler()
	{
		setFormatter(FormatterPtr(new SimpleFormatter));
	}

	void LogHandler::publish(const LogRecord& rec)
	{
		if(checkFilter(rec) != DENY)
		{
			doPublish(rec);
		}
	}

	FilterResult LogHandler::checkFilter(const LogRecord& rec)
	{
		std::list<FilterPtr>::iterator it = m_filters.begin();
		std::list<FilterPtr>::iterator endit = m_filters.end();
		while(it != endit)
		{
			FilterResult result = (*it)->decide(rec);
			if(result != NEUTRAL)
				return result;

			++it;
		}
		return ACCEPT;
	}

	void LogHandler::setFormatter(FormatterPtr formatter)
	{
		m_formatter = formatter;
	}

	FormatterPtr LogHandler::getFormatter()
	{
		return m_formatter;
	}

	void LogHandler::addFilter(FilterPtr filter)
	{
		m_filters.push_back(filter);
	}

	std::list<FilterPtr> LogHandler::getFilters()
	{
		return m_filters;
	}

	void LogHandler::setName(const std::string& name)
	{
		m_name = name;
	}

	std::string LogHandler::getName()
	{
		return m_name;
	}

}
