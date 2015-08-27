#include "stdafx.h"

#include <cacti/kernel/Thread.h>

#include <cacti/kernel/Mutex.h>

#ifdef WIN32
#	include <assert.h>
#	include <process.h>
#else
#	include <assert.h>
#	include <unistd.h>
#	include <errno.h>
#	include <pthread.h>
#	include <sched.h>
#endif

#include <memory>

#define new DEBUG_NEW

namespace 
{
	class ThreadParam
	{
	public:
		ThreadParam(cacti::Runable* runobj, bool owned)
			: m_runobj(runobj)
			, m_ownedRunobj(owned)
			, m_started(false)
		{
		}
		
		void wait()
		{
			cacti::RecursiveMutex::ScopedLock scopedLock(m_mutex);
			while(!m_started)
			{
				m_mutex.unlock();
				m_event.wait();
				m_mutex.lock();
			}
		}

		void started()
		{
			cacti::RecursiveMutex::ScopedLock scopedLock(m_mutex);
			m_started = true;
			m_event.signal();
		}

		cacti::RecursiveMutex	m_mutex;
		cacti::Event			m_event;
		cacti::Runable*			m_runobj;
		bool					m_ownedRunobj;
		bool					m_started;
	};

#ifdef WIN32
	unsigned __stdcall threadProxy(void *param)
#else
	void* threadProxy(void *param)
#endif
	{

#ifndef _DEBUG
		try
		{
#endif
			ThreadParam *p = static_cast<ThreadParam*>(param);
			assert(p);

			cacti::Runable* runobj = p->m_runobj;

			// remember the runobj if we owned it.
			std::auto_ptr<cacti::Runable>ownedRunobj;
			if(p->m_ownedRunobj)
				ownedRunobj.reset(runobj);
			
			p->started();
			runobj->run();
#ifndef _DEBUG
		}
		catch (...)
		{
		}
#endif
		return 0;
	}
}

namespace cacti
{
	Thread::Thread(const char* name/* = ""*/)
#ifdef WIN32
		: m_thread(INVALID_HANDLE_VALUE)
		, m_id(0)
#else
		: m_thread(0)
#endif
	{
		if(name)
			m_name = name;
	}

	Thread::~Thread()
	{
		closeHandle();
	}
	
	void Thread::closeHandle()
	{
#ifdef WIN32 
		BOOL ret = FALSE;
		if(m_thread != INVALID_HANDLE_VALUE)
		{		
			ret = CloseHandle(m_thread);
			assert(ret);
		}
#else
		if(m_thread != 0)
			pthread_detach(m_thread);
#endif
	}

	bool Thread::operator==(const Thread &other) const
	{
#ifdef WIN32
		return m_id == other.m_id;
#else
		return pthread_equal(m_thread, other.m_thread) != 0;
#endif
	}

	bool Thread::operator!=(const Thread &other) const
	{
		return !operator==(other);
	}

	void Thread::start(Runable* runobj, bool owned)
	{
		assert(runobj);

		ThreadParam param(runobj, owned);

#ifdef WIN32
		assert(m_thread == INVALID_HANDLE_VALUE); // only start once !

		if(m_thread != INVALID_HANDLE_VALUE)
			return ;	// throw an exception will better or not ?

		m_thread = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, &threadProxy,
			&param, 0, &m_id));
		if(m_thread == INVALID_HANDLE_VALUE)
			throw SyscallException(__FILE__, __LINE__);

		if(m_name.empty())
		{
			char tmp[16];
			sprintf(tmp, "%08X", m_id);
			m_name = tmp;
		}
#else
		assert(m_thread == 0); // only start once !

		int ret = 0;
		ret = pthread_create(&m_thread, 0, &threadProxy, &param);
		if(ret != 0)
			throw SyscallException(__FILE__, __LINE__);

		if(m_name.empty())
		{
			char tmp[16];
			sprintf(tmp, "%08X", m_thread);
			m_name = tmp;
		}
#endif
		// wait for thread really started!
		// otherwise the join method will run incorrectly
		// if it was call immediately after the thread started.
		param.wait();
	}
	
	void Thread::join()
	{
		int ret = 0;
#ifdef WIN32 
		if(m_thread != INVALID_HANDLE_VALUE)
		{
			ret = WaitForSingleObject(m_thread, INFINITE);
			assert(ret == WAIT_OBJECT_0);
			ret = CloseHandle(m_thread);
			assert(ret);
			m_thread = INVALID_HANDLE_VALUE;
			m_id = 0;
		}
#else
		if(m_thread != 0)
		{
			ret = pthread_join(m_thread, 0);
			assert(ret == 0);
			m_thread = 0;
		}
#endif
	}

	void Thread::sleep(const Timestamp& xt)
	{
#ifdef WIN32
		Sleep((DWORD)xt.getAsMilliseconds());
#else
		timespec ts;
		xt.toTimespecDuration(ts);
		nanosleep(&ts, 0);

#endif
	}

	void Thread::yield()
	{
#ifdef WIN32
		Sleep(0);
#else
		int ret = 0;
		ret = sched_yield();
		assert(ret == 0);
		
#endif
	}

	Thread::thread_id Thread::currentId()
	{
#ifdef WIN32
		return GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}

	void Thread::run()
	{
		// invalid usage
		assert(false); 
	}
}
