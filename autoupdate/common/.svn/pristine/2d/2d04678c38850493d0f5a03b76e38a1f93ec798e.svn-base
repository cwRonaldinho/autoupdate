#ifndef INCLUDED_LOGGING_LOG_RECORD_H
#define INCLUDED_LOGGING_LOG_RECORD_H

#include <cacti/logging/LogLevel.h>
#include <cacti/util/Timestamp.h>

#include <string>
#include <map>

namespace cacti 
{
	typedef std::map<std::string, std::string> PARAMMAP;

    //
    // The internal representation of logging events. When a affirmative
    // logging decision is made a <code>LogRecord</code> instance is
    // created. This instance is passed around the different logging
    // components.
    //
    // <p>This class is of concern to those wishing to extend logging. 
    struct LogRecord {
    public:
        LogRecord(const std::string& logger, const char* format, 
			va_list args,
			const LogControlID& level, 
			bool hasIndex = false, unsigned int index = 0, 
			const Timestamp& ts = Timestamp::getNowTime());

		LogRecord(const std::string& logger, const std::string& message, 
			const LogControlID& level, 
			bool hasIndex = false, unsigned int index = 0, 
			const Timestamp& ts = Timestamp::getNowTime());

		// Cross-reference log index
		bool  hasIndex;
		const unsigned int index;
        // The logger name.
        const std::string loggerName;

        // Priority of logging event.
        const LogControlID level;

        // The name of thread in which this logging event was generated,
        //    e.g. the PID. 
        const std::string threadName;

        /** The number of seconds elapsed since the epoch 
            (1/1/1970 00:00:00 UTC) until logging event was created. */
        Timestamp timestamp;

		// The application supplied message of logging event.
		const std::string message;
		const char* format;
		va_list args;
    };
}

#endif // INCLUDED_LOGGING_LOG_RECORD_H
