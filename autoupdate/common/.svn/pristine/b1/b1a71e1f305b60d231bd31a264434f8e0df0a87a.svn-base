#ifndef INCLUDED_QUEUE_H
#define INCLUDED_QUEUE_H

#if defined(HAS_PRAGMA_ONCE)
#pragma PRAGMA_ONCE_DECLARE
#endif

#include <cacti/util/NonCopyable.h>
#include <cacti/kernel/Mutex.h>
#include <cacti/config/Portability.h>

#include <vector>

namespace cacti
{
	template<class T>
	class BoundedQueue : private NonCopyable
	{
		typedef cacti::RecursiveMutex::ScopedLock ScopedLock;
	public:
		BoundedQueue(int max) 
			: m_begin(0)
			, m_end(0)
			, m_buffered(0)
			, m_circularBuf(max) 
		{
		}

		void put(const T &m) 
		{
			ScopedLock lk(m_monitor);
			while (m_buffered >= m_circularBuf.size())
			{
				lk.unlock();
				m_notFull.wait();
				lk.lock();
			}
			purePut(m);
			m_notEmpty.signal();
		}

		void putHead(const T& m)
		{
			ScopedLock lk(m_monitor);
			while (m_buffered >= m_circularBuf.size())
			{
				lk.unlock();
				m_notFull.wait();
				lk.lock();
			}
			purePutHead(m);
			m_notEmpty.signal();
		}

		bool tryPut(const T &m)
		{
			ScopedLock lk(m_monitor);
			if(m_buffered >= m_circularBuf.size())
			{
				return false;
			}
			else
			{
				purePut(m);
				m_notEmpty.signal();
				return true;
			}
		}

		bool tryPutHead(const T &m)
		{
			ScopedLock lk(m_monitor);
			if(m_buffered >= m_circularBuf.size())
			{
				return false;
			}
			else
			{
				purePutHead(m);
				m_notEmpty.signal();
				return true;
			}
		}

		void get(T &m) 
		{
			ScopedLock lk(m_monitor);
			while (m_buffered <= 0)
			{
				lk.unlock();
				m_notEmpty.wait();
				lk.lock();
			}
			pureGet(m);
			m_notFull.signal();
		}

		bool get(T &m, int wait)
		{
			ScopedLock lk(m_monitor);
			Timestamp xt(wait, 0);
			bool signaled = false;
			while (m_buffered <= 0)
			{
				lk.unlock();
				signaled = m_notEmpty.wait(xt);
				lk.lock();
				if(!signaled)
					return false;
			}
			pureGet(m);
			m_notFull.signal();
			return true;
		}

		bool tryGet(T &m)
		{
			ScopedLock lk(m_monitor);
			if(m_buffered <= 0)
			{
				return false;
			}
			else
			{
				pureGet(m);
				m_notFull.signal();
				return true;
			}
		}

		size_t size() 
		{
			return m_buffered;
		}
	private:
		inline void purePut(const T& m)
		{
			m_circularBuf[m_end] = m;
			m_end = (m_end+1) % m_circularBuf.size();
			++m_buffered;
		}
		inline void purePutHead(const T& m)
		{
			if(m_begin == 0)
				m_begin = m_circularBuf.size();
			m_begin--;
			m_circularBuf[m_begin] = m;
			++m_buffered;
		}
		inline void pureGet(T& m)
		{
			m = m_circularBuf[m_begin];
			// clear the memory
			T t;
			std::swap(m_circularBuf[m_begin], t);
			m_begin = (m_begin+1) % m_circularBuf.size();
			--m_buffered;
		}
	private:
		size_t m_begin;
		size_t m_end;
		size_t m_buffered;
		std::vector<T> m_circularBuf;
		Event m_notFull;
		Event m_notEmpty;
		RecursiveMutex m_monitor;
	};
}	
// todo : 
//	change a "Event" to something, the current implementation of "Event"
//  on LINUX platform is inefficient! Maybe use condition is a good idea.

#endif // INCLUDED_QUEUE_H
