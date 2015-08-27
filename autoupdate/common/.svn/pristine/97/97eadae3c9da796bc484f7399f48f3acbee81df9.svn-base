#ifndef INCLUDED_SEQID_H
#define INCLUDED_SEQID_H

#pragma once

#include "cacti/kernel/Mutex.h"

namespace cacti
{
	template<typename LockPolicy>
    class BasicSequenceGenerator
    {
	public:
		enum 
		{
			INVALID_ID  = 0x00000000
		};
		enum
		{
			SELF_ID     = 0x00000001
		};
		enum
		{
			MIN_ID      = 0x00001000
		};
		enum
		{
			MAX_ID      = 0x8FFFFFFF
		};

    public:
        BasicSequenceGenerator(unsigned int min=MIN_ID, unsigned int max=MAX_ID)
			: m_min(min)
			, m_max(max)
        {
			if(m_min <= SELF_ID)
				m_min = SELF_ID+1; 
            m_idseq = m_min;
        }
        static unsigned int getSelf()
        {
            return SELF_ID;
        }
        static bool isSelf(unsigned int id)
        {
            return id == SELF_ID;
        }
        static unsigned int getInvalid()
        {
            return INVALID_ID;
        }
        static bool isValid(unsigned int id)
        {
            return id != INVALID_ID;
        }
        unsigned int next()
        {
			typename LockPolicy::ScopedLock scope_cs(m_mutex);
            if(++m_idseq >= m_max)
                m_idseq = m_min;
            return m_idseq;
        }
    private:
        LockPolicy	 m_mutex;
		unsigned int m_min;
		unsigned int m_max;
        unsigned int m_idseq;
    };
	typedef BasicSequenceGenerator<cacti::RecursiveMutex> SequenceGenerator;
	typedef BasicSequenceGenerator<cacti::LockNothing> SingleThreadSequenceGenerator;
}

#endif
