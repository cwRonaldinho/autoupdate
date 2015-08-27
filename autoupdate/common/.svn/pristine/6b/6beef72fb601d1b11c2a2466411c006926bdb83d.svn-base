#ifndef INCLUDED_CACTI_UTIL_TIMESTAMP_H
#define INCLUDED_CACTI_UTIL_TIMESTAMP_H

#include <time.h>
#include <string>

namespace cacti
{
	class Timestamp 
	{
	public:

		static const int FORMAT_BUFFER_SIZE			= 40;
		static const int ONE_SEC_IN_MILLISEC		= 1000;
		static const int ONE_SEC_IN_MICROSEC		= 1000000;	
		static const int ONE_SEC_IN_NANOSEC			= 1000000000;
		static const int ONE_MILLISEC_IN_MICROSEC	= 1000;
		static const int ONE_MICROSEC_IN_NANOSEC    = 1000;
		static const int INFINITE_TIME				= ~0;

	public:
		Timestamp();
		Timestamp(time_t sec);
		Timestamp(time_t sec, time_t microseconds);
		Timestamp(const Timestamp& other);
		static Timestamp getNowTime();
		static time_t strToTime(std::string& strTime,const char* format);

		time_t getSeconds() const { return m_seconds; }
		void   setSeconds(time_t s) { m_seconds = s;	}

		time_t getAsMilliseconds() const;

		time_t getMicroSeconds() const	{ return m_microseconds;}
		void   setMicroSeconds(time_t us)	{ m_microseconds = us;	}

		time_t setTime(struct tm& t);
		time_t getTime() const;

		void gmtime(struct tm& t) const;
		void localtime(struct tm& t) const;

#ifndef WIN32
		void toTimespec(timespec &ts) const;
		void toTimespecDuration(timespec &ts) const;
#endif
		/**
		* Returns a string with a "formatted time" specified by
		* <code>fmt</code>.  It used the <code>strftime()</code>
		* function to do this.  
		* <p>
		* Look at your platform's <code>strftime()</code> documentation
		* for the formatting options available.
		* <p>
		*/
		std::string getFormattedTime(const char* fmt, bool useGM = false) const;
		// Operators
		Timestamp& operator+=(const Timestamp& rhs);
		Timestamp& operator-=(const Timestamp& rhs);
		Timestamp& operator/=(time_t rhs);
		Timestamp& operator*=(time_t rhs);
		bool operator==(const Timestamp& rhs)
		{ 
			return  m_seconds == rhs.m_seconds &&
					m_microseconds == rhs.m_microseconds; 
		}
		bool operator!=(const Timestamp& rhs) 
		{ 
			return !(*this == rhs); 
		}

	private:
		time_t m_seconds;		// √Î
		time_t m_microseconds;	// Œ¢√Î
	};

	const Timestamp operator+(const Timestamp& lhs, const Timestamp& rhs);
	const Timestamp operator-(const Timestamp& lhs, const Timestamp& rhs);
	const Timestamp operator/(const Timestamp& lhs, time_t rhs);
	const Timestamp operator*(const Timestamp& lhs, time_t rhs);

	bool operator< (const Timestamp& lhs, const Timestamp& rhs);
	bool operator<=(const Timestamp& lhs, const Timestamp& rhs);

	bool operator> (const Timestamp& lhs, const Timestamp& rhs);
	bool operator>=(const Timestamp& lhs, const Timestamp& rhs);

	bool operator==(const Timestamp& lhs, const Timestamp& rhs);
	bool operator!=(const Timestamp& lhs, const Timestamp& rhs);
}

#endif // INCLUDED_CACTI_UTIL_TIMESTAMP_H
