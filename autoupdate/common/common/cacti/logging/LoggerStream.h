#ifndef INCLUDED_LOGGERSTREAM_H
#define INCLUDED_LOGGERSTREAM_H

#include <cacti/config/Portability.h>
#include <cacti/logging/LogLevel.h>

namespace cacti
{
	class Logger;

	// 
	// This class enables streaming simple types and objects to a logger.
	// Use logger.errorStream(), etc. to obtain a LoggerStream class.
	class LoggerStream {
	public:

		//
		// Enumeration of special 'Separators'. Currently only contains the
		// 'ENDLINE' separator, which separates two log messages.
		typedef enum {
			ENDLINE
		} Separator;

		LoggerStream(Logger& logger, const LogControlID& level);

		~LoggerStream();

		inline Logger& getLogger() const { return m_logger; };

		inline LogControlID getLevel() const throw() { 
			return m_level; 
		};

		//
		// Streams in a Separator. If the separator equals 
		// LoggerStream::ENDLINE it sends the contents of the stream buffer
		// to the Logger with set level and empties the buffer.
		// @param separator The Separator
		// @returns A reference to itself.
		LoggerStream& operator<<(Separator separator);

		///
		// Flush the contents of the stream buffer to the Logger and
		// empties the buffer.
		void flush();

		//
		// Stream in arbitrary types and objects.  
		// @param t The value or object to stream in.
		// @returns A reference to itself.
		template<typename T> LoggerStream& operator<<(const T& t) 
		{
			if (getLevel() != LogLevel::NOTSET) 
			{
				if (!m_buffer) {
					if (!(m_buffer = new std::ostringstream)) {
						// XXX help help help
					}
				}
				(*m_buffer) << t;
			}
			return *this;
		}

	private:
		Logger& m_logger;
		const LogControlID& m_level;
		std::ostringstream* m_buffer;
	};

}
#endif // INCLUDED_LOGGERSTREAM_H
