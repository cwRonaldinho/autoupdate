#ifndef INCLUDED_LOGGING_LOG_FILE_HANDLER_H
#define INCLUDED_LOGGING_LOG_FILE_HANDLER_H

#include <cacti/logging/LogHandler.h>

#include <fstream>

namespace cacti 
{
	class FileHandler : public LogHandler
	{
	public:
		FileHandler(PARAMMAP& param);
		FileHandler(
			const std::string& filename,
			std::ios::openmode mode = std::ios::trunc,
			bool immediateFlush = true);
		virtual ~FileHandler();

		void setImmediateFlush(bool flag) { m_immediateFlush = flag; }
		void flush();

	protected:
		void open();
		// Log in ConsoleHandler specific way.
		// @param event  The LogRecord to log.
		virtual void doPublish(const LogRecord& rec);

	protected:
		std::string m_filename;
		std::ofstream m_output;
		bool m_immediateFlush;
		RecursiveMutex m_mutex;

	private:
		// Disallow copying of instances of this class
		FileHandler(const FileHandler&);
		FileHandler& operator=(const FileHandler&);
	};

	//////////////////////////////////////////////////////////////////////////	
	/**
	* RollingFileAppender extends FileAppender to backup the log files when 
	* they reach a certain size. 
	*/
	class RollingFileHandler : public FileHandler
	{
	public:
		RollingFileHandler(PARAMMAP& param);
		RollingFileHandler(
			const std::string& filename,
			long maxFileSize = 100*1024*1024,		// 50 MB
			int maxBackupIndex = 20,					// one backup
			bool immediateFlush = true);

		~RollingFileHandler();

		void setMaxFileSize(long newFileSize) 
		{
			m_maxFileSize = newFileSize;
		}

	protected:
		// Log in ConsoleHandler specific way.
		// @param event  The LogRecord to log.
		virtual void doPublish(const LogRecord& rec);
		virtual std::string rollover();

	protected:
		int m_maxFileSize;
		int m_maxBackupIndex;
	};

	//////////////////////////////////////////////////////////////////////////
	enum DailyRollingFileSchedule { MONTHLY, WEEKLY, DAILY,
		TWICE_DAILY, HOURLY, MINUTELY};

	/**
	* DailyRollingFileAppender extends {@link FileAppender} so that the
	* underlying file is rolled over at a user chosen frequency.
	*
	* <p>
	*         
	*/
	class DailyRollingFileHandler : public FileHandler
	{
	public:
		DailyRollingFileHandler(PARAMMAP& param);
		DailyRollingFileHandler(
			const std::string& filename,
			DailyRollingFileSchedule schedule = DAILY,
			int maxBackupIndex = 10,
			bool immediateFlush = true);
		
		~DailyRollingFileHandler();

	protected:
		virtual void doPublish(const LogRecord& rec);
		virtual std::string rollover();
		Timestamp calculateNextRolloverTime(const Timestamp& t) const;
		std::string getFilename(const Timestamp& t) const;

	protected:
		DailyRollingFileSchedule m_schedule;
		std::string m_scheduledFilename;
		Timestamp m_nextRolloverTime;
		int m_maxBackupIndex;
	};

	class StarFileHandler : public FileHandler
	{
	public:
		StarFileHandler(PARAMMAP& param);
		StarFileHandler(
			const std::string& filename,
			long maxFileSize = 100*1024*1024,		// 50 MB
			int maxBackupIndex = 20,
			bool immediateFlush = true);
		~StarFileHandler();
		void init();
		void setMaxFileSize(long newFileSize);
		void doPublish(const LogRecord& rec);
	protected:
		std::string rollover();
		Timestamp calculateNextRolloverTime(const Timestamp& t) const;
		std::string getFilename(const Timestamp& t) const;
	protected:
		cacti::Timestamp m_nextRolloverTime;
		std::string m_scheduledFilename;
		int m_maxBackupIndex;
		long m_maxFileSize;
	};
}

#endif // INCLUDED_LOGGING_LOG_FILE_HANDLER_H
