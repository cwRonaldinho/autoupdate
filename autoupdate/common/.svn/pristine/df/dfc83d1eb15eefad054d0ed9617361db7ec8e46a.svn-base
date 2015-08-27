#ifndef INCLUDED_LOGGING_FORMATTER_H
#define INCLUDED_LOGGING_FORMATTER_H

#include <cacti/logging/LogRecord.h>

#include <boost/shared_ptr.hpp>

#include <string>

namespace cacti
{
	class Formatter
	{
	public:
		virtual ~Formatter() { };

		/**
		* Formats the LoggingEvent data to a string that appenders can log.
		* Implement this method to create your own layout format.
		* @param event The LoggingEvent.
		* @returns an logable string.
		**/
		virtual void format(std::ostream& stream, const LogRecord& rec) = 0;
	};

	typedef boost::shared_ptr<Formatter> FormatterPtr;

	/**
	* SimpleFormatter consists of the LogLevel of the log statement,
	* followed by " - " and then the log message itself. For example,
	*
	* <pre>
	*         DEBUG - Hello world
	* </pre>
	**/
	class SimpleFormatter : public Formatter
	{
	public:
		SimpleFormatter(){};
		SimpleFormatter(PARAMMAP& param){};
		virtual void format(std::ostream& stream, const LogRecord& rec);
	};      

	/**
	* TTICFormatter
	* TTCCFormatter format consists of time, thread, index and 
	* diagnostic context information, hence the name.
	* MMDD HH:MM::SS <thread> [index] log message
	* <pre>
	* 0530 17:18:26 [00000001] device(0x3) already exist!
	* </pre
	**/
	class TTICFormatter : public Formatter
	{
	public:
		TTICFormatter(PARAMMAP& param);
		TTICFormatter()
			: m_logIndex(false)
			, m_logTime(true)
			, m_logThread(false)
			, m_logName(false)
			, m_logLevel(true)
			, m_autoNewLine(false)
		{
		}

		bool autoNewLine()		    { return m_autoNewLine; }
		void autoNewLine(bool flag)	{ m_autoNewLine = flag;	}

		bool logTime()				{ return m_logTime;		}
		void logTime(bool flag)		{ m_logTime = flag;		}

		bool logIndex()				{ return m_logIndex;	}
		void logIndex(bool flag)	{ m_logIndex = flag;	}
		
		bool logThread()			{ return m_logThread;	}
		void logThread(bool flag)	{ m_logThread = flag;	}

		bool logName()				{ return m_logName;		}
		void logName(bool flag)		{ m_logName = flag;		}

		bool logLevel()				{ return m_logLevel;	}
		void logLevel(bool flag)	{ m_logLevel = flag;	}

		virtual void format(std::ostream& stream, const LogRecord& rec);
	
	protected:
		virtual void formatTimestamp(std::ostream& stream, 
			const Timestamp& time);

	private:
		bool m_autoNewLine;
		bool m_logTime;
		bool m_logIndex;
		bool m_logName;
		bool m_logThread;
		bool m_logLevel;
	};

	class StarFormatter : public TTICFormatter
	{
	public:
		StarFormatter(){};
		StarFormatter(PARAMMAP& param);
		virtual void format(std::ostream& stream, const LogRecord& rec);
	private:
		char m_buffer[8000];
	};
}

#endif // INCLUDED_LOGGING_FORMATTER_H

