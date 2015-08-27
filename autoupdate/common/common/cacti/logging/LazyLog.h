#ifndef INCLUDED_LAZY_LOG_H
#define INCLUDED_LAZY_LOG_H

#include "cacti/logging/Logger.h"
#include "cacti/logging/FileHandler.h"
#include "cacti/logging/Formatter.h"

namespace cacti
{
	class LazyLog
	{
	public:
		LazyLog(const char *module, const char *version = "");
		virtual ~LazyLog();

		void open();
		void close();
		void flush();
		void setLevel(LogControlID level);
		void setImmediateFlush(bool flag);
		void setMaxSize(long newSize);
		void vprint(unsigned int ID, const char *format, va_list argptr);
		void vprint(const char *format, va_list argptr);
		void print(const char *format,...);
		void trace(LogControlID level, const char *format,...);
		void print(unsigned int ID, const char *format,...);
		void printnt(const char *format,...);
		void tracent(LogControlID level, const char *format,...);
		void printb(const char *title, const unsigned char *buf, int len);
		void traceb(LogControlID level, const char *title, const unsigned char *buf,int len);

		void printb(unsigned int ID, const char *title, const unsigned char *buf, int len);
		void traceb(unsigned int ID, LogControlID level, const char *title, const unsigned char *buf,int len);

		void printnb(const unsigned char *buf,int len);
		void tracenb(LogControlID level, const unsigned char *buf,int len);

		void perror(const char *format,...);
		Logger& getLogger();
	private:
		//∑¿÷π¥ÌŒÛ”√∑®£∫printnt(0, "%d", 1234);
		void printnt(int , const char *format,...);
		void perror(int , const char *format,...);

		char* convert(char* str);

	private:
		std::string m_module;
		std::string m_version;
		Logger* m_logImpl;
		StarFileHandler* m_handler;
		LogHandlerPtr m_handlerHolder;

		bool m_enabled;
	}; 
}

#endif // INCLUDED_LAZY_LOG_H
