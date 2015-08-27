#include "stdafx.h"

#include <cacti/logging/FileHandler.h>

#include <cacti/config/Portability.h>
#include <cacti/util/Timestamp.h>
#include <cacti/util/FileSystem.h>

#include <stdio.h>
#include <stdlib.h>

#define new DEBUG_NEW

namespace cacti
{
	FileHandler::FileHandler(PARAMMAP& param)
	{
		m_filename = param["filename"];

		if(param["flush"] == "false")
			m_immediateFlush = false;
		else
			m_immediateFlush = true;
		
		std::ios::openmode mode;
		if(param["mode"] == "truncate")
			mode = std::ios::trunc;
		else
			mode = std::ios::app;

		m_output.open(m_filename.c_str(), mode);
	}
	FileHandler::FileHandler(const std::string& filename,
		std::ios::openmode mode /*= std::ios::trunc*/,
		bool immediateFlush /*= true*/)

		: m_filename(filename)
		, m_immediateFlush(immediateFlush)
	{
		m_output.open(m_filename.c_str(), mode);
	}

	FileHandler::~FileHandler()
	{
		RecursiveMutex::ScopedLock lock(m_mutex);
		{
			m_output.close();
		}
	}

	void FileHandler::flush()
	{
		RecursiveMutex::ScopedLock lock(m_mutex);
		{
			m_output.flush();
		}
	}

	void FileHandler::doPublish(const LogRecord& rec)
	{
		RecursiveMutex::ScopedLock lock(m_mutex);
		{
			if(m_output.good())
			{
				m_formatter->format(m_output, rec);
				if(m_immediateFlush) 
				{
					m_output.flush();
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	namespace 
	{
		void rolloverFiles(const std::string& filename, int maxBackupIndex)
		{
			// Delete the oldest file
			std::ostringstream buffer;
			buffer << filename << '.' << maxBackupIndex;
			::remove(buffer.str().c_str());

			// rename {(maxBackupIndex - 1), ..., 2, 1} to {maxBackupIndex, ..., 3, 2}
			for(int i=maxBackupIndex - 1; i >= 1; i--) 
			{
				std::ostringstream source;
				std::ostringstream target;

				source << filename << '.' << i;
				target << filename << '.'<< (i+1);

				rename(source.str().c_str(), target.str().c_str());
			}
		} // end rolloverFiles()
	}

	//////////////////////////////////////////////////////////////////////////
	RollingFileHandler::RollingFileHandler(PARAMMAP& param)
		: FileHandler(param)
	{
		m_maxFileSize= atoi(param["maxsize"].c_str());
		if(m_maxFileSize<=0)
			m_maxFileSize = 100*1024*1024;

		m_maxBackupIndex = atoi(param["maxbackup"].c_str());
		if(m_maxBackupIndex<=0)
			m_maxBackupIndex = 20;

		rollover();
	}
	RollingFileHandler::RollingFileHandler(const std::string& filename, 
		long maxFileSize /* = 50*1024*1024 */,  
		int maxBackupIndex /* = 10 */,  
		bool immediateFlush /* = true */)

		: FileHandler(filename, std::ios::app, immediateFlush) 
	{
		m_maxFileSize = maxFileSize;
		m_maxBackupIndex = maxBackupIndex;
		
		rollover();
	}

	RollingFileHandler::~RollingFileHandler()
	{		
	}

	void RollingFileHandler::doPublish(const LogRecord& rec)
	{
		RecursiveMutex::ScopedLock lock(m_mutex);

		m_formatter->format(m_output, rec);
		if(m_immediateFlush) 
		{
			m_output.flush();
		}

		if(m_output.tellp() > m_maxFileSize) 
		{
			rollover();
		}
	}

	std::string RollingFileHandler::rollover()
	{
		// If m_maxBackupIndex <= 0, then there is no file renaming to be done.
		if(m_maxBackupIndex > 0) 
		{
			rolloverFiles(m_filename, m_maxBackupIndex);

			// Close the current file
			m_output.close();
			m_output.clear();	// reset flags since the C++ standard specified that all the
								// flags should remain unchanged on a close

			// Rename fileName to fileName.1
			std::string target(m_filename);
			target += ".1";

			rename(m_filename.c_str(), target.c_str());

			// Open a new file
			m_output.open(m_filename.c_str(), std::ios::trunc);

			// 
			return target;
		}
		else 
		{
			// Close the current file
			m_output.close();
			m_output.clear();	// reset flags since the C++ standard specified that all the
								// flags should remain unchanged on a close

			// Open it up again in truncation mode
			m_output.open(m_filename.c_str(), std::ios::out | std::ios::trunc);

			return "";
		}
	}

	//////////////////////////////////////////////////////////////////////////
	DailyRollingFileHandler::DailyRollingFileHandler(PARAMMAP& param)
		: FileHandler(param)
	{
		m_maxBackupIndex = atoi(param["maxbackup"].c_str());
		if(m_maxBackupIndex<=0)
			m_maxBackupIndex = 10;

		// @TODO
	}
	DailyRollingFileHandler::DailyRollingFileHandler(const std::string& filename, 
		DailyRollingFileSchedule schedule /* = DAILY */, 
		int maxBackupIndex /* = 10 */,
		bool immediateFlush /* = true */)

		: FileHandler(filename, std::ios::app, immediateFlush)
		, m_maxBackupIndex(maxBackupIndex)
		, m_schedule(schedule)
	{
		Timestamp now = Timestamp::getNowTime();

		now.setMicroSeconds(0);
		struct tm time;
		now.localtime(time);

		time.tm_sec = 0;
		switch(schedule) 
		{
		case MONTHLY:
			time.tm_mday = 1;
			time.tm_hour = 0;
			time.tm_min = 0;
			break;

		case WEEKLY:
			time.tm_mday -= (time.tm_wday % 7);
			time.tm_hour = 0;
			time.tm_min = 0;
			break;

		case DAILY:
			time.tm_hour = 0;
			time.tm_min = 0;
			break;

		case TWICE_DAILY:
			if(time.tm_hour >= 12) 
			{
				time.tm_hour = 12;
			}
			else 
			{
				time.tm_hour = 0;
			}
			time.tm_min = 0;
			break;

		case HOURLY:
			time.tm_min = 0;
			break;

		case MINUTELY:
			break;
		};
		now.setTime(time);

		m_scheduledFilename = getFilename(now);
		m_nextRolloverTime = calculateNextRolloverTime(now);
	}

	DailyRollingFileHandler::~DailyRollingFileHandler()
	{
		rollover();
	}

	void DailyRollingFileHandler::doPublish(const LogRecord& rec)
	{
		RecursiveMutex::ScopedLock lock(m_mutex);

		if(rec.timestamp >= m_nextRolloverTime) 
		{
			rollover();
		}

		m_formatter->format(m_output, rec);
		if(m_immediateFlush) 
		{
			m_output.flush();
		}
	}

	std::string DailyRollingFileHandler::rollover()
	{
		// Close the current file
		m_output.close();
		m_output.clear();	// reset flags since the C++ standard specified that all the
						// flags should remain unchanged on a close

		// If we've already rolled over this time period, we'll make sure that we
		// don't overwrite any of those previous files.
		rolloverFiles(m_scheduledFilename, m_maxBackupIndex);
		std::ostringstream backupTarget;
		backupTarget << m_scheduledFilename << ('.') << 1;
		rename(m_scheduledFilename.c_str(), backupTarget.str().c_str());

		// Rename filename to scheduledFilename
		rename(m_filename.c_str(), m_scheduledFilename.c_str());

		// Open a new file
		m_output.open(m_filename.c_str(), std::ios::trunc);

		std::string rt = m_scheduledFilename;
		// Calculate the next rollover time
		if(Timestamp::getNowTime() >= m_nextRolloverTime) 
		{
			m_scheduledFilename = getFilename(m_nextRolloverTime);
			m_nextRolloverTime = calculateNextRolloverTime(m_nextRolloverTime);
		}
		return rt;
	}

	Timestamp DailyRollingFileHandler::calculateNextRolloverTime(const Timestamp& t) const
	{
		switch(m_schedule) 
		{
		case MONTHLY: 
			{
				struct tm nextMonthTime;
				t.localtime(nextMonthTime);
				nextMonthTime.tm_mon += 1;
				nextMonthTime.tm_isdst = 0;

				Timestamp ret;
				if(ret.setTime(nextMonthTime) == -1) 
				{
					ret = (t + Timestamp(2678400));
				}

				return ret;
			}

		case WEEKLY:
			return (t + Timestamp(604800)); // 7 * 24 * 60 * 60 seconds

		case DAILY:
			return (t + Timestamp(86400)); //      24 * 60 * 60 seconds

		case TWICE_DAILY:
			return (t + Timestamp(43200)); //      12 * 60 * 60 seconds

		case HOURLY:
			return (t + Timestamp(3600));  //           60 * 60 seconds

		case MINUTELY:
			return (t + Timestamp(60));    //                60 seconds
		};
		return (t + Timestamp(86400));
	}

	std::string DailyRollingFileHandler::getFilename(const Timestamp& t) const
	{
		std::string pattern;
		switch(m_schedule) 
		{
		case MONTHLY:
			pattern = ("%Y-%m");
			break;

		case WEEKLY:
			pattern = ("%Y-%W");
			break;

		case DAILY:
			pattern = ("%Y-%m-%d");
			break;

		case TWICE_DAILY:
			pattern = ("%Y-%m-%d-%p");
			break;

		case HOURLY:
			pattern = ("%Y-%m-%d-%H");
			break;

		case MINUTELY:
			pattern = ("%Y-%m-%d-%H-%M");
			break;
		};

		return m_filename + (".") + t.getFormattedTime(pattern.c_str(), false);
	}

	//////////////////////////////////////////////////////////////////////////
	StarFileHandler::StarFileHandler(PARAMMAP& param)
		: FileHandler(param)
	{
		m_maxFileSize= atoi(param["maxsize"].c_str());
		if(m_maxFileSize<=0)
			m_maxFileSize = 100*1024*1024;

		m_maxBackupIndex = atoi(param["maxbackup"].c_str());
		if(m_maxBackupIndex<=0)
			m_maxBackupIndex = 20;
		
		init();
	}
	StarFileHandler::StarFileHandler(
		const std::string& filename,
		long maxFileSize /*= 50*1024*1024*/,		// 10 MB
		int maxBackupIndex /*= 10*/,
		bool immediateFlush /*= true*/)

		: FileHandler(filename, std::ios::app, immediateFlush)
		, m_maxBackupIndex(maxBackupIndex)
		, m_maxFileSize(maxFileSize)
	{
		init();
	}

	StarFileHandler::~StarFileHandler()
	{
	}
	
	void StarFileHandler::init()
	{
		Timestamp now = Timestamp::getNowTime();

		now.setMicroSeconds(0);
		struct tm time;
		now.localtime(time);

		time.tm_sec = 0;
		time.tm_hour = 0;
		time.tm_min = 0;

		m_scheduledFilename = getFilename(now);
		m_nextRolloverTime = calculateNextRolloverTime(now);

		// if current file has content means it's an old file, we roll it at this time
		if(FileSystem::fileSize(m_filename.c_str()) > 0)
			rollover();
	}
	void StarFileHandler::setMaxFileSize(long newFileSize) 
	{
		m_maxFileSize = newFileSize;
	}

	void StarFileHandler::doPublish(const LogRecord& rec)
	{
		RecursiveMutex::ScopedLock lock(m_mutex);

		if(rec.timestamp >= m_nextRolloverTime) 
		{
			rollover();
		}

		m_formatter->format(m_output, rec);
		if(m_immediateFlush) 
		{
			m_output.flush();
		}

		if(m_output.tellp() > m_maxFileSize)
		{
			rollover();
		}
	}

	std::string StarFileHandler::rollover()
	{
		m_output << cacti::Timestamp::getNowTime().getFormattedTime("%m%d %H:%M:%S ") 
			<< GetTickCount()%1000 << " <StarFileHandler> file roll!" << std::endl;

		// Close the current file
		m_output.close();
		m_output.clear();	// reset flags since the C++ standard specified that all the
		// flags should remain unchanged on a close

		// If we've already rolled over this time period, we'll make sure that we
		// don't overwrite any of those previous files.
		rolloverFiles(m_scheduledFilename, m_maxBackupIndex);
		std::ostringstream backupTarget;
		std::ostringstream alwaysTarget;
		backupTarget << m_scheduledFilename << ('.') << 1;
		alwaysTarget << m_filename << ("_bak");
//		rename(m_scheduledFilename.c_str(), backupTarget.str().c_str());
		rename(alwaysTarget.str().c_str(), backupTarget.str().c_str());

		// Rename filename to scheduledFilename
//		rename(m_filename.c_str(), m_scheduledFilename.c_str());
		rename(m_filename.c_str(), alwaysTarget.str().c_str());

		// Open a new file
		m_output.open(m_filename.c_str(), std::ios::trunc);

		m_output << cacti::Timestamp::getNowTime().getFormattedTime("%m%d %H:%M:%S ") 
			<< GetTickCount()%1000 <<" <StarFileHandler> new roll file!" << std::endl;

		std::string rt = m_scheduledFilename;
		// Calculate the next rollover time
		if(Timestamp::getNowTime() >= m_nextRolloverTime) 
		{
			m_scheduledFilename = getFilename(m_nextRolloverTime);
			m_nextRolloverTime = calculateNextRolloverTime(m_nextRolloverTime);
		}
		return rt;
	}
}
