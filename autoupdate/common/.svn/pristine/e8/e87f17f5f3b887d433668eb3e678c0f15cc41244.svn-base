#ifndef INCLUDED_LOGGING_FILTER_H
#define INCLUDED_LOGGING_FILTER_H

#include <cacti/logging/LogRecord.h>

#include <boost/shared_ptr.hpp>

namespace cacti
{
	enum FilterResult 
	{ 
		DENY, 
		/**< The log event must be dropped immediately 
		 *  without consulting with the remaining 
		 *  filters, if any, in the chain. */
		 NEUTRAL, 
		 /**< This filter is neutral with respect to
          *  the log event; the remaining filters, if 
          *  if any, should be consulted for a final 
          *  decision. */
		  ACCEPT 
		  /**< The log event must be logged immediately 
           *  without consulting with the remaining 
           *  filters, if any, in the chain. */
	};

	class Filter
	{
	public:
		virtual ~Filter() {};

		virtual FilterResult decide(const LogRecord& rec) = 0;
	}; 

	typedef boost::shared_ptr<Filter> FilterPtr;

	class DenyAllFilter : public Filter
	{
	public:
		virtual FilterResult decide(const LogRecord& rec);
	};

	class LevelMatchFilter : public Filter
	{
	public:
		LevelMatchFilter(bool acceptOnMatch, LogControlID matchLevel);

		virtual FilterResult decide(const LogRecord& rec);

	private:
		bool m_acceptOnMatch;
		LogControlID m_matchLevel;
	};

	class LevelRangeFilter : public Filter
	{
	public:
		LevelRangeFilter(bool acceptOnMatch, LogControlID min, LogControlID max);

		virtual FilterResult decide(const LogRecord& rec);

	private:
		bool m_acceptOnMatch;
		LogControlID m_minLevel;
		LogControlID m_maxLevel;
	};

	//class StringMatchFilter : public Filter
	//{
	//public:
	//	StringMatchFilter(bool acceptOnMatch, const char* matchString);

	//	virtual FilterResult decide(const LogRecord& rec);

	//private:
	//	bool m_acceptOnMatch;
	//	std::string m_stringToMatch;
	//};
}

#endif // INCLUDED_LOGGING_FILTER_H
