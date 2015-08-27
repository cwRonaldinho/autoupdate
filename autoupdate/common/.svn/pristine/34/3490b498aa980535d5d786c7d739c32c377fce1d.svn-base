#ifndef INCLUDED_MTL_MESSAGETIMER_H
#define INCLUDED_MTL_MESSAGETIMER_H

#pragma once

#include "cacti/util/Timer.h"
#include "cacti/mtl/DispatcherInterface.h"
namespace cacti
{
	class MessageTimer : public ActiveTimer
	{
	public:
		MessageTimer(DispatcherInterface* dispatcher);

		TimerID set(u32 expires, const ServiceIdentifier& sid, UserTransferMessagePtr utm);
		void    cancel(TimerID timerid);

	private:
		DispatcherInterface* m_dispatcher;
	};
};
#endif // INCLUDED_MTL_MESSAGETIMER_H
