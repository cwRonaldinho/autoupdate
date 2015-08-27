/*
	@file	baconst.h
 	@comment
		constant definition in BA System .		
*/
#if		!defined( BACONST_H_00 )
#define	BACONST_H_00	(1)

#define	NS_BEGIN(x)		namespace	x	{
#define	NS_END(x)		};

//	-----------------------------------------------------------------------

NS_BEGIN( cacti )

//
//	bill/charge indicator.
//
enum	_BILLING_IND	{
	BI_NOCHRG			= 0,		//	no charge.
	BI_CHRG				= 1,		//	charge
	BI_RESERVE			= 2,		//	reserve
};

//
//	callee status indicator.
//
enum	_CALLEE_STATUS	{
	CS_NOIND			= 0,		//	no indication.
	CS_FREE				= 1,		//	free
	CS_CONNFREE			= 2,		//	connect when free
	CS_RESERVE			= 3,		//	reserved
};

//
//	call info request type
//
enum	_CALLREQ_TYPE	{
	CRT_REQUEST_CALLER		= 1,	//	request caller.
	CRT_REQUEST_ORIGCALLED	= 2,	//	request original called.
};

//
//	channel operation flag.
//
enum	_LOCAL_REMOTE	{
	LR_LOCAL			= 0,
	LR_REMOTE			= 1,
};

//
//	category of block/unblock
//
enum	_CATBLOCK	{
	CB_MAINT			= 0,	//	maintenance blocking/unblocking.
	CB_HARDWARE			= 1,	//	hardware blocking/unblocking.
};

//
//	channel priority
//
enum	_CHANNEL_PRIORITY	{
	CP_FIRST			= 0,
	CP_SECOND			= 1,
};

enum _GLOBAL_PORT {
	CFGSERVER_PORT		= 7789,
	MONITOR_AGENT_PORT  = 2007,
};

#define LOGGER_MTL_DISPATCHER ("mtl.dispatcher")
#define LOGGER_MTL_SKELETON ("mtl.skeleton")
#define LOGGER_MTL_PEERS ("mtl.peers")
#define LOGGER_MTL_TIMER ("mtl.timer")
#define LOGGER_SOCKET_CLIENT ("socket.client")
#define LOGGER_SOCKET_SERVER ("socket.server")
#define LOGGER_SOCKET_ALARM ("socket.alarm")
#define LOGGER_STARTUP_SYS ("startup.sys")
#define LOGGER_STARTUP_CONSOLE ("startup.console")

NS_END( cacti )

#endif	//	BACONST_H_00
