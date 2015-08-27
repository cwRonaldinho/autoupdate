#ifndef __INC__ALARMSRV_H
#define __INC__ALARMSRV_H

#include "Service.h"
#include "cacti/mtl/MessageDispatcher.h"

using namespace cacti;

class USService;

class USServer : public Service
{
public:
	USServer():Service("自动更新模块", "AutoUpdate_V1.0.1","自动更新模块")
	{
		m_pAlarmSrvService = NULL;
		m_pMessageDispatcher = NULL;
		m_appport = 0;
	}

	virtual bool start();
	virtual void stop();
	virtual void snapshot();

private:
	void handleUICommand(std::vector<std::string> & vec);		// 处理控制台命令

private:
	MessageDispatcher	*m_pMessageDispatcher;
	USService		*m_pAlarmSrvService;
	int				m_appport;					// 本程序欲模拟的程序对应的appport
};
#endif //__INC__ALARMSRV_H
