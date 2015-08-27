#ifndef INCLUDED_CACTI_TIMER_H
#define INCLUDED_CACTI_TIMER_H

#pragma once

#include "cacti/kernel/SysObj.h"
#include "cacti/util/Timestamp.h"
#include "cacti/message/TypeDef.h"

#include "cacti/logging/Logger.h"

#include <map>

namespace cacti
{
    //the task base class
    class TimerTask
    {
    public:
        virtual void onTimer() = 0;
		virtual std::string toString(){ return "";};
		virtual ~TimerTask(){};
    };

    // 实现定时器算法逻辑，本身不带定时驱动器
    // ************************************************************************
    // 算法描述：
    // TimerLogic实现了一个模拟的时钟，这个时钟的最上一层（类似秒针）是一个256个刻度的盘子，
    // 下面四层（类似分钟，时针）有刻度是64的小盘子，上层转一圈之后下一层的刻度就走一个刻度。
    // 指示当前盘子走到那个刻度是TimerVec（或TimerVecRoot）的index。
    // 添加定时器的时候根据定时的时间将定时器数据（TimerList）放在恰当的盘子的恰当刻度中。
    // 时钟开始运转之后每一个滴答就检查第一个盘子的当前刻度是否有数据，如果有则触发该定时器
    // 当第一个盘子走完一圈之后就意味着下一个盘子的当前刻度的定时器（如果有）
    // 应该转移到第一个盘子上面来了，这时候将下一个盘子的当前刻度定时器全部重新插入第一个盘子
    // 然后依次检查第三、第四、第五个盘子是否也要将当前刻度的定时器重新插入上层。
    // 检查的依据就是上一层是否已经运转一圈了。
    //************************************************************************
	typedef size_t TimerID;
    struct TimerList
    {
        TimerList       *next;			// the next must at first
        TimerList       *prev;

		// Timer private data
		TimerID			 timerid;		// 冗余定时器ID，使得自己可以发现自己的ID
        u32			     expires;       // 多少个tick之后定时器到达
        TimerTask       *task;          // 定时器任务数据
        TimerList()
            : next(NULL)
            , prev(NULL)
			, task(NULL)
        {}
    };

    class TimerLogic
    {
    public:
		enum { NOOF_TVECS   = 5 };
		enum { TVN_BITS     = 6 };
		enum { TVR_BITS     = 8 };
		enum { TVN_SIZE     = (1 << TVN_BITS)	};		// 下面4层的大小是64
		enum { TVR_SIZE     = (1 << TVR_BITS)	};		// 最上层刻度大小256
		enum { TVN_MASK     = (TVN_SIZE - 1)	};
		enum { TVR_MASK     = (TVR_SIZE - 1)	};
		// TimerVec and TimerVecRoot has same grammar structure
        struct TimerVec					// other circle
        {
            int         index;			//
            TimerList  *vec[TVN_SIZE];
        };  
        struct TimerVecRoot				// root circle
        {
            int         index;
            TimerList  *vec[TVR_SIZE];
        };
        /////////////////////////////////////////////////////////////////////////////////
        TimerLogic();
        bool    insert (TimerList *timer);
        void    erase  (TimerList *curr);
        void    cascade(TimerVec  *tv);

    protected:
		unsigned int  m_tickCount;

        // a clock has a root and 4 other circle
        TimerVec      *m_tvecs[NOOF_TVECS];
        TimerVecRoot  m_tv1;
        TimerVec      m_tv2, m_tv3, m_tv4, m_tv5;

		cacti::Logger& m_logger;
    };

    // 定时驱动器，驱动tickCount往前进行。
    // 并负责派送消息到TimerTask
    // 使用TimerList对象的地址作为定时器的ID	
	class ActiveTimer : public TimerLogic
	{
    public:
		static const u32 MIN_TIMESLICE		= 50;       // ms
		static const u32 MAX_TIMESLICE		= 1000;		// ms
		static const u32 DEFAULT_TIMESLICE	= 100;		// ms
		static const u32 MAX_TIMER_COUNT    = 1024*4;	
		static const u32 ENLARGE_SIZE		= 1024;
		static const TimerID INVALID_ID		= ~0;

        ActiveTimer(u32 timerslice = DEFAULT_TIMESLICE);
        ~ActiveTimer();
		
		bool start();
		void stop();

        // 设置一个定时器，定时长度为expires，定时器到达通知owner，通知的时候带回userdata
		// 注意：userdata应该是一个堆中的指针
        TimerID set(u32 expires, TimerTask *owner);
        // 取消一个定时器，返回用户数据
        TimerTask* cancel(TimerID timerid);

	private:
		void	enlarge();
		void    clockDriver();
		u32		calcEscTime(u32& prev, u32 now);

    private:
		cacti::Thread m_thread;
        volatile bool m_exitSignaled;
        u32  m_timeslice;                       //precision of tick
		cacti::RecursiveMutex m_mutex;
		std::vector<TimerList*> m_allocatedTimer;
		std::list<size_t> m_freeTimers;
		cacti::Logger& m_logger;
    };
};


#endif	// INCLUDED_CACTI_TIMER_H
