#include "stdafx.h"

#include "cacti/util/Timer.h"
#include "sxconst.h"

#define new DEBUG_NEW

namespace cacti
{
    TimerLogic::TimerLogic()
		: m_logger(Logger::getInstance(LOGGER_MTL_TIMER))
		, m_tickCount(0)
    {
		m_logger.setAdditivity(true);

        memset(&m_tv1, 0, sizeof(m_tv1));
        memset(&m_tv2, 0, sizeof(m_tv2));
        memset(&m_tv3, 0, sizeof(m_tv3));
        memset(&m_tv4, 0, sizeof(m_tv4));
        memset(&m_tv5, 0, sizeof(m_tv5));
        m_tvecs[0] = (TimerVec *)&m_tv1;
        m_tvecs[1] = &m_tv2;
        m_tvecs[2] = &m_tv3;
        m_tvecs[3] = &m_tv4;
        m_tvecs[4] = &m_tv5;
    }

    // 将一个定时器插入到合适的盘子合适的刻度
    bool TimerLogic::insert (TimerList *timer)
    {
		u32 expires = timer->expires;
        u32 idx = expires - m_tickCount;
        TimerList **vec;

		int vidx;
		int vpos;

        if(idx < TVR_SIZE) 
        {
            int i = expires & TVR_MASK;
            vec = m_tv1.vec + i;

			vidx = i;
			vpos = 1;
        } 
		else if (idx < (1 << (TVR_BITS + TVN_BITS)) ) 
        {
            int i = (expires >> TVR_BITS) & TVN_MASK;
            vec = m_tv2.vec + i;

			vidx = i;
			vpos = 2;
        } 
		else if (idx < (1 << (TVR_BITS + 2 * TVN_BITS)) ) 
        {
            int i = (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
            vec = m_tv3.vec + i;
			
			vidx = i;
			vpos = 3;
        } 
		else if (idx < (1 << (TVR_BITS + 3 * TVN_BITS)) ) 
        {
            int i = (expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
            vec = m_tv4.vec + i;

			vidx = i;
			vpos = 4;
        } 
		else if ((signed long) idx < 0) 
        {
            // can happen if you add a timer with expires == jiffies,
            // or you set a timer to go off in the past
			// then set it into current index
            vec = m_tv1.vec + m_tv1.index;

			expires = 0;

			vidx = 0;
			vpos = 1;
        } 
		else if (idx <= 0xffffffffUL)
        {
            int i = (expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
            vec = m_tv5.vec + i;

			vidx = i;
			vpos = 5;
        } 
		else 
        {
            return false;
        }
		
		if(m_logger.isDebugEnabled())
		{
			m_logger.debug("[TM%d] idx=%d, vidx=%d, vpos=%d, expire=%d\n", 
				timer->timerid, idx, vidx, vpos, expires);
		}

		timer->next = *vec;
        if (timer->next)   // 赋值next指针
            (*vec)->prev = timer;
        *vec = timer;
        timer->prev = (TimerList *)vec;
        return true;
    }
    void TimerLogic::erase  (TimerList *curr)
    {
        TimerList  *prev = curr->prev;
        if (prev)
        {
            TimerList *next = curr->next;
            prev->next = next;
            if (next)
                next->prev = prev;
            curr->next = curr->prev = 0;
        }
    }
    void TimerLogic::cascade(TimerVec  *tv)
    {
        // cascade all the timers from tv up one level 
        TimerList *timer = tv->vec[tv->index];
        TimerList *tmp = NULL;
        while (timer)
        {
			if(m_logger.isDebugEnabled())
			{
				m_logger.debug("[TM%d] cascade\n", timer->timerid);
			}
            tmp   = timer;
            timer = timer->next;
            insert(tmp);
        }
        tv->vec[tv->index] = NULL;
        tv->index = (tv->index + 1) & TVN_MASK;
    }

    ActiveTimer::ActiveTimer(u32 timeslice)
        : m_exitSignaled(false)
		, m_logger(Logger::getInstance(LOGGER_MTL_TIMER))
		, m_allocatedTimer(MAX_TIMER_COUNT)
    {
        if(timeslice < MIN_TIMESLICE)
            m_timeslice = DEFAULT_TIMESLICE;
        else if(timeslice > MAX_TIMESLICE)
			m_timeslice = DEFAULT_TIMESLICE;
		else
            m_timeslice = timeslice;

		for(size_t i=0; i<m_allocatedTimer.size(); ++i)
		{
			m_allocatedTimer[i] = new TimerList;
			m_allocatedTimer[i]->timerid = INVALID_ID;
		}
    }

    ActiveTimer::~ActiveTimer()
    {
		for(size_t i=0; i<m_allocatedTimer.size(); ++i)
		{
			if( m_allocatedTimer[i]->timerid != INVALID_ID)
					delete m_allocatedTimer[i]->task;
			delete m_allocatedTimer[i];
		}
    }

	bool ActiveTimer::start()
	{
		// remember the index
		for(size_t i=0; i<m_allocatedTimer.size(); ++i)
			m_freeTimers.push_back(i);

		m_logger.info("[TM] start\n");

		m_exitSignaled = false;
		m_thread.start(this, &ActiveTimer::clockDriver);
		return true;
	}
	
	void ActiveTimer::stop()
	{
		m_exitSignaled = true;
		m_thread.join();
		m_logger.info("[TM] stopped\n");
	}

    TimerID ActiveTimer::set(u32 expires, TimerTask *owner)
    {
		cacti::RecursiveMutex::ScopedLock sl(m_mutex);

		if(m_freeTimers.size() <= 0)
		{
			m_logger.warn("[TM] enlarge the size!\n");
			enlarge();
		}

		// 在头取一个定时器ID出来
		size_t id = m_freeTimers.front();
        TimerList* timer = m_allocatedTimer[id];
        timer->expires    = expires/m_timeslice + m_tickCount;    // calculate tick count
        timer->task       = owner;
		timer->timerid	  = id;

		if(m_logger.isDebugEnabled())
		{
			m_logger.debug("[TM%d] set with %d tick\n", 
				id,
				timer->expires);
		}

        if(insert(timer))
        {
			m_freeTimers.pop_front();
		}
		else
		{
			id = INVALID_ID;
			m_logger.warn("[TM] set timer(%d) failed\n", timer->expires);
		}

		return id;
    }

	void ActiveTimer::enlarge()
	{
		size_t oldsize = m_allocatedTimer.size();
		size_t newsize = oldsize + ENLARGE_SIZE;
		m_allocatedTimer.resize(newsize);
		for(size_t i=oldsize; i<newsize; ++i)
		{
			m_allocatedTimer[i] = new TimerList;
			m_allocatedTimer[i]->timerid = INVALID_ID;
			m_freeTimers.push_back(i);
		}
	}

	// canceling an timeout timerid will be dangerous!
    TimerTask* ActiveTimer::cancel(TimerID timerid)
    {
		cacti::RecursiveMutex::ScopedLock sl(m_mutex);
		if(timerid>=0 && timerid<=m_allocatedTimer.size())
		{
			if(m_allocatedTimer[timerid]->timerid != INVALID_ID)
			{
				erase(m_allocatedTimer[timerid]);
				m_allocatedTimer[timerid]->timerid = INVALID_ID;
				m_freeTimers.push_back(timerid);

				if(m_logger.isDebugEnabled())
				{
					m_logger.debug("[TM%d] cancel\n", timerid);
				}

				return m_allocatedTimer[timerid]->task;
			}
		}
		return 0;
    }

    void ActiveTimer::clockDriver()
    {
        volatile TimerList *curr  = NULL;
        TimerList *timer = NULL;
		u32 prev = (u32)Timestamp::getNowTime().getAsMilliseconds();
		// m_tv1.index = 1;
        while(!m_exitSignaled)
        {
			cacti::Thread::sleep(Timestamp(0, 1000*m_timeslice));

			u32 now = (u32)Timestamp::getNowTime().getAsMilliseconds();
			
			u32 esc = calcEscTime(prev, now);
			int cc = (int)(esc / m_timeslice);			// 计算经过了多少个tick

			if(m_logger.isDebugEnabled())
			{
				m_logger.debug("[TM] cc(%d) esc(%d), tick(%d)\n", cc, esc, m_tickCount);
			}

			m_mutex.lock();
			for(int i=0; i<cc; ++i)
			{
				// 大盘已经转一周了，将下面的定时器往上移动
				if (m_tv1.index == 0)
				{
					int n = 1;
					do{
						if(m_logger.isDebugEnabled())
						{
							m_logger.debug("[TM] cascade, n=%d, idx=%d\n", n, m_tvecs[n]->index);
						}
						cascade(m_tvecs[n]);
					} while (m_tvecs[n]->index == 1 && ++n < NOOF_TVECS);
					// m_tvecs[n]->index == 1 表示上一个层已经转过一圈了。
					if(m_logger.isDebugEnabled())
					{
						m_logger.debug("[TM] tv1.index=%d, tv2.index=%d, tv3.index=%d, tv4.index=%d, tv5.index=%d\n",
							m_tv1.index, m_tv2.index, m_tv3.index, m_tv4.index, m_tv5.index);
					}
				}

				// 检查当前时刻的定时器列表，一一触发他们
				while((curr = m_tv1.vec[m_tv1.index]))
				{
					// 移走已经触发的定时器
					if(m_logger.isDebugEnabled())
					{
						m_logger.debug("[TM%d] signaled\n", curr->timerid);
					}
					curr->task->onTimer();
					cancel(curr->timerid);
					curr = NULL;
				}
				// 刻度前行
				m_tickCount++;
				// 时钟刻度走一格
				m_tv1.index = (m_tv1.index + 1) & TVR_MASK;
			}
			prev += cc*m_timeslice;					// prev只记录我们逻辑上过了多少时间

			m_mutex.unlock();
        }
    }

	u32 ActiveTimer::calcEscTime(u32& prev, u32 now)
	{
		if(now < prev)
		{
			m_logger.warn("[TM] system time was turn backward, now(%d) prev(%d)\n",
				now, prev);

			// system time was return		
			prev = now-m_timeslice;
			return m_timeslice;
		}
		else if(prev + 8*m_timeslice < now)
		{
			m_logger.warn("[TM] system time was turn forward, now(%d) prev(%d)\n",
				now, prev);

			// system time was forward
			prev = now-m_timeslice;
			return m_timeslice;
		}
		else
		{
			return (now-prev);
		}
	}
}
