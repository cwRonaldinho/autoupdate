#ifndef INCLUDED_CACTI_KERNEL_MUTEX_H
#define INCLUDED_CACTI_KERNEL_MUTEX_H

#if defined(HAS_PRAGMA_ONCE)
#pragma PRAGMA_ONCE_DECLARE
#endif

#include <cacti/util/Timestamp.h>
#include <cacti/util/NonCopyable.h>

#include <cacti/kernel/KernelException.h>

#ifndef WIN32
#	include <errno.h>
#	include <pthread.h>
#	if defined( __linux__ )
#		include <semaphore.h>
#	else	// other UNIX
#		include <synch.h>    
#		include <semaphore.h>
#	endif
#else
#	include <Windows.h>
#	include <process.h>
#endif

#include <string>


namespace cacti 
{
	// auto locker for a block scope
	template <class T> class ScopedLock : private NonCopyable
	{
	public:
		ScopedLock(T &l)
			: m_locker(l)
		{
			m_locker.lock();
		}
		~ScopedLock()
		{
			m_locker.unlock();
		}
		// with the same interface
		void lock() 
		{
			m_locker.lock();
		}
		void unlock()
		{
			m_locker.unlock();
		}
	private:
		T &m_locker;
	};

	// lock nothing for policy
	struct LockNothing
	{
		typedef ScopedLock<LockNothing> ScopedLock;
		void lock(){}
		void unlock(){}
	};

	/////////////////////////////////////////////////////////////////////////
	// recursive mutex which will not lock it self.
	class RecursiveMutex : private NonCopyable
	{
	public:
		typedef ScopedLock<RecursiveMutex> ScopedLock;
	public:
		RecursiveMutex(const char* name = NULL);
		~RecursiveMutex();	
		void lock(); 
		void unlock();
	private:
		std::string m_name;
#if 	defined(WIN32)
		CRITICAL_SECTION m_cs;
#else 
		pthread_mutex_t	m_mutex;
#endif
	};


	/////////////////////////////////////////////////////////////////////////
	class Mutex : private NonCopyable
	{
	public:
		typedef ScopedLock<Mutex> ScopedLock;
	public:
		Mutex(const char* name = NULL);
		~Mutex(); 
		void lock();
		void unlock();
	private:
		std::string m_name;
#ifdef WIN32
		CRITICAL_SECTION m_cs;
#else
		pthread_mutex_t m_mutex;
#endif
	};

	/////////////////////////////////////////////////////////////////////////
	// event initial state is non-signaled
	// and manual reset from signaled state to non-signaled state
	class Event : private NonCopyable
	{
	public:
		Event();
		~Event();
		void reset();
		// just signal only waiting thread !
		void signal();
		void wait();
		// return true if object signaled in time
		// else timeout
		bool wait(const Timestamp& xt);

	private:		
#ifdef WIN32
		HANDLE m_handle;
#else
		void init();
		void destroy();
		pthread_cond_t  m_cond;
		pthread_mutex_t m_mutex;
#endif
	};
}
#endif // #ifndef INCLUDED_MUTEX_H
