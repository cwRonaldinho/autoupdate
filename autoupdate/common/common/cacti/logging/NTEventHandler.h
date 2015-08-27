#ifndef INCLUDED_LOGGING_LOG_NTEVENT_HANDLER_H
#define INCLUDED_LOGGING_LOG_NTEVENT_HANDLER_H

#ifdef WIN32

#include <cacti/logging/LogHandler.h>

namespace cacti 
{
	class NTEventHandler : public LogHandler
	{
	public:
		NTEventHandler(
			const std::string& server, 
			const std::string& log, 
			const std::string& source);

		virtual ~NTEventHandler();

	protected:
		// Log in ConsoleHandler specific way.
		// @param event  The LogRecord to log.
		virtual void doPublish(const LogRecord& rec);

		virtual WORD getEventType(const LogRecord& rec);
		virtual WORD getEventCategory(const LogRecord& rec);
		void addRegistryInfo();

	protected:
		std::string m_server;
		std::string m_log;
		std::string m_source;
		HANDLE m_hEventLog;
		SID* m_pCurrentUserSID;

	private:
		NTEventHandler(const NTEventHandler&);
		NTEventHandler& operator = (const NTEventHandler&);
	};
}
#else
#error NTEventHandler is not available on none-Win32 platforms
#endif

#endif // INCLUDED_LOGGING_LOG_NTEVENT_HANDLER_H
