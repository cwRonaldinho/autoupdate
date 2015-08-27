#ifndef INCLUDED_LOGGING_LOG_CONSOLE_HANDLER_H
#define INCLUDED_LOGGING_LOG_CONSOLE_HANDLER_H

#include <cacti/logging/LogHandler.h>

namespace cacti 
{
	class ConsoleHandler : public LogHandler
	{
	public:
		ConsoleHandler(PARAMMAP& param);
		ConsoleHandler(bool logToStdErr = false, 
					   bool immediateFlush = true);
		virtual ~ConsoleHandler();

		void flush();
		
	protected:
		// Log in ConsoleHandler specific way.
		// @param event  The LogRecord to log.
		virtual void doPublish(const LogRecord& rec);
		bool hasChinese(const char* message);

	protected:
		FILE* m_stream;
		bool m_immediateFlush;

	private:
		ConsoleHandler(const ConsoleHandler&);
		ConsoleHandler& operator = (const ConsoleHandler&);
	};
}

#endif // INCLUDED_LOGGING_LOG_CONSOLE_HANDLER_H

