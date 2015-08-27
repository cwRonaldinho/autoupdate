#include "stdafx.h"

#include <cacti/logging/Formatter.h>
#include <cacti/util/StringUtil.h>
#include <cacti/config/Portability.h>

#include <iomanip>

using namespace std;

#define new DEBUG_NEW

namespace cacti 
{
	void SimpleFormatter::format(std::ostream& stream, 
								 const LogRecord& record)
	{
		if(record.format)
			stream << cacti::StringUtil::vform(record.format, record.args);
		else
			stream << record.message;
	}

	TTICFormatter::TTICFormatter(PARAMMAP& param)
	{
		m_logIndex = (param["index"] == "true") ? true : false;
		m_logTime  = (param["time"] == "false") ? false : true;
		m_logThread = (param["thread"] == "true") ? true : false;
		m_logName = (param["name"] == "true") ? true : false;
		m_logLevel = (param["level"] == "false") ? false : true;
		m_autoNewLine = (param["newline"] == "true") ? true : false;
	}

	void TTICFormatter::format(std::ostream& stream, 
							   const LogRecord& rec)
	{
		char oldfill = stream.fill();
		stream.fill('0');

		if(logTime() && rec.timestamp.getAsMilliseconds() != 0)
		{
			// format the timestamp
			formatTimestamp(stream, rec.timestamp);
		}

		if(logThread())
		{
			stream << " <"
				   << rec.threadName
				   << ">";
		}

		if(logIndex())
		{
			// format the index
			stream << " ["
				   << setw(8) << hex << rec.index << dec
				   << "]";
		}

		if(logName())
		{
			stream << " {"
				   << rec.loggerName
				   << "}";
		}

		if(logLevel())
		{
			stream << " ("
				   << rec.level.getLabel()
				   << ")";
		}

		//////
		if(rec.format)
		{
			stream << " "
				<< StringUtil::vform(rec.format, rec.args);
		}
		else
		{
			stream << rec.message;
		}

		if(autoNewLine())
			stream << endl;

		stream.fill(oldfill);
	}

	void TTICFormatter::formatTimestamp(std::ostream& stream, const Timestamp& time)
	{		
		struct tm tmTime;
		time.localtime(tmTime);

		stream << setw(2) << tmTime.tm_mon+1
			   << setw(2) << tmTime.tm_mday
			   << " "
		       << setw(2) << tmTime.tm_hour << ":"
			   << setw(2) << tmTime.tm_min << ":"
			   << setw(2) << tmTime.tm_sec;
	}

	//////////////////////////////////////////////////////////////////////////
	StarFormatter::StarFormatter(PARAMMAP& param)
		: TTICFormatter(param)
	{
	}
	void StarFormatter::format(std::ostream& stream, const cacti::LogRecord& rec)
	{		
		std::string timestr = "";
		if(rec.timestamp.getAsMilliseconds() != 0)
			timestr = rec.timestamp.getFormattedTime("%m%d %H:%M:%S ");

		int index = 0;
		m_buffer[index] = '\0';
		if(!timestr.empty())
		{
			if(rec.hasIndex)
			{
				index = snprintf(m_buffer, sizeof(m_buffer)-1, "%s%3d [%X] ", timestr.c_str(),GetTickCount()%1000, rec.index);
			}
			else
			{
				index = snprintf(m_buffer, sizeof(m_buffer)-1, "%s%3d ", timestr.c_str(),GetTickCount()%1000);
			}

			if(index >= 0)
			{
				if(rec.format)
					index = vsnprintf(m_buffer+index, sizeof(m_buffer)-index-1, rec.format, rec.args);
				else
					index = snprintf(m_buffer+index, sizeof(m_buffer)-index-1, "%s", rec.message.c_str());
			}
		}
		else
		{
			if(rec.hasIndex)
			{
				index = snprintf(m_buffer, sizeof(m_buffer)-1, "[%X] ", rec.index);
			}

			if(index >= 0)
			{
				if(rec.format)
					index = vsnprintf(m_buffer+index, sizeof(m_buffer)-index-1, rec.format, rec.args);
				else
					index = snprintf(m_buffer+index, sizeof(m_buffer)-index-1, "%s", rec.message.c_str());
			}
		}

		stream << m_buffer;
	}
}

