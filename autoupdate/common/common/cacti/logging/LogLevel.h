#ifndef INCLUDED_LOGGING_LOGLEVEL_H
#define INCLUDED_LOGGING_LOGLEVEL_H

#include <string>

#ifdef ERR
#undef ERR
#endif

namespace cacti 
{
	class LogControlID
	{
	public:
		LogControlID(int value)
			: m_value(value)
		{
		}
		LogControlID(int value, const std::string& label)
			: m_value(value)
			, m_label(label)
		{
		}

		const std::string& getLabel() const
		{
			return m_label;
		}
		
		const int getValue() const
		{
			return m_value;
		}
		
		operator int() const
		{
			return getValue();
		}

	private:
		int m_value;
		std::string m_label;
	};

	struct LogLevel 
	{
		static const LogControlID FATAL;
		static const LogControlID ALERT;
		static const LogControlID ERR;
		static const LogControlID WARN;
		static const LogControlID INFO;
		static const LogControlID DBG;
		static const LogControlID NOTSET;
	};
}

#endif // INCLUDED_LOGGING_LOGLEVEL_H
