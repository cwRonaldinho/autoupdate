#include "stdafx.h"

#include "cacti/mtl/MessageDispatcher.h"
#include "cacti/mtl/MessageTimer.h"

#define new DEBUG_NEW

namespace cacti
{
	class MessageTimerTask : public TimerTask
	{
	public:
		MessageTimerTask(DispatcherInterface* dispatcher, const ServiceIdentifier& sid, UserTransferMessagePtr utm)
			: m_dispatcher(dispatcher)
			, m_req(sid)
			, m_utm(utm)
		{
		}

		void onTimer()
		{
			m_dispatcher->postMessage(m_req, m_req, m_utm, false);
			delete this;
		}

		std::string toString(){ return m_req.toString(); }

		DispatcherInterface*   m_dispatcher;
		ServiceIdentifier      m_req;
		UserTransferMessagePtr m_utm;
	};

	MessageTimer::MessageTimer(DispatcherInterface* dispatcher)
		: m_dispatcher(dispatcher)
	{
	}

	TimerID MessageTimer::set(u32 expires, const ServiceIdentifier& sid, UserTransferMessagePtr utm)
	{
		return ActiveTimer::set(expires, new MessageTimerTask(m_dispatcher, sid, utm));
	}

	void    MessageTimer::cancel(TimerID timerid)
	{
		MessageTimerTask* task = (MessageTimerTask*) ActiveTimer::cancel(timerid);

		delete task;
	}
}
