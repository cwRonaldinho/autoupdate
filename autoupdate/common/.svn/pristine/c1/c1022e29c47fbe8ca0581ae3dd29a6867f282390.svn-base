#pragma once

#include "cacti/socket/SocketWrapper.h"
#include "cacti/message/TransferMessage.h"
#include "cacti/logging/Logger.h"
#include "cacti/logging/ConsoleHandler.h"

using namespace cacti;

#define LOGGERALARMOPR "alarmopr"

class AlarmOperator
{
public:
	AlarmOperator(const char* addr, u_short port);
	~AlarmOperator();

	UserTransferMessagePtr doCommand(UserTransferMessagePtr utm);
	bool doEvent(UserTransferMessagePtr utm);
	bool connect();
	UserTransferMessagePtr AlarmOperator::receive();

private:
	std::string m_address;
	u_short     m_port;
	
	Logger&		m_logger;
	SocketConnector m_connector;
};
