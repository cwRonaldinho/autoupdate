#ifndef __DAILYFILE_H
#define __DAILYFILE_H

#include <string>
using namespace std;

#include "cacti/kernel/Mutex.h"
namespace cacti
{
	class DailyFile
	{
	public:
		DailyFile( string extention,bool timeflag = false );
		~DailyFile() ;
		
		int open();
		void print(const char * format,...);
		void print(time_t ti, const char * format,...);

	private:
		FILE *GetFileHandle(time_t ti);
		FILE *OpenFilebytm(time_t ti,bool replaceold =true);

		FILE *m_pFilehandle;

		int m_mday;
		int m_month;
		int m_year;
		time_t	m_lasttime;
		bool	m_bTimeFlag;

		string m_strExtension;
		RecursiveMutex m_filelock;
	};
}

#endif //__DAILYFILE_H