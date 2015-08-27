#ifndef INCLUDED_LOGGING_NULL_HANDLER_H
#define INCLUDED_LOGGING_NULL_HANDLER_H

#include <cacti/logging/LogRecord.h>

namespace cacti 
{
	class NullLogHandler 
	{
	protected:
		virtual void doPublish(const LogRecord& rec){}
	};
}

#endif // INCLUDED_LOGGING_NULL_HANDLER_H
