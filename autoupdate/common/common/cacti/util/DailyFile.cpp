#include "stdafx.h"

#include "cacti/util/DailyFile.h"
#include "cacti/util/FileSystem.h"
#include "cacti/util/Timestamp.h"
#include "cacti/util/StringUtil.h"

namespace cacti
{
	DailyFile::DailyFile(string extention, bool timeflag /*= false*/)
	{
		m_strExtension = extention ;
		string  path = "./"+extention;
		m_pFilehandle = NULL;
		m_lasttime = time(0);
		m_bTimeFlag = timeflag;

		FileSystem::mkdir(path.c_str());
	}
	DailyFile::~DailyFile()
	{ 
		if(m_pFilehandle) 
			fclose(m_pFilehandle);
	}

	void DailyFile::print(time_t ti, const char * format ,...)
	{
		RecursiveMutex::ScopedLock filelock(m_filelock);


		FILE *fp = GetFileHandle( ti );
		if( fp )
		{
			va_list va;
			va_start(va, format);
			string content  = StringUtil::vform(format, va);
			if(m_bTimeFlag)
			{
				Timestamp atime(time(0));
				tm tmTime;
				atime.localtime( tmTime );

				fprintf(fp,"%02d%02d %02d:%02d:%02d ", 
					tmTime.tm_mon+1, 
					tmTime.tm_mday, 
					tmTime.tm_hour, 
					tmTime.tm_min, 
					tmTime.tm_sec );

			}
			fprintf(fp,"%s",content.c_str());
			fflush(fp);
			va_end(va);
		}
		if( fp && fp != m_pFilehandle  )//for old time than current, open and close each time
			fclose( fp );

	}
	void DailyFile::print(const char * format,...)
	{
		RecursiveMutex::ScopedLock filelock(m_filelock);
		
		va_list va;
		va_start(va, format);
		string content  = StringUtil::vform(format, va);	
		print( time(0),"%s",content.c_str());
		
		va_end(va);

	}

	FILE * DailyFile::GetFileHandle(time_t ti)
	{
	
       if(ti >= m_lasttime)
		   return OpenFilebytm(ti);
	   else
		   return OpenFilebytm(ti,false);

	}

	FILE * DailyFile::OpenFilebytm(time_t ti,bool replaceold /*=true*/)
	{

		Timestamp atime(ti);
		tm tmtime;
		atime.localtime( tmtime );
		if( m_pFilehandle && tmtime.tm_year == m_year 
			&& tmtime.tm_mon == m_month && tmtime.tm_mday == m_mday )
		{
			return m_pFilehandle;
		}

		char buf[64];
		sprintf(buf,"/%04d%02d%02d.",tmtime.tm_year+1900, tmtime.tm_mon+1,
			tmtime.tm_mday );

		string filename = "./"+ m_strExtension+buf+m_strExtension;
		FILE *fp = fopen( filename.c_str(),"a+t");

		if( fp && replaceold)
		{
			if(m_pFilehandle)
				fclose(m_pFilehandle);

			m_pFilehandle = fp;
			m_year = tmtime.tm_year;
			m_month = tmtime.tm_mon;
			m_mday = tmtime.tm_mday;
			m_lasttime = ti;
			
		}
		return fp;
	}

}