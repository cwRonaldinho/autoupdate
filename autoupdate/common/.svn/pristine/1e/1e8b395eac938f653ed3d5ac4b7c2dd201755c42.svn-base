#ifndef INCLUDED_CACTI_KERNEL_THREAD_H
#define INCLUDED_CACTI_KERNEL_THREAD_H

#include <cacti/util/Timestamp.h>
#include <cacti/util/NonCopyable.h>
#include <cacti/kernel/KernelException.h>

#include <assert.h>

#ifndef WIN32
#	include <pthread.h>
#else
#	include <Windows.h>
#	include <process.h>
#endif

namespace cacti 
{
	// The thread interface
	class Runable : private NonCopyable
	{
	public:
		virtual ~Runable(){};
		virtual void run() = 0;
	};
}

namespace 
{
	// inner class for start method.
	// define those class in the "start" function maybe more suitable.
	// but, GCC 2.96(maybe) and later) do not support inner class 
	// in function with template parameter! so we define it in here ugly
	template <class C>
	class ClassFunctionThread : public cacti::Runable
	{
	public:
		explicit ClassFunctionThread(C* c, void (C::*cf)())
			: m_class(c)
			, m_cf(cf)
		{
			assert(m_class);
			assert(m_cf);
		}

	private:
		void run()
		{
			(m_class->*m_cf)();
		}

	private:
		C* m_class;
		void (C::*m_cf)();
	};

	// class for "void start(void (*func)())"
	class FunctionThread : public cacti::Runable
	{
	public:
		explicit FunctionThread(void (*func)())
			: m_func(func)
		{
		}

	private:
		void run()
		{
			m_func();
		}

	private:
       void (*m_func)();
	};
}

namespace cacti
{
	// basic thread implement
	class Thread : public Runable 
	{
	public:
#ifdef WIN32
		typedef DWORD thread_id;
#else
		typedef  pthread_t thread_id;
#endif

	public:
		Thread(const char* name = "");
		virtual ~Thread();
	
		bool operator==(const Thread &other) const;
		bool operator!=(const Thread &other) const;

		// start the thread with runobj->run()
		// if owned == true, then runobj will be deleted 
		// by the thread when it run finished.
		void start(Runable *runobj, bool owned);

		// start thread with Thread::run()
		// client must overriding run() method
		void start()
		{
			// runobj not owned by the thread
			start(this, false);	
		}

		// start the thread with a class<T> function
		template <class T>
			void start(T *c, void (T::*cf)())
		{
			start(new ClassFunctionThread<T>(c, cf), true);
		}
		
		// start thread with alone function
		void start(void (*func)())
		{
			start(new FunctionThread(func), true);
		}

		const char* getName()
		{
			return m_name.c_str();
		}

		// wait for thread finished
		void join();		
		// static method for calling thread sleep
		static void sleep(const Timestamp& xt);
		
		// static method for calling thread abandon£¨·ÅÆú£©current time slice
		static void yield();
	
		// return the running thread id
		static thread_id currentId();

	private:
		// final execution of a thread
		virtual void run();
		void closeHandle();

		std::string m_name;

#ifdef WIN32
		HANDLE m_thread;	// thread handle
		unsigned m_id;		// thread identity
#else
		pthread_t m_thread;
#endif
	};
}

#endif
