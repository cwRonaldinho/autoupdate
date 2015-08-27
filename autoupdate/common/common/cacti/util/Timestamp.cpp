#include "stdafx.h"

#include <cacti/util/Timestamp.h>

#include <string>
#include <assert.h>

#include <sys/timeb.h>
#ifndef WIN32
#include <sys/time.h>
#endif

#define new DEBUG_NEW

using namespace std;

namespace cacti
{
	Timestamp::Timestamp()
		: m_seconds(0)
		, m_microseconds(0)
	{
	}

	Timestamp::Timestamp(time_t seconds, time_t microseconds)
		: m_seconds(seconds)
		, m_microseconds(microseconds)
	{
	}

	Timestamp::Timestamp(time_t time)
		: m_seconds(time)
		, m_microseconds(0)
	{
	}

	Timestamp::Timestamp(const Timestamp& other)
		: m_seconds(other.m_seconds)
		, m_microseconds(other.m_microseconds)
	{

	}
	time_t Timestamp::strToTime(string& strTime,const char* format)
	{
		if(!format)
			return 0;

		size_t pos=0,gap = 0;
		tm time_tm;
		Timestamp tt = Timestamp::getNowTime();
		tt.localtime(time_tm);
		string  formatStr = format;

		for (  size_t pos_format = 0; pos_format != string::npos ; pos_format ++)
		{
			pos_format = formatStr.find_first_of("%",pos_format);
			if(string::npos == pos_format )
				break;

			if( pos_format+1 >= formatStr.length() ) 
				break;
			pos = pos_format + gap;
			char formatChar = formatStr.at(pos_format+1);

			if( 'Y' == formatChar ) 
			{
				 if (pos+3 > strTime.length())
					 break;
			}
			else 
			{
				if( pos+1 > strTime.length() )
					break;
			}
			switch( formatChar) 
			{
			case 'Y':
				time_tm.tm_year = atoi(strTime.substr(pos,4).c_str())-1900;
				gap += 2;
				break;
			case 'm':
				time_tm.tm_mon = atoi(strTime.substr(pos,2).c_str())-1;
				break;
			case 'd':
				time_tm.tm_mday = atoi(strTime.substr(pos,2).c_str());
				break;
			case 'H':
				time_tm.tm_hour = atoi(strTime.substr(pos,2).c_str());
				break;
			case 'M':
				time_tm.tm_min = atoi(strTime.substr(pos,2).c_str());
				break;
			case 'S':
				time_tm.tm_sec = atoi(strTime.substr(pos,2).c_str());
				break;
			default:
				break;
			}		
		}
		time_t t = mktime(&time_tm);					
		return t;
	}

	Timestamp Timestamp::getNowTime()
	{
#ifndef WIN32
		struct timeval tv;
		::gettimeofday(&tv, 0);
		return Timestamp(tv.tv_sec, tv.tv_usec);
#else
		struct timeb tb;
		::ftime(&tb);
		return Timestamp(tb.time, 1000 * tb.millitm);
#endif
		// low resolution
		// return Timestamp(time(0));
	}

	time_t Timestamp::getAsMilliseconds() const
	{
		return m_seconds*ONE_SEC_IN_MILLISEC + m_microseconds/ONE_MILLISEC_IN_MICROSEC;
	}

	//////////////////////////////////////////////////////////////////////////////
	time_t Timestamp::setTime(struct tm& t)
	{
		time_t time = ::mktime(&t);
		if(time != -1) 
		{
			m_seconds = time;
		}

		return time;
	}

	time_t Timestamp::getTime() const
	{
		return m_seconds;
	}

	void Timestamp::gmtime(struct tm& t) const
	{
		time_t clock = m_seconds;
#ifdef WIN32
		struct tm* tmp = ::gmtime(&clock);
		if(tmp)
			t = *tmp;
#else
		::gmtime_r(&clock, &t);
#endif
	}

	void Timestamp::localtime(struct tm& t) const
	{
		time_t clock = m_seconds;
#ifdef WIN32
		struct tm* tmp = ::localtime(&clock);
		if(tmp)
			t = *tmp;
#else
		::localtime_r(&clock, &t);
#endif
	}

#ifndef WIN32 
	void Timestamp::toTimespec(timespec &ts) const
	{
		Timestamp now = getNowTime();
		
		ts.tv_sec  = now.m_seconds + m_seconds;
		ts.tv_nsec = (now.m_microseconds + m_microseconds) * 1000;

		if(ts.tv_nsec > ONE_SEC_IN_NANOSEC) 
		{
			++ts.tv_sec;
			ts.tv_nsec -= ONE_SEC_IN_NANOSEC;
		}
	}

	void Timestamp::toTimespecDuration(timespec &ts) const
	{
		ts.tv_sec = m_seconds;
		ts.tv_nsec = m_microseconds * ONE_MICROSEC_IN_NANOSEC;
	}
#endif

	std::string Timestamp::getFormattedTime(const char* fmt, 
		bool useGM /*= false*/) const
	{
		assert(fmt);

		char buffer[FORMAT_BUFFER_SIZE];
		struct tm tmTime;

		if(useGM) 
		{
			gmtime(tmTime);
		}
		else 
		{
			localtime(tmTime);
		}

		size_t len = ::strftime(buffer, FORMAT_BUFFER_SIZE, fmt, &tmTime);

		buffer[len] = '\0';
		return buffer;
	}

	Timestamp& Timestamp::operator+=(const Timestamp& rhs)
	{
		m_seconds += rhs.m_seconds;
		m_microseconds += rhs.m_microseconds;

		if(m_microseconds > ONE_SEC_IN_MICROSEC) {
			++m_seconds;
			m_microseconds -= ONE_SEC_IN_MICROSEC;
		}

		return *this;
	}

	Timestamp& Timestamp::operator-=(const Timestamp& rhs)
	{
		m_seconds -= rhs.m_seconds;
		m_microseconds -= rhs.m_microseconds;

		if(m_microseconds < 0) {
			--m_seconds;
			m_microseconds += ONE_SEC_IN_MICROSEC;
		}

		return *this;
	}

	Timestamp& Timestamp::operator/=(time_t rhs)
	{
		time_t rem_secs = m_seconds % rhs;
		m_seconds /= rhs;

		m_microseconds /= rhs;
		m_microseconds += ((rem_secs * ONE_SEC_IN_MICROSEC) / rhs);

		return *this;
	}

	Timestamp& Timestamp::operator*=(time_t rhs)
	{
		time_t new_usec = m_microseconds * rhs;
		time_t overflow_sec = new_usec / ONE_SEC_IN_MICROSEC;
		m_microseconds = new_usec % ONE_SEC_IN_MICROSEC;

		m_seconds *= rhs;
		m_seconds += overflow_sec;

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Timestamp globals
	//////////////////////////////////////////////////////////////////////////////
	const Timestamp operator+(const Timestamp& lhs, const Timestamp& rhs)
	{
		return Timestamp(lhs) += rhs;
	}

	const Timestamp operator-(const Timestamp& lhs, const Timestamp& rhs)
	{
		return Timestamp(lhs) -= rhs;
	}

	const Timestamp operator/(const Timestamp& lhs, time_t rhs)
	{
		return Timestamp(lhs) /= rhs;
	}

	const Timestamp operator*(const Timestamp& lhs, time_t rhs)
	{
		return Timestamp(lhs) *= rhs;
	}

	bool operator<(const Timestamp& lhs, const Timestamp& rhs)
	{
		return ( (lhs.getSeconds() < rhs.getSeconds())
			|| ( (lhs.getSeconds() == rhs.getSeconds()) && (lhs.getMicroSeconds() < rhs.getMicroSeconds()) ) );
	}

	bool operator<=(const Timestamp& lhs, const Timestamp& rhs)
	{
		return ((lhs < rhs) || (lhs == rhs));
	}

	bool operator>(const Timestamp& lhs, const Timestamp& rhs)
	{
		return ( (lhs.getSeconds() > rhs.getSeconds())
			|| ( (lhs.getSeconds() == rhs.getSeconds()) && (lhs.getMicroSeconds() > rhs.getMicroSeconds()) ) );
	}

	bool operator>=(const Timestamp& lhs, const Timestamp& rhs)
	{
		return ((lhs > rhs) || (lhs == rhs));
	}

	bool operator==(const Timestamp& lhs, const Timestamp& rhs)
	{
		return (lhs.getSeconds() == rhs.getSeconds() && lhs.getMicroSeconds() == rhs.getMicroSeconds());
	}

	bool operator!=(const Timestamp& lhs, const Timestamp& rhs)
	{
		return !(lhs == rhs);
	}
}
