#ifndef INCLUDED_CONFIGURELOADER_H
#define INCLUDED_CONFIGURELOADER_H

#include "LogHandler.h"
#include "Logger.h"

#include <string>
#include <map>

namespace cacti
{
	class ConfigureLoader
	{
	public:
		static bool loadXML(const char* xml);
		static bool updateLevel(const char* xml);

	private:
		static LogHandlerPtr createHandler(const char* bystr, PARAMMAP& params);
		static FormatterPtr createFormatter(const char* bystr, PARAMMAP& params);

		static LogControlID createLevel(const char* bystr);
	};
}

#endif // INCLUDED_CONFIGURELOADER_H
