#include "stdafx.h"

#include <cacti/logging/Filter.h>

namespace cacti
{
	FilterResult DenyAllFilter::decide(const LogRecord& rec)
	{
		return DENY;
	}

	//////////////////////////////////////////////////////////////////////////	
	LevelMatchFilter::LevelMatchFilter(bool acceptOnMatch, LogControlID matchLevel)
		: m_acceptOnMatch(acceptOnMatch)
		, m_matchLevel(matchLevel)
	{
	}

	FilterResult LevelMatchFilter::decide(const LogRecord& rec)
	{
		if(m_matchLevel == LogLevel::NOTSET)
			return NEUTRAL;

		if(rec.level == m_matchLevel)
			return m_acceptOnMatch ? ACCEPT : DENY;

		return NEUTRAL;
	}

	//////////////////////////////////////////////////////////////////////////
	LevelRangeFilter::LevelRangeFilter(bool acceptOnMatch, 
									   LogControlID min, LogControlID max)
		: m_acceptOnMatch(acceptOnMatch)
		, m_minLevel(min)
		, m_maxLevel(max)
	{
	}

	FilterResult LevelRangeFilter::decide(const LogRecord& rec)
	{
		if(m_minLevel != LogLevel::NOTSET && rec.level < m_minLevel)
			return DENY;

		if(m_maxLevel != LogLevel::NOTSET && rec.level > m_maxLevel)
			return DENY;

		if(m_acceptOnMatch) {
			// this filter set up to bypass later filters and always return
			// accept if priority in range
			return ACCEPT;
		}
		else {
			// event is ok for this filter; allow later filters to have a look...
			return NEUTRAL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//StringMatchFilter::StringMatchFilter(bool acceptOnMatch, const char* stringToMatch)
	//	: m_acceptOnMatch(acceptOnMatch)
	//	, m_stringToMatch(stringToMatch)
	//{
	//}

	//FilterResult StringMatchFilter::decide(const LogRecord& rec)
	//{
	//	const std::string &message = rec.message;

	//	if(m_stringToMatch.length() == 0 || message.length() == 0) 
	//	{
	//		return NEUTRAL;
	//	}

	//	if(message.find(m_stringToMatch) == std::string::npos) 
	//	{
	//		return NEUTRAL;
	//	}
	//	else 
	//	{  // we've got a match
	//		return (m_acceptOnMatch ? ACCEPT : DENY);
	//	}

	//}
}
