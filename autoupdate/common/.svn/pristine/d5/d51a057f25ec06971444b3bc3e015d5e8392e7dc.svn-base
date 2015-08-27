#ifndef INCLUDED_LOGGER_H
#define INCLUDED_LOGGER_H

#include <cacti/logging/LogLevel.h>
#include <cacti/logging/LoggerStream.h>
#include <cacti/logging/LogHandler.h>
#include <boost/format.hpp>
#include <cacti/util/NonCopyable.h>
#include <cacti/kernel/SysObj.h>

#include <boost/shared_ptr.hpp>

#include <list>
#include <vector>
#include <stdexcept>

#undef ERR

namespace cacti 
{
	class Logger;
	typedef boost::shared_ptr<Logger> LoggerPtr;

	class Logger
	{
		friend class LogManager;

	private:
		// Disallow copying of instances of this class
		Logger(const Logger&);
		Logger& operator=(const Logger&);

	public:
		virtual ~Logger();

		static Logger& getRoot();

		static void setRootLevel(const LogControlID& level);

		static LogControlID getRootLevel() throw();

		static Logger& getInstance(const std::string& name);
		
		// 
		// shutdown all loggers, reset the system to initialize status
		static void shutdown();

		const std::string& getName() const throw(); 

		void setLevel(const LogControlID& level) ;

		LogControlID getLevel() const throw();

		// 
		// Starting from this Logger, search the logger hierarchy for a
		// set level and return it. Otherwise, return the level 
		// of the root logger.
		//
		// <p>The Logger class is designed so that this method executes as
		// quickly as possible.
		LogControlID getChainedLevel() const throw();

		bool isLevelEnabled(const LogControlID& level) const throw();

		//
		// Adds an LogHandler to this Logger.
		// This method passes ownership from the caller to the Logger.
		void addHandler(LogHandlerPtr handler);

		std::list<LogHandlerPtr> getAllHandlers();
		LogHandlerPtr getHandler(const std::string& name);

		void removeAllHandlers();
		void removeHandler(LogHandlerPtr handler);		
		// Remove handler by handler's name,
		// The handler must be call setName to set the handler's name
		void removeHandler(const std::string& name);
		//
		// Call the appenders in the hierarchy starting at
		//  <code>this</code>.  If no appenders could be found, emit a
		// warning.
		// 
		// <p>This method always calls all the appenders inherited form the
		// hierarchy circumventing any evaluation of whether to log or not to
		// log the particular log request.
		virtual void callHandlers(const LogRecord& record) throw();

		/**
		* Set the additivity flag for this Category instance.
		**/
		void setAdditivity(bool additivity);

		/**
		* Returns the additivity flag for this Category instance.
		**/        
		bool getAdditivity() const throw();

		LoggerPtr getParent() throw();

		const LoggerPtr getParent() const throw();

		/** 
		* Log a message with the specified level.
		* @param level The level of this log message.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void log(const LogControlID& level, const char* stringFormat, ...) 
			throw();

		void log(const LogControlID& level, unsigned int index, 
			const char* stringFormat, ...) 
			throw();

		/** 
		* Log a message with the specified level.
		* @param level The level of this log message.
		* @param bf boost format object, 
		*  just like: format("test%04d,%s,%X\n")%1%2%3;
		*  
		**/
		void log(const LogControlID& level, const boost::format& bf) throw();
		void log(const LogControlID& level, unsigned int index, const boost::format& bf) throw();

		/** 
		* Log a message with the specified level.
		* @param level The level of this log message.
		* @param stringFormat Format specifier for the string to write
		* in the log file.
		* @param va The arguments for stringFormat.
		**/  
		void logva(const LogControlID& level, 
			const char* stringFormat,
			va_list va) throw();

		void logva(const LogControlID& level, 
			unsigned int index,
			const char* stringFormat,
			va_list va) throw();
		void logvaNoTime(const LogControlID& level,
			const char* stringFormat,
			va_list va) throw();
		/** 
		* Log a message with the specified level.
		* @param level The level of this log message.
		* @param message string to write in the log file
		**/  
		void log(const LogControlID& level, 
			const std::string& message) throw();

		void log(const LogControlID& level, 
			unsigned int index, 
			const std::string& message) throw();
		void logNoTime(const LogControlID& level, 
			const std::string& message) throw();
		/** 
		* Log a message with debug level.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void debug(const char* stringFormat, ...) throw();

		/** 
		* Log a message with debug level.
		* @param message string to write in the log file
		**/  
		void debug(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level DEBUG.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isDebugEnabled() const throw() { 
			return isLevelEnabled(LogLevel::DBG);
		};

		/**
		* Return a LoggerStream with level DEBUG.
		* @returns The LoggerStream.
		**/
		inline LoggerStream debugStream() {
			return getStream(LogLevel::DBG);
		}

		/** 
		* Log a message with info level.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void info(const char* stringFormat, ...) throw();

		/** 
		* Log a message with info level.
		* @param message string to write in the log file
		**/  
		void info(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level INFO.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isInfoEnabled() const throw() { 
			return isLevelEnabled(LogLevel::INFO);
		};

		/**
		* Return a LoggerStream with level INFO.
		* @returns The LoggerStream.
		**/
		inline LoggerStream infoStream() {
			return getStream(LogLevel::INFO);
		}

		/** 
		* Log a message with warn level.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void warn(const char* stringFormat, ...) throw();

		/** 
		* Log a message with warn level.
		* @param message string to write in the log file
		**/  
		void warn(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level WARN.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isWarnEnabled() const throw() { 
			return isLevelEnabled(LogLevel::WARN);
		};

		/**
		* Return a LoggerStream with level WARN.
		* @returns The LoggerStream.
		**/
		inline LoggerStream warnStream() {
			return getStream(LogLevel::WARN);
		};

		/** 
		* Log a message with error level.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void error(const char* stringFormat, ...) throw();

		/** 
		* Log a message with error level.
		* @param message string to write in the log file
		**/  
		void error(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level ERROR.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isErrorEnabled() const throw() { 
			return isLevelEnabled(LogLevel::ERR);
		};

		/**
		* Return a LoggerStream with level ERROR.
		* @returns The LoggerStream.
		**/
		inline LoggerStream errorStream() {
			return getStream(LogLevel::ERR);
		};

		/** 
		* Log a message with alert level.
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void alert(const char* stringFormat, ...) throw();

		/** 
		* Log a message with alert level.
		* @param message string to write in the log file
		**/  
		void alert(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level ALERT.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isAlertEnabled() const throw() { 
			return isLevelEnabled(LogLevel::ALERT);
		};

		/**
		* Return a LoggerStream with level ALERT.
		* @returns The LoggerStream.
		**/
		inline LoggerStream alertStream() throw() {
			return getStream(LogLevel::ALERT);
		};

		/** 
		* Log a message with fatal level. 
		* @param stringFormat Format specifier for the string to write 
		* in the log file.
		* @param ... The arguments for stringFormat 
		**/  
		void fatal(const char* stringFormat, ...) throw();

		/** 
		* Log a message with fatal level.
		* @param message string to write in the log file
		**/  
		void fatal(const std::string& message) throw();

		/**
		* Return true if the Logger will log messages with level FATAL.
		* @returns Whether the Logger will log.
		**/ 
		inline bool isFatalEnabled() const throw() { 
			return isLevelEnabled(LogLevel::FATAL);
		};

		/**
		* Return a LoggerStream with level FATAL.
		* @returns The LoggerStream.
		**/
		inline LoggerStream fatalStream() {
			return getStream(LogLevel::FATAL);
		};

		/**
		* Return a LoggerStream with given Priority.
		* @param level The Priority of the LoggerStream.
		* @returns The requested LoggerStream.
		**/
		virtual LoggerStream getStream(const LogControlID& level);

	protected:
		//
		// Constructor 
		// @param name the fully qualified name of this Logger
		// @param parent the parent of this parent, or 0 for the root 
		// Logger
		// @param level the level for this Logger. Defaults to
		// LogLevel::NOTSET
		Logger(const std::string& name, LoggerPtr parent, 
			const LogControlID& level = LogLevel::NOTSET);


		virtual void logUnconditionally(const LogControlID& level, 
			const char* format, va_list args) throw();

		virtual void logUnconditionally(const LogControlID& level, 
			unsigned int index,
			const char* format, va_list args) throw();
		virtual void logUnconditionallyNoTime(const LogControlID& level, 
			const char* format, va_list args) throw();	

private:
		 // The name of this logger.
		const std::string m_name;

		// The parent of this logger. All loggers have al least one
		// ancestor which is the root logger. 
		LoggerPtr m_parent;

		/**
		*  The assigned level of this logger. 
		**/
		LogControlID m_level;

		std::list<LogHandlerPtr> m_handlers;
		mutable RecursiveMutex m_handlerSetMutex;

		/** 
		* Additivity is set to true by default, i.e. a child inherits its
		* ancestor's appenders by default. 
		*/
		bool m_isAdditive;
	};

}
#endif // INCLUDED_LOGGER_H
