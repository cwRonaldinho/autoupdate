#include "stdafx.h"

#include <cacti/kernel/Mutex.h>

#ifdef WIN32
#	include <assert.h>
#	include <time.h>
#else
#	include <assert.h>
#	include <unistd.h>
#	include <errno.h>
#	include <pthread.h>
#endif

#define new DEBUG_NEW

#ifdef WIN32
// win32 platform
namespace cacti 
{
	/////////////////////////////////////////////////////////////////////////
	RecursiveMutex::RecursiveMutex(const char* name)	
	{
		if(name)
			m_name = name;

		InitializeCriticalSection(&m_cs);
	}

	RecursiveMutex::~RecursiveMutex()	
	{ 
		DeleteCriticalSection(&m_cs);
	}

	void RecursiveMutex::lock()         
	{ 
		EnterCriticalSection(&m_cs);
	}

	void RecursiveMutex::unlock()
	{ 
		LeaveCriticalSection(&m_cs);
	}

	/////////////////////////////////////////////////////////////////////////
	Mutex::Mutex(const char* name)
	{
		if(name)
			m_name = name;

		InitializeCriticalSection(&m_cs);
	}
	Mutex::~Mutex() 
	{
		DeleteCriticalSection(&m_cs);
	}

	void Mutex::lock()
	{ 
		EnterCriticalSection(&m_cs);
	}

	void Mutex::unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	/////////////////////////////////////////////////////////////////////////
	Event::Event()
	{
		// auto-reset, initial state is non-signaled
		m_handle = ::CreateEvent(0, FALSE, FALSE, 0);
		if(m_handle == INVALID_HANDLE_VALUE)
			throw SyscallException(__FILE__, __LINE__);
	}

	Event::~Event()
	{
		BOOL ret = FALSE;
		ret = ::CloseHandle(m_handle);
		assert(ret);
	}

	void Event::reset()
	{
		::ResetEvent(m_handle);
	}
	void Event::signal() 
	{
		BOOL ret = FALSE;
		ret = ::SetEvent(m_handle);
		assert(ret);
	}

	void Event::wait() 
	{
		::WaitForSingleObject(m_handle, INFINITE);
	}

	bool Event::wait(const Timestamp& xt)
	{
		int ret = 0;
		ret = WaitForSingleObject(m_handle,(DWORD) xt.getAsMilliseconds());
		assert(ret != WAIT_FAILED && ret != WAIT_ABANDONED);
		return ret == WAIT_OBJECT_0;
	}
}
#else
// non-win32 platform

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

namespace cacti 
{
	/////////////////////////////////////////////////////////////////////////
	RecursiveMutex::RecursiveMutex(const char* name)
	{
		if(name)
			m_name = name;
		int rc = 0;

#if defined(__linux) && !defined(__USE_UNIX98)
		const pthread_mutexattr_t attr = { PTHREAD_MUTEX_RECURSIVE_NP };
#else
		pthread_mutexattr_t attr;
		rc = pthread_mutexattr_init(&attr);
		assert(rc == 0);
		rc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		assert(rc == 0);
#endif
		rc = pthread_mutex_init(&m_mutex, &attr);
#if defined(__linux) && !defined(__USE_UNIX98)
		// Nothing to do
#else
		{	
			// use a new local variable(rc)
			int rc = pthread_mutexattr_destroy(&attr);
			assert(rc == 0);
		}
#endif

		if(rc != 0)
			throw SyscallException(__FILE__, __LINE__);
	};
	RecursiveMutex::~RecursiveMutex()
	{
		int ret = 0;
		ret = pthread_mutex_destroy(&m_mutex);
		assert(ret == 0);
	};

	void RecursiveMutex::lock()
	{ 
		int ret = 0;
		ret = pthread_mutex_lock(&m_mutex);
		assert(ret == 0);
	};

	void RecursiveMutex::unlock()
	{ 
		int ret = 0;
		ret = pthread_mutex_unlock(&m_mutex);	
		assert(ret == 0);
	};

	/////////////////////////////////////////////////////////////////////////
	Mutex::Mutex(const char* name)
	{ 
		if(name)
			m_name = name;

		int ret = 0;
		ret = pthread_mutex_init(&m_mutex, 0);
		if(ret != 0)
			throw SyscallException(__FILE__, __LINE__);
	};

	Mutex::~Mutex() 
	{
		int ret = 0;
		ret = pthread_mutex_destroy(&m_mutex); 
		assert(ret == 0);
	};

	void Mutex::lock()		
	{ 
		int ret = 0;
		ret = pthread_mutex_lock(&m_mutex);
		assert(ret == 0);
	};

	void Mutex::unlock()	
	{ 
		int ret = 0;
		ret = pthread_mutex_unlock(&m_mutex);	
		assert(ret == 0);
	};

	/////////////////////////////////////////////////////////////////////////
	Event::Event()
	{
		init();
	}
	Event::~Event()
	{
		destroy();
	}
	void Event::init()
	{
		int ret = 0;
		ret = pthread_cond_init(&m_cond, 0);
		assert(ret == 0);
		ret = pthread_mutex_init(&m_mutex, 0);
		assert(ret == 0);	
	}
	void Event::destroy()
	{
		int ret = 0;
		ret = pthread_cond_destroy(&m_cond);
		assert(ret == 0);
		ret = pthread_mutex_destroy(&m_mutex);
		assert(ret == 0);
	}
	void Event::reset()
	{
		destroy();
		init();
	}
	void Event::signal() 
	{
		pthread_cond_signal(&m_cond);
	}

	void Event::wait()
	{
		pthread_mutex_lock(&m_mutex);
		pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
	}

	// return true if object signaled in time
	// else timeout
	bool Event::wait(const Timestamp& xt)
	{
		int ret = 0;
		ret = pthread_mutex_lock(&m_mutex);
		assert(ret == 0);

		timespec ts;
		xt.toTimespec(ts);
		ret = pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
		assert(ret == 0 || ret == ETIMEDOUT);
		int ret2 = pthread_mutex_unlock(&m_mutex);
		assert(ret2 == 0);
		return ret == 0;
	}
}

#endif // INCLUDED_MUTEX_H
